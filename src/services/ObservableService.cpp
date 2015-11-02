#include "ObservableService.h"

#include <stddef.h>

#include "../beans/automation/Task.h"
#include "../beans/observable/ObservableResultList.h"
#include "../BaseObjectRegistry.h"
#include "../modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../modules/GPDModule.h"
#include "../modules/observable/Observable.h"
#include "../modules/process/DVCSModule.h"
#include "../modules/scale/ScaleModule.h"
#include "../modules/xb_to_xi/XiConverterModule.h"
#include "../ModuleObjectFactory.h"
#include "../ObservableObjectFactory.h"
#include "../utils/fileUtils/FileUtils.h"
#include "../utils/GenericType.h"
#include "../utils/ParameterList.h"
#include "../utils/stringUtils/Formatter.h"
#include "../utils/stringUtils/StringUtils.h"

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
    if (StringUtils::equals(task.getFunctionName(),
            ObservableService::FUNCTION_NAME_COMPUTE_DVCS_OBSERVABLE)) {
        computeDVCSObservableTask(task);
    } else if (StringUtils::equals(task.getFunctionName(),
            ObservableService::FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL)) {
        computeManyKinematicOneModelTask(task);
    } else {
        throwException(__func__,
                "unknown function name = " + task.getFunctionName());
    }
}

ObservableResultList ObservableService::computeDVCSObservable(
        DVCSModule* pDVCSModule, Observable* pObservable,
        const ObservableKinematic &observableKinematic,
        DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule) {

    pDVCSModule->setDVCSConvolCoeffFunctionModule(
            pDVCSConvolCoeffFunctionModule);
    pObservable->setDVCSModule(pDVCSModule);

    return pObservable->compute(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2(),
            observableKinematic.getListOfPhi());
}

ObservableResultList ObservableService::computeManyKinematicOneModel(
        std::vector<ObservableKinematic> listOfKinematic,
        DVCSModule* pDVCSModule, Observable* pObservable,
        DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule) {

    ObservableResultList results;

    for (unsigned int i = 0; i != listOfKinematic.size(); i++) {
        results.add(
                computeDVCSObservable(pDVCSModule, pObservable,
                        listOfKinematic[i], pDVCSConvolCoeffFunctionModule));
    }

    return results;
}

void ObservableService::computeDVCSObservableTask(Task& task) {

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

    ObservableResultList result = computeDVCSObservable(pDVCSModule,
            pObservable, kinematic, pDVCSConvolCoeffFunctionModule);

    info(__func__,
            Formatter() << task.getFunctionName() << "("
                    << pObservable->getClassName() << ")" << '\n'
                    << result.toString());
}

void ObservableService::computeManyKinematicOneModelTask(Task& task) {

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

    std::vector<ObservableKinematic> listOfKinematic;

    if (task.isAvailableParameterList("ObservableKinematic")) {
        ParameterList parameterList = task.getLastAvailableParameterList();
        if (parameterList.isAvailable("file")) {
            std::string filePath = parameterList.getLastAvailable().toString();
            std::vector<std::string> kinematicString = FileUtils::readByLine(
                    filePath);

            if (kinematicString.empty()) {
                throwException(__func__,
                        Formatter() << "Empty kinematic input file : "
                                << filePath);
            }

            for (size_t i = 0; i != kinematicString.size(); i++) {
                std::vector<std::string> kinematicValues = StringUtils::split(
                        kinematicString[i], '|');
                if (kinematicValues.size() < 4) {
                    throwException(__func__,
                            Formatter()
                                    << "Missing column value in your kinematic input file : "
                                    << filePath
                                    << " ; You must provided 4 column");
                }

                listOfKinematic.push_back(
                        ObservableKinematic(kinematicValues[0],
                                kinematicValues[1], kinematicValues[2],
                                kinematicValues[3]));

            }

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

    ObservableResultList result = computeManyKinematicOneModel(listOfKinematic,
            pDVCSModule, pObservable, pDVCSConvolCoeffFunctionModule);

    info(__func__,
            Formatter() << task.getFunctionName() << "("
                    << pObservable->getClassName() << ")" << '\n'
                    << result.toString());
}
