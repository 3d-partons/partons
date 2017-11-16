#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntSinPhiMPhisSin1Phi.h"

namespace PARTONS {

const unsigned int DVCSAltIntSinPhiMPhisSin1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAltIntSinPhiMPhisSin1Phi(
                        "DVCSAltIntSinPhiMPhisSin1Phi"));

DVCSAltIntSinPhiMPhisSin1Phi::DVCSAltIntSinPhiMPhisSin1Phi(
        const std::string &className) :
        DVCSAltIntSinPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAltIntSinPhiMPhisSin1Phi::DVCSAltIntSinPhiMPhisSin1Phi(
        const DVCSAltIntSinPhiMPhisSin1Phi& other) :
        DVCSAltIntSinPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAltIntSinPhiMPhisSin1Phi::~DVCSAltIntSinPhiMPhisSin1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAltIntSinPhiMPhisSin1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAltIntSinPhiMPhisSin1Phi::functionToIntegrateObservable);
}

DVCSAltIntSinPhiMPhisSin1Phi* DVCSAltIntSinPhiMPhisSin1Phi::clone() const {
    return new DVCSAltIntSinPhiMPhisSin1Phi(*this);
}

void DVCSAltIntSinPhiMPhisSin1Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAltIntSinPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAltIntSinPhiMPhisSin1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAltIntSinPhiMPhis::computePhiObservable(x) * sin(x);
}

double DVCSAltIntSinPhiMPhisSin1Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (Constant::PI);
}

} /* namespace PARTONS */
