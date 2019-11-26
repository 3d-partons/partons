#include "../../../../../include/partons/modules/xi_converter/TCS/TCSXiConverterExact.h"

#include <cmath>

#include "../../../../../include/partons/beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int TCSXiConverterExact::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSXiConverterExact("TCSXiConverterExact"));

TCSXiConverterExact::TCSXiConverterExact(const std::string &className) :
        TCSXiConverterModule(className) {
}

TCSXiConverterExact::TCSXiConverterExact(const TCSXiConverterExact &other) :
        TCSXiConverterModule(other) {
}

TCSXiConverterExact::~TCSXiConverterExact() {
}

TCSXiConverterExact* TCSXiConverterExact::clone() const {
    return new TCSXiConverterExact(*this);
}

PhysicalType<double> TCSXiConverterExact::compute(
        const TCSObservableKinematic& kinematic) {

    double s =
            pow(Constant::PROTON_MASS, 2)
                    + 2. * Constant::PROTON_MASS
                            * kinematic.getE().makeSameUnitAs(PhysicalUnit::GEV).getValue();

    double t = kinematic.getT().makeSameUnitAs(PhysicalUnit::GEV2).getValue();

    double Q2Prim =
            kinematic.getQ2Prim().makeSameUnitAs(PhysicalUnit::GEV2).getValue();

    return PhysicalType<double>(
            -1 * (0.5 * t - Q2Prim)
                    / (2 * s + t - 2 * pow(Constant::PROTON_MASS, 2) - Q2Prim),
            PhysicalUnit::NONE);
}

} /* namespace PARTONS */
