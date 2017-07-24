#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSCFFDispersionRelation.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <utility>

#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/active_flavors_thresholds/ActiveFlavorsThresholdsQuarkMasses.h"
#include "../../../../../include/partons/modules/gpd/GPDModule.h"
#include "../../../../../include/partons/modules/gpd_subtraction_constant/GPDSubtractionConstantModule.h"
#include "../../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongStandard.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"

namespace PARTONS {


const unsigned int DVCSCFFDispersionRelation::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFDispersionRelation(
                        "DVCSCFFDispersionRelation"));

DVCSCFFDispersionRelation::DVCSCFFDispersionRelation(
        const std::string& className) :
        DVCSConvolCoeffFunctionModule(className) {

    //relate GPDs to functions
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &DVCSConvolCoeffFunctionModule::computeUnpolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::E,
                    &DVCSConvolCoeffFunctionModule::computeUnpolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Ht,
                    &DVCSConvolCoeffFunctionModule::computePolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Et,
                    &DVCSConvolCoeffFunctionModule::computePolarized));

    //initialize
    m_pSubtractionConstantModule = 0;

    //initialize
    initFunctorsForIntegrations();
}

DVCSCFFDispersionRelation* DVCSCFFDispersionRelation::clone() const {
    return new DVCSCFFDispersionRelation(*this);
}

DVCSCFFDispersionRelation::~DVCSCFFDispersionRelation() {

    setSubtractionConstantModule(0);

    if (m_p_int_dispersionRelationIntegralPartDiagonalA) {
        delete m_p_int_dispersionRelationIntegralPartDiagonalA;
        m_p_int_dispersionRelationIntegralPartDiagonalA = 0;
    }

    if (m_p_int_dispersionRelationIntegralPartDiagonalB) {
        delete m_p_int_dispersionRelationIntegralPartDiagonalB;
        m_p_int_dispersionRelationIntegralPartDiagonalB = 0;
    }
}

void DVCSCFFDispersionRelation::initFunctorsForIntegrations() {

    m_p_int_dispersionRelationIntegralPartDiagonalA =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSCFFDispersionRelation::dispersionRelationIntegralPartDiagonalA);

    m_p_int_dispersionRelationIntegralPartDiagonalB =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSCFFDispersionRelation::dispersionRelationIntegralPartDiagonalB);
}

//TODO init in mother class ? ; propagate init to mother class ?
void DVCSCFFDispersionRelation::resolveObjectDependencies() {

    setIntegrator(NumA::IntegratorType1D::DEXP);

    //TODO not used!
    m_pRunningAlphaStrongModule =
            Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                    RunningAlphaStrongStandard::classId);

    m_pNfConvolCoeffFunction =
            Partons::getInstance()->getModuleObjectFactory()->newActiveFlavorsThresholdsModule(
                    ActiveFlavorsThresholdsQuarkMasses::classId);
}

void DVCSCFFDispersionRelation::configure(
        const ElemUtils::Parameters &parameters) {
    DVCSConvolCoeffFunctionModule::configure(parameters);
}

void DVCSCFFDispersionRelation::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    //mother
    DVCSConvolCoeffFunctionModule::prepareSubModules(subModulesData);

    //search
    std::map<std::string, BaseObjectData>::const_iterator it;

    //GPD subtraction constant module
    it =
            subModulesData.find(
                    GPDSubtractionConstantModule::GPD_SUBTRACTION_CONSTANT_MODULE_CLASS_NAME);

    //check if there
    if (it != subModulesData.end()) {

        if (m_pSubtractionConstantModule != 0) {
            setSubtractionConstantModule(0);
        }

        if (m_pSubtractionConstantModule == 0) {
            m_pSubtractionConstantModule =
                    Partons::getInstance()->getModuleObjectFactory()->newGPDSubtractionConstantModule(
                            (it->second).getModuleClassName());

            info(__func__,
                    ElemUtils::Formatter()
                            << "Configured with GPDSubtractionConstantModule = "
                            << m_pSubtractionConstantModule->getClassName());

            m_pSubtractionConstantModule->configure(
                    (it->second).getParameters());
            m_pSubtractionConstantModule->prepareSubModules(
                    (it->second).getSubModules());
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << getClassName()
                            << " is GPDSubtractionConstantModule dependent and you have not provided one");
        }
    }
}

DVCSCFFDispersionRelation::DVCSCFFDispersionRelation(
        const DVCSCFFDispersionRelation& other) :
        DVCSConvolCoeffFunctionModule(other) {

    //copy
    if (other.m_pSubtractionConstantModule) {
        m_pSubtractionConstantModule =
                other.m_pSubtractionConstantModule->clone();
    } else {
        m_pSubtractionConstantModule = 0;
    }

    //initialize
    initFunctorsForIntegrations();
}

void DVCSCFFDispersionRelation::initModule() {
    DVCSConvolCoeffFunctionModule::initModule();
}

void DVCSCFFDispersionRelation::isModuleWellConfigured() {

    if (!m_pSubtractionConstantModule) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "SubtractionConstantModule not set");
    }

    DVCSConvolCoeffFunctionModule::isModuleWellConfigured();
}

std::complex<double> DVCSCFFDispersionRelation::computeUnpolarized() {

    //check pQCD order
    if (m_qcdOrderType != PerturbativeQCDOrderType::LO) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Calculation not supported for pQCD order = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }

    //check if null
    if (m_pSubtractionConstantModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "SubtractionConstantModule not set");
    }

    //real part
    double Re = 0.;
    std::vector<double> parameters;

    Re += integrate(m_p_int_dispersionRelationIntegralPartDiagonalA, 0., m_xi,
            parameters);
    Re += integrate(m_p_int_dispersionRelationIntegralPartDiagonalA, m_xi, 1.,
            parameters);
    Re -= integrate(m_p_int_dispersionRelationIntegralPartDiagonalB, 0., 1.,
            parameters);

    Re += computeSquareChargeAveragedGPD(m_xi) * (log(m_xi) - log(1. - m_xi));

    //subtraction constant
    double Sub = m_pSubtractionConstantModule->compute(m_xi, m_t, m_MuF2,
            m_MuR2, m_currentGPDComputeType);

    //imaginary part
    double Im = Constant::PI * computeSquareChargeAveragedGPD(m_xi);

    //return
    return std::complex<double>(Re - Sub, Im);
}

std::complex<double> DVCSCFFDispersionRelation::computePolarized() {

    //check pQCD order
    if (m_qcdOrderType != PerturbativeQCDOrderType::LO) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Calculation not supported for pQCD order = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }

    //real part
    double Re = 0.;

    std::vector<double> parameters;

    Re += integrate(m_p_int_dispersionRelationIntegralPartDiagonalA, 0., m_xi,
            parameters);
    Re += integrate(m_p_int_dispersionRelationIntegralPartDiagonalA, m_xi, 1.,
            parameters);
    Re += integrate(m_p_int_dispersionRelationIntegralPartDiagonalB, 0., 1.,
            parameters);

    Re += computeSquareChargeAveragedGPD(m_xi) * (log(m_xi) - log(1. - m_xi));

    //imaginary part
    double Im = Constant::PI * computeSquareChargeAveragedGPD(m_xi);

    //return
    return std::complex<double>(Re, Im);
}

double DVCSCFFDispersionRelation::computeSquareChargeAveragedGPD(
        double xi) {

    //get GPD result
    PartonDistribution partonDistribution = m_pGPDModule->compute(xi, xi, m_t,
            m_MuF2, m_MuR2, m_currentGPDComputeType);

    //return singlet combination
    return partonDistribution.getQuarkDistribution(QuarkFlavor::UP).getQuarkDistributionPlus()
            * Constant::U2_ELEC_CHARGE
            + partonDistribution.getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistributionPlus()
                    * Constant::D2_ELEC_CHARGE
            + partonDistribution.getQuarkDistribution(QuarkFlavor::STRANGE).getQuarkDistributionPlus()
                    * Constant::S2_ELEC_CHARGE;
}

double DVCSCFFDispersionRelation::dispersionRelationIntegralPartDiagonalA(
        double xi, std::vector<double> par) {
    return (computeSquareChargeAveragedGPD(xi)
            - computeSquareChargeAveragedGPD(m_xi)) / (m_xi - xi);
}

double DVCSCFFDispersionRelation::dispersionRelationIntegralPartDiagonalB(
        double xi, std::vector<double> par) {
    return computeSquareChargeAveragedGPD(xi) / (m_xi + xi);
}

GPDSubtractionConstantModule* DVCSCFFDispersionRelation::getSubtractionConstantModule() const {
    return m_pSubtractionConstantModule;
}

void DVCSCFFDispersionRelation::setSubtractionConstantModule(
        GPDSubtractionConstantModule* subtractionConstantModule) {

    m_pModuleObjectFactory->updateModulePointerReference(
            m_pSubtractionConstantModule, subtractionConstantModule);
    m_pSubtractionConstantModule = subtractionConstantModule;
}

} /* namespace PARTONS */
