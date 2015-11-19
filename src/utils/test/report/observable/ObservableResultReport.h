#ifndef OBSERVABLE_RESULT_REPORT_H
#define OBSERVABLE_RESULT_REPORT_H

/**
 * @file ObservableResultReport.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date 17 November 2015
 * @version 1.0
 *
 * @class ObservableResultReport
 *
 * @brief
 */

#include <string>

#include "../../DoubleComparisonReport.h"

class ObservableResultReport: public ComparisonReport {
public:
    ObservableResultReport();
    ObservableResultReport(std::string observableName, DoubleComparisonReport observableReport);
    virtual ~ObservableResultReport();

    virtual std::string toString() const;
    const DoubleComparisonReport& getObservableReport() const;
    void setObservableReport(const DoubleComparisonReport& observableReport);
    const std::string& getObservableName() const;
    void setObservableName(const std::string& observableName);

private:
    std::string m_observableName;
    DoubleComparisonReport m_observableReport;
};

#endif /* OBSERVABLE_RESULT_REPORT_H */
