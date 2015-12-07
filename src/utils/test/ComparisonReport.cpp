#include "ComparisonReport.h"

#include <stddef.h>

#include "../stringUtils/Formatter.h"

ComparisonReport::ComparisonReport() :
        m_failed(false) {
}

ComparisonReport::ComparisonReport(const std::string &objectClassName) :
        m_objectClassName(objectClassName) {

}

ComparisonReport::ComparisonReport(const std::string& objectClassName,
        const std::string& context) :
        m_objectClassName(objectClassName), m_context(context) {
}

ComparisonReport::ComparisonReport(bool comparisonResult) :
        m_comparisonResult(comparisonResult) {
}

ComparisonReport::~ComparisonReport() {
}

//TODO remove
bool ComparisonReport::isEqual() const {
    return true;
}

//TODO implement
std::string ComparisonReport::toString() const {
    Formatter formatter;

    formatter << m_objectClassName << '\n';

    if (!m_context.empty()) {
        formatter << "Context = " << m_context << '\n';
    }

    for (size_t i = 0; i != m_data.size(); i++) {
        formatter << m_data[i].toString() << '\n';
    }

    for (size_t i = 0; i != m_children.size(); i++) {
        formatter << m_children[i].toString() << '\n';
    }

    return formatter.str();
}

void ComparisonReport::addChildren(const ComparisonReport &children) {
    m_children.push_back(children);
}

// TODO improve memory ; avoid copy object
void ComparisonReport::addChildren(const ComparisonReportList &children) {

    std::vector<ComparisonReport> reports;

    for (size_t i = 0; i != reports.size(); i++) {
        m_children.push_back(reports[i]);
    }
}

bool ComparisonReport::isFailed() const {
    return m_failed;
}

void ComparisonReport::addComparisonData(const ComparisonData& comparisonData) {
    m_data.push_back(comparisonData);
}

void ComparisonReport::setComparisonResult(bool comparisonResult) {
    m_comparisonResult = comparisonResult;
}

void ComparisonReport::setClassName(const std::string& objectClassName) {
    m_objectClassName = objectClassName;
}

void ComparisonReport::setContext(const std::string& context) {
    m_context = context;
}

const std::string& ComparisonReport::getClassName() const {
    return m_objectClassName;
}

const std::string& ComparisonReport::getContext() const {
    return m_context;
}
