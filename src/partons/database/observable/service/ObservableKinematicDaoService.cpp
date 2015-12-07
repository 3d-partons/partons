#include "../../../../../include/partons/database/observable/service/ObservableKinematicDaoService.h"

ObservableKinematicDaoService::ObservableKinematicDaoService() :
        BaseObject("ObservableKinematicDaoService") {
}

ObservableKinematicDaoService::~ObservableKinematicDaoService() {
}

int ObservableKinematicDaoService::insert(
        const ObservableKinematic &observableKinematic) const {
    return m_observableKinematicDao.insert(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2(),
            observableKinematic.getPhi());
}

int ObservableKinematicDaoService::getKinematicId(
        const ObservableKinematic& observableKinematic) const {
    return m_observableKinematicDao.select(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2(),
            observableKinematic.getPhi());
}

List<ObservableKinematic> ObservableKinematicDaoService::getKinematicListByComputationId(
        int computationId) const {
    return m_observableKinematicDao.getKinematicListByComputationId(
            computationId);
}

ObservableKinematic ObservableKinematicDaoService::getKinematicById(
        const int kinematicId) const {
    return m_observableKinematicDao.getKinematicById(kinematicId);
}
