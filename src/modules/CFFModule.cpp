#include "CFFModule.h"

#include <stdexcept>
#include <utility>

#include "../beans/cff/CFFInputData.h"
#include "../beans/cff/CFFOutputData.h"
#include "../utils/logger/LoggerManager.h"
#include "../utils/stringUtils/Formatter.h"
#include "GPDModule.h"

CFFModule::CFFModule(const std::string &className)
        : ModuleObject(className), m_xi(0.), m_xB(0.), m_t(0.), m_Q2(0.), m_MuF(
                0.), m_MuR(0.), m_qcdOrderType(QCDOrderType::UNDEFINED), m_currentGPDComputeType(
                GPDComputeType::UNDEFINED), m_gpdComputeType(
                GPDComputeType::UNDEFINED), m_pGPDModule(0) {

}

CFFModule::CFFModule(const CFFModule &other)
        : ModuleObject(other) {
    m_listOfCFFComputeFunctionAvailable =
            other.m_listOfCFFComputeFunctionAvailable;
    m_it = other.m_it;

    m_xi = other.m_xi;
    m_xB = other.m_xB;
    m_t = other.m_t;
    m_Q2 = other.m_Q2;
    m_MuF = other.m_MuF;
    m_MuR = other.m_MuR;

    m_qcdOrderType = other.m_qcdOrderType;
    m_currentGPDComputeType = other.m_currentGPDComputeType;
    m_gpdComputeType = other.m_gpdComputeType;

    if (other.m_pGPDModule != 0) {
        // GPDModule is an abstract class, so it's impossible to use copy constructor to get a new instance of the object
        m_pGPDModule = (other.m_pGPDModule)->clone();
    } else {
        m_pGPDModule = 0;
    }
}

CFFModule::~CFFModule() {
}

//TODO implement
void CFFModule::initModule() {
    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "executed");

    m_xi = m_xB / (2 - m_xB);
}

//TODO implement
void CFFModule::isModuleWellConfigured() {
    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "executed");

    // Test kinematic domain of Xi
    if (m_xi < 0 || m_xi > 1) {
        Formatter formatter;
        formatter
                << "[CFFModule::isModuleWellConfigured] : Input value of Xi = "
                << m_xi << " do not lay between 0 and 1.";
        throw std::runtime_error(formatter.str());
    }

    // Test kinematic domain of t
    if (m_t > 0) {
        Formatter formatter;
        formatter << "[CFFModule::isModuleWellConfigured] : Input value of t = "
                << m_t << " is not <= 0.";
        throw std::runtime_error(formatter.str());
    }

    // Test kinematic domain of Q2
    if (m_Q2 < 0) {
        Formatter formatter;
        formatter
                << "[CFFModule::isModuleWellConfigured] : Input value of Q2 = "
                << m_Q2 << " is not > 0.";
        throw std::runtime_error(formatter.str());
    }
}

CFFOutputData CFFModule::compute(const double xB, const double t,
        const double Q2, const double MuF, const double MuR,
        GPDComputeType::Type gpdComputeType) {
    preCompute(xB, t, Q2, MuF, MuR, gpdComputeType);

    CFFOutputData cffOutputData(CFFInputData(m_xB, m_t, m_Q2));

    switch (m_gpdComputeType) {
    case GPDComputeType::ALL: {

        for (m_it = m_listOfCFFComputeFunctionAvailable.begin();
                m_it != m_listOfCFFComputeFunctionAvailable.end(); m_it++) {
            m_currentGPDComputeType = m_it->first;

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "m_currentGPDComputeType = "
                            << GPDComputeType(m_currentGPDComputeType).toString());

            // call function store in the base class map
            cffOutputData.add(m_currentGPDComputeType,
                    ((*this).*(m_it->second))());
        }
        break;
    }
    default: {
        m_it = m_listOfCFFComputeFunctionAvailable.find(m_gpdComputeType);
        if (m_it != m_listOfCFFComputeFunctionAvailable.end()) {
            m_currentGPDComputeType = m_gpdComputeType;
            // call function store in the base class map
            cffOutputData.add(m_currentGPDComputeType,
                    ((*this).*(m_it->second))());
        } else {
            //TODO throw an exception : can't compute this
        }
        break;
    }
    }

    return cffOutputData;
}

std::complex<double> CFFModule::computeUnpolarized() {
    throw std::runtime_error(
            "[CFFModule::computeUnpolarized] check your child implementation : "
                    + getClassName());
}

std::complex<double> CFFModule::computePolarized() {
    throw std::runtime_error(
            "[CFFModule::computeUnpolarized] check your child implementation : "
                    + getClassName());
}

void CFFModule::preCompute(const double xB, const double t, const double Q2,
        const double MuF, const double MuR,
        GPDComputeType::Type gpdComputeType) {

    m_xB = xB;
    m_t = t;
    m_Q2 = Q2;
    m_MuF = MuF;
    m_MuR = MuR;
    m_gpdComputeType = gpdComputeType;

    // execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "executed");
}

const GPDModule* CFFModule::getGpdModule() const {
    return m_pGPDModule;
}

void CFFModule::setGpdModule(GPDModule* gpdModule) {
    m_pGPDModule = gpdModule;
}

QCDOrderType::Type CFFModule::getQcdOrderType() const {
    return m_qcdOrderType;
}

void CFFModule::setQcdOrderType(QCDOrderType::Type qcdOrderType) {
    m_qcdOrderType = qcdOrderType;
}
