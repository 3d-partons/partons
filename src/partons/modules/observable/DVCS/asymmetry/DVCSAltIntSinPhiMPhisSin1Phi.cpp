#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntSinPhiMPhisSin1Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAltIntSinPhiMPhisSin1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAltIntSinPhiMPhisSin1Phi(
                        "DVCSAltIntSinPhiMPhisSin1Phi"));

DVCSAltIntSinPhiMPhisSin1Phi::DVCSAltIntSinPhiMPhisSin1Phi(
        const std::string &className) :
        DVCSAltIntSinPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAltIntSinPhiMPhisSin1Phi::DVCSAltIntSinPhiMPhisSin1Phi(
        const DVCSAltIntSinPhiMPhisSin1Phi& other) :
        DVCSAltIntSinPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAltIntSinPhiMPhisSin1Phi::~DVCSAltIntSinPhiMPhisSin1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAltIntSinPhiMPhisSin1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAltIntSinPhiMPhisSin1Phi::functionToIntegrateObservable);
}

DVCSAltIntSinPhiMPhisSin1Phi* DVCSAltIntSinPhiMPhisSin1Phi::clone() const {
    return new DVCSAltIntSinPhiMPhisSin1Phi(*this);
}

void DVCSAltIntSinPhiMPhisSin1Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAltIntSinPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAltIntSinPhiMPhisSin1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAltIntSinPhiMPhis::computeObservable(
            DVCSObservableKinematic(m_xB, m_t, m_Q2, m_E, x),
            gpdTypesFromVector(params)).getValue() * sin(x);
}

PhysicalType<double> DVCSAltIntSinPhiMPhisSin1Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = gpdTypesToVector(gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
