#include "../../../../../include/partons/beans/convol_coeff_function/JET/JETConvolCoeffFunctionResult.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

JETConvolCoeffFunctionResult::JETConvolCoeffFunctionResult() :
        ConvolCoeffFunctionResult<JETConvolCoeffFunctionKinematic>(
                "JETConvolCoeffFunctionResult", ChannelType::JET) {
}

JETConvolCoeffFunctionResult::JETConvolCoeffFunctionResult(
        const JETConvolCoeffFunctionKinematic& kinematic) :
        ConvolCoeffFunctionResult<JETConvolCoeffFunctionKinematic>(
                "JETConvolCoeffFunctionResult", ChannelType::JET, kinematic) {
}

JETConvolCoeffFunctionResult::JETConvolCoeffFunctionResult(
        const JETConvolCoeffFunctionResult& other) :
        ConvolCoeffFunctionResult(other) {
}

JETConvolCoeffFunctionResult::~JETConvolCoeffFunctionResult() {
}

} /* namespace PARTONS */
