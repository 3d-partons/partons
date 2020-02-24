#include "../../../../../include/partons/beans/observable/TCS/TCSObservableResult.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

TCSObservableResult::TCSObservableResult() :
        ObservableResult<TCSObservableKinematic>("TCSObservableResult",
                ChannelType::TCS) {
}

TCSObservableResult::TCSObservableResult(const PhysicalType<double>& value) :
        ObservableResult("TCSObservableResult", ChannelType::TCS, value) {
}

TCSObservableResult::TCSObservableResult(
        const TCSObservableKinematic& kinematic) :
        ObservableResult("TCSObservableResult", ChannelType::TCS, kinematic) {
}

TCSObservableResult::TCSObservableResult(const PhysicalType<double>& value,
        const TCSObservableKinematic& kinematic) :
        ObservableResult("TCSObservableResult", ChannelType::TCS, value,
                kinematic) {
}

TCSObservableResult::TCSObservableResult(const TCSObservableResult& other) :
        ObservableResult<TCSObservableKinematic>(other) {
}

TCSObservableResult::~TCSObservableResult() {
}

} /* namespace PARTONS */
