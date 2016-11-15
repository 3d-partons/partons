#include "../../../../include/partons/modules/observable/AulSin2phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int AulSin2phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AulSin2phi("AulSin2phi"));

AulSin2phi::AulSin2phi(const std::string &className) :
        Aul(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

AulSin2phi::AulSin2phi(const AulSin2phi& other) :
        Aul(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

AulSin2phi::~AulSin2phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }

}

void AulSin2phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AulSin2phi::functionToIntegrateObservable);
}

AulSin2phi* AulSin2phi::clone() const {
    return new AulSin2phi(*this);
}

double AulSin2phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return Aul::computePhiObservable(x) * sin(2 * x);
}

double AulSin2phi::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * PI),
            emptyParameters) / PI;
}
