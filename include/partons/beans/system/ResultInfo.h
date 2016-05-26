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

class ResultInfo: public BaseObject {
public:
    ResultInfo();

    ResultInfo(const ResultInfo &other);

    virtual ~ResultInfo();

    virtual std::string toString() const;

    const Computation& getComputation() const;
    void setComputation(const Computation& computation);
    const std::string& getEnvironmentConfigurationHashSum() const;
    void setEnvironmentConfigurationHashSum(
            const std::string& environmentConfigurationHashSum);
    const std::string& getScenarioHashSum() const;
    void setScenarioHashSum(const std::string& scenarioHashSum);
    unsigned int getScenarioTaskIndexNumber() const;
    void setScenarioTaskIndexNumber(unsigned int scenarioTaskIndexNumber);

private:
    Computation m_computation;

    std::string m_scenarioHashSum;
    std::string m_environmentConfigurationHashSum;

    unsigned int m_scenarioTaskIndexNumber;
};

#endif /* RESULT_INFO_H */
