#include "../../../../include/partons/modules/observable/AulSinPhi.h"

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
const unsigned int AulSinPhi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AulSinPhi("AulSinPhi"));

AulSinPhi::AulSinPhi(const std::string &className) :
        FourierObservable(className), m_pAulObservable(0), m_pFunctionToIntegrateObservable(
                0) {

    m_pAulObservable =
            Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    Aul::classId);

    m_channel = ObservableChannel::DVCS;

    initFunctorsForIntegrations();
}

AulSinPhi::AulSinPhi(const AulSinPhi& other) :
        FourierObservable(other) {
    if (other.m_pAulObservable != 0) {
        m_pAulObservable = other.m_pAulObservable->clone();
    } else {
        m_pAulObservable = 0;
    }

    initFunctorsForIntegrations();
}

AulSinPhi::~AulSinPhi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }

}

void AulSinPhi::resolveObjectDependencies() {
    m_pAulObservable =
            static_cast<Aul*>(Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    Aul::classId));
}

void AulSinPhi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AulSinPhi::functionToIntegrateObservable);
}

AulSinPhi* AulSinPhi::clone() const {
    return new AulSinPhi(*this);
}

////TODO check
double AulSinPhi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pAulObservable->compute(m_pProcessModule, x) * sin(x);
}

double AulSinPhi::compute() {

    //TODO improve, replace by configuration.
    //   m_pAulObservable->setProcessModule(m_pProcessModule);

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * PI),
            emptyParameters) / cos(-1);

}
