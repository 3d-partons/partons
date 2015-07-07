/**
 * @file GPDResultReport.h
 * @author: Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date 6 July 2015
 * @version 1.0
 *
 * @class GPDResultReport
 *
 * @brief Describes the result of the comparisons (relative and absolute) of two instances of GPDresult objects within given tolerances.
 */

#ifndef GPDRESULTREPORT_H_
#define GPDRESULTREPORT_H_

#include <map>
#include <string>

#include "../../../beans/gpd/GPDType.h"
#include "PartonDistributionReport.h"

class GPDResultReport: public ComparisonReport {
public:
    GPDResultReport();
    virtual ~GPDResultReport();

    virtual std::string toString() const;
    bool isCommonGpdType() const;
    void setCommonGpdType(bool commonGpdType);
    void addPartonDistributionReport(GPDType::Type gpdType,
            PartonDistributionReport partonDistributionReport);
    const PartonDistributionReport& getPartonDistributionReport(
            GPDType::Type gpdType) const;

private:
    bool m_commonGPDType;
    std::map<GPDType::Type, PartonDistributionReport> m_partonDistributionReports;
};

#endif /* GPDRESULTREPORT_H_ */
