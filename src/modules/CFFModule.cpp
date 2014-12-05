#include "CFFModule.h"

#include "GPDModule.h"

CFFModule::CFFModule(std::string _ID)
        : ModuleObject(_ID), m_xi(0.), m_xB(0.), m_t(0.), m_Q2(0.), m_MuF(0.), m_MuR(
                0.), m_qcdOrderType(QCDOrderType::UNDEFINED), m_currentGPDComputeType(
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

    if (other.m_pGPDModule != 0) {
        // GPDModule is an abstract class, so it's impossible to use copy constructor to get a new instance of the object
        m_pGPDModule = (other.m_pGPDModule)->clone();
    } else {
        m_pGPDModule = 0;
    }
}

CFFModule::~CFFModule() {
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
