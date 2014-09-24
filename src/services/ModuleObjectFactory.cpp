#include "ModuleObjectFactory.h"

#include <sstream>
#include <utility>

#include "../cff/CFFModule.h"
#include "../gpd/GPDModule.h"

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

//TODO travailler avec les références

ModuleObject* ModuleObjectFactory::getModule(const std::string & ID) {
    return m_moduleRegistry[ID]->clone();
}

GPDModule* ModuleObjectFactory::getGPDModule(const std::string & ID) {

    return static_cast<GPDModule*>(getModule(ID));
}

CFFModule* ModuleObjectFactory::getCFFModule(const std::string & ID) {
    return static_cast<CFFModule*>(getModule(ID));
}

std::string ModuleObjectFactory::toString() {
    std::ostringstream os;
    os << "[BaseModuleFactory]" << std::endl;
    for (m_it = m_moduleRegistry.begin(); m_it != m_moduleRegistry.end();
            m_it++) {
        os << m_it->first << " - id = " << m_it->second->getId() << std::endl;
    }

    return os.str();
}
