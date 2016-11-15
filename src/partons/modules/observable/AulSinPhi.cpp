#include "../../../../include/partons/modules/observable/AulSinPhi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int AulSinPhi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AulSinPhi("AulSinPhi"));

AulSinPhi::AulSinPhi(const std::string &className) :
        Aul(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

AulSinPhi::AulSinPhi(const AulSinPhi& other) :
        Aul(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

AulSinPhi::~AulSinPhi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }

}

void AulSinPhi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AulSinPhi::functionToIntegrateObservable);
}

AulSinPhi* AulSinPhi::clone() const {
    return new AulSinPhi(*this);
}

double AulSinPhi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return Aul::computePhiObservable(x) * sin(x);
}

double AulSinPhi::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * PI),
            emptyParameters) / PI;
}
