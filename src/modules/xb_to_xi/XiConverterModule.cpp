#include "XiConverterModule.h"

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
