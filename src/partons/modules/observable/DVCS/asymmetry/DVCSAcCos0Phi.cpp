#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAcCos0Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {


const unsigned int DVCSAcCos0Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAcCos0Phi("DVCSAcCos0Phi"));

DVCSAcCos0Phi::DVCSAcCos0Phi(const std::string &className) :
        DVCSAc(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAcCos0Phi::DVCSAcCos0Phi(const DVCSAcCos0Phi& other) :
        DVCSAc(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAcCos0Phi::~DVCSAcCos0Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAcCos0Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAcCos0Phi::functionToIntegrateObservable);
}

DVCSAcCos0Phi* DVCSAcCos0Phi::clone() const {
    return new DVCSAcCos0Phi(*this);
}

void DVCSAcCos0Phi::configure(const ElemUtils::Parameters &parameters) {

	DVCSAc::configure(parameters);
	MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAcCos0Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAc::computePhiObservable(x);
}

double DVCSAcCos0Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (2 * Constant::PI);
}

} /* namespace PARTONS */
