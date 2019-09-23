#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAluDVCSSin1Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAluDVCSSin1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAluDVCSSin1Phi("DVCSAluDVCSSin1Phi"));

DVCSAluDVCSSin1Phi::DVCSAluDVCSSin1Phi(const std::string &className) :
        DVCSAluDVCS(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAluDVCSSin1Phi::DVCSAluDVCSSin1Phi(const DVCSAluDVCSSin1Phi& other) :
        DVCSAluDVCS(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

DVCSAluDVCSSin1Phi::~DVCSAluDVCSSin1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAluDVCSSin1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAluDVCSSin1Phi::functionToIntegrateObservable);
}

DVCSAluDVCSSin1Phi* DVCSAluDVCSSin1Phi::clone() const {
    return new DVCSAluDVCSSin1Phi(*this);
}

void DVCSAluDVCSSin1Phi::configure(const ElemUtils::Parameters &parameters) {

    DVCSAluDVCS::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAluDVCSSin1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSAluDVCS::computeObservable(
            DVCSObservableKinematic(m_xB, m_t, m_Q2, m_E, x),
            gpdTypesFromVector(params)).getValue() * sin(x);
}

PhysicalType<double> DVCSAluDVCSSin1Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = gpdTypesToVector(gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);

}

} /* namespace PARTONS */
