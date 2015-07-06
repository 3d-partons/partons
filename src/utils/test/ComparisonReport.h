/*
 * @file ComparisonReport.h
 * @author: Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date 2 July 2015
 * @version 1.0
 *
 * @class ComparisonReport
 *
 * @brief Purely virtual class to describe interfaces of Report classes used to compare objects in tests.
 */

#ifndef COMPARISONREPORT_H_
#define COMPARISONREPORT_H_

#include <string>

// TODO: Add "I" for "Interface" to the class name, i.e. change to ComparisonReportI
class ComparisonReport {
public:
    ComparisonReport();
    ComparisonReport(bool comparisonResult);
    virtual ~ComparisonReport();

    bool isEqual() const;
    virtual std::string toString() const = 0;
    void setComparisonResult(bool comparisonResult);

protected:
    bool m_comparisonResult;
};

#endif /* COMPARISONREPORT_H_ */
