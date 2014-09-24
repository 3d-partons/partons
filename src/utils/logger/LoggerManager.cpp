#include "LoggerManager.h"

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <utility>
#include <vector>

#include "../ini/IniFileParser.h"
#include "../stringUtils/Formatter.h"
#include "../stringUtils/StringUtils.h"

// Global static pointer used to ensure a single instance of the class.
LoggerManager* LoggerManager::m_pInstance = 0;

//TODO remplacer le path du fichier qui est en dur
LoggerManager::LoggerManager() :
        m_outputFilePath("default.log"), m_defaultLevel(LoggerLevel::INFO), m_printMode(
                LoggerPrintMode::COUT), m_active(false) {
}

LoggerManager* LoggerManager::getInstance() {
    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new LoggerManager();
    }

    return m_pInstance;
}

LoggerManager::~LoggerManager() {
    for (m_it = m_customClassLevels.begin(); m_it != m_customClassLevels.end();
            m_it++) {
        delete (m_it->second);
    }

    if (m_pInstance != 0) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

void LoggerManager::init(const std::string & configFilePath) {
    m_configFilePath = configFilePath;

    parseConfigurationFile();
}

//TODO ameliorer le parsing du fichier de config et les erreurs
void LoggerManager::parseConfigurationFile() {
    IniFileParser iniFileParser(m_configFilePath);
    iniFileParser.parse();

    std::map<std::string, std::string> data = iniFileParser.getValues();
    std::map<std::string, std::string>::iterator it;

    for (it = data.begin(); it != data.end(); it++) {
        if (StringUtils::equalsIgnoreCase(it->first, ENABLE_NAME)) {
            m_active = StringUtils::fromStringToBool(it->second);
        }
        // retrieve default level value
        else if (StringUtils::equalsIgnoreCase(it->first, DEFAULT_LEVEL_NAME)) {
            std::string defaultLevel = it->second;
            StringUtils::to_upperCase(defaultLevel);
            m_defaultLevel = LoggerLevel::fromString(defaultLevel);
        }
        // retrieve print mode
        else if (StringUtils::equalsIgnoreCase(it->first, PRINT_MODE_NAME)) {
            m_printMode = LoggerPrintMode::fromString(it->second);
        }
        // handle custom level
        else {
            std::vector<std::string> cache = StringUtils::split(it->first, '.');
            if (cache.size() > 1 && cache[0] == "logger") {
                std::string className = cache[1];
                std::string funcName = "";
                LoggerLevel customLevel = LoggerLevel::fromString(it->second);

                switch (cache.size()) {
                case 2: {
                    m_it = m_customClassLevels.find(className);

                    if (m_it != m_customClassLevels.end()) {
                        (m_it->second)->setDefaultClassLevel(customLevel);
                    } else {
                        m_customClassLevels.insert(
                                std::pair<std::string, LoggerClassLevel*>(
                                        className,
                                        new LoggerClassLevel(className,
                                                customLevel)));
                    }
                    break;
                }
                case 3: {
                    funcName = cache[2];

                    m_it = m_customClassLevels.find(className);

                    if (m_it == m_customClassLevels.end()) {
                        m_it = m_customClassLevels.insert(
                                std::pair<std::string, LoggerClassLevel*>(
                                        className,
                                        new LoggerClassLevel(className,
                                                LoggerLevel::NONE))).first;
                    }
                    (m_it->second)->add(funcName, customLevel);
                    break;
                }
                }
            }
        }
    }
}

void LoggerManager::stop() {
    m_active = false;
}

void LoggerManager::defineLevel(LoggerLevel loggerLevel) {
    m_defaultLevel = loggerLevel;
}

void LoggerManager::update() {
    while (isActive() || !m_messageQueue.empty()) {
        while (!m_messageQueue.empty()) {
            handleMessage(m_messageQueue.front());
            m_messageQueue.pop();
        }

        //TODO Implementations of sleep or usleep with SIGALRM are non-conformant, per POSIX
        usleep(30);
    }

    std::cout << "[LoggerManager] terminated ..." << std::endl;
}

bool LoggerManager::isLoggable(LoggerMessage loggerMessage) {
    bool result = true;

    // find if there is some custom policy for the desired class
    m_it = m_customClassLevels.find(loggerMessage.getClassNameSource());
    // if we find a result
    if (m_it != m_customClassLevels.end()) {
        LoggerLevel funcLevel = (m_it->second)->find(
                loggerMessage.getFunctionNameSource());
        // check if there is some custom policy for the desired function
        if (funcLevel.t_ != LoggerLevel::NONE) {
            if (loggerMessage.getLevel().t_
                    < (m_it->second)->find(
                            loggerMessage.getFunctionNameSource()).t_) {
                result = false;
            }
        }
        // else check if there is a custom default policy for the class
        else if ((m_it->second)->getDefaultClassLevel().t_
                != LoggerLevel::NONE) {
            if (loggerMessage.getLevel().t_
                    < (m_it->second)->getDefaultClassLevel().t_) {
                // if true then don't log message
                result = false;
            }

        }
        // if there is no default policy for the class use global policy
        else if (loggerMessage.getLevel().t_ < m_defaultLevel.t_) {
            result = false;
        }
    }
    // else use global policy for the test
    else if (loggerMessage.getLevel().t_ < m_defaultLevel.t_) {
        result = false;

    }

    return result;
}

//TODO implementer les autres sorties de logging
void LoggerManager::handleMessage(LoggerMessage loggerMessage) {

    if (isLoggable(loggerMessage)) {
        switch (m_printMode.getType()) {
        case LoggerPrintMode::COUT: {
            writeConsole(loggerMessage);
            break;
        }
        case LoggerPrintMode::FILE: {
            //writeFile(loggerMessage.getMessage());
            break;
        }
        case LoggerPrintMode::BOTH: {
            writeConsole(loggerMessage);
            //writeBoth(loggerMessage.getMessage());
            break;
        }
        default:
            break;
        }
    }
}

std::string LoggerManager::formatDate(time_t time) {
//tm dateFormatter = localtime(&time);

//    std::ostringstream os;
//    os << dateFormatter.tm_year << "-" << dateFormatter.tm_mon << "-"
//            << dateFormatter.tm_mday;
//
//    return os.str();

    return "";
}

void LoggerManager::writeConsole(LoggerMessage loggerMessage) {
//char* dt = ctime(&loggerMessage.getTime());

//    std::cout << formatDate(loggerMessage.getTime()) << " "
//            << loggerMessage.getClassNameSource() << "::"
//            << loggerMessage.getFunctionNameSource() << "() "
//            << loggerMessage.getMessage() << std::endl;

    std::cout << loggerMessage.getLevel().toString() << " ["
            << loggerMessage.getClassNameSource() << "::"

            << loggerMessage.getFunctionNameSource() << "]: "
            << loggerMessage.getMessage() << std::endl;
}

void* LoggerManager::run() {
    // Wait for the thread before terminate the main function
    pthread_join(getThreadId(), 0);
    update();
    return 0;
}

bool LoggerManager::isActive() {
    return m_active;
}

bool LoggerManager::isLoggableMessage(LoggerLevel loggerLevel) {
    return (loggerLevel.t_ >= m_defaultLevel.t_) ? true : false;
}

void LoggerManager::debug(const std::string & className,
        const std::string & functionName, const std::string & message) {
    addMessageToQueue(
            LoggerMessage(LoggerLevel::DEBUG, className, functionName,
                    message));
}
void LoggerManager::info(const std::string & className,
        const std::string & functionName, const std::string & message) {
    addMessageToQueue(
            LoggerMessage(LoggerLevel::INFO, className, functionName, message));

}
void LoggerManager::warn(const std::string & className,
        const std::string & functionName, const std::string & message) {
    addMessageToQueue(
            LoggerMessage(LoggerLevel::WARN, className, functionName, message));
}
void LoggerManager::error(const std::string & className,
        const std::string & functionName, const std::string & message) {
    addMessageToQueue(
            LoggerMessage(LoggerLevel::ERROR, className, functionName,
                    message));
}

//TODO remettre en place les mutex
void LoggerManager::addMessageToQueue(LoggerMessage loggerMessage) {
//m_mutex.lock();
    if (m_active == true) {
        m_messageQueue.push(loggerMessage);
    }
//m_mutex.unlock();
}

std::string LoggerManager::toString() {
    Formatter formatter;
    formatter << "ConfigFilePath = " << m_configFilePath << "\n";
    formatter << "DefaultLevel = " << m_defaultLevel.toString() << "\n";
    formatter << "PrintMode = " << m_printMode.toString() << "\n";

    for (m_it = m_customClassLevels.begin(); m_it != m_customClassLevels.end();
            m_it++) {
        formatter << m_it->first << " = "
                << (m_it->second)->getDefaultClassLevel().toString() << "\n";
    }

    return formatter;
}

