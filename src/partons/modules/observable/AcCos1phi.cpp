#include "../../../../include/partons/modules/observable/AcCos1phi.h"

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
const unsigned int AcCos1phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AcCos1phi("AcCos1phi"));

AcCos1phi::AcCos1phi(const std::string &className) :
        FourierObservable(className), m_pFunctionToIntegrateAcObservable(0) {
    m_channel = ObservableChannel::DVCS;

    initFunctorsForIntegrations();
}

AcCos1phi::AcCos1phi(const AcCos1phi& other) :
        FourierObservable(other) {

    m_pFunctionToIntegrateAcObservable = 0;

    initFunctorsForIntegrations();
}

AcCos1phi::~AcCos1phi() {
    if (m_pFunctionToIntegrateAcObservable) {
        delete m_pFunctionToIntegrateAcObservable;
        m_pFunctionToIntegrateAcObservable = 0;
    }
}

void AcCos1phi::resolveObjectDependencies() {
    m_pPhiObservable =
            Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    Ac::classId);
}

void AcCos1phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateAcObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AcCos1phi::functionToIntegrateAcObservable);
}

AcCos1phi* AcCos1phi::clone() const {
    return new AcCos1phi(*this);
}

//TODO check
double AcCos1phi::functionToIntegrateAcObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pPhiObservable->compute(x) * cos(x);
}

double AcCos1phi::compute() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateAcObservable, 0., (2 * PI),
            emptyParameters)/PI;
}
