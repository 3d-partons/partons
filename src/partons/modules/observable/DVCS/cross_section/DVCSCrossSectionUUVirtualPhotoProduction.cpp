#include "../../../../../../include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionUUVirtualPhotoProduction.h"

#include <NumA/linear_algebra/vector/Vector3D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/beans/process/VCSSubProcessType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSCrossSectionUUVirtualPhotoProduction::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCrossSectionUUVirtualPhotoProduction(
                        "DVCSCrossSectionUUVirtualPhotoProduction"));

DVCSCrossSectionUUVirtualPhotoProduction::DVCSCrossSectionUUVirtualPhotoProduction(
        const std::string &className) :
        DVCSObservable(className) {
}

DVCSCrossSectionUUVirtualPhotoProduction::DVCSCrossSectionUUVirtualPhotoProduction(
        const DVCSCrossSectionUUVirtualPhotoProduction& other) :
        DVCSObservable(other) {
}

DVCSCrossSectionUUVirtualPhotoProduction::~DVCSCrossSectionUUVirtualPhotoProduction() {
}

DVCSCrossSectionUUVirtualPhotoProduction* DVCSCrossSectionUUVirtualPhotoProduction::clone() const {
    return new DVCSCrossSectionUUVirtualPhotoProduction(*this);
}

PhysicalType<double> DVCSCrossSectionUUVirtualPhotoProduction::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //evaluate
    DVCSObservableResult A = m_pProcessModule->compute(1., -1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType,
            VCSSubProcessType::DVCS);

    DVCSObservableResult B = m_pProcessModule->compute(-1., -1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType,
            VCSSubProcessType::DVCS);

    //combine
    PhysicalType<double> result = (A.getValue() + B.getValue()) / 2.;

    //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result *= 2. * Constant::PI;

    //virtual photon flux
    result /= getVirtualPhotonFlux(kinematic);

    //change to nb
    return result.makeSameUnitAs(PhysicalUnit::NB);
}

double DVCSCrossSectionUUVirtualPhotoProduction::getVirtualPhotonFlux(
        const DVCSObservableKinematic& kinematic) const {

    double xB = kinematic.getXB().getValue();
    double Q2 = kinematic.getQ2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    double E = kinematic.getE().makeSameUnitAs(PhysicalUnit::GEV).getValue();

    double nu = Q2 / (2 * Constant::PROTON_MASS * xB);
    double y = nu / E;

    double eps = 2 * xB * Constant::PROTON_MASS / sqrt(Q2);
    double e = (1. - y - pow(eps * y / 2., 2))
            / (1 - y + pow(y, 2) / 2 + pow(eps * y / 2., 2));

    double value = Constant::FINE_STRUCTURE_CONSTANT / (2 * Constant::PI);
    value *= pow(y, 2) / (1 - e);
    value *= (1. - xB) / (xB * Q2);

    return value;
}

} /* namespace PARTONS */
