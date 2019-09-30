#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../../include/partons/beans/Scales.h"
#include "../../../../../include/partons/modules/scales/TCS/TCSScalesQ2PrimMultiplier.h"

namespace PARTONS {

const std::string TCSScalesQ2PrimMultiplier::PARAMETER_NAME_LAMBDA = "lambda";

const unsigned int TCSScalesQ2PrimMultiplier::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSScalesQ2PrimMultiplier("TCSScalesQ2PrimMultiplier"));

TCSScalesQ2PrimMultiplier::TCSScalesQ2PrimMultiplier(
        const std::string &className) :
        TCSScalesModule(className), m_lambda(1.) {
}

TCSScalesQ2PrimMultiplier::TCSScalesQ2PrimMultiplier(
        const TCSScalesQ2PrimMultiplier &other) :
        TCSScalesModule(other), m_lambda(other.m_lambda) {
}

TCSScalesQ2PrimMultiplier::~TCSScalesQ2PrimMultiplier() {
}

TCSScalesQ2PrimMultiplier* TCSScalesQ2PrimMultiplier::clone() const {
    return new TCSScalesQ2PrimMultiplier(*this);
}

void TCSScalesQ2PrimMultiplier::configure(
        const ElemUtils::Parameters &parameters) {

    TCSScalesModule::configure(parameters);

    if (parameters.isAvailable(
            TCSScalesQ2PrimMultiplier::PARAMETER_NAME_LAMBDA)) {

        m_lambda = parameters.getLastAvailable().toDouble();

        info(__func__,
                ElemUtils::Formatter()
                        << TCSScalesQ2PrimMultiplier::PARAMETER_NAME_LAMBDA
                        << " configured with value = " << m_lambda);
    }
}

Scales TCSScalesQ2PrimMultiplier::compute(
        const TCSObservableKinematic& kinematic) {

    PhysicalType<double> scale(m_lambda * kinematic.getQ2Prim());

    return Scales(scale, scale);
}

} /* namespace PARTONS */
