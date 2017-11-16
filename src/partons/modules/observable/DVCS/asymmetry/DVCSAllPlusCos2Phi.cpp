#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAllPlusCos2Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {

const unsigned int DVCSAllPlusCos2Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAllPlusCos2Phi("DVCSAllPlusCos2Phi"));

DVCSAllPlusCos2Phi::DVCSAllPlusCos2Phi(const std::string &className) :
        DVCSAllPlus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAllPlusCos2Phi::DVCSAllPlusCos2Phi(const DVCSAllPlusCos2Phi& other) :
        DVCSAllPlus(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAllPlusCos2Phi::~DVCSAllPlusCos2Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAllPlusCos2Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAllPlusCos2Phi::functionToIntegrateObservable);
}

DVCSAllPlusCos2Phi* DVCSAllPlusCos2Phi::clone() const {
    return new DVCSAllPlusCos2Phi(*this);
}

void DVCSAllPlusCos2Phi::configure(const ElemUtils::Parameters &parameters) {

    DVCSAllPlus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAllPlusCos2Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAllPlus::computePhiObservable(x) * cos(2 * x);
}

double DVCSAllPlusCos2Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (Constant::PI);
}

} /* namespace PARTONS */
