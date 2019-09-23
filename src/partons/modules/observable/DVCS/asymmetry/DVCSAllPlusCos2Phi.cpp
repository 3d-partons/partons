#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAllPlusCos2Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAllPlusCos2Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAllPlusCos2Phi("DVCSAllPlusCos2Phi"));

DVCSAllPlusCos2Phi::DVCSAllPlusCos2Phi(const std::string &className) :
        DVCSAllPlus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

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
    return DVCSAllPlus::computeObservable(
            DVCSObservableKinematic(m_xB, m_t, m_Q2, m_E, x),
            gpdTypesFromVector(params)).getValue() * cos(2 * x);
}

PhysicalType<double> DVCSAllPlusCos2Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = gpdTypesToVector(gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
