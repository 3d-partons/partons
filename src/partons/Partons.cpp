#include "../../include/partons/Partons.h"

#include <SFML/System/Lock.hpp>
#include <SFML/System/Mutex.hpp>

#include "../../include/partons/BaseObjectFactory.h"
#include "../../include/partons/BaseObjectRegistry.h"
#include "../../include/partons/database/DatabaseManager.h"
#include "../../include/partons/utils/logger/LoggerManager.h"
#include "../../include/partons/utils/PropertiesManager.h"
#include "../../include/partons/utils/stringUtils/StringUtils.h"

sf::Mutex Partons::m_mutex;

// Global static pointer used to ensure a single instance of the class.
Partons* Partons::m_pInstance = 0;

Partons::Partons() :
        m_pBaseObjectRegistry(0), m_pBaseObjectFactory(0), m_pLoggerManager(0) {

    m_pBaseObjectRegistry = BaseObjectRegistry::getInstance();
    m_pBaseObjectFactory = BaseObjectFactory::getInstance();
    m_pLoggerManager = LoggerManager::getInstance();
}

Partons::~Partons() {
    sf::Lock lock(m_mutex); // mutex.lock()

    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = 0;
    }
} // mutex.unlock()

Partons* Partons::getInstance() {
    sf::Lock lock(m_mutex); // mutex.lock()

    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new Partons();
    }

    return m_pInstance;
} // mutex.unlock()

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
    //m_pLoggerManager->start();
    m_pLoggerManager->launch();
}

void Partons::close() {
    sf::Lock lock(m_mutex); // mutex.lock()

    DatabaseManager::getInstance()->close();

    if (m_pLoggerManager) {
        // Send close signal to logger
        m_pLoggerManager->close();

        // Wait the end of queue message
        m_pLoggerManager->wait();

        // Finally delete LoggerManager pointer
        m_pLoggerManager->delete_();
        m_pLoggerManager = 0;
    }

    if (m_pBaseObjectFactory) {
        // Delete all objects instantiated by the factory
        m_pBaseObjectFactory->delete_();
        m_pBaseObjectFactory = 0;
    }

    if (m_pBaseObjectFactory) {
        // Delete all objects stored in the registry
        m_pBaseObjectRegistry->delete_();
        m_pBaseObjectRegistry = 0;
    }
} // mutex.unlock()

std::string Partons::getCurrentWorkingDirectory() {
    return m_currentWorkingDirectoryPath;
}

void Partons::setScale(double MuF2, double MuR2) {
    m_scale = Scale(MuF2, MuR2);
}

Scale Partons::getScale() const {
    return m_scale;
}
