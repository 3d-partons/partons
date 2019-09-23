#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAluIntSin1Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAluIntSin1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAluIntSin1Phi("DVCSAluIntSin1Phi"));

DVCSAluIntSin1Phi::DVCSAluIntSin1Phi(const std::string &className) :
        DVCSAluInt(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAluIntSin1Phi::DVCSAluIntSin1Phi(const DVCSAluIntSin1Phi& other) :
        DVCSAluInt(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

DVCSAluIntSin1Phi::~DVCSAluIntSin1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAluIntSin1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAluIntSin1Phi::functionToIntegrateObservable);
}

DVCSAluIntSin1Phi* DVCSAluIntSin1Phi::clone() const {
    return new DVCSAluIntSin1Phi(*this);
}

void DVCSAluIntSin1Phi::configure(const ElemUtils::Parameters &parameters) {

    DVCSAluInt::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAluIntSin1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAluInt::computeObservable(
            DVCSObservableKinematic(m_xB, m_t, m_Q2, m_E, x),
            gpdTypesFromVector(params)).getValue() * sin(x);
}

PhysicalType<double> DVCSAluIntSin1Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = gpdTypesToVector(gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
