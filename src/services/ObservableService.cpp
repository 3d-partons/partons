#include "ObservableService.h"

#include "../beans/automation/Task.h"
#include "../beans/KinematicUtils.h"
#include "../BaseObjectRegistry.h"
#include "../database/observable/service/ObservableResultDaoService.h"
#include "../modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../modules/GPDModule.h"
#include "../modules/observable/Observable.h"
#include "../modules/process/DVCSModule.h"
#include "../modules/scale/ScaleModule.h"
#include "../modules/xb_to_xi/XiConverterModule.h"
#include "../ModuleObjectFactory.h"
#include "../ObservableObjectFactory.h"
#include "../utils/GenericType.h"
#include "../utils/ParameterList.h"
#include "../utils/stringUtils/Formatter.h"
#include "../utils/stringUtils/StringUtils.h"
#include "../utils/test/report/observable/ObservableResultListReport.h"

const std::string ObservableService::FUNCTION_NAME_COMPUTE_DVCS_OBSERVABLE =
        "computeDVCSObservable";

const std::string ObservableService::FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL =
        "computeManyKinematicOneModel";

// Initialise [class]::classId with a unique name.
const unsigned int ObservableService::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new ObservableService("ObservableService"));

ObservableService::ObservableService(const std::string &className) :
        ServiceObject(className) {

}

ObservableService::~ObservableService() {
}

//TODO implement all function
void ObservableService::computeTask(Task &task) {

    ResultList<ObservableResult> observableResultList;

    if (StringUtils::equals(task.getFunctionName(),
            ObservableService::FUNCTION_NAME_COMPUTE_DVCS_OBSERVABLE)) {
        observableResultList.add(computeDVCSObservableTask(task));
    } else if (StringUtils::equals(task.getFunctionName(),
            ObservableService::FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL)) {
        observableResultList = computeManyKinematicOneModelTask(task);
    } else {
        throwException(__func__,
                "unknown function name = " + task.getFunctionName());
    }

    if (task.isStoreInDB()) {
        ObservableResultDaoService observableResultDaoService;
        int computationId = observableResultDaoService.insert(
                observableResultList);

        if (computationId != -1) {
            info(__func__,
                    Formatter()
                            << "ObservableResultList object has been stored in database with computation_id = "
                            << computationId);
        } else {
            error(__func__,
                    Formatter()
                            << "ObservableResultList object : insertion into database failed");
        }
    }
}

ObservableResult ObservableService::computeDVCSObservable(
        DVCSModule* pDVCSModule, Observable* pObservable,
        const ObservableKinematic &observableKinematic,
        DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule) const {

    pDVCSModule->setDVCSConvolCoeffFunctionModule(
            pDVCSConvolCoeffFunctionModule);
    pObservable->setDVCSModule(pDVCSModule);

    return pObservable->compute(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2(),
            observableKinematic.getPhi());
}

ResultList<ObservableResult> ObservableService::computeManyKinematicOneModel(
        const List<ObservableKinematic> & listOfKinematic,
        DVCSModule* pDVCSModule, Observable* pObservable,
        DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule) const {

    ResultList<ObservableResult> results;

    for (unsigned int i = 0; i != listOfKinematic.size(); i++) {
        results.add(
                computeDVCSObservable(pDVCSModule, pObservable,
                        listOfKinematic[i], pDVCSConvolCoeffFunctionModule));
    }

    return results;
}

ObservableResult ObservableService::computeDVCSObservableTask(Task& task) {

    // create ScaleModule
    ScaleModule* pScaleModule = 0;

    if (task.isAvailableParameterList("ScaleModule")) {
        pScaleModule = ModuleObjectFactory::newScaleModule(
                task.getLastAvailableParameterList().get("id").toString());
        pScaleModule->configure(task.getLastAvailableParameterList());
    } else {
        throwException(__func__,
                Formatter() << "Missing object : <ScaleModule> for method "
                        << task.getFunctionName());
    }

    // create XiConverterModule
    XiConverterModule* pXiConverterModule = 0;

    if (task.isAvailableParameterList("XiConverterModule")) {
        pXiConverterModule = ModuleObjectFactory::newXiConverterModule(
                task.getLastAvailableParameterList().get("id").toString());
        pXiConverterModule->configure(task.getLastAvailableParameterList());
    } else {
        throwException(__func__,
                Formatter()
                        << "Missing object : <XiConverterModule> for method "
                        << task.getFunctionName());
    }

    //create a GPDKinematic and init it with a list of parameters
    ObservableKinematic kinematic;

    if (task.isAvailableParameterList("ObservableKinematic")) {
        kinematic = ObservableKinematic(task.getLastAvailableParameterList());
    } else {
        throwException(__func__,
                Formatter()
                        << "Missing object : <ObservableKinematic> for method "
                        << task.getFunctionName());
    }

    Observable* pObservable = 0;

    if (task.isAvailableParameterList("Observable")) {
        pObservable = ObservableObjectFactory::newObservable(
                task.getLastAvailableParameterList().get("id").toString());
        pObservable->configure(task.getLastAvailableParameterList());
    } else {
        throwException(__func__,
                Formatter() << "Missing object : <Observable> for method "
                        << task.getFunctionName());
    }

    GPDModule* pGPDModule = 0;

    if (task.isAvailableParameterList("GPDModule")) {
        pGPDModule = ModuleObjectFactory::newGPDModule(
                task.getLastAvailableParameterList().get("id").toString());
        pGPDModule->configure(task.getLastAvailableParameterList());
    } else {
        throwException(__func__,
                Formatter() << "Missing object : <GPDModule> for method "
                        << task.getFunctionName());
    }

    DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule = 0;

    if (task.isAvailableParameterList("DVCSConvolCoeffFunctionModule")) {
        pDVCSConvolCoeffFunctionModule =
                ModuleObjectFactory::newDVCSConvolCoeffFunctionModule(
                        task.getLastAvailableParameterList().get("id").toString());
        pDVCSConvolCoeffFunctionModule->configure(
                task.getLastAvailableParameterList());
    } else {
        throwException(__func__,
                Formatter()
                        << "Missing object : <GPDEvolutionModule> for method "
                        << task.getFunctionName());
    }

    DVCSModule* pDVCSModule = 0;

    if (task.isAvailableParameterList("DVCSModule")) {
        pDVCSModule = ModuleObjectFactory::newDVCSModule(
                task.getLastAvailableParameterList().get("id").toString());
        pDVCSModule->configure(task.getLastAvailableParameterList());
    } else {
        throwException(__func__,
                Formatter() << "Missing object : <DVCSModule> for method "
                        << task.getFunctionName());
    }

    //TODO how to remove it and autoconfigure ?
    pDVCSConvolCoeffFunctionModule->setGPDModule(pGPDModule);
    pDVCSModule->setPScaleModule(pScaleModule);
    pDVCSModule->setPXiConverterModule(pXiConverterModule);

    ObservableResult result = computeDVCSObservable(pDVCSModule, pObservable,
            kinematic, pDVCSConvolCoeffFunctionModule);

    info(__func__,
            Formatter() << task.getFunctionName() << "("
                    << pObservable->getClassName() << ")" << '\n'
                    << result.toString());

    return result;
}

ResultList<ObservableResult> ObservableService::computeManyKinematicOneModelTask(
        Task& task) {

    // create ScaleModule
    ScaleModule* pScaleModule = 0;

    if (task.isAvailableParameterList("ScaleModule")) {
        pScaleModule = ModuleObjectFactory::newScaleModule(
                task.getLastAvailableParameterList().get("id").toString());
        pScaleModule->configure(task.getLastAvailableParameterList());
    } else {
        throwException(__func__,
                Formatter() << "Missing object : <ScaleModule> for method "
                        << task.getFunctionName());
    }

    // create XiConverterModule
    XiConverterModule* pXiConverterModule = 0;

    if (task.isAvailableParameterList("XiConverterModule")) {
        pXiConverterModule = ModuleObjectFactory::newXiConverterModule(
                task.getLastAvailableParameterList().get("id").toString());
        pXiConverterModule->configure(task.getLastAvailableParameterList());
    } else {
        throwException(__func__,
                Formatter()
                        << "Missing object : <XiConverterModule> for method "
                        << task.getFunctionName());
    }

    List<ObservableKinematic> listOfKinematic;

    if (task.isAvailableParameterList("ObservableKinematic")) {
        ParameterList parameterList = task.getLastAvailableParameterList();
        if (parameterList.isAvailable("file")) {
            listOfKinematic = KinematicUtils::getObservableKinematicFromFile(
                    parameterList.getLastAvailable().toString());
        } else {
            throwException(__func__,
                    Formatter()
                            << "Missing parameter file in object <ObservableKinematic> for method "
                            << task.getFunctionName());
        }

    } else {
        throwException(__func__,
                Formatter()
                        << "Missing object : <ObservableKinematic> for method "
                        << task.getFunctionName());
    }

    Observable* pObservable = 0;

    if (task.isAvailableParameterList("Observable")) {
        pObservable = ObservableObjectFactory::newObservable(
                task.getLastAvailableParameterList().get("id").toString());
        pObservable->configure(task.getLastAvailableParameterList());
    } else {
        throwException(__func__,
                Formatter() << "Missing object : <Observable> for method "
                        << task.getFunctionName());
    }

    GPDModule* pGPDModule = 0;

    if (task.isAvailableParameterList("GPDModule")) {
        pGPDModule = ModuleObjectFactory::newGPDModule(
                task.getLastAvailableParameterList().get("id").toString());
        pGPDModule->configure(task.getLastAvailableParameterList());
    } else {
        throwException(__func__,
                Formatter() << "Missing object : <GPDModule> for method "
                        << task.getFunctionName());
    }

    DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule = 0;

    if (task.isAvailableParameterList("DVCSConvolCoeffFunctionModule")) {
        pDVCSConvolCoeffFunctionModule =
                ModuleObjectFactory::newDVCSConvolCoeffFunctionModule(
                        task.getLastAvailableParameterList().get("id").toString());
        pDVCSConvolCoeffFunctionModule->configure(
                task.getLastAvailableParameterList());
    } else {
        throwException(__func__,
                Formatter()
                        << "Missing object : <GPDEvolutionModule> for method "
                        << task.getFunctionName());
    }

    DVCSModule* pDVCSModule = 0;

    if (task.isAvailableParameterList("DVCSModule")) {
        pDVCSModule = ModuleObjectFactory::newDVCSModule(
                task.getLastAvailableParameterList().get("id").toString());
        pDVCSModule->configure(task.getLastAvailableParameterList());
    } else {
        throwException(__func__,
                Formatter() << "Missing object : <DVCSModule> for method "
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
            Formatter() << task.getFunctionName() << "("
                    << pObservable->getClassName() << ")" << '\n'
                    << result.toString());

    return result;
}

ObservableResultListReport ObservableService::compareResultList(
        const ResultList<ObservableResult>& resultList_01,
        const ResultList<ObservableResult>& resultList_02,
        const Tolerances &tolerances) const {

    //TODO implement
    return ObservableResultListReport();
}
