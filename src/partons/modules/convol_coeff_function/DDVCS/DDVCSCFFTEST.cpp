#include "../../../../../include/partons/modules/convol_coeff_function/DDVCS/DDVCSCFFTEST.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <utility>

#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/gpd/GPDModule.h"

namespace PARTONS {

const unsigned int DDVCSCFFTEST::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSCFFTEST("DDVCSCFFTEST"));

DDVCSCFFTEST::DDVCSCFFTEST(const std::string &className) :
        DDVCSConvolCoeffFunctionModule(className) {

    //relate CFF types with functions
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &DDVCSConvolCoeffFunctionModule::computeUnpolarized));

    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::E,
                    &DDVCSConvolCoeffFunctionModule::computeUnpolarized));

    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Ht,
                    &DDVCSConvolCoeffFunctionModule::computePolarized));

    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Et,
                    &DDVCSConvolCoeffFunctionModule::computePolarized));

    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::HL,
                    &DDVCSConvolCoeffFunctionModule::computeUnpolarized));

    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::EL,
                    &DDVCSConvolCoeffFunctionModule::computeUnpolarized));

    //set functors
    initFunctorsForIntegrations();

    //set variables
    m_partonDistributionEtaXiSummed = 0.;
}

DDVCSCFFTEST::DDVCSCFFTEST(const DDVCSCFFTEST &other) :
        DDVCSConvolCoeffFunctionModule(other) {

    //set functors
    initFunctorsForIntegrations();

    //set variables
    m_partonDistributionEtaXiSummed = other.m_partonDistributionEtaXiSummed;
}

void DDVCSCFFTEST::resolveObjectDependencies() {

    //resolve for parent module
    DDVCSConvolCoeffFunctionModule::resolveObjectDependencies();

    //set integration type
    setIntegrator(NumA::IntegratorType1D::DEXP);
}

DDVCSCFFTEST::~DDVCSCFFTEST() {

    // destroy functors
    if (m_pConvolutionUnpolarized) {
        delete m_pConvolutionUnpolarized;
        m_pConvolutionUnpolarized = 0;
    }

    if (m_pConvolutionPolarized) {
        delete m_pConvolutionPolarized;
        m_pConvolutionPolarized = 0;
    }
}

DDVCSCFFTEST* DDVCSCFFTEST::clone() const {
    return new DDVCSCFFTEST(*this);
}

void DDVCSCFFTEST::initFunctorsForIntegrations() {

    m_pConvolutionUnpolarized = NumA::Integrator1D::newIntegrationFunctor(this,
            &DDVCSCFFTEST::convolutionUnpolarized);

    m_pConvolutionPolarized = NumA::Integrator1D::newIntegrationFunctor(this,
            &DDVCSCFFTEST::convolutionPolarized);
}

void DDVCSCFFTEST::initModule() {

    //init parent module
    DDVCSConvolCoeffFunctionModule::initModule();
}

void DDVCSCFFTEST::isModuleWellConfigured() {

    //check parent module
    DDVCSConvolCoeffFunctionModule::isModuleWellConfigured();

    //check if GPD module set
    if (m_pGPDModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "GPDModule* is NULL");
    }

    //check pQCD order
    if (m_qcdOrderType == PerturbativeQCDOrderType::UNDEFINED) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "QCDOrderType is UNDEFINED");
    }

    //check if LO or NLO
    if (m_qcdOrderType != PerturbativeQCDOrderType::LO) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Perturbative QCD order can only be LO,  here: "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }
}

std::complex<double> DDVCSCFFTEST::computeUnpolarized() {

    //check for HL and EL
    if (m_currentGPDComputeType == GPDType::HL
            || m_currentGPDComputeType == GPDType::EL) {
        return std::complex<double>(0., 0.);

    }

    //parameters
    std::vector<double> params(1);

    //absolute value of eta
    double absEta = fabs(m_eta);

    //evaluate GPD at (eta, xi)
    m_partonDistributionEtaXiSummed = computeSquareChargeAveragedGPD(
            m_pGPDModule->compute(
                    GPDKinematic(absEta, m_xi, m_t, m_MuF2, m_MuR2),
                    m_currentGPDComputeType));

    //CFF values
    double im = ((m_eta > 0.) ? (1) : (-1)) * M_PI
            * m_partonDistributionEtaXiSummed;
    double re = 0.;

    params.at(0) = -1.;
    re += integrate(m_pConvolutionUnpolarized, 0., absEta, params);
    re += integrate(m_pConvolutionUnpolarized, absEta, 1., params);

    re += ((m_eta > 0.) ?
            (log(absEta / (1. - absEta))) : (-log((1. + absEta) / absEta)))
            * m_partonDistributionEtaXiSummed;

    params.at(0) = 1.;
    re -= integrate(m_pConvolutionUnpolarized, 0., absEta, params);
    re -= integrate(m_pConvolutionUnpolarized, absEta, 1., params);

    re -= ((m_eta > 0.) ?
            (log((1. + absEta) / absEta)) : (-log(absEta / (1. - absEta))))
            * m_partonDistributionEtaXiSummed;

    return std::complex<double>(re, im);
}

std::complex<double> DDVCSCFFTEST::computePolarized() {

    //parameters
    std::vector<double> params(1);

    //absolute value of eta
    double absEta = fabs(m_eta);

    //evaluate GPD at (eta, xi)
    m_partonDistributionEtaXiSummed = computeSquareChargeAveragedGPD(
            m_pGPDModule->compute(
                    GPDKinematic(absEta, m_xi, m_t, m_MuF2, m_MuR2),
                    m_currentGPDComputeType));

    //CFF values
    double im = ((m_eta > 0.) ? (1) : (-1)) * M_PI
            * m_partonDistributionEtaXiSummed;
    double re = 0.;

    params.at(0) = -1.;
    re += integrate(m_pConvolutionPolarized, 0., absEta, params);
    re += integrate(m_pConvolutionPolarized, absEta, 1., params);

    re += ((m_eta > 0.) ?
            (log(absEta / (1. - absEta))) : (-log((1. + absEta) / absEta)))
            * m_partonDistributionEtaXiSummed;

    params.at(0) = 1.;
    re += integrate(m_pConvolutionPolarized, 0., absEta, params);
    re += integrate(m_pConvolutionPolarized, absEta, 1., params);

    re += ((m_eta > 0.) ?
            (log((1. + absEta) / absEta)) : (-log(absEta / (1. - absEta))))
            * m_partonDistributionEtaXiSummed;

    return std::complex<double>(re, im);
}

double DDVCSCFFTEST::computeSquareChargeAveragedGPD(
        const PartonDistribution& partonDistribution) const {

    double result = 0.;

    result +=
            (partonDistribution.getQuarkDistribution(QuarkFlavor::UP).getQuarkDistributionPlus())
                    * Constant::U2_ELEC_CHARGE;
    result +=
            (partonDistribution.getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistributionPlus())
                    * Constant::D2_ELEC_CHARGE;
    result +=
            (partonDistribution.getQuarkDistribution(QuarkFlavor::STRANGE).getQuarkDistributionPlus())
                    * Constant::S2_ELEC_CHARGE;

    return result;

}

double DDVCSCFFTEST::convolutionUnpolarized(double x,
        std::vector<double> params) {

    double partonDistributionXXiSummed = computeSquareChargeAveragedGPD(
            m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
                    m_currentGPDComputeType));

    return (partonDistributionXXiSummed - m_partonDistributionEtaXiSummed)
            / (m_eta + params.at(0) * x);
}

double DDVCSCFFTEST::convolutionPolarized(double x,
        std::vector<double> params) {

    double partonDistributionXXiSummed = computeSquareChargeAveragedGPD(
            m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
                    m_currentGPDComputeType));

    return (partonDistributionXXiSummed - m_partonDistributionEtaXiSummed)
            / (m_eta + params.at(0) * x);
}

} /* namespace PARTONS */

