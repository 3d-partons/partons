#include "../../../../../include/partons/modules/scales/JET/JETScalesModule.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

const std::string JETScalesModule::JET_SCALES_MODULE_CLASS_NAME =
        "JETScalesModule";

JETScalesModule::JETScalesModule(const std::string &className) :
        ScalesModule<JETObservableKinematic>(className, ChannelType::JET) {
}

JETScalesModule::JETScalesModule(const JETScalesModule &other) :
        ScalesModule<JETObservableKinematic>(other) {
}

JETScalesModule::~JETScalesModule() {
}

void JETScalesModule::initModule() {
}

void JETScalesModule::isModuleWellConfigured() {
}

} /* namespace PARTONS */
