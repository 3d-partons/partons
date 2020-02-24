#include "../../../../../include/partons/modules/scales/TCS/TCSScalesModule.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

const std::string TCSScalesModule::TCS_SCALES_MODULE_CLASS_NAME =
        "TCSScalesModule";

TCSScalesModule::TCSScalesModule(const std::string &className) :
        ScalesModule<TCSObservableKinematic>(className, ChannelType::TCS) {
}

TCSScalesModule::TCSScalesModule(const TCSScalesModule &other) :
        ScalesModule<TCSObservableKinematic>(other) {
}

TCSScalesModule::~TCSScalesModule() {
}

void TCSScalesModule::initModule() {
}

void TCSScalesModule::isModuleWellConfigured() {
}

} /* namespace PARTONS */
