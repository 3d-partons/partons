/**
 * @file GluonDistributionReport.h
 * @author: Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date 3 July 2015
 * @version 1.0
 *
 * @class GluonDistributionReport
 *
 * @brief Describes the result of the comparisons (relative and absolute) of two instances of GluonDistribution class within given tolerances.
 */

#ifndef GLUONDISTRIBUTIONREPORT_H_
#define GLUONDISTRIBUTIONREPORT_H_

#include <string>

#include "../DoubleComparisonReport.h"

class GluonDistributionReport: public ComparisonReport {
public:
    GluonDistributionReport();
    GluonDistributionReport(DoubleComparisonReport gluonComparisonReport);
    virtual ~GluonDistributionReport();

    virtual std::string toString() const;
    void setGluonComparisonReport(
            const DoubleComparisonReport& gluonComparisonReport);

private:
    double m_absoluteDifference;
    double m_relativeDifference;
    double m_absoluteTolerance;
    double m_relativeTolerance;
};

#endif /* GLUONDISTRIBUTIONREPORT_H_ */
