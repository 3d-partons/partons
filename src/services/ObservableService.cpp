#include "ObservableService.h"

#include <utility>
#include <vector>

#include "../beans/kinematic/ObservableKinematic.h"
#include "../beans/Scenario.h"
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

std::map<double, double> ObservableService::computeDVCSObservableWithPhiDependency(
        DVCSModule* pDVCSModule, Observable* pObservable,
        ObservableKinematic observableKinematic, CFFOutputData cffOutputData) {

    std::map<double, double> results;
    std::vector<double> listOfPhi = observableKinematic.getListOfPhi();

    for (unsigned int i = 0; i != listOfPhi.size(); i++) {

        results.insert(
                std::pair<double, double>(listOfPhi[i],
                        pDVCSModule->computeWithPhiDependency(
                                observableKinematic.getXB(),
                                observableKinematic.getT(),
                                observableKinematic.getQ2(), listOfPhi[i],
                                cffOutputData, pObservable)));
    }

    return results;
}

//TODO implement all function
//TODO passer les chaine de caractere en variable final static
void ObservableService::computeScenario(Scenario scenario) {
}
