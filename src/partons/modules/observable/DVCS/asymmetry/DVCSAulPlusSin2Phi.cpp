#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAulPlusSin2Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAulPlusSin2Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAulPlusSin2Phi("DVCSAulPlusSin2Phi"));

DVCSAulPlusSin2Phi::DVCSAulPlusSin2Phi(const std::string &className) :
        DVCSAulPlus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAulPlusSin2Phi::DVCSAulPlusSin2Phi(const DVCSAulPlusSin2Phi& other) :
        DVCSAulPlus(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

DVCSAulPlusSin2Phi::~DVCSAulPlusSin2Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAulPlusSin2Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAulPlusSin2Phi::functionToIntegrateObservable);
}

DVCSAulPlusSin2Phi* DVCSAulPlusSin2Phi::clone() const {
    return new DVCSAulPlusSin2Phi(*this);
}

void DVCSAulPlusSin2Phi::configure(const ElemUtils::Parameters &parameters) {

    DVCSAulPlus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAulPlusSin2Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAulPlus::computeObservable(
            DVCSObservableKinematic(m_xB, m_t, m_Q2, m_E, x),
            gpdTypesFromVector(params)).getValue() * sin(2 * x);
}

PhysicalType<double> DVCSAulPlusSin2Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = gpdTypesToVector(gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
