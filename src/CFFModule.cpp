#include "CFFModule.h"

#include "GPDService.h"

CFFModule::CFFModule(std::string _ID) :
        BaseModule(_ID), pGPDService(GPDService::getInstance()) {

}

CFFModule::~CFFModule() {
    //TODO a verifier
    if (pGPDService != 0) {
        delete pGPDService;
        pGPDService = 0;
    }
}
