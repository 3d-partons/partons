#include "../../include/partons/BaseObjectFactory.h"

#include <SFML/System/Lock.hpp>
#include <SFML/System/Mutex.hpp>
#include <utility>

#include "../../include/partons/BaseObjectRegistry.h"

sf::Mutex BaseObjectFactory::m_mutex;

// Global static pointer used to ensure a single instance of the class.
BaseObjectFactory* BaseObjectFactory::m_pInstance = 0;

BaseObjectFactory::BaseObjectFactory() :
        m_pBaseObjectRegistry(0) {
    m_pBaseObjectRegistry = BaseObjectRegistry::getInstance();
}

BaseObjectFactory::~BaseObjectFactory() {

    //TODO enable

//    // m_pCreatedBaseObjectList never delete'ed. (exist until program termination)
//    // because we can't guarantee correct destruction order
//    for (std::map<unsigned int, BaseObject*>::iterator it =
//            m_pInstantiatedObject.begin(); it != m_pInstantiatedObject.end();
//            it++) {
//        if ((it->second)) {
//            delete (it->second);
//            (it->second) = 0;
//        }
//    }
}

void BaseObjectFactory::delete_() {
    sf::Lock lock(m_mutex); // mutex.lock()

    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = 0;
    }
} // mutex.unlock()

BaseObjectFactory* BaseObjectFactory::getInstance() {
    sf::Lock lock(m_mutex); // mutex.lock()

    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new BaseObjectFactory();
    }

    return m_pInstance;
} // mutex.unlock()

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

