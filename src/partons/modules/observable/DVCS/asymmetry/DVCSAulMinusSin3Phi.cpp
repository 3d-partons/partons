#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAulMinusSin3Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {


const unsigned int DVCSAulMinusSin3Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAulMinusSin3Phi("DVCSAulMinusSin3Phi"));

DVCSAulMinusSin3Phi::DVCSAulMinusSin3Phi(const std::string &className) :
        DVCSAulMinus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAulMinusSin3Phi::DVCSAulMinusSin3Phi(const DVCSAulMinusSin3Phi& other) :
        DVCSAulMinus(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

DVCSAulMinusSin3Phi::~DVCSAulMinusSin3Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAulMinusSin3Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAulMinusSin3Phi::functionToIntegrateObservable);
}

DVCSAulMinusSin3Phi* DVCSAulMinusSin3Phi::clone() const {
    return new DVCSAulMinusSin3Phi(*this);
}

void DVCSAulMinusSin3Phi::configure(const ElemUtils::Parameters &parameters) {

	DVCSAulMinus::configure(parameters);
	MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAulMinusSin3Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAulMinus::computePhiObservable(x) * sin(3 * x);
}

double DVCSAulMinusSin3Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / Constant::PI;
}

} /* namespace PARTONS */
