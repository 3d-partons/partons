#include "LoggerManager.h"

#include <iostream>
#include <utility>
#include <vector>

#include "../StringUtils.h"

// Global static pointer used to ensure a single instance of the class.
LoggerManager* LoggerManager::m_pInstance = 0;

//TODO remplacer le path du fichier qui est en dur
LoggerManager::LoggerManager() :
        m_configFilePath(
                "/home/bryan/Documents/workspace/PARTONS/data/logger.cfg"), m_outputFilePath(
                "default.log"), m_defaultLevel(LoggerLevel::INFO), m_printMode(
                LoggerPrintMode::COUT), m_active(true) {
    parseConfigurationFile();
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
}

//TODO ameliorer le parsing du fichier de config et les erreurs
void LoggerManager::parseConfigurationFile() {
    vectorIni data = m_parserIni.parse_ini_file(m_configFilePath);

    for (unsigned i = 0; i != data.size(); i++) {
        if (StringUtils::equalsIgnoreCase(data[i].first, ENABLE_NAME)) {
            m_active = StringUtils::fromStringToBool(data[i].second);
        }
        // retrieve default level value
        else if (StringUtils::equalsIgnoreCase(data[i].first,
                DEFAULT_LEVEL_NAME)) {
            std::string defaultLevel = data[i].second;
            StringUtils::to_upperCase(defaultLevel);
            m_defaultLevel = LoggerLevel::fromString(defaultLevel);
        }
        // retrieve print mode
        else if (StringUtils::equalsIgnoreCase(data[i].first,
                PRINT_MODE_NAME)) {
            std::string printMode = data[i].second;
            StringUtils::to_upperCase(printMode);
            m_printMode = LoggerPrintMode::fromString(printMode);
        }
        // handle custom level
        else {
            std::vector<std::string> cache = StringUtils::split(data[i].first,
                    '.');
            if (cache.size() > 1 && cache[0] == "logger") {
                std::string className = cache[1];
                std::string funcName = "";
                std::string tempLevel = data[i].second;
                StringUtils::to_upperCase(tempLevel);
                LoggerLevel customLevel = LoggerLevel::fromString(tempLevel);

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

//    for (m_it = m_customClassLevels.begin(); m_it != m_customClassLevels.end();
//            m_it++) {
//        std::cerr << "className = " << (m_it->second)->getClassName()
//                << " defaultPolicy = "
//                << (m_it->second)->getDefaultClassLevel().toString()
//                << std::endl;
//    }
}

void LoggerManager::terminate() {
    m_active = false;
}

void LoggerManager::defineLevel(LoggerLevel loggerLevel) {
    m_defaultLevel = loggerLevel;
}

void LoggerManager::update() {
    while (isActive()) {
        while (!m_messageQueue.empty()) {
            handleMessage(m_messageQueue.front());
            m_messageQueue.pop();
        }
    }

    std::cout << "[LoggerManager] terminated ..." << std::endl;
}

bool LoggerManager::isLoggable(LoggerLevel parentLevel,
        LoggerLevel childLevel) {
    return (childLevel.t_ <= parentLevel.t_) ? true : false;
}

bool LoggerManager::isLoggable(LoggerMessage loggerMessage) {
    bool result = true;

//    // find if there is some custom policy for the desired class
//    m_it = m_customClassLevels.find(loggerMessage.getClassNameSource());
//    // if we find a result
//    if (m_it != m_customClassLevels.end()) {
//        // and default class's policy equals DEBUG
//        if ((m_it->second)->getDefaultClassLevel().t_ == LoggerLevel::DEBUG) {
//            // log all message from this class
//            result = true;
//        }
//        // else
//        else {
//            // we retrieve the custom policy for the desired class's function
//            LoggerLevel funcLevel = (m_it->second)->find(
//                    loggerMessage.getFunctionNameSource());
//            // if function's policy equals DEBUG
//            if (funcLevel.t_ == LoggerLevel::DEBUG) {
//                result = true;
//            } else if (isLoggable((m_it->second)->getDefaultClassLevel(),
//                    funcLevel)) {
//                result = true;
//            }
//        }
//    }

// ONLY FOR FILTER CLASS POLICY

// find if there is some custom policy for the desired class
    m_it = m_customClassLevels.find(loggerMessage.getClassNameSource());
    // if we find a result
    if (m_it != m_customClassLevels.end()) {

        // check if message's policy < class's policy
        LoggerLevel classLevel = (m_it->second)->getDefaultClassLevel();
        if (loggerMessage.getLevel().t_ < classLevel.t_) {
            // if true then don't log message
            result = false;
        }

//        // looking for function's policy
//        LoggerLevel funcLevel = (m_it->second)->find(
//                loggerMessage.getFunctionNameSource());
//
//        // if NONE
//        if (funcLevel.t_ == LoggerLevel::NONE) {
//            // retrieve class's policy
//            LoggerLevel classLevel = (m_it->second)->getDefaultClassLevel();
//
//            if (classLevel.t_ != LoggerLevel::NONE) {
//                // if message's policy < class's default level
//                if (loggerMessage.getLevel().t_ < classLevel.t_) {
//                    // don't log message
//                    result = false;
//                }
//            }
//
//            // else keep logging message
//        }
    }

    return result;
}

void LoggerManager::handleMessage(LoggerMessage loggerMessage) {

    if (isLoggable(loggerMessage)) {
        switch (m_printMode.t_) {
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
    update();

    return 0;
}

bool LoggerManager::isActive() {
    return m_active;
}

bool LoggerManager::isLoggableMessage(LoggerLevel loggerLevel) {
    return (loggerLevel.t_ >= m_defaultLevel.t_) ? true : false;
}

void LoggerManager::debug(std::string className, std::string functionName,
        std::string message) {
    addMessageToQueue(
            LoggerMessage(LoggerLevel::DEBUG, className, functionName,
                    message));

}
void LoggerManager::info(std::string className, std::string functionName,
        std::string message) {
    addMessageToQueue(
            LoggerMessage(LoggerLevel::INFO, className, functionName, message));

}
void LoggerManager::warn(std::string className, std::string functionName,
        std::string message) {
    addMessageToQueue(
            LoggerMessage(LoggerLevel::WARN, className, functionName, message));
}
void LoggerManager::error(std::string className, std::string functionName,
        std::string message) {
    addMessageToQueue(
            LoggerMessage(LoggerLevel::ERROR, className, functionName,
                    message));
}

void LoggerManager::addMessageToQueue(LoggerMessage loggerMessage) {
//m_mutex.lock();
    m_messageQueue.push(loggerMessage);
//m_mutex.unlock();
}

bool LoggerManager::isDebug() {
    if (isActive()) {
        return isLoggableMessage(LoggerLevel::DEBUG);
    }
    return false;
}

bool LoggerManager::isInfo() {
    if (isActive()) {
        return isLoggableMessage(LoggerLevel::INFO);
    }
    return false;
}

bool LoggerManager::isWarn() {
    if (isActive()) {
        return isLoggableMessage(LoggerLevel::WARN);
    }
    return false;

}

bool LoggerManager::isError() {
    if (isActive()) {
        return isLoggableMessage(LoggerLevel::ERROR);
    }
    return false;

}

