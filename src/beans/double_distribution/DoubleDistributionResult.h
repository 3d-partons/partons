#ifndef DOUBLE_DISTRIBUTION_RESULT_H
#define DOUBLE_DISTRIBUTION_RESULT_H

/**
 * @file DoubleDistributionResult.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 30 July 2015
 * @version 1.0
 *
 * @class DoubleDistributionResult
 *
 * @brief
 */

#include <map>
#include <string>

#include "../parton_distribution/PartonDistribution.h"
#include "DoubleDistributionKinematic.h"
#include "DoubleDistributionType.h"

class DoubleDistributionResult: public BaseObject {
public:
    DoubleDistributionResult();
    virtual ~DoubleDistributionResult();

    void addPartonDistribution(
            DoubleDistributionType::Type doubleDistributionType,
            PartonDistribution partonDistribution);
    const PartonDistribution& getPartonDistribution(
            DoubleDistributionType::Type doubleDistributionType) const;

    std::string toString();

private:
    std::map<DoubleDistributionType::Type, PartonDistribution> m_partonDistributions;

    DoubleDistributionKinematic m_kinematic;
};

#endif /* DOUBLE_DISTRIBUTION_RESULT_H */
