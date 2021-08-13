#include "../../../../../include/partons/beans/observable/GAM2/GAM2ObservableResult.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

GAM2ObservableResult::GAM2ObservableResult() :
        ObservableResult<GAM2ObservableKinematic>("GAM2ObservableResult",
                ChannelType::GAM2) {
}

GAM2ObservableResult::GAM2ObservableResult(const PhysicalType<double>& value) :
        ObservableResult("GAM2ObservableResult", ChannelType::GAM2, value) {
}

GAM2ObservableResult::GAM2ObservableResult(
        const GAM2ObservableKinematic& kinematic) :
        ObservableResult("GAM2ObservableResult", ChannelType::GAM2, kinematic) {
}

GAM2ObservableResult::GAM2ObservableResult(const PhysicalType<double>& value,
        const GAM2ObservableKinematic& kinematic) :
        ObservableResult("GAM2ObservableResult", ChannelType::GAM2, value,
                kinematic) {
}

GAM2ObservableResult::GAM2ObservableResult(const GAM2ObservableResult& other) :
        ObservableResult<GAM2ObservableKinematic>(other) {
}

GAM2ObservableResult::~GAM2ObservableResult() {
}

} /* namespace PARTONS */
