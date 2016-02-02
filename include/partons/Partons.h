#ifndef PARTONS_H
#define PARTONS_H

/**
 * @file Partons.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 21 April 2015
 * @version 1.0
 *
 * @class Partons
 *
 * @brief
 */

#include <string>

#include "beans/Scale.h"

class ModuleObjectFactory;

class ServiceObjectRegistry;

class BaseObjectFactory;
class BaseObjectRegistry;
class LoggerManager;

class Partons {
public:
    /**
     * Share a unique pointer of this class
     */
    static Partons* getInstance();

    /**
     * Default destructor
     */
    virtual ~Partons();

    void init(char** argv);
    void close();

    std::string getCurrentWorkingDirectory();

    void setScale(double MuF2, double MuR2);
    Scale getScale() const;

    BaseObjectRegistry* getBaseObjectRegistry() const;
    ServiceObjectRegistry* getServiceObjectRegistry() const;
    BaseObjectFactory* getBaseObjectFactory() const;
    ModuleObjectFactory* getModuleObjectFactory() const;
    LoggerManager* getLoggerManager() const;

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
    LoggerManager* m_pLoggerManager;

    std::string m_currentWorkingDirectoryPath;

    Scale m_scale;
};

#endif /* PARTONS_H */
