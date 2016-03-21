#include "../../../../include/partons/modules/radon_inverse/RadonInverseModule.h"

RadonInverseModule::RadonInverseModule() :
        ModuleObject("RadonInverseModule") {
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

void RadonInverseModule::isModuleWellConfigured() {
    //TODO implement
}
