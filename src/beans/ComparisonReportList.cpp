#include "ComparisonReportList.h"

#include "../utils/stringUtils/Formatter.h"

ComparisonReportList::ComparisonReportList() {
    // TODO Auto-generated constructor stub

}

ComparisonReportList::~ComparisonReportList() {
    // TODO Auto-generated destructor stub
}

size_t ComparisonReportList::getNumberOfFailedComparaison() const {
    size_t numberOfFailedComparaison = 0;

    for (size_t i = 0; i != m_reports.size(); i++) {
        if (m_reports[i].isFailed()) {
            numberOfFailedComparaison += 1;
        }
    }

    return numberOfFailedComparaison;
}

std::string ComparisonReportList::toString() const {
    Formatter formatter;

    for (size_t i = 0; i != m_reports.size(); i++) {
        formatter << m_reports[i].toString() << '\n';
    }

    return formatter.str();
}

void ComparisonReportList::add(const ComparisonReport& comparisonReport) {
    m_reports.push_back(comparisonReport);
}

void ComparisonReportList::toHTMLFile(const std::string& filePath) const {
    //TODO implement
}
