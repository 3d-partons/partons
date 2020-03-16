#include "../../../../../include/partons/modules/xi_converter/TCS/TCSXiConverterTauToXiNucleiSimple.h"

#include <cmath>

#include "../../../../../include/partons/beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int TCSXiConverterTauToXiNucleiSimple::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSXiConverterTauToXiNucleiSimple(
                        "TCSXiConverterTauToXiNucleiSimple"));

TCSXiConverterTauToXiNucleiSimple::TCSXiConverterTauToXiNucleiSimple(
        const std::string &className) :
        TCSXiConverterModule(className), Nuclei(className) {
}

TCSXiConverterTauToXiNucleiSimple::TCSXiConverterTauToXiNucleiSimple(
        const TCSXiConverterTauToXiNucleiSimple &other) :
        TCSXiConverterModule(other), Nuclei(other) {
}

TCSXiConverterTauToXiNucleiSimple::~TCSXiConverterTauToXiNucleiSimple() {
}

TCSXiConverterTauToXiNucleiSimple* TCSXiConverterTauToXiNucleiSimple::clone() const {
    return new TCSXiConverterTauToXiNucleiSimple(*this);
}

PhysicalType<double> TCSXiConverterTauToXiNucleiSimple::compute(
        const TCSObservableKinematic& kinematic) {

    //USE THESE VARIABLES
    //m_A;
    //m_Z;
    //

    double s =
            pow(Constant::PROTON_MASS, 2)
                    + 2. * Constant::PROTON_MASS
                            * kinematic.getE().makeSameUnitAs(PhysicalUnit::GEV).getValue();

    double Q2Prim =
            kinematic.getQ2Prim().makeSameUnitAs(PhysicalUnit::GEV2).getValue();

    double tau = Q2Prim / (s - pow(Constant::PROTON_MASS, 2));

    return PhysicalType<double>(tau / (2. - tau), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
