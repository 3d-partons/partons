#include "../../../../include/partons/beans/observable/ObservableResult.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/utils/compare/CompareUtils.h"
#include "../../../../include/partons/utils/compare/ComparisonData.h"

namespace PARTONS {

const std::string ObservableResult::PARAMETER_NAME_OBSERVABLE_VALUE =
        "observable value";

ObservableResult::ObservableResult() :
        Result("ObservableResult"), m_observableName("UNDEFINED"), m_value(0.), m_observableType(
                ObservableType::UNDEFINED) {
}

ObservableResult::ObservableResult(const std::string &observableName,
        double value) :
        Result("ObservableResult"), m_observableName(observableName), m_value(
                value), m_observableType(ObservableType::UNDEFINED) {
}

ObservableResult::~ObservableResult() {
}

std::string ObservableResult::toString() const {
    return ElemUtils::Formatter() << m_kinematic.toString() << " observable = "
            << m_value;
}

void ObservableResult::compare(ComparisonReport &rootComparisonReport,
        const ObservableResult &referenceObject,
        std::string parentObjectInfo) const {

    //TODO faire un test pour valider la cinématique associée

    if (m_observableType != referenceObject.getObservableType()
            || !(ElemUtils::StringUtils::equalsIgnoreCase(m_observableName,
                    referenceObject.getObservableName()))) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Cannot compare objects, they are different (different name or type)");
    }

    ComparisonData xb_comparisonData = CompareUtils::compareDouble(
            ObservableResult::PARAMETER_NAME_OBSERVABLE_VALUE, getValue(),
            referenceObject.getValue(), rootComparisonReport.getTolerances(),
            ElemUtils::Formatter() << parentObjectInfo
                    << this->getObjectInfo());
    rootComparisonReport.addComparisonData(xb_comparisonData);
}

// ##### GETTERS & SETTERS #####

void ObservableResult::setObservableType(ObservableType::Type observableType) {
    m_observableType = observableType;
}

const ObservableKinematic& ObservableResult::getKinematic() const {
    return m_kinematic;
}

ObservableType::Type ObservableResult::getObservableType() const {
    return m_observableType;
}

const std::string& ObservableResult::getObservableName() const {
    return m_observableName;
}

const ErrorBar& ObservableResult::getStatError() const {
    return m_statError;
}

const ErrorBar& ObservableResult::getSystError() const {
    return m_systError;
}

const ErrorBar& ObservableResult::getScaleError() const {
    return m_scaleError;
}

double ObservableResult::getValue() const {
    return m_value;
}

void ObservableResult::setKinematic(const ObservableKinematic &kinematic) {
    m_kinematic = kinematic;
}

void ObservableResult::setStatError(const ErrorBar& statError) {
    m_statError = statError;
}

void ObservableResult::setSystError(const ErrorBar& systError) {
    m_systError = systError;
}

void ObservableResult::setScaleError(const ErrorBar& scaleError) {
    m_scaleError = scaleError;
}

void ObservableResult::setValue(double value) {
    m_value = value;
}

std::string ObservableResult::getObjectInfo() const {
    return ElemUtils::Formatter() << "Observable " << m_observableName
            << " with kinematic( " << m_kinematic.toString() << ")";
}

bool ObservableResult::operator <(const ObservableResult& other) const {
    return (m_kinematic < other.m_kinematic);
}

} /* namespace PARTONS */
