#include "../../include/partons/BaseObject.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/thread/Packet.h>

#include "../../include/partons/BaseObjectFactory.h"
#include "../../include/partons/Partons.h"

unsigned int BaseObject::m_uniqueObjectIdCounter = 0;

BaseObject::BaseObject(const std::string &className) :
        m_objectId(getUniqueObjectId()), m_className(className), m_indexId(-1) {
}

BaseObject::BaseObject(const BaseObject& other) {
    m_objectId = getUniqueObjectId();
    m_className = other.m_className;
    m_indexId = other.m_indexId;

    debug(__func__,
            ElemUtils::Formatter() << "Object(" << getClassName()
                    << ") cloned with objectId(" << getObjectId() << ")");
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

void BaseObject::resolveObjectDependencies() {
    // Nothing to do
}

std::string BaseObject::toString() const {
    ElemUtils::Formatter formatter;

    formatter << "[" << getClassName() << "]\n";
    formatter << "m_className = " << m_className << " - " << "m_objectId = "
            << m_objectId << " indexId = " << m_indexId;

    return formatter.str();
}

unsigned int BaseObject::getUniqueObjectId() {
    return BaseObject::m_uniqueObjectIdCounter++;
}

void BaseObject::info(const std::string& functionName,
        const std::string& message) const {
    ElemUtils::LoggerManager* pLoggerManager =
            ElemUtils::LoggerManager::getInstance();

    pLoggerManager->info(getClassName(), functionName, message);
}

void BaseObject::debug(const std::string& functionName,
        const std::string& message) const {
    ElemUtils::LoggerManager* pLoggerManager =
            ElemUtils::LoggerManager::getInstance();

    pLoggerManager->debug(getClassName(), functionName, message);
}

void BaseObject::warn(const std::string& functionName,
        const std::string& message) const {
    ElemUtils::LoggerManager* pLoggerManager =
            ElemUtils::LoggerManager::getInstance();

    pLoggerManager->warn(getClassName(), functionName, message);
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

int BaseObject::getIndexId() const {
    return m_indexId;
}

void BaseObject::setIndexId(int indexId) {
    m_indexId = indexId;
}

bool BaseObject::operator <(const BaseObject& other) const {
    return (m_indexId < other.m_indexId)
            || (m_indexId == other.m_indexId && m_objectId < other.m_objectId);
}

void BaseObject::serialize(ElemUtils::Packet& packet) const {
    packet << m_className << m_indexId;
}

void BaseObject::unserialize(ElemUtils::Packet& packet) {
    packet >> m_className;
    packet >> m_indexId;
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet, BaseObject& object) {
    object.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet, BaseObject& object) {
    object.unserialize(packet);
    return packet;
}

void BaseObject::errorMissingParameter(const std::string& parameterName) const {
    throw ElemUtils::CustomException(m_className, __func__,
            ElemUtils::Formatter() << "Missing parameter name = "
                    << parameterName);
}
