#ifndef LOGGER_MESSAGE_H
#define LOGGER_MESSAGE_H

/**
 * @file LoggerMessage.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 05 September 2014
 * @version 1.0
 *
 * @class LoggerMessage
 * @brief
 */

#include <ctime>
#include <string>

#include "LoggerLevel.h"

class LoggerMessage {
public:
    LoggerMessage(LoggerLevel level, std::string classNameSource,
            std::string functionNameSource, std::string message);
    ~LoggerMessage();

    const std::time_t& getTime() const;
    void setTime(std::time_t time);
    const std::string& getClassNameSource() const;
    void setClassNameSource(const std::string& classNameSource);
    const std::string& getFunctionNameSource() const;
    void setFunctionNameSource(const std::string& functionNameSource);
    const std::string& getMessage() const;
    void setMessage(const std::string& message);
    LoggerLevel getLevel() const;
    void setLevel(LoggerLevel level);

private:
    LoggerLevel m_level;
    std::time_t m_time;
    std::string m_classNameSource;
    std::string m_functionNameSource;
    std::string m_message;
};

#endif /* LOGGER_MESSAGE_H */
