#include "../../../../../../include/partons/modules/observable/TCS/cross_section/TCSCrossSectionUUThetaPhiIntegrated.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>

#include "../../../../../../include/partons/beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int TCSCrossSectionUUThetaPhiIntegrated::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSCrossSectionUUThetaPhiIntegrated(
                        "TCSCrossSectionUUThetaPhiIntegrated"));

TCSCrossSectionUUThetaPhiIntegrated::TCSCrossSectionUUThetaPhiIntegrated(
        const std::string &className) :
        TCSCrossSectionUUThetaIntegrated(className), m_pFunctionToIntegrateObservablePhi(
                0) {
    initFunctorsForIntegrations();
}

TCSCrossSectionUUThetaPhiIntegrated::TCSCrossSectionUUThetaPhiIntegrated(
        const TCSCrossSectionUUThetaPhiIntegrated& other) :
        TCSCrossSectionUUThetaIntegrated(other), m_pFunctionToIntegrateObservablePhi(
                0) {
    initFunctorsForIntegrations();
}

TCSCrossSectionUUThetaPhiIntegrated::~TCSCrossSectionUUThetaPhiIntegrated() {

    if (m_pFunctionToIntegrateObservablePhi) {
        delete m_pFunctionToIntegrateObservablePhi;
        m_pFunctionToIntegrateObservablePhi = 0;
    }
}

void TCSCrossSectionUUThetaPhiIntegrated::initFunctorsForIntegrations() {

    TCSCrossSectionUUThetaIntegrated::initFunctorsForIntegrations();

    m_pFunctionToIntegrateObservablePhi =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &TCSCrossSectionUUThetaPhiIntegrated::functionToIntegrateObservablePhi);
}

TCSCrossSectionUUThetaPhiIntegrated* TCSCrossSectionUUThetaPhiIntegrated::clone() const {
    return new TCSCrossSectionUUThetaPhiIntegrated(*this);
}

double TCSCrossSectionUUThetaPhiIntegrated::functionToIntegrateObservablePhi(
        double x, std::vector<double> params) {

    TCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return TCSCrossSectionUUThetaIntegrated::computeObservable(kinematic,
            gpdType).getValue();
}

PhysicalType<double> TCSCrossSectionUUThetaPhiIntegrated::computeObservable(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesToVector(
            kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservablePhi, 0., 2 * Constant::PI,
                    params), PhysicalUnit::NB);
}

} /* namespace PARTONS */
