#ifndef COMPARISON_REPORT_H
#define COMPARISON_REPORT_H

/**
 * @file ComparisonReport.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date February 25, 2016
 * @version 1.0
 */

#include <NumA/utils/Tolerances.h>
#include <stddef.h>
#include <string>
#include <vector>

#include "ComparisonData.h"

/**
 * @class ComparisonReport
 *
 * @brief Comparison report.
 *
 * This class stores a comparison result for a set of data.
 */
class ComparisonReport {

public:

	/**
	 * Constructor.
	 * @param tolerances Tolerance.
	 */
    ComparisonReport(const NumA::Tolerances &tolerances);

    /**
     * Constructor.
     * @param environmentSetting String of characters indicating the environment settings.
     * @param objectClassNameTested Name of class to be examined.
     * @param referenceObjectClassName Name of reference class.
     */
    ComparisonReport(const std::string &environmentSetting,
            const std::string &objectClassNameTested,
            const std::string &referenceObjectClassName);

    /**
     * Destructor.
     */
    virtual ~ComparisonReport();

    /**
     * Add comparison data for a single comparison point.
     * @param comparisonData ComparisonData object containing comparison information for a single point.
     */
    void addComparisonData(const ComparisonData &comparisonData);

    /**
     * Return a pre-formatted characters string for output visualization of class member's values.
     * @return Pre-formatted characters string.
     */
    std::string toString() const;

    /**
     * Return a pre-formatted characters string for data that have failed the comparison.
     * @return Pre-formatted characters string.
     */
    std::string showComparedDataFailed() const;

    /**
     * Return a pre-formatted characters string containing general information on the comparison.
     * @return Pre-formatted characters string.
     */
    std::string showComparisonStats() const;

    /**
     * Clear vectors containing comparison data (ComparisonData objects) for single points.
     */
    void clearComparedData();

	//********************************************************
	//*** SETTERS AND GETTERS ********************************
	//********************************************************

    /**
     * Check if the comparison is passed.
     */
    bool isPassed() const;

    /**
     * Get tolerance assumed in this test.
     */
    const NumA::Tolerances& getTolerances() const;

    /**
     * Set tolerance assumed in this test.
     */
    void setTolerances(const NumA::Tolerances& tolerances);

    /**
     * Number of data points that have failed the comparison.
     */
    size_t sizeOfComparedDataFailed() const;

private:

    /**
     *  String of characters indicating the environment settings.
     */
    std::string m_environmentSetting;

    /**
     * Name of class to be examined.
     */
    std::string m_objectClassNameTested;

    /**
     * Name of reference class.
     */
    std::string m_referenceObjectClassName;

    /**
     * Vector containing comparison data for single points that have passed the comparison.
     */
    std::vector<ComparisonData> m_comparedDataPassed;

    /**
     * Vector containing comparison data for single points that have failed the comparison.
     */
    std::vector<ComparisonData> m_comparedDataFailed;

    /**
     * Tolerance assumed in this test.
     */
    NumA::Tolerances m_tolerances;

    /**
     * Number of points that have passed the comparison.
     */
    unsigned int m_numberOfComparedDataPassed;

    /**
     * Number of points that have failed the comparison.
     */
    unsigned int m_numberOfComparedDataFailed;
};

#endif /* COMPARISON_REPORT_H */
