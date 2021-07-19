#include "../../../../../include/partons/modules/scales/DVMP/DVMPScalesModule.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

const std::string DVMPScalesModule::DVMP_SCALES_MODULE_CLASS_NAME =
        "DVMPScalesModule";

DVMPScalesModule::DVMPScalesModule(const std::string &className) :
        ScalesModule<DVMPObservableKinematic>(className, ChannelType::DVMP) {
}

DVMPScalesModule::DVMPScalesModule(const DVMPScalesModule &other) :
        ScalesModule<DVMPObservableKinematic>(other) {
}

DVMPScalesModule::~DVMPScalesModule() {
}

void DVMPScalesModule::initModule() {
}

void DVMPScalesModule::isModuleWellConfigured() {
}

} /* namespace PARTONS */
