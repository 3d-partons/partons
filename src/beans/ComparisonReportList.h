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

#include "../utils/math/Tolerances.h"
#include "../utils/test/ComparisonReport.h"
#include "ComparisonMode.h"

class ComparisonReportList {
public:
    ComparisonReportList();
    virtual ~ComparisonReportList();

    void add(const ComparisonReport &comparisonReport);

    size_t getNumberOfFailedComparaison() const;

    std::string toString() const;

    void toHTMLFile(const std::string &filePath) const;

private:
    std::vector<ComparisonReport> m_reports;

    std::string m_objectTypeCompared;
    ComparisonMode m_comparisonMode;
    Tolerances m_tolerances;
    size_t m_numberOfComparedObjet;
};

#endif /* COMPARISON_REPORT_LIST_H */
