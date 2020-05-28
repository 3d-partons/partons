#include "../../../../../../include/partons/modules/observable/DVMP/cross_section/DVMPCrossSectionUUMinusPi0.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/MesonPolarization.h"
#include "../../../../../../include/partons/beans/observable/DVMP/DVMPObservableResult.h"
#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/process/DVMP/DVMPProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVMPCrossSectionUUMinusPi0::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPCrossSectionUUMinusPi0("DVMPCrossSectionUUMinusPi0"));

DVMPCrossSectionUUMinusPi0::DVMPCrossSectionUUMinusPi0(
        const std::string &className) :
        DVMPObservable(className) {
}

DVMPCrossSectionUUMinusPi0::DVMPCrossSectionUUMinusPi0(
        const DVMPCrossSectionUUMinusPi0& other) :
        DVMPObservable(other) {
}

DVMPCrossSectionUUMinusPi0::~DVMPCrossSectionUUMinusPi0() {
}

DVMPCrossSectionUUMinusPi0* DVMPCrossSectionUUMinusPi0::clone() const {
    return new DVMPCrossSectionUUMinusPi0(*this);
}

PhysicalType<double> DVMPCrossSectionUUMinusPi0::computeObservable(
        const DVMPObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //evaluate
    DVMPObservableResult A = m_pProcessModule->compute(1., -1,
            NumA::Vector3D(0., 0., 0.), MesonPolarization::UNDEFINED, kinematic,
            gpdType);

    DVMPObservableResult B = m_pProcessModule->compute(-1., -1,
            NumA::Vector3D(0., 0., 0.), MesonPolarization::UNDEFINED, kinematic,
            gpdType);

    //combine
    PhysicalType<double> result = (A.getValue() + B.getValue()) / 2.;

    //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result *= 2. * Constant::PI;

    //change to nb
    return result.makeSameUnitAs(PhysicalUnit::NB);
}

} /* namespace PARTONS */
