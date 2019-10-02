#include "../../../../../../include/partons/modules/observable/TCS/asymmetry/TCSAcuThetaIntegrated.h"

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

const std::string TCSAcuThetaIntegrated::PARAMETER_NAME_THETA_LIMIT =
        "theta_limit";

const unsigned int TCSAcuThetaIntegrated::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSAcuThetaIntegrated("TCSAcuThetaIntegrated"));

TCSAcuThetaIntegrated::TCSAcuThetaIntegrated(const std::string &className) :
        TCSAcu(className), MathIntegratorModule(), m_pFunctionToIntegrateObservableTheta(
                0) {

    m_thetaRange = std::pair<double, double>(Constant::PI / 4.,
            3 * Constant::PI / 4.);

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

TCSAcuThetaIntegrated::TCSAcuThetaIntegrated(const TCSAcuThetaIntegrated& other) :
        TCSAcu(other), MathIntegratorModule(other), m_pFunctionToIntegrateObservableTheta(
                0) {

    m_thetaRange = other.m_thetaRange;

    initFunctorsForIntegrations();
}

TCSAcuThetaIntegrated::~TCSAcuThetaIntegrated() {

    if (m_pFunctionToIntegrateObservableTheta) {
        delete m_pFunctionToIntegrateObservableTheta;
        m_pFunctionToIntegrateObservableTheta = 0;
    }
}

void TCSAcuThetaIntegrated::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservableTheta =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &TCSAcuThetaIntegrated::functionToIntegrateObservableTheta);
}

TCSAcuThetaIntegrated* TCSAcuThetaIntegrated::clone() const {
    return new TCSAcuThetaIntegrated(*this);
}

void TCSAcuThetaIntegrated::configure(const ElemUtils::Parameters &parameters) {

    TCSAcu::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);

    if (parameters.isAvailable(
            TCSAcuThetaIntegrated::PARAMETER_NAME_THETA_LIMIT)) {

        PhysicalType<double> limit(parameters.getLastAvailable().toDouble(),
                PhysicalUnit::DEG);

        if (limit.getValue() < 0. || limit.getValue() > 180.) {
            warn(__func__,
                    ElemUtils::Formatter() << "Illegal value of parameter "
                            << TCSAcuThetaIntegrated::PARAMETER_NAME_THETA_LIMIT
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
                            << TCSAcuThetaIntegrated::PARAMETER_NAME_THETA_LIMIT
                            << " changed to " << limit.toString());
        }
    }
}

double TCSAcuThetaIntegrated::functionToIntegrateObservableTheta(double x,
        std::vector<double> params) {

    TCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromVector(params, kinematic, gpdType);

    kinematic.setTheta(PhysicalType<double>(x, PhysicalUnit::RAD));

    return TCSAcu::computeObservable(kinematic, gpdType).getValue();
}

PhysicalType<double> TCSAcuThetaIntegrated::computeObservable(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesToVector(
            kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservableTheta, m_thetaRange.first,
                    m_thetaRange.second, params), PhysicalUnit::NONE);
}

const std::pair<double, double>& TCSAcuThetaIntegrated::getThetaRange() const {
    return m_thetaRange;
}

void TCSAcuThetaIntegrated::setThetaRange(
        const std::pair<double, double>& thetaRange) {
    m_thetaRange = thetaRange;
}

} /* namespace PARTONS */
