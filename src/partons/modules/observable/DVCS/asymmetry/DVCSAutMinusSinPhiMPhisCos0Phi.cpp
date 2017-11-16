#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAutMinusSinPhiMPhisCos0Phi.h"

namespace PARTONS {

const unsigned int DVCSAutMinusSinPhiMPhisCos0Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAutMinusSinPhiMPhisCos0Phi(
                        "DVCSAutMinusSinPhiMPhisCos0Phi"));

DVCSAutMinusSinPhiMPhisCos0Phi::DVCSAutMinusSinPhiMPhisCos0Phi(
        const std::string &className) :
        DVCSAutMinusSinPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAutMinusSinPhiMPhisCos0Phi::DVCSAutMinusSinPhiMPhisCos0Phi(
        const DVCSAutMinusSinPhiMPhisCos0Phi& other) :
        DVCSAutMinusSinPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAutMinusSinPhiMPhisCos0Phi::~DVCSAutMinusSinPhiMPhisCos0Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAutMinusSinPhiMPhisCos0Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAutMinusSinPhiMPhisCos0Phi::functionToIntegrateObservable);
}

DVCSAutMinusSinPhiMPhisCos0Phi* DVCSAutMinusSinPhiMPhisCos0Phi::clone() const {
    return new DVCSAutMinusSinPhiMPhisCos0Phi(*this);
}

void DVCSAutMinusSinPhiMPhisCos0Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAutMinusSinPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAutMinusSinPhiMPhisCos0Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAutMinusSinPhiMPhis::computePhiObservable(x);// * cos(0 * x);
}

double DVCSAutMinusSinPhiMPhisCos0Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (2 * Constant::PI);
}

} /* namespace PARTONS */
