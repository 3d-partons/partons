#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAulMinusSin1Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAulMinusSin1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAulMinusSin1Phi("DVCSAulMinusSin1Phi"));

DVCSAulMinusSin1Phi::DVCSAulMinusSin1Phi(const std::string &className) :
        DVCSAulMinus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAulMinusSin1Phi::DVCSAulMinusSin1Phi(const DVCSAulMinusSin1Phi& other) :
        DVCSAulMinus(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

DVCSAulMinusSin1Phi::~DVCSAulMinusSin1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAulMinusSin1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAulMinusSin1Phi::functionToIntegrateObservable);
}

DVCSAulMinusSin1Phi* DVCSAulMinusSin1Phi::clone() const {
    return new DVCSAulMinusSin1Phi(*this);
}

void DVCSAulMinusSin1Phi::configure(const ElemUtils::Parameters &parameters) {

    DVCSAulMinus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAulMinusSin1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAulMinus::computeObservable(
            DVCSObservableKinematic(m_xB, m_t, m_Q2, m_E, x),
            gpdTypesFromVector(params)).getValue() * sin(x);
}

PhysicalType<double> DVCSAulMinusSin1Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = gpdTypesToVector(gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
