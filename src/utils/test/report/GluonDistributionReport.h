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
    const DoubleComparisonReport& getGluonComparisonReport() const;
    void setGluonComparisonReport(
            const DoubleComparisonReport& gluonComparisonReport);

private:
    DoubleComparisonReport m_gluonComparisonReport;
};

#endif /* GLUONDISTRIBUTIONREPORT_H_ */
