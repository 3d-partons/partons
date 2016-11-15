#include "../../../../include/partons/modules/scale/ScaleModule.h"

const std::string ScaleModule::SCALE_MODULE_CLASS_NAME = "ScaleModule";

ScaleModule::ScaleModule(const std::string &className) :
        ModuleObject(className) {
}

ScaleModule::~ScaleModule() {
}

ScaleModule::ScaleModule(const ScaleModule &other) :
        ModuleObject(other) {

}

void ScaleModule::initModule() {
    //TODO implement
}

void ScaleModule::isModuleWellConfigured() {
    //TODO implement
}

void ScaleModule::configure(const ElemUtils::Parameters &parameters) {
    ModuleObject::configure(parameters);
}

void ScaleModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    // Nothing to do.
}
