#include "../../../../include/partons/utils/compare/ComparisonReport.h"

#include <stddef.h>

#include "../../../../include/partons/utils/stringUtils/Formatter.h"

ComparisonReport::ComparisonReport() {

}

ComparisonReport::ComparisonReport(const std::string& environmentSetting,
        const std::string& objectClassNameTested,
        const std::string& referenceObjectClassName) :
        m_environmentSetting(environmentSetting), m_objectClassNameTested(
                objectClassNameTested), m_referenceObjectClassName(
                referenceObjectClassName) {
}

ComparisonReport::~ComparisonReport() {
}

std::string ComparisonReport::toString() const {

    Formatter formatter;

    formatter << '\n' << "[Environment setting] \n" << m_environmentSetting
            << '\n' << '\n';

    formatter << "[Objects compared] \n" << m_objectClassNameTested
            << " compared to " << m_referenceObjectClassName << '\n' << '\n';

    formatter << "[Tolerances]" << '\n' << m_tolerances.toString() << '\n'
            << '\n';

//TODO ajouter un mode verbose pour supprimer l'affichage des tests réussi, ça n'a aucune utilité.
    formatter << "Number of objects compared with test PASSED = "
            << m_comparedDataPassed.size() << '\n';
    for (size_t i = 0; i != m_comparedDataPassed.size(); i++) {
        formatter << m_comparedDataPassed[i].toString() << '\n' << '\n';
    }

    formatter << '\n' << "Number of objects compared with test FAILED = "
            << m_comparedDataFailed.size() << '\n';
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
    if (comparisonData.isIsComparisonPassed()) {
        m_comparedDataPassed.push_back(comparisonData);
    } else {
        m_comparedDataFailed.push_back(comparisonData);
    }
}

bool ComparisonReport::isPassed() const {
    return (m_comparedDataFailed.size() == 0) ? true : false;
}
