#include "../../../../include/partons/modules/observable/AulSin2phi.h"

#include <NumA/integration/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <cmath>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/modules/observable/Aul.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

// Initialise [class]::classId with a unique name.
const unsigned int AulSin2phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AulSin2phi("AulSin2phi"));

AulSin2phi::AulSin2phi(const std::string &className) :
        FourierObservable(className), m_pFunctionToIntegrateObservable(0) {
    m_channel = ObservableChannel::DVCS;

    initFunctorsForIntegrations();
}

AulSin2phi::AulSin2phi(const AulSin2phi& other) :
        FourierObservable(other) {
    initFunctorsForIntegrations();
}

AulSin2phi::~AulSin2phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }

}

void AulSin2phi::resolveObjectDependencies() {
    m_pPhiObservable =
            Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    Aul::classId);
}

void AulSin2phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AulSin2phi::functionToIntegrateObservable);
}

AulSin2phi* AulSin2phi::clone() const {
    return new AulSin2phi(*this);
}

////TODO check
double AulSin2phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pPhiObservable->compute(x) * sin(2 * x);
}

double AulSin2phi::compute() {

    //TODO improve, replace by configuration.
    //   m_pAulObservable->setProcessModule(m_pProcessModule);

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * PI),
            emptyParameters) / PI;

}
