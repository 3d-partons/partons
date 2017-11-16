#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntSinPhiMPhisSin2Phi.h"

namespace PARTONS {

const unsigned int DVCSAltIntSinPhiMPhisSin2Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAltIntSinPhiMPhisSin2Phi(
                        "DVCSAltIntSinPhiMPhisSin2Phi"));

DVCSAltIntSinPhiMPhisSin2Phi::DVCSAltIntSinPhiMPhisSin2Phi(
        const std::string &className) :
        DVCSAltIntSinPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAltIntSinPhiMPhisSin2Phi::DVCSAltIntSinPhiMPhisSin2Phi(
        const DVCSAltIntSinPhiMPhisSin2Phi& other) :
        DVCSAltIntSinPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAltIntSinPhiMPhisSin2Phi::~DVCSAltIntSinPhiMPhisSin2Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAltIntSinPhiMPhisSin2Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAltIntSinPhiMPhisSin2Phi::functionToIntegrateObservable);
}

DVCSAltIntSinPhiMPhisSin2Phi* DVCSAltIntSinPhiMPhisSin2Phi::clone() const {
    return new DVCSAltIntSinPhiMPhisSin2Phi(*this);
}

void DVCSAltIntSinPhiMPhisSin2Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAltIntSinPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAltIntSinPhiMPhisSin2Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAltIntSinPhiMPhis::computePhiObservable(x) * sin(2 * x);
}

double DVCSAltIntSinPhiMPhisSin2Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (Constant::PI);
}

} /* namespace PARTONS */
