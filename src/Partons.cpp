#include "Partons.h"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // win32

#include <pthread.h>

#include "BaseObjectFactory.h"
#include "BaseObjectRegistry.h"
#include "database/DatabaseManager.h"
#include "utils/logger/LoggerManager.h"
#include "utils/PropertiesManager.h"
#include "utils/stringUtils/StringUtils.h"

// Global static pointer used to ensure a single instance of the class.
Partons* Partons::m_pInstance = 0;

Partons::Partons() :
        m_pBaseObjectRegistry(0), m_pBaseObjectFactory(0), m_pLoggerManager(0) {

    m_pBaseObjectRegistry = BaseObjectRegistry::getInstance();
    m_pBaseObjectFactory = BaseObjectFactory::getInstance();
    m_pLoggerManager = LoggerManager::getInstance();
}

Partons::~Partons() {
    if (m_pInstance) {
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
    // Get current working directory
    m_currentWorkingDirectoryPath = StringUtils::removeAfterLast(argv[0], '/');

    // 1. Init PropertiesManager to provides configurations
    PropertiesManager::getInstance()->init(m_currentWorkingDirectoryPath);

    // 2. Init the logger to trac info/warn/debug message
    m_pLoggerManager->init();

    // 3. Init each object stored in the registry
    m_pBaseObjectRegistry->initBaseObject();

    // Database connexion
    DatabaseManager::getInstance();

    // 4. Start logger's thread
    m_pLoggerManager->start();
}

void Partons::close() {
#ifdef WIN32
    Sleep(milliseconds);
#else
    usleep(1000 * 1000);
#endif // win32

    //TODO test portability
    while (!m_pLoggerManager->isEmptyMessageQueue()) {
#ifdef WIN32
        Sleep(milliseconds);
#else
        usleep(30 * 1000);
#endif // win32
    }
    // Send close signal to logger
    m_pLoggerManager->close();

    // Wait the end of the Logger thread
    void* result;
    if (m_pLoggerManager != 0) {
        pthread_join(m_pLoggerManager->getThreadId(), &result);
    }

    DatabaseManager::getInstance()->close();

    // Finally delete LoggerManager pointer
    delete m_pLoggerManager;
    m_pLoggerManager = 0;

    // Delete all objects instantiated by the factory
    delete m_pBaseObjectFactory;
    m_pBaseObjectFactory = 0;

    // Delete all objects stored in the registry
    delete m_pBaseObjectRegistry;
    m_pBaseObjectRegistry = 0;
}

std::string Partons::getCurrentWorkingDirectory() {
    return m_currentWorkingDirectoryPath;
}

void Partons::setScale(double MuF2, double MuR2) {
    m_scale = Scale(MuF2, MuR2);
}

Scale Partons::getScale() const {
    return m_scale;
}

LoggerManager* Partons::getLoggerManager() {
    return m_pLoggerManager;
}
