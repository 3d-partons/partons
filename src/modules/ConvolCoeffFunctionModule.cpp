#include "ConvolCoeffFunctionModule.h"

ConvolCoeffFunctionModule::ConvolCoeffFunctionModule(
        const std::string &className) :
        ModuleObject(className) {
}

ConvolCoeffFunctionModule::~ConvolCoeffFunctionModule() {
}

ConvolCoeffFunctionModule::ConvolCoeffFunctionModule(
        const ConvolCoeffFunctionModule& other) :
        ModuleObject(other), MathIntegratorModule(other) {
}
