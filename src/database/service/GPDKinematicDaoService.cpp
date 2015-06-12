#include "GPDKinematicDaoService.h"

#include "../../beans/kinematic/GPDKinematic.h"
#include "../dao/GPDKinematicDao.h"

GPDKinematicDaoService::GPDKinematicDaoService()
        : BaseObject("GPDKinematicDaoService") {

}
GPDKinematicDaoService::~GPDKinematicDaoService() {

}

int GPDKinematicDaoService::insert(const GPDKinematic &gpdKinematic) {
    return GPDKinematicDao::insert(gpdKinematic.getKinematicType(),
            gpdKinematic.getX(), gpdKinematic.getXi(), gpdKinematic.getT(),
            gpdKinematic.getMuF(), gpdKinematic.getMuR());
}

int GPDKinematicDaoService::select(const GPDKinematic &gpdKinematic) {
    return GPDKinematicDao::select(gpdKinematic.getKinematicType(),
            gpdKinematic.getX(), gpdKinematic.getXi(), gpdKinematic.getT(),
            gpdKinematic.getMuF(), gpdKinematic.getMuR());
}
