#ifndef PARTONS_H
#define PARTONS_H

/**
 * @file Partons.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date April 21, 2015
 * @version 1.0
 */

#include <string>

class BaseObjectFactory;
class BaseObjectRegistry;
class EnvironmentConfiguration;
class ModuleObjectFactory;
class ServiceObjectRegistry;

namespace ElemUtils {
class LoggerManager;
} /* namespace ElemUtils */

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

    void init(int argc, char** argv);
    void close();

    std::string getCurrentWorkingDirectory();

    BaseObjectRegistry* getBaseObjectRegistry() const;
    ServiceObjectRegistry* getServiceObjectRegistry() const;
    BaseObjectFactory* getBaseObjectFactory() const;
    ModuleObjectFactory* getModuleObjectFactory() const;
    ElemUtils::LoggerManager* getLoggerManager() const;
    EnvironmentConfiguration* getEnvironmentConfiguration() const;

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

    std::string m_currentWorkingDirectoryPath;
    EnvironmentConfiguration* m_pEnvironmentConfiguration;

    void retrieveEnvironmentConfiguration();
};

#endif /* PARTONS_H */
