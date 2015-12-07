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
#include <vector>

#include "../../../beans/gpd/GPDType.h"
#include "GPDKinematicReport.h"
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
    const std::vector<GPDType::Type>& getLhsGpdTypes() const;
    void setLhsGpdTypes(const std::vector<GPDType::Type>& lhsGpdTypes);
    const std::vector<GPDType::Type>& getRhsGpdTypes() const;
    void setRhsGpdTypes(const std::vector<GPDType::Type>& rhsGpdTypes);

    const GPDKinematicReport& getGpdKinematicReport() const {
        return m_gpdKinematicReport;
    }

    void setGpdKinematicReport(const GPDKinematicReport& gpdKinematicReport) {
        m_gpdKinematicReport = gpdKinematicReport;
    }

private:
    bool m_commonGPDType;
    GPDKinematicReport m_gpdKinematicReport;
    std::map<GPDType::Type, PartonDistributionReport> m_partonDistributionReports;
    std::vector<GPDType::Type> m_lhsGpdTypes;
    std::vector<GPDType::Type> m_rhsGpdTypes;
};

#endif /* GPDRESULTREPORT_H_ */
