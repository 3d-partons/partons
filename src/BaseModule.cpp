#include "BaseModule.h"

BaseModule::BaseModule(std::string _ID) : BaseObject(_ID),
        ID(_ID) {
}

BaseModule::~BaseModule() {
}

size_t BaseModule::getNbOfParameters() {
    return parameters.size();
}

const std::vector<double>& BaseModule::getParameters() const {
    return parameters;
}

void BaseModule::setParameters(const std::vector<double>& parameters) {
    this->parameters = parameters;
}

const std::string& BaseModule::getID() const {
    return ID;
}
