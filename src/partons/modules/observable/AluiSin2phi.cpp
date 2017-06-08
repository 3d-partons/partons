#include "../../../../include/partons/modules/observable/AluiSin2phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int AluiSin2phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AluiSin2phi("AluiSin2phi"));

AluiSin2phi::AluiSin2phi(const std::string &className) :
        Alui(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

AluiSin2phi::AluiSin2phi(const AluiSin2phi& other) :
        Alui(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

AluiSin2phi::~AluiSin2phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }

}

void AluiSin2phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AluiSin2phi::functionToIntegrateObservable);
}

AluiSin2phi* AluiSin2phi::clone() const {
    return new AluiSin2phi(*this);
}

double AluiSin2phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return Alui::computePhiObservable(x) * sin(2 * x);
}

double AluiSin2phi::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / Constant::PI;
}
