#include "../../../../include/partons/modules/observable/FourierObservable.h"

#include <NumA/MathIntegrator.h>

#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

FourierObservable::FourierObservable(const std::string &className) :
        Observable(className) {
    m_observableType = ObservableType::FOURIER;
    m_mathIntegrator.setIntegrationMode(NumA::MathIntegrator::ROOT);
}

FourierObservable::~FourierObservable() {
}

//TODO check
double FourierObservable::compute() {
    return m_mathIntegrator.integrateWithROOT(this,
            &FourierObservable::functionToIntegrate, 0., (2 * PI));
}
