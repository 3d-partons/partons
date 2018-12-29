#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

DVCSConvolCoeffFunctionResult::DVCSConvolCoeffFunctionResult() :
        ConvolCoeffFunctionResult("DVCSConvolCoeffFunctionResult",
                ChannelType::DVCS) {
}

DVCSConvolCoeffFunctionResult::DVCSConvolCoeffFunctionResult(
        const DVCSConvolCoeffFunctionKinematic& kinematic) :
        ConvolCoeffFunctionResult("DVCSConvolCoeffFunctionResult",
                ChannelType::DVCS), m_kinematic(kinematic) {
}

DVCSConvolCoeffFunctionResult::DVCSConvolCoeffFunctionResult(
        const DVCSConvolCoeffFunctionResult& other) :
        ConvolCoeffFunctionResult(other) {
    m_kinematic = other.m_kinematic;
}

DVCSConvolCoeffFunctionResult::~DVCSConvolCoeffFunctionResult() {
}

std::string DVCSConvolCoeffFunctionResult::toString() const {

    ElemUtils::Formatter formatter;

    formatter << ConvolCoeffFunctionResult::toString();
    formatter << "Kinematics: " << m_kinematic.toString();

    return formatter.str();
}

bool DVCSConvolCoeffFunctionResult::operator <(
        const DVCSConvolCoeffFunctionResult& other) const {
    return (m_kinematic < other.m_kinematic);
}

const DVCSConvolCoeffFunctionKinematic& DVCSConvolCoeffFunctionResult::getKinematic() const {
    return m_kinematic;
}

void DVCSConvolCoeffFunctionResult::setKinematic(
        const DVCSConvolCoeffFunctionKinematic& kinematic) {
    m_kinematic = kinematic;
}

} /* namespace PARTONS */
