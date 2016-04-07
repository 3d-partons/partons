#include "../../../../include/partons/modules/observable/FourierObservable.h"

#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../../include/partons/beans/observable/ObservableType.h"

FourierObservable::FourierObservable(const std::string &className) :
        Observable(className), MathIntegratorModule() {
    m_observableType = ObservableType::FOURIER;

    setIntegrator(NumA::IntegratorType1D::GK21_ADAPTIVE);
}

FourierObservable::FourierObservable(const FourierObservable& other) :
        Observable(other), MathIntegratorModule(other) {
}

FourierObservable::~FourierObservable() {
}
