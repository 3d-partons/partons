#include "Partons.h"

#include <pthread.h>

//#include "database/DatabaseManager.h"
#include "ModuleObjectFactory.h"
#include "utils/logger/LoggerManager.h"
#include "utils/PropertiesManager.h"
#include "utils/stringUtils/StringUtils.h"

// Global static pointer used to ensure a single instance of the class.
Partons* Partons::m_pInstance = 0;

Partons::Partons()
        : m_pLoggerManager(LoggerManager::getInstance()) {
}

Partons::~Partons() {
    if (m_pInstance != 0) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

Partons* Partons::getInstance() {
    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new Partons();
    }

    return m_pInstance;
}

void Partons::init(char** argv) {
    m_currentWorkingDirectoryPath = StringUtils::removeAfterLast(argv[0], '/');

    // 1. Init PropertiesManager to provides configurations
    PropertiesManager::getInstance()->init(m_currentWorkingDirectoryPath);

    // 2. Init the logger to trac info/warn/debug message
    m_pLoggerManager->init();

    // 3. Init the ModuleFactory
    ModuleObjectFactory::getInstance()->init();

    // Database connexion
    // DatabaseManager::getInstance();

    // 4. Start logger's thread
    // Waiting for the end of the logger thread before main return
    pthread_join(m_pLoggerManager->getThreadId(), 0);
    // Start main function of the logger.
    m_pLoggerManager->start();
}

void Partons::stop() {
    m_pLoggerManager->stop();
}

std::string Partons::getCurrentWorkingDirectory() {
    return m_currentWorkingDirectoryPath;
}
