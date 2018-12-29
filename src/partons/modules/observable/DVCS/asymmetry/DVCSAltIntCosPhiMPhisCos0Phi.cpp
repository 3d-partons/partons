#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntCosPhiMPhisCos0Phi.h"
#include "../../../../../../include/partons/beans/channel/ObservableChannel.h"

namespace PARTONS {

const unsigned int DVCSAltIntCosPhiMPhisCos0Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAltIntCosPhiMPhisCos0Phi(
                        "DVCSAltIntCosPhiMPhisCos0Phi"));

DVCSAltIntCosPhiMPhisCos0Phi::DVCSAltIntCosPhiMPhisCos0Phi(
        const std::string &className) :
        DVCSAltIntCosPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ChannelType::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAltIntCosPhiMPhisCos0Phi::DVCSAltIntCosPhiMPhisCos0Phi(
        const DVCSAltIntCosPhiMPhisCos0Phi& other) :
        DVCSAltIntCosPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAltIntCosPhiMPhisCos0Phi::~DVCSAltIntCosPhiMPhisCos0Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAltIntCosPhiMPhisCos0Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAltIntCosPhiMPhisCos0Phi::functionToIntegrateObservable);
}

DVCSAltIntCosPhiMPhisCos0Phi* DVCSAltIntCosPhiMPhisCos0Phi::clone() const {
    return new DVCSAltIntCosPhiMPhisCos0Phi(*this);
}

void DVCSAltIntCosPhiMPhisCos0Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAltIntCosPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAltIntCosPhiMPhisCos0Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAltIntCosPhiMPhis::computePhiObservable(x); // * cos(0 * x);
}

double DVCSAltIntCosPhiMPhisCos0Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (2 * Constant::PI);
}

} /* namespace PARTONS */
