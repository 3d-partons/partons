#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAulPlusSin1Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {

const unsigned int DVCSAulPlusSin1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAulPlusSin1Phi("DVCSAulPlusSin1Phi"));

DVCSAulPlusSin1Phi::DVCSAulPlusSin1Phi(const std::string &className) :
        DVCSAulPlus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAulPlusSin1Phi::DVCSAulPlusSin1Phi(const DVCSAulPlusSin1Phi& other) :
        DVCSAulPlus(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

DVCSAulPlusSin1Phi::~DVCSAulPlusSin1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAulPlusSin1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAulPlusSin1Phi::functionToIntegrateObservable);
}

DVCSAulPlusSin1Phi* DVCSAulPlusSin1Phi::clone() const {
    return new DVCSAulPlusSin1Phi(*this);
}

void DVCSAulPlusSin1Phi::configure(const ElemUtils::Parameters &parameters) {

    DVCSAulPlus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAulPlusSin1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAulPlus::computePhiObservable(x) * sin(x);
}

double DVCSAulPlusSin1Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / Constant::PI;
}

} /* namespace PARTONS */
