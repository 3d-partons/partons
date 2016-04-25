#include "../../include/partons/ModuleObject.h"

const std::string ModuleObject::CLASS_NAME = "className";

ModuleObject::ModuleObject(const std::string &className) :
        BaseObject(className), ElemUtils::Thread() {
}

ModuleObject::ModuleObject(const ModuleObject &other) :
        BaseObject(other), ElemUtils::Thread(other) {
}

ModuleObject::~ModuleObject() {
    // Nothing to destroy
}

void ModuleObject::configure(const ElemUtils::Parameters &parameters) {
    // Nothing to do
}

std::string ModuleObject::toString() const {
    return BaseObject::toString();
}

void ModuleObject::resolveObjectDependencies() {
    BaseObject::resolveObjectDependencies();
}
