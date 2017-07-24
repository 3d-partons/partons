#include "../../../../include/partons/modules/scales/ScalesQ2Multiplier.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/beans/Scales.h"
#include "../../../../include/partons/BaseObjectRegistry.h"

const std::string ScalesQ2Multiplier::PARAMETER_NAME_LAMBDA = "lambda";

const unsigned int ScalesQ2Multiplier::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new ScalesQ2Multiplier("ScalesQ2Multiplier"));

ScalesQ2Multiplier::ScalesQ2Multiplier(const std::string &className) :
        ScalesModule(className), m_lambda(1.) {
}

ScalesQ2Multiplier::~ScalesQ2Multiplier() {
}

ScalesQ2Multiplier* ScalesQ2Multiplier::clone() const {
    return new ScalesQ2Multiplier(*this);
}

Scales ScalesQ2Multiplier::compute(double Q2) {
    initModule();
    isModuleWellConfigured();

    double scale = 0.;

    scale = m_lambda * Q2;

    return Scales(scale, scale);
}

ScalesQ2Multiplier::ScalesQ2Multiplier(const ScalesQ2Multiplier& other) :
        ScalesModule(other) {
    m_lambda = other.m_lambda;
}

void ScalesQ2Multiplier::initModule() {
    ScalesModule::initModule();
}

void ScalesQ2Multiplier::isModuleWellConfigured() {
    ScalesModule::isModuleWellConfigured();
}

void ScalesQ2Multiplier::configure(const ElemUtils::Parameters &parameters) {
    if (parameters.isAvailable(ScalesQ2Multiplier::PARAMETER_NAME_LAMBDA)) {
        m_lambda = parameters.getLastAvailable().toDouble();

        info(__func__,
                ElemUtils::Formatter() << ScalesQ2Multiplier::PARAMETER_NAME_LAMBDA
                        << " configured with value = " << m_lambda);
    }

    ScalesModule::configure(parameters);
}
