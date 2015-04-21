#include "ModuleObjectFactory.h"

#include <sstream>
#include <stdexcept>
#include <utility>

#include "modules/CFFModule.h"
#include "modules/EvolQCDModule.h"
#include "modules/GPDModule.h"
#include "modules/MathIntegratorModule.h"
#include "modules/observable/DVCSModule.h"
#include "modules/RunningAlphaStrongModule.h"

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

    // m_moduleRegistry never delete'ed. (exist until program termination)
    // because we can't guarantee correct destruction order
    for (m_it = m_moduleRegistry.begin(); m_it != m_moduleRegistry.end();
            m_it++) {
        delete (m_it->second);
        (m_it->second) = 0;
    }

    if (m_pInstance != 0) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

std::string ModuleObjectFactory::registerModule(ModuleObject * pModuleObject) {
    m_it = m_moduleRegistry.find(pModuleObject->getClassName());
    if (m_it == m_moduleRegistry.end()) {
        m_moduleRegistry.insert(
                std::pair<std::string, ModuleObject*>(
                        pModuleObject->getClassName(), pModuleObject));
    } else {
        throw std::runtime_error(
                "[ModuleObjectFactory::registerModule] Module ID is already in the registry - Please check your moduleID = "
                        + pModuleObject->getClassName());
    }

    return pModuleObject->getClassName();
}

// Some modules depend of other so we need to make references at NULL and assign pointer later.
// Performed by the init() method of target module.
void ModuleObjectFactory::init() {
    for (m_it = m_moduleRegistry.begin(); m_it != m_moduleRegistry.end();
            m_it++) {
        (m_it->second)->init();
    }
}

ModuleObject* ModuleObjectFactory::newModule(const std::string & ID) {
    return m_moduleRegistry[ID]->clone();
}

GPDModule* ModuleObjectFactory::newGPDModule(const std::string & ID) {

    return static_cast<GPDModule*>(newModule(ID));
}

CFFModule* ModuleObjectFactory::newCFFModule(const std::string & ID) {
    return static_cast<CFFModule*>(newModule(ID));
}

EvolQCDModule* ModuleObjectFactory::newEvolQCDModule(const std::string & ID) {
    return static_cast<EvolQCDModule*>(newModule(ID));
}

DVCSModule* ModuleObjectFactory::newDVCSModule(const std::string & ID) {
    return static_cast<DVCSModule*>(newModule(ID));
}

MathIntegratorModule* ModuleObjectFactory::newMathIntegratorModule(
        const std::string & ID) {
    return static_cast<MathIntegratorModule*>(newModule(ID));
}

RunningAlphaStrongModule* ModuleObjectFactory::newRunningAlphaStrongModule(
        const std::string & ID) {
    return static_cast<RunningAlphaStrongModule*>(newModule(ID));
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
