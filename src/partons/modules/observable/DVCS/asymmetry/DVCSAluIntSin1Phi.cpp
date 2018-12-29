#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAluIntSin1Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {


const unsigned int DVCSAluIntSin1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAluIntSin1Phi("DVCSAluIntSin1Phi"));

DVCSAluIntSin1Phi::DVCSAluIntSin1Phi(const std::string &className) :
        DVCSAluInt(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ChannelType::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAluIntSin1Phi::DVCSAluIntSin1Phi(const DVCSAluIntSin1Phi& other) :
        DVCSAluInt(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

DVCSAluIntSin1Phi::~DVCSAluIntSin1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAluIntSin1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAluIntSin1Phi::functionToIntegrateObservable);
}

DVCSAluIntSin1Phi* DVCSAluIntSin1Phi::clone() const {
    return new DVCSAluIntSin1Phi(*this);
}

void DVCSAluIntSin1Phi::configure(const ElemUtils::Parameters &parameters) {

	DVCSAluInt::configure(parameters);
	MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAluIntSin1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAluInt::computePhiObservable(x) * sin(x);
}

double DVCSAluIntSin1Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / Constant::PI;
}

} /* namespace PARTONS */
