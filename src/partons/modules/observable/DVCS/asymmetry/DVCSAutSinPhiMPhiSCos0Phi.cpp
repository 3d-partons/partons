#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAutSinPhiMPhiSCos0Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

const unsigned int DVCSAutSinPhiMPhiSCos0Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAutSinPhiMPhiSCos0Phi("DVCSAutSinPhiMPhiSCos0Phi"));

DVCSAutSinPhiMPhiSCos0Phi::DVCSAutSinPhiMPhiSCos0Phi(
        const std::string &className) :
        DVCSAutSinPhiMPhiS(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAutSinPhiMPhiSCos0Phi::DVCSAutSinPhiMPhiSCos0Phi(
        const DVCSAutSinPhiMPhiSCos0Phi& other) :
        DVCSAutSinPhiMPhiS(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAutSinPhiMPhiSCos0Phi::~DVCSAutSinPhiMPhiSCos0Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAutSinPhiMPhiSCos0Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAutSinPhiMPhiSCos0Phi::functionToIntegrateObservable);
}

DVCSAutSinPhiMPhiSCos0Phi* DVCSAutSinPhiMPhiSCos0Phi::clone() const {
    return new DVCSAutSinPhiMPhiSCos0Phi(*this);
}

double DVCSAutSinPhiMPhiSCos0Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAutSinPhiMPhiS::computePhiObservable(x) * cos(x);
}

double DVCSAutSinPhiMPhiSCos0Phi::computeFourierObservable() {

    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / (2 * Constant::PI);
}
