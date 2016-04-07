#include "../../../include/partons/modules/ConvolCoeffFunctionModule.h"

ConvolCoeffFunctionModule::ConvolCoeffFunctionModule(
        const std::string &className) :
        ModuleObject(className), MathIntegratorModule() {
}

ConvolCoeffFunctionModule::~ConvolCoeffFunctionModule() {
}

ConvolCoeffFunctionModule::ConvolCoeffFunctionModule(
        const ConvolCoeffFunctionModule& other) :
        ModuleObject(other), MathIntegratorModule(other) {
}

void ConvolCoeffFunctionModule::configure(
        const ElemUtils::Parameters &parameters) {
    configureIntegrator(parameters);

    ModuleObject::configure(parameters);
}
