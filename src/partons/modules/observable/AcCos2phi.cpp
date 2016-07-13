#include "../../../../include/partons/modules/observable/AcCos2phi.h"

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
const unsigned int AcCos2phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AcCos2phi("AcCos2phi"));

AcCos2phi::AcCos2phi(const std::string &className) :
        FourierObservable(className), m_pFunctionToIntegrateAcObservable(0) {
    m_channel = ObservableChannel::DVCS;

    initFunctorsForIntegrations();
}

AcCos2phi::AcCos2phi(const AcCos2phi& other) :
        FourierObservable(other) {

    m_pFunctionToIntegrateAcObservable = 0;

    initFunctorsForIntegrations();
}

AcCos2phi::~AcCos2phi() {
    if (m_pFunctionToIntegrateAcObservable) {
        delete m_pFunctionToIntegrateAcObservable;
        m_pFunctionToIntegrateAcObservable = 0;
    }
}

void AcCos2phi::resolveObjectDependencies() {
    m_pPhiObservable =
            Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    Ac::classId);
}

void AcCos2phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateAcObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AcCos2phi::functionToIntegrateAcObservable);
}

AcCos2phi* AcCos2phi::clone() const {
    return new AcCos2phi(*this);
}

//TODO check
double AcCos2phi::functionToIntegrateAcObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pPhiObservable->compute(x) * cos(2 * x);
}

double AcCos2phi::compute() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateAcObservable, 0., (2 * PI),
            emptyParameters);
}
