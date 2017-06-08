#include "../../../../include/partons/modules/observable/AcCos1phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int AcCos1phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AcCos1phi("AcCos1phi"));

AcCos1phi::AcCos1phi(const std::string &className) :
        Ac(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

AcCos1phi::AcCos1phi(const AcCos1phi& other) :
        Ac(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;

    initFunctorsForIntegrations();
}

AcCos1phi::~AcCos1phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void AcCos1phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AcCos1phi::functionToIntegrateObservable);
}

AcCos1phi* AcCos1phi::clone() const {
    return new AcCos1phi(*this);
}

double AcCos1phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return Ac::computePhiObservable(x) * cos(x);
}

double AcCos1phi::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / Constant::PI;
}
