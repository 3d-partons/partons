#include "../../../../../../include/partons/modules/observable/DVMP/cross_section/DVMPCrossSectionUUUMinusTIntegrated.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <NumA/integration/one_dimension/GaussLegendreIntegrator1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/Observable.h"
#include "../../../../../../include/partons/modules/process/DVMP/DVMPProcessModule.h"
#include "../../../../../../include/partons/modules/xi_converter/DVMP/DVMPXiConverterModule.h"
#include "../../../../../../include/partons/modules/xi_converter/XiConverterModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVMPCrossSectionUUUMinusTIntegrated::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPCrossSectionUUUMinusTIntegrated(
                        "DVMPCrossSectionUUUMinusTIntegrated"));

DVMPCrossSectionUUUMinusTIntegrated::DVMPCrossSectionUUUMinusTIntegrated(
        const std::string &className) :
        DVMPCrossSectionUUUMinus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0), m_tLimit(-1.) {

    setIntegrator(NumA::IntegratorType1D::GL);
    static_cast<NumA::GaussLegendreIntegrator1D*>(getMathIntegrator())->setN(5);
    initFunctorsForIntegrations();
}

DVMPCrossSectionUUUMinusTIntegrated::DVMPCrossSectionUUUMinusTIntegrated(
        const DVMPCrossSectionUUUMinusTIntegrated& other) :
        DVMPCrossSectionUUUMinus(other), MathIntegratorModule(other), m_pFunctionToIntegrateObservable(
                0), m_tLimit(
                other.m_tLimit) {

    static_cast<NumA::GaussLegendreIntegrator1D*>(getMathIntegrator())->setN(5);
    initFunctorsForIntegrations();
}

DVMPCrossSectionUUUMinusTIntegrated::~DVMPCrossSectionUUUMinusTIntegrated() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVMPCrossSectionUUUMinusTIntegrated::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVMPCrossSectionUUUMinusTIntegrated::functionToIntegrateObservable);
}

DVMPCrossSectionUUUMinusTIntegrated* DVMPCrossSectionUUUMinusTIntegrated::clone() const {
    return new DVMPCrossSectionUUUMinusTIntegrated(*this);
}

void DVMPCrossSectionUUUMinusTIntegrated::configure(
        const ElemUtils::Parameters &parameters) {

    DVMPCrossSectionUUUMinus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVMPCrossSectionUUUMinusTIntegrated::functionToIntegrateObservable(
        double x, std::vector<double> params) {

    DVMPObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setT(PhysicalType<double>(x, PhysicalUnit::GEV2));

    double value = DVMPCrossSectionUUUMinus::computeObservable(kinematic, gpdType).getValue();

    if(std::isnan(value)){
        warn(__func__, "value is nan, return 1.E8 instead");
        return 1.E8;
    }

    return value;
}

PhysicalType<double> DVMPCrossSectionUUUMinusTIntegrated::computeObservable(
        const DVMPObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(
            kinematic, gpdType);

    double xi = m_pProcessModule->getXiConverterModule()->compute(
            DVMPObservableKinematic(kinematic.getXB(), kinematic.getT(),
                    kinematic.getQ2(), kinematic.getE(), kinematic.getPhi(),
                    kinematic.getMesonType())).getValue();

    double tMin = -4 * pow(xi, 2)
            / (1. - pow(xi, 2) * pow(Constant::PROTON_MASS, 2));

    if (tMin < m_tLimit) {
        warn(__func__,
                ElemUtils::Formatter()
                        << "tMin smaller than integration limit");

        return PhysicalType<double>(0., PhysicalUnit::NB);
    }

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, m_tLimit, tMin, params),
            PhysicalUnit::NB);
}

} /* namespace PARTONS */
