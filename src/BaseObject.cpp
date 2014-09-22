#include "BaseObject.h"

#include "utils/logger/LoggerManager.h"

BaseObject::BaseObject(std::string className) :
        m_pLoggerManager(LoggerManager::getInstance()), m_className(className) {

}

BaseObject::~BaseObject() {

}

const std::string BaseObject::getClassName() {
    return m_className;
}
