#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAcCos2Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

const unsigned int DVCSAcCos2Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAcCos2Phi("DVCSAcCos2Phi"));

DVCSAcCos2Phi::DVCSAcCos2Phi(const std::string &className) :
        DVCSAc(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAcCos2Phi::DVCSAcCos2Phi(const DVCSAcCos2Phi& other) :
        DVCSAc(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAcCos2Phi::~DVCSAcCos2Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAcCos2Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAcCos2Phi::functionToIntegrateObservable);
}

DVCSAcCos2Phi* DVCSAcCos2Phi::clone() const {
    return new DVCSAcCos2Phi(*this);
}

void DVCSAcCos2Phi::configure(const ElemUtils::Parameters &parameters) {

	DVCSAc::configure(parameters);
	MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAcCos2Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAc::computePhiObservable(x) * cos(2 * x);
}

double DVCSAcCos2Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / Constant::PI;
}
