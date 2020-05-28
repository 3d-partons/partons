#include "../../../../../../include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionUUMinusPhiIntegrated.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/Observable.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSCrossSectionUUMinusPhiIntegrated::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCrossSectionUUMinusPhiIntegrated(
                        "DVCSCrossSectionUUMinusPhiIntegrated"));

DVCSCrossSectionUUMinusPhiIntegrated::DVCSCrossSectionUUMinusPhiIntegrated(
        const std::string &className) :
        DVCSCrossSectionUUMinus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSCrossSectionUUMinusPhiIntegrated::DVCSCrossSectionUUMinusPhiIntegrated(
        const DVCSCrossSectionUUMinusPhiIntegrated& other) :
        DVCSCrossSectionUUMinus(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSCrossSectionUUMinusPhiIntegrated::~DVCSCrossSectionUUMinusPhiIntegrated() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSCrossSectionUUMinusPhiIntegrated::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSCrossSectionUUMinusPhiIntegrated::functionToIntegrateObservable);
}

DVCSCrossSectionUUMinusPhiIntegrated* DVCSCrossSectionUUMinusPhiIntegrated::clone() const {
    return new DVCSCrossSectionUUMinusPhiIntegrated(*this);
}

void DVCSCrossSectionUUMinusPhiIntegrated::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSCrossSectionUUMinus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSCrossSectionUUMinusPhiIntegrated::functionToIntegrateObservable(
        double x, std::vector<double> params) {

    DVCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return DVCSCrossSectionUUMinus::computeObservable(kinematic, gpdType).getValue();
}

PhysicalType<double> DVCSCrossSectionUUMinusPhiIntegrated::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(
            kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params), PhysicalUnit::NB);
}

} /* namespace PARTONS */
