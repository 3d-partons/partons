#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAltDVCSCosPhiMPhisCos0Phi.h"

namespace PARTONS {

const unsigned int DVCSAltDVCSCosPhiMPhisCos0Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAltDVCSCosPhiMPhisCos0Phi(
                        "DVCSAltDVCSCosPhiMPhisCos0Phi"));

DVCSAltDVCSCosPhiMPhisCos0Phi::DVCSAltDVCSCosPhiMPhisCos0Phi(
        const std::string &className) :
        DVCSAltDVCSCosPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAltDVCSCosPhiMPhisCos0Phi::DVCSAltDVCSCosPhiMPhisCos0Phi(
        const DVCSAltDVCSCosPhiMPhisCos0Phi& other) :
        DVCSAltDVCSCosPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAltDVCSCosPhiMPhisCos0Phi::~DVCSAltDVCSCosPhiMPhisCos0Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAltDVCSCosPhiMPhisCos0Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAltDVCSCosPhiMPhisCos0Phi::functionToIntegrateObservable);
}

DVCSAltDVCSCosPhiMPhisCos0Phi* DVCSAltDVCSCosPhiMPhisCos0Phi::clone() const {
    return new DVCSAltDVCSCosPhiMPhisCos0Phi(*this);
}

void DVCSAltDVCSCosPhiMPhisCos0Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAltDVCSCosPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAltDVCSCosPhiMPhisCos0Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAltDVCSCosPhiMPhis::computePhiObservable(x);// * cos(0 * x);
}

double DVCSAltDVCSCosPhiMPhisCos0Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (2 * Constant::PI);
}

} /* namespace PARTONS */
