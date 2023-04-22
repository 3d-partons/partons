#include "../../../../../include/partons/beans/observable/DDVCS/DDVCSObservableResult.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

DDVCSObservableResult::DDVCSObservableResult() :
        ObservableResult<DDVCSObservableKinematic>("DDVCSObservableResult",
                ChannelType::DDVCS) {
}

DDVCSObservableResult::DDVCSObservableResult(const PhysicalType<double>& value) :
        ObservableResult("DDVCSObservableResult", ChannelType::DDVCS, value) {
}

DDVCSObservableResult::DDVCSObservableResult(
        const DDVCSObservableKinematic& kinematic) :
        ObservableResult("DDVCSObservableResult", ChannelType::DDVCS, kinematic) {
}

DDVCSObservableResult::DDVCSObservableResult(const PhysicalType<double>& value,
        const DDVCSObservableKinematic& kinematic) :
        ObservableResult("DDVCSObservableResult", ChannelType::DDVCS, value,
                kinematic) {
}

DDVCSObservableResult::DDVCSObservableResult(const DDVCSObservableResult& other) :
        ObservableResult<DDVCSObservableKinematic>(other) {
}

DDVCSObservableResult::~DDVCSObservableResult() {
}

} /* namespace PARTONS */
