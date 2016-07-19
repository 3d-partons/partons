#include "../../../../include/partons/modules/observable/AcCos0phi.h"

#include <NumA/integration/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <cmath>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/modules/observable/Ac.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

// Initialise [class]::classId with a unique name.
const unsigned int AcCos0phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AcCos0phi("AcCos0phi"));

AcCos0phi::AcCos0phi(const std::string &className) :
        FourierObservable(className), m_pFunctionToIntegrateAcObservable(0) {
    m_channel = ObservableChannel::DVCS;

    initFunctorsForIntegrations();
}

AcCos0phi::AcCos0phi(const AcCos0phi& other) :
        FourierObservable(other) {

    m_pFunctionToIntegrateAcObservable = 0;

    initFunctorsForIntegrations();
}

AcCos0phi::~AcCos0phi() {
    if (m_pFunctionToIntegrateAcObservable) {
        delete m_pFunctionToIntegrateAcObservable;
        m_pFunctionToIntegrateAcObservable = 0;
    }
}

void AcCos0phi::resolveObjectDependencies() {
    m_pPhiObservable =
            Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    Ac::classId);
}

void AcCos0phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateAcObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AcCos0phi::functionToIntegrateAcObservable);
}

AcCos0phi* AcCos0phi::clone() const {
    return new AcCos0phi(*this);
}

//TODO check
double AcCos0phi::functionToIntegrateAcObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pPhiObservable->compute(x) * cos(x);
}

double AcCos0phi::compute() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateAcObservable, 0., (2 * PI),
            emptyParameters)/(2*PI);
}
