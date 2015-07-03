/*
 * @file PartonDistributionReport.h
 * @author: Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date 2 July 2015
 * @version 1.0
 *
 * @class PartonDistributionReport
 *
 * @brief Describes the result of the comparisons (relative and absolute) of two instances of PartonDistribution objects within given tolerances.
 */

#ifndef PARTONDISTRIBUTIONREPORT_H_
#define PARTONDISTRIBUTIONREPORT_H_

#include <map>
#include <string>

#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/QuarkFlavor.h"
#include "GluonDistributionReport.h"
#include "QuarkDistributionReport.h"

class PartonDistributionReport: ComparisonReport {
public:
    PartonDistributionReport();
    PartonDistributionReport(GPDType::Type gpdType);
    virtual ~PartonDistributionReport();

    std::string toString() const;
    bool isCommonPartonType() const; ///< true if there is at least one common parton type in the two instances of PartonDistribution.
    void addQuarkDistributionReport(
            const QuarkDistributionReport &quarkDistributionReport);
    void setGluonDistributionReport(
            const GluonDistributionReport &gluonDistributionReport);
    void setCommonPartonType(bool commonPartonType);

private:
    bool m_commonPartonType;
    std::map<QuarkFlavor::Type, QuarkDistributionReport> m_quarkDistributionReports;
    GPDType::Type m_gpdType;
    GluonDistributionReport m_gluonDistributionReport;
};

#endif /* PARTONDISTRIBUTIONREPORT_H_ */
