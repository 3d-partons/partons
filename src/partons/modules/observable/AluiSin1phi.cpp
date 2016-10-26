#include "../../../../include/partons/modules/observable/AluiSin1phi.h"

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
const unsigned int AluiSin1phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AluiSin1phi("AluiSin1phi"));

AluiSin1phi::AluiSin1phi(const std::string &className) :
        FourierObservable(className), m_pFunctionToIntegrateObservable(0) {
    m_channel = ObservableChannel::DVCS;

    initFunctorsForIntegrations();
}

AluiSin1phi::AluiSin1phi(const AluiSin1phi& other) :
        FourierObservable(other) {
    initFunctorsForIntegrations();
}

AluiSin1phi::~AluiSin1phi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }

}

void AluiSin1phi::resolveObjectDependencies() {
    m_pPhiObservable =
            Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    Alui::classId);
}

void AluiSin1phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AluiSin1phi::functionToIntegrateObservable);
}

AluiSin1phi* AluiSin1phi::clone() const {
    return new AluiSin1phi(*this);
}

////TODO check
double AluiSin1phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pPhiObservable->compute(x) * sin(x);
}

double AluiSin1phi::compute() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * PI),
            emptyParameters) / PI;

}
