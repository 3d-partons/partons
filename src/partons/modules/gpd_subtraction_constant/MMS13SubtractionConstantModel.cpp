#include "../../../../include/partons/modules/gpd_subtraction_constant/MMS13SubtractionConstantModel.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <utility>

#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/gpd/MMS13Model.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

const unsigned int MMS13SubtractionConstantModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new MMS13SubtractionConstantModel(
                        "MMS13SubtractionConstantModel"));

MMS13SubtractionConstantModel::MMS13SubtractionConstantModel(
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

MMS13SubtractionConstantModel::~MMS13SubtractionConstantModel() {

    if (m_p_int_dTermIntegral) {
        delete m_p_int_dTermIntegral;
        m_p_int_dTermIntegral = 0;
    }
}

MMS13SubtractionConstantModel* MMS13SubtractionConstantModel::clone() const {
    return new MMS13SubtractionConstantModel(*this);
}

void MMS13SubtractionConstantModel::resolveObjectDependencies() {

    setIntegrator(NumA::IntegratorType1D::DEXP);

    m_pMMS13Model =
            static_cast<MMS13Model*>(Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    MMS13Model::classId));
}

void MMS13SubtractionConstantModel::initFunctorsForIntegrations() {
    m_p_int_dTermIntegral = NumA::Integrator1D::newIntegrationFunctor(this,
            &MMS13SubtractionConstantModel::dTermIntegral);
}

MMS13SubtractionConstantModel::MMS13SubtractionConstantModel(
        const MMS13SubtractionConstantModel& other) :
        GPDSubtractionConstantModule(other), MathIntegratorModule(other) {

    m_pMMS13Model = other.m_pMMS13Model;

    initFunctorsForIntegrations();
}

double MMS13SubtractionConstantModel::computeH() {

    std::vector<double> parameters;
    //TODO H, E, etc nie potrzebne
    //TODO dodac const gdzie sie da w ostatniej pracy
    return integrate(m_p_int_dTermIntegral, -1., 1., parameters);
}

double MMS13SubtractionConstantModel::computeE() {

    std::vector<double> parameters;
    return -1 * integrate(m_p_int_dTermIntegral, -1., 1., parameters);
}

double MMS13SubtractionConstantModel::dTermIntegral(double zeta,
        std::vector<double> par) {
    return m_pMMS13Model->DTerm(zeta) / (1. - zeta);
}

void MMS13SubtractionConstantModel::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "TODO : implement");
}
