#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAluDVCSSin1Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

const unsigned int DVCSAluDVCSSin1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAluDVCSSin1Phi("DVCSAluDVCSSin1Phi"));

DVCSAluDVCSSin1Phi::DVCSAluDVCSSin1Phi(const std::string &className) :
        DVCSAluDVCS(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAluDVCSSin1Phi::DVCSAluDVCSSin1Phi(const DVCSAluDVCSSin1Phi& other) :
        DVCSAluDVCS(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

DVCSAluDVCSSin1Phi::~DVCSAluDVCSSin1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAluDVCSSin1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAluDVCSSin1Phi::functionToIntegrateObservable);
}

DVCSAluDVCSSin1Phi* DVCSAluDVCSSin1Phi::clone() const {
    return new DVCSAluDVCSSin1Phi(*this);
}

void DVCSAluDVCSSin1Phi::configure(const ElemUtils::Parameters &parameters) {

	DVCSAluDVCS::configure(parameters);
	MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAluDVCSSin1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAluDVCS::computePhiObservable(x) * sin(x);
}

double DVCSAluDVCSSin1Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / Constant::PI;

}
