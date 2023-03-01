#include "../../../../../../include/partons/modules/observable/TCS/asymmetry/TCSAcuThetaIntegrated.h"

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

const std::string TCSAcuThetaIntegrated::PARAMETER_NAME_THETA_LIMIT =
        "theta_limit";

const unsigned int TCSAcuThetaIntegrated::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSAcuThetaIntegrated("TCSAcuThetaIntegrated"));

TCSAcuThetaIntegrated::TCSAcuThetaIntegrated(const std::string &className) :
        TCSObservable(className), MathIntegratorModule(), m_pFunctionToIntegrateObservableTheta1(
                0), m_pFunctionToIntegrateObservableTheta2(0) {

    m_thetaRange = std::pair<double, double>(Constant::PI / 4.,
            3 * Constant::PI / 4.);

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

TCSAcuThetaIntegrated::TCSAcuThetaIntegrated(const TCSAcuThetaIntegrated& other) :
        TCSObservable(other), MathIntegratorModule(other), m_pFunctionToIntegrateObservableTheta1(
                0), m_pFunctionToIntegrateObservableTheta2(0) {

    m_thetaRange = other.m_thetaRange;

    initFunctorsForIntegrations();
}

TCSAcuThetaIntegrated::~TCSAcuThetaIntegrated() {

    if (m_pFunctionToIntegrateObservableTheta1) {
        delete m_pFunctionToIntegrateObservableTheta1;
        m_pFunctionToIntegrateObservableTheta1 = 0;
    }

    if (m_pFunctionToIntegrateObservableTheta2) {
        delete m_pFunctionToIntegrateObservableTheta2;
        m_pFunctionToIntegrateObservableTheta2 = 0;
    }
}

void TCSAcuThetaIntegrated::initFunctorsForIntegrations() {

    m_pFunctionToIntegrateObservableTheta1 =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &TCSAcuThetaIntegrated::functionToIntegrateObservableTheta1);

    m_pFunctionToIntegrateObservableTheta2 =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &TCSAcuThetaIntegrated::functionToIntegrateObservableTheta2);
}

TCSAcuThetaIntegrated* TCSAcuThetaIntegrated::clone() const {
    return new TCSAcuThetaIntegrated(*this);
}

void TCSAcuThetaIntegrated::configure(const ElemUtils::Parameters &parameters) {

    TCSObservable::configure(parameters);
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

double TCSAcuThetaIntegrated::functionToIntegrateObservableTheta1(double x,
        std::vector<double> params) {

    TCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setTheta(PhysicalType<double>(x, PhysicalUnit::RAD));

    return m_pProcessModule->compute(1, NumA::Vector3D(0., 0., 0.), kinematic,
            gpdType).getValue().getValue();
}

double TCSAcuThetaIntegrated::functionToIntegrateObservableTheta2(double x,
        std::vector<double> params) {

    TCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setTheta(PhysicalType<double>(x, PhysicalUnit::RAD));

    return m_pProcessModule->compute(-1, NumA::Vector3D(0., 0., 0.), kinematic,
            gpdType).getValue().getValue();
}

PhysicalType<double> TCSAcuThetaIntegrated::computeObservable(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(
            kinematic, gpdType);

    //evaluate
    double A = integrate(m_pFunctionToIntegrateObservableTheta1,
            m_thetaRange.first, m_thetaRange.second, params);

    double B = integrate(m_pFunctionToIntegrateObservableTheta2,
            m_thetaRange.first, m_thetaRange.second, params);

    //combine
    if (A + B == 0.) {

        warn(__func__, "Asymmetry denominator is zero");
        return PhysicalType<double>(0., PhysicalUnit::NONE);
    }

    //DEBUG
//    return PhysicalType<double>(A-B, PhysicalUnit::GEVm2);
    //END DEBUG

    //return
    return PhysicalType<double>((A - B) / (A + B), PhysicalUnit::NONE);
}

const std::pair<double, double>& TCSAcuThetaIntegrated::getThetaRange() const {
    return m_thetaRange;
}

void TCSAcuThetaIntegrated::setThetaRange(
        const std::pair<double, double>& thetaRange) {
    m_thetaRange = thetaRange;
}

} /* namespace PARTONS */
