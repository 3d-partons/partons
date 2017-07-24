#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAllMinusCos0Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

const unsigned int DVCSAllMinusCos0Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAllMinusCos0Phi("DVCSAllMinusCos0Phi"));

DVCSAllMinusCos0Phi::DVCSAllMinusCos0Phi(const std::string &className) :
        DVCSAllMinus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAllMinusCos0Phi::DVCSAllMinusCos0Phi(const DVCSAllMinusCos0Phi& other) :
        DVCSAllMinus(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAllMinusCos0Phi::~DVCSAllMinusCos0Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAllMinusCos0Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAllMinusCos0Phi::functionToIntegrateObservable);
}

DVCSAllMinusCos0Phi* DVCSAllMinusCos0Phi::clone() const {
    return new DVCSAllMinusCos0Phi(*this);
}

void DVCSAllMinusCos0Phi::configure(const ElemUtils::Parameters &parameters) {

	DVCSAllMinus::configure(parameters);
	MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAllMinusCos0Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAllMinus::computePhiObservable(x) * cos(0 * x);
}

double DVCSAllMinusCos0Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (2 * Constant::PI);
}
