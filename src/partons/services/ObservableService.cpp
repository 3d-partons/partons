#include "../../../include/partons/services/ObservableService.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>

#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/KinematicUtils.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/database/observable/service/ObservableResultDaoService.h"
#include "../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../../include/partons/modules/GPDModule.h"
#include "../../../include/partons/modules/observable/Observable.h"
#include "../../../include/partons/modules/process/DVCSModule.h"
#include "../../../include/partons/modules/scale/ScaleModule.h"
#include "../../../include/partons/modules/xb_to_xi/XiConverterModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"
#include "../../../include/partons/services/ConvolCoeffFunctionService.h"
#include "../../../include/partons/ServiceObjectRegistry.h"

const std::string ObservableService::FUNCTION_NAME_COMPUTE_DVCS_OBSERVABLE =
        "computeDVCSObservable";

const std::string ObservableService::FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL =
        "computeManyKinematicOneModel";

// Initialise [class]::classId with a unique name.
const unsigned int ObservableService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new ObservableService("ObservableService"));

ObservableService::ObservableService(const std::string &className) :
        ServiceObjectTyped(className) {
}

ObservableService::~ObservableService() {
}

//TODO implement all function
//TODO check before executing computeTask if the service name equal current service class name to avoid computing method from another service
void ObservableService::computeTask(Task &task) {

    List<ObservableResult> observableResultList;

    if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            ObservableService::FUNCTION_NAME_COMPUTE_DVCS_OBSERVABLE)) {
        observableResultList.add(computeObservableTask(task));
    } else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            ObservableService::FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL)) {
        observableResultList = computeManyKinematicOneModelTask(task);
    } else {
        error(__func__, "unknown function name = " + task.getFunctionName());
    }

    if (task.isStoreInDB()) {
        ObservableResultDaoService observableResultDaoService;
        int computationId = observableResultDaoService.insert(
                observableResultList);

        if (computationId != -1) {
            info(__func__,
                    ElemUtils::Formatter()
                            << "ObservableResultList object has been stored in database with computation_id = "
                            << computationId);
        } else {
            error(__func__,
                    ElemUtils::Formatter()
                            << "ObservableResultList object : insertion into database failed");
        }
    }

    add(observableResultList);
}

List<ObservableResult> ObservableService::computeManyKinematicOneModel(
        const List<ObservableKinematic> & listOfKinematic,
        Observable* pObservable) {

    List<ObservableResult> results;

    // TODO voir s'il n'est pas possible de déplacer ça de manière générique dans la classe parent

    List<ElemUtils::Packet> listOfPacket;

    for (unsigned int i = 0; i != listOfKinematic.size(); i++) {
        ElemUtils::Packet packet;
        ObservableKinematic obsK;
        obsK = listOfKinematic[i];
        packet << obsK;
        listOfPacket.add(packet);
    }

    addTasks(listOfPacket);

    initComputationalThread(pObservable);
    launchAllThreadAndWaitingFor();

    // ####################################################

    results = getResultList();
    clearResultListBuffer();

    return results;
}

ObservableResult ObservableService::computeObservableTask(Task& task) {

    //create a Kinematic and init it with a list of parameters
    ObservableKinematic kinematic;

    if (task.isAvailableParameters("ObservableKinematic")) {
        kinematic = ObservableKinematic(task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <ObservableKinematic> for method "
                        << task.getFunctionName());
    }

    Observable* pObservable = newObservableModuleFromTask(task);

    ObservableResult result = computeObservable(kinematic, pObservable);

    info(__func__,
            ElemUtils::Formatter() << task.getFunctionName() << "("
                    << pObservable->getClassName() << ")" << '\n'
                    << result.toString());

    return result;
}

List<ObservableResult> ObservableService::computeManyKinematicOneModelTask(
        Task& task) {

    List<ObservableKinematic> listOfKinematic;

    if (task.isAvailableParameters("ObservableKinematic")) {
        ElemUtils::Parameters parameters = task.getLastAvailableParameters();
        if (parameters.isAvailable("file")) {
            listOfKinematic = KinematicUtils::getObservableKinematicFromFile(
                    parameters.getLastAvailable().toString());
        } else {
            error(__func__,
                    ElemUtils::Formatter()
                            << "Missing parameter file in object <ObservableKinematic> for method "
                            << task.getFunctionName());
        }

    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <ObservableKinematic> for method "
                        << task.getFunctionName());
    }

    Observable* pObservable = newObservableModuleFromTask(task);

    List<ObservableResult> result = computeManyKinematicOneModel(
            listOfKinematic, pObservable);

    info(__func__,
            ElemUtils::Formatter() << task.getFunctionName() << "("
                    << pObservable->getClassName() << ")" << '\n'
                    << result.toString());

    return result;
}

ObservableChannel::Type ObservableService::getObservableChannel(
        const std::string& observableClassName) const {
    BaseObjectRegistry* pBaseObjectRegistry =
            Partons::getInstance()->getBaseObjectRegistry();

    BaseObject* pTempBaseObject = pBaseObjectRegistry->get(observableClassName);
    Observable* pTempObservable = static_cast<Observable*>(pTempBaseObject);

    return pTempObservable->getChannel();
}

//TODO pour les listes au-dessus utiliser cette fonctionnalité pour ne pas dupliquer les implémentations
//TODO refactoring string exception, wrong xml element name
Observable* ObservableService::newObservableModuleFromTask(
        const Task& task) const {
    // create ScaleModule
    ScaleModule* pScaleModule = 0;

    if (task.isAvailableParameters("ScaleModule")) {
        pScaleModule =
                m_pModuleObjectFactory->newScaleModule(
                        task.getLastAvailableParameters().get(
                                ModuleObject::CLASS_NAME).toString());
        pScaleModule->configure(task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <ScaleModule> for method "
                        << task.getFunctionName());
    }

    // create XiConverterModule
    XiConverterModule* pXiConverterModule = 0;

    if (task.isAvailableParameters("XiConverterModule")) {
        pXiConverterModule =
                m_pModuleObjectFactory->newXiConverterModule(
                        task.getLastAvailableParameters().get(
                                ModuleObject::CLASS_NAME).toString());
        pXiConverterModule->configure(task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <XiConverterModule> for method "
                        << task.getFunctionName());
    }

    Observable* pObservable = 0;

    if (task.isAvailableParameters("Observable")) {
        pObservable =
                m_pModuleObjectFactory->newObservable(
                        task.getLastAvailableParameters().get(
                                ModuleObject::CLASS_NAME).toString());
        pObservable->configure(task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <Observable> for method "
                        << task.getFunctionName());
    }

    ProcessModule* pProcessModule = newProcessModuleFromTask(task);

    //TODO how to remove it and autoconfigure ?
    pProcessModule->setPScaleModule(pScaleModule);
    pProcessModule->setPXiConverterModule(pXiConverterModule);

    pObservable->setProcessModule(pProcessModule);

    return pObservable;
}

ObservableResult ObservableService::computeObservable(
        const ObservableKinematic& observableKinematic,
        Observable* pObservable) const {
    return pObservable->compute(observableKinematic);
}

//TODO how to use it ?
Observable* ObservableService::configureObservable(Observable* pObservable,
        ProcessModule* pProcessModule,
        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule,
        GPDModule* pGPDModule) const {

    //TODO add test that check if there is a missing model for a specific Module

    if (!pProcessModule) {
        error(__func__,
                "pProcessModule is NULL pointer ; cannot configure Observable");
    } else {
        //TODO add setter for ConvolCoeffFunction
        //pProcessModule
    }

    if (!pObservable) {
        error(__func__,
                "pObservable is NULL pointer ; cannot configure Observable");
    }

    pProcessModule->setConvolCoeffFunctionModule(
            Partons::getInstance()->getServiceObjectRegistry()->getConvolCoeffFunctionService()->configureConvolCoeffFunctionModule(
                    pConvolCoeffFunctionModule, pGPDModule));

    pObservable->setProcessModule(pProcessModule);

    return pObservable;
}

ProcessModule* ObservableService::newProcessModuleFromTask(
        const Task& task) const {
    GPDModule* pGPDModule = 0;

    if (task.isAvailableParameters("GPDModule")) {
        pGPDModule =
                m_pModuleObjectFactory->newGPDModule(
                        task.getLastAvailableParameters().get(
                                ModuleObject::CLASS_NAME).toString());
        pGPDModule->configure(task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <GPDModule> for method "
                        << task.getFunctionName());
    }

    DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule = 0;

    if (task.isAvailableParameters("DVCSConvolCoeffFunctionModule")) {
        pDVCSConvolCoeffFunctionModule =
                m_pModuleObjectFactory->newDVCSConvolCoeffFunctionModule(
                        task.getLastAvailableParameters().get(
                                ModuleObject::CLASS_NAME).toString());
        pDVCSConvolCoeffFunctionModule->configure(
                task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <GPDEvolutionModule> for method "
                        << task.getFunctionName());
    }

    DVCSModule* pDVCSModule = 0;

    if (task.isAvailableParameters("DVCSModule")) {
        pDVCSModule =
                m_pModuleObjectFactory->newDVCSModule(
                        task.getLastAvailableParameters().get(
                                ModuleObject::CLASS_NAME).toString());
        pDVCSModule->configure(task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <DVCSModule> for method "
                        << task.getFunctionName());
    }

    //TODO how to remove it and autoconfigure ?
    pDVCSModule->setConvolCoeffFunctionModule(
            Partons::getInstance()->getServiceObjectRegistry()->getConvolCoeffFunctionService()->configureConvolCoeffFunctionModule(
                    pDVCSConvolCoeffFunctionModule, pGPDModule));

    return pDVCSModule;
}
