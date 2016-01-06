/*
 * DVCSCFFVGGModel.cpp
 *
 *  Created on: Dec 20, 2015
 *      Author: Michel Guidal and Pawel Sznajder (IPNO)
 */

#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSCFFVGGModel.h"

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/beans/gpd/GPDResult.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/modules/alphaS/RunningAlphaStrong.h"
#include "../../../../../include/partons/modules/evolution/gpd/ExampleEvolQCDModel.h"
#include "../../../../../include/partons/modules/GPDModule.h"
#include "../../../../../include/partons/utils/stringUtils/Formatter.h"

const unsigned int DVCSCFFVGGModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFVGGModel("DVCSCFFVGGModel"));

const double DVCSCFFVGGModel::eps_cffint = 1.E-3;

DVCSCFFVGGModel::DVCSCFFVGGModel(const std::string& className) :
        DVCSConvolCoeffFunctionModule(className) {

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
}

DVCSCFFVGGModel* DVCSCFFVGGModel::clone() const {
    return new DVCSCFFVGGModel(*this);
}

DVCSCFFVGGModel::~DVCSCFFVGGModel() {
}

void DVCSCFFVGGModel::init() {

    m_mathIntegrator.setIntegrationMode(NumA::MathIntegrator::GLNP);

    m_pRunningAlphaStrongModule =
            ModuleObjectFactory::newRunningAlphaStrongModule(
                    RunningAlphaStrong::classId);

    m_pNfConvolCoeffFunction = ModuleObjectFactory::newActiveFlavorsModule(
            ExampleEvolQCDModel::classId);
}

DVCSCFFVGGModel::DVCSCFFVGGModel(const DVCSCFFVGGModel& other) :
        DVCSConvolCoeffFunctionModule(other) {

    xixit = other.xixit;
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
        throw std::runtime_error(
                Formatter()
                        << "[DVCSCFFVGGModel::computeUnpolarized()] Calculation not supported for pQCD order = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }

    //check allowed range of xi
    if (m_xi - eps_cffint < 0. || m_xi + eps_cffint > 1.) {
        throw std::runtime_error(
                Formatter()
                        << "[DVCSCFFVGGModel::computeUnpolarized()] Invalid xi +/- eps = "
                        << m_xi - eps_cffint << "/" << m_xi + eps_cffint);
    }

    //calculate GPD at (xi, xi, t)
    calculate_xixit_value();

    //parameters for the integration (zero-lenght vector in this case)
    std::vector<double> parameters;
    int n_int_steps = 100;

    //direct Faynman diagram
    double intd1 = m_mathIntegrator.integrate(this,
            &DVCSCFFVGGModel::intd_vector_part, 0., m_xi - eps_cffint,
            parameters, n_int_steps);

    double intd2 = m_mathIntegrator.integrate(this,
            &DVCSCFFVGGModel::intd_vector_part, m_xi - eps_cffint,
            m_xi + eps_cffint, parameters, n_int_steps);

    double intd3 = m_mathIntegrator.integrate(this,
            &DVCSCFFVGGModel::intd_vector_part, m_xi + eps_cffint, 1.,
            parameters, n_int_steps);

    std::complex<double> direct(
            intd1 + intd2 + intd3 + xixit * log((1. - m_xi) / m_xi),
            -PI * xixit);

    //crossed Faynman diagram
    double intc1 = m_mathIntegrator.integrate(this,
            &DVCSCFFVGGModel::intc_vector_part, 0., m_xi, parameters,
            n_int_steps);

    double intc2 = m_mathIntegrator.integrate(this,
            &DVCSCFFVGGModel::intc_vector_part, m_xi, 1., parameters,
            n_int_steps);

    std::complex<double> crossed(intc1 + intc2, 0.);

    //return
    return direct + crossed;
}

std::complex<double> DVCSCFFVGGModel::computePolarized() {

    //check pQCD order
    if (m_qcdOrderType != PerturbativeQCDOrderType::LO) {
        throw std::runtime_error(
                Formatter()
                        << "[DVCSCFFVGGModel::computePolarized()] Calculation not supported for pQCD order = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }

    return DVCSCFFVGGModel::computeUnpolarized();
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

double DVCSCFFVGGModel::intd_vector_part(std::vector<double> x,
        std::vector<double> par) {

    GPDResult gpdResult = m_pGPDModule->compute(x[0], m_xi, m_t, m_MuF2, m_MuR2,
            m_currentGPDComputeType);

    return (calculate_gpd_combination(gpdResult) - xixit) / (x[0] - m_xi);
}

double DVCSCFFVGGModel::intc_vector_part(std::vector<double> x,
        std::vector<double> par) {

    GPDResult gpdResult = m_pGPDModule->compute(x[0], m_xi, m_t, m_MuF2, m_MuR2,
            m_currentGPDComputeType);

    return calculate_gpd_combination(gpdResult) / (x[0] + m_xi);
}

