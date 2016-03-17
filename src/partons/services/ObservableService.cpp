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

    ResultList<ObservableResult> observableResultList;

    if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            ObservableService::FUNCTION_NAME_COMPUTE_DVCS_OBSERVABLE)) {
        observableResultList.add(computeDVCSObservableTask(task));
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

ObservableResult ObservableService::computeDVCSObservable(
        DVCSModule* pDVCSModule, Observable* pObservable,
        const ObservableKinematic &observableKinematic,
        DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule) const {

    pDVCSModule->setDVCSConvolCoeffFunctionModule(
            pDVCSConvolCoeffFunctionModule);
    pObservable->setDVCSModule(pDVCSModule);

    return pObservable->compute(observableKinematic);
}

ResultList<ObservableResult> ObservableService::computeManyKinematicOneModel(
        const List<ObservableKinematic> & listOfKinematic,
        DVCSModule* pDVCSModule, Observable* pObservable,
        DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule) {

    ResultList<ObservableResult> results;

    pDVCSModule->setDVCSConvolCoeffFunctionModule(
            pDVCSConvolCoeffFunctionModule);
    pObservable->setDVCSModule(pDVCSModule);

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

ObservableResult ObservableService::computeDVCSObservableTask(Task& task) {

    // create ScaleModule
    ScaleModule* pScaleModule = 0;

    if (task.isAvailableParameters("ScaleModule")) {
        pScaleModule = m_pModuleObjectFactory->newScaleModule(
                task.getLastAvailableParameters().get("id").toString());
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
        pXiConverterModule = m_pModuleObjectFactory->newXiConverterModule(
                task.getLastAvailableParameters().get("id").toString());
        pXiConverterModule->configure(task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <XiConverterModule> for method "
                        << task.getFunctionName());
    }

    //create a GPDKinematic and init it with a list of parameters
    ObservableKinematic kinematic;

    if (task.isAvailableParameters("ObservableKinematic")) {
        kinematic = ObservableKinematic(task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <ObservableKinematic> for method "
                        << task.getFunctionName());
    }

    Observable* pObservable = 0;

    if (task.isAvailableParameters("Observable")) {
        pObservable = m_pModuleObjectFactory->newObservable(
                task.getLastAvailableParameters().get("id").toString());
        pObservable->configure(task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <Observable> for method "
                        << task.getFunctionName());
    }

    GPDModule* pGPDModule = 0;

    if (task.isAvailableParameters("GPDModule")) {
        pGPDModule = m_pModuleObjectFactory->newGPDModule(
                task.getLastAvailableParameters().get("id").toString());
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
                        task.getLastAvailableParameters().get("id").toString());
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
        pDVCSModule = m_pModuleObjectFactory->newDVCSModule(
                task.getLastAvailableParameters().get("id").toString());
        pDVCSModule->configure(task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <DVCSModule> for method "
                        << task.getFunctionName());
    }

    //TODO how to remove it and autoconfigure ?
    pDVCSConvolCoeffFunctionModule->setGPDModule(pGPDModule);
    pDVCSModule->setPScaleModule(pScaleModule);
    pDVCSModule->setPXiConverterModule(pXiConverterModule);

    ObservableResult result = computeDVCSObservable(pDVCSModule, pObservable,
            kinematic, pDVCSConvolCoeffFunctionModule);

    info(__func__,
            ElemUtils::Formatter() << task.getFunctionName() << "("
                    << pObservable->getClassName() << ")" << '\n'
                    << result.toString());

    return result;
}

ResultList<ObservableResult> ObservableService::computeManyKinematicOneModelTask(
        Task& task) {

    // create ScaleModule
    ScaleModule* pScaleModule = 0;

    if (task.isAvailableParameters("ScaleModule")) {
        pScaleModule = m_pModuleObjectFactory->newScaleModule(
                task.getLastAvailableParameters().get("id").toString());
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
        pXiConverterModule = m_pModuleObjectFactory->newXiConverterModule(
                task.getLastAvailableParameters().get("id").toString());
        pXiConverterModule->configure(task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <XiConverterModule> for method "
                        << task.getFunctionName());
    }

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

    Observable* pObservable = 0;

    if (task.isAvailableParameters("Observable")) {
        pObservable = m_pModuleObjectFactory->newObservable(
                task.getLastAvailableParameters().get("id").toString());
        pObservable->configure(task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <Observable> for method "
                        << task.getFunctionName());
    }

    GPDModule* pGPDModule = 0;

    if (task.isAvailableParameters("GPDModule")) {
        pGPDModule = m_pModuleObjectFactory->newGPDModule(
                task.getLastAvailableParameters().get("id").toString());
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
                        task.getLastAvailableParameters().get("id").toString());
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
        pDVCSModule = m_pModuleObjectFactory->newDVCSModule(
                task.getLastAvailableParameters().get("id").toString());
        pDVCSModule->configure(task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <DVCSModule> for method "
                        << task.getFunctionName());
    }

    //TODO how to remove it and autoconfigure ?
    pDVCSConvolCoeffFunctionModule->setGPDModule(pGPDModule);
    pDVCSModule->setPScaleModule(pScaleModule);
    pDVCSModule->setPXiConverterModule(pXiConverterModule);

    ResultList<ObservableResult> result = computeManyKinematicOneModel(
            listOfKinematic, pDVCSModule, pObservable,
            pDVCSConvolCoeffFunctionModule);

    info(__func__,
            ElemUtils::Formatter() << task.getFunctionName() << "("
                    << pObservable->getClassName() << ")" << '\n'
                    << result.toString());

    return result;
}
