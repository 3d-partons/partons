#include "../../../../../include/partons/beans/observable/DVCS/DVCSObservableResult.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

DVCSObservableResult::DVCSObservableResult() :
        ObservableResult<DVCSObservableKinematic>("DVCSObservableResult",
                ChannelType::DVCS) {
}

DVCSObservableResult::DVCSObservableResult(
        const DVCSObservableKinematic& kinematic) :
        ObservableResult("DVCSObservableResult", ChannelType::DVCS, kinematic) {
}

DVCSObservableResult::DVCSObservableResult(const DVCSObservableResult& other) :
        ObservableResult<DVCSObservableKinematic>(other) {
}

DVCSObservableResult::~DVCSObservableResult() {
}

} /* namespace PARTONS */
