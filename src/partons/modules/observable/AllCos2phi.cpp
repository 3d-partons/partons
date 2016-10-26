#include "../../../../include/partons/modules/observable/AllCos2phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/modules/observable/All.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

// Initialise [class]::classId with a unique name.
const unsigned int AllCos2phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AllCos2phi("AllCos2phi"));

AllCos2phi::AllCos2phi(const std::string &className) :
        FourierObservable(className), m_pFunctionToIntegrateAllObservable(0) {
    m_channel = ObservableChannel::DVCS;

    initFunctorsForIntegrations();
}

AllCos2phi::AllCos2phi(const AllCos2phi& other) :
        FourierObservable(other) {

    m_pFunctionToIntegrateAllObservable = 0;

    initFunctorsForIntegrations();
}

AllCos2phi::~AllCos2phi() {
    if (m_pFunctionToIntegrateAllObservable) {
        delete m_pFunctionToIntegrateAllObservable;
        m_pFunctionToIntegrateAllObservable = 0;
    }
}

void AllCos2phi::resolveObjectDependencies() {
    m_pPhiObservable =
            Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    All::classId);
}

void AllCos2phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateAllObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AllCos2phi::functionToIntegrateAllObservable);
}

AllCos2phi* AllCos2phi::clone() const {
    return new AllCos2phi(*this);
}

//TODO check
double AllCos2phi::functionToIntegrateAllObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pPhiObservable->compute(x) * cos(2 * x);
}

double AllCos2phi::compute() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateAllObservable, 0., (2 * PI),
            emptyParameters)/(PI);
}
