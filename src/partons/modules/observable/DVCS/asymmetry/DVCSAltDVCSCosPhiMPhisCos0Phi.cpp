#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAltDVCSCosPhiMPhisCos0Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAltDVCSCosPhiMPhisCos0Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAltDVCSCosPhiMPhisCos0Phi(
                        "DVCSAltDVCSCosPhiMPhisCos0Phi"));

DVCSAltDVCSCosPhiMPhisCos0Phi::DVCSAltDVCSCosPhiMPhisCos0Phi(
        const std::string &className) :
        DVCSAltDVCSCosPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

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
    return DVCSAltDVCSCosPhiMPhis::computeObservable(
            DVCSObservableKinematic(m_xB, m_t, m_Q2, m_E, x),
            gpdTypesFromVector(params)).getValue(); // * cos(0 * x);
}

PhysicalType<double> DVCSAltDVCSCosPhiMPhisCos0Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = gpdTypesToVector(gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (2 * Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
