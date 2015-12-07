#include "../../../../include/partons/modules/scale/ScaleModule.h"

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
