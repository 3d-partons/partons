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
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "../thread/Thread.h"
#include "LoggerClassLevel.h"
#include "LoggerLevel.h"
#include "LoggerPrintMode.h"

class LoggerMessage;

//TODO relatif folder for configFile

const std::string ENABLE_NAME = "enable";
const std::string DEFAULT_LEVEL_NAME = "default.level";
const std::string PRINT_MODE_NAME = "print.mode";
const std::string OUTPUT_LOG_FILE_PATH = "log.file.path";

class LoggerManager: public Thread {
public:
    static LoggerManager* getInstance();

    /**
     * Default destructor
     */
    virtual ~LoggerManager();

    void init();

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

    void addMessageToBuffer(LoggerMessage loggerMessage);

    std::string toString();

    void flushBuffer();

private:
    static LoggerManager* m_pInstance;

    /**
     * Private default constructor for a unique instance
     */
    LoggerManager();

    sf::Mutex m_mutex;

    std::ofstream m_fileOutputStream;
    std::string m_outputFilePath;
    LoggerLevel m_defaultLevel;
    LoggerPrintMode m_printMode;

    std::map<std::string, LoggerClassLevel*> m_customClassLevels;
    std::map<std::string, LoggerClassLevel*>::iterator m_it;

    std::string m_buffer;

    bool m_active;

    void parseConfigurationFile(const std::string &filePath);

    void update();

    bool isLoggable(LoggerMessage loggerMessage);

    void writeConsole();
    void writeFile();
};

#endif /* LOGGER_MANAGER_H */
