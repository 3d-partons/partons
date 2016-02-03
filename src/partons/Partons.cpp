#include "../../include/partons/Partons.h"

#include "../../include/partons/BaseObjectFactory.h"
#include "../../include/partons/BaseObjectRegistry.h"
#include "../../include/partons/database/DatabaseManager.h"
#include "../../include/partons/ModuleObjectFactory.h"
#include "../../include/partons/ServiceObjectRegistry.h"
#include "../../include/partons/utils/logger/LoggerManager.h"
#include "../../include/partons/utils/PropertiesManager.h"
#include "../../include/partons/utils/stringUtils/StringUtils.h"

// Global static pointer used to ensure a single instance of the class.
Partons* Partons::m_pInstance = 0;

Partons* Partons::getInstance() {
    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new Partons();
    }

    return m_pInstance;
}

Partons::Partons() :
        m_pBaseObjectRegistry(BaseObjectRegistry::getInstance()), m_pServiceObjectRegistry(
                0), m_pBaseObjectFactory(0), m_pModuleObjectFactory(0), m_pLoggerManager(
                LoggerManager::getInstance()) {

    m_pBaseObjectFactory = new BaseObjectFactory(m_pBaseObjectRegistry);
    m_pModuleObjectFactory = new ModuleObjectFactory(m_pBaseObjectFactory);
    m_pServiceObjectRegistry = new ServiceObjectRegistry(m_pBaseObjectRegistry);
}

Partons::~Partons() {
    if (m_pServiceObjectRegistry) {
        delete m_pServiceObjectRegistry;
        m_pServiceObjectRegistry = 0;
    }

    if (m_pBaseObjectFactory) {
        // Delete all objects stored in the registry
        delete m_pBaseObjectRegistry;
        m_pBaseObjectRegistry = 0;
    }

    if (m_pLoggerManager) {
        delete m_pLoggerManager;
    }

    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = 0;
    }
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
    //m_pLoggerManager->start();
    m_pLoggerManager->launch();
}

void Partons::close() {
    DatabaseManager::getInstance()->close();

    if (m_pLoggerManager) {
        // Send close signal to logger
        m_pLoggerManager->close();

        // Wait the end of queue message
        m_pLoggerManager->wait();
    }
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

BaseObjectRegistry* Partons::getBaseObjectRegistry() const {
    return m_pBaseObjectRegistry;
}

ServiceObjectRegistry* Partons::getServiceObjectRegistry() const {
    return m_pServiceObjectRegistry;
}

BaseObjectFactory* Partons::getBaseObjectFactory() const {
    return m_pBaseObjectFactory;
}

ModuleObjectFactory* Partons::getModuleObjectFactory() const {
    return m_pModuleObjectFactory;
}

LoggerManager* Partons::getLoggerManager() const {
    return m_pLoggerManager;
}
