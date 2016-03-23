#include "../../../../include/partons/modules/radon_inverse/RadonInverseModule.h"

#include "../../../../include/partons/BaseObjectRegistry.h"

// Initialise [class]::classId with a unique name.
const unsigned int RadonInverseModule::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new RadonInverseModule("RadonInverseModule"));

RadonInverseModule::RadonInverseModule(const std::string &className) :
        ModuleObject(className) {
}

RadonInverseModule::~RadonInverseModule() {
}

RadonInverseModule::RadonInverseModule(const RadonInverseModule& other) :
        ModuleObject(other) {
    //TODO implement
}

RadonInverseModule* RadonInverseModule::clone() const {
    return new RadonInverseModule(*this);
}

void RadonInverseModule::initModule() {
    //TODO implement
}

void RadonInverseModule::configure(const ElemUtils::Parameters& parameters) {
}

void RadonInverseModule::isModuleWellConfigured() {
    //TODO implement
}

