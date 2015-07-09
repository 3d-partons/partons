#include "MathIntegratorModule.h"

#include "../utils/logger/LoggerManager.h"

MathIntegratorModule::MathIntegratorModule(const std::string &className) :
        ModuleObject(className), m_pFunction(
                &ModuleObject::functionsToIntegrate) {
}

MathIntegratorModule::~MathIntegratorModule() {

}

MathIntegratorModule::MathIntegratorModule(const MathIntegratorModule &other) :
        ModuleObject(other) {
    m_pFunction = other.m_pFunction;
}

void MathIntegratorModule::preCompute(unsigned int functionName,
        ModuleObject* pModuleObject) {
    m_pLoggerManager->debug(getClassName(), __func__, "Entered");
    pModuleObject->setCurrentFunctionToIntegrate(functionName);
}

