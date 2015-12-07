#ifndef GPD_RESULT_H
#define GPD_RESULT_H

/**
 * @file GPDResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 April 2015
 * @version 1.0
 *
 * @class GPDResult
 *
 * @brief
 */

#include <map>
#include <string>
#include <vector>

#include "../List.h"
#include "../parton_distribution/PartonDistribution.h"
#include "../Result.h"
#include "GPDKinematic.h"
#include "GPDType.h"

class GPDResult: public Result {
public:
    static const std::string GPD_RESULT_DB_TABLE_NAME;

    GPDResult();
    //GPDResult(GPDKinematic &gpdKinematic);
    virtual ~GPDResult();

    std::vector<GPDType> listGPDTypeComputed();
    void addPartonDistribution(GPDType::Type gpdType,
            PartonDistribution partonDistribution);
    const PartonDistribution& getPartonDistribution(
            GPDType::Type gpdType) const;

    List<PartonDistribution> getPartonDistributionList() const;

    virtual std::string toString();

    ComparisonReport compare(const GPDResult &referenceObject,
            const Tolerances &tolerances) const;

    // ##### GETTERS & SETTERS #####

    const std::map<GPDType::Type, PartonDistribution>& getPartonDistributions() const;
    void setPartonDistributions(
            const std::map<GPDType::Type, PartonDistribution>& partonDistributions);
    const GPDKinematic& getKinematic() const;
    void setKinematic(const GPDKinematic& kinematic);

private:
    std::map<GPDType::Type, PartonDistribution> m_partonDistributions;
    //std::map<GPDType::Type, PartonDistribution>::iterator m_it;

    GPDKinematic m_kinematic;
};

#endif /* GPD_RESULT_H */
