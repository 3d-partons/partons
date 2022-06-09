#include "../../../../../../include/partons/modules/observable/DDVCS/cross_section/DDVCSCrossSectionUUMinus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/process/DDVCS/DDVCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DDVCSCrossSectionUUMinus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSCrossSectionUUMinus("DDVCSCrossSectionUUMinus"));

DDVCSCrossSectionUUMinus::DDVCSCrossSectionUUMinus(const std::string &className) :
        DDVCSObservable(className) {
}

DDVCSCrossSectionUUMinus::DDVCSCrossSectionUUMinus(
        const DDVCSCrossSectionUUMinus& other) :
        DDVCSObservable(other) {
}

DDVCSCrossSectionUUMinus::~DDVCSCrossSectionUUMinus() {
}

DDVCSCrossSectionUUMinus* DDVCSCrossSectionUUMinus::clone() const {
    return new DDVCSCrossSectionUUMinus(*this);
}

PhysicalType<double> DDVCSCrossSectionUUMinus::computeObservable(
        const DDVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //evaluate
    DDVCSObservableResult A = m_pProcessModule->compute(1., -1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType);

    DDVCSObservableResult B = m_pProcessModule->compute(-1., -1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType);

    //combine
    PhysicalType<double> result = (A.getValue() + B.getValue()) / 2.;

    //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result *= 2. * Constant::PI;

    //change to nb
    return result.makeSameUnitAs(PhysicalUnit::NB);
}

} /* namespace PARTONS */
