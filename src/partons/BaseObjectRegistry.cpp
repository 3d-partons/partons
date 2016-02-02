#include "../../include/partons/BaseObjectRegistry.h"

#include <SFML/System/Lock.hpp>
#include <SFML/System/Mutex.hpp>
#include <sstream>
#include <stdexcept>
#include <utility>

sf::Mutex BaseObjectRegistry::m_mutex;

// Global static pointer used to ensure a single instance of the class.
BaseObjectRegistry* BaseObjectRegistry::m_pInstance = 0;

unsigned int BaseObjectRegistry::m_uniqueClassIdCounter = 0;

BaseObjectRegistry::BaseObjectRegistry() {
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

void BaseObjectRegistry::delete_() {
    sf::Lock lock(m_mutex); // mutex.lock()

    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = 0;
    }
} // mutex.unlock();

BaseObjectRegistry* BaseObjectRegistry::getInstance() {
    sf::Lock lock(m_mutex); // mutex.lock()

    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new BaseObjectRegistry();
    }

    return m_pInstance;
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

//TODO check NULL pointer instead ?
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

    std::ostringstream os;
    os << "[ModuleObjectFactory]" << std::endl;
    for (m_itBaseObjectList = m_baseObjectList.begin();
            m_itBaseObjectList != m_baseObjectList.end();
            m_itBaseObjectList++) {
        if ((m_itBaseObjectList->second) != 0) {
            os << (m_itBaseObjectList->second)->toString() << std::endl;
        }
    }

    return os.str();
} // mutex.unlock();

size_t BaseObjectRegistry::size() const {
    sf::Lock lock(m_mutex); // mutex.lock()

    return m_baseObjectList.size();
} // mutex.unlock();
