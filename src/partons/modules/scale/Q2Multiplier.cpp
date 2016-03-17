#include "../../../../include/partons/modules/scale/Q2Multiplier.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/beans/Scale.h"
#include "../../../../include/partons/BaseObjectRegistry.h"

const std::string Q2Multiplier::PARAMETER_NAME_LAMBDA = "lambda";

// Initialise [class]::classId with a unique name.
const unsigned int Q2Multiplier::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new Q2Multiplier("Q2Multiplier"));

Q2Multiplier::Q2Multiplier(const std::string &className) :
        ScaleModule(className), m_lambda(1.) {
}

Q2Multiplier::~Q2Multiplier() {
}

Q2Multiplier* Q2Multiplier::clone() const {
    return new Q2Multiplier(*this);
}

Scale Q2Multiplier::compute(double Q2) {
    initModule();
    isModuleWellConfigured();

    double scale = 0.;

    scale = m_lambda * Q2;

    return Scale(scale, scale);
}

Q2Multiplier::Q2Multiplier(const Q2Multiplier& other) :
        ScaleModule(other) {
    m_lambda = other.m_lambda;
}

void Q2Multiplier::initModule() {
    ScaleModule::initModule();
}

void Q2Multiplier::isModuleWellConfigured() {
    ScaleModule::isModuleWellConfigured();

//    if (m_lambda == 1.) {
//        warn(__func__, Formatter() << "m_lambda = " << m_lambda);
//    }
}

void Q2Multiplier::configure(const ElemUtils::Parameters &parameters) {
    if (parameters.isAvailable(Q2Multiplier::PARAMETER_NAME_LAMBDA)) {
        m_lambda = parameters.getLastAvailable().toDouble();

        info(__func__,
                ElemUtils::Formatter() << Q2Multiplier::PARAMETER_NAME_LAMBDA
                        << " configured with value = " << m_lambda);
    }

    ScaleModule::configure(parameters);
}
