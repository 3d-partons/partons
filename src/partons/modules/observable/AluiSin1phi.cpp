#include "../../../../include/partons/modules/observable/AluiSin1phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int AluiSin1phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AluiSin1phi("AluiSin1phi"));

AluiSin1phi::AluiSin1phi(const std::string &className) :
        Alui(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

AluiSin1phi::AluiSin1phi(const AluiSin1phi& other) :
        Alui(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

AluiSin1phi::~AluiSin1phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }

}

void AluiSin1phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AluiSin1phi::functionToIntegrateObservable);
}

AluiSin1phi* AluiSin1phi::clone() const {
    return new AluiSin1phi(*this);
}

double AluiSin1phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return Alui::computePhiObservable(x) * sin(x);
}

double AluiSin1phi::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * PI),
            emptyParameters) / PI;
}
