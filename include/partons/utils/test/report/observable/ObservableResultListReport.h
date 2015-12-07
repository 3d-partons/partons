#ifndef OBSERVABLE_RESULT_LIST_REPORT_H
#define OBSERVABLE_RESULT_LIST_REPORT_H

/**
 * @file ObservableResultListReport.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date 17 November 2015
 * @version 1.0
 *
 * @class ObservableResultListReport
 *
 * @brief
 */

#include <string>
#include <vector>

#include "ObservableResultReport.h"

class ObservableResultListReport: public ComparisonReport {
public:
    ObservableResultListReport();
    ObservableResultListReport(unsigned int lhsObservableResultListSize,
            unsigned int rhsObservableResultListSize);
    ObservableResultListReport(unsigned int lhsObservableResultListSize,
            unsigned int rhsObservableResultListSize,
            const std::vector<ObservableResultReport>& observableResultReports);
    virtual ~ObservableResultListReport();

    void addObservableResultReport(
            const ObservableResultReport& observableResultReport);
    const ObservableResultReport& getObservableResultReport(
            unsigned int index) const;

    unsigned int getSize() const;

    const std::vector<unsigned int>& getDifferentResultIndex() const;
    const unsigned int& getDifferentResultIndex(unsigned int index) const;
    void setDifferentResultIndex(
            const std::vector<unsigned int>& differentResultIndex);
    void addDifferentResultIndex(unsigned int differentResultIndex);
    unsigned int getLhsObservableResultListSize() const;
    void setLhsObservableResultListSize(
            unsigned int lhsObservableResultListSize);
    unsigned int getRhsObservableResultListSize() const;
    void setRhsObservableResultListSize(
            unsigned int rhsObservableResultListSize);
    bool isSameSize() const;

    const std::vector<ObservableResultReport>& getObservableResultReports() const;
    void setObservableResultReports(
            const std::vector<ObservableResultReport>& observableResultReports);

    virtual std::string toString() const;

private:
    bool m_sameSize;
    unsigned int m_lhsObservableResultListSize;
    unsigned int m_rhsObservableResultListSize;
    std::vector<unsigned int> m_differentResultIndex;
    std::vector<ObservableResultReport> m_observableResultReports;

    void testComparison();
};

#endif /* OBSERVABLE_RESULT_LIST_REPORT_H */
