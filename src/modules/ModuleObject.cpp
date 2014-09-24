#include "ModuleObject.h"

//#include <iostream>

unsigned int ModuleObject::m_uniqueID = 0;

ModuleObject::ModuleObject(const std::string & moduleID) :
        BaseObject(moduleID), m_id(getUniqueID()), m_moduleID(moduleID) {
}

ModuleObject::ModuleObject(const ModuleObject &other) :
        BaseObject(other), m_id(getUniqueID()) {

    m_parameters = other.getParameters();
    m_moduleID = other.getModuleID();
}

ModuleObject* ModuleObject::clone() const {
    return new ModuleObject(*this);
}

ModuleObject::~ModuleObject() {
}

size_t ModuleObject::getNbOfParameters() {
    return m_parameters.size();
}

const std::vector<double>& ModuleObject::getParameters() const {
    return m_parameters;
}

void ModuleObject::setParameters(const std::vector<double>& parameters) {
    this->m_parameters = parameters;
}

const std::string& ModuleObject::getModuleID() const {
    return m_moduleID;
}

unsigned int ModuleObject::getUniqueID() {
    return ModuleObject::m_uniqueID++;
}

unsigned int ModuleObject::getId() const {
    return m_id;
}
