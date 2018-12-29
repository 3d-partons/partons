#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntCosPhiMPhisCos2Phi.h"
#include "../../../../../../include/partons/beans/channel/ObservableChannel.h"

namespace PARTONS {

const unsigned int DVCSAltIntCosPhiMPhisCos2Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAltIntCosPhiMPhisCos2Phi(
                        "DVCSAltIntCosPhiMPhisCos2Phi"));

DVCSAltIntCosPhiMPhisCos2Phi::DVCSAltIntCosPhiMPhisCos2Phi(
        const std::string &className) :
        DVCSAltIntCosPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ChannelType::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAltIntCosPhiMPhisCos2Phi::DVCSAltIntCosPhiMPhisCos2Phi(
        const DVCSAltIntCosPhiMPhisCos2Phi& other) :
        DVCSAltIntCosPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAltIntCosPhiMPhisCos2Phi::~DVCSAltIntCosPhiMPhisCos2Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAltIntCosPhiMPhisCos2Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAltIntCosPhiMPhisCos2Phi::functionToIntegrateObservable);
}

DVCSAltIntCosPhiMPhisCos2Phi* DVCSAltIntCosPhiMPhisCos2Phi::clone() const {
    return new DVCSAltIntCosPhiMPhisCos2Phi(*this);
}

void DVCSAltIntCosPhiMPhisCos2Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAltIntCosPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAltIntCosPhiMPhisCos2Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAltIntCosPhiMPhis::computePhiObservable(x) * cos(2 * x);
}

double DVCSAltIntCosPhiMPhisCos2Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (Constant::PI);
}

} /* namespace PARTONS */
