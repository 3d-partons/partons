#include "../../../../../include/partons/modules/scales/GAM2/GAM2ScalesModule.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

const std::string GAM2ScalesModule::GAM2_SCALES_MODULE_CLASS_NAME =
        "GAM2ScalesModule";

GAM2ScalesModule::GAM2ScalesModule(const std::string &className) :
        ScalesModule<GAM2ObservableKinematic>(className, ChannelType::GAM2) {
}

GAM2ScalesModule::GAM2ScalesModule(const GAM2ScalesModule &other) :
        ScalesModule<GAM2ObservableKinematic>(other) {
}

GAM2ScalesModule::~GAM2ScalesModule() {
}

void GAM2ScalesModule::initModule() {
}

void GAM2ScalesModule::isModuleWellConfigured() {
}

} /* namespace PARTONS */
