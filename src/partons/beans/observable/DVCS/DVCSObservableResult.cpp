#include "../../../../../include/partons/beans/observable/DVCS/DVCSObservableResult.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

DVCSObservableResult::DVCSObservableResult() :
        ObservableResult<DVCSObservableKinematic>("DVCSObservableResult",
                ChannelType::DVCS) {
}

DVCSObservableResult::DVCSObservableResult(const PhysicalType<double>& value) :
        ObservableResult("DVCSObservableResult", ChannelType::DVCS, value) {
}

DVCSObservableResult::DVCSObservableResult(
        const DVCSObservableKinematic& kinematic) :
        ObservableResult("DVCSObservableResult", ChannelType::DVCS, kinematic) {
}

DVCSObservableResult::DVCSObservableResult(const PhysicalType<double>& value,
        const DVCSObservableKinematic& kinematic, ObservableType::Type type) :
        ObservableResult("DVCSObservableResult", ChannelType::DVCS, value,
                kinematic, type) {
}

DVCSObservableResult::DVCSObservableResult(const DVCSObservableResult& other) :
        ObservableResult<DVCSObservableKinematic>(other) {
}

DVCSObservableResult::~DVCSObservableResult() {
}

} /* namespace PARTONS */
