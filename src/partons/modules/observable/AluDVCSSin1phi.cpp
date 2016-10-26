#include "../../../../include/partons/modules/observable/AluDVCSSin1phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/modules/observable/AluDVCS.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

// Initialise [class]::classId with a unique name.
const unsigned int AluDVCSSin1phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AluDVCSSin1phi("AluDVCSSin1phi"));

AluDVCSSin1phi::AluDVCSSin1phi(const std::string &className) :
        FourierObservable(className), m_pFunctionToIntegrateObservable(0) {
    m_channel = ObservableChannel::DVCS;

    initFunctorsForIntegrations();
}

AluDVCSSin1phi::AluDVCSSin1phi(const AluDVCSSin1phi& other) :
        FourierObservable(other) {
    initFunctorsForIntegrations();
}

AluDVCSSin1phi::~AluDVCSSin1phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }

}

void AluDVCSSin1phi::resolveObjectDependencies() {
    m_pPhiObservable =
            Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    AluDVCS::classId);
}

void AluDVCSSin1phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AluDVCSSin1phi::functionToIntegrateObservable);
}

AluDVCSSin1phi* AluDVCSSin1phi::clone() const {
    return new AluDVCSSin1phi(*this);
}

////TODO check
double AluDVCSSin1phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pPhiObservable->compute(x) * sin(x);
}

double AluDVCSSin1phi::compute() {



    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * PI),
            emptyParameters) / PI;

}
