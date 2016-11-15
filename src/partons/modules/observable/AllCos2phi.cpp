#include "../../../../include/partons/modules/observable/AllCos2phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int AllCos2phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AllCos2phi("AllCos2phi"));

AllCos2phi::AllCos2phi(const std::string &className) :
        All(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

AllCos2phi::AllCos2phi(const AllCos2phi& other) :
        All(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;

    initFunctorsForIntegrations();
}

AllCos2phi::~AllCos2phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void AllCos2phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AllCos2phi::functionToIntegrateObservable);
}

AllCos2phi* AllCos2phi::clone() const {
    return new AllCos2phi(*this);
}

double AllCos2phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return All::computePhiObservable(x) * cos(2 * x);
}

double AllCos2phi::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * PI),
            emptyParameters) / (PI);
}
