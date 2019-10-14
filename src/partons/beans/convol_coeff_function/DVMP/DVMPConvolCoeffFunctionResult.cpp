#include "../../../../../include/partons/beans/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionResult.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

DVMPConvolCoeffFunctionResult::DVMPConvolCoeffFunctionResult() :
        ConvolCoeffFunctionResult<DVMPConvolCoeffFunctionKinematic>(
                "DVMPConvolCoeffFunctionResult", ChannelType::DVMP) {
}

DVMPConvolCoeffFunctionResult::DVMPConvolCoeffFunctionResult(
        const DVMPConvolCoeffFunctionKinematic& kinematic) :
        ConvolCoeffFunctionResult<DVMPConvolCoeffFunctionKinematic>(
                "DVMPConvolCoeffFunctionResult", ChannelType::DVMP, kinematic) {
}

DVMPConvolCoeffFunctionResult::DVMPConvolCoeffFunctionResult(
        const DVMPConvolCoeffFunctionResult& other) :
        ConvolCoeffFunctionResult(other) {
}

DVMPConvolCoeffFunctionResult::~DVMPConvolCoeffFunctionResult() {
}

} /* namespace PARTONS */
