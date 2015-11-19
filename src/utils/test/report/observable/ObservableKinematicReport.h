/**
 * @file ObservableKinematicReport
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date 19 November 2015
 * @version 1.0
 *
 * @class ObservableKinematicReport
 *
 * @brief Compare two different ObservableKinematic objects within given tolerances.
 */

#ifndef OBSERVABLEKINEMATICREPORT_H_
#define OBSERVABLEKINEMATICREPORT_H_

#include <string>
#include <vector>

#include "../../DoubleComparisonReport.h"

class ObservableKinematicReport: public ComparisonReport {
public:
    ObservableKinematicReport();
    virtual ~ObservableKinematicReport();

    virtual std::string toString() const;
    const DoubleComparisonReport& getQ2Report() const;
    void setQ2Report(const DoubleComparisonReport& q2Report);
    const DoubleComparisonReport& getTReport() const;
    void setTReport(const DoubleComparisonReport& tReport);
    const DoubleComparisonReport& getXBReport() const;
    void setXBReport(const DoubleComparisonReport& xBReport);
    const std::vector<DoubleComparisonReport>& getListOfPhiReport() const;
    void setListOfPhiReport(
            const std::vector<DoubleComparisonReport>& listOfPhiReport);

private:
    DoubleComparisonReport m_xBReport;
    DoubleComparisonReport m_tReport;
    DoubleComparisonReport m_Q2Report;
    std::vector<DoubleComparisonReport> m_listOfPhiReport;
};

#endif /* OBSERVABLEKINEMATICREPORT_H_ */
