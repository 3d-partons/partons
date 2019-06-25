#include "../../../../include/partons/utils/compare/ComparisonReport.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
//#include <stddef.h>

#include "../../../../include/partons/beans/system/EnvironmentConfiguration.h"
#include "../../../../include/partons/Partons.h"

namespace PARTONS {


ComparisonReport::ComparisonReport(const NumA::Tolerances &tolerances) :
        m_environmentSetting(ElemUtils::StringUtils::EMPTY), m_objectClassNameTested(
                ElemUtils::StringUtils::EMPTY), m_referenceObjectClassName(
                ElemUtils::StringUtils::EMPTY), m_tolerances(tolerances), m_numberOfComparedDataFailed(
                0), m_numberOfComparedDataPassed(0) {
    m_environmentSetting =
            Partons::getInstance()->getEnvironmentConfiguration()->getFile();
}

ComparisonReport::ComparisonReport(const std::string& environmentSetting,
        const std::string& objectClassNameTested,
        const std::string& referenceObjectClassName) :
        m_environmentSetting(environmentSetting), m_objectClassNameTested(
                objectClassNameTested), m_referenceObjectClassName(
                referenceObjectClassName), m_numberOfComparedDataFailed(0), m_numberOfComparedDataPassed(
                0) {
    Partons::getInstance()->getEnvironmentConfiguration()->getFile();
}

ComparisonReport::~ComparisonReport() {
}

std::string ComparisonReport::toString() const {

    ElemUtils::Formatter formatter;

    formatter << '\n' << "[Environment setting] \n" << m_environmentSetting
            << '\n' << '\n';

    formatter << "[Objects compared] \n" << m_objectClassNameTested
            << " compared to " << m_referenceObjectClassName << '\n' << '\n';

    formatter << "[Tolerances]" << '\n' << m_tolerances.toString() << '\n'
            << '\n';

//TODO ajouter un mode verbose pour supprimer l'affichage des tests réussi, ça n'a aucune utilité.
    formatter << "Number of objects compared with test PASSED = "
            << m_numberOfComparedDataPassed << '\n';
//    for (size_t i = 0; i != m_comparedDataPassed.size(); i++) {
//        formatter << m_comparedDataPassed[i].toString() << '\n' << '\n';
//    }

    formatter << '\n' << "Number of objects compared with test FAILED = "
            << m_numberOfComparedDataFailed << '\n';
    for (size_t i = 0; i != m_comparedDataFailed.size(); i++) {
        formatter << m_comparedDataFailed[i].toString() << '\n' << '\n';
    }

    return formatter.str();
}

const NumA::Tolerances& ComparisonReport::getTolerances() const {
    return m_tolerances;
}

void ComparisonReport::setTolerances(const NumA::Tolerances& tolerances) {
    this->m_tolerances = tolerances;
}

void ComparisonReport::addComparisonData(const ComparisonData& comparisonData) {
    if (comparisonData.isComparisonPassed()) {
        m_comparedDataPassed.push_back(comparisonData);
        m_numberOfComparedDataPassed += 1;
    } else {
        m_comparedDataFailed.push_back(comparisonData);
        m_numberOfComparedDataFailed += 1;
    }
}

bool ComparisonReport::isPassed() const {
    return (m_numberOfComparedDataFailed == 0) ? true : false;
}

void ComparisonReport::clearComparedData() {
    m_comparedDataFailed.clear();
    m_comparedDataPassed.clear();
}

std::string ComparisonReport::showComparedDataFailed() const {
    ElemUtils::Formatter formatter;

    for (size_t i = 0; i != m_comparedDataFailed.size(); i++) {
        formatter << m_comparedDataFailed[i].toString() << '\n' << '\n';
    }

    return formatter.str();
}

std::string ComparisonReport::showComparisonStats() const {
    ElemUtils::Formatter formatter;

    formatter << "Number of objects compared with test PASSED = "
            << m_numberOfComparedDataPassed << '\n';

    formatter << '\n' << "Number of objects compared with test FAILED = "
            << m_numberOfComparedDataFailed << '\n';

    return formatter.str();
}

size_t ComparisonReport::sizeOfComparedDataFailed() const {
    return m_comparedDataFailed.size();
}

} /* namespace PARTONS */
