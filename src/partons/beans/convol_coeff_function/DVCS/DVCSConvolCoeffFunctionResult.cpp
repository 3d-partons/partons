#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

DVCSConvolCoeffFunctionResult::DVCSConvolCoeffFunctionResult() :
        ConvolCoeffFunctionResult<DVCSConvolCoeffFunctionKinematic>(
                "DVCSConvolCoeffFunctionResult", ChannelType::DVCS) {
}

DVCSConvolCoeffFunctionResult::DVCSConvolCoeffFunctionResult(
        const DVCSConvolCoeffFunctionKinematic& kinematic) :
        ConvolCoeffFunctionResult<DVCSConvolCoeffFunctionKinematic>(
                "DVCSConvolCoeffFunctionResult", ChannelType::DVCS, kinematic) {
}

DVCSConvolCoeffFunctionResult::DVCSConvolCoeffFunctionResult(
        const DVCSConvolCoeffFunctionResult& other) :
        ConvolCoeffFunctionResult(other) {
}

DVCSConvolCoeffFunctionResult::~DVCSConvolCoeffFunctionResult() {
}

} /* namespace PARTONS */
