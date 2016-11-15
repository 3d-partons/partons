#include "../../../../include/partons/modules/observable/AcCos3phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int AcCos3phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AcCos3phi("AcCos3phi"));

AcCos3phi::AcCos3phi(const std::string &className) :
        Ac(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

AcCos3phi::AcCos3phi(const AcCos3phi& other) :
        Ac(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;

    initFunctorsForIntegrations();
}

AcCos3phi::~AcCos3phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void AcCos3phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AcCos3phi::functionToIntegrateObservable);
}

AcCos3phi* AcCos3phi::clone() const {
    return new AcCos3phi(*this);
}

double AcCos3phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return Ac::computePhiObservable(x) * cos(3 * x);
}

double AcCos3phi::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * PI),
            emptyParameters) / PI;
}
