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

#include "../../../../beans/observable/ObservableType.h"
#include "../../DoubleComparisonReport.h"
#include "ObservableKinematicReport.h"

class ObservableResultReport: public ComparisonReport {
public:
    ObservableResultReport();
    ObservableResultReport(const std::string& observableName,
            ObservableType::Type observableType,
            const DoubleComparisonReport& observableReport,
            const ObservableKinematicReport& observableKinematicReport);
    ObservableResultReport(const std::string& observableName,
            ObservableType::Type observableType,
            const DoubleComparisonReport& observableReport,
            const ObservableKinematicReport& observableKinematicReport,
            bool sameObservable);
    virtual ~ObservableResultReport();

    virtual std::string toString() const;
    const DoubleComparisonReport& getObservableReport() const;
    void setObservableReport(const DoubleComparisonReport& observableReport);
    const ObservableKinematicReport& getObservableKinematicReport() const;
    void setObservableKinematicReport(
            const ObservableKinematicReport& observableKinematicReport);

    const std::string& getObservableName() const;
    void setObservableName(const std::string& observableName);
    ObservableType::Type getObservableType() const;
    void setObservableType(ObservableType::Type observableType);
    bool isSameObservable() const;
    void setSameObservable(bool sameObservable);

private:
    std::string m_observableName;
    ObservableType::Type m_observableType;
    bool m_sameObservable;
    DoubleComparisonReport m_observableReport;
    ObservableKinematicReport m_observableKinematicReport;

    void testComparison();
};

#endif /* OBSERVABLE_RESULT_REPORT_H */
