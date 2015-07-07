/**
 * @file DoubleComparisonReport.h
 * @author: Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date 2 July 2015
 * @version 1.0
 *
 * @class DoubleComparisonReport
 *
 * @brief Describes the result of the comparisons (relative and absolute) of two real numbers within given tolerances.
 */

#ifndef DOUBLECOMPARISONREPORT_H_
#define DOUBLECOMPARISONREPORT_H_

#include <string>

#include "ComparisonReport.h"

class Tolerances;

class DoubleComparisonReport: public ComparisonReport {
public:
    DoubleComparisonReport();
    DoubleComparisonReport(bool comparisonResult, double absoluteDifference,
            double relativeDifference, double absoluteTolerance,
            double relativeTolerance);
    DoubleComparisonReport(bool comparisonResult, double absoluteDifference,
            double relativeDifference, Tolerances& tolerances);
    virtual ~DoubleComparisonReport();

    virtual std::string toString() const;
    double getAbsoluteDifference() const;
    double getAbsoluteTolerance() const;
    double getRelativeDifference() const;
    double getRelativeTolerance() const;

private:
    double m_absoluteDifference;
    double m_relativeDifference;
    double m_absoluteTolerance;
    double m_relativeTolerance;

};

#endif /* DOUBLECOMPARISONREPORT_H_ */
