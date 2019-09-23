#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntCosPhiMPhisCos0Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAltIntCosPhiMPhisCos0Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAltIntCosPhiMPhisCos0Phi(
                        "DVCSAltIntCosPhiMPhisCos0Phi"));

DVCSAltIntCosPhiMPhisCos0Phi::DVCSAltIntCosPhiMPhisCos0Phi(
        const std::string &className) :
        DVCSAltIntCosPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAltIntCosPhiMPhisCos0Phi::DVCSAltIntCosPhiMPhisCos0Phi(
        const DVCSAltIntCosPhiMPhisCos0Phi& other) :
        DVCSAltIntCosPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAltIntCosPhiMPhisCos0Phi::~DVCSAltIntCosPhiMPhisCos0Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAltIntCosPhiMPhisCos0Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAltIntCosPhiMPhisCos0Phi::functionToIntegrateObservable);
}

DVCSAltIntCosPhiMPhisCos0Phi* DVCSAltIntCosPhiMPhisCos0Phi::clone() const {
    return new DVCSAltIntCosPhiMPhisCos0Phi(*this);
}

void DVCSAltIntCosPhiMPhisCos0Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAltIntCosPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAltIntCosPhiMPhisCos0Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAltIntCosPhiMPhis::computeObservable(
            DVCSObservableKinematic(m_xB, m_t, m_Q2, m_E, x),
            gpdTypesFromVector(params)).getValue(); // * cos(0 * x);
}

PhysicalType<double> DVCSAltIntCosPhiMPhisCos0Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = gpdTypesToVector(gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (2 * Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
