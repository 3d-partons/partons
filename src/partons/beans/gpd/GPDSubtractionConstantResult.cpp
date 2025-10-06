#include "../../../../include/partons/beans/gpd/GPDSubtractionConstantResult.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

GPDSubtractionConstantResult::GPDSubtractionConstantResult() :
        Result<GPDSubtractionConstantKinematic>("GPDSubtractionConstantResult",
                ChannelType::UNDEFINED) {
}

GPDSubtractionConstantResult::GPDSubtractionConstantResult(
        const PhysicalType<double>& value) :
        Result<GPDSubtractionConstantKinematic>("GPDSubtractionConstantResult",
                ChannelType::UNDEFINED), m_value(value) {
}

GPDSubtractionConstantResult::GPDSubtractionConstantResult(
        const GPDSubtractionConstantKinematic& kinematic) :
        Result<GPDSubtractionConstantKinematic>("GPDSubtractionConstantResult",
                ChannelType::UNDEFINED, kinematic) {
}

GPDSubtractionConstantResult::GPDSubtractionConstantResult(
        const PhysicalType<double>& value,
        const GPDSubtractionConstantKinematic& kinematic) :
        Result<GPDSubtractionConstantKinematic>("GPDSubtractionConstantResult",
                ChannelType::UNDEFINED, kinematic), m_value(value) {
}

GPDSubtractionConstantResult::GPDSubtractionConstantResult(
        const GPDSubtractionConstantResult &other) :
        Result<GPDSubtractionConstantKinematic>(other) {
    m_value = other.m_value;
}

GPDSubtractionConstantResult::~GPDSubtractionConstantResult() {
}

std::string GPDSubtractionConstantResult::toString() const {

    ElemUtils::Formatter formatter;

    formatter << "\n" << Result<GPDSubtractionConstantKinematic>::toString()
            << "\n\n";
    formatter << "Result: " << m_value.toString();
    formatter << '\n';

    return formatter.str();
}

const PhysicalType<double>& GPDSubtractionConstantResult::getValue() const {
    return m_value;
}

void GPDSubtractionConstantResult::setValue(const PhysicalType<double>& value) {
    m_value = value;
}

} /* namespace PARTONS */
