#include "../../../../../../include/partons/modules/observable/TCS/asymmetry/TCSR.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int TCSR::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(new TCSR("TCSR"));

TCSR::TCSR(const std::string &className) :
        TCSCrossSectionUUWeightedThetaIntegrated(className), m_pFunctionToIntegrateObservablePhi1(
                0), m_pFunctionToIntegrateObservablePhi2(0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

TCSR::TCSR(const TCSR& other) :
        TCSCrossSectionUUWeightedThetaIntegrated(other), m_pFunctionToIntegrateObservablePhi1(
                0), m_pFunctionToIntegrateObservablePhi2(0) {

    initFunctorsForIntegrations();
}

TCSR::~TCSR() {

    if (m_pFunctionToIntegrateObservablePhi1) {
        delete m_pFunctionToIntegrateObservablePhi1;
        m_pFunctionToIntegrateObservablePhi1 = 0;
    }

    if (m_pFunctionToIntegrateObservablePhi2) {
        delete m_pFunctionToIntegrateObservablePhi2;
        m_pFunctionToIntegrateObservablePhi2 = 0;
    }
}

void TCSR::initFunctorsForIntegrations() {

    m_pFunctionToIntegrateObservablePhi1 =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &TCSR::functionToIntegrateObservablePhi1);

    m_pFunctionToIntegrateObservablePhi2 =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &TCSR::functionToIntegrateObservablePhi2);
}

TCSR* TCSR::clone() const {
    return new TCSR(*this);
}

double TCSR::functionToIntegrateObservablePhi1(double x,
        std::vector<double> params) {

    TCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return TCSCrossSectionUUWeightedThetaIntegrated::computeObservable(
            kinematic, gpdType).getValue() * cos(x);
}

double TCSR::functionToIntegrateObservablePhi2(double x,
        std::vector<double> params) {

    TCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return TCSCrossSectionUUWeightedThetaIntegrated::computeObservable(
            kinematic, gpdType).getValue();
}

PhysicalType<double> TCSR::computeObservable(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesToVector(
            kinematic, gpdType);

    double A = integrate(m_pFunctionToIntegrateObservablePhi1, 0.,
            2 * Constant::PI, params);

    double B = integrate(m_pFunctionToIntegrateObservablePhi2, 0.,
            2 * Constant::PI, params);

    return PhysicalType<double>(2 * A / B, PhysicalUnit::NONE);
}

} /* namespace PARTONS */
