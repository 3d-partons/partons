#include "../../../../../../include/partons/modules/observable/TCS/cross_section/TCSCrossSectionUUWeighted.h"

#include <NumA/linear_algebra/vector/Vector3D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/process/TCS/TCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int TCSCrossSectionUUWeighted::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSCrossSectionUUWeighted("TCSCrossSectionUUWeighted"));

TCSCrossSectionUUWeighted::TCSCrossSectionUUWeighted(
        const std::string &className) :
        TCSObservable(className) {
}

TCSCrossSectionUUWeighted::TCSCrossSectionUUWeighted(
        const TCSCrossSectionUUWeighted& other) :
        TCSObservable(other) {
}

TCSCrossSectionUUWeighted::~TCSCrossSectionUUWeighted() {
}

TCSCrossSectionUUWeighted* TCSCrossSectionUUWeighted::clone() const {
    return new TCSCrossSectionUUWeighted(*this);
}

PhysicalType<double> TCSCrossSectionUUWeighted::computeObservable(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    //evaluate
    TCSObservableResult A = m_pProcessModule->compute(1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType);

    TCSObservableResult B = m_pProcessModule->compute(-1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType);

    //combine
    PhysicalType<double> result = (A.getValue() + B.getValue()) / 2.;

    //weight
    double t = kinematic.getT().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    double Q2Prim =
            kinematic.getQ2Prim().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    double E = kinematic.getE().makeSameUnitAs(PhysicalUnit::GEV).getValue();
    double theta =
            kinematic.getTheta().makeSameUnitAs(PhysicalUnit::RAD).getValue();
    double phi =
            kinematic.getPhi().makeSameUnitAs(PhysicalUnit::RAD).getValue();

    double Mp2 = Constant::PROTON_MASS * Constant::PROTON_MASS;

    double s = Mp2 + 2. * Constant::PROTON_MASS * E;

    double r = sqrt(
            (s - Q2Prim - Mp2) * (s - Q2Prim - Mp2) - 4. * Q2Prim * Mp2);

    double DeltaT2 = -t * t * s - Mp2 * Q2Prim * Q2Prim;
    DeltaT2 += t * ((s + Mp2) * Q2Prim - (s - Mp2) * (s - Mp2));
    DeltaT2 /= (s - Mp2) * (s - Mp2);

    double b = Q2Prim * (s - Mp2 - Q2Prim) + t * (s + Q2Prim - Mp2);
    b *= cos(theta) / r;
    b += -2. * (s - Mp2) * sqrt(Q2Prim) * sqrt(DeltaT2) * sin(theta) * cos(phi)
            / r;

    double L = (Q2Prim - t) * (Q2Prim - t) - b * b;
    L /= 4.;

    double L0 = pow(Q2Prim, 2) * pow(sin(theta), 2) / 4.;

    result *= L / L0;

    //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result *= 2. * Constant::PI;

    //change to nb
    return result.makeSameUnitAs(PhysicalUnit::NB);
}

} /* namespace PARTONS */
