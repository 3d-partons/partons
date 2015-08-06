#include "GPDKinematicDaoService.h"

#include "../../beans/gpd/GPDKinematic.h"
#include "../dao/GPDKinematicDao.h"

GPDKinematicDaoService::GPDKinematicDaoService() :
        BaseObject("GPDKinematicDaoService") {

}
GPDKinematicDaoService::~GPDKinematicDaoService() {

}

int GPDKinematicDaoService::insert(const GPDKinematic &gpdKinematic) {
    return GPDKinematicDao::insert(gpdKinematic.getKinematicType(),
            gpdKinematic.getX(), gpdKinematic.getXi(), gpdKinematic.getT(),
            gpdKinematic.getMuF2(), gpdKinematic.getMuR2());
}

int GPDKinematicDaoService::select(const GPDKinematic &gpdKinematic) {
    return GPDKinematicDao::select(gpdKinematic.getKinematicType(),
            gpdKinematic.getX(), gpdKinematic.getXi(), gpdKinematic.getT(),
            gpdKinematic.getMuF2(), gpdKinematic.getMuR2());
}
