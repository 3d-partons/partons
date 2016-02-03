#include "../../include/partons/BaseObjectFactory.h"

#include <SFML/System/Lock.hpp>
#include <utility>

#include "../../include/partons/BaseObjectRegistry.h"

BaseObjectFactory::BaseObjectFactory(BaseObjectRegistry* pBaseObjectRegistry) :
        m_pBaseObjectRegistry(pBaseObjectRegistry) {
}

BaseObjectFactory::~BaseObjectFactory() {
    // Nothing to destroy
    // m_pBaseObjectRegistry pointer will be deleted by the Partons class
}

BaseObject* BaseObjectFactory::newBaseObject(unsigned int classId) {
    BaseObject* pBaseObject = m_pBaseObjectRegistry->get(classId)->clone();
    store(pBaseObject);
    return pBaseObject;
}

BaseObject* BaseObjectFactory::newBaseObject(const std::string& className) {
    BaseObject* pBaseObject = m_pBaseObjectRegistry->get(className)->clone();
    store(pBaseObject);
    return pBaseObject;
}

void BaseObjectFactory::store(BaseObject* pBaseObject) {
    sf::Lock lock(m_mutex); // mutex.lock()

    m_pInstantiatedObject.insert(
            std::make_pair(pBaseObject->getObjectId(), pBaseObject));
} // mutex.unlock()

void BaseObjectFactory::removeFromStore(unsigned int baseObjectUniqueId) {
    sf::Lock lock(m_mutex); // mutex.lock()

    std::map<unsigned int, BaseObject*>::iterator it =
            m_pInstantiatedObject.find(baseObjectUniqueId);
    if (it != m_pInstantiatedObject.end()) {
        m_pInstantiatedObject.erase(baseObjectUniqueId);
    }
} // mutex.unlock()

