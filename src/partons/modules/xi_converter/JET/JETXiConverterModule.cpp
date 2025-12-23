#include "../../../../../include/partons/modules/xi_converter/JET/JETXiConverterModule.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

const std::string JETXiConverterModule::JET_XI_CONVERTER_MODULE_CLASS_NAME =
        "JETXiConverterModule";

JETXiConverterModule::JETXiConverterModule(const std::string &className) :
        XiConverterModule<JETObservableKinematic>(className, ChannelType::JET) {
}

JETXiConverterModule::JETXiConverterModule(const JETXiConverterModule &other) :
        XiConverterModule<JETObservableKinematic>(other) {
}

JETXiConverterModule::~JETXiConverterModule() {
}

void JETXiConverterModule::initModule() {
}

void JETXiConverterModule::isModuleWellConfigured() {
}

} /* namespace PARTONS */
