#include "ObservableService.h"

#include <vector>

#include "../beans/automation/Task.h"
//#include "../beans/observable/AluObservable.h"
#include "../beans/observable/Observable.h"
#include "../beans/observable/ObservableResultList.h"
#include "../BaseObjectRegistry.h"
#include "../modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../modules/GPDModule.h"
#include "../modules/observable/DVCSModule.h"
#include "../ModuleObjectFactory.h"
#include "../ObservableObjectFactory.h"
#include "../utils/GenericType.h"
#include "../utils/ParameterList.h"
#include "../utils/stringUtils/Formatter.h"
#include "../utils/stringUtils/StringUtils.h"

const std::string ObservableService::FUNCTION_NAME_COMPUTE_DVCS_OBSERVABLE =
        "computeDVCSObservable";

// Initialise [class]::classId with a unique name.
const unsigned int ObservableService::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new ObservableService("ObservableService"));

ObservableService::ObservableService(const std::string &className) :
        ServiceObject(className) {

}

ObservableService::~ObservableService() {
//TODO
}

//TODO implement all function
void ObservableService::computeTask(Task &task) {
    if (StringUtils::equals(task.getFunctionName(),
            ObservableService::FUNCTION_NAME_COMPUTE_DVCS_OBSERVABLE)) {

        //create a GPDKinematic and init it with a list of parameters
        ObservableKinematic kinematic;

        if (task.isAvailableParameterList("ObservableKinematic")) {
            kinematic = ObservableKinematic(
                    task.getLastAvailableParameterList());
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

        ObservableResultList result = computeDVCSObservable(pDVCSModule,
                pObservable, kinematic, pDVCSConvolCoeffFunctionModule);

        info(__func__, Formatter() << result.toString());

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

