#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAulPlusSin3Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {

const unsigned int DVCSAulPlusSin3Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAulPlusSin3Phi("DVCSAulPlusSin3Phi"));

DVCSAulPlusSin3Phi::DVCSAulPlusSin3Phi(const std::string &className) :
        DVCSAulPlus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAulPlusSin3Phi::DVCSAulPlusSin3Phi(const DVCSAulPlusSin3Phi& other) :
        DVCSAulPlus(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

DVCSAulPlusSin3Phi::~DVCSAulPlusSin3Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAulPlusSin3Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAulPlusSin3Phi::functionToIntegrateObservable);
}

DVCSAulPlusSin3Phi* DVCSAulPlusSin3Phi::clone() const {
    return new DVCSAulPlusSin3Phi(*this);
}

void DVCSAulPlusSin3Phi::configure(const ElemUtils::Parameters &parameters) {

    DVCSAulPlus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAulPlusSin3Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAulPlus::computePhiObservable(x) * sin(3 * x);
}

double DVCSAulPlusSin3Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / Constant::PI;
}

} /* namespace PARTONS */
