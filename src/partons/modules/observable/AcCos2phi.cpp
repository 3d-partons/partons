#include "../../../../include/partons/modules/observable/AcCos2phi.h"

#include <NumA/integration/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <cmath>

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
        FourierObservable(className), m_pAcObservable(0), m_pFunctionToIntegrate(
                0) {
    m_pAcObservable =
            Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    Ac::classId);

    initFunctorsForIntegrations();
}

AcCos2phi::AcCos2phi(const AcCos2phi& other) :
        FourierObservable(other) {
    if (other.m_pAcObservable != 0) {
        m_pAcObservable = other.m_pAcObservable->clone();
    } else {
        m_pAcObservable = 0;
    }

    initFunctorsForIntegrations();
}

AcCos2phi::~AcCos2phi() {
    if (m_pFunctionToIntegrate) {
        delete m_pFunctionToIntegrate;
        m_pFunctionToIntegrate = 0;
    }
}

void AcCos2phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrate = NumA::Integrator1D::newIntegrationFunctor(this,
            &AcCos2phi::functionToIntegrate);
}

AcCos2phi* AcCos2phi::clone() const {
    return new AcCos2phi(*this);
}

//TODO check
double AcCos2phi::functionToIntegrate(double x, std::vector<double> params) {
    // x[0] = phi
    return m_pAcObservable->compute(m_pProcess, x) * cos(2 * x);
}

double AcCos2phi::compute() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrate, 0., (2 * PI), emptyParameters);
}
