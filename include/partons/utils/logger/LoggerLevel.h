#ifndef LOGGER_LEVEL_H
#define LOGGER_LEVEL_H

/**
 * @file LoggerLevel.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 05 September 2014
 * @version 1.0
 *
 * @class LoggerLevel
 * @brief
 */

#include <string>

#include "../stringUtils/StringUtils.h"

class LoggerLevel {
public:
    enum Type {
        DEBUG = 0, INFO = 1, WARN = 2, ERROR = 3, OFF = 4, NONE = 5
    };

    LoggerLevel(Type t)
            : m_type(t) {
    }

    std::string toString() {
        switch (m_type) {
        case DEBUG:
            return "DEBUG";
            break;
        case INFO:
            return "INFO";
            break;
        case WARN:
            return "WARN";
            break;
        case ERROR:
            return "ERROR";
            break;
        case OFF:
            return "OFF";
            break;
        default:
            return "NONE";
        }
    }

    static LoggerLevel fromString(std::string level) {
        StringUtils::to_upperCase(level);
        if (level == "DEBUG") {
            return DEBUG;
        } else if (level == "INFO") {
            return INFO;
        } else if (level == "WARN") {
            return WARN;
        } else if (level == "ERROR") {
            return ERROR;
        } else if (level == "OFF") {
            return OFF;
        }

        return NONE;
    }

    LoggerLevel::Type getType() const {
        return m_type;
    }

    void setType(Type type) {
        m_type = type;
    }

private:
    LoggerLevel::Type m_type;
};

#endif /* LOGGER_LEVEL_H */
