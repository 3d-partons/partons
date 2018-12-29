#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAcCos1Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {


const unsigned int DVCSAcCos1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAcCos1Phi("DVCSAcCos1Phi"));

DVCSAcCos1Phi::DVCSAcCos1Phi(const std::string &className) :
        DVCSAc(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ChannelType::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAcCos1Phi::DVCSAcCos1Phi(const DVCSAcCos1Phi& other) :
        DVCSAc(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAcCos1Phi::~DVCSAcCos1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAcCos1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAcCos1Phi::functionToIntegrateObservable);
}

DVCSAcCos1Phi* DVCSAcCos1Phi::clone() const {
    return new DVCSAcCos1Phi(*this);
}

void DVCSAcCos1Phi::configure(const ElemUtils::Parameters &parameters) {

	DVCSAc::configure(parameters);
	MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAcCos1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAc::computePhiObservable(x) * cos(x);
}

double DVCSAcCos1Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / Constant::PI;
}

} /* namespace PARTONS */
