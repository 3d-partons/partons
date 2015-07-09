#include "BaseObjectManager.h"

#include <utility>

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
    // Unallocated memory for all instantiated object
    for (std::map<unsigned int, BaseObject*>::iterator it =
            m_instantiatedObject.begin(); it != m_instantiatedObject.end();
            it++) {
        if (it->second != 0) {
            delete (it->second);
            (it->second) = 0;
        }
    }

    if (m_pInstance != 0) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

void BaseObjectManager::add(BaseObject* pBaseObject) {
    m_instantiatedObject.insert(
            std::make_pair<unsigned int, BaseObject*>(
                    pBaseObject->getObjectId(), pBaseObject));
}
