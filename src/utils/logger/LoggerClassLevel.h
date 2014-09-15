#ifndef LOGGER_CLASS_LEVEL_H
#define LOGGER_CLASS_LEVEL_H

/**
 * @file LoggerClassLevel.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 12 September 2014
 * @version 1.0
 *
 * Last update : 12 September 2014
 *
 * @class LoggerClassLevel
 * @brief
 */

#include <map>
#include <string>

#include "LoggerLevel.h"

class LoggerClassLevel {
public:
    LoggerClassLevel(const std::string& className,
            const LoggerLevel& loggerLevel);
    ~LoggerClassLevel();

    void add(const std::string& funcName, const LoggerLevel& funcLevel);
    const LoggerLevel find(const std::string& funcName);

    const std::string& getClassName() const;
    void setClassName(const std::string& className);
    LoggerLevel getDefaultClassLevel() const;
    void setDefaultClassLevel(LoggerLevel defaultClassLevel);

private:
    std::string m_className;
    LoggerLevel m_defaultClassLevel;
    std::map<std::string, LoggerLevel> m_funcNames;
    std::map<std::string, LoggerLevel>::iterator m_it;
};

#endif /* LOGGER_CLASS_LEVEL_H */
