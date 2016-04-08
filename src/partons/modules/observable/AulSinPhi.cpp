#include "../../../../include/partons/modules/observable/AulSinPhi.h"

#include <NumA/integration/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <cmath>
#include <vector>

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
        FourierObservable(className), m_pAulObservable(0), m_pFunctionToIntegrateNumObservable(
                0), m_pFunctionToIntegrateDenObservable(0) {

//    Observable* pObs =
//            Partons::getInstance()->getModuleObjectFactory()->newObservable(
//                    Aul::classId);
//
//    m_pAulObservable =
//            static_cast<Aul*>(Partons::getInstance()->getModuleObjectFactory()->newObservable(
//                    Aul::classId));

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
    if (m_pFunctionToIntegrateNumObservable) {
        delete m_pFunctionToIntegrateNumObservable;
        m_pFunctionToIntegrateNumObservable = 0;
    }

    if (m_pFunctionToIntegrateDenObservable) {
        delete m_pFunctionToIntegrateDenObservable;
        m_pFunctionToIntegrateDenObservable = 0;
    }

}

void AulSinPhi::init() {
    m_pAulObservable =
            static_cast<Aul*>(Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    Aul::classId));
}

void AulSinPhi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateNumObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AulSinPhi::functionToIntegrateNumObservable);
    m_pFunctionToIntegrateDenObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &AulSinPhi::functionToIntegrateDenObservable);

}

AulSinPhi* AulSinPhi::clone() const {
    return new AulSinPhi(*this);
}

////TODO check
double AulSinPhi::functionToIntegrateNumObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pAulObservable->Num(m_pProcess, x) * sin(x);
}

double AulSinPhi::functionToIntegrateDenObservable(double x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pAulObservable->Den(m_pProcess, x);
}

double AulSinPhi::compute() {

    //TODO improve, replace by configuration.
    m_pAulObservable->setDVCSModule(m_pProcess);

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
