#include "../../../../../include/partons/modules/convol_coeff_function/DDVCS/DDVCSCFFStandard.h"

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

const unsigned int DDVCSCFFStandard::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSCFFStandard("DDVCSCFFStandard"));

DDVCSCFFStandard::DDVCSCFFStandard(const std::string &className) :
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
    m_partonDistributionRhoXiSummed = 0.;
}

DDVCSCFFStandard::DDVCSCFFStandard(const DDVCSCFFStandard &other) :
        DDVCSConvolCoeffFunctionModule(other) {

    //set functors
    initFunctorsForIntegrations();

    //set variables
    m_partonDistributionRhoXiSummed = other.m_partonDistributionRhoXiSummed;
}

void DDVCSCFFStandard::resolveObjectDependencies() {

    //resolve for parent module
    DDVCSConvolCoeffFunctionModule::resolveObjectDependencies();

    //set integration type
    setIntegrator(NumA::IntegratorType1D::DEXP);
}

DDVCSCFFStandard::~DDVCSCFFStandard() {

    // destroy functors
    if (m_pConvolution) {
        delete m_pConvolution;
        m_pConvolution = 0;
    }

    if (m_pConvolutionNoSubtraction) {
        delete m_pConvolutionNoSubtraction;
        m_pConvolutionNoSubtraction = 0;
    }
}

DDVCSCFFStandard* DDVCSCFFStandard::clone() const {
    return new DDVCSCFFStandard(*this);
}

void DDVCSCFFStandard::initFunctorsForIntegrations() {

    m_pConvolution = NumA::Integrator1D::newIntegrationFunctor(this,
            &DDVCSCFFStandard::convolution);
    m_pConvolutionNoSubtraction = NumA::Integrator1D::newIntegrationFunctor(
            this, &DDVCSCFFStandard::convolutionNoSubtraction);
}

void DDVCSCFFStandard::initModule() {
    DDVCSConvolCoeffFunctionModule::initModule();
}

void DDVCSCFFStandard::isModuleWellConfigured() {

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

std::complex<double> DDVCSCFFStandard::computeUnpolarized() {

    //check for HL and EL
    if (m_currentGPDComputeType == GPDType::HL
            || m_currentGPDComputeType == GPDType::EL) {
        return std::complex<double>(0., 0.);
    }

    //parameters
    std::vector<double> params(1);

    //absolute value of rho
    double absRho = fabs(m_rho);

    //evaluate GPD at (|rho|, xi)
    if (absRho != 0.) {
        m_partonDistributionRhoXiSummed = computeSquareChargeAveragedGPD(
                m_pGPDModule->compute(
                        GPDKinematic(absRho, m_xi, m_t, m_MuF2, m_MuR2),
                        m_currentGPDComputeType));
    }

    //CFF values
    double im = M_PI * m_partonDistributionRhoXiSummed;
    double re = 0.;

    params.at(0) = -1.;
    if (absRho != 0.) {
        re += integrate(m_pConvolution, 0., absRho, params);
        re += integrate(m_pConvolution, absRho, 1., params);
        re += log(absRho / (1. - absRho)) * m_partonDistributionRhoXiSummed;
    } else {
        re += integrate(m_pConvolutionNoSubtraction, 0., 1., params);
    }

    params.at(0) = 1.;
    if (absRho != 0.) {
        re -= integrate(m_pConvolution, 0., absRho, params);
        re -= integrate(m_pConvolution, absRho, 1., params);
        re -= log((1. + absRho) / absRho) * m_partonDistributionRhoXiSummed;
    } else {
        re -= integrate(m_pConvolutionNoSubtraction, 0., 1., params);
    }

    return std::complex<double>(re, ((m_rho > 0.) ? (1) : (-1)) * im);
}

std::complex<double> DDVCSCFFStandard::computePolarized() {

    //parameters
    std::vector<double> params(1);

    //absolute value of rho
    double absRho = fabs(m_rho);

    //evaluate GPD at (|rho|, xi)
    m_partonDistributionRhoXiSummed = computeSquareChargeAveragedGPD(
            m_pGPDModule->compute(
                    GPDKinematic(absRho, m_xi, m_t, m_MuF2, m_MuR2),
                    m_currentGPDComputeType));

    //CFF values
    double im = M_PI * m_partonDistributionRhoXiSummed;
    double re = 0.;

    params.at(0) = -1.;
    if (absRho != 0.) {
        re += integrate(m_pConvolution, 0., absRho, params);
        re += integrate(m_pConvolution, absRho, 1., params);
        re += log(absRho / (1. - absRho)) * m_partonDistributionRhoXiSummed;
    }else{
        re += integrate(m_pConvolutionNoSubtraction, 0., 1., params);
    }

    params.at(0) = 1.;
    if (absRho != 0.) {
        re += integrate(m_pConvolution, 0., absRho, params);
        re += integrate(m_pConvolution, absRho, 1., params);
        re += log((1. + absRho) / absRho) * m_partonDistributionRhoXiSummed;
    }else{
        re += integrate(m_pConvolutionNoSubtraction, 0., 1., params);
    }

    return std::complex<double>(((m_rho > 0.) ? (1) : (-1)) * re, im);
}

double DDVCSCFFStandard::computeSquareChargeAveragedGPD(
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

double DDVCSCFFStandard::convolution(double x, std::vector<double> params) {

    double partonDistributionXXiSummed = computeSquareChargeAveragedGPD(
            m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
                    m_currentGPDComputeType));

    return (partonDistributionXXiSummed - m_partonDistributionRhoXiSummed)
            / (fabs(m_rho) + params.at(0) * x);
}

double DDVCSCFFStandard::convolutionNoSubtraction(double x, std::vector<double> params) {

    double partonDistributionXXiSummed = computeSquareChargeAveragedGPD(
            m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
                    m_currentGPDComputeType));

    return partonDistributionXXiSummed
            / (fabs(m_rho) + params.at(0) * x);
}

}/* namespace PARTONS */

