#include "BaseObject.h"

#include <stdexcept>
#include <string>

#include "BaseObjectManager.h"
#include "modules/ModuleObject.h"
#include "utils/logger/LoggerManager.h"
#include "utils/stringUtils/Formatter.h"

unsigned int BaseObject::uniqueID = 0;

BaseObject::BaseObject(const std::string &className) :
        /*m_pBaseObjectManager(BaseObjectManager::getInstance()),*/m_pLoggerManager(
                LoggerManager::getInstance()), m_objectId(getUniqueID()), m_className(
                className) {
    //m_pBaseObjectManager->add(this);
    BaseObjectManager::getInstance()->add(this);
}

BaseObject::BaseObject(const BaseObject& other) {
    m_objectId = getUniqueID();
    m_className = other.m_className;
    m_pLoggerManager = other.m_pLoggerManager;

    // m_pBaseObjectManager->add(this);
    BaseObjectManager::getInstance()->add(this);
}
BaseObject* BaseObject::clone() const {
    return new BaseObject(*this);
}

BaseObject::~BaseObject() {

}

void BaseObject::throwException(const std::string &functionName,
        const std::string &errorMessage) {
    m_pLoggerManager->debug(getClassName(), functionName, errorMessage);
    throw std::runtime_error(
            Formatter() << "[" << getClassName() << "::" << functionName << "] "
                    << errorMessage);
}

std::string BaseObject::toString() {
    return Formatter() << "m_className = " << m_className << " - "
            << "m_objectId = " << m_objectId;
}

const std::string& BaseObject::getClassName() const {
    return m_className;
}

void BaseObject::setClassName(const std::string& className) {
    m_className = className;
}

unsigned int BaseObject::getUniqueID() {
    return BaseObject::uniqueID++;
}

unsigned int BaseObject::getObjectId() const {
    return m_objectId;
}

void BaseObject::setObjectId(unsigned int objectId) {
    m_objectId = objectId;
}
