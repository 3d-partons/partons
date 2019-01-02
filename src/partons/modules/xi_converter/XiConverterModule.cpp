#include "../../../../include/partons/modules/xi_converter/XiConverterModule.h"

#include "../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

const std::string XiConverterModule::XI_CONVERTER_MODULE_CLASS_NAME =
        "XiConverterModule";

XiConverterModule::XiConverterModule(const std::string &className) :
        ModuleObject(className, ChannelType::UNDEFINED) {
}

XiConverterModule::~XiConverterModule() {
}

XiConverterModule::XiConverterModule(const XiConverterModule& other) :
        ModuleObject(other) {
}

void XiConverterModule::isModuleWellConfigured() {
}

void XiConverterModule::initModule() {
}

void XiConverterModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    ModuleObject::prepareSubModules(subModulesData);

    // Nothing to do.
}

} /* namespace PARTONS */
