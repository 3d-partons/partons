#include "../../../../../../include/partons/modules/observable/DVMP/cross_section/DVMPCrossSectionUUUMinusPhiIntegrated.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../../../../include/partons/beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/Observable.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVMPCrossSectionUUUMinusPhiIntegrated::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPCrossSectionUUUMinusPhiIntegrated(
                        "DVMPCrossSectionUUUMinusPhiIntegrated"));

DVMPCrossSectionUUUMinusPhiIntegrated::DVMPCrossSectionUUUMinusPhiIntegrated(
        const std::string &className) :
        DVMPCrossSectionUUUMinus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVMPCrossSectionUUUMinusPhiIntegrated::DVMPCrossSectionUUUMinusPhiIntegrated(
        const DVMPCrossSectionUUUMinusPhiIntegrated& other) :
        DVMPCrossSectionUUUMinus(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVMPCrossSectionUUUMinusPhiIntegrated::~DVMPCrossSectionUUUMinusPhiIntegrated() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVMPCrossSectionUUUMinusPhiIntegrated::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVMPCrossSectionUUUMinusPhiIntegrated::functionToIntegrateObservable);
}

DVMPCrossSectionUUUMinusPhiIntegrated* DVMPCrossSectionUUUMinusPhiIntegrated::clone() const {
    return new DVMPCrossSectionUUUMinusPhiIntegrated(*this);
}

void DVMPCrossSectionUUUMinusPhiIntegrated::configure(
        const ElemUtils::Parameters &parameters) {

    DVMPCrossSectionUUUMinus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVMPCrossSectionUUUMinusPhiIntegrated::functionToIntegrateObservable(
        double x, std::vector<double> params) {

    DVMPObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return DVMPCrossSectionUUUMinus::computeObservable(kinematic, gpdType).getValue();
}

PhysicalType<double> DVMPCrossSectionUUUMinusPhiIntegrated::computeObservable(
        const DVMPObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(
            kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params), PhysicalUnit::NB);
}

} /* namespace PARTONS */
