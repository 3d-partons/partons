#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAllMinusCos1Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAllMinusCos1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAllMinusCos1Phi("DVCSAllMinusCos1Phi"));

DVCSAllMinusCos1Phi::DVCSAllMinusCos1Phi(const std::string &className) :
        DVCSAllMinus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAllMinusCos1Phi::DVCSAllMinusCos1Phi(const DVCSAllMinusCos1Phi& other) :
        DVCSAllMinus(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAllMinusCos1Phi::~DVCSAllMinusCos1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAllMinusCos1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAllMinusCos1Phi::functionToIntegrateObservable);
}

DVCSAllMinusCos1Phi* DVCSAllMinusCos1Phi::clone() const {
    return new DVCSAllMinusCos1Phi(*this);
}

void DVCSAllMinusCos1Phi::configure(const ElemUtils::Parameters &parameters) {

    DVCSAllMinus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAllMinusCos1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAllMinus::computeObservable(
            DVCSObservableKinematic(m_xB, m_t, m_Q2, m_E, x),
            gpdTypesFromVector(params)).getValue() * cos(x);
}

PhysicalType<double> DVCSAllMinusCos1Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = gpdTypesToVector(gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
