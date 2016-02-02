#ifndef LOGGER_MANAGER_H
#define LOGGER_MANAGER_H

/**
 * @file LoggerManager.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 05 September 2014
 * @version 2.0
 *
 * @class LoggerManager
 * @brief \<singleton\> Use for handle logs.
 */

#include <SFML/System/Mutex.hpp>
#include <ctime>
#include <map>
#include <queue>
#include <string>

#include "../thread/Thread.h"
#include "LoggerClassLevel.h"
#include "LoggerLevel.h"
#include "LoggerMessage.h"
#include "LoggerPrintMode.h"

//TODO relatif folder for configFile

const std::string ENABLE_NAME = "enable";
const std::string DEFAULT_LEVEL_NAME = "default.level";
const std::string PRINT_MODE_NAME = "print.mode";
const std::string OUTPUT_LOG_FILE_PATH = "log.file.path";

class LoggerManager: public Thread {
public:
    /**
     * Private default constructor for a unique instance
     */
    LoggerManager();

    /**
     * Default destructor
     */
    virtual ~LoggerManager();

    void init();

    bool isEmptyMessageQueue();

    void close();

    void defineLevel(LoggerLevel loggerLevel);

    void run();

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
    sf::Mutex m_mutex;

    std::string m_outputFilePath;
    LoggerLevel m_defaultLevel;
    LoggerPrintMode m_printMode;

    std::map<std::string, LoggerClassLevel*> m_customClassLevels;
    std::map<std::string, LoggerClassLevel*>::iterator m_it;

    std::queue<LoggerMessage> m_messageQueue;

    bool m_active;

    void parseConfigurationFile(const std::string &filePath);

    void update();

    void handleMessage(LoggerMessage loggerMessage);

    std::string formatDate(time_t time);

    bool isLoggable(LoggerMessage loggerMessage);

    void writeConsole(const std::string &message);
    void writeFile(const std::string &message);
};

#endif /* LOGGER_MANAGER_H */
