#include "ObservableKinematicDaoService.h"

ObservableKinematicDaoService::ObservableKinematicDaoService() :
        BaseObject("ObservableKinematicDaoService") {
}

ObservableKinematicDaoService::~ObservableKinematicDaoService() {
}

int ObservableKinematicDaoService::insert(
        const ObservableKinematic &observableKinematic) const {
    return m_observableKinematicDao.insert(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2(),
            observableKinematic.getListOfPhi_str());
}

int ObservableKinematicDaoService::getKinematicId(
        const ObservableKinematic& observableKinematic) const {
    return m_observableKinematicDao.select(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2(),
            observableKinematic.getListOfPhi_str());
}
