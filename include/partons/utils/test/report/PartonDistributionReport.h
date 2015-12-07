/**
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
#include <vector>

#include "../../../beans/QuarkFlavor.h"
#include "GluonDistributionReport.h"
#include "QuarkDistributionReport.h"

class PartonDistributionReport: public ComparisonReport {
public:
    PartonDistributionReport();
    virtual ~PartonDistributionReport();

    std::string toString() const;
    bool isCommonPartonType() const; ///< true if there is at least one common parton type in the two instances of PartonDistribution.
    void addQuarkDistributionReport(
            const QuarkDistributionReport &quarkDistributionReport);
    void setGluonDistributionReport(
            const GluonDistributionReport &gluonDistributionReport);
    void setCommonPartonType(bool commonPartonType);
    const GluonDistributionReport& getGluonDistributionReport() const;
    const QuarkDistributionReport& getQuarkDistributionReport(
            QuarkFlavor::Type quarkFlavorType) const;
    const std::vector<QuarkFlavor::Type>& getLhsQuarkFlavors() const;
    void setLhsQuarkFlavors(
            const std::vector<QuarkFlavor::Type>& lhsQuarkFlavors);
    bool isLhsUndefinedGluons() const;
    void setLhsUndefinedGluons(bool lhsUndefinedGluons);
    const std::vector<QuarkFlavor::Type>& getRhsQuarkFlavors() const;
    void setRhsQuarkFlavors(
            const std::vector<QuarkFlavor::Type>& rhsQuarkFlavors);
    bool isRhsUndefinedGluons() const;
    void setRhsUndefinedGluons(bool rhsUndefinedGluons);

private:
    bool m_commonPartonType;
    bool m_lhsUndefinedGluons;
    bool m_rhsUndefinedGluons;
    std::map<QuarkFlavor::Type, QuarkDistributionReport> m_quarkDistributionReports;
    std::vector<QuarkFlavor::Type> m_lhsQuarkFlavors;
    std::vector<QuarkFlavor::Type> m_rhsQuarkFlavors;
    GluonDistributionReport m_gluonDistributionReport;
};

#endif /* PARTONDISTRIBUTIONREPORT_H_ */
