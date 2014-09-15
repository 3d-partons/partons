#include "CFFService.h"

// Global static pointer used to ensure a single instance of the class.
CFFService* CFFService::pInstance = 0;

CFFService::CFFService() {

}

CFFService* CFFService::getInstance() {
    // Only allow one instance of class to be generated.
    if (!pInstance) {
        pInstance = new CFFService();
    }

    return pInstance;
}

CFFService::~CFFService() {
    if (pInstance != 0) {
        delete pInstance;
        pInstance = 0;
    }
}

//TODO implementer
CFFOutputData* CFFService::compute(CFFInputData* _pCFFInputData,
        GPDModule* _pGPDModule) {
    return 0;
}
