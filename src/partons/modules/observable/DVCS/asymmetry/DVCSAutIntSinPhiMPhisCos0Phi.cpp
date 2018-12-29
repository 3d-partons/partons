#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAutIntSinPhiMPhisCos0Phi.h"
#include "../../../../../../include/partons/beans/channel/ObservableChannel.h"

namespace PARTONS {

const unsigned int DVCSAutIntSinPhiMPhisCos0Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAutIntSinPhiMPhisCos0Phi(
                        "DVCSAutIntSinPhiMPhisCos0Phi"));

DVCSAutIntSinPhiMPhisCos0Phi::DVCSAutIntSinPhiMPhisCos0Phi(
        const std::string &className) :
        DVCSAutIntSinPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ChannelType::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAutIntSinPhiMPhisCos0Phi::DVCSAutIntSinPhiMPhisCos0Phi(
        const DVCSAutIntSinPhiMPhisCos0Phi& other) :
        DVCSAutIntSinPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAutIntSinPhiMPhisCos0Phi::~DVCSAutIntSinPhiMPhisCos0Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAutIntSinPhiMPhisCos0Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAutIntSinPhiMPhisCos0Phi::functionToIntegrateObservable);
}

DVCSAutIntSinPhiMPhisCos0Phi* DVCSAutIntSinPhiMPhisCos0Phi::clone() const {
    return new DVCSAutIntSinPhiMPhisCos0Phi(*this);
}

void DVCSAutIntSinPhiMPhisCos0Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAutIntSinPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAutIntSinPhiMPhisCos0Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAutIntSinPhiMPhis::computePhiObservable(x);// * cos(0 * x);
}

double DVCSAutIntSinPhiMPhisCos0Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (2 * Constant::PI);
}

} /* namespace PARTONS */
