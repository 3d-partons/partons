#include "../../../../../include/partons/database/convol_coeff_function/service/ConvolCoeffFunctionKinematicDaoService.h"

ConvolCoeffFunctionKinematicDaoService::ConvolCoeffFunctionKinematicDaoService() :
        BaseObject("ConvolCoeffFunctionKinematicDaoService") {
}

ConvolCoeffFunctionKinematicDaoService::~ConvolCoeffFunctionKinematicDaoService() {
}

int ConvolCoeffFunctionKinematicDaoService::insert(
        const DVCSConvolCoeffFunctionKinematic& kinematic) const {
    return m_convolCoeffFunctionKinematicDao.insert(kinematic.getXi(),
            kinematic.getT(), kinematic.getQ2(), kinematic.getMuF2(),
            kinematic.getMuR2());
}

int ConvolCoeffFunctionKinematicDaoService::getKinematicId(
        const DVCSConvolCoeffFunctionKinematic& kinematic) const {
    return m_convolCoeffFunctionKinematicDao.select(kinematic.getXi(),
            kinematic.getT(), kinematic.getQ2(), kinematic.getMuF2(),
            kinematic.getMuR2());
}

DVCSConvolCoeffFunctionKinematic ConvolCoeffFunctionKinematicDaoService::getKinematicById(
        const int id) const {
    return m_convolCoeffFunctionKinematicDao.getKinematicById(id);
}
