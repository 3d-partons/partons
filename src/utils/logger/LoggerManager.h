#ifndef LOGGER_MANAGER_H
#define LOGGER_MANAGER_H

/**
 * @file LoggerManager.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 05 September 2014
 * @version 2.0
 *
 * Last update : 22 September 2014
 *
 * @class LoggerManager
 * @brief \<singleton\> Use for handle logs.
 */

#include <ctime>
#include <map>
#include <queue>
#include <string>

#include "LoggerClassLevel.h"
#include "LoggerLevel.h"
#include "LoggerMessage.h"
#include "LoggerPrintMode.h"
#include "Thread.h"

const std::string ENABLE_NAME = "enable";
const std::string DEFAULT_LEVEL_NAME = "default.level";
const std::string PRINT_MODE_NAME = "print.mode";

class LoggerManager: public Thread {
public:
    /**
     * Share a unique pointer of this class
     */
    static LoggerManager* getInstance();
    ~LoggerManager();

    void init(const std::string & configFilePath);

    void terminate();

    void defineLevel(LoggerLevel loggerLevel);

    void writeConsole(LoggerMessage loggerMessage);

    bool isActive();

    void* run();

    void debug(const std::string & className, const std::string & functionName,
            const std::string & message);
    void info(const std::string & className, const std::string & functionName,
            const std::string & message);
    void warn(const std::string & className, const std::string & functionName,
            const std::string & message);
    void error(const std::string & className, const std::string & functionName,
            const std::string & message);

    bool isLoggableMessage(LoggerLevel loggerLevel);

    void addMessageToQueue(LoggerMessage loggerMessage);

    std::string toString();

private:
    /**
     * Private pointer of this class for a unique instance
     */
    static LoggerManager* m_pInstance;

    /**
     * Private default constructor for a unique instance
     */
    LoggerManager();

    std::string m_configFilePath;
    std::string m_outputFilePath;
    LoggerLevel m_defaultLevel;
    LoggerPrintMode m_printMode;

    std::map<std::string, LoggerClassLevel*> m_customClassLevels;
    std::map<std::string, LoggerClassLevel*>::iterator m_it;

    std::queue<LoggerMessage> m_messageQueue;

    bool m_active;

    void parseConfigurationFile();

    void update();

    void handleMessage(LoggerMessage loggerMessage);

    std::string formatDate(time_t time);

    bool isLoggable(LoggerMessage loggerMessage);
};

#endif /* LOGGER_MANAGER_H */
