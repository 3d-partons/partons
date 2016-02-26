#ifndef COMPARISON_REPORT_H
#define COMPARISON_REPORT_H

/**
 * @file ComparisonReport.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 25 February 2016
 * @version 1.0
 *
 * @class ComparisonReport
 *
 * @brief
 */

#include <NumA/utils/Tolerances.h>
#include <string>
#include <vector>

#include "ComparisonData.h"

class ComparisonReport {
public:
    ComparisonReport();
    virtual ~ComparisonReport();

    void addComparisonData(const ComparisonData &comparisonData);

    std::string toString() const;

    // #############################
    // ##### GETTERS & SETTERS #####
    // #############################

    bool isPassed() const;

    const NumA::Tolerances& getTolerances() const;
    void setTolerances(const NumA::Tolerances& tolerances);

private:
    //TODO m_environmentConfiguration = read VM configuration and SVN revision number
    std::string m_environmentConfiguration;
    std::string m_moduleClassNameTested;

    std::vector<ComparisonData> m_comparedDataPassed;
    std::vector<ComparisonData> m_comparedDataFailed;

    NumA::Tolerances m_tolerances;
};

#endif /* COMPARISON_REPORT_H */
