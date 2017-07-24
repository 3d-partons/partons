/*
 * ResultInfo.h
 *
 *  Created on: May 20, 2016
 *      Author: debian
 */

#ifndef RESULT_INFO_H
#define RESULT_INFO_H

#include <string>

#include "../Computation.h"

namespace PARTONS {

/**
 * @class ResultInfo
 *
 * @brief Class storing information associated to result evaluation.
 *
 * This class is used to store an information associated to the evaluation of a single result, like the hash sum of used scenario or id of used task.
 */
class ResultInfo: public BaseObject {

public:

    /**
     * Default constructor.
     */
    ResultInfo();

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ResultInfo(const ResultInfo &other);

    /**
     * Destructor.
     */
    virtual ~ResultInfo();

    virtual std::string toString() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get computation information associated to the corresponding result.
     */
    const Computation& getComputation() const;

    /**
     * Set computation information associated to the corresponding result.
     */
    void setComputation(const Computation& computation);

    /**
     * Get hash sum of scenario associated to the corresponding result.
     */
    const std::string& getScenarioHashSum() const;

    /**
     * Set hash sum of scenario associated to the corresponding result.
     */
    void setScenarioHashSum(const std::string& scenarioHashSum);

    /**
     * Get hash sum of environment configuration associated to the corresponding result.
     */
    const std::string& getEnvironmentConfigurationHashSum() const;

    /**
     * Set hash sum of environment configuration associated to the corresponding result.
     */
    void setEnvironmentConfigurationHashSum(
            const std::string& environmentConfigurationHashSum);

    /**
     * Get id of task used to evaluate the corresponding result.
     */
    unsigned int getScenarioTaskIndexNumber() const;

    /**
     * Set id of task used to evaluate the corresponding result.
     */
    void setScenarioTaskIndexNumber(unsigned int scenarioTaskIndexNumber);

private:

    /**
     * Computation information associated to the corresponding result.
     */
    Computation m_computation;

    /**
     * Hash sum of scenario associated to the corresponding result.
     */
    std::string m_scenarioHashSum;

    /**
     * Hash sum of environment configuration associated to the corresponding result.
     */
    std::string m_environmentConfigurationHashSum;

    /**
     * Id of task used to evaluate the corresponding result.
     */
    unsigned int m_scenarioTaskIndexNumber;
};

} /* namespace PARTONS */

#endif /* RESULT_INFO_H */
