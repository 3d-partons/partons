#include "LambdaQ2Scale.h"

#include "../../beans/Scale.h"
#include "../../BaseObjectRegistry.h"
#include "../../utils/stringUtils/Formatter.h"

// Initialise [class]::classId with a unique name.
const unsigned int LambdaQ2Scale::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new LambdaQ2Scale("LambdaQ2Scale"));

LambdaQ2Scale::LambdaQ2Scale(const std::string &className) :
        ScaleModule(className), m_lambda(1.) {
}

LambdaQ2Scale::~LambdaQ2Scale() {
}

LambdaQ2Scale* LambdaQ2Scale::clone() const {
    return new LambdaQ2Scale(*this);
}

Scale LambdaQ2Scale::compute(double Q2) {
    initModule();
    isModuleWellConfigured();

    double scale = 0.;

    scale = m_lambda * Q2;

    return Scale(scale, scale);
}

LambdaQ2Scale::LambdaQ2Scale(const LambdaQ2Scale& other) :
        ScaleModule(other) {
    m_lambda = other.m_lambda;
}

void LambdaQ2Scale::initModule() {
    ScaleModule::initModule();
}

void LambdaQ2Scale::isModuleWellConfigured() {
    ScaleModule::isModuleWellConfigured();

    if (m_lambda == 1.) {
        warn(__func__, Formatter() << "m_lambda = " << m_lambda);
    }
}
