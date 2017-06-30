#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <utility>

#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/gpd/GPDMMS13.h"
#include "../../../../include/partons/modules/gpd_subtraction_constant/GPDSubtractionConstantMMS13.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

const unsigned int GPDSubtractionConstantMMS13::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDSubtractionConstantMMS13(
                        "GPDSubtractionConstantMMS13"));

GPDSubtractionConstantMMS13::GPDSubtractionConstantMMS13(
        const std::string& className) :
        GPDSubtractionConstantModule(className), MathIntegratorModule() {

    m_pMMS13Model = 0;

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H,
                    &GPDSubtractionConstantModule::computeH));

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::E,
                    &GPDSubtractionConstantModule::computeE));

    initFunctorsForIntegrations();
}

GPDSubtractionConstantMMS13::~GPDSubtractionConstantMMS13() {

    if (m_p_int_dTermIntegral) {
        delete m_p_int_dTermIntegral;
        m_p_int_dTermIntegral = 0;
    }
}

GPDSubtractionConstantMMS13* GPDSubtractionConstantMMS13::clone() const {
    return new GPDSubtractionConstantMMS13(*this);
}

void GPDSubtractionConstantMMS13::resolveObjectDependencies() {

    setIntegrator(NumA::IntegratorType1D::DEXP);

    m_pMMS13Model =
            static_cast<GPDMMS13*>(Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    GPDMMS13::classId));
}

void GPDSubtractionConstantMMS13::initFunctorsForIntegrations() {
    m_p_int_dTermIntegral = NumA::Integrator1D::newIntegrationFunctor(this,
            &GPDSubtractionConstantMMS13::dTermIntegral);
}

GPDSubtractionConstantMMS13::GPDSubtractionConstantMMS13(
        const GPDSubtractionConstantMMS13& other) :
        GPDSubtractionConstantModule(other), MathIntegratorModule(other) {

    m_pMMS13Model = other.m_pMMS13Model;

    initFunctorsForIntegrations();
}

double GPDSubtractionConstantMMS13::computeH() {

    std::vector<double> parameters;
    //TODO H, E, etc nie potrzebne
    //TODO dodac const gdzie sie da w ostatniej pracy
    return integrate(m_p_int_dTermIntegral, -1., 1., parameters);
}

double GPDSubtractionConstantMMS13::computeE() {

    std::vector<double> parameters;
    return -1 * integrate(m_p_int_dTermIntegral, -1., 1., parameters);
}

double GPDSubtractionConstantMMS13::dTermIntegral(double zeta,
        std::vector<double> par) {
    return m_pMMS13Model->DTerm(zeta) / (1. - zeta);
}

void GPDSubtractionConstantMMS13::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    GPDSubtractionConstantModule::prepareSubModules(subModulesData);
}
