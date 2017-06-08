#include "../../../../include/partons/modules/observable/AllCos0phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int AllCos0phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AllCos0phi("AllCos0phi"));

AllCos0phi::AllCos0phi(const std::string &className) :
        All(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

AllCos0phi::AllCos0phi(const AllCos0phi& other) :
        All(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;

    initFunctorsForIntegrations();
}

AllCos0phi::~AllCos0phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void AllCos0phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AllCos0phi::functionToIntegrateObservable);
}

AllCos0phi* AllCos0phi::clone() const {
    return new AllCos0phi(*this);
}

double AllCos0phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return All::computePhiObservable(x) * cos(0 * x);
}

double AllCos0phi::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (2 * Constant::PI);
}
