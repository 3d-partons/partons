#include "../../../../include/partons/modules/observable/FourierObservable.h"

#include <NumA/integration/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

FourierObservable::FourierObservable(const std::string &className) :
        Observable(className), MathIntegratorModule(), m_pFunctionToIntegrate(0) {
    m_observableType = ObservableType::FOURIER;
    m_mathIntegrator = NumA::Integrator1D::newIntegrator(
            NumA::IntegratorType1D::GK21_ADAPTIVE);

    initFunctorsForIntegrations();
}

FourierObservable::FourierObservable(const FourierObservable& other) :
        Observable(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

void FourierObservable::initFunctorsForIntegrations() {
    m_pFunctionToIntegrate = NumA::Integrator1D::newIntegrationFunctor(this,
            &FourierObservable::functionToIntegrate);
}

FourierObservable::~FourierObservable() {
    if (m_pFunctionToIntegrate) {
        delete m_pFunctionToIntegrate;
        m_pFunctionToIntegrate = 0;
    }
}

//TODO check
double FourierObservable::compute() {
    std::vector<double> emptyParameters;

    return m_mathIntegrator->integrate(m_pFunctionToIntegrate, 0., (2 * PI),
            emptyParameters);
}
