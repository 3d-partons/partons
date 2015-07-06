#include "GPDModule.h"

#include <stdexcept>
#include <utility>

#include "../beans/gpd/GPDResult.h"
#include "../utils/GenericType.h"
#include "../utils/logger/LoggerManager.h"
#include "../utils/ParameterList.h"
#include "../utils/stringUtils/Formatter.h"
#include "EvolQCDModule.h"

GPDModule::GPDModule(const std::string &className) :
        ModuleObject(className), m_x(0.), m_xi(0.), m_t(0.), m_MuF(0.), m_MuR(
                0.), m_gpdType(GPDType::UNDEFINED), m_MuF_ref(0.), m_nf(0), m_pEvolQCDModule(
                0) {
}

GPDModule::GPDModule(const GPDModule &other) :
        ModuleObject(other) {
    m_x = other.m_x;
    m_xi = other.m_xi;
    m_t = other.m_t;
    m_MuF = other.m_MuF;
    m_MuR = other.m_MuR;

    m_gpdType = other.m_gpdType;

    m_MuF_ref = other.m_MuF_ref;
    m_nf = other.m_nf;

    if (other.m_pEvolQCDModule != 0) {
        m_pEvolQCDModule = other.m_pEvolQCDModule->clone();
    } else {
        m_pEvolQCDModule = 0;
    }

    m_listGPDComputeTypeAvailable = other.m_listGPDComputeTypeAvailable;
    m_it = other.m_it;

//		if (other.m_pPDFModule != 0) {
//			m_pPDFModule = other.m_pPDFModule->clone();
//		}
//    else
//    {
//        m_pPDFModule = 0;
//    }
}

GPDModule::~GPDModule() {
}

void GPDModule::configure(ParameterList parameters) {
    //TODO replace hard coded string by static const string
    m_MuF_ref = (parameters.get("MuRef")).getDouble();
}

//TODO implement
void GPDModule::initModule() {
    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "executed");
}

//TODO implement
void GPDModule::isModuleWellConfigured() {
    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "executed");
}

void GPDModule::preCompute(double x, double xi, double t, double MuF,
        double MuR, GPDType::Type gpdType) {
    m_x = x;
    m_xi = xi;
    m_t = t;
    m_MuF = MuF;
    m_MuR = MuR;
    m_gpdType = gpdType;

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "x = " << m_x << "    xi = " << m_xi << "    t = "
                    << m_t << " GeV2    MuF = " << m_MuF << " GeV    MuR = "
                    << m_MuR << " GeV");

    // execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();
}

GPDResult GPDModule::compute(double x, double xi, double t, double MuF,
        double MuR, GPDType::Type gpdType) {

    preCompute(x, xi, t, MuF, MuR, gpdType);

    return compute(false);
}

GPDResult GPDModule::computeWithEvolution(double x, double xi, double t,
        double MuF, double MuR, GPDType::Type gpdType) {

    preCompute(x, xi, t, MuF, MuR, gpdType);

    bool evolution = false;

    if (m_pEvolQCDModule != 0) {
        if (m_pEvolQCDModule->isRunnable(m_MuF, m_MuF_ref,
                EvolQCDModule::RELATIVE)) {
            evolution = true;
        }
    } else {
        //TODO exception pas de module d'evolution
    }

    return compute(evolution);
}

GPDResult GPDModule::compute(bool evolution) {
    GPDResult gpdResult;
    switch (m_gpdType) {
    case GPDType::ALL: {
        for (m_it = m_listGPDComputeTypeAvailable.begin();
                m_it != m_listGPDComputeTypeAvailable.end(); m_it++) {
            PartonDistribution partonDistribution = ((*this).*(m_it->second))();

            if (evolution) {
                partonDistribution = m_pEvolQCDModule->compute(m_x, m_xi, m_t,
                        m_MuF, m_MuR, this, partonDistribution);
            }

            gpdResult.addPartonDistribution(m_it->first, partonDistribution);
        }
        break;
    }
    default: {
        m_it = m_listGPDComputeTypeAvailable.find(m_gpdType);
        if (m_it != m_listGPDComputeTypeAvailable.end()) {
            PartonDistribution partonDistribution = ((*this).*(m_it->second))();

            if (evolution) {
                partonDistribution = m_pEvolQCDModule->compute(m_x, m_xi, m_t,
                        m_MuF, m_MuR, this, partonDistribution);
            }

            gpdResult.addPartonDistribution(m_it->first, partonDistribution);
        } else {
            throwException(__func__,
                    Formatter() << "GPD(" << GPDType(m_gpdType).toString()
                            << ") is not available for this GPD model");
        }
        break;
    }
    }

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << gpdResult.toString());

    return gpdResult;
}

//TODO implement
PartonDistribution GPDModule::computeH() {
    throw std::runtime_error("");
}

PartonDistribution GPDModule::computeE() {
    throw std::runtime_error("");
}

PartonDistribution GPDModule::computeHt() {
    throw std::runtime_error("");
}

PartonDistribution GPDModule::computeEt() {
    throw std::runtime_error("");
}

unsigned int GPDModule::getNf() const {
    return m_nf;
}

void GPDModule::setNf(unsigned int nf) {
    m_nf = nf;
}

const EvolQCDModule* GPDModule::getEvolQcdModule() const {
    return m_pEvolQCDModule;
}

void GPDModule::setEvolQcdModule(EvolQCDModule* pEvolQcdModule) {
    m_pEvolQCDModule = pEvolQcdModule;
    if (m_pEvolQCDModule != 0) {
        m_pEvolQCDModule->setGpdModule(this);
    }
}

double GPDModule::getMuFRef() const {
    return m_MuF_ref;
}

std::string GPDModule::toString() {
    return ModuleObject::toString();
}

double GPDModule::getMuF() const {
    return m_MuF;
}

void GPDModule::setMuF(double muF) {
    m_MuF = muF;
}

double GPDModule::getMuR() const {
    return m_MuR;
}

void GPDModule::setMuR(double muR) {
    m_MuR = muR;
}

double GPDModule::getT() const {
    return m_t;
}

void GPDModule::setT(double t) {
    m_t = t;
}

double GPDModule::getX() const {
    return m_x;
}

void GPDModule::setX(double x) {
    m_x = x;
}

double GPDModule::getXi() const {
    return m_xi;
}

void GPDModule::setXi(double xi) {
    m_xi = xi;
}
