#include "BaseObject.h"

//#include <iostream>

#include "utils/logger/LoggerManager.h"

BaseObject::BaseObject(std::string className) :
        m_pLoggerManager(LoggerManager::getInstance()), m_className(className) {

}

BaseObject::BaseObject(const BaseObject& other) {
    m_className = other.getClassName();
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
