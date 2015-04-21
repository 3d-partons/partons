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
    void stop();

    std::string getCurrentWorkingDirectory();

private:
    /**
     * Private pointer of this class for a unique instance
     */
    static Partons* m_pInstance;

    /**
     * Default constructor
     */
    Partons();

    LoggerManager* m_pLoggerManager;

    std::string m_currentWorkingDirectoryPath;
};

#endif /* PARTONS_H */
