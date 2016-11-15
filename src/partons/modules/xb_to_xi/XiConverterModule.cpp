#include "../../../../include/partons/modules/xb_to_xi/XiConverterModule.h"

const std::string XiConverterModule::XI_CONVERTER_MODULE_CLASS_NAME =
        "XiConverterModule";

XiConverterModule::XiConverterModule(const std::string &className) :
        ModuleObject(className) {
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
    // Nothing to do.
}
