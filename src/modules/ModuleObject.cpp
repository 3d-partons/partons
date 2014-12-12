#include "ModuleObject.h"

#include <stdexcept>

#include "../beans/Parameters.h"
#include "MathIntegratorModule.h"

ModuleObject::ModuleObject(const std::string &className)
        : BaseObject(className), m_currentFunctionToIntegrate(0), m_pMathIntegratorModule(
                0) {
}

ModuleObject::ModuleObject(const ModuleObject &other)
        : BaseObject(other) {

    m_currentFunctionToIntegrate = other.m_currentFunctionToIntegrate;

    if (other.m_pMathIntegratorModule != 0) {
        m_pMathIntegratorModule = other.m_pMathIntegratorModule->clone();
    } else {
        m_pMathIntegratorModule = 0;
    }

//TODO copie constructor
//m_parameters = other.m_parameters;
}

ModuleObject::~ModuleObject() {
}

//TODO exception nothing to  configure
void ModuleObject::configure(Parameters parameters) {

}

//TODO better exception message
double ModuleObject::functionsToIntegrate(const double * x,
        const double * parameters) {
    throw std::runtime_error("[ModuleObject::functionsToIntegrate");
}

size_t ModuleObject::getNbOfParameters() {
    return m_parameters.size();
}

std::string ModuleObject::toString() {
    return BaseObject::toString();
}

unsigned int ModuleObject::getCurrentFunctionToIntegrate() const {
    return m_currentFunctionToIntegrate;
}

void ModuleObject::setCurrentFunctionToIntegrate(
        unsigned int currentFunctionToIntegrate) {
    m_currentFunctionToIntegrate = currentFunctionToIntegrate;
}
