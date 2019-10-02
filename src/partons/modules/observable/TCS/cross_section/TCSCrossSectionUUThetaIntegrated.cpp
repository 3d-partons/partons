#include "../../../../../../include/partons/modules/observable/TCS/cross_section/TCSCrossSectionUUThetaIntegrated.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../../../../include/partons/beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/Observable.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string TCSCrossSectionUUThetaIntegrated::PARAMETER_NAME_THETA_LIMIT =
        "theta_limit";

const unsigned int TCSCrossSectionUUThetaIntegrated::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSCrossSectionUUThetaIntegrated(
                        "TCSCrossSectionUUThetaIntegrated"));

TCSCrossSectionUUThetaIntegrated::TCSCrossSectionUUThetaIntegrated(
        const std::string &className) :
        TCSCrossSectionUU(className), MathIntegratorModule(), m_pFunctionToIntegrateObservableTheta(
                0) {

    m_thetaRange = std::pair<double, double>(Constant::PI / 4.,
            3 * Constant::PI / 4.);

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

TCSCrossSectionUUThetaIntegrated::TCSCrossSectionUUThetaIntegrated(
        const TCSCrossSectionUUThetaIntegrated& other) :
        TCSCrossSectionUU(other), MathIntegratorModule(other), m_pFunctionToIntegrateObservableTheta(
                0) {

    m_thetaRange = other.m_thetaRange;

    initFunctorsForIntegrations();
}

TCSCrossSectionUUThetaIntegrated::~TCSCrossSectionUUThetaIntegrated() {

    if (m_pFunctionToIntegrateObservableTheta) {
        delete m_pFunctionToIntegrateObservableTheta;
        m_pFunctionToIntegrateObservableTheta = 0;
    }
}

void TCSCrossSectionUUThetaIntegrated::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservableTheta =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &TCSCrossSectionUUThetaIntegrated::functionToIntegrateObservableTheta);
}

TCSCrossSectionUUThetaIntegrated* TCSCrossSectionUUThetaIntegrated::clone() const {
    return new TCSCrossSectionUUThetaIntegrated(*this);
}

void TCSCrossSectionUUThetaIntegrated::configure(
        const ElemUtils::Parameters &parameters) {

    TCSCrossSectionUU::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);

    if (parameters.isAvailable(
            TCSCrossSectionUUThetaIntegrated::PARAMETER_NAME_THETA_LIMIT)) {

        PhysicalType<double> limit(parameters.getLastAvailable().toDouble(),
                PhysicalUnit::DEG);

        if (limit.getValue() < 0. || limit.getValue() > 180.) {
            warn(__func__,
                    ElemUtils::Formatter() << "Illegal value of parameter "
                            << TCSCrossSectionUUThetaIntegrated::PARAMETER_NAME_THETA_LIMIT
                            << ", " << limit.toString() << ", ignored");
        } else {
            m_thetaRange =
                    std::pair<double, double>(
                            Constant::PI / 2.
                                    - limit.makeSameUnitAs(PhysicalUnit::RAD).getValue(),
                            Constant::PI / 2.
                                    + limit.makeSameUnitAs(PhysicalUnit::RAD).getValue());

            info(__func__,
                    ElemUtils::Formatter() << "Parameter "
                            << TCSCrossSectionUUThetaIntegrated::PARAMETER_NAME_THETA_LIMIT
                            << " changed to " << limit.toString());
        }
    }
}

double TCSCrossSectionUUThetaIntegrated::functionToIntegrateObservableTheta(
        double x, std::vector<double> params) {
    return TCSCrossSectionUU::computeObservable(
            TCSObservableKinematic(m_t, m_Q2Prim, m_E, m_phi, x),
            gpdTypesFromVector(params)).getValue();
}

PhysicalType<double> TCSCrossSectionUUThetaIntegrated::computeObservable(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    std::vector<double> params = gpdTypesToVector(gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservableTheta, m_thetaRange.first,
                    m_thetaRange.second, params), PhysicalUnit::NB);
}

const std::pair<double, double>& TCSCrossSectionUUThetaIntegrated::getThetaRange() const {
    return m_thetaRange;
}

void TCSCrossSectionUUThetaIntegrated::setThetaRange(
        const std::pair<double, double>& thetaRange) {
    m_thetaRange = thetaRange;
}

} /* namespace PARTONS */
