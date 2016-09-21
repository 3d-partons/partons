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
 * @brief
 */
class ComparisonReport {
public:
    ComparisonReport(const NumA::Tolerances &tolerances);
    ComparisonReport(const std::string &environmentSetting,
            const std::string &objectClassNameTested,
            const std::string &referenceObjectClassName);
    virtual ~ComparisonReport();

    void addComparisonData(const ComparisonData &comparisonData);

    std::string toString() const;

    std::string showComparedDataFailed() const;
    std::string showComparisonStats() const;

    void clearComparedData();

    // #############################
    // ##### GETTERS & SETTERS #####
    // #############################

    bool isPassed() const;

    const NumA::Tolerances& getTolerances() const;
    void setTolerances(const NumA::Tolerances& tolerances);
    size_t sizeOfComparedDataFailed() const;

private:
    std::string m_environmentSetting;
    std::string m_objectClassNameTested;
    std::string m_referenceObjectClassName;

    std::vector<ComparisonData> m_comparedDataPassed;
    std::vector<ComparisonData> m_comparedDataFailed;

    NumA::Tolerances m_tolerances;

    unsigned int m_numberOfComparedDataPassed;
    unsigned int m_numberOfComparedDataFailed;
};

#endif /* COMPARISON_REPORT_H */
