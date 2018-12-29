#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAltDVCSSinPhiMPhisSin1Phi.h"
#include "../../../../../../include/partons/beans/channel/ObservableChannel.h"

namespace PARTONS {

const unsigned int DVCSAltDVCSSinPhiMPhisSin1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAltDVCSSinPhiMPhisSin1Phi(
                        "DVCSAltDVCSSinPhiMPhisSin1Phi"));

DVCSAltDVCSSinPhiMPhisSin1Phi::DVCSAltDVCSSinPhiMPhisSin1Phi(
        const std::string &className) :
        DVCSAltDVCSSinPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ChannelType::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAltDVCSSinPhiMPhisSin1Phi::DVCSAltDVCSSinPhiMPhisSin1Phi(
        const DVCSAltDVCSSinPhiMPhisSin1Phi& other) :
        DVCSAltDVCSSinPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAltDVCSSinPhiMPhisSin1Phi::~DVCSAltDVCSSinPhiMPhisSin1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAltDVCSSinPhiMPhisSin1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAltDVCSSinPhiMPhisSin1Phi::functionToIntegrateObservable);
}

DVCSAltDVCSSinPhiMPhisSin1Phi* DVCSAltDVCSSinPhiMPhisSin1Phi::clone() const {
    return new DVCSAltDVCSSinPhiMPhisSin1Phi(*this);
}

void DVCSAltDVCSSinPhiMPhisSin1Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAltDVCSSinPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAltDVCSSinPhiMPhisSin1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAltDVCSSinPhiMPhis::computePhiObservable(x) * sin(x);
}

double DVCSAltDVCSSinPhiMPhisSin1Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (Constant::PI);
}

} /* namespace PARTONS */
