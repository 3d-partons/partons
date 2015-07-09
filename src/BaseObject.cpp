#include "BaseObject.h"

#include <stdexcept>

#include "BaseObjectManager.h"
#include "utils/logger/LoggerManager.h"
#include "utils/stringUtils/Formatter.h"

unsigned int BaseObject::m_uniqueObjectIdCounter = 0;

BaseObject::BaseObject(const std::string &className) :
        m_pLoggerManager(LoggerManager::getInstance()), m_objectId(
                getUniqueObjectId()), m_className(className) {
    // Use to self registering class object
    BaseObjectManager::getInstance()->add(this);
}

BaseObject::BaseObject(const BaseObject& other) {
    m_objectId = getUniqueObjectId();
    m_className = other.m_className;
    m_pLoggerManager = other.m_pLoggerManager;

    // Use to self registering copied class object
    BaseObjectManager::getInstance()->add(this);
}
BaseObject* BaseObject::clone() const {
    return new BaseObject(*this);
}

BaseObject::~BaseObject() {
// Nothing to destroy
}

// TODO replace all throw exception by this function call
void BaseObject::throwException(const std::string &functionName,
        const std::string &errorMessage) const {
    if (m_pLoggerManager != 0) {
        m_pLoggerManager->error(getClassName(), functionName, errorMessage);
    }
    throw std::runtime_error(
            Formatter() << "[" << getClassName() << "::" << functionName << "] "
                    << errorMessage);
}

void BaseObject::init() {

}

std::string BaseObject::toString() {
    return Formatter() << "m_className = " << m_className << " - "
            << "m_objectId = " << m_objectId;
}

unsigned int BaseObject::getUniqueObjectId() {
    return BaseObject::m_uniqueObjectIdCounter++;
}

/* ===== GETTERS & SETTERS ===== */

const std::string& BaseObject::getClassName() const {
    return m_className;
}

unsigned int BaseObject::getObjectId() const {
    return m_objectId;
}

void BaseObject::setObjectId(unsigned int objectId) {
    m_objectId = objectId;
}

