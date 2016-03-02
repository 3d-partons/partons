#include "../../include/partons/BaseObject.h"

#include <stdexcept>

#include "../../include/partons/BaseObjectFactory.h"
#include "../../include/partons/Partons.h"
#include "../../include/partons/utils/logger/LoggerManager.h"
#include "../../include/partons/utils/stringUtils/Formatter.h"

unsigned int BaseObject::m_uniqueObjectIdCounter = 0;

BaseObject::BaseObject(const std::string &className) :
        m_objectId(getUniqueObjectId()), m_className(className) {
}

BaseObject::BaseObject(const BaseObject& other) {
    m_objectId = getUniqueObjectId();
    m_className = other.m_className;
}
BaseObject* BaseObject::clone() const {
    return new BaseObject(*this);
}

BaseObject::~BaseObject() {
    // Nothing to destroy

    //Self removing from factory store if previously created by it
    Partons::getInstance()->getBaseObjectFactory()->removeFromStore(
            getObjectId());
}

void BaseObject::init() {
    // Nothing to do
}

std::string BaseObject::toString() const {
    Formatter formatter;

    formatter << "m_className = " << m_className << " - " << "m_objectId = "
            << m_objectId << '\n';

    return formatter.str();
}

unsigned int BaseObject::getUniqueObjectId() {
    return BaseObject::m_uniqueObjectIdCounter++;
}

void BaseObject::info(const std::string& functionName,
        const std::string& message) const {
    LoggerManager* pLoggerManager = LoggerManager::getInstance();

    pLoggerManager->info(getClassName(), functionName, message);
}

void BaseObject::debug(const std::string& functionName,
        const std::string& message) const {
    LoggerManager* pLoggerManager = LoggerManager::getInstance();

    pLoggerManager->debug(getClassName(), functionName, message);
}

void BaseObject::warn(const std::string& functionName,
        const std::string& message) const {
    LoggerManager* pLoggerManager = LoggerManager::getInstance();

    pLoggerManager->warn(getClassName(), functionName, message);
}

//TODO remove error or throwException function from BaseObject class
void BaseObject::error(const std::string& functionName,
        const std::string& message) const {
    LoggerManager* pLoggerManager = LoggerManager::getInstance();

    pLoggerManager->error(getClassName(), functionName, message);

    throwException(Formatter() << functionName << "[" << m_objectId << "]",
            message);
}

void BaseObject::throwException(const std::string functionName,
        const std::string errorMessage) const {
    throw std::runtime_error(
            Formatter() << "[" << getClassName() << "::" << functionName << "] "
                    << errorMessage);
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
