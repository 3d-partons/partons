#include "../../include/partons/BaseObjectRegistry.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <SFML/System/Lock.hpp>
#include <stdexcept>
#include <utility>

BaseObjectRegistry* BaseObjectRegistry::m_pInstance = 0;

BaseObjectRegistry* BaseObjectRegistry::getInstance() {
    if (!m_pInstance) {
        m_pInstance = new BaseObjectRegistry();
    }
    return m_pInstance;
}

BaseObjectRegistry::BaseObjectRegistry() :
        m_uniqueClassIdCounter(0) {
}

BaseObjectRegistry::~BaseObjectRegistry() {
    sf::Lock lock(m_mutex); // mutex.lock()

    // m_baseObjectList never delete'ed. (exist until program termination)
    // because we can't guarantee correct destruction order

    // loop over registry objects list to delete them
    for (std::map<unsigned int, BaseObject*>::iterator it =
            m_baseObjectList.begin(); it != m_baseObjectList.end(); it++) {
        if (it->second) {
            delete (it->second);
            (it->second) = 0;
        }
    }
} // mutex.unlock();

unsigned int BaseObjectRegistry::getUniqueClassId() {
    return m_uniqueClassIdCounter++;
}

unsigned int BaseObjectRegistry::registerBaseObject(BaseObject * pBaseObject) {

    sf::Lock lock(m_mutex); // mutex.lock()

    // Defined a temporary class identifier
    unsigned int classId = 0;

    // First check if a previous instance of this class has been registered
    m_itTranslate = m_translateList.find(pBaseObject->getClassName());

    // If not, store it.
    if (m_itTranslate == m_translateList.end()) {
        // Retrieve the final class identifier
        classId = getUniqueClassId();

        // Registered the new instance of the class in both registry lists.
        m_translateList.insert(
                std::pair<std::string, BaseObject*>(pBaseObject->getClassName(),
                        pBaseObject));
        m_baseObjectList.insert(
                std::pair<unsigned int, BaseObject*>(classId, pBaseObject));
    }
    // Else, throw an exception
    else {
        throw std::runtime_error(
                ElemUtils::Formatter()
                        << "(BaseObjectRegistry::registerModule) Module class name is already in the registry - Please check your class name = "
                        << pBaseObject->getClassName());
    }

    // Return the final class identifier to persist it into the static classId variable of the registered class.
    return classId;
} // mutex.unlock();

void BaseObjectRegistry::initBaseObject() {
    sf::Lock lock(m_mutex); // mutex.lock()

    // Some objects depend of other so we need to make those references at NULL and assign pointers later.
    // It's performed by the init() method of target object.
    // So loop over registry list to resolve objects dependancies
    for (std::map<unsigned int, BaseObject*>::const_iterator it =
            m_baseObjectList.begin(); it != m_baseObjectList.end(); it++) {
        if (it->second) {
            (it->second)->init();
        } else {
            throw std::runtime_error(
                    ElemUtils::Formatter()
                            << "(BaseObjectRegistry::initBaseObject) Registry is corrupted some objects pointers are NULL");
        }
    }
} // mutex.unlock();

BaseObject* BaseObjectRegistry::get(unsigned int classId) const {
    sf::Lock lock(m_mutex); // mutex.lock()

    // if class identifier not found into the registry : throw exception
    if (!isAvailable(classId)) {
        throw std::runtime_error(
                "(BaseObjectRegistry::get) Cannot found class identifier into the registry");
    }

    return getLastAvailableObjectIdentifiedByClassId();
} // mutex.unlock();

BaseObject* BaseObjectRegistry::get(const std::string &className) const {
    sf::Lock lock(m_mutex); // mutex.lock()

    // if class name not found into the registry : throw exception
    if (!isAvailable(className)) {
        throw std::runtime_error(
                "(BaseObjectRegistry::get) Cannot found class name into the registry ; spell check className parameter or if your object is available for className = "
                        + className);
    }

    return getLastAvailableObjectIdentifiedByClassName();
} // mutex.unlock();

bool BaseObjectRegistry::isAvailable(const std::string &className) const {
    sf::Lock lock(m_mutex); // mutex.lock()

    m_itTranslate = m_translateList.find(className);

    return (m_itTranslate == m_translateList.end()) ? false : true;
} // mutex.unlock();

bool BaseObjectRegistry::isAvailable(const unsigned int classId) const {
    sf::Lock lock(m_mutex); // mutex.lock()

    m_itBaseObjectList = m_baseObjectList.find(classId);

    return (m_itBaseObjectList == m_baseObjectList.end()) ? false : true;
} // mutex.unlock();

std::string BaseObjectRegistry::toString() const {
    sf::Lock lock(m_mutex); // mutex.lock()

    ElemUtils::Formatter formatter;
    formatter << "[ModuleObjectFactory]" << '\n';
    for (std::map<unsigned int, BaseObject*>::const_iterator it =
            m_baseObjectList.begin(); it != m_baseObjectList.end(); it++) {
        if ((it->second) != 0) {
            formatter << (it->second)->toString() << '\n';
        }
    }

    return formatter.str();
} // mutex.unlock();

size_t BaseObjectRegistry::size() const {
    sf::Lock lock(m_mutex); // mutex.lock()

    return m_baseObjectList.size();
} // mutex.unlock();

BaseObject* BaseObjectRegistry::getLastAvailableObjectIdentifiedByClassName() const {
    return m_itTranslate->second;
}

BaseObject* BaseObjectRegistry::getLastAvailableObjectIdentifiedByClassId() const {
    return m_itBaseObjectList->second;
}
