#include <cmath>

#include "../../../../../include/partons/beans/observable/GAM2/GAM2ObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/xi_converter/GAM2/GAM2XiConverterExact.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int GAM2XiConverterExact::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GAM2XiConverterExact("GAM2XiConverterExact"));

GAM2XiConverterExact::GAM2XiConverterExact(const std::string &className) :
        GAM2XiConverterModule(className) {
}

GAM2XiConverterExact::GAM2XiConverterExact(const GAM2XiConverterExact &other) :
        GAM2XiConverterModule(other) {
}

GAM2XiConverterExact::~GAM2XiConverterExact() {
}

GAM2XiConverterExact* GAM2XiConverterExact::clone() const {
    return new GAM2XiConverterExact(*this);
}

PhysicalType<double> GAM2XiConverterExact::compute(
        const GAM2ObservableKinematic& kinematic) {

    double s =
            pow(Constant::PROTON_MASS, 2)
                    + 2. * Constant::PROTON_MASS
                            * kinematic.getE().makeSameUnitAs(PhysicalUnit::GEV).getValue();

    return PhysicalType<double>(
            (kinematic.getMgg2().getValue() - kinematic.getT().getValue())
                    / (2. * s), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
