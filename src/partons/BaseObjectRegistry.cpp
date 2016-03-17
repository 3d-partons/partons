#include "../../include/partons/BaseObjectRegistry.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <SFML/System/Lock.hpp>
#include <stdexcept>
#include <utility>

BaseObjectRegistry* BaseObjectRegistry::m_pInstance = 0;
unsigned int BaseObjectRegistry::m_uniqueClassIdCounter = 0;

BaseObjectRegistry* BaseObjectRegistry::getInstance() {
    if (!m_pInstance) {
        m_pInstance = new BaseObjectRegistry();
    }
    return m_pInstance;
}

BaseObjectRegistry::BaseObjectRegistry() {
    // Nothing to do
}

BaseObjectRegistry::~BaseObjectRegistry() {
    sf::Lock lock(m_mutex); // mutex.lock()

    // m_baseObjectList never delete'ed. (exist until program termination)
    // because we can't guarantee correct destruction order
    for (m_itBaseObjectList = m_baseObjectList.begin();
            m_itBaseObjectList != m_baseObjectList.end();
            m_itBaseObjectList++) {
        if (m_itBaseObjectList->second) {
            delete (m_itBaseObjectList->second);
            (m_itBaseObjectList->second) = 0;
        }
    }
} // mutex.unlock();

unsigned int BaseObjectRegistry::getUniqueClassId() {
    return BaseObjectRegistry::m_uniqueClassIdCounter++;
}

unsigned int BaseObjectRegistry::registerBaseObject(BaseObject * pBaseObject) {

    sf::Lock lock(m_mutex); // mutex.lock()

    unsigned int classId = 0;
    m_itTranslate = m_translate.find(pBaseObject->getClassName());
    if (m_itTranslate == m_translate.end()) {
        classId = getUniqueClassId();
        m_translate.insert(
                std::pair<std::string, BaseObject*>(pBaseObject->getClassName(),
                        pBaseObject));
        m_baseObjectList.insert(
                std::pair<unsigned int, BaseObject*>(classId, pBaseObject));
    } else {
        throw std::runtime_error(
                "[BaseObjectRegistry::registerModule] Module class name is already in the registry - Please check your class name = "
                        + pBaseObject->getClassName());
    }

    return classId;
} // mutex.unlock();

// Some modules depend of other so we need to make references at NULL and assign pointer later.
// Performed by the init() method of target module.
void BaseObjectRegistry::initBaseObject() {

    sf::Lock lock(m_mutex); // mutex.lock()

    for (m_itBaseObjectList = m_baseObjectList.begin();
            m_itBaseObjectList != m_baseObjectList.end();
            m_itBaseObjectList++) {
        if (m_itBaseObjectList->second) {
            (m_itBaseObjectList->second)->init();
        }
    }
} // mutex.unlock();

BaseObject* BaseObjectRegistry::get(unsigned int classId) {
    sf::Lock lock(m_mutex); // mutex.lock()

    return m_baseObjectList[classId];
} // mutex.unlock();

BaseObject* BaseObjectRegistry::get(const std::string &className) {
    sf::Lock lock(m_mutex); // mutex.lock()

    // if class name not available in the registry : throw exception
    if (!isAvailable(className)) {
        throw std::runtime_error(
                "[BaseObjectRegistry::get] Class name not registered in the registry ; spell check class name or available module for class name = "
                        + className);
    }

    return m_translate[className];
} // mutex.unlock();

bool BaseObjectRegistry::isAvailable(const std::string &className) {
    sf::Lock lock(m_mutex); // mutex.lock()

    m_itTranslate = m_translate.find(className);

    return (m_itTranslate == m_translate.end()) ? false : true;
} // mutex.unlock();

std::string BaseObjectRegistry::toString() {
    sf::Lock lock(m_mutex); // mutex.lock()

    ElemUtils::Formatter formatter;
    formatter << "[ModuleObjectFactory]" << '\n';
    for (m_itBaseObjectList = m_baseObjectList.begin();
            m_itBaseObjectList != m_baseObjectList.end();
            m_itBaseObjectList++) {
        if ((m_itBaseObjectList->second) != 0) {
            formatter << (m_itBaseObjectList->second)->toString() << '\n';
        }
    }

    return formatter.str();
} // mutex.unlock();

size_t BaseObjectRegistry::size() {
    sf::Lock lock(m_mutex); // mutex.lock()

    return m_baseObjectList.size();
} // mutex.unlock();
