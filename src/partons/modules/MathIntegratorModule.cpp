#include "../../../include/partons/modules/MathIntegratorModule.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>

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
