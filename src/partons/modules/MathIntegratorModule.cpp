#include "../../../include/partons/modules/MathIntegratorModule.h"

#include <NumA/integration/one_dimension/Integrator1D.h>

MathIntegratorModule::MathIntegratorModule() :
        m_mathIntegrator(0) {

}

MathIntegratorModule::~MathIntegratorModule() {
    if (m_mathIntegrator) {
        delete m_mathIntegrator;
        m_mathIntegrator = 0;
    }
}

MathIntegratorModule::MathIntegratorModule(const MathIntegratorModule& other) {
    if (other.m_mathIntegrator) {
        // TODO remove hardcoded new
        m_mathIntegrator = NumA::Integrator1D::newIntegrator(
                NumA::IntegratorType1D::GK21_ADAPTIVE);
    } else {
        m_mathIntegrator = 0;
    }
}

void MathIntegratorModule::setIntegrator(
        NumA::IntegratorType1D::Type integratorType) {
    if (m_mathIntegrator) {
        delete m_mathIntegrator;
        m_mathIntegrator = 0;
    }

    m_mathIntegrator = NumA::Integrator1D::newIntegrator(integratorType);
}

double MathIntegratorModule::integrate(NumA::FunctionType1D* pFunction,
        double a, double b, std::vector<double>& parameters) {
    return m_mathIntegrator->integrate(pFunction, a, b, parameters);
}

void MathIntegratorModule::configureIntegrator(NumA::Parameters& parameters) {
    m_mathIntegrator->configure(parameters);
}

NumA::Integrator1D* MathIntegratorModule::getMathIntegrator() {
    return m_mathIntegrator;
}
