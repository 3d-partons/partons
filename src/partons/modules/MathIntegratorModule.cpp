#include "../../../include/partons/modules/MathIntegratorModule.h"

#include <ElementaryUtils/logger/LoggerManager.h>
#include <ElementaryUtils/parameters/Parameter.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../include/partons/Partons.h"

const std::string MathIntegratorModule::PARAM_NAME_INTEGRATOR_TYPE =
		"integrator_type";

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
		m_mathIntegrator = other.m_mathIntegrator->clone();
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

void MathIntegratorModule::configureIntegrator(
		const ElemUtils::Parameters& parameters) {

	if (parameters.isAvailable(
			MathIntegratorModule::PARAM_NAME_INTEGRATOR_TYPE)) {

		NumA::IntegratorType1D::Type integratorType = NumA::IntegratorType1D(
				parameters.getLastAvailable().getString());

		setIntegrator(integratorType);

		Partons::getInstance()->getLoggerManager()->info("MathIntegratorModule",
				__func__,
				ElemUtils::Formatter()
						<< MathIntegratorModule::PARAM_NAME_INTEGRATOR_TYPE
						<< " configured with value = "
						<< NumA::IntegratorType1D(integratorType).toString());
	}

	if (m_mathIntegrator) {
		m_mathIntegrator->configure(parameters);
	}
}

NumA::Integrator1D* MathIntegratorModule::getMathIntegrator() {
	return m_mathIntegrator;
}
