#include "ObservableService.h"

#include <vector>

#include "../beans/observable/Observable.h"
#include "../beans/observable/ObservableKinematic.h"
#include "../beans/observable/ObservableResultList.h"
#include "../BaseObjectRegistry.h"
#include "../modules/observable/DVCSModule.h"

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

//TODO implement all function
//TODO passer les chaine de caractere en variable final static
void ObservableService::computeTask(const Task &task) {
}
