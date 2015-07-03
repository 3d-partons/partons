#include "Partons.h"

#include <pthread.h>

#include "BaseObjectManager.h"
#include "BaseObjectRegistry.h"
#include "database/DatabaseManager.h"
#include "utils/logger/LoggerManager.h"
#include "utils/PropertiesManager.h"
#include "utils/stringUtils/StringUtils.h"

// Global static pointer used to ensure a single instance of the class.
Partons* Partons::m_pInstance = 0;

Partons::Partons() :
        m_pBaseObjectManager(BaseObjectManager::getInstance()), m_pLoggerManager(
                LoggerManager::getInstance()) {
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
    BaseObjectRegistry::getInstance()->init();

    // Database connexion
    DatabaseManager::getInstance();

    // 4. Start logger's thread
    // Waiting for the end of the logger thread before main return
    //TODO
    // pthread_join(m_pLoggerManager->getThreadId(), 0);
    // Start main function of the logger.
    m_pLoggerManager->start();
}

void Partons::close() {
    DatabaseManager::getInstance()->close();

    // Wait the end of the Logger thread
    void* result;
    pthread_join(m_pLoggerManager->getThreadId(), &result);

//    while (!(m_pLoggerManager->isEmptyMessageQueue())) {
//        sleep(1);
//    }

    // Logger must be delete at last
    m_pLoggerManager->close();

    // Delete all objects at the end and only at the end.
    delete m_pBaseObjectManager;
    m_pBaseObjectManager = 0;
}

std::string Partons::getCurrentWorkingDirectory() {
    return m_currentWorkingDirectoryPath;
}
