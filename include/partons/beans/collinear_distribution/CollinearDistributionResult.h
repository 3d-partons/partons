#ifndef COLLINEAR_DISTRIBUTION_RESULT_H
#define COLLINEAR_DISTRIBUTION_RESULT_H

/**
 * @file CollinearDistributionResult.h
 * @author: Valerio BERTONE (CEA Saclay)
 * @date 18 July 2020
 * @version 1.0
 */

#include <map>
#include <string>
#include <vector>

#include "../../utils/compare/ComparisonReport.h"
#include "../parton_distribution/PartonDistribution.h"
#include "../Result.h"
#include "CollinearDistributionKinematic.h"
#include "CollinearDistributionType.h"

namespace PARTONS {

/**
 * @class CollinearDistributionResult
 *
 * @brief Class representing single result of a collinear-distribution
 * computation.
 *
 * This class is used to store results of a single collinear
 * distribution computation. In other words, the class stores a
 * collection of PartonDistribution objects, with each of them
 * containing a part of the result defined for a specific collinear
 * distribution type.
 */
class CollinearDistributionResult: public Result<CollinearDistributionKinematic> {

public:

    /**
     * Default constructor.
     */
    CollinearDistributionResult();

    /**
     * Assignment constructor.
     * @param kinematic collinear distribution kinematics to be assigned.
     */
    CollinearDistributionResult(const CollinearDistributionKinematic& kinematic);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    CollinearDistributionResult(const CollinearDistributionResult &other);

    /**
     * Destructor.
     */
    virtual ~CollinearDistributionResult();

    virtual std::string toString() const;

    /**
     * Add parton distribution associated to given collinear distribution type.
     * @param colldistType collinear distribution type of parton distribution to be inserted.
     * @param partonDistribution Parton distribution to be added.
     */
    void addPartonDistribution(CollinearDistributionType::Type colldistType,
            const PartonDistribution& partonDistribution);

    /**
     * Get reference to parton distribution associated to given collinear distribution type.
     * @param colldistType collinear distribution type associated to parton distribution to be selected.
     * @return Reference to selected parton distribution.
     */
    const PartonDistribution& getPartonDistribution(
            CollinearDistributionType::Type colldistType) const;

    /**
     * Check if the object stores parton distribution of given collinear distribution type.
     * @param colldistType collinear distribution type to be checked.
     * @return True if the object stores parton distribution of given collinear distribution type, otherwise false.
     * @see CollinearDistributionResult::getLastAvailable()
     */
    bool isAvailable(const CollinearDistributionType::Type &colldistType) const;

    /**
     * Get reference to parton distribution marked by the last call of CollinearDistributionResult::isAvailible() function.
     * @return Reference to selected parton distribution.
     * @see CollinearDistributionResult::isAvailible()
     */
    PartonDistribution& getLastAvailable() const;

    /**
     * Compare to other CollinearDistributionResult object and store comparison result in given comparison report.
     * @param rootComparisonReport Reference to comparison report to be used to store comparison result.
     * @param referenceObject Reference to object to be compared.
     * @param parentObjectInfo Addition information coming from the parent object (if needed).
     */
    void compare(ComparisonReport &rootComparisonReport,
            const CollinearDistributionResult &referenceObject,
            std::string parentObjectInfo = "") const;

    /**
     * Get list of collinear distribution types associated to stored parton distributions.
     * @return Vector of associated types.
     */
    std::vector<CollinearDistributionType> listCollinearDistributionTypeComputed() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get reference to map containing stored parton distributions distinguished by associated collinear distribution types.
     */
    const std::map<CollinearDistributionType::Type, PartonDistribution>& getPartonDistributions() const;

    /**
     * Set map containing stored parton distributions distinguished by associated collinear distribution types.
     */
    void setPartonDistributions(
            const std::map<CollinearDistributionType::Type, PartonDistribution>& partonDistributions);

private:

    /**
     * Map containing stored parton distributions distinguished by associated collinear distribution types.
     */
    std::map<CollinearDistributionType::Type, PartonDistribution> m_partonDistributions;

    /**
     * Iterator used to mark a specific entry in CollinearDistributionResult::m_partonDistributions.
     */
    mutable std::map<CollinearDistributionType::Type, PartonDistribution>::const_iterator m_it;
};

} /* namespace PARTONS */

#endif /* COLLINEAR_DISTRIBUTION_RESULT_H */
