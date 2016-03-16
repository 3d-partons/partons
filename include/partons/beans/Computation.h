#ifndef COMPUTATION_H
#define COMPUTATION_H

/**
 * @file Computation.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 November 2015
 * @version 1.0
 *
 * @class Computation
 *
 * @brief
 */

#include <ctime>

#include "../database/DatabaseObject.h"

class EnvironmentConfiguration;
class ComputationConfiguration;

class Computation: public DatabaseObject {
public:
    Computation();
    Computation(int indexId, time_t dateTime,
            ComputationConfiguration* pComputationConfiguration,
            EnvironmentConfiguration* pEnvironmentConfiguration);
    virtual ~Computation();

    // ##### GETTERS & SETTERS #####

    const time_t getDateTime() const;
    void setDateTime(time_t dateTime);
    ComputationConfiguration* getComputationConfiguration() const;
    void setComputationConfiguration(
            ComputationConfiguration* pComputationConfiguration);
    EnvironmentConfiguration* getEnvironmentConfiguration() const;
    void setEnvironmentConfiguration(
            EnvironmentConfiguration* pEnvironmentConfiguration);

private:
    time_t m_dateTime;

    ComputationConfiguration* m_pComputationConfiguration;
    EnvironmentConfiguration* m_pEnvironmentConfiguration;
};

#endif /* COMPUTATION_H */
