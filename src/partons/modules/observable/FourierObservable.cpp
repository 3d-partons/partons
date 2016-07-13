#include "../../../../include/partons/modules/observable/FourierObservable.h"

#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../../include/partons/beans/observable/ObservableType.h"

FourierObservable::FourierObservable(const std::string &className) :
        Observable(className), MathIntegratorModule(), m_pPhiObservable(0) {
    m_observableType = ObservableType::FOURIER;

    setIntegrator(NumA::IntegratorType1D::DEXP);
}

FourierObservable::FourierObservable(const FourierObservable& other) :
        Observable(other), MathIntegratorModule(other), m_pPhiObservable(0) {
    if (other.m_pPhiObservable != 0) {
        m_pPhiObservable = other.m_pPhiObservable->clone();
    }
}

FourierObservable::~FourierObservable() {
}

void FourierObservable::setProcessModule(ProcessModule* pProcessModule) {
    Observable::setProcessModule(pProcessModule);

    if (m_pPhiObservable != 0) {
        m_pPhiObservable->setProcessModule(pProcessModule);
    }
    else
    {
        error(__func__, "m_pPhiObservable is NULL pointer");
    }
}
