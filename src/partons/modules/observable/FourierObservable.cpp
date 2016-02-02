#include "../../../../include/partons/modules/observable/FourierObservable.h"

#include <NumA/integration/MathIntegrator.h>
#include <vector>

#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

FourierObservable::FourierObservable(const std::string &className) :
        Observable(className), MathIntegratorModule() {
    m_observableType = ObservableType::FOURIER;
    m_mathIntegrator.setIntegrationMode(
            NumA::MathIntegrator::GSL_ADAPTIVE_SINGULAR);
}

FourierObservable::~FourierObservable() {
}

//TODO check
double FourierObservable::compute() {
    std::vector<double> emptyParameters;

    return m_mathIntegrator.integrate(this,
            &FourierObservable::functionToIntegrate, 0., (2 * PI),
            emptyParameters);
}

FourierObservable::FourierObservable(const FourierObservable& other) :
        Observable(other), MathIntegratorModule(other) {
}
