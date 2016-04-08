#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSCFFVGGModel.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/integration/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/GLNPIntegrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <map>
#include <utility>

#include "../../../../../include/partons/beans/gpd/GPDResult.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/alphaS/RunningAlphaStrong.h"
#include "../../../../../include/partons/modules/evolution/gpd/ExampleEvolQCDModel.h"
#include "../../../../../include/partons/modules/GPDModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"

const unsigned int DVCSCFFVGGModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFVGGModel("DVCSCFFVGGModel"));

const std::string DVCSCFFVGGModel::PARAMETER_NAME_EPS =
        "DVCSCFFVGGModel_Eps";

DVCSCFFVGGModel::DVCSCFFVGGModel(const std::string& className) :
        DVCSConvolCoeffFunctionModule(className) {

    eps_cffint = 1.E-3;
    xixit = -1.;

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

    initFunctorsForIntegrations();
}

DVCSCFFVGGModel* DVCSCFFVGGModel::clone() const {
    return new DVCSCFFVGGModel(*this);
}

DVCSCFFVGGModel::~DVCSCFFVGGModel() {

    if (m_pIntd_vector_part) {
        delete m_pIntd_vector_part;
        m_pIntd_vector_part = 0;
    }

    if (m_pIntc_vector_part) {
        delete m_pIntc_vector_part;
        m_pIntc_vector_part = 0;
    }
}

void DVCSCFFVGGModel::initFunctorsForIntegrations() {
    m_pIntd_vector_part = NumA::Integrator1D::newIntegrationFunctor(this,
            &DVCSCFFVGGModel::intd_vector_part);
    m_pIntc_vector_part = NumA::Integrator1D::newIntegrationFunctor(this,
            &DVCSCFFVGGModel::intc_vector_part);
}

//TODO init in mother class ? ; propagate init to mother class ?
void DVCSCFFVGGModel::resolveObjectDependencies() {

    setIntegrator(NumA::IntegratorType1D::GLNP);
    ElemUtils::Parameters parameters(NumA::GLNPIntegrator1D::PARAM_NAME_N, 40);
    configureIntegrator(parameters);

    m_pRunningAlphaStrongModule =
            Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                    RunningAlphaStrong::classId);

    m_pNfConvolCoeffFunction =
            Partons::getInstance()->getModuleObjectFactory()->newActiveFlavorsModule(
                    ExampleEvolQCDModel::classId);
}

void DVCSCFFVGGModel::configure(const ElemUtils::Parameters &parameters) {

    if (parameters.isAvailable(DVCSCFFVGGModel::PARAMETER_NAME_EPS)) {
        eps_cffint = parameters.getLastAvailable().toDouble();
    }
}

DVCSCFFVGGModel::DVCSCFFVGGModel(const DVCSCFFVGGModel& other) :
        DVCSConvolCoeffFunctionModule(other) {

    eps_cffint = other.eps_cffint;
    xixit = other.xixit;

    initFunctorsForIntegrations();
}

void DVCSCFFVGGModel::initModule() {
    DVCSConvolCoeffFunctionModule::initModule();
}

void DVCSCFFVGGModel::isModuleWellConfigured() {
    DVCSConvolCoeffFunctionModule::isModuleWellConfigured();
}

std::complex<double> DVCSCFFVGGModel::computeUnpolarized() {

    //check pQCD order
    if (m_qcdOrderType != PerturbativeQCDOrderType::LO) {
        error(__FUNCTION__,
                ElemUtils::Formatter()
                        << "Calculation not supported for pQCD order = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }

    //calculate GPD at (xi, xi, t)
    calculate_xixit_value();

    //return
    return calculate_direct() + calculate_crossed();
}

std::complex<double> DVCSCFFVGGModel::computePolarized() {

    //check pQCD order
    if (m_qcdOrderType != PerturbativeQCDOrderType::LO) {
        error(__FUNCTION__,
                ElemUtils::Formatter()
                        << "Calculation not supported for pQCD order = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }

    //calculate GPD at (xi, xi, t)
    calculate_xixit_value();

    //return
    return calculate_direct() - calculate_crossed();
}

double DVCSCFFVGGModel::calculate_gpd_combination(GPDResult gpdResult) {

    return gpdResult.getPartonDistribution(m_currentGPDComputeType).getQuarkDistribution(
            QuarkFlavor::UP).getQuarkDistributionPlus() * U2_ELEC_CHARGE
            + gpdResult.getPartonDistribution(m_currentGPDComputeType).getQuarkDistribution(
                    QuarkFlavor::DOWN).getQuarkDistributionPlus()
                    * D2_ELEC_CHARGE
            + gpdResult.getPartonDistribution(m_currentGPDComputeType).getQuarkDistribution(
                    QuarkFlavor::STRANGE).getQuarkDistributionPlus()
                    * S2_ELEC_CHARGE;
}

void DVCSCFFVGGModel::calculate_xixit_value() {

    GPDResult gpdResult = m_pGPDModule->compute(m_xi, m_xi, m_t, m_MuF2, m_MuR2,
            m_currentGPDComputeType);

    xixit = calculate_gpd_combination(gpdResult);
}

double DVCSCFFVGGModel::intd_vector_part(double x, std::vector<double> par) {

    GPDResult gpdResult = m_pGPDModule->compute(x, m_xi, m_t, m_MuF2, m_MuR2,
            m_currentGPDComputeType);

    return (calculate_gpd_combination(gpdResult) - xixit) / (x - m_xi);
}

double DVCSCFFVGGModel::intc_vector_part(double x, std::vector<double> par) {

    GPDResult gpdResult = m_pGPDModule->compute(x, m_xi, m_t, m_MuF2, m_MuR2,
            m_currentGPDComputeType);

    return calculate_gpd_combination(gpdResult) / (x + m_xi);
}

std::complex<double> DVCSCFFVGGModel::calculate_direct() {

    //check allowed range of xi
    if (m_xi - eps_cffint < 0. || m_xi + eps_cffint > 1.) {
        error(__FUNCTION__,
                ElemUtils::Formatter() << "Invalid xi +/- eps = "
                        << m_xi - eps_cffint << "/" << m_xi + eps_cffint);
    }

    //parameters for the integration (zero-length vector in this case)
    std::vector<double> parameters;

    //direct Faynman diagram
    double intd1 = integrate(m_pIntd_vector_part, 0., m_xi - eps_cffint,
            parameters);

    double intd2 = integrate(m_pIntd_vector_part, m_xi - eps_cffint,
            m_xi + eps_cffint, parameters);

    double intd3 = integrate(m_pIntd_vector_part, m_xi + eps_cffint, 1.,
            parameters);

    //return (multiplied by -1 to match the convention used by other CFF modules where 1/(xi-x) instead of 1/(x-xi) is used)
    return -1.
            * std::complex<double>(
                    intd1 + intd2 + intd3 + xixit * log((1. - m_xi) / m_xi),
                    -PI * xixit);
}

std::complex<double> DVCSCFFVGGModel::calculate_crossed() {

    //check allowed range of xi
    if (m_xi - eps_cffint < 0. || m_xi + eps_cffint > 1.) {
        error(__FUNCTION__,
                ElemUtils::Formatter() << "Invalid xi +/- eps = "
                        << m_xi - eps_cffint << "/" << m_xi + eps_cffint);
    }

    //parameters for the integration (zero-length vector in this case)
    std::vector<double> parameters;

    //crossed Faynman diagram
    double intc1 = integrate(m_pIntc_vector_part, 0., m_xi, parameters);

    double intc2 = integrate(m_pIntc_vector_part, m_xi, 1., parameters);

    //return (multiplied by -1 to match the convention used by other CFF modules where 1/(xi-x) instead of 1/(x-xi) is used)
    return -1. * std::complex<double>(intc1 + intc2, 0.);
}
