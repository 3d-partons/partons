#ifndef PARTONS_H
#define PARTONS_H

/**
 * @file Partons.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date April 21, 2015
 * @version 1.0
 */

#include <string>

namespace ElemUtils {
class LoggerManager;
} /* namespace ElemUtils */

namespace PARTONS {

class DatabaseManager;

class BaseObjectFactory;
class BaseObjectRegistry;
class EnvironmentConfiguration;
class ModuleObjectFactory;
class ServiceObjectRegistry;

/**
 * @class Partons
 *
 * @brief
 */
class Partons {
public:
    static const std::string PROPERTIES_FILE_NAME;

    /**
     * Share a unique pointer of this class
     */
    static Partons* getInstance();

    /**
     * Default destructor
     */
    virtual ~Partons();

    /**
     * Must be called before any other thing.
     * It initializes each mandatory bloc in right order.
     *
     * @param argc
     * @param argv
     */
    void init(int argc, char** argv);

    /**
     * Called at the very end of the code.
     * Send close order to the LoggerManager and wait to the end of the job (flush of the logger buffer) before free memory.
     */
    void close();

    std::string getCurrentWorkingDirectory();

    BaseObjectRegistry* getBaseObjectRegistry() const;
    ServiceObjectRegistry* getServiceObjectRegistry() const;
    BaseObjectFactory* getBaseObjectFactory() const;
    ModuleObjectFactory* getModuleObjectFactory() const;
    ElemUtils::LoggerManager* getLoggerManager() const;
    EnvironmentConfiguration* getEnvironmentConfiguration() const;

    //TODO vérifier tous les ElemUtils::CustomException des fois il manque la directive throw.

private:
    /**
     * Private pointer of this class for a unique instance
     */
    static Partons* m_pInstance;

    /**
     * Default constructor
     */
    Partons();

    BaseObjectRegistry* m_pBaseObjectRegistry;
    ServiceObjectRegistry* m_pServiceObjectRegistry;
    BaseObjectFactory* m_pBaseObjectFactory;
    ModuleObjectFactory* m_pModuleObjectFactory;
    ElemUtils::LoggerManager* m_pLoggerManager;
    DatabaseManager* m_pDatabaseManager;

    std::string m_currentWorkingDirectoryPath;
    EnvironmentConfiguration* m_pEnvironmentConfiguration;

    /**
     * Retrieve environment configuration.
     */
    void retrieveEnvironmentConfiguration();

    /**
     * Check if "partons.properties" file path is readable. Throw exception else.
     */
    void checkMandatoryFiles();

    /**
     * Prints information about the version to Logger.
     */
    void printVersion() const;
};

} /* namespace PARTONS */

#endif /* PARTONS_H */
