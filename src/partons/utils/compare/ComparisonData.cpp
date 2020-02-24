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

    formatter << m_variableName << " = " << m_variableValue
            << " (computed) compared to = " << m_variableValueReference
            << " (reference)";

    formatter << " " << m_differences.toString();

    return formatter.str();
}

bool ComparisonData::isComparisonPassed() const {
    return m_isComparisonPassed;
}

void ComparisonData::setComparisonPassed(bool isComparisonPassed) {
    m_isComparisonPassed = isComparisonPassed;
}

const std::string& PARTONS::ComparisonData::getVariableName() const {
    return m_variableName;
}

void PARTONS::ComparisonData::setVariableName(const std::string& variableName) {
    m_variableName = variableName;
}

const std::string& PARTONS::ComparisonData::getVariableValue() const {
    return m_variableValue;
}

void PARTONS::ComparisonData::setVariableValue(
        const std::string& variableValue) {
    m_variableValue = variableValue;
}

const std::string& PARTONS::ComparisonData::getVariableValueReference() const {
    return m_variableValueReference;
}

void PARTONS::ComparisonData::setVariableValueReference(
        const std::string& variableValueReference) {
    m_variableValueReference = variableValueReference;
}

const NumA::Differences& PARTONS::ComparisonData::getDifferences() const {
    return m_differences;
}

void PARTONS::ComparisonData::setDifferences(
        const NumA::Differences& differences) {
    m_differences = differences;
}

const std::string& PARTONS::ComparisonData::getObjectInfo() const {
    return m_objectInfo;
}

void PARTONS::ComparisonData::setObjectInfo(const std::string& objectInfo) {
    m_objectInfo = objectInfo;
}

} /* namespace PARTONS */
