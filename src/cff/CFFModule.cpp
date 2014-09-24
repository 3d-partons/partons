#include "CFFModule.h"

#include "../services/GPDService.h"

CFFModule::CFFModule(std::string _ID) :
        ModuleObject(_ID), m_pGPDService(GPDService::getInstance()), m_pGPDModule(
                0), m_qcdOrderType(0) {

}

CFFModule::CFFModule(const CFFModule &other) :
        ModuleObject(other) {
//TODO pointer member copy here
}

CFFModule::~CFFModule() {
    //TODO a verifier
    if (m_pGPDService != 0) {
        delete m_pGPDService;
        m_pGPDService = 0;
    }
}

void CFFModule::setGPDModule(GPDModule* gpdModule) {
    m_pGPDModule = gpdModule;
}

void CFFModule::setQCDOrderType(QCDOrderType* qcdOrderType) {
    m_qcdOrderType = qcdOrderType;
}
