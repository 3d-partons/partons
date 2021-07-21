#include "../../../../../include/partons/modules/scales/DVMP/DVMPScalesQ2Multiplier.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../../include/partons/beans/Scales.h"

namespace PARTONS {

const std::string DVMPScalesQ2Multiplier::PARAMETER_NAME_LAMBDA = "lambda";

const unsigned int DVMPScalesQ2Multiplier::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPScalesQ2Multiplier("DVMPScalesQ2Multiplier"));

DVMPScalesQ2Multiplier::DVMPScalesQ2Multiplier(const std::string &className) :
        DVMPScalesModule(className), m_lambda(1.) {
}

DVMPScalesQ2Multiplier::DVMPScalesQ2Multiplier(
        const DVMPScalesQ2Multiplier &other) :
        DVMPScalesModule(other), m_lambda(other.m_lambda) {
}

DVMPScalesQ2Multiplier::~DVMPScalesQ2Multiplier() {
}

DVMPScalesQ2Multiplier* DVMPScalesQ2Multiplier::clone() const {
    return new DVMPScalesQ2Multiplier(*this);
}

void DVMPScalesQ2Multiplier::configure(
        const ElemUtils::Parameters &parameters) {

    DVMPScalesModule::configure(parameters);

    if (parameters.isAvailable(DVMPScalesQ2Multiplier::PARAMETER_NAME_LAMBDA)) {

        m_lambda = parameters.getLastAvailable().toDouble();

        info(__func__,
                ElemUtils::Formatter()
                        << DVMPScalesQ2Multiplier::PARAMETER_NAME_LAMBDA
                        << " configured with value = " << m_lambda);
    }
}

Scales DVMPScalesQ2Multiplier::compute(
        const DVMPObservableKinematic& kinematic) {

    PhysicalType<double> scale(m_lambda * kinematic.getQ2());

    return Scales(scale, scale);
}

} /* namespace PARTONS */
