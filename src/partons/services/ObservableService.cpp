#include "../../../include/partons/services/ObservableService.h"

#include <ElementaryUtils/file_utils/FileUtils.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>

#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/KinematicUtils.h"
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
#include "../../../include/partons/utils/plot2D/Plot2DList.h"

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

//TODO implement all function
//TODO check before executing computeTask if the service name equal current service class name to avoid computing method from another service
void ObservableService::computeTask(Task &task) {
    List<ObservableResult> observableResultList;

    if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            ObservableService::FUNCTION_NAME_COMPUTE_OBSERVABLE)) {
        observableResultList.add(computeObservableTask(task));
    } else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            ObservableService::FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL)) {
        observableResultList = computeManyKinematicOneModelTask(task);
    } else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            ObservableService::FUNCTION_NAME_GENERATE_PLOT_FILE)) {
        generatePlotFileTask(task);
    } else if (!ServiceObjectTyped<ObservableKinematic, ObservableResult>::computeGeneralTask(
            task)) {
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
        Observable* pObservable, const GPDType::Type gpdType) {

    List<ObservableResult> results;

    // TODO voir s'il n'est pas possible de déplacer ça de manière générique dans la classe parent

    List<ElemUtils::Packet> listOfPacket;

    for (unsigned int i = 0; i != listOfKinematic.size(); i++) {
        ElemUtils::Packet packet;
        ObservableKinematic obsK;
        obsK = listOfKinematic[i];
        packet << obsK << GPDType(gpdType);
        listOfPacket.add(packet);
    }

    addTasks(listOfPacket);

    initComputationalThread(pObservable);
    launchAllThreadAndWaitingFor();

// ####################################################

    sortResultList();

    results = getResultList();

    clearResultListBuffer();
    clearKinematicListBuffer();

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

    GPDType::Type gpdType = GPDType::ALL;

    if (task.isAvailableParameters("GPDType")) {
        gpdType =
                static_cast<GPDType::Type>(task.getLastAvailableParameters().get(
                        "type").toUInt());
    }

    Observable* pObservable = newObservableModuleFromTask(task);

    return computeObservable(kinematic, pObservable, gpdType);
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

    GPDType::Type gpdType = GPDType::ALL;

    if (task.isAvailableParameters("GPDType")) {
        gpdType =
                static_cast<GPDType::Type>(task.getLastAvailableParameters().get(
                        "type").toUInt());
    }

    Observable* pObservable = newObservableModuleFromTask(task);

    return computeManyKinematicOneModel(listOfKinematic, pObservable, gpdType);
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
        const GPDType::Type gpdType) const {
    return pObservable->compute(observableKinematic, gpdType);
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
        ProcessModule* pProcessModule,
        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule) const {

    if (pObservable == 0) {
        error(__func__, "You have not provided any Observable");
    }

    configureProcessModule(pProcessModule, pConvolCoeffFunctionModule);

    pObservable->setProcessModule(pProcessModule);

    return pObservable;
}

ProcessModule* ObservableService::configureProcessModule(
        ProcessModule* pProcessModule,
        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule) const {

    if (pProcessModule == 0) {
        error(__func__, "You have not provided any ProcessModule");
    }

    if (pProcessModule->isCCFModuleDependent()) {
        if (pConvolCoeffFunctionModule == 0) {
            error(__func__,
                    "This ProcessModule is ConvolCoeffFunctionModule dependent but you have not provided any ConvolCoeffFunctionModule");
        }

        pProcessModule->setConvolCoeffFunctionModule(
                pConvolCoeffFunctionModule);
    }

    return pProcessModule;
}

void ObservableService::generatePlotFileTask(Task& task) {

    std::string filePath = ElemUtils::StringUtils::EMPTY;

    std::vector<std::string> selectParams;
    std::vector<ElemUtils::GenericType> whereParams;

    if (task.isAvailableParameters("output")) {
        filePath = task.getLastAvailableParameters().get("filePath").toString();
    } else {
        error(__func__,
                "The output-type parameter is missing in the xml file ");
    }
    if (task.isAvailableParameters("select")) {
        selectParams.push_back(
                task.getLastAvailableParameters().get("xPlot").toString());
        selectParams.push_back(
                task.getLastAvailableParameters().get("yPlot").toString());
    } else {
        error(__func__,
                "The select-type parameter is missing in the xml file ");
    }
    if (task.isAvailableParameters("where")) {
        whereParams.push_back(task.getLastAvailableParameters().get("xB"));
        whereParams.push_back(task.getLastAvailableParameters().get("t"));
        whereParams.push_back(task.getLastAvailableParameters().get("Q2"));
        whereParams.push_back(
                task.getLastAvailableParameters().get("computation_id"));
    } else {
        error(__func__, "The where-type parameter is missing in the xml file ");
    }

    generatePlotFile(filePath, selectParams, whereParams);

}

void ObservableService::generatePlotFile(const std::string& filePath,
        std::vector<std::string>& selectParams,
        std::vector<ElemUtils::GenericType>& whereParams) const {

    ObservableResultDaoService observableResultDaoService;

    ElemUtils::Formatter formatter;

    formatter << "SELECT ";

    for (unsigned int i = 0; i != selectParams.size(); i++) {
        formatter << selectParams[i];
        if (i + 1 < selectParams.size()) {
            formatter << ", ";
        }
    }

    formatter << " FROM observable_plot_2d_view WHERE xB = "
            << whereParams[0].toDouble() << " AND t = "
            << whereParams[1].toDouble() << " AND Q2 = "
            << whereParams[2].toDouble() << " AND computation_id = "
            << whereParams[3].toInt();

    debug(__func__, formatter.str());

    Plot2DList plot2DList =
            observableResultDaoService.getPlot2DListFromCustomQuery(
                    formatter.str());

    if (plot2DList.isEmpty()) {
        warn(__func__,
                "There is no coincidence all over the database with the given parameters");
    } else {
        info(__func__,
                ElemUtils::Formatter() << plot2DList.size()
                        << " data in coincidence with the given parameters have been found in the database and written in the file : "
                        << filePath);

    }

    ElemUtils::FileUtils::writeLine(filePath, plot2DList.toStringPlotFile(' '));
}
