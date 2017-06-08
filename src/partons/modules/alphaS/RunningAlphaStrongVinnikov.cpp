#include "../../../../include/partons/modules/alphaS/RunningAlphaStrongVinnikov.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <cmath>

#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

const unsigned int RunningAlphaStrongVinnikov::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new RunningAlphaStrongVinnikov("RunningAlphaStrongVinnikov"));

RunningAlphaStrongVinnikov::RunningAlphaStrongVinnikov(
        const std::string &className) :
        RunningAlphaStrongModule(className) {

    LOG_LAMBDA_5 = -1.4872;
    LOG_LAMBDA_4 = -1.1221;
    LOG_LAMBDA_3 = -0.9883;

    setNf(3);
}

RunningAlphaStrongVinnikov::RunningAlphaStrongVinnikov(
        const RunningAlphaStrongVinnikov &other) :
        RunningAlphaStrongModule(other) {

    LOG_LAMBDA_5 = other.LOG_LAMBDA_5;
    LOG_LAMBDA_4 = other.LOG_LAMBDA_4;
    LOG_LAMBDA_3 = other.LOG_LAMBDA_3;
}

RunningAlphaStrongVinnikov* RunningAlphaStrongVinnikov::clone() const {
    return new RunningAlphaStrongVinnikov(*this);
}

RunningAlphaStrongVinnikov::~RunningAlphaStrongVinnikov() {
}

void RunningAlphaStrongVinnikov::initModule() {
    RunningAlphaStrongModule::initModule();
}

void RunningAlphaStrongVinnikov::isModuleWellConfigured() {
    RunningAlphaStrongModule::isModuleWellConfigured();
}

double RunningAlphaStrongVinnikov::compute() {

    double L;
    double t = log(m_Mu2);

    double B0 = (11.0 - 2.0 * m_nf / 3.0);
    double B1 = (51.0 - 19.0 * m_nf / 3.0);

    if (m_nf == 3)
        L = t - 2.0 * LOG_LAMBDA_3;
    else if (m_nf == 4)
        L = t - 2.0 * LOG_LAMBDA_4;
    else if (m_nf == 5)
        L = t - 2.0 * LOG_LAMBDA_5;
    else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "N_F not between 3 and 5");
    }

    m_alphaS = 4.0 * Constant::PI / (B0 * L)
            * (1.0 - 2.0 * B1 * log(L) / (B0 * B0 * L));

    if (m_alphaS > 1.0)
        warn(__func__, ElemUtils::Formatter() << "alpha_s = " << m_alphaS);

    return m_alphaS;
}
