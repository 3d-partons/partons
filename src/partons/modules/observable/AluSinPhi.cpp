#include "../../../../include/partons/modules/observable/AluSinPhi.h"

#include <NumA/integration/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <cmath>
//#include <vector>

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
        FourierObservable(className), m_pAluObservable(0), m_pFunctionToIntegrateNumObservable(
                0), m_pFunctionToIntegrateDenObservable(0) {

//    Observable* pObs =
//            Partons::getInstance()->getModuleObjectFactory()->newObservable(
//                    Alu::classId);
//
//    m_pAluObservable =
//            static_cast<Alu*>(Partons::getInstance()->getModuleObjectFactory()->newObservable(
//                    Alu::classId));

    m_channel = ObservableChannel::DVCS;

    initFunctorsForIntegrations();
}

AluSinPhi::AluSinPhi(const AluSinPhi& other) :
        FourierObservable(other) {
    if (other.m_pAluObservable != 0) {
        m_pAluObservable = other.m_pAluObservable->clone();
    } else {
        m_pAluObservable = 0;
    }

    initFunctorsForIntegrations();
}

AluSinPhi::~AluSinPhi() {
    if (m_pFunctionToIntegrateNumObservable) {
        delete m_pFunctionToIntegrateNumObservable;
        m_pFunctionToIntegrateNumObservable = 0;
    }

    if (m_pFunctionToIntegrateDenObservable) {
        delete m_pFunctionToIntegrateDenObservable;
        m_pFunctionToIntegrateDenObservable = 0;
    }

}

void AluSinPhi::resolveObjectDependencies() {
    m_pAluObservable =
            static_cast<Alu*>(Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    Alu::classId));
}

void AluSinPhi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateNumObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AluSinPhi::functionToIntegrateNumObservable);
    m_pFunctionToIntegrateDenObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AluSinPhi::functionToIntegrateDenObservable);

}

AluSinPhi* AluSinPhi::clone() const {
    return new AluSinPhi(*this);
}

////TODO check
double AluSinPhi::functionToIntegrateNumObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pAluObservable->Num(m_pProcessModule, x) * sin(x);
}

double AluSinPhi::functionToIntegrateDenObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pAluObservable->Den(m_pProcessModule, x);
}

double AluSinPhi::compute() {

    //TODO improve, replace by configuration.
    m_pAluObservable->setProcessModule(m_pProcessModule);

    std::vector<double> emptyParameters;

    double N = integrate(m_pFunctionToIntegrateNumObservable, 0., (2 * PI),
            emptyParameters);
    double D = integrate(m_pFunctionToIntegrateDenObservable, 0., (2 * PI),
            emptyParameters);
    if (D != 0) {
        return N / D;
    } else {
        return 2.0;
    }
}
