#include "../../../../../include/partons/beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionResult.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

TCSConvolCoeffFunctionResult::TCSConvolCoeffFunctionResult() :
        ConvolCoeffFunctionResult<TCSConvolCoeffFunctionKinematic>(
                "TCSConvolCoeffFunctionResult", ChannelType::TCS) {
}

TCSConvolCoeffFunctionResult::TCSConvolCoeffFunctionResult(
        const TCSConvolCoeffFunctionKinematic& kinematic) :
        ConvolCoeffFunctionResult<TCSConvolCoeffFunctionKinematic>(
                "TCSConvolCoeffFunctionResult", ChannelType::TCS, kinematic) {
}

TCSConvolCoeffFunctionResult::TCSConvolCoeffFunctionResult(
        const TCSConvolCoeffFunctionResult& other) :
        ConvolCoeffFunctionResult(other) {
}

TCSConvolCoeffFunctionResult::~TCSConvolCoeffFunctionResult() {
}

} /* namespace PARTONS */
