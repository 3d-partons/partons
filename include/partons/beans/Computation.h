#ifndef COMPUTATION_H
#define COMPUTATION_H

/**
 * @file Computation.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include <ctime>

#include "../database/DatabaseObject.h"

class EnvironmentConfiguration;
class Scenario;

/** @class Computation
 *
 * @brief
 */
class Computation: public DatabaseObject {
public:
    /**
     * Default constructor
     */
    Computation();

    /**
     * Copy constructor
     *
     * @param other
     */
    Computation(const Computation &other);

    /**
     * Constructor
     *
     * @param indexId
     * @param dateTime
     * @param pScenario
     * @param pEnvironmentConfiguration
     */
    Computation(int indexId, time_t dateTime, Scenario* pScenario,
            EnvironmentConfiguration* pEnvironmentConfiguration);

    /**
     * Default destructor
     */
    virtual ~Computation();

    // ##### GETTERS & SETTERS #####

    time_t getDateTime() const;
    void setDateTime(time_t dateTime);
    EnvironmentConfiguration* getEnvironmentConfiguration() const;
    void setEnvironmentConfiguration(
            EnvironmentConfiguration* pEnvironmentConfiguration);
    Scenario* getScenario() const;
    void setScenario(Scenario* pScenario);

private:
    time_t m_dateTime;

    Scenario* m_pScenario;
    EnvironmentConfiguration* m_pEnvironmentConfiguration;
};

#endif /* COMPUTATION_H */
