#include "../../../../../include/partons/modules/scales/DVCS/DVCSScalesModule.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

const std::string DVCSScalesModule::DVCS_SCALES_MODULE_CLASS_NAME =
        "DVCSScalesModule";

DVCSScalesModule::DVCSScalesModule(const std::string &className) :
        ScalesModule<DVCSObservableKinematic>(className, ChannelType::DVCS) {
}

DVCSScalesModule::DVCSScalesModule(const DVCSScalesModule &other) :
        ScalesModule<DVCSObservableKinematic>(other) {
}

DVCSScalesModule::~DVCSScalesModule() {
}

void DVCSScalesModule::initModule() {
}

void DVCSScalesModule::isModuleWellConfigured() {
}

} /* namespace PARTONS */
