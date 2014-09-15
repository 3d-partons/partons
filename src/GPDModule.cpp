#include "GPDModule.h"

#include "models/GPDOutputData.h"

GPDModule::GPDModule(std::string _ID) :
        BaseModule(_ID), pGPDOutputData(0) {
}

GPDModule::~GPDModule() {
    if (pGPDOutputData != 0) {
        delete pGPDOutputData;
        pGPDOutputData = 0;
    }
}
