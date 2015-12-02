#include "ComparisonData.h"

#include "../utils/stringUtils/Formatter.h"

ComparisonData::ComparisonData() {
}

ComparisonData::ComparisonData(const std::string& variableName,
        const std::string& additionalInfo) :
        m_variableName(variableName), m_additionalInfo(additionalInfo) {
}

ComparisonData::~ComparisonData() {
}

std::string ComparisonData::toString() const {
    Formatter formatter;

    formatter << "variableName = " << m_variableName << " additonalInfo = "
            << m_additionalInfo;

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
