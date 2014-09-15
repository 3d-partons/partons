#include "BaseModuleFactory.h"

#include <utility>

#include "CFFModule.h"
#include "GPDModule.h"

// Global static pointer used to ensure a single instance of the class.
BaseModuleFactory* BaseModuleFactory::pInstance = 0;

BaseModuleFactory::BaseModuleFactory() {
}

BaseModuleFactory* BaseModuleFactory::getInstance() {
    // Only allow one instance of class to be generated.
    if (!pInstance) {
        pInstance = new BaseModuleFactory();
    }

    return pInstance;
}

BaseModuleFactory::~BaseModuleFactory() {

    //TODO Deallocation of modules

    if (pInstance != 0) {
        delete pInstance;
        pInstance = 0;
    }
}

//bool BaseModuleFactory::registerModule(const std::string& _ID,
//        BaseModule* _pBaseModule) {
//    modules.insert(std::pair<std::string, BaseModule*>(_ID, _pBaseModule));
//    return true;
//}

bool BaseModuleFactory::registerModule(BaseModule* _pBaseModule) {
    modules.insert(
            std::pair<std::string, BaseModule*>(_pBaseModule->getClassName(),
                    _pBaseModule));

    return true;
}

BaseModule* BaseModuleFactory::getModule(std::string _ID) {
    return modules[_ID];
}

GPDModule* BaseModuleFactory::getGPDModule(std::string _ID) {
    return static_cast<GPDModule*>(getModule(_ID));
}

CFFModule* BaseModuleFactory::getCFFModule(std::string _ID) {
    return static_cast<CFFModule*>(getModule(_ID));
}
