#include "../../include/partons/BaseObjectFactory.h"

#include <utility>

#include "../../include/partons/BaseObjectRegistry.h"

// Global static pointer used to ensure a single instance of the class.
BaseObjectFactory* BaseObjectFactory::m_pInstance = 0;

BaseObjectFactory::BaseObjectFactory() :
        m_pBaseObjectRegistry(0) {
    m_pBaseObjectRegistry = BaseObjectRegistry::getInstance();
}

BaseObjectFactory::~BaseObjectFactory() {
    // m_pCreatedBaseObjectList never delete'ed. (exist until program termination)
    // because we can't guarantee correct destruction order
    for (std::map<unsigned int, BaseObject*>::iterator it =
            m_pInstantiatedObject.begin(); it != m_pInstantiatedObject.end();
            it++) {
        if ((it->second)) {
            delete (it->second);
            (it->second) = 0;
        }
    }
}

void BaseObjectFactory::delete_() {
    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

BaseObjectFactory* BaseObjectFactory::getInstance() {
    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new BaseObjectFactory();
    }

    return m_pInstance;
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
    m_pInstantiatedObject.insert(
            std::make_pair(pBaseObject->getObjectId(), pBaseObject));
}

void BaseObjectFactory::removeFromStore(unsigned int baseObjectUniqueId) {
    std::map<unsigned int, BaseObject*>::iterator it =
            m_pInstantiatedObject.find(baseObjectUniqueId);
    if (it != m_pInstantiatedObject.end()) {
        m_pInstantiatedObject.erase(baseObjectUniqueId);
    }
}

