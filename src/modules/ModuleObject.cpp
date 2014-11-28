#include "ModuleObject.h"

ModuleObject::ModuleObject(const std::string & moduleID) :
		BaseObject(moduleID), m_moduleID(moduleID) {
}

ModuleObject::ModuleObject(const ModuleObject &other) :
		BaseObject(other) {

	m_parameters = other.m_parameters;
	m_moduleID = other.m_moduleID;
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

std::string ModuleObject::toString()
{
    return BaseObject::toString();
}
