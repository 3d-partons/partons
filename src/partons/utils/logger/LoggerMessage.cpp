#include "../../../../include/partons/utils/logger/LoggerMessage.h"

#include <sstream>

LoggerMessage::LoggerMessage(LoggerLevel level, std::string classNameSource,
        std::string functionNameSource, std::string message) :
        m_level(level), m_time(std::time(0)), m_classNameSource(
                classNameSource), m_functionNameSource(functionNameSource), m_message(
                message) {
}

LoggerMessage::~LoggerMessage() {
}

const std::time_t& LoggerMessage::getTime() const {
    return m_time;
}

void LoggerMessage::setTime(std::time_t time) {
    m_time = time;
}

const std::string& LoggerMessage::getClassNameSource() const {
    return m_classNameSource;
}

void LoggerMessage::setClassNameSource(const std::string& classNameSource) {
    m_classNameSource = classNameSource;
}

const std::string& LoggerMessage::getFunctionNameSource() const {
    return m_functionNameSource;
}

void LoggerMessage::setFunctionNameSource(
        const std::string& functionNameSource) {
    m_functionNameSource = functionNameSource;
}

const std::string& LoggerMessage::getMessage() const {
    return m_message;
}

void LoggerMessage::setMessage(const std::string& message) {
    m_message = message;
}

LoggerLevel LoggerMessage::getLevel() const {
    return m_level;
}

void LoggerMessage::setLevel(LoggerLevel level) {
    m_level = level;
}

std::string LoggerMessage::formatDate(const time_t &time) const {

    // see : http://stackoverflow.com/questions/16357999/current-date-and-time-as-string

    struct tm* timeinfo;
    char buffer[80];
    timeinfo = localtime(&time);

    strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeinfo);

    return std::string(buffer);
}

std::string LoggerMessage::toString() const {
    std::stringstream sstream;
    sstream << formatDate(m_time) << " [" << m_level.toString() << "] ("
            << m_classNameSource << "::" << m_functionNameSource << ") "
            << m_message;
    return sstream.str();
}
