/**
 * @file GPDKinematicReport
 * @author: Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date 13 juil. 2015
 * @version 1.0
 *
 * @class GPDKinematicReport
 *
 * @brief Compare two different GPDKinematic objects within given tolerances.
 */

#ifndef GPDKINEMATICREPORT_H_
#define GPDKINEMATICREPORT_H_

//#include "../ComparisonReport.h"
#include "../DoubleComparisonReport.h"

class GPDKinematicReport: public ComparisonReport {
public:
    GPDKinematicReport();
    virtual ~GPDKinematicReport();

    virtual std::string toString() const;
    const DoubleComparisonReport& getMuFReport() const;
    void setMuFReport(const DoubleComparisonReport& muFReport);
    const DoubleComparisonReport& getMuRReport() const;
    void setMuRReport(const DoubleComparisonReport& muRReport);
    const DoubleComparisonReport& getTReport() const;
    void setTReport(const DoubleComparisonReport& tReport);
    const DoubleComparisonReport& getXiReport() const;
    void setXiReport(const DoubleComparisonReport& xiReport);
    const DoubleComparisonReport& getXReport() const;
    void setXReport(const DoubleComparisonReport& xReport);

private:
    DoubleComparisonReport m_xReport;
    DoubleComparisonReport m_xiReport;
    DoubleComparisonReport m_tReport;
    DoubleComparisonReport m_MuFReport;
    DoubleComparisonReport m_MuRReport;
};

#endif /* GPDKINEMATICREPORT_H_ */
