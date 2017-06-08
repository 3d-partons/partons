#include "../../../../include/partons/modules/observable/AcCos0phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int AcCos0phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AcCos0phi("AcCos0phi"));

AcCos0phi::AcCos0phi(const std::string &className) :
        Ac(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

AcCos0phi::AcCos0phi(const AcCos0phi& other) :
        Ac(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;

    initFunctorsForIntegrations();
}

AcCos0phi::~AcCos0phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void AcCos0phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AcCos0phi::functionToIntegrateObservable);
}

AcCos0phi* AcCos0phi::clone() const {
    return new AcCos0phi(*this);
}

double AcCos0phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return Ac::computePhiObservable(x) * cos(0 * x);
}

double AcCos0phi::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (2 * Constant::PI);
}
