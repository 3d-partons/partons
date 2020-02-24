#include "../../../../../include/partons/modules/xi_converter/TCS/TCSXiConverterModule.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

const std::string TCSXiConverterModule::TCS_XI_CONVERTER_MODULE_CLASS_NAME =
        "TCSXiConverterModule";

TCSXiConverterModule::TCSXiConverterModule(const std::string &className) :
        XiConverterModule<TCSObservableKinematic>(className, ChannelType::TCS) {
}

TCSXiConverterModule::TCSXiConverterModule(const TCSXiConverterModule &other) :
        XiConverterModule<TCSObservableKinematic>(other) {
}

TCSXiConverterModule::~TCSXiConverterModule() {
}

void TCSXiConverterModule::initModule() {
}

void TCSXiConverterModule::isModuleWellConfigured() {
}

} /* namespace PARTONS */
