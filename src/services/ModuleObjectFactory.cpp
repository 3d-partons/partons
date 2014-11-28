#include "ModuleObjectFactory.h"

#include <sstream>
#include <utility>

#include "../modules/CFFModule.h"
#include "../modules/EvolQCDModule.h"
#include "../modules/GPDModule.h"
#include "../modules/observable/DVCSModule.h"

// Global static pointer used to ensure a single instance of the class.
ModuleObjectFactory* ModuleObjectFactory::m_pInstance = 0;

ModuleObjectFactory::ModuleObjectFactory() {
}

ModuleObjectFactory* ModuleObjectFactory::getInstance() {
    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new ModuleObjectFactory();
    }

    return m_pInstance;
}

ModuleObjectFactory::~ModuleObjectFactory() {

    // m_modules never delete'ed. (exist until program termination)
    // because we can't guarantee correct destruction order

    if (m_pInstance != 0) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

bool ModuleObjectFactory::registerModule(ModuleObject * pModuleObject) {
    m_moduleRegistry.insert(
            std::pair<std::string, ModuleObject*>(pModuleObject->getClassName(),
                    pModuleObject));

    return true;
}

ModuleObject* ModuleObjectFactory::getModule(const std::string & ID) {
    return m_moduleRegistry[ID]->clone();
}

GPDModule* ModuleObjectFactory::getGPDModule(const std::string & ID) {

    return static_cast<GPDModule*>(getModule(ID));
}

CFFModule* ModuleObjectFactory::getCFFModule(const std::string & ID) {
    return static_cast<CFFModule*>(getModule(ID));
}

EvolQCDModule* ModuleObjectFactory::getEvolQCDModule(const std::string & ID) {
    return static_cast<EvolQCDModule*>(getModule(ID));
}

DVCSModule* ModuleObjectFactory::getDVCSModule(const std::string & ID) {
    return static_cast<DVCSModule*>(getModule(ID));
}

std::string ModuleObjectFactory::toString() {
    std::ostringstream os;
    os << "[BaseModuleFactory]" << std::endl;
    for (m_it = m_moduleRegistry.begin(); m_it != m_moduleRegistry.end();
            m_it++) {
        os << m_it->second->toString() << std::endl;
    }

    return os.str();
}
