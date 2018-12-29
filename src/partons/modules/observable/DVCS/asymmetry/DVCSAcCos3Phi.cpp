#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAcCos3Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {


const unsigned int DVCSAcCos3Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAcCos3Phi("DVCSAcCos3Phi"));

DVCSAcCos3Phi::DVCSAcCos3Phi(const std::string &className) :
        DVCSAc(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ChannelType::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAcCos3Phi::DVCSAcCos3Phi(const DVCSAcCos3Phi& other) :
        DVCSAc(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAcCos3Phi::~DVCSAcCos3Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAcCos3Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAcCos3Phi::functionToIntegrateObservable);
}

DVCSAcCos3Phi* DVCSAcCos3Phi::clone() const {
    return new DVCSAcCos3Phi(*this);
}

void DVCSAcCos3Phi::configure(const ElemUtils::Parameters &parameters) {

	DVCSAc::configure(parameters);
	MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAcCos3Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAc::computePhiObservable(x) * cos(3 * x);
}

double DVCSAcCos3Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / Constant::PI;
}

} /* namespace PARTONS */
