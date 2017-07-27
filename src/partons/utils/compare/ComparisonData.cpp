#include "../../../../include/partons/utils/compare/ComparisonData.h"

#include <ElementaryUtils/string_utils/Formatter.h>

namespace PARTONS {


ComparisonData::ComparisonData(bool isPassed, const std::string& variableName,
        const std::string& variableValue,
        const std::string& variableValueReference,
        const NumA::Differences &differences, const std::string &objectInfo) :
        m_isComparisonPassed(isPassed), m_variableName(variableName), m_variableValue(
                variableValue), m_variableValueReference(
                variableValueReference), m_differences(differences), m_objectInfo(
                objectInfo) {

}

ComparisonData::~ComparisonData() {
}

std::string ComparisonData::toString() const {
    ElemUtils::Formatter formatter;

    formatter << m_objectInfo << '\n';

    if (m_isComparisonPassed) {
        formatter << "[PASSED] ";
    } else {
        formatter << "[FAILED] ";
    }

    formatter << m_variableName << " = " << m_variableValue << " (computed) compared to = "
            << m_variableValueReference << " (reference)";

    formatter << " " << m_differences.toString();

    return formatter.str();
}

bool ComparisonData::isIsComparisonPassed() const {
    return m_isComparisonPassed;
}

void ComparisonData::setIsComparisonPassed(bool isComparisonPassed) {
    m_isComparisonPassed = isComparisonPassed;
}

} /* namespace PARTONS */
