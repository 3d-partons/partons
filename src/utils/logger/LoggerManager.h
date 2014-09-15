#ifndef LOGGER_MANAGER_H
#define LOGGER_MANAGER_H

/**
 * @file LoggerManager.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 05 September 2014
 * @version 1.0
 *
 * Last update : 12 September 2014
 *
 * @class LoggerManager
 * @brief \<singleton\> Use for handle logs.
 */

#include <ctime>
#include <map>
#include <queue>
#include <string>

#include "../ini/ParserIniFile.h"
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

    void terminate();

    void defineLevel(LoggerLevel loggerLevel);

    void writeConsole(LoggerMessage loggerMessage);

    bool isActive();

    void* run();

    bool isDebug();
    bool isInfo();
    bool isWarn();
    bool isError();

    void debug(std::string className, std::string functionName,
            std::string message);
    void info(std::string className, std::string functionName,
            std::string message);
    void warn(std::string className, std::string functionName,
            std::string message);
    void error(std::string className, std::string functionName,
            std::string message);

    bool isLoggableMessage(LoggerLevel loggerLevel);

    void addMessageToQueue(LoggerMessage loggerMessage);

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

    ParserIniFile m_parserIni;

    bool m_active;

    void parseConfigurationFile();

    void update();

    void handleMessage(LoggerMessage loggerMessage);

    std::string formatDate(time_t time);

    bool isLoggable(LoggerLevel parentLevel, LoggerLevel childLevel);
    bool isLoggable(LoggerMessage loggerMessage);
};

#endif /* LOGGER_MANAGER_H */
