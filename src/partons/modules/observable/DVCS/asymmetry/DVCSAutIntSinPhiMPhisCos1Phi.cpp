#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAutIntSinPhiMPhisCos1Phi.h"
#include "../../../../../../include/partons/beans/channel/ObservableChannel.h"

namespace PARTONS {

const unsigned int DVCSAutIntSinPhiMPhisCos1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAutIntSinPhiMPhisCos1Phi(
                        "DVCSAutIntSinPhiMPhisCos1Phi"));

DVCSAutIntSinPhiMPhisCos1Phi::DVCSAutIntSinPhiMPhisCos1Phi(
        const std::string &className) :
        DVCSAutIntSinPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ChannelType::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAutIntSinPhiMPhisCos1Phi::DVCSAutIntSinPhiMPhisCos1Phi(
        const DVCSAutIntSinPhiMPhisCos1Phi& other) :
        DVCSAutIntSinPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAutIntSinPhiMPhisCos1Phi::~DVCSAutIntSinPhiMPhisCos1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAutIntSinPhiMPhisCos1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAutIntSinPhiMPhisCos1Phi::functionToIntegrateObservable);
}

DVCSAutIntSinPhiMPhisCos1Phi* DVCSAutIntSinPhiMPhisCos1Phi::clone() const {
    return new DVCSAutIntSinPhiMPhisCos1Phi(*this);
}

void DVCSAutIntSinPhiMPhisCos1Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAutIntSinPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAutIntSinPhiMPhisCos1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAutIntSinPhiMPhis::computePhiObservable(x) * cos(x);
}

double DVCSAutIntSinPhiMPhisCos1Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (Constant::PI);
}

} /* namespace PARTONS */
