#include "LoggerClassLevel.h"

#include <utility>

LoggerClassLevel::LoggerClassLevel(const std::string& className,
        const LoggerLevel& loggerLevel)
        : m_className(className), m_defaultClassLevel(loggerLevel) {
}

LoggerClassLevel::~LoggerClassLevel() {

}

void LoggerClassLevel::add(const std::string& funcName,
        const LoggerLevel& funcLevel) {
    m_funcNames.insert(
            std::pair<std::string, LoggerLevel>(funcName, funcLevel));
}

const LoggerLevel LoggerClassLevel::find(const std::string& funcName) {
    m_it = m_funcNames.find(funcName);
    return (m_it != m_funcNames.end()) ?
            m_it->second : LoggerLevel(LoggerLevel::NONE);
}

const std::string& LoggerClassLevel::getClassName() const {
    return m_className;
}

void LoggerClassLevel::setClassName(const std::string& className) {
    m_className = className;
}

LoggerLevel LoggerClassLevel::getDefaultClassLevel() const {
    return m_defaultClassLevel;
}

void LoggerClassLevel::setDefaultClassLevel(LoggerLevel defaultClassLevel) {
    m_defaultClassLevel = defaultClassLevel;
}
