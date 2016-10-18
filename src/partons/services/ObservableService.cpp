#include "../../../include/partons/services/ObservableService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>

#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/KinematicUtils.h"
#include "../../../include/partons/beans/system/ResultInfo.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/database/observable/service/ObservableResultDaoService.h"
#include "../../../include/partons/modules/observable/Observable.h"
#include "../../../include/partons/modules/process/DVCSModule.h"
#include "../../../include/partons/modules/scale/ScaleModule.h"
#include "../../../include/partons/modules/xb_to_xi/XiConverterModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"
#include "../../../include/partons/services/ConvolCoeffFunctionService.h"
#include "../../../include/partons/ServiceObjectRegistry.h"
#include "../../../include/partons/utils/exceptions/CCFModuleNullPointerException.h"

const std::string ObservableService::FUNCTION_NAME_COMPUTE_OBSERVABLE =
        "computeObservable";

const std::string ObservableService::FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL =
        "computeManyKinematicOneModel";

const std::string ObservableService::FUNCTION_NAME_GENERATE_PLOT_FILE =
        "generatePlotFile";

// Initialise [class]::classId with a unique name.
const unsigned int ObservableService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new ObservableService("ObservableService"));

ObservableService::ObservableService(const std::string &className) :
        ServiceObjectTyped<ObservableKinematic, ObservableResult>(className) {
}

ObservableService::~ObservableService() {
}

void ObservableService::resolveObjectDependencies() {
    ServiceObject::resolveObjectDependencies();

    try {
        m_batchSize = ElemUtils::GenericType(
                ElemUtils::PropertiesManager::getInstance()->getString(
                        "observable.service.batch.size")).toUInt();
    } catch (const std::exception &e) {
        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }
}

//TODO implement all function
//TODO check before executing computeTask if the service name equal current service class name to avoid computing method from another service
void ObservableService::computeTask(Task &task) {
    ServiceObjectTyped<ObservableKinematic, ObservableResult>::computeTask(
            task);

    List<ObservableResult> resultList;

    if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            ObservableService::FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL)) {
        resultList = computeManyKinematicOneModelTask(task);
    } else {

        if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                ObservableService::FUNCTION_NAME_COMPUTE_OBSERVABLE)) {
            resultList.add(computeObservableTask(task));
        } else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                ObservableService::FUNCTION_NAME_GENERATE_PLOT_FILE)) {
            generatePlotFileTask(task);
        } else if (!ServiceObjectTyped<ObservableKinematic, ObservableResult>::computeGeneralTask(
                task)) {
            errorUnknownMethod(task);
        }

        if (task.isStoreInDB()) {
            ObservableResultDaoService observableResultDaoService;
            int computationId = observableResultDaoService.insert(resultList);

            if (computationId != -1) {
                info(__func__,
                        ElemUtils::Formatter()
                                << "ObservableResultList object has been stored in database with computation_id = "
                                << computationId);
            } else {
                throw ElemUtils::CustomException(getClassName(), __func__,
                        "ObservableResultList object : insertion into database failed");
            }
        }
    }

    m_resultListBuffer = resultList;
}

List<ObservableResult> ObservableService::computeManyKinematicOneModel(
        const List<ObservableKinematic> & listOfKinematic,
        Observable* pObservable, const List<GPDType> & listOfGPDType,
        const bool storeInDB) {

    // TODO voir s'il n'est pas possible de déplacer ça de manière générique dans la classe parent

    info(__func__,
            ElemUtils::Formatter() << listOfKinematic.size()
                    << " will be computed");

    List<ObservableResult> results;
    List<ElemUtils::Packet> listOfPacket;
    List<GPDType> finalListOfGPDType = listOfGPDType;

    initComputationalThread(pObservable);

    // ##### Batch feature start section #####
    unsigned int i = 0;
    unsigned int j = 0;

    while (i != listOfKinematic.size()) {
        listOfPacket.clear();
        j = 0;

        while ((j != m_batchSize) && (i != listOfKinematic.size())) {
            ElemUtils::Packet packet;
            ObservableKinematic kinematic;
            kinematic = listOfKinematic[i];
            packet << kinematic << finalListOfGPDType;
            listOfPacket.add(packet);
            i++;
            j++;
        }

        addTasks(listOfPacket);
        launchAllThreadAndWaitingFor();
        sortResultList();

        updateResultInfo(getResultList(), m_resultInfo);

        if (storeInDB) {
            ObservableResultDaoService resultDaoService;
            resultDaoService.insert(getResultList());
        } else {
            results.add(getResultList());
        }

        clearResultListBuffer();
    }
    // ##### Batch feature end section #####

    clearAllThread();

    return results;
}

ObservableResult ObservableService::computeObservableTask(Task& task) {

    //create a Kinematic and init it with a list of parameters
    ObservableKinematic kinematic;

    if (task.isAvailableParameters("ObservableKinematic")) {
        kinematic = ObservableKinematic(task.getLastAvailableParameters());
    } else {
        ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <ObservableKinematic> for method "
                        << task.getFunctionName());
    }

    List<GPDType> listOfGPDType;
    try {
        listOfGPDType = getGPDTypeListFromTask(task);
    } catch (const ElemUtils::CustomException &e) {
        //Nothing to do
    }

    Observable* pObservable = newObservableModuleFromTask(task);

    return computeObservable(kinematic, pObservable, listOfGPDType);
}

List<ObservableResult> ObservableService::computeManyKinematicOneModelTask(
        Task& task) {

    List<ObservableKinematic> listOfKinematic;

    if (task.isAvailableParameters("ObservableKinematic")) {
        ElemUtils::Parameters parameters = task.getLastAvailableParameters();
        if (parameters.isAvailable("file")) {
            listOfKinematic = KinematicUtils().getObservableKinematicFromFile(
                    parameters.getLastAvailable().toString());
        } else {
            ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Missing parameter file in object <ObservableKinematic> for method "
                            << task.getFunctionName());
        }

    } else {
        ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <ObservableKinematic> for method "
                        << task.getFunctionName());
    }

    List<GPDType> listOfGPDType;
    try {
        listOfGPDType = getGPDTypeListFromTask(task);
    } catch (const ElemUtils::CustomException &e) {
        //Nothing to do
    }

    Observable* pObservable = newObservableModuleFromTask(task);

    return computeManyKinematicOneModel(listOfKinematic, pObservable,
            listOfGPDType, task.isStoreInDB());
}

ObservableChannel::Type ObservableService::getObservableChannel(
        const std::string& observableClassName) const {
    BaseObjectRegistry* pBaseObjectRegistry =
            Partons::getInstance()->getBaseObjectRegistry();

    BaseObject* pTempBaseObject = pBaseObjectRegistry->get(observableClassName);
    Observable* pTempObservable = static_cast<Observable*>(pTempBaseObject);

    return pTempObservable->getChannel();
}

ObservableResult ObservableService::computeObservable(
        const ObservableKinematic& observableKinematic, Observable* pObservable,
        const List<GPDType> & listOfGPDType) const {
    return pObservable->compute(observableKinematic, listOfGPDType);
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
        ElemUtils::CustomException(getClassName(), __func__,
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
        ElemUtils::CustomException(getClassName(), __func__,
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
        ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <Observable> for method "
                        << task.getFunctionName());
    }

    ProcessModule* pProcessModule = newProcessModuleFromTask(task);

    //TODO how to remove it and autoconfigure ?
    pProcessModule->setPScaleModule(pScaleModule);
    pProcessModule->setPXiConverterModule(pXiConverterModule);

    return configureObservable(pObservable, pProcessModule);
}

ProcessModule* ObservableService::newProcessModuleFromTask(
        const Task& task) const {
    DVCSModule* pDVCSModule = 0;

    if (task.isAvailableParameters("DVCSModule")) {
        pDVCSModule =
                m_pModuleObjectFactory->newDVCSModule(
                        task.getLastAvailableParameters().get(
                                ModuleObject::CLASS_NAME).toString());
        pDVCSModule->configure(task.getLastAvailableParameters());
    }

    ConvolCoeffFunctionModule* pConvolCoeffFunctionModule = 0;

    try {
        pConvolCoeffFunctionModule =
                Partons::getInstance()->getServiceObjectRegistry()->getConvolCoeffFunctionService()->newConvolCoeffFunctionModuleFromTask(
                        task);
    } catch (const CCFModuleNullPointerException &e) {
        // Nothing to do.
        // An exception is raised if <ConvolCoeffFunctionModule> element cannot be found in the task parameterList, but in this case a ProcessModule can be ConvolCoeffFunctionModule independent.
        // So just catch the exception and continue to run the program.
    }

    return configureProcessModule(pDVCSModule, pConvolCoeffFunctionModule);
}

Observable* ObservableService::configureObservable(Observable* pObservable,
        ProcessModule* pProcessModule) const {

    if (pObservable == 0) {
        ElemUtils::CustomException(getClassName(), __func__,
                "You have not provided any Observable");
    }

    pObservable->setProcessModule(pProcessModule);

    return pObservable;
}

ProcessModule* ObservableService::configureProcessModule(
        ProcessModule* pProcessModule,
        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule) const {

    if (pProcessModule == 0) {
        ElemUtils::CustomException(getClassName(), __func__,
                "You have not provided any ProcessModule");
    }

    if (pProcessModule->isCCFModuleDependent()) {
        if (pConvolCoeffFunctionModule == 0) {
            ElemUtils::CustomException(getClassName(), __func__,
                    "This ProcessModule is ConvolCoeffFunctionModule dependent but you have not provided any ConvolCoeffFunctionModule");
        }

        pProcessModule->setConvolCoeffFunctionModule(
                pConvolCoeffFunctionModule);
    }

    return pProcessModule;
}

void ObservableService::generatePlotFileTask(Task& task) {
    generatePlotFile(getOutputFilePathForPlotFileTask(task),
            generateSQLQueryForPlotFileTask(task, "observable_plot_2d_view"),
            ' ');
}
