#include "ObservableKinematicDaoService.h"

#include "../dao/ObservableKinematicDao.h"

ObservableKinematicDaoService::ObservableKinematicDaoService() :
        BaseObject("ObservableKinematicDaoService") {
}

ObservableKinematicDaoService::~ObservableKinematicDaoService() {
}

int ObservableKinematicDaoService::insert(
        const ObservableKinematic &observableKinematic) const {
    return ObservableKinematicDao::insert(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2(),
            observableKinematic.getListOfPhi_str());
}

int ObservableKinematicDaoService::getKinematicId(
        const ObservableKinematic& observableKinematic) const {
    return ObservableKinematicDao::select(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2(),
            observableKinematic.getListOfPhi_str());
}
