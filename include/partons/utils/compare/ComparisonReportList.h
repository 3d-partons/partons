#ifndef COMPARISON_REPORT_LIST_H
#define COMPARISON_REPORT_LIST_H

/**
 * @file ComparisonReportList.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 30 November 2015
 * @version 1.0
 *
 * @class ComparisonReportList
 *
 * @brief
 */

#include <stddef.h>
#include <string>
#include <vector>

#include "../math/Tolerances.h"
#include "../test/ComparisonReport.h"
#include "ComparisonMode.h"

class ComparisonReportList {
public:
    ComparisonReportList();
    virtual ~ComparisonReportList();

    void add(const ComparisonReport &comparisonReport);

    size_t getNumberOfFailedComparaison() const;

    std::string toString() const;

    void toHTMLFile(const std::string &filePath) const;

    // ##### GETTERS & SETTERS #####

    ComparisonMode getComparisonMode() const;
    size_t getNumberOfComparedObjet() const;
    const std::string& getObjectTypeCompared() const;
    const std::vector<ComparisonReport>& getReports() const;
    const Tolerances& getTolerances() const;

    void setComparisonMode(ComparisonMode comparisonMode);
    void setNumberOfComparedObjet(size_t numberOfComparedObjet);
    void setObjectTypeCompared(const std::string& objectTypeCompared);
    void setReports(const std::vector<ComparisonReport>& reports);
    void setTolerances(const Tolerances& tolerances);

private:
    std::vector<ComparisonReport> m_reports;
    std::vector<ComparisonReport*> m_failedReports;

    std::string m_objectTypeCompared;
    ComparisonMode m_comparisonMode;
    Tolerances m_tolerances;
    size_t m_numberOfComparedObjet;
};

#endif /* COMPARISON_REPORT_LIST_H */
