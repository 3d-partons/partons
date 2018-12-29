#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntCosPhiMPhisCos1Phi.h"
#include "../../../../../../include/partons/beans/channel/ObservableChannel.h"

namespace PARTONS {

const unsigned int DVCSAltIntCosPhiMPhisCos1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAltIntCosPhiMPhisCos1Phi(
                        "DVCSAltIntCosPhiMPhisCos1Phi"));

DVCSAltIntCosPhiMPhisCos1Phi::DVCSAltIntCosPhiMPhisCos1Phi(
        const std::string &className) :
        DVCSAltIntCosPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ChannelType::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAltIntCosPhiMPhisCos1Phi::DVCSAltIntCosPhiMPhisCos1Phi(
        const DVCSAltIntCosPhiMPhisCos1Phi& other) :
        DVCSAltIntCosPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAltIntCosPhiMPhisCos1Phi::~DVCSAltIntCosPhiMPhisCos1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAltIntCosPhiMPhisCos1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAltIntCosPhiMPhisCos1Phi::functionToIntegrateObservable);
}

DVCSAltIntCosPhiMPhisCos1Phi* DVCSAltIntCosPhiMPhisCos1Phi::clone() const {
    return new DVCSAltIntCosPhiMPhisCos1Phi(*this);
}

void DVCSAltIntCosPhiMPhisCos1Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAltIntCosPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAltIntCosPhiMPhisCos1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAltIntCosPhiMPhis::computePhiObservable(x) * cos(x);
}

double DVCSAltIntCosPhiMPhisCos1Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (Constant::PI);
}

} /* namespace PARTONS */
