#include "../../include/partons/Partons.h"

#include <ElementaryUtils/file_utils/FileUtils.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../include/partons/beans/system/EnvironmentConfiguration.h"
#include "../../include/partons/BaseObjectFactory.h"
#include "../../include/partons/BaseObjectRegistry.h"
#include "../../include/partons/database/DatabaseManager.h"
#include "../../include/partons/ModuleObjectFactory.h"
#include "../../include/partons/ServiceObjectRegistry.h"

const std::string Partons::PROPERTIES_FILE_NAME = "partons.properties";

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
                ElemUtils::LoggerManager::getInstance()), m_pEnvironmentConfiguration(
                0) {

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

    if (m_pBaseObjectRegistry) {
        delete m_pBaseObjectRegistry;
        m_pBaseObjectRegistry = 0;
    }

    if (m_pEnvironmentConfiguration) {
        delete m_pEnvironmentConfiguration;
        m_pEnvironmentConfiguration = 0;
    }

    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

void Partons::init(int argc, char** argv) {

    //TODO check with windows system path, how to handle '/' & '\' characters
    // Get current working directory
    m_currentWorkingDirectoryPath = ElemUtils::StringUtils::removeAfterLast(
            argv[0], '/');

    // 1. Init PropertiesManager to provides configurations
    ElemUtils::PropertiesManager::getInstance()->init(
            m_currentWorkingDirectoryPath + Partons::PROPERTIES_FILE_NAME);

    // 2. Init the logger to trac info/warn/debug message
    m_pLoggerManager->init();

    // 3. Init each object stored in the registry
    m_pBaseObjectRegistry->resolveBaseObjectDependencies();

    // Database connexion
    DatabaseManager::getInstance();

    // 4. Start logger's thread
    //m_pLoggerManager->start();
    m_pLoggerManager->launch();

    // 5. Retrieve environment configuration
    retrieveEnvironmentConfiguration();
}

void Partons::retrieveEnvironmentConfiguration() {
    std::string configuration = ElemUtils::FileUtils::read(
            ElemUtils::PropertiesManager::getInstance()->getString(
                    "environment.configuration.file.path"));
    std::string md5 = "undefined";
    m_pEnvironmentConfiguration = new EnvironmentConfiguration();
    m_pEnvironmentConfiguration->setFile(configuration);
}

void Partons::close() {
    DatabaseManager::getInstance()->close();

    if (m_pLoggerManager) {
        // Send close signal to logger
        m_pLoggerManager->close();

        // Wait the end of queue message
        m_pLoggerManager->wait();
    }

    if (m_pLoggerManager) {
        delete m_pLoggerManager;
        m_pLoggerManager = 0;
    }
}

std::string Partons::getCurrentWorkingDirectory() {
    return m_currentWorkingDirectoryPath;
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

ElemUtils::LoggerManager* Partons::getLoggerManager() const {
    return m_pLoggerManager;
}

EnvironmentConfiguration* Partons::getEnvironmentConfiguration() const {
    return m_pEnvironmentConfiguration;
}
