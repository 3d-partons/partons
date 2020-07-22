#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/MesonPolarization.h"
#include "../../../../../../include/partons/beans/observable/DVMP/DVMPObservableResult.h"
#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/DVMP/cross_section/DVMPCrossSectionUUUMinus.h"
#include "../../../../../../include/partons/modules/process/DVMP/DVMPProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVMPCrossSectionUUUMinus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPCrossSectionUUUMinus("DVMPCrossSectionUUUMinus"));

DVMPCrossSectionUUUMinus::DVMPCrossSectionUUUMinus(
        const std::string &className) :
        DVMPObservable(className) {
}

DVMPCrossSectionUUUMinus::DVMPCrossSectionUUUMinus(
        const DVMPCrossSectionUUUMinus& other) :
        DVMPObservable(other) {
}

DVMPCrossSectionUUUMinus::~DVMPCrossSectionUUUMinus() {
}

DVMPCrossSectionUUUMinus* DVMPCrossSectionUUUMinus::clone() const {
    return new DVMPCrossSectionUUUMinus(*this);
}

PhysicalType<double> DVMPCrossSectionUUUMinus::computeObservable(
        const DVMPObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //TODO VECTOR MESON POLARISATION

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
