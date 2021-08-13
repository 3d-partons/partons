#include "../../../../../include/partons/beans/convol_coeff_function/GAM2/GAM2ConvolCoeffFunctionResult.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

GAM2ConvolCoeffFunctionResult::GAM2ConvolCoeffFunctionResult() :
        ConvolCoeffFunctionResult<GAM2ConvolCoeffFunctionKinematic>(
                "GAM2ConvolCoeffFunctionResult", ChannelType::GAM2) {
}

GAM2ConvolCoeffFunctionResult::GAM2ConvolCoeffFunctionResult(
        const GAM2ConvolCoeffFunctionKinematic& kinematic) :
        ConvolCoeffFunctionResult<GAM2ConvolCoeffFunctionKinematic>(
                "GAM2ConvolCoeffFunctionResult", ChannelType::GAM2, kinematic) {
}

GAM2ConvolCoeffFunctionResult::GAM2ConvolCoeffFunctionResult(
        const GAM2ConvolCoeffFunctionResult& other) :
        ConvolCoeffFunctionResult(other) {
}

GAM2ConvolCoeffFunctionResult::~GAM2ConvolCoeffFunctionResult() {
}

} /* namespace PARTONS */
