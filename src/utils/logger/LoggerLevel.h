#ifndef LOGGER_LEVEL_H
#define LOGGER_LEVEL_H

/**
 * @file LoggerLevel.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 05 September 2014
 * @version 1.0
 *
 * Last update : 12 September 2014
 *
 * @class LoggerLevel
 * @brief
 */

#include <string>

struct LoggerLevel {
    //prevent automatic conversion for any other built-in types such as bool, int, etc
    template<typename T>
    operator T() const;

public:
    enum Type {
        DEBUG = 0, INFO = 1, WARN = 2, ERROR = 3, OFF = 4, NONE = 5
    };
    Type t_;
    LoggerLevel(Type t) :
            t_(t) {
    }
    operator Type() const {
        return t_;
    }

    std::string toString() {
        switch (t_) {
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

    static LoggerLevel fromString(const std::string &level) {
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
};

#endif /* LOGGER_LEVEL_H */
