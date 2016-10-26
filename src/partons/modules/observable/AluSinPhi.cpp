#include "../../../../include/partons/modules/observable/AluSinPhi.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/modules/observable/Alu.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

// Initialise [class]::classId with a unique name.
const unsigned int AluSinPhi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AluSinPhi("AluSinPhi"));

AluSinPhi::AluSinPhi(const std::string &className) :
        FourierObservable(className), m_pFunctionToIntegrateObservable(0) {

    m_channel = ObservableChannel::DVCS;

    initFunctorsForIntegrations();
}

AluSinPhi::AluSinPhi(const AluSinPhi& other) :
        FourierObservable(other) {
    initFunctorsForIntegrations();
}

AluSinPhi::~AluSinPhi() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }

}

void AluSinPhi::resolveObjectDependencies() {
    m_pPhiObservable =
            Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    Alu::classId);
}

void AluSinPhi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AluSinPhi::functionToIntegrateObservable);
}

AluSinPhi* AluSinPhi::clone() const {
    return new AluSinPhi(*this);
}

////TODO check
double AluSinPhi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return m_pPhiObservable->compute(x) * sin(x);
}

double AluSinPhi::compute() {

    //TODO improve, replace by configuration.
    //   m_pAluObservable->setProcessModule(m_pProcessModule);

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * PI),
            emptyParameters) / PI;

}
