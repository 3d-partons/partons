#include "../../../../include/partons/beans/observable/ObservableResult.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/utils/compare/CompareUtils.h"
#include "../../../../include/partons/utils/compare/ComparisonData.h"

namespace PARTONS {

ObservableResult::ObservableResult(const std::string &className,
        ChannelType::Type channelType) :
        Result(className, channelType), m_value(0.), m_observableType(
                ObservableType::UNDEFINED) {
}

ObservableResult::ObservableResult(const ObservableResult& other) :
        Result(other), m_value(other.m_value), m_observableType(
                other.m_observableType) {
}

ObservableResult::~ObservableResult() {
}

std::string ObservableResult::toString() const {

    ElemUtils::Formatter formatter;

    formatter << Result::toString();

    formatter << "\tOservable: " << m_value << " type: "
            << ObservableType(m_observableType).toString() << '\n';

    return formatter.str();
}

void ObservableResult::set(double value, ObservableType::Type observableType) {

    m_value = value;
    m_observableType = observableType;
}

void ObservableResult::compare(ComparisonReport &rootComparisonReport,
        const ObservableResult &referenceObject,
        std::string parentObjectInfo) const {

    //TODO faire un test pour valider la cinématique associée

    if (m_observableType != referenceObject.getObservableType()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Cannot compare objects, they are different (different name or type)");
    }

    ComparisonData xb_comparisonData = CompareUtils::compareDouble(
            "observable value", getValue(), referenceObject.getValue(),
            rootComparisonReport.getTolerances(),
            ElemUtils::Formatter() << parentObjectInfo
                    << this->getObjectInfo());
    rootComparisonReport.addComparisonData(xb_comparisonData);
}

double ObservableResult::getValue() const {
    return m_value;
}

void ObservableResult::setValue(double value) {
    m_value = value;
}

void ObservableResult::setObservableType(ObservableType::Type observableType) {
    m_observableType = observableType;
}

ObservableType::Type ObservableResult::getObservableType() const {
    return m_observableType;
}

} /* namespace PARTONS */
