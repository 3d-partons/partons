#include "../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongGK.h"

#include <cmath>

#include "../../../../include/partons/BaseObjectRegistry.h"

namespace PARTONS {

const unsigned int RunningAlphaStrongGK::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new RunningAlphaStrongGK("RunningAlphaStrongGK"));

RunningAlphaStrongGK::RunningAlphaStrongGK(const std::string &className) :
        RunningAlphaStrongModule(className), m_cLambdaQCD(0.22) {
    setNf(3);
}

RunningAlphaStrongGK::RunningAlphaStrongGK(const RunningAlphaStrongGK &other) :
        RunningAlphaStrongModule(other), m_cLambdaQCD(other.m_cLambdaQCD) {
}

RunningAlphaStrongGK* RunningAlphaStrongGK::clone() const {
    return new RunningAlphaStrongGK(*this);
}

RunningAlphaStrongGK::~RunningAlphaStrongGK() {
}

void RunningAlphaStrongGK::initModule() {
    RunningAlphaStrongModule::initModule();
}

void RunningAlphaStrongGK::isModuleWellConfigured() {
    RunningAlphaStrongModule::isModuleWellConfigured();
}

double RunningAlphaStrongGK::compute() {

    m_alphaS = (12.0 * M_PI)
            / ((33. - 2. * m_nf) * log(m_Mu2 / pow(m_cLambdaQCD, 2.)));

    return m_alphaS;
}

} /* namespace PARTONS */
