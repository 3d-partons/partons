#include "../../../../../include/partons/modules/xi_converter/DVCS/DVCSXiConverterModule.h"

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

const std::string DVCSXiConverterModule::DVCS_XI_CONVERTER_MODULE_CLASS_NAME =
        "DVCSXiConverterModule";

DVCSXiConverterModule::DVCSXiConverterModule(const std::string &className) :
        XiConverterModule<DVCSObservableKinematic>(className, ChannelType::DVCS) {
}

DVCSXiConverterModule::DVCSXiConverterModule(const DVCSXiConverterModule &other) :
        XiConverterModule<DVCSObservableKinematic>(other) {
}

DVCSXiConverterModule::~DVCSXiConverterModule() {
}

void DVCSXiConverterModule::initModule() {
}

void DVCSXiConverterModule::isModuleWellConfigured() {
}

} /* namespace PARTONS */
