#include "RootIntegrationMode.h"

#include <Math/AllIntegrationTypes.h>
#include <Math/Integrator.h>
#include <Math/WrappedTF1.h>
#include <TF1.h>

#include "../../services/ModuleObjectFactory.h"

// Initialise [class]::moduleID with a unique name and enable registerModule() to be executed before the main() function.
const std::string RootIntegrationMode::moduleID =
        ModuleObjectFactory::getInstance()->registerModule(
                new RootIntegrationMode("RootIntegrationMode"));

RootIntegrationMode::RootIntegrationMode(const std::string &className)
        : MathIntegratorModule(className) {

}

RootIntegrationMode::~RootIntegrationMode() {

}

RootIntegrationMode::RootIntegrationMode(const RootIntegrationMode &other)
        : MathIntegratorModule(other) {
}

RootIntegrationMode* RootIntegrationMode::clone() const {
    return new RootIntegrationMode(*this);
}

//TODO implement
void RootIntegrationMode::isModuleWellConfigured() {

}

void RootIntegrationMode::initModule() {

}

double RootIntegrationMode::compute(unsigned int functionName,
        ModuleObject* pModuleObject, double xMin, double xMax) {
    preCompute(functionName, pModuleObject);

    ROOT::Math::Integrator integrator(
            ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR, 0., 1.e-3);

    TF1 tf1("RootIntegrationMode::compute", pModuleObject, m_pFunction, 0., 1.,
            0);

    ROOT::Math::WrappedTF1 wrappedTf1(tf1);
    integrator.SetFunction(wrappedTf1);

    return integrator.Integral(xMin, xMax);

}
