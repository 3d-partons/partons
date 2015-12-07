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

    formatter << "m_objectTypeCompared = " << m_objectTypeCompared;
    formatter << " m_comparisonMode = " << m_comparisonMode.toString();
    formatter << " m_tolerances = " << m_tolerances.toString();
    formatter << " m_numberOfComparedObjet = " << m_numberOfComparedObjet;
    formatter << '\n';

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

ComparisonMode ComparisonReportList::getComparisonMode() const {
    return m_comparisonMode;
}

size_t ComparisonReportList::getNumberOfComparedObjet() const {
    return m_numberOfComparedObjet;
}

const std::string& ComparisonReportList::getObjectTypeCompared() const {
    return m_objectTypeCompared;
}

const std::vector<ComparisonReport>& ComparisonReportList::getReports() const {
    return m_reports;
}

const Tolerances& ComparisonReportList::getTolerances() const {
    return m_tolerances;
}

void ComparisonReportList::setComparisonMode(ComparisonMode comparisonMode) {
    m_comparisonMode = comparisonMode;
}

void ComparisonReportList::setNumberOfComparedObjet(
        size_t numberOfComparedObjet) {
    m_numberOfComparedObjet = numberOfComparedObjet;
}

void ComparisonReportList::setObjectTypeCompared(
        const std::string& objectTypeCompared) {
    m_objectTypeCompared = objectTypeCompared;
}

void ComparisonReportList::setReports(
        const std::vector<ComparisonReport>& reports) {
    m_reports = reports;
}

void ComparisonReportList::setTolerances(const Tolerances& tolerances) {
    m_tolerances = tolerances;
}
