#include "../../../../include/partons/modules/observable/AllCos1phi.h"

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
const unsigned int AllCos1phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AllCos1phi("AllCos1phi"));

AllCos1phi::AllCos1phi(const std::string &className) :
        FourierObservable(className), m_pFunctionToIntegrateAllObservable(0) {
    m_channel = ObservableChannel::DVCS;

    initFunctorsForIntegrations();
}

AllCos1phi::AllCos1phi(const AllCos1phi& other) :
        FourierObservable(other) {

    m_pFunctionToIntegrateAllObservable = 0;

    initFunctorsForIntegrations();
}

AllCos1phi::~AllCos1phi() {
    if (m_pFunctionToIntegrateAllObservable) {
        delete m_pFunctionToIntegrateAllObservable;
        m_pFunctionToIntegrateAllObservable = 0;
    }
}

void AllCos1phi::resolveObjectDependencies() {
    m_pPhiObservable =
            Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    All::classId);
}

void AllCos1phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateAllObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AllCos1phi::functionToIntegrateAllObservable);
}

AllCos1phi* AllCos1phi::clone() const {
    return new AllCos1phi(*this);
}

//TODO check
double AllCos1phi::functionToIntegrateAllObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pPhiObservable->compute(x) * cos(1 * x);
}

double AllCos1phi::compute() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateAllObservable, 0., (2 * PI),
            emptyParameters)/(PI);
}
