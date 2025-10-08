#include "../../include/partons/BaseObjectRegistry.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <SFML/System/Lock.hpp>
#include <utility>

namespace PARTONS {


BaseObjectRegistry* BaseObjectRegistry::m_pInstance = 0;

unsigned int BaseObjectRegistry::m_uniqueClassIdCounter = 0;

BaseObjectRegistry* BaseObjectRegistry::getInstance() {
    if (!m_pInstance) {
        m_pInstance = new BaseObjectRegistry();
    }
    return m_pInstance;
}

BaseObjectRegistry::BaseObjectRegistry() {
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
    return BaseObjectRegistry::m_uniqueClassIdCounter++;
}

unsigned int BaseObjectRegistry::registerBaseObject(BaseObject * pBaseObject) {

    sf::Lock lock(m_mutex); // mutex.lock()

    // Defined a temporary class identifier
    unsigned int classId = 0;

    // First check if a previous instance of this class has been registered
    // If not, store it.
    if (!isAvailable(pBaseObject->getClassName())) {
        // Retrieve the final class identifier
        classId = getUniqueClassId();

        // Registered the new instance of the class in both registry lists.
        m_translateList.insert(
                std::pair<std::string, BaseObject*>(pBaseObject->getClassName(),
                        pBaseObject));
        m_baseObjectList.insert(
                std::pair<unsigned int, BaseObject*>(classId, pBaseObject));
        m_classIdByClassName.insert(
                std::pair<std::string, unsigned int>(
                        pBaseObject->getClassName(), classId));
    }
    // Else, throw an exception
    else {
        throw ElemUtils::CustomException("BaseObjectRegistry", __func__,
                ElemUtils::Formatter()
                        << "Module class name is already in the registry - Please check your class name = "
                        << pBaseObject->getClassName());
    }

    // Return the final class identifier to persist it into the static classId variable of the registered class.
    return classId;
} // mutex.unlock();

void BaseObjectRegistry::resolveBaseObjectDependencies() {
    sf::Lock lock(m_mutex); // mutex.lock()

    // Some objects depend of other so we need to make those references at NULL and assign pointers later.
    // It's performed by the resolveObjectDependencies() method of target object.
    // So loop over registry list to resolve objects dependancies
    for (std::map<unsigned int, BaseObject*>::const_iterator it =
            m_baseObjectList.begin(); it != m_baseObjectList.end(); it++) {
        if (it->second) {
            (it->second)->resolveObjectDependencies();
        } else {
            throw ElemUtils::CustomException("BaseObjectRegistry", __func__,
                    "Registry is corrupted some objects pointers are NULL");
        }
    }
} // mutex.unlock();

BaseObject* BaseObjectRegistry::get(unsigned int classId) const {
    sf::Lock lock(m_mutex); // mutex.lock()

    // if class identifier not found into the registry : throw exception
    BaseObject* pObject = isAvailable(classId);
    if (!pObject) {
        throw ElemUtils::CustomException("BaseObjectRegistry", __func__,
                "Cannot found class identifier into the registry");
    }

    return pObject;
} // mutex.unlock();

BaseObject* BaseObjectRegistry::get(const std::string &className) const {
    sf::Lock lock(m_mutex); // mutex.lock()

    // if class name not found into the registry : throw exception
    BaseObject* pObject = isAvailable(className);
    if (!pObject) {
        throw ElemUtils::CustomException("BaseObjectRegistry", __func__,
                ElemUtils::Formatter()
                        << "Cannot found class name into the registry ; spell check className parameter or if your object is available for className = "
                        << className);
    }

    return pObject;
} // mutex.unlock();

BaseObject* BaseObjectRegistry::isAvailable(
        const std::string &className) const {
    sf::Lock lock(m_mutex); // mutex.lock()

    std::map<std::string, BaseObject*>::const_iterator it =
            m_translateList.find(className);

    return (it == m_translateList.end()) ? 0 : it->second;
} // mutex.unlock();

BaseObject* BaseObjectRegistry::isAvailable(const unsigned int classId) const {
    sf::Lock lock(m_mutex); // mutex.lock()

    std::map<unsigned int, BaseObject*>::const_iterator it =
            m_baseObjectList.find(classId);

    return (it == m_baseObjectList.end()) ? 0 : it->second;
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

unsigned int BaseObjectRegistry::getObjectClassIdByClassName(
        const std::string& className) {
    std::map<std::string, unsigned int>::const_iterator it =
            m_classIdByClassName.find(className);

    return (it == m_classIdByClassName.end()) ? 0 : it->second;
}

List<std::string> BaseObjectRegistry::getBaseObjectClassNames() const {

    List<std::string> result;

    std::map<std::string, BaseObject*>::const_iterator it;

    for (it = m_translateList.begin(); it != m_translateList.end();  it++) {
        result.add(it->first);
    }

    return result;
}

} /* namespace PARTONS */
