#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/beans/Scale.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/scale/ScaleQ2Multiplier.h"

const std::string ScaleQ2Multiplier::PARAMETER_NAME_LAMBDA = "lambda";

// Initialise [class]::classId with a unique name.
const unsigned int ScaleQ2Multiplier::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new ScaleQ2Multiplier("ScaleQ2Multiplier"));

ScaleQ2Multiplier::ScaleQ2Multiplier(const std::string &className) :
        ScaleModule(className), m_lambda(1.) {
}

ScaleQ2Multiplier::~ScaleQ2Multiplier() {
}

ScaleQ2Multiplier* ScaleQ2Multiplier::clone() const {
    return new ScaleQ2Multiplier(*this);
}

Scale ScaleQ2Multiplier::compute(double Q2) {
    initModule();
    isModuleWellConfigured();

    double scale = 0.;

    scale = m_lambda * Q2;

    return Scale(scale, scale);
}

ScaleQ2Multiplier::ScaleQ2Multiplier(const ScaleQ2Multiplier& other) :
        ScaleModule(other) {
    m_lambda = other.m_lambda;
}

void ScaleQ2Multiplier::initModule() {
    ScaleModule::initModule();
}

void ScaleQ2Multiplier::isModuleWellConfigured() {
    ScaleModule::isModuleWellConfigured();
}

void ScaleQ2Multiplier::configure(const ElemUtils::Parameters &parameters) {
    if (parameters.isAvailable(ScaleQ2Multiplier::PARAMETER_NAME_LAMBDA)) {
        m_lambda = parameters.getLastAvailable().toDouble();

        info(__func__,
                ElemUtils::Formatter() << ScaleQ2Multiplier::PARAMETER_NAME_LAMBDA
                        << " configured with value = " << m_lambda);
    }

    ScaleModule::configure(parameters);
}
