#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAutSinPhiMPhisCos0Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

const unsigned int DVCSAutSinPhiMPhisCos0Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAutSinPhiMPhisCos0Phi("DVCSAutSinPhiMPhisCos0Phi"));

DVCSAutSinPhiMPhisCos0Phi::DVCSAutSinPhiMPhisCos0Phi(
        const std::string &className) :
        DVCSAutSinPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAutSinPhiMPhisCos0Phi::DVCSAutSinPhiMPhisCos0Phi(
        const DVCSAutSinPhiMPhisCos0Phi& other) :
        DVCSAutSinPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAutSinPhiMPhisCos0Phi::~DVCSAutSinPhiMPhisCos0Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAutSinPhiMPhisCos0Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAutSinPhiMPhisCos0Phi::functionToIntegrateObservable);
}

DVCSAutSinPhiMPhisCos0Phi* DVCSAutSinPhiMPhisCos0Phi::clone() const {
    return new DVCSAutSinPhiMPhisCos0Phi(*this);
}

void DVCSAutSinPhiMPhisCos0Phi::configure(const ElemUtils::Parameters &parameters) {

	DVCSAutSinPhiMPhis::configure(parameters);
	MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAutSinPhiMPhisCos0Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAutSinPhiMPhis::computePhiObservable(x) * cos(x);
}

double DVCSAutSinPhiMPhisCos0Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (2 * Constant::PI);
}
