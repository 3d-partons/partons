#include "../../../../../include/partons/modules/xi_converter/TCS/TCSXiConverterTauToXi.h"

#include <cmath>

#include "../../../../../include/partons/beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int TCSXiConverterTauToXi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSXiConverterTauToXi("TCSXiConverterTauToXi"));

TCSXiConverterTauToXi::TCSXiConverterTauToXi(const std::string &className) :
        TCSXiConverterModule(className) {
}

TCSXiConverterTauToXi::TCSXiConverterTauToXi(const TCSXiConverterTauToXi &other) :
        TCSXiConverterModule(other) {
}

TCSXiConverterTauToXi::~TCSXiConverterTauToXi() {
}

TCSXiConverterTauToXi* TCSXiConverterTauToXi::clone() const {
    return new TCSXiConverterTauToXi(*this);
}

PhysicalType<double> TCSXiConverterTauToXi::compute(
        const TCSObservableKinematic& kinematic) {

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
