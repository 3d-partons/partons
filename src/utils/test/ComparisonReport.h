#ifndef COMPARISON_REPORT_H
#define COMPARISON_REPORT_H

/**
 * @file ComparisonReport.h
 * @author: Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date 2 July 2015
 * @version 1.0
 *
 * @class ComparisonReport
 *
 * @brief Purely virtual class to describe interfaces of Report classes used to compare objects in tests.
 */

#include <string>
#include <vector>

#include "../../beans/ComparisonData.h"

// TODO: Add "I" for "Interface" to the class name, i.e. change to ComparisonReportI
class ComparisonReport {
public:
    ComparisonReport();
    ComparisonReport(const std::string &className);
    ComparisonReport(const std::string &className, const std::string &context);
    virtual ~ComparisonReport();

    //TODO remove
    ComparisonReport(bool comparisonResult);
    void setComparisonResult(bool comparisonResult);
    //

    void addChildren(const ComparisonReport &children);
    void addComparisonData(const ComparisonData &comparisonData);

    std::string toString() const;

    bool isFailed() const;
    bool isEqual() const;

    void setClassName(const std::string& className);
    void setContext(const std::string& context);

    const std::string& getClassName() const;
    const std::string& getContext() const;

//TODO remove
protected:
    bool m_comparisonResult;
//

private:
    std::vector<ComparisonReport> m_children;
    std::vector<ComparisonData> m_data;

    std::string m_className;
    std::string m_context;

    bool m_failed;
};

#endif /* COMPARISON_REPORT_H */
