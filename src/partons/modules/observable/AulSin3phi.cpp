#include "../../../../include/partons/modules/observable/AulSin3phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int AulSin3phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AulSin3phi("AulSin3phi"));

AulSin3phi::AulSin3phi(const std::string &className) :
        Aul(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

AulSin3phi::AulSin3phi(const AulSin3phi& other) :
        Aul(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

AulSin3phi::~AulSin3phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }

}

void AulSin3phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AulSin3phi::functionToIntegrateObservable);
}

AulSin3phi* AulSin3phi::clone() const {
    return new AulSin3phi(*this);
}

double AulSin3phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return Aul::computePhiObservable(x) * sin(3 * x);
}

double AulSin3phi::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * PI),
            emptyParameters) / PI;
}
