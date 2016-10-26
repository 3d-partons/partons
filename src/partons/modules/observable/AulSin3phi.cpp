#include "../../../../include/partons/modules/observable/AulSin3phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/modules/observable/Aul.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

// Initialise [class]::classId with a unique name.
const unsigned int AulSin3phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AulSin3phi("AulSin3phi"));

AulSin3phi::AulSin3phi(const std::string &className) :
        FourierObservable(className), m_pFunctionToIntegrateObservable(0) {
    m_channel = ObservableChannel::DVCS;

    initFunctorsForIntegrations();
}

AulSin3phi::AulSin3phi(const AulSin3phi& other) :
        FourierObservable(other) {
    initFunctorsForIntegrations();
}

AulSin3phi::~AulSin3phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }

}

void AulSin3phi::resolveObjectDependencies() {
    m_pPhiObservable =
            Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    Aul::classId);
}

void AulSin3phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AulSin3phi::functionToIntegrateObservable);
}

AulSin3phi* AulSin3phi::clone() const {
    return new AulSin3phi(*this);
}

////TODO check
double AulSin3phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pPhiObservable->compute(x) * sin(3 *x);
}

double AulSin3phi::compute() {

    //TODO improve, replace by configuration.
    //   m_pAulObservable->setProcessModule(m_pProcessModule);

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * PI),
            emptyParameters) / PI;

}
