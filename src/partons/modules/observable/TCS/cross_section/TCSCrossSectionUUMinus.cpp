#include "../../../../../../include/partons/modules/observable/TCS/cross_section/TCSCrossSectionUUMinus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/process/TCS/TCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int TCSCrossSectionUUMinus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSCrossSectionUUMinus("TCSCrossSectionUUMinus"));

TCSCrossSectionUUMinus::TCSCrossSectionUUMinus(const std::string &className) :
        TCSObservable(className) {
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

PhysicalType<double> TCSCrossSectionUUMinus::computeObservable(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    //evaluate
    TCSObservableResult A = m_pProcessModule->compute(1., -1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType);

    TCSObservableResult B = m_pProcessModule->compute(-1., -1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType);

    //combine
    PhysicalType<double> result = (A.getValue() + B.getValue()) / 2.;

    //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result *= 2. * Constant::PI;

    //change to nb
    return result.makeSameUnitAs(PhysicalUnit::NB);
}

} /* namespace PARTONS */
