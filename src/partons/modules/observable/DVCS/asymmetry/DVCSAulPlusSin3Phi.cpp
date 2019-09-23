#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAulPlusSin3Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAulPlusSin3Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAulPlusSin3Phi("DVCSAulPlusSin3Phi"));

DVCSAulPlusSin3Phi::DVCSAulPlusSin3Phi(const std::string &className) :
        DVCSAulPlus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAulPlusSin3Phi::DVCSAulPlusSin3Phi(const DVCSAulPlusSin3Phi& other) :
        DVCSAulPlus(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

DVCSAulPlusSin3Phi::~DVCSAulPlusSin3Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAulPlusSin3Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAulPlusSin3Phi::functionToIntegrateObservable);
}

DVCSAulPlusSin3Phi* DVCSAulPlusSin3Phi::clone() const {
    return new DVCSAulPlusSin3Phi(*this);
}

void DVCSAulPlusSin3Phi::configure(const ElemUtils::Parameters &parameters) {

    DVCSAulPlus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAulPlusSin3Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAulPlus::computeObservable(
            DVCSObservableKinematic(m_xB, m_t, m_Q2, m_E, x),
            gpdTypesFromVector(params)).getValue() * sin(3 * x);
}

PhysicalType<double> DVCSAulPlusSin3Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = gpdTypesToVector(gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
