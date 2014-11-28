#include "BaseObject.h"

#include <string>

#include "modules/ModuleObject.h"
#include "utils/logger/LoggerManager.h"
#include "utils/stringUtils/Formatter.h"

unsigned int BaseObject::uniqueID = 0;

BaseObject::BaseObject(std::string className)
        : m_pLoggerManager(LoggerManager::getInstance()), m_objectId(
                getUniqueID()), m_className(className) {

}

BaseObject::BaseObject(const BaseObject& other) {
    m_objectId = getUniqueID();
    m_className = other.m_className;
    m_pLoggerManager = LoggerManager::getInstance();
}
BaseObject* BaseObject::clone() const {
    return new BaseObject(*this);
}

BaseObject::~BaseObject() {

}

const std::string& BaseObject::getClassName() const {
    return m_className;
}

void BaseObject::setClassName(const std::string& className) {
    m_className = className;
}

unsigned int BaseObject::getUniqueID() {
    return ModuleObject::uniqueID++;
}

unsigned int BaseObject::getObjectId() const {
    return m_objectId;
}

void BaseObject::setObjectId(unsigned int objectId) {
    m_objectId = objectId;
}

std::string BaseObject::toString() {
    return Formatter() << "m_className = " << m_className << " - " << "m_objectId = " << m_objectId;
}
