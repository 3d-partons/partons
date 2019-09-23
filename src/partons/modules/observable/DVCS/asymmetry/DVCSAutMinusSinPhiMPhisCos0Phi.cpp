#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAutMinusSinPhiMPhisCos0Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAutMinusSinPhiMPhisCos0Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAutMinusSinPhiMPhisCos0Phi(
                        "DVCSAutMinusSinPhiMPhisCos0Phi"));

DVCSAutMinusSinPhiMPhisCos0Phi::DVCSAutMinusSinPhiMPhisCos0Phi(
        const std::string &className) :
        DVCSAutMinusSinPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAutMinusSinPhiMPhisCos0Phi::DVCSAutMinusSinPhiMPhisCos0Phi(
        const DVCSAutMinusSinPhiMPhisCos0Phi& other) :
        DVCSAutMinusSinPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAutMinusSinPhiMPhisCos0Phi::~DVCSAutMinusSinPhiMPhisCos0Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAutMinusSinPhiMPhisCos0Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAutMinusSinPhiMPhisCos0Phi::functionToIntegrateObservable);
}

DVCSAutMinusSinPhiMPhisCos0Phi* DVCSAutMinusSinPhiMPhisCos0Phi::clone() const {
    return new DVCSAutMinusSinPhiMPhisCos0Phi(*this);
}

void DVCSAutMinusSinPhiMPhisCos0Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAutMinusSinPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAutMinusSinPhiMPhisCos0Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAutMinusSinPhiMPhis::computeObservable(
            DVCSObservableKinematic(m_xB, m_t, m_Q2, m_E, x),
            gpdTypesFromVector(params)).getValue(); // * cos(0 * x);
}

PhysicalType<double> DVCSAutMinusSinPhiMPhisCos0Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = gpdTypesToVector(gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (2 * Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
