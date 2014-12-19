#include "DVCSCFFFourQuarksModel.h"

#include <TMath.h>

#include "../../FundamentalPhysicalConstants.h"
#include "../../services/ModuleObjectFactory.h"
#include "../../utils/logger/LoggerManager.h"

// Initialise [class]::moduleID with a unique name.
const std::string DVCSCFFFourQuarksModel::moduleID =
        ModuleObjectFactory::getInstance()->registerModule(
                new DVCSCFFFourQuarksModel("DVCSCFFFourQuarksModel"));

DVCSCFFFourQuarksModel::DVCSCFFFourQuarksModel(const std::string &className)
        : DVCSCFFModel(className), m_Constant(3.) {

}

DVCSCFFFourQuarksModel::DVCSCFFFourQuarksModel(
        const DVCSCFFFourQuarksModel &other)
        : DVCSCFFModel(other) {
    m_Constant = other.m_Constant;
}

DVCSCFFFourQuarksModel* DVCSCFFFourQuarksModel::clone() const {
    return new DVCSCFFFourQuarksModel(*this);
}

void DVCSCFFFourQuarksModel::initModule() {
    DVCSCFFModel::initModule();

    m_Constant = 0.;
}

void DVCSCFFFourQuarksModel::isModuleWellConfigured() {
    DVCSCFFModel::isModuleWellConfigured();
}

std::complex<double> DVCSCFFFourQuarksModel::KernelGluonNLOV(double x) {
    m_pLoggerManager->debug(getClassName(), __func__, "entered");

    double z = x / m_xi;

    std::complex<double> LogOneMinusz(0., 0.);
    if (x < m_xi) {
        // remplacer z par x/xi
        LogOneMinusz = std::complex<double>(TMath::Log((1. - z) / 2.), 0.);
    }
    if (x > m_xi) {
        LogOneMinusz = std::complex<double>(TMath::Log((z - 1.) / 2.), -PI);
    }

    std::complex<double> GluonNLOV(LogOneMinusz);
    GluonNLOV += (m_logQ2OverMu2 - 2.);
    GluonNLOV /= 1. - z;
    GluonNLOV += LogOneMinusz / (1. + z);
    GluonNLOV *= (m_nbOfActiveFlavour / 2.);
    GluonNLOV += -KernelGluonNLOA(x);
    GluonNLOV *= -m_Constant;

    return GluonNLOV;
}
