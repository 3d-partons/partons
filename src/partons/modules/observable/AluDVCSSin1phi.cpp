#include "../../../../include/partons/modules/observable/AluDVCSSin1phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int AluDVCSSin1phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AluDVCSSin1phi("AluDVCSSin1phi"));

AluDVCSSin1phi::AluDVCSSin1phi(const std::string &className) :
        AluDVCS(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

AluDVCSSin1phi::AluDVCSSin1phi(const AluDVCSSin1phi& other) :
        AluDVCS(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

AluDVCSSin1phi::~AluDVCSSin1phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void AluDVCSSin1phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AluDVCSSin1phi::functionToIntegrateObservable);
}

AluDVCSSin1phi* AluDVCSSin1phi::clone() const {
    return new AluDVCSSin1phi(*this);
}

double AluDVCSSin1phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return AluDVCS::computePhiObservable(x) * sin(x);
}

double AluDVCSSin1phi::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / Constant::PI;

}
