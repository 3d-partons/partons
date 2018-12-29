#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAutDVCSSinPhiMPhisCos0Phi.h"
#include "../../../../../../include/partons/beans/channel/ObservableChannel.h"

namespace PARTONS {

const unsigned int DVCSAutDVCSSinPhiMPhisCos0Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAutDVCSSinPhiMPhisCos0Phi(
                        "DVCSAutDVCSSinPhiMPhisCos0Phi"));

DVCSAutDVCSSinPhiMPhisCos0Phi::DVCSAutDVCSSinPhiMPhisCos0Phi(
        const std::string &className) :
        DVCSAutDVCSSinPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ChannelType::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAutDVCSSinPhiMPhisCos0Phi::DVCSAutDVCSSinPhiMPhisCos0Phi(
        const DVCSAutDVCSSinPhiMPhisCos0Phi& other) :
        DVCSAutDVCSSinPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAutDVCSSinPhiMPhisCos0Phi::~DVCSAutDVCSSinPhiMPhisCos0Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAutDVCSSinPhiMPhisCos0Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAutDVCSSinPhiMPhisCos0Phi::functionToIntegrateObservable);
}

DVCSAutDVCSSinPhiMPhisCos0Phi* DVCSAutDVCSSinPhiMPhisCos0Phi::clone() const {
    return new DVCSAutDVCSSinPhiMPhisCos0Phi(*this);
}

void DVCSAutDVCSSinPhiMPhisCos0Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAutDVCSSinPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAutDVCSSinPhiMPhisCos0Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAutDVCSSinPhiMPhis::computePhiObservable(x);// * cos(0 * x);
}

double DVCSAutDVCSSinPhiMPhisCos0Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (2 * Constant::PI);
}

} /* namespace PARTONS */
