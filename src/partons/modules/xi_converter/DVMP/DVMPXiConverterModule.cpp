#include "../../../../../include/partons/modules/xi_converter/DVMP/DVMPXiConverterModule.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

const std::string DVMPXiConverterModule::DVMP_XI_CONVERTER_MODULE_CLASS_NAME =
        "DVMPXiConverterModule";

DVMPXiConverterModule::DVMPXiConverterModule(const std::string &className) :
        XiConverterModule<DVMPObservableKinematic>(className, ChannelType::DVMP) {
}

DVMPXiConverterModule::DVMPXiConverterModule(const DVMPXiConverterModule &other) :
        XiConverterModule<DVMPObservableKinematic>(other) {
}

DVMPXiConverterModule::~DVMPXiConverterModule() {
}

void DVMPXiConverterModule::initModule() {
}

void DVMPXiConverterModule::isModuleWellConfigured() {
}

} /* namespace PARTONS */
