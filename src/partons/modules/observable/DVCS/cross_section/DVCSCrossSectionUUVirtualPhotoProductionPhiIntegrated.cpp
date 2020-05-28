#include "../../../../../../include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/Observable.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated(
                        "DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated"));

DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated::DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated(
        const std::string &className) :
        DVCSCrossSectionUUVirtualPhotoProduction(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated::DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated(
        const DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated& other) :
        DVCSCrossSectionUUVirtualPhotoProduction(other), MathIntegratorModule(
                other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated::~DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated::functionToIntegrateObservable);
}

DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated* DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated::clone() const {
    return new DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated(*this);
}

void DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSCrossSectionUUVirtualPhotoProduction::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated::functionToIntegrateObservable(
        double x, std::vector<double> params) {

    DVCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return DVCSCrossSectionUUVirtualPhotoProduction::computeObservable(
            kinematic, gpdType).getValue();
}

PhysicalType<double> DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(
            kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params), PhysicalUnit::NB);
}

} /* namespace PARTONS */
