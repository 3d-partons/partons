#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAulMinusSin2Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

const unsigned int DVCSAulMinusSin2Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAulMinusSin2Phi("DVCSAulMinusSin2Phi"));

DVCSAulMinusSin2Phi::DVCSAulMinusSin2Phi(const std::string &className) :
        DVCSAulMinus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAulMinusSin2Phi::DVCSAulMinusSin2Phi(const DVCSAulMinusSin2Phi& other) :
        DVCSAulMinus(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

DVCSAulMinusSin2Phi::~DVCSAulMinusSin2Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAulMinusSin2Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAulMinusSin2Phi::functionToIntegrateObservable);
}

DVCSAulMinusSin2Phi* DVCSAulMinusSin2Phi::clone() const {
    return new DVCSAulMinusSin2Phi(*this);
}

void DVCSAulMinusSin2Phi::configure(const ElemUtils::Parameters &parameters) {

	DVCSAulMinus::configure(parameters);
	MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAulMinusSin2Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAulMinus::computePhiObservable(x) * sin(2 * x);
}

double DVCSAulMinusSin2Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / Constant::PI;
}
