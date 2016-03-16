#include "../../../../include/partons/utils/logger/LoggerManager.h"

#include <SFML/System/Lock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <stdexcept>
#include <utility>
#include <vector>

#include "../../../../include/partons/utils/fileUtils/FileUtils.h"
#include "../../../../include/partons/utils/logger/LoggerMessage.h"
#include "../../../../include/partons/utils/parser/IniFileParser.h"
#include "../../../../include/partons/utils/PropertiesManager.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"
#include "../../../../include/partons/utils/stringUtils/StringUtils.h"

LoggerManager* LoggerManager::m_pInstance = 0;

LoggerManager* LoggerManager::getInstance() {
    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new LoggerManager();
    }
    return m_pInstance;
}

//TODO remplacer le nom de la propriété "log.file.path" par un static final string
LoggerManager::LoggerManager() :
        Thread(), m_outputFilePath("default.log"), m_defaultLevel(
                LoggerLevel::INFO), m_printMode(LoggerPrintMode::COUT), m_active(
                true) {
}

LoggerManager::~LoggerManager() {
    sf::Lock lock(m_mutex); // mutex.lock()

    // flush remaining buffer
    flushBuffer();

    std::cerr << "(LoggerManager::flushBuffer()) Buffer flushed" << std::endl;

    //TODO close file when exception throw
    FileUtils::close(m_fileOutputStream);

    for (m_it = m_customClassLevels.begin(); m_it != m_customClassLevels.end();
            m_it++) {
        if (m_it->second) {
            delete (m_it->second);
            (m_it->second) = 0;
        }
    }
} // mutex.unlock();

void LoggerManager::init() {
    parseConfigurationFile(
            PropertiesManager::getInstance()->getString("log.file.path"));
}

//TODO ameliorer le parsing du fichier de config et les erreurs
void LoggerManager::parseConfigurationFile(const std::string &filePath) {
    IniFileParser iniFileParser;
    iniFileParser.parse(filePath);

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
        // retrieve path for log file
        else if (StringUtils::equalsIgnoreCase(it->first,
                OUTPUT_LOG_FILE_PATH)) {
            m_outputFilePath = (it->second);
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

void LoggerManager::close() {
    sf::Lock lock(m_mutex); // mutex.lock()

    m_active = false;
} // mutex.unlock()

void LoggerManager::defineLevel(LoggerLevel loggerLevel) {
    sf::Lock lock(m_mutex); // mutex.lock()

    m_defaultLevel = loggerLevel;
} // mutex.unlock()

void LoggerManager::update() {
    bool fileOpened = FileUtils::open(m_fileOutputStream, m_outputFilePath);

    if (fileOpened) {
        while (m_active) {
            flushBuffer();
            sf::sleep(sf::milliseconds(30));
        }
    } else {
        throw std::runtime_error(
                Formatter() << "(LoggerManager::update) Cannot open log file = "
                        << m_outputFilePath << "\n"
                        << "LoggerManager terminated !");
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
        if (funcLevel.getType() != LoggerLevel::NONE) {
            if (loggerMessage.getLevel().getType()
                    < (m_it->second)->find(
                            loggerMessage.getFunctionNameSource()).getType()) {
                result = false;
            }
        }
        // else check if there is a custom default policy for the class
        else if ((m_it->second)->getDefaultClassLevel().getType()
                != LoggerLevel::NONE) {
            if (loggerMessage.getLevel().getType()
                    < (m_it->second)->getDefaultClassLevel().getType()) {
                // if true then don't log message
                result = false;
            }

        }
        // if there is no default policy for the class use global policy
        else if (loggerMessage.getLevel().getType()
                < m_defaultLevel.getType()) {
            result = false;
        }
    }
    // else use global policy for the test
    else if (loggerMessage.getLevel().getType() < m_defaultLevel.getType()) {
        result = false;

    }

    return result;
}

void LoggerManager::flushBuffer() {
    sf::Lock lock(m_mutex); // mutex.lock()

    if (!m_buffer.empty()) {

        switch (m_printMode.getType()) {
        case LoggerPrintMode::COUT: {
            writeConsole();
            break;
        }
        case LoggerPrintMode::FILE: {
            writeFile();
            break;
        }
        case LoggerPrintMode::BOTH: {
            writeFile();
            writeConsole();
            break;
        }
        default:
            break;
        }

        m_buffer.clear();
    }
} // mutex.unlock()

void LoggerManager::writeConsole() {
    std::cout << m_buffer;
}

void LoggerManager::writeFile() {
    FileUtils::write(m_fileOutputStream, m_buffer);
}

void LoggerManager::run() {
    update();
}

void LoggerManager::debug(const std::string & className,
        const std::string & functionName, const std::string & message) {
    addMessageToBuffer(
            LoggerMessage(LoggerLevel::DEBUG, className, functionName,
                    message));
}
void LoggerManager::info(const std::string & className,
        const std::string & functionName, const std::string & message) {
    addMessageToBuffer(
            LoggerMessage(LoggerLevel::INFO, className, functionName, message));

}
void LoggerManager::warn(const std::string & className,
        const std::string & functionName, const std::string & message) {
    addMessageToBuffer(
            LoggerMessage(LoggerLevel::WARN, className, functionName, message));
}
void LoggerManager::error(const std::string & className,
        const std::string & functionName, const std::string & message) {
    addMessageToBuffer(
            LoggerMessage(LoggerLevel::ERROR, className, functionName,
                    message));
}

void LoggerManager::addMessageToBuffer(LoggerMessage loggerMessage) {
    sf::Lock lock(m_mutex); // mutex.lock()

    if (m_active == true && isLoggable(loggerMessage)) {
        m_buffer += loggerMessage.toString() + '\n';
    }
} // mutex.unlock()

std::string LoggerManager::toString() {
    sf::Lock lock(m_mutex); // mutex.lock()

    Formatter formatter;
    formatter << "DefaultLevel = " << m_defaultLevel.toString() << "\n";
    formatter << "PrintMode = " << m_printMode.toString() << "\n";

    for (m_it = m_customClassLevels.begin(); m_it != m_customClassLevels.end();
            m_it++) {
        formatter << m_it->first << " = "
                << (m_it->second)->getDefaultClassLevel().toString() << "\n";
    }

    return formatter;
} // mutex.unlock()

