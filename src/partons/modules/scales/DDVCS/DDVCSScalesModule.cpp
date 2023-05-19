#include "../../../../../include/partons/modules/scales/DDVCS/DDVCSScalesModule.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

const std::string DDVCSScalesModule::DDVCS_SCALES_MODULE_CLASS_NAME =
        "DDVCSScalesModule";

DDVCSScalesModule::DDVCSScalesModule(const std::string &className) :
        ScalesModule<DDVCSObservableKinematic>(className, ChannelType::DDVCS) {
}

DDVCSScalesModule::DDVCSScalesModule(const DDVCSScalesModule &other) :
        ScalesModule<DDVCSObservableKinematic>(other) {
}

DDVCSScalesModule::~DDVCSScalesModule() {
}

void DDVCSScalesModule::initModule() {
}

void DDVCSScalesModule::isModuleWellConfigured() {
}

} /* namespace PARTONS */
