#include "../../../../../include/partons/modules/xi_converter/GAM2/GAM2XiConverterModule.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

const std::string GAM2XiConverterModule::GAM2_XI_CONVERTER_MODULE_CLASS_NAME =
        "GAM2XiConverterModule";

GAM2XiConverterModule::GAM2XiConverterModule(const std::string &className) :
        XiConverterModule<GAM2ObservableKinematic>(className, ChannelType::GAM2) {
}

GAM2XiConverterModule::GAM2XiConverterModule(const GAM2XiConverterModule &other) :
        XiConverterModule<GAM2ObservableKinematic>(other) {
}

GAM2XiConverterModule::~GAM2XiConverterModule() {
}

void GAM2XiConverterModule::initModule() {
}

void GAM2XiConverterModule::isModuleWellConfigured() {
}

} /* namespace PARTONS */
