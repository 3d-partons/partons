#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAutDVCSSinPhiMPhisCos0Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAutDVCSSinPhiMPhisCos0Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAutDVCSSinPhiMPhisCos0Phi(
                        "DVCSAutDVCSSinPhiMPhisCos0Phi"));

DVCSAutDVCSSinPhiMPhisCos0Phi::DVCSAutDVCSSinPhiMPhisCos0Phi(
        const std::string &className) :
        DVCSAutDVCSSinPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAutDVCSSinPhiMPhisCos0Phi::DVCSAutDVCSSinPhiMPhisCos0Phi(
        const DVCSAutDVCSSinPhiMPhisCos0Phi& other) :
        DVCSAutDVCSSinPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAutDVCSSinPhiMPhisCos0Phi::~DVCSAutDVCSSinPhiMPhisCos0Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAutDVCSSinPhiMPhisCos0Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAutDVCSSinPhiMPhisCos0Phi::functionToIntegrateObservable);
}

DVCSAutDVCSSinPhiMPhisCos0Phi* DVCSAutDVCSSinPhiMPhisCos0Phi::clone() const {
    return new DVCSAutDVCSSinPhiMPhisCos0Phi(*this);
}

void DVCSAutDVCSSinPhiMPhisCos0Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAutDVCSSinPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAutDVCSSinPhiMPhisCos0Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAutDVCSSinPhiMPhis::computeObservable(
            DVCSObservableKinematic(m_xB, m_t, m_Q2, m_E, x),
            gpdTypesFromVector(params)).getValue(); // * cos(0 * x);
}

PhysicalType<double> DVCSAutDVCSSinPhiMPhisCos0Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = gpdTypesToVector(gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (2 * Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
