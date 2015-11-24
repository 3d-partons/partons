#include "ComparisonReport.h"

ComparisonReport::ComparisonReport() :
        m_comparisonResult(false) {
}

ComparisonReport::ComparisonReport(bool comparisonResult) :
        m_comparisonResult(comparisonResult) {
}

ComparisonReport::~ComparisonReport() {
}

bool ComparisonReport::isEqual() const {
    return m_comparisonResult;
}

void ComparisonReport::setComparisonResult(bool comparisonResult) {
    m_comparisonResult = comparisonResult;
}

//TODO implement
std::string ComparisonReport::toString() {
    return "";
}
