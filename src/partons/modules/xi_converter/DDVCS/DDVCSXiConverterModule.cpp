#include "../../../../../include/partons/modules/xi_converter/DDVCS/DDVCSXiConverterModule.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

const std::string DDVCSXiConverterModule::DDVCS_XI_CONVERTER_MODULE_CLASS_NAME =
        "DDVCSXiConverterModule";

DDVCSXiConverterModule::DDVCSXiConverterModule(const std::string &className) :
        XiConverterModule<DDVCSObservableKinematic>(className, ChannelType::DDVCS) {
}

DDVCSXiConverterModule::DDVCSXiConverterModule(const DDVCSXiConverterModule &other) :
        XiConverterModule<DDVCSObservableKinematic>(other) {
}

DDVCSXiConverterModule::~DDVCSXiConverterModule() {
}

void DDVCSXiConverterModule::initModule() {
}

void DDVCSXiConverterModule::isModuleWellConfigured() {
}

} /* namespace PARTONS */
