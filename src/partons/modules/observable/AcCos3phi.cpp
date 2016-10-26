#include "../../../../include/partons/modules/observable/AcCos3phi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/modules/observable/Ac.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

// Initialise [class]::classId with a unique name.
const unsigned int AcCos3phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AcCos3phi("AcCos3phi"));

AcCos3phi::AcCos3phi(const std::string &className) :
        FourierObservable(className), m_pFunctionToIntegrateAcObservable(0) {
    m_channel = ObservableChannel::DVCS;

    initFunctorsForIntegrations();
}

AcCos3phi::AcCos3phi(const AcCos3phi& other) :
        FourierObservable(other) {

    m_pFunctionToIntegrateAcObservable = 0;

    initFunctorsForIntegrations();
}

AcCos3phi::~AcCos3phi() {
    if (m_pFunctionToIntegrateAcObservable) {
        delete m_pFunctionToIntegrateAcObservable;
        m_pFunctionToIntegrateAcObservable = 0;
    }
}

void AcCos3phi::resolveObjectDependencies() {
    m_pPhiObservable =
            Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    Ac::classId);
}

void AcCos3phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateAcObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AcCos3phi::functionToIntegrateAcObservable);
}

AcCos3phi* AcCos3phi::clone() const {
    return new AcCos3phi(*this);
}

//TODO check
double AcCos3phi::functionToIntegrateAcObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pPhiObservable->compute(x) * cos(3 * x);
}

double AcCos3phi::compute() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateAcObservable, 0., (2 * PI),
            emptyParameters)/PI;
}
