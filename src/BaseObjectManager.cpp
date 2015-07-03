
#include "BaseObjectManager.h"

#include <string>
#include <utility>

#include "utils/logger/LoggerManager.h"
#include "utils/stringUtils/Formatter.h"

// Global static pointer used to ensure a single instance of the class.
BaseObjectManager* BaseObjectManager::m_pInstance = 0;

BaseObjectManager::BaseObjectManager() {
}

BaseObjectManager* BaseObjectManager::getInstance() {
    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new BaseObjectManager();
    }

    return m_pInstance;
}

BaseObjectManager::~BaseObjectManager() {
    LoggerManager::getInstance()->debug("BaseObjectManager", __func__,
            Formatter() << "Number of instantiated object to destroy="
                    << m_instantiatedObject.size());

    // Unallocated memory for all instantiated object
    for (m_it = m_instantiatedObject.begin();
            m_it != m_instantiatedObject.end(); m_it++) {
        if (m_it->second) {
            LoggerManager::getInstance()->debug("BaseObjectManager", __func__,
                    Formatter() << "Destroying ... : "
                            << m_it->second->getClassName() << " | "
                            << m_it->second->getObjectId());
            delete (m_it->second);
            (m_it->second) = 0;
        }
    }

    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

void BaseObjectManager::add(BaseObject* pBaseObject) {
    m_instantiatedObject.insert(
            std::make_pair<unsigned int, BaseObject*>(
                    pBaseObject->getObjectId(), pBaseObject));
}
