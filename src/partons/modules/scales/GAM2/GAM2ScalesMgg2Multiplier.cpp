#include "../../../../../include/partons/modules/scales/GAM2/GAM2ScalesMgg2Multiplier.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../../include/partons/beans/Scales.h"

namespace PARTONS {

const std::string GAM2ScalesMgg2Multiplier::PARAMETER_NAME_LAMBDA = "lambda";

const unsigned int GAM2ScalesMgg2Multiplier::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GAM2ScalesMgg2Multiplier("GAM2ScalesMgg2Multiplier"));

GAM2ScalesMgg2Multiplier::GAM2ScalesMgg2Multiplier(const std::string &className) :
        GAM2ScalesModule(className), m_lambda(1.) {
}

GAM2ScalesMgg2Multiplier::GAM2ScalesMgg2Multiplier(
        const GAM2ScalesMgg2Multiplier &other) :
        GAM2ScalesModule(other), m_lambda(other.m_lambda) {
}

GAM2ScalesMgg2Multiplier::~GAM2ScalesMgg2Multiplier() {
}

GAM2ScalesMgg2Multiplier* GAM2ScalesMgg2Multiplier::clone() const {
    return new GAM2ScalesMgg2Multiplier(*this);
}

void GAM2ScalesMgg2Multiplier::configure(
        const ElemUtils::Parameters &parameters) {

    GAM2ScalesModule::configure(parameters);

    if (parameters.isAvailable(
            GAM2ScalesMgg2Multiplier::PARAMETER_NAME_LAMBDA)) {

        m_lambda = parameters.getLastAvailable().toDouble();

        info(__func__,
                ElemUtils::Formatter()
                        << GAM2ScalesMgg2Multiplier::PARAMETER_NAME_LAMBDA
                        << " configured with value = " << m_lambda);
    }
}

Scales GAM2ScalesMgg2Multiplier::compute(
        const GAM2ObservableKinematic& kinematic) {

    PhysicalType<double> scale(m_lambda * kinematic.getMgg2());

    return Scales(scale, scale);
}

} /* namespace PARTONS */
