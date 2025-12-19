#include "../../../../../include/partons/beans/observable/JET/JETObservableResult.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

JETObservableResult::JETObservableResult() :
        ObservableResult<JETObservableKinematic>("JETObservableResult",
                ChannelType::JET) {
}

JETObservableResult::JETObservableResult(const PhysicalType<double>& value) :
        ObservableResult("JETObservableResult", ChannelType::JET, value) {
}

JETObservableResult::JETObservableResult(
        const JETObservableKinematic& kinematic) :
        ObservableResult("JETObservableResult", ChannelType::JET, kinematic) {
}

JETObservableResult::JETObservableResult(const PhysicalType<double>& value,
        const JETObservableKinematic& kinematic) :
        ObservableResult("JETObservableResult", ChannelType::JET, value,
                kinematic) {
}

JETObservableResult::JETObservableResult(const JETObservableResult& other) :
        ObservableResult<JETObservableKinematic>(other) {
}

JETObservableResult::~JETObservableResult() {
}

} /* namespace PARTONS */
