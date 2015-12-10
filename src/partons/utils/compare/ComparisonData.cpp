#include "../../../../include/partons/utils/compare/ComparisonData.h"

#include "../../../../include/partons/utils/compare/Differences.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"

//ComparisonData::ComparisonData() :
//        m_pDifferences(0), m_variableName(StringUtils::EMPTY), m_additionalInfo(
//                StringUtils::EMPTY) {
//}

ComparisonData::ComparisonData(bool isFailed, const std::string& variableName,
        const std::string& variableValue,
        const std::string& variableValueReference) :
        m_failed(isFailed), m_variableName(variableName), m_variableValue(
                variableValue), m_variableValueReference(
                variableValueReference), m_pTolerances(0), m_pDifferences(0) {
}

ComparisonData::ComparisonData(bool isFailed, const std::string& variableName,
        const std::string& variableValue,
        const std::string& variableValueReference, Tolerances* pTolerances,
        Differences* pDifferences) :
        m_failed(isFailed), m_variableName(variableName), m_variableValue(
                variableValue), m_variableValueReference(
                variableValueReference), m_pTolerances(pTolerances), m_pDifferences(
                pDifferences) {
}

ComparisonData::~ComparisonData() {
}

std::string ComparisonData::toString() const {
    Formatter formatter;

    if (m_failed) {
        formatter << "[FAILED]";
    } else {
        formatter << "[PASSED]";
    }

    formatter << " variableName = " << m_variableName << " with value = "
            << m_variableValue << " compared to = " << m_variableValueReference;

    if (m_pDifferences != 0) {
        formatter << " " << m_pDifferences->toString();
    }

    return formatter.str();
}

void ComparisonData::setAdditionalInfo(const std::string& additionalInfo) {
    m_additionalInfo = additionalInfo;
}

void ComparisonData::setVariableName(const std::string& variableName) {
    m_variableName = variableName;
}

const std::string& ComparisonData::getAdditionalInfo() const {
    return m_additionalInfo;
}

const std::string& ComparisonData::getVariableName() const {
    return m_variableName;
}

