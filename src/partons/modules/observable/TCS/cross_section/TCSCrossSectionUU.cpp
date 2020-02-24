#include "../../../../../../include/partons/modules/observable/TCS/cross_section/TCSCrossSectionUU.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/process/TCS/TCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int TCSCrossSectionUU::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSCrossSectionUU("TCSCrossSectionUU"));

TCSCrossSectionUU::TCSCrossSectionUU(const std::string &className) :
        TCSObservable(className) {
}

TCSCrossSectionUU::TCSCrossSectionUU(const TCSCrossSectionUU& other) :
        TCSObservable(other) {
}

TCSCrossSectionUU::~TCSCrossSectionUU() {
}

TCSCrossSectionUU* TCSCrossSectionUU::clone() const {
    return new TCSCrossSectionUU(*this);
}

PhysicalType<double> TCSCrossSectionUU::computeObservable(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    //evaluate
    TCSObservableResult A = m_pProcessModule->compute(1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType);

    TCSObservableResult B = m_pProcessModule->compute(-1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType);

    //combine
    PhysicalType<double> result = (A.getValue() + B.getValue()) / 2.;

    //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result *= 2. * Constant::PI;

    //change to nb
    return result.makeSameUnitAs(PhysicalUnit::NB);
}

} /* namespace PARTONS */
