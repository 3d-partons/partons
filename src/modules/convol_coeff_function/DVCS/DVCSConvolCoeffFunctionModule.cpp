#include "DVCSConvolCoeffFunctionModule.h"

#include <stdexcept>
#include <utility>

#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../../ModuleObjectFactory.h"
#include "../../../utils/logger/LoggerManager.h"
#include "../../../utils/stringUtils/Formatter.h"
#include "../../active_flavors/NfFunctionExample.h"
#include "../../GPDModule.h"
#include "../../RunningAlphaStrongModule.h"

DVCSConvolCoeffFunctionModule::DVCSConvolCoeffFunctionModule(
        const std::string &className) :
        ConvolCoeffFunctionModule(className), m_xi(0.), m_t(0.), m_Q2(0.), m_MuF2(
                0.), m_MuR2(0.), m_nf(0), m_qcdOrderType(
                PerturbativeQCDOrderType::UNDEFINED), m_currentGPDComputeType(
                GPDType::UNDEFINED), m_gpdType(GPDType::UNDEFINED), m_pGPDModule(
                0), m_pRunningAlphaStrongModule(0), m_pNfConvolCoeffFunction(0) {

}

DVCSConvolCoeffFunctionModule::DVCSConvolCoeffFunctionModule(
        const DVCSConvolCoeffFunctionModule &other) :
        ConvolCoeffFunctionModule(other) {
    m_listOfCFFComputeFunctionAvailable =
            other.m_listOfCFFComputeFunctionAvailable;
    m_it = other.m_it;

    m_xi = other.m_xi;
    m_t = other.m_t;
    m_Q2 = other.m_Q2;
    m_MuF2 = other.m_MuF2;
    m_MuR2 = other.m_MuR2;

    m_nf = other.m_nf;

    m_qcdOrderType = other.m_qcdOrderType;
    m_currentGPDComputeType = other.m_currentGPDComputeType;
    m_gpdType = other.m_gpdType;

    if (other.m_pGPDModule != 0) {
        // GPDModule is an abstract class, so it's impossible to use copy constructor to get a new instance of the object
        m_pGPDModule = (other.m_pGPDModule)->clone();
    } else {
        m_pGPDModule = 0;
    }

    if (other.m_pRunningAlphaStrongModule != 0) {
        // RunningAlphaStrongModule is an abstract class, so it's impossible to use copy constructor to get a new instance of the object
        m_pRunningAlphaStrongModule =
                (other.m_pRunningAlphaStrongModule)->clone();
    } else {
        m_pRunningAlphaStrongModule = 0;
    }

    if (other.m_pNfConvolCoeffFunction != 0) {
        // ActiveFlavorsModule is an abstract class, so it's impossible to use copy constructor to get a new instance of the object
        m_pNfConvolCoeffFunction = (other.m_pNfConvolCoeffFunction)->clone();
    } else {
        m_pNfConvolCoeffFunction = 0;
    }
}

DVCSConvolCoeffFunctionModule::~DVCSConvolCoeffFunctionModule() {
}

void DVCSConvolCoeffFunctionModule::init() {
    m_pNfConvolCoeffFunction = ModuleObjectFactory::newActiveFlavorsModule(
            NfFunctionExample::classId);
}

//TODO implement
void DVCSConvolCoeffFunctionModule::initModule() {

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "executed");
}

//TODO implement
void DVCSConvolCoeffFunctionModule::isModuleWellConfigured() {
    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "executed");

    // Test kinematic domain of Xi
    if (m_xi < 0 || m_xi > 1) {
        Formatter formatter;
        formatter << "Input value of Xi = " << m_xi
                << " do not lay between 0 and 1.";
        throwException(__func__, formatter.str());
    }

    // Test kinematic domain of t
    if (m_t > 0) {
        Formatter formatter;
        formatter << " Input value of t = " << m_t << " is not <= 0.";
        throwException(__func__, formatter.str());
    }

    // Test kinematic domain of Q2
    if (m_Q2 < 0) {
        Formatter formatter;
        formatter << "Input value of Q2 = " << m_Q2 << " is not > 0.";
        throwException(__func__, formatter.str());
    }

    if (m_pGPDModule == 0) {
        throwException(__func__, "m_pGPDModule is NULL");
    }

    if (m_pRunningAlphaStrongModule == 0) {
        throwException(__func__, "m_pRunningAlphaStrongModule is NULL");
    }

    if (m_pNfConvolCoeffFunction == 0) {
        throwException(__func__, "m_pNfConvolCoeffFunction is NULL");
    }
}

DVCSConvolCoeffFunctionResult DVCSConvolCoeffFunctionModule::compute(
        const double xi, const double t, const double Q2, const double MuF2,
        const double MuR2, GPDType::Type gpdComputeType) {
    preCompute(xi, t, Q2, MuF2, MuR2, gpdComputeType);

    DVCSConvolCoeffFunctionResult dvcsConvolCoeffFunctionResult;

    switch (m_gpdType) {
    case GPDType::ALL: {

        for (m_it = m_listOfCFFComputeFunctionAvailable.begin();
                m_it != m_listOfCFFComputeFunctionAvailable.end(); m_it++) {
            m_currentGPDComputeType = m_it->first;

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "m_currentGPDComputeType = "
                            << GPDType(m_currentGPDComputeType).toString());

            // call function store in the base class map
            dvcsConvolCoeffFunctionResult.add(m_currentGPDComputeType,
                    ((*this).*(m_it->second))());
        }
        break;
    }
    default: {
        m_it = m_listOfCFFComputeFunctionAvailable.find(m_gpdType);
        if (m_it != m_listOfCFFComputeFunctionAvailable.end()) {
            m_currentGPDComputeType = m_gpdType;
            // call function store in the base class map
            dvcsConvolCoeffFunctionResult.add(m_currentGPDComputeType,
                    ((*this).*(m_it->second))());
        } else {
            //TODO throw an exception : can't compute this
        }
        break;
    }
    }

    return dvcsConvolCoeffFunctionResult;
}

std::complex<double> DVCSConvolCoeffFunctionModule::computeUnpolarized() {
    throw std::runtime_error(
            "[CFFModule::computeUnpolarized] check your child implementation : "
                    + getClassName());
}

std::complex<double> DVCSConvolCoeffFunctionModule::computePolarized() {
    throw std::runtime_error(
            "[CFFModule::computeUnpolarized] check your child implementation : "
                    + getClassName());
}

void DVCSConvolCoeffFunctionModule::preCompute(const double xi, const double t,
        const double Q2, const double MuF2, const double MuR2,
        GPDType::Type gpdComputeType) {

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "enter preCompute() function ...");

    m_xi = xi;
    m_t = t;
    m_Q2 = Q2;
    m_MuF2 = MuF2;
    m_MuR2 = MuR2;
    m_gpdType = gpdComputeType;

    // execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();
}

const GPDModule* DVCSConvolCoeffFunctionModule::getGPDModule() const {
    return m_pGPDModule;
}

void DVCSConvolCoeffFunctionModule::setGPDModule(GPDModule* gpdModule) {
    m_pGPDModule = gpdModule;
}

PerturbativeQCDOrderType::Type DVCSConvolCoeffFunctionModule::getQCDOrderType() const {
    return m_qcdOrderType;
}

void DVCSConvolCoeffFunctionModule::setQCDOrderType(
        PerturbativeQCDOrderType::Type qcdOrderType) {
    m_qcdOrderType = qcdOrderType;
}

