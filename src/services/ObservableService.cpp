#include "ObservableService.h"

#include <typeinfo>
#include <utility>
#include <vector>

#include "../beans/kinematic/ObservableKinematic.h"
#include "../modules/observable/DVCSModule.h"

// Global static pointer used to ensure a single instance of the class.
ObservableService* ObservableService::m_pInstance = 0;

ObservableService::ObservableService()
        : BaseObject(typeid(*this).name()) {

}

ObservableService* ObservableService::getInstance() {
    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new ObservableService();
    }

    return m_pInstance;
}

ObservableService::~ObservableService() {
    if (m_pInstance != 0) {
        delete m_pInstance;
        m_pInstance = 0;
    }
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
