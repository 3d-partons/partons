#include "../../../../include/partons/modules/observable/AcCos2phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int AcCos2phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AcCos2phi("AcCos2phi"));

AcCos2phi::AcCos2phi(const std::string &className) :
        Ac(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

AcCos2phi::AcCos2phi(const AcCos2phi& other) :
        Ac(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;

    initFunctorsForIntegrations();
}

AcCos2phi::~AcCos2phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void AcCos2phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AcCos2phi::functionToIntegrateObservable);
}

AcCos2phi* AcCos2phi::clone() const {
    return new AcCos2phi(*this);
}

double AcCos2phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return Ac::computePhiObservable(x) * cos(2 * x);
}

double AcCos2phi::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / Constant::PI;
}
