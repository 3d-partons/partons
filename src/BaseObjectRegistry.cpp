/*
 * BaseObjectRegistry.cpp
 *
 *  Created on: Jun 25, 2015
 *      Author: debian
 */

#include "BaseObjectRegistry.h"

#include <stdexcept>
#include <utility>

// Global static pointer used to ensure a single instance of the class.
BaseObjectRegistry* BaseObjectRegistry::m_pInstance = 0;

unsigned int BaseObjectRegistry::m_uniqueClassIdCounter = 0;

BaseObjectRegistry::BaseObjectRegistry() {
    // TODO Auto-generated constructor stub

}

BaseObjectRegistry::~BaseObjectRegistry() {
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

    if (m_pInstance != 0) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

BaseObjectRegistry* BaseObjectRegistry::getInstance() {
    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new BaseObjectRegistry();
    }

    return m_pInstance;
}

unsigned int BaseObjectRegistry::getUniqueClassId() {
    return BaseObjectRegistry::m_uniqueClassIdCounter++;
}

unsigned int BaseObjectRegistry::registerBaseObject(BaseObject * pBaseObject) {
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
}

// Some modules depend of other so we need to make references at NULL and assign pointer later.
// Performed by the init() method of target module.
void BaseObjectRegistry::init() {
    for (m_itBaseObjectList = m_baseObjectList.begin();
            m_itBaseObjectList != m_baseObjectList.end();
            m_itBaseObjectList++) {
        if (m_itBaseObjectList->second) {
            (m_itBaseObjectList->second)->init();
        }
    }
}

//TODO check NULL pointer instead ?
BaseObject* BaseObjectRegistry::get(unsigned int classId) {
    return m_baseObjectList[classId];
}

BaseObject* BaseObjectRegistry::get(const std::string &className) {
    return m_translate[className];
}

//TODO
std::string BaseObjectRegistry::toString() {
//    std::ostringstream os;
//    os << "[ModuleObjectFactory]" << std::endl;
//    for (m_it = m_moduleRegistry.begin(); m_it != m_moduleRegistry.end();
//            m_it++) {
//        os << m_it->second->toString() << std::endl;
//    }
//
//    return os.str();

    return "";
}
