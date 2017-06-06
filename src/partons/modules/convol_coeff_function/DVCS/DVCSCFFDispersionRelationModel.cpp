#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSCFFDispersionRelationModel.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <map>
#include <utility>

#include "../../../../../include/partons/beans/automation/BaseObjectData.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/active_flavors/NfFunctionExample.h"
#include "../../../../../include/partons/modules/alphaS/RunningAlphaStrong.h"
#include "../../../../../include/partons/modules/GPDModule.h"
#include "../../../../../include/partons/modules/GPDSubtractionConstantModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"

const unsigned int DVCSCFFDispersionRelationModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFDispersionRelationModel(
                        "DVCSCFFDispersionRelationModel"));

DVCSCFFDispersionRelationModel::DVCSCFFDispersionRelationModel(
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

DVCSCFFDispersionRelationModel* DVCSCFFDispersionRelationModel::clone() const {
    return new DVCSCFFDispersionRelationModel(*this);
}

DVCSCFFDispersionRelationModel::~DVCSCFFDispersionRelationModel() {

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

void DVCSCFFDispersionRelationModel::initFunctorsForIntegrations() {

    m_p_int_dispersionRelationIntegralPartDiagonalA =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSCFFDispersionRelationModel::dispersionRelationIntegralPartDiagonalA);

    m_p_int_dispersionRelationIntegralPartDiagonalB =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSCFFDispersionRelationModel::dispersionRelationIntegralPartDiagonalB);
}

//TODO init in mother class ? ; propagate init to mother class ?
void DVCSCFFDispersionRelationModel::resolveObjectDependencies() {

    setIntegrator(NumA::IntegratorType1D::DEXP);

    m_pRunningAlphaStrongModule =
            Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                    RunningAlphaStrong::classId);

    m_pNfConvolCoeffFunction =
            Partons::getInstance()->getModuleObjectFactory()->newActiveFlavorsModule(
                    NfFunctionExample::classId);
}

void DVCSCFFDispersionRelationModel::configure(
        const ElemUtils::Parameters &parameters) {
    DVCSConvolCoeffFunctionModule::configure(parameters);
}

void DVCSCFFDispersionRelationModel::prepareSubModules(
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

DVCSCFFDispersionRelationModel::DVCSCFFDispersionRelationModel(
        const DVCSCFFDispersionRelationModel& other) :
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

void DVCSCFFDispersionRelationModel::initModule() {
    DVCSConvolCoeffFunctionModule::initModule();
}

void DVCSCFFDispersionRelationModel::isModuleWellConfigured() {

    if (!m_pSubtractionConstantModule) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "SubtractionConstantModule not set");
    }

    DVCSConvolCoeffFunctionModule::isModuleWellConfigured();
}

std::complex<double> DVCSCFFDispersionRelationModel::computeUnpolarized() {

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
    double Im = M_PI * computeSquareChargeAveragedGPD(m_xi);

    //return
    return std::complex<double>(Re - Sub, Im);
}

std::complex<double> DVCSCFFDispersionRelationModel::computePolarized() {

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
    double Im = M_PI * computeSquareChargeAveragedGPD(m_xi);

    //return
    return std::complex<double>(Re, Im);
}

double DVCSCFFDispersionRelationModel::computeSquareChargeAveragedGPD(
        double xi) {

    //get GPD result
    PartonDistribution partonDistribution = m_pGPDModule->compute(xi, xi, m_t,
            m_MuF2, m_MuR2, m_currentGPDComputeType);

    //return singlet combination
    return partonDistribution.getQuarkDistribution(QuarkFlavor::UP).getQuarkDistributionPlus()
            * U2_ELEC_CHARGE
            + partonDistribution.getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistributionPlus()
                    * D2_ELEC_CHARGE
            + partonDistribution.getQuarkDistribution(QuarkFlavor::STRANGE).getQuarkDistributionPlus()
                    * S2_ELEC_CHARGE;
}

double DVCSCFFDispersionRelationModel::dispersionRelationIntegralPartDiagonalA(
        double xi, std::vector<double> par) {
    return (computeSquareChargeAveragedGPD(xi)
            - computeSquareChargeAveragedGPD(m_xi)) / (m_xi - xi);
}

double DVCSCFFDispersionRelationModel::dispersionRelationIntegralPartDiagonalB(
        double xi, std::vector<double> par) {
    return computeSquareChargeAveragedGPD(xi) / (m_xi + xi);
}

GPDSubtractionConstantModule* DVCSCFFDispersionRelationModel::getSubtractionConstantModule() const {
    return m_pSubtractionConstantModule;
}

void DVCSCFFDispersionRelationModel::setSubtractionConstantModule(
        GPDSubtractionConstantModule* subtractionConstantModule) {

    m_pModuleObjectFactory->updateModulePointerReference(
            m_pSubtractionConstantModule, subtractionConstantModule);
    m_pSubtractionConstantModule = subtractionConstantModule;
}
