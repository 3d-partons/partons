#include "../../../../include/partons/beans/gpd/GPDSubtractionConstantResult.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../include/partons/beans/system/ResultInfo.h"
#include "../../../../include/partons/utils/compare/CompareUtils.h"
#include "../../../../include/partons/utils/compare/ComparisonData.h"

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

void GPDSubtractionConstantResult::compare(
        ComparisonReport &rootComparisonReport,
        const GPDSubtractionConstantResult &referenceObject,
        std::string parentObjectInfo) const {

    if (m_kinematic != referenceObject.getKinematic()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Cannot perform comparison because kinematics is different ; With GPDSubtractionConstantResult index id = "
                        << referenceObject.getIndexId() << '\n' << toString()
                        << '\n' << referenceObject.toString());
    }

    ComparisonData comparisonData = CompareUtils::compareDouble(
            "observable value", getValue().getValue(),
            referenceObject.getValue().getValue(),
            rootComparisonReport.getTolerances(),
            ElemUtils::Formatter() << parentObjectInfo
                    << this->getResultInfo().toString());
    rootComparisonReport.addComparisonData(comparisonData);
}

const PhysicalType<double>& GPDSubtractionConstantResult::getValue() const {
    return m_value;
}

void GPDSubtractionConstantResult::setValue(const PhysicalType<double>& value) {
    m_value = value;
}

} /* namespace PARTONS */
