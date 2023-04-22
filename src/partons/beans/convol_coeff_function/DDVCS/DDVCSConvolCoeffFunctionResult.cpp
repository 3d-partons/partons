#include "../../../../../include/partons/beans/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionResult.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

DDVCSConvolCoeffFunctionResult::DDVCSConvolCoeffFunctionResult() :
        ConvolCoeffFunctionResult<DDVCSConvolCoeffFunctionKinematic>(
                "DDVCSConvolCoeffFunctionResult", ChannelType::DDVCS) {
}

DDVCSConvolCoeffFunctionResult::DDVCSConvolCoeffFunctionResult(
        const DDVCSConvolCoeffFunctionKinematic& kinematic) :
        ConvolCoeffFunctionResult<DDVCSConvolCoeffFunctionKinematic>(
                "DDVCSConvolCoeffFunctionResult", ChannelType::DDVCS, kinematic) {
}

DDVCSConvolCoeffFunctionResult::DDVCSConvolCoeffFunctionResult(
        const DDVCSConvolCoeffFunctionResult& other) :
        ConvolCoeffFunctionResult(other) {
}

DDVCSConvolCoeffFunctionResult::~DDVCSConvolCoeffFunctionResult() {
}

} /* namespace PARTONS */
