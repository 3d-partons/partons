#include "../../../../../../include/partons/modules/observable/TCS/cross_section/TCSCrossSectionUUMinus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/process/TCS/TCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int TCSCrossSectionUUMinus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSCrossSectionUUMinus("TCSCrossSectionUUMinus"));

TCSCrossSectionUUMinus::TCSCrossSectionUUMinus(const std::string &className) :
        TCSObservable(className, ObservableType::PHI) {
}

TCSCrossSectionUUMinus::TCSCrossSectionUUMinus(
        const TCSCrossSectionUUMinus& other) :
        TCSObservable(other) {
}

TCSCrossSectionUUMinus::~TCSCrossSectionUUMinus() {
}

TCSCrossSectionUUMinus* TCSCrossSectionUUMinus::clone() const {
    return new TCSCrossSectionUUMinus(*this);
}

PhysicalType<double> TCSCrossSectionUUMinus::computePhiTCSObservable(
        const TCSObservableKinematic& kinematic) {

    //evaluate
    TCSObservableResult A = m_pProcessModule->compute(1., -1,
            NumA::Vector3D(0., 0., 0.), kinematic);

    TCSObservableResult B = m_pProcessModule->compute(-1., -1,
            NumA::Vector3D(0., 0., 0.), kinematic);

    //combine
    PhysicalType<double> result = (A.getValue() + B.getValue()) / 2.;

    //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result = result * 2. * Constant::PI;

    //change to nb TODO conversion
    result = result * Constant::CONV_GEVm2_TO_NBARN;
    result.setUnit(PhysicalUnit::NB);

    return result;
}

} /* namespace PARTONS */
