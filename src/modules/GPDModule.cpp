#include "GPDModule.h"

#include <stdexcept>
#include <utility>

#include "../beans/gpd/GPDOutputData.h"
#include "../beans/GenericData.h"
#include "../beans/Parameters.h"
#include "../utils/logger/LoggerManager.h"
#include "../utils/stringUtils/Formatter.h"
#include "EvolQCDModule.h"

GPDModule::GPDModule(const std::string &className)
        : ModuleObject(className), m_x(0.), m_xi(0.), m_t(0.), m_MuF(0.), m_MuR(
                0.), m_gpdComputeType(GPDComputeType::UNDEFINED), m_MuF_ref(0.), m_nf(
                0.), m_pEvolQCDModule(0) {
}

GPDModule::GPDModule(const GPDModule &other)
        : ModuleObject(other) {
    m_x = other.m_x;
    m_xi = other.m_xi;
    m_t = other.m_t;
    m_MuF = other.m_MuF;
    m_MuR = other.m_MuR;

    m_gpdComputeType = other.m_gpdComputeType;

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

void GPDModule::configure(Parameters parameters) {
    GenericData* pMuF_ref_value = parameters.getUnique(e_MUF_REF_STRING_KEY);

    if (pMuF_ref_value != 0) {
        m_MuF_ref = pMuF_ref_value->getDouble();
    }
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

void GPDModule::preCompute(const double &x, const double &xi, const double &t,
        const double &MuF, const double &MuR,
        GPDComputeType::Type gpdComputeType) {
    m_x = x;
    m_xi = xi;
    m_t = t;
    m_MuF = MuF;
    m_MuR = MuR;
    m_gpdComputeType = gpdComputeType;

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "x = " << m_x << "    xi = " << m_xi << "    t = "
                    << m_t << " GeV2    MuF = " << m_MuF << " GeV    MuR = "
                    << m_MuR << " GeV");

    // execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();
}

GPDOutputData GPDModule::compute(const double &_x, const double &_xi,
        const double &_t, const double &_MuF, const double &_MuR,
        GPDComputeType::Type gpdComputeType) {

    preCompute(_x, _xi, _t, _MuF, _MuR, gpdComputeType);

    return compute(false);
}

GPDOutputData GPDModule::computeWithEvolution(const double &_x,
        const double &_xi, const double &_t, const double &_MuF,
        const double &_MuR, GPDComputeType::Type gpdComputeType) {

    preCompute(_x, _xi, _t, _MuF, _MuR, gpdComputeType);

    bool evolution = false;

    if (m_pEvolQCDModule != 0) {
        if (m_pEvolQCDModule->isRunnable(_MuF, m_MuF_ref,
                EvolQCDModule::RELATIVE)) {
            evolution = true;
        }
    } else {
        //TODO exception pas de module d'evolution
    }

    return compute(evolution);
}

GPDOutputData GPDModule::compute(bool evolution) {
    GPDOutputData gpdOutputData;

    switch (m_gpdComputeType) {
    case GPDComputeType::ALL: {
        for (m_it = m_listGPDComputeTypeAvailable.begin();
                m_it != m_listGPDComputeTypeAvailable.end(); m_it++) {
            GPDResultData gpdResultData = ((*this).*(m_it->second))();

            if (evolution) {
                gpdResultData = m_pEvolQCDModule->compute(m_x, m_xi, m_t, m_MuF,
                        m_MuR, gpdResultData);
            }

            gpdOutputData.addGPDResultData(gpdResultData);
        }
        break;
    }
    default: {
        m_it = m_listGPDComputeTypeAvailable.find(m_gpdComputeType);
        if (m_it != m_listGPDComputeTypeAvailable.end()) {
            GPDResultData gpdResultData = ((*this).*(m_it->second))();

            if (evolution) {
                gpdResultData = m_pEvolQCDModule->compute(m_x, m_xi, m_t, m_MuF,
                        m_MuR, gpdResultData);
            }

            gpdOutputData.addGPDResultData(gpdResultData);
        } else {
            throwException(__func__,
                    Formatter() << "GPD("
                            << GPDComputeType(m_gpdComputeType).toString()
                            << ") is not available for this GPD model");
        }
        break;
    }
    }

    return gpdOutputData;
}

//TODO implement
GPDResultData GPDModule::computeH() {
    throw std::runtime_error("");
}

GPDResultData GPDModule::computeE() {
    throw std::runtime_error("");
}

GPDResultData GPDModule::computeHt() {
    throw std::runtime_error("");
}

GPDResultData GPDModule::computeEt() {
    throw std::runtime_error("");
}

double GPDModule::getNf() const {
    return m_nf;
}

void GPDModule::setNf(double nf) {
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
