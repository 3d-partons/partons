#include "ModuleObject.h"

ModuleObject::ModuleObject(const std::string & moduleID) :
		BaseObject(moduleID), m_moduleID(moduleID) {
}

ModuleObject::ModuleObject(const ModuleObject &other) :
		BaseObject(other) {

	m_parameters = other.m_parameters;
	m_moduleID = other.m_moduleID;
}

ModuleObject* ModuleObject::clone() const {
	return new ModuleObject(*this);
}

ModuleObject::~ModuleObject() {
}

void ModuleObject::isModuleConfigured() {
	//do nothing; the definition is just mandatory for enable the use of the clone function
}

void ModuleObject::updateVariables() {
	//do nothing; the definition is just mandatory for enable the use of the clone function
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

