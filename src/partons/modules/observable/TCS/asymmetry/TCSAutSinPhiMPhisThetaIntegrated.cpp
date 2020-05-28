#include "../../../../../../include/partons/modules/observable/TCS/asymmetry/TCSAutSinPhiMPhisThetaIntegrated.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/Observable.h"
#include "../../../../../../include/partons/modules/process/TCS/TCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string TCSAutSinPhiMPhisThetaIntegrated::PARAMETER_NAME_THETA_LIMIT =
        "theta_limit";

const unsigned int TCSAutSinPhiMPhisThetaIntegrated::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSAutSinPhiMPhisThetaIntegrated(
                        "TCSAutSinPhiMPhisThetaIntegrated"));

TCSAutSinPhiMPhisThetaIntegrated::TCSAutSinPhiMPhisThetaIntegrated(
        const std::string &className) :
        TCSObservable(className), MathIntegratorModule(), m_pFunctionToIntegrateObservableTheta1(
                0), m_pFunctionToIntegrateObservableTheta2(0) {

    m_thetaRange = std::pair<double, double>(Constant::PI / 4.,
            3 * Constant::PI / 4.);

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

TCSAutSinPhiMPhisThetaIntegrated::TCSAutSinPhiMPhisThetaIntegrated(
        const TCSAutSinPhiMPhisThetaIntegrated& other) :
        TCSObservable(other), MathIntegratorModule(other), m_pFunctionToIntegrateObservableTheta1(
                0), m_pFunctionToIntegrateObservableTheta2(0) {

    m_thetaRange = other.m_thetaRange;

    initFunctorsForIntegrations();
}

TCSAutSinPhiMPhisThetaIntegrated::~TCSAutSinPhiMPhisThetaIntegrated() {

    if (m_pFunctionToIntegrateObservableTheta1) {
        delete m_pFunctionToIntegrateObservableTheta1;
        m_pFunctionToIntegrateObservableTheta1 = 0;
    }

    if (m_pFunctionToIntegrateObservableTheta2) {
        delete m_pFunctionToIntegrateObservableTheta2;
        m_pFunctionToIntegrateObservableTheta2 = 0;
    }

    if (m_pFunctionToIntegrateObservableTheta3) {
        delete m_pFunctionToIntegrateObservableTheta3;
        m_pFunctionToIntegrateObservableTheta3 = 0;
    }

    if (m_pFunctionToIntegrateObservableTheta4) {
        delete m_pFunctionToIntegrateObservableTheta4;
        m_pFunctionToIntegrateObservableTheta4 = 0;
    }
}

void TCSAutSinPhiMPhisThetaIntegrated::initFunctorsForIntegrations() {

    m_pFunctionToIntegrateObservableTheta1 =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &TCSAutSinPhiMPhisThetaIntegrated::functionToIntegrateObservableTheta1);

    m_pFunctionToIntegrateObservableTheta2 =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &TCSAutSinPhiMPhisThetaIntegrated::functionToIntegrateObservableTheta2);

    m_pFunctionToIntegrateObservableTheta3 =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &TCSAutSinPhiMPhisThetaIntegrated::functionToIntegrateObservableTheta3);

    m_pFunctionToIntegrateObservableTheta4 =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &TCSAutSinPhiMPhisThetaIntegrated::functionToIntegrateObservableTheta4);
}

TCSAutSinPhiMPhisThetaIntegrated* TCSAutSinPhiMPhisThetaIntegrated::clone() const {
    return new TCSAutSinPhiMPhisThetaIntegrated(*this);
}

void TCSAutSinPhiMPhisThetaIntegrated::configure(
        const ElemUtils::Parameters &parameters) {

    TCSObservable::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);

    if (parameters.isAvailable(
            TCSAutSinPhiMPhisThetaIntegrated::PARAMETER_NAME_THETA_LIMIT)) {

        PhysicalType<double> limit(parameters.getLastAvailable().toDouble(),
                PhysicalUnit::DEG);

        if (limit.getValue() < 0. || limit.getValue() > 180.) {
            warn(__func__,
                    ElemUtils::Formatter() << "Illegal value of parameter "
                            << TCSAutSinPhiMPhisThetaIntegrated::PARAMETER_NAME_THETA_LIMIT
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
                            << TCSAutSinPhiMPhisThetaIntegrated::PARAMETER_NAME_THETA_LIMIT
                            << " changed to " << limit.toString());
        }
    }
}

double TCSAutSinPhiMPhisThetaIntegrated::functionToIntegrateObservableTheta1(
        double x, std::vector<double> params) {

    TCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setTheta(PhysicalType<double>(x, PhysicalUnit::RAD));

    return m_pProcessModule->compute(1, NumA::Vector3D(0., 1., 0.), kinematic,
            gpdType).getValue().getValue();
}

double TCSAutSinPhiMPhisThetaIntegrated::functionToIntegrateObservableTheta2(
        double x, std::vector<double> params) {

    TCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setTheta(PhysicalType<double>(x, PhysicalUnit::RAD));

    return m_pProcessModule->compute(-1, NumA::Vector3D(0., 1., 0.), kinematic,
            gpdType).getValue().getValue();
}

double TCSAutSinPhiMPhisThetaIntegrated::functionToIntegrateObservableTheta3(
        double x, std::vector<double> params) {

    TCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setTheta(PhysicalType<double>(x, PhysicalUnit::RAD));

    return m_pProcessModule->compute(1, NumA::Vector3D(0., -1., 0.), kinematic,
            gpdType).getValue().getValue();
}

double TCSAutSinPhiMPhisThetaIntegrated::functionToIntegrateObservableTheta4(
        double x, std::vector<double> params) {

    TCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setTheta(PhysicalType<double>(x, PhysicalUnit::RAD));

    return m_pProcessModule->compute(-1, NumA::Vector3D(0., -1., 0.), kinematic,
            gpdType).getValue().getValue();
}

PhysicalType<double> TCSAutSinPhiMPhisThetaIntegrated::computeObservable(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(
            kinematic, gpdType);

    //evaluate
    double A = integrate(m_pFunctionToIntegrateObservableTheta1,
            m_thetaRange.first, m_thetaRange.second, params);

    double B = integrate(m_pFunctionToIntegrateObservableTheta2,
            m_thetaRange.first, m_thetaRange.second, params);

    double C = integrate(m_pFunctionToIntegrateObservableTheta3,
            m_thetaRange.first, m_thetaRange.second, params);

    double D = integrate(m_pFunctionToIntegrateObservableTheta4,
            m_thetaRange.first, m_thetaRange.second, params);

    //combine
    if (A + B + C + D == 0.) {

        warn(__func__, "Asymmetry denominator is zero");
        return PhysicalType<double>(0., PhysicalUnit::NONE);
    }

    //return
    return PhysicalType<double>(((A + B) - (C + D)) / (A + B + C + D),
            PhysicalUnit::NONE);
}

const std::pair<double, double>& TCSAutSinPhiMPhisThetaIntegrated::getThetaRange() const {
    return m_thetaRange;
}

void TCSAutSinPhiMPhisThetaIntegrated::setThetaRange(
        const std::pair<double, double>& thetaRange) {
    m_thetaRange = thetaRange;
}

} /* namespace PARTONS */
