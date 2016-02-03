#include "../../include/partons/ModuleObject.h"

#include "../../include/partons/utils/ParameterList.h"

ModuleObject::ModuleObject(const std::string &className) :
        BaseObject(className), Thread() {
}

ModuleObject::ModuleObject(const ModuleObject &other) :
        BaseObject(other), Thread(other) {
}

ModuleObject::~ModuleObject() {
    // Nothing to destroy
}

//TODO exception nothing to  configure
void ModuleObject::configure(ParameterList parameters) {

}

std::string ModuleObject::toString() const {
    return BaseObject::toString();
}

void ModuleObject::init() {

}
