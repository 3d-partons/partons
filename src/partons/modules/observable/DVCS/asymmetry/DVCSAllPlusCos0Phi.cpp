#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAllPlusCos0Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {

const unsigned int DVCSAllPlusCos0Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAllPlusCos0Phi("DVCSAllPlusCos0Phi"));

DVCSAllPlusCos0Phi::DVCSAllPlusCos0Phi(const std::string &className) :
        DVCSAllPlus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ChannelType::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAllPlusCos0Phi::DVCSAllPlusCos0Phi(const DVCSAllPlusCos0Phi& other) :
        DVCSAllPlus(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAllPlusCos0Phi::~DVCSAllPlusCos0Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAllPlusCos0Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAllPlusCos0Phi::functionToIntegrateObservable);
}

DVCSAllPlusCos0Phi* DVCSAllPlusCos0Phi::clone() const {
    return new DVCSAllPlusCos0Phi(*this);
}

void DVCSAllPlusCos0Phi::configure(const ElemUtils::Parameters &parameters) {

    DVCSAllPlus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAllPlusCos0Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAllPlus::computePhiObservable(x); // * cos(0 * x);
}

double DVCSAllPlusCos0Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (2 * Constant::PI);
}

} /* namespace PARTONS */
