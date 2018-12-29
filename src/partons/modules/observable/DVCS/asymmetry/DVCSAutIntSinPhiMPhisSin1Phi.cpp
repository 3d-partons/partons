#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAutIntSinPhiMPhisSin1Phi.h"
#include "../../../../../../include/partons/beans/channel/ObservableChannel.h"

namespace PARTONS {

const unsigned int DVCSAutIntSinPhiMPhisSin1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAutIntSinPhiMPhisSin1Phi(
                        "DVCSAutIntSinPhiMPhisSin1Phi"));

DVCSAutIntSinPhiMPhisSin1Phi::DVCSAutIntSinPhiMPhisSin1Phi(
        const std::string &className) :
        DVCSAutIntSinPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ChannelType::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAutIntSinPhiMPhisSin1Phi::DVCSAutIntSinPhiMPhisSin1Phi(
        const DVCSAutIntSinPhiMPhisSin1Phi& other) :
        DVCSAutIntSinPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAutIntSinPhiMPhisSin1Phi::~DVCSAutIntSinPhiMPhisSin1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAutIntSinPhiMPhisSin1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAutIntSinPhiMPhisSin1Phi::functionToIntegrateObservable);
}

DVCSAutIntSinPhiMPhisSin1Phi* DVCSAutIntSinPhiMPhisSin1Phi::clone() const {
    return new DVCSAutIntSinPhiMPhisSin1Phi(*this);
}

void DVCSAutIntSinPhiMPhisSin1Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAutIntSinPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAutIntSinPhiMPhisSin1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAutIntSinPhiMPhis::computePhiObservable(x) * sin(x);
}

double DVCSAutIntSinPhiMPhisSin1Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (Constant::PI);
}

} /* namespace PARTONS */
