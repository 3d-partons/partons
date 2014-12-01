#include "GPDModule.h"

#include <iostream>
#include <utility>

#include "../beans/gpd/GPDOutputData.h"
#include "../utils/logger/LoggerManager.h"
#include "../utils/stringUtils/Formatter.h"
#include "EvolQCDModule.h"

GPDModule::GPDModule(const std::string &moduleID)
        : ModuleObject(moduleID), m_pEvolQCDModule(0) {
}

GPDModule::GPDModule(const GPDModule &other)
        : ModuleObject(other) {
    m_listGPDComputeTypeAvailable = other.m_listGPDComputeTypeAvailable;
    m_it = other.m_it;

    m_x = other.m_x;
    m_xi = other.m_xi;
    m_t = other.m_t;
    m_MuF = other.m_MuF;
    m_MuR = other.m_MuR;

    m_MuF_ref = other.m_MuF_ref;

    if (other.m_pEvolQCDModule != 0) {
        m_pEvolQCDModule = other.m_pEvolQCDModule->clone();
    } else {
        m_pEvolQCDModule = 0;
    }

//		if (other.m_pPDFModule != 0) {
//			m_pPDFModule = other.m_pPDFModule->clone();
//		}
}

GPDModule::~GPDModule() {
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
        const double &MuF, const double &MuR) {
    m_x = x;
    m_xi = xi;
    m_t = t;
    m_MuF = MuF;
    m_MuR = MuR;

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "x = " << m_x << "    xi = " << m_xi << "    t = "
                    << m_t << " GeV2    MuF = " << m_MuF << " GeV    MuR = "
                    << m_MuR << " GeV");

    initModule();

    isModuleWellConfigured();
}

GPDOutputData GPDModule::compute(const double &_x, const double &_xi,
        const double &_t, const double &_MuF, const double &_MuR,
        GPDComputeType::Type gpdComputeType) {

    preCompute(_x, _xi, _t, _MuF, _MuR);

    GPDOutputData gpdOutputData;

    switch (gpdComputeType) {
    case GPDComputeType::ALL: {
        for (m_it = m_listGPDComputeTypeAvailable.begin();
                m_it != m_listGPDComputeTypeAvailable.end(); m_it++) {
            GPDResultData gpdResultData = ((*this).*(m_it->second))();
            gpdOutputData.addGPDResultData(gpdResultData);
        }
        break;
    }
    default: {
        m_it = m_listGPDComputeTypeAvailable.find(gpdComputeType);
        if (m_it != m_listGPDComputeTypeAvailable.end()) {
            GPDResultData gpdResultData = ((*this).*(m_it->second))();
            gpdOutputData.addGPDResultData(gpdResultData);
        } else {
            //TODO remplacer par une exception
            std::cerr << "[GK11Model::compute] GPDComputeType not available !"
                    << std::endl;
        }
        break;
    }
    }

    return gpdOutputData;
}

GPDOutputData GPDModule::computeWithEvolution(const double &_x,
        const double &_xi, const double &_t, const double &_MuF,
        const double &_MuR, GPDComputeType::Type gpdComputeType) {

    preCompute(_x, _xi, _t, _MuF, _MuR);

    GPDOutputData gpdOutputData;

    switch (gpdComputeType) {
    case GPDComputeType::ALL: {
        for (m_it = m_listGPDComputeTypeAvailable.begin();
                m_it != m_listGPDComputeTypeAvailable.end(); m_it++) {
            GPDResultData gpdResultData = ((*this).*(m_it->second))();

            if (m_pEvolQCDModule != 0
                    && m_pEvolQCDModule->isRunnable(_MuF, m_MuF_ref,
                            EvolQCDModule::RELATIVE)) {
                gpdResultData = m_pEvolQCDModule->compute(m_x, m_xi, m_t, m_MuF,
                        m_MuR, gpdResultData);
            }

            gpdOutputData.addGPDResultData(gpdResultData);
        }
        break;
    }
    default: {
        m_it = m_listGPDComputeTypeAvailable.find(gpdComputeType);
        if (m_it != m_listGPDComputeTypeAvailable.end()) {
            GPDResultData gpdResultData = ((*this).*(m_it->second))();

            if (m_pEvolQCDModule != 0
                    && m_pEvolQCDModule->isRunnable(_MuF, m_MuF_ref,
                            EvolQCDModule::RELATIVE)) {
                gpdResultData = m_pEvolQCDModule->compute(m_x, m_xi, m_t, m_MuF,
                        m_MuR, gpdResultData);
            }

            gpdOutputData.addGPDResultData(gpdResultData);
        } else {
            //TODO remplacer par une exception
            std::cerr << "[GK11Model::compute] GPDComputeType not available !"
                    << std::endl;
        }
        break;
    }
    }

    return gpdOutputData;
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

//TODO est-il nécessaire de tester le pointeur null ?
void GPDModule::setEvolQcdModule(EvolQCDModule* pEvolQcdModule) {
    m_pEvolQCDModule = pEvolQcdModule;
    if (m_pEvolQCDModule != 0)
        m_pEvolQCDModule->setGpdModule(this);
}

double GPDModule::getMuFRef() const {
    return m_MuF_ref;
}

std::string GPDModule::toString() {
    return ModuleObject::toString();
}
