#include "GPDKinematicDaoService.h"

GPDKinematicDaoService::GPDKinematicDaoService() :
        BaseObject("GPDKinematicDaoService") {

}
GPDKinematicDaoService::~GPDKinematicDaoService() {

}

int GPDKinematicDaoService::insert(const GPDKinematic &gpdKinematic) const {
    return m_GPDKinematicDao.insert(gpdKinematic.getX(), gpdKinematic.getXi(),
            gpdKinematic.getT(), gpdKinematic.getMuF2(), gpdKinematic.getMuR2());
}

int GPDKinematicDaoService::select(const GPDKinematic &gpdKinematic) const {
    return m_GPDKinematicDao.select(gpdKinematic.getX(), gpdKinematic.getXi(),
            gpdKinematic.getT(), gpdKinematic.getMuF2(), gpdKinematic.getMuR2());
}
