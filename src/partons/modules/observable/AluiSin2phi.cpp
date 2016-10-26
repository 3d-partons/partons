#include "../../../../include/partons/modules/observable/AluiSin2phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/modules/observable/Alui.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

// Initialise [class]::classId with a unique name.
const unsigned int AluiSin2phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AluiSin2phi("AluiSin2phi"));

AluiSin2phi::AluiSin2phi(const std::string &className) :
        FourierObservable(className), m_pFunctionToIntegrateObservable(0) {
    m_channel = ObservableChannel::DVCS;

    initFunctorsForIntegrations();
}

AluiSin2phi::AluiSin2phi(const AluiSin2phi& other) :
        FourierObservable(other) {
    initFunctorsForIntegrations();
}

AluiSin2phi::~AluiSin2phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }

}

void AluiSin2phi::resolveObjectDependencies() {
    m_pPhiObservable =
            Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    Alui::classId);
}

void AluiSin2phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AluiSin2phi::functionToIntegrateObservable);
}

AluiSin2phi* AluiSin2phi::clone() const {
    return new AluiSin2phi(*this);
}

////TODO check
double AluiSin2phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pPhiObservable->compute(x) * sin(2 * x);
}

double AluiSin2phi::compute() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * PI),
            emptyParameters) / PI;

}
