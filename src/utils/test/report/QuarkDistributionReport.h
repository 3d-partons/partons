/**
 * @file QuarkDistributionReport.h
 * @author: Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date 2 July 2015
 * @version 1.0
 *
 * @class QuarkDistributionReport
 *
 * @brief Describes the result of the comparisons (relative and absolute) of two instances of QuarkDistributions class within given tolerances.
 */

#ifndef QUARKDISTRIBUTIONREPORT_H_
#define QUARKDISTRIBUTIONREPORT_H_

#include <string>

#include "../../../beans/QuarkFlavor.h"
#include "../DoubleComparisonReport.h"

class QuarkDistributionReport: public ComparisonReport {
public:
    QuarkDistributionReport();
    QuarkDistributionReport(QuarkFlavor::Type quarkFlavor,
            DoubleComparisonReport quarkDistributionReport,
            DoubleComparisonReport quarkDistributionPlusReport,
            DoubleComparisonReport quarkDistributionMinusReport);
    virtual ~QuarkDistributionReport();

    virtual std::string toString() const;
    QuarkFlavor getQuarkFlavor() const;

private:
    QuarkFlavor m_quarkFlavor;
    DoubleComparisonReport m_quarkDistributionReport;
    DoubleComparisonReport m_quarkDistributionPlusReport;
    DoubleComparisonReport m_quarkDistributionMinusReport;
};

#endif /* QUARKDISTRIBUTIONREPORT_H_ */
