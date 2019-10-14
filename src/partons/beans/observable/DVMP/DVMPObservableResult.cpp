#include "../../../../../include/partons/beans/observable/DVMP/DVMPObservableResult.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

DVMPObservableResult::DVMPObservableResult() :
        ObservableResult<DVMPObservableKinematic>("DVMPObservableResult",
                ChannelType::DVMP) {
}

DVMPObservableResult::DVMPObservableResult(const PhysicalType<double>& value) :
        ObservableResult("DVMPObservableResult", ChannelType::DVMP, value) {
}

DVMPObservableResult::DVMPObservableResult(
        const DVMPObservableKinematic& kinematic) :
        ObservableResult("DVMPObservableResult", ChannelType::DVMP, kinematic) {
}

DVMPObservableResult::DVMPObservableResult(const PhysicalType<double>& value,
        const DVMPObservableKinematic& kinematic) :
        ObservableResult("DVMPObservableResult", ChannelType::DVMP, value,
                kinematic) {
}

DVMPObservableResult::DVMPObservableResult(const DVMPObservableResult& other) :
        ObservableResult<DVMPObservableKinematic>(other) {
}

DVMPObservableResult::~DVMPObservableResult() {
}

} /* namespace PARTONS */
