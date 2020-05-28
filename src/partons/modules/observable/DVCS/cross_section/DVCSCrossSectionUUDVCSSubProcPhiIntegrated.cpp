#include "../../../../../../include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionUUDVCSSubProcPhiIntegrated.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/Observable.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSCrossSectionUUDVCSSubProcPhiIntegrated::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCrossSectionUUDVCSSubProcPhiIntegrated(
                        "DVCSCrossSectionUUDVCSSubProcPhiIntegrated"));

DVCSCrossSectionUUDVCSSubProcPhiIntegrated::DVCSCrossSectionUUDVCSSubProcPhiIntegrated(
        const std::string &className) :
        DVCSCrossSectionUUDVCSSubProc(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSCrossSectionUUDVCSSubProcPhiIntegrated::DVCSCrossSectionUUDVCSSubProcPhiIntegrated(
        const DVCSCrossSectionUUDVCSSubProcPhiIntegrated& other) :
        DVCSCrossSectionUUDVCSSubProc(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSCrossSectionUUDVCSSubProcPhiIntegrated::~DVCSCrossSectionUUDVCSSubProcPhiIntegrated() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSCrossSectionUUDVCSSubProcPhiIntegrated::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSCrossSectionUUDVCSSubProcPhiIntegrated::functionToIntegrateObservable);
}

DVCSCrossSectionUUDVCSSubProcPhiIntegrated* DVCSCrossSectionUUDVCSSubProcPhiIntegrated::clone() const {
    return new DVCSCrossSectionUUDVCSSubProcPhiIntegrated(*this);
}

void DVCSCrossSectionUUDVCSSubProcPhiIntegrated::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSCrossSectionUUDVCSSubProc::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSCrossSectionUUDVCSSubProcPhiIntegrated::functionToIntegrateObservable(
        double x, std::vector<double> params) {

    DVCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return DVCSCrossSectionUUDVCSSubProc::computeObservable(kinematic, gpdType).getValue();
}

PhysicalType<double> DVCSCrossSectionUUDVCSSubProcPhiIntegrated::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(
            kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params), PhysicalUnit::NB);
}

} /* namespace PARTONS */
