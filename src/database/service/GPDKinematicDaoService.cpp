#include "GPDKinematicDaoService.h"

#include "../../beans/kinematic/GPDKinematic.h"
#include "../dao/GPDKinematicDao.h"

//GPDKinematicDaoService::GPDKinematicDaoService() {
//
//}
//
//GPDKinematicDaoService::~GPDKinematicDaoService() {
//
//}

//TODO implement scenarioId, remove 0 value
int GPDKinematicDaoService::insert(const GPDKinematic &gpdKinematic) {
    return GPDKinematicDao::insert( /* */ 0 /* */ , gpdKinematic.getKinematicType(),
            gpdKinematic.getX(), gpdKinematic.getXi(), gpdKinematic.getT(),
            gpdKinematic.getMuF(), gpdKinematic.getMuR());
}
