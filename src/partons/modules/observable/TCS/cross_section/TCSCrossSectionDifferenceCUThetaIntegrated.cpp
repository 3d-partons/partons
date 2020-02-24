#include "../../../../../../include/partons/modules/observable/TCS/cross_section/TCSCrossSectionDifferenceCUThetaIntegrated.h"

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

const std::string TCSCrossSectionDifferenceCUThetaIntegrated::PARAMETER_NAME_THETA_LIMIT =
        "theta_limit";

const unsigned int TCSCrossSectionDifferenceCUThetaIntegrated::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSCrossSectionDifferenceCUThetaIntegrated(
                        "TCSCrossSectionDifferenceCUThetaIntegrated"));

TCSCrossSectionDifferenceCUThetaIntegrated::TCSCrossSectionDifferenceCUThetaIntegrated(
        const std::string &className) :
        TCSCrossSectionDifferenceCU(className), MathIntegratorModule(), m_pFunctionToIntegrateObservableTheta(
                0) {

    m_thetaRange = std::pair<double, double>(Constant::PI / 4.,
            3 * Constant::PI / 4.);

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

TCSCrossSectionDifferenceCUThetaIntegrated::TCSCrossSectionDifferenceCUThetaIntegrated(
        const TCSCrossSectionDifferenceCUThetaIntegrated& other) :
        TCSCrossSectionDifferenceCU(other), MathIntegratorModule(other), m_pFunctionToIntegrateObservableTheta(
                0) {

    m_thetaRange = other.m_thetaRange;

    initFunctorsForIntegrations();
}

TCSCrossSectionDifferenceCUThetaIntegrated::~TCSCrossSectionDifferenceCUThetaIntegrated() {

    if (m_pFunctionToIntegrateObservableTheta) {
        delete m_pFunctionToIntegrateObservableTheta;
        m_pFunctionToIntegrateObservableTheta = 0;
    }
}

void TCSCrossSectionDifferenceCUThetaIntegrated::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservableTheta =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &TCSCrossSectionDifferenceCUThetaIntegrated::functionToIntegrateObservableTheta);
}

TCSCrossSectionDifferenceCUThetaIntegrated* TCSCrossSectionDifferenceCUThetaIntegrated::clone() const {
    return new TCSCrossSectionDifferenceCUThetaIntegrated(*this);
}

void TCSCrossSectionDifferenceCUThetaIntegrated::configure(
        const ElemUtils::Parameters &parameters) {

    TCSCrossSectionDifferenceCU::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);

    if (parameters.isAvailable(
            TCSCrossSectionDifferenceCUThetaIntegrated::PARAMETER_NAME_THETA_LIMIT)) {

        PhysicalType<double> limit(parameters.getLastAvailable().toDouble(),
                PhysicalUnit::DEG);

        if (limit.getValue() < 0. || limit.getValue() > 180.) {
            warn(__func__,
                    ElemUtils::Formatter() << "Illegal value of parameter "
                            << TCSCrossSectionDifferenceCUThetaIntegrated::PARAMETER_NAME_THETA_LIMIT
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
                            << TCSCrossSectionDifferenceCUThetaIntegrated::PARAMETER_NAME_THETA_LIMIT
                            << " changed to " << limit.toString());
        }
    }
}

double TCSCrossSectionDifferenceCUThetaIntegrated::functionToIntegrateObservableTheta(
        double x, std::vector<double> params) {

    TCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setTheta(PhysicalType<double>(x, PhysicalUnit::RAD));

    return TCSCrossSectionDifferenceCU::computeObservable(kinematic, gpdType).getValue();
}

PhysicalType<double> TCSCrossSectionDifferenceCUThetaIntegrated::computeObservable(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(
            kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservableTheta, m_thetaRange.first,
                    m_thetaRange.second, params), PhysicalUnit::NB);
}

const std::pair<double, double>& TCSCrossSectionDifferenceCUThetaIntegrated::getThetaRange() const {
    return m_thetaRange;
}

void TCSCrossSectionDifferenceCUThetaIntegrated::setThetaRange(
        const std::pair<double, double>& thetaRange) {
    m_thetaRange = thetaRange;
}

} /* namespace PARTONS */
