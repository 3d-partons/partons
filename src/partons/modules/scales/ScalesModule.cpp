#include "../../../../include/partons/modules/scales/ScalesModule.h"

#include "../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

const std::string ScalesModule::SCALES_MODULE_CLASS_NAME = "ScalesModule";

ScalesModule::ScalesModule(const std::string &className) :
        ModuleObject(className, ChannelType::UNDEFINED) {
}

ScalesModule::~ScalesModule() {
}

ScalesModule::ScalesModule(const ScalesModule &other) :
        ModuleObject(other) {

}

void ScalesModule::initModule() {
    //TODO implement
}

void ScalesModule::isModuleWellConfigured() {
    //TODO implement
}

void ScalesModule::configure(const ElemUtils::Parameters &parameters) {
    ModuleObject::configure(parameters);
}

void ScalesModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    ModuleObject::prepareSubModules(subModulesData);

    // Nothing to do.
}

} /* namespace PARTONS */
