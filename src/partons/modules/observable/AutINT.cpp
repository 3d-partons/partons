#include "../../../../include/partons/modules/observable/AutINT.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int AutINT::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AutINT("AutINT"));

AutINT::AutINT(const std::string &className) :
        AutSinPhiPhiS(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

AutINT::AutINT(const AutINT& other) :
        AutSinPhiPhiS(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;

    initFunctorsForIntegrations();
}

AutINT::~AutINT() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void AutINT::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AutINT::functionToIntegrateObservable);
}

AutINT* AutINT::clone() const {
    return new AutINT(*this);
}

double AutINT::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return AutSinPhiPhiS::computePhiObservable(x)*cos(x);
}

double AutINT::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * PI),
            emptyParameters) / (2 * PI);
}
