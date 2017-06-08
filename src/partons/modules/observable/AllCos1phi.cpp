#include "../../../../include/partons/modules/observable/AllCos1phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int AllCos1phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AllCos1phi("AllCos1phi"));

AllCos1phi::AllCos1phi(const std::string &className) :
        All(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

AllCos1phi::AllCos1phi(const AllCos1phi& other) :
        All(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;

    initFunctorsForIntegrations();
}

AllCos1phi::~AllCos1phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void AllCos1phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AllCos1phi::functionToIntegrateObservable);
}

AllCos1phi* AllCos1phi::clone() const {
    return new AllCos1phi(*this);
}

double AllCos1phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return All::computePhiObservable(x) * cos(1 * x);
}

double AllCos1phi::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (Constant::PI);
}
