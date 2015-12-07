/**
 * @file GPDResultListReport.h
 * @author: Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date 7 juil. 2015
 * @version 1.0
 *
 * @class GPDResultReportList
 *
 * @brief Describes the result of the comparisons (relative and absolute) of two instances of GPDResultList objects within given tolerances.
 */

#ifndef GPDRESULTLISTREPORT_H_
#define GPDRESULTLISTREPORT_H_

#include <stddef.h>
#include <string>
#include <vector>

#include "GPDResultReport.h"

class GPDResultListReport: public ComparisonReport {
public:
    GPDResultListReport();
    virtual ~GPDResultListReport();

    void add(const GPDResultReport &gpdResultReport);
    const GPDResultReport& get(unsigned int index) const;

    size_t getSize() const;
    const GPDResultReport& getUniqueReport();

    virtual std::string toString() const;
    bool isSameSize() const;
    void setSameSize(bool sameSize);
    const std::vector<unsigned int>& getDifferentResultIndex() const;
    void setDifferentResultIndex(
            const std::vector<unsigned int>& differentResultIndex);
    unsigned int getLhsGpdResultListSize() const;
    void setLhsGpdResultListSize(unsigned int lhsGpdResultListSize);
    unsigned int getRhsGpdResultListSize() const;
    void setRhsGpdResultListSize(unsigned int rhsGpdResultListSize);

private:
    bool m_sameSize;
    unsigned int m_lhsGPDResultListSize;
    unsigned int m_rhsGPDResultListSize;
    std::vector<unsigned int> m_differentResultIndex;
    std::vector<GPDResultReport> m_gpdResultReports;
};

#endif /* GPDRESULTLISTREPORT_H_ */
