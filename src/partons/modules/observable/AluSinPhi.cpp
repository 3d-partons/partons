#include "../../../../include/partons/modules/observable/AluSinPhi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int AluSinPhi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AluSinPhi("AluSinPhi"));

AluSinPhi::AluSinPhi(const std::string &className) :
        Alu(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

AluSinPhi::AluSinPhi(const AluSinPhi& other) :
        Alu(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

AluSinPhi::~AluSinPhi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }

}

void AluSinPhi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AluSinPhi::functionToIntegrateObservable);
}

AluSinPhi* AluSinPhi::clone() const {
    return new AluSinPhi(*this);
}

double AluSinPhi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return Alu::computePhiObservable(x) * sin(x);
}

double AluSinPhi::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / Constant::PI;
}
