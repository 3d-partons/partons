#include "../../../../../../include/partons/modules/observable/TCS/cross_section/TCSCrossSectionUUWeightedThetaIntegrated.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../../../../include/partons/beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string TCSCrossSectionUUWeightedThetaIntegrated::PARAMETER_NAME_THETA_LIMIT =
        "theta_limit";

const unsigned int TCSCrossSectionUUWeightedThetaIntegrated::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSCrossSectionUUWeightedThetaIntegrated(
                        "TCSCrossSectionUUWeightedThetaIntegrated"));

TCSCrossSectionUUWeightedThetaIntegrated::TCSCrossSectionUUWeightedThetaIntegrated(
        const std::string &className) :
        TCSCrossSectionUUWeighted(className), MathIntegratorModule(), m_pFunctionToIntegrateObservableTheta(
                0) {

    m_thetaRange = std::pair<double, double>(Constant::PI / 4.,
            3 * Constant::PI / 4.);

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

TCSCrossSectionUUWeightedThetaIntegrated::TCSCrossSectionUUWeightedThetaIntegrated(
        const TCSCrossSectionUUWeightedThetaIntegrated& other) :
        TCSCrossSectionUUWeighted(other), MathIntegratorModule(other), m_pFunctionToIntegrateObservableTheta(
                0) {

    m_thetaRange = other.m_thetaRange;

    initFunctorsForIntegrations();
}

TCSCrossSectionUUWeightedThetaIntegrated::~TCSCrossSectionUUWeightedThetaIntegrated() {

    if (m_pFunctionToIntegrateObservableTheta) {
        delete m_pFunctionToIntegrateObservableTheta;
        m_pFunctionToIntegrateObservableTheta = 0;
    }
}

void TCSCrossSectionUUWeightedThetaIntegrated::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservableTheta =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &TCSCrossSectionUUWeightedThetaIntegrated::functionToIntegrateObservableTheta);
}

TCSCrossSectionUUWeightedThetaIntegrated* TCSCrossSectionUUWeightedThetaIntegrated::clone() const {
    return new TCSCrossSectionUUWeightedThetaIntegrated(*this);
}

void TCSCrossSectionUUWeightedThetaIntegrated::configure(
        const ElemUtils::Parameters &parameters) {

    TCSCrossSectionUUWeighted::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);

    if (parameters.isAvailable(
            TCSCrossSectionUUWeightedThetaIntegrated::PARAMETER_NAME_THETA_LIMIT)) {

        PhysicalType<double> limit(parameters.getLastAvailable().toDouble(),
                PhysicalUnit::DEG);

        if (limit.getValue() < 0. || limit.getValue() > 180.) {
            warn(__func__,
                    ElemUtils::Formatter() << "Illegal value of parameter "
                            << TCSCrossSectionUUWeightedThetaIntegrated::PARAMETER_NAME_THETA_LIMIT
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
                            << TCSCrossSectionUUWeightedThetaIntegrated::PARAMETER_NAME_THETA_LIMIT
                            << " changed to " << limit.toString());
        }
    }
}

double TCSCrossSectionUUWeightedThetaIntegrated::functionToIntegrateObservableTheta(
        double x, std::vector<double> params) {

    TCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromVector(params, kinematic, gpdType);

    kinematic.setTheta(PhysicalType<double>(x, PhysicalUnit::RAD));

    return TCSCrossSectionUUWeighted::computeObservable(kinematic, gpdType).getValue();
}

PhysicalType<double> TCSCrossSectionUUWeightedThetaIntegrated::computeObservable(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesToVector(
            kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservableTheta, m_thetaRange.first,
                    m_thetaRange.second, params), PhysicalUnit::NB);
}

const std::pair<double, double>& TCSCrossSectionUUWeightedThetaIntegrated::getThetaRange() const {
    return m_thetaRange;
}

void TCSCrossSectionUUWeightedThetaIntegrated::setThetaRange(
        const std::pair<double, double>& thetaRange) {
    m_thetaRange = thetaRange;
}

} /* namespace PARTONS */
