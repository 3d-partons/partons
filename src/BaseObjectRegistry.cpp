#include "BaseObjectRegistry.h"

#include <stdexcept>
#include <utility>

// Global static pointer used to ensure a single instance of the class.
BaseObjectRegistry* BaseObjectRegistry::m_pInstance = 0;

BaseObjectRegistry::BaseObjectRegistry() {
}

BaseObjectRegistry::~BaseObjectRegistry() {
}

BaseObjectRegistry* BaseObjectRegistry::getInstance() {
    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new BaseObjectRegistry();
    }

    return m_pInstance;
}

void BaseObjectRegistry::add(BaseObject* baseObject) {
    m_it = m_registry.find(baseObject->getClassName());
    if (m_it == m_registry.end()) {
        m_registry.insert(
                std::make_pair<std::string, BaseObject*>(
                        baseObject->getClassName(), baseObject));
    } else {
        throw std::runtime_error(
                "[BaseObjectRegistry::add] BaseObject className is already in the registry - Please check your id = "
                        + baseObject->getClassName());
    }

    // return pService->getClassName();

}

BaseObject* BaseObjectRegistry::get(const std::string & baseObjectName) {
    return m_registry[baseObjectName];
}
