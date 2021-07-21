#include "../../include/partons/Partons.h"

#include <ElementaryUtils/file_utils/FileUtils.h>
#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/ElementaryUtilsVersion.h>
#include <NumA/NumAVersion.h>

#include "../../include/partons/beans/system/EnvironmentConfiguration.h"
#include "../../include/partons/BaseObjectFactory.h"
#include "../../include/partons/BaseObjectRegistry.h"
#include "../../include/partons/database/DatabaseManager.h"
#include "../../include/partons/ModuleObjectFactory.h"
#include "../../include/partons/ServiceObjectRegistry.h"
#include "../../include/partons/PartonsVersion.h"

namespace PARTONS {

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
                ElemUtils::LoggerManager::getInstance()), m_pDatabaseManager(
                DatabaseManager::getInstance()), m_pEnvironmentConfiguration(0) {

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

void Partons::checkMandatoryFiles() {
    if (!ElemUtils::FileUtils::isReadable(
            m_currentWorkingDirectoryPath + Partons::PROPERTIES_FILE_NAME)) {
        throw ElemUtils::CustomException("Partons", __func__,
                ElemUtils::Formatter() << "Missing configuration file "
                        << Partons::PROPERTIES_FILE_NAME << " in folder "
                        << m_currentWorkingDirectoryPath);
    }
}

void Partons::init(int argc, char** argv) {

    //TODO check with windows system path, how to handle '/' & '\' characters
    // Get current working directory
    m_currentWorkingDirectoryPath = ElemUtils::StringUtils::removeAfterLast(
            argv[0], '/');

    checkMandatoryFiles();

    // 1. Init PropertiesManager to provides configurations
    ElemUtils::PropertiesManager::getInstance()->init(
            m_currentWorkingDirectoryPath + Partons::PROPERTIES_FILE_NAME);

    // 2. Init the logger to trac info/warn/debug message
    m_pLoggerManager->init();

    // 3. Print version and welcome message
    printVersion();

    // 4. Init each object stored in the registry
    m_pBaseObjectRegistry->resolveBaseObjectDependencies();

    // 5. Database connexion
    DatabaseManager::getInstance()->init();

    // 6. Start logger's thread
    //m_pLoggerManager->start();
    m_pLoggerManager->launch();

    // 7. Retrieve environment configuration
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
    m_pDatabaseManager->close();

    //TODO Segmentation fault, why? Qt asynchronous?
//    if (m_pDatabaseManager) {
//        delete m_pDatabaseManager;
//        m_pDatabaseManager = 0;
//    }

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

void Partons::printVersion() const {

    m_pLoggerManager->info("Partons", __func__,
            ElemUtils::Formatter() << "PARTONS " << PARTONS_VERSION_MAJOR << "."
                    << PARTONS_VERSION_MINOR << "." << PARTONS_VERSION_PATCH
                    << " (http://partons.cea.fr) distributed under GNU Public License");
    m_pLoggerManager->info("Partons", __func__,
            ElemUtils::Formatter() << "Git branch and revision of this version is "
                    << PARTONS_GIT_BRANCH << "/" << PARTONS_GIT_REVISION);
    m_pLoggerManager->info("Partons", __func__,
            ElemUtils::Formatter() << "Built using Elementary-Utils "
                    << ELEMENTARY_UTILS_VERSION_MAJOR << "."
                    << ELEMENTARY_UTILS_VERSION_MINOR << "."
                    << ELEMENTARY_UTILS_VERSION_PATCH << " ("
                    << ELEMENTARY_UTILS_GIT_BRANCH << "/"
                    << ELEMENTARY_UTILS_GIT_REVISION << ")" << " and NumA++ "
                    << NUMA_VERSION_MAJOR << "." << NUMA_VERSION_MINOR << "."
                    << NUMA_VERSION_PATCH << " (" << NUMA_GIT_BRANCH << "/"
                    << NUMA_GIT_REVISION << ")");
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

} /* namespace PARTONS */
