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
    m_epsilon = 1.E-7;
}

DDVCSCFFTEST::DDVCSCFFTEST(const DDVCSCFFTEST &other) :
        DDVCSConvolCoeffFunctionModule(other) {

    //set functors
    initFunctorsForIntegrations();

    //set variables
    m_partonDistributionEtaXiSummed = other.m_partonDistributionEtaXiSummed;
    m_epsilon = other.m_epsilon;
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

    if (m_pConvolutionUnpolarizedL) {
        delete m_pConvolutionUnpolarizedL;
        m_pConvolutionUnpolarizedL = 0;
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

    m_pConvolutionUnpolarizedL = NumA::Integrator1D::newIntegrationFunctor(this,
            &DDVCSCFFTEST::convolutionUnpolarizedL);
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

//GPD type
    GPDType::Type gpdType;

    switch (m_currentGPDComputeType) {
    case GPDType::HL:
        gpdType = GPDType::H;
        break;
    case GPDType::EL:
        gpdType = GPDType::E;
        break;
    default:
        gpdType = m_currentGPDComputeType;
    }

    //evaluate GPD at (eta, xi)
    m_partonDistributionEtaXiSummed = computeSquareChargeAveragedGPD(
            m_pGPDModule->compute(
                    GPDKinematic(m_eta, m_xi, m_t, m_MuF2, m_MuR2), gpdType));

    //CFF values
    double re = 0.;
    double im = 0.;

    /*
     * parameters:
     * first element is gpdType;
     * second is 0. for computing real part of CFF and 1. for imaginary
     * third is 0. for CFF at LO and 1. at NLO
     */

    std::vector<double> parameters(3, 0.); //parameters(length, one initialization for all positions)

    if (m_currentGPDComputeType == GPDType::H
            || m_currentGPDComputeType == GPDType::E) {

        parameters.at(0) = static_cast<double>(gpdType);

        //computing real part of CFF
        parameters.at(1) = 0.;
        re = integrate(m_pConvolutionUnpolarized, 0., 1., parameters);
        //computing imaginary part of CFF
        parameters.at(1) = 1.;
        im = integrate(m_pConvolutionUnpolarized, 0., 1., parameters);
    }

    if (m_currentGPDComputeType == GPDType::HL
            || m_currentGPDComputeType == GPDType::EL) {

        if (m_qcdOrderType == PerturbativeQCDOrderType::LO) {

            return std::complex<double>(0., 0.);
        }

        parameters.at(0) = static_cast<double>(gpdType);

        //computing real part of CFF
        parameters.at(1) = 0.;
        re = integrate(m_pConvolutionUnpolarized, 0., 1., parameters);
        //computing imaginary part of CFF
        parameters.at(1) = 1.;
        im = integrate(m_pConvolutionUnpolarized, 0., 1., parameters);

    }

    //parameters
//    std::vector<double> parameters(1, 0.);
//
//    if (m_currentGPDComputeType == GPDType::H
//            || m_currentGPDComputeType == GPDType::E) {
//
//        parameters.at(0) = static_cast<double>(gpdType);
//
//        re = integrate(m_pConvolutionUnpolarized, 0., 1., parameters);
//    }
//
//    if (m_currentGPDComputeType == GPDType::HL
//            || m_currentGPDComputeType == GPDType::EL) {
//
//        parameters.at(0) = static_cast<double>(gpdType);
//
//        re = integrate(m_pConvolutionUnpolarizedL, 0., 1., parameters);
//    }

    return std::complex<double>(re, im);
}

std::complex<double> DDVCSCFFTEST::computePolarized() {

    //GPD type
    GPDType::Type gpdType;

    //evaluate GPD at (eta, xi)
    m_partonDistributionEtaXiSummed = computeSquareChargeAveragedGPD(
            m_pGPDModule->compute(
                    GPDKinematic(m_eta, m_xi, m_t, m_MuF2, m_MuR2), gpdType));

    //CFF values
    double re = 0.;
    double im = 0.;

    /*
     * parameters:
     * first element is gpdType;
     * second is 0. for computing real part of CFF and 1. for imaginary
     * third is 0. for CFF at LO and 1. at NLO
     */

    std::vector<double> parameters(3, 0.); //parameters(length, one initialization for all positions)

    if (m_currentGPDComputeType == GPDType::Ht
            || m_currentGPDComputeType == GPDType::Et) {

        parameters.at(0) = static_cast<double>(gpdType);

        //computing real part of CFF
        parameters.at(1) = 0.;
        re = integrate(m_pConvolutionPolarized, 0., 1., parameters);
        //computing imaginary part of CFF
        parameters.at(1) = 1.;
        im = integrate(m_pConvolutionPolarized, 0., 1., parameters);
    }

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

    GPDType::Type gpdType = static_cast<GPDType::Type>(params.at(0));

    double convo; //Function to be convoluted/integrated in x

    double PV_LO; //PV integral in CFF at LO
    double ANALITIC_LO; // analitic term in CFF at LO

    double epsilonAbsEta = m_epsilon * fabs(m_eta);

    double partonDistributionXXiSummed = computeSquareChargeAveragedGPD(
            m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
                    gpdType));

    if (params.at(1) == 0.) {

        //Term that goes with +eta (adding all flavors)
        PV_LO =
                (x - m_eta) / (pow(x - m_eta, 2.) + epsilonAbsEta)
                        * (partonDistributionXXiSummed
                                - m_partonDistributionEtaXiSummed);

        //Term that goes with -eta (adding all flavors)
        PV_LO +=
                (x + m_eta) / (pow(x + m_eta, 2.) + epsilonAbsEta)
                        * (partonDistributionXXiSummed
                                + m_partonDistributionEtaXiSummed);

        //Analitic term to CFF for +eta (addding all flavors)
        ANALITIC_LO = m_partonDistributionEtaXiSummed
                * log(fabs(1. - 1. / m_eta));

        //Analitic term to CFF for -eta (addding all flavors)
        ANALITIC_LO += -1 * m_partonDistributionEtaXiSummed
                * log(fabs(1. + 1. / m_eta));

        convo = (-1.) * (PV_LO + ANALITIC_LO);

    } else if (params.at(1) == 1.) {

        convo = M_PI * m_partonDistributionEtaXiSummed;

    }

    return convo;
}

double DDVCSCFFTEST::convolutionPolarized(double x,
        std::vector<double> params) {

    GPDType::Type gpdType = static_cast<GPDType::Type>(params.at(0));

    double convo; //Function to be convoluted/integrated in x

    double PV_LO; //PV integral in CFF at LO
    double ANALITIC_LO; // analitic term in CFF at LO

    double epsilonAbsEta = m_epsilon * fabs(m_eta);

    double partonDistributionXXiSummed = computeSquareChargeAveragedGPD(
            m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
                    gpdType));

    if (params.at(1) == 0.) {

        //Term that goes with +eta (adding all flavors)
        PV_LO =
                (x - m_eta) / (pow(x - m_eta, 2.) + epsilonAbsEta)
                        * (partonDistributionXXiSummed
                                - m_partonDistributionEtaXiSummed);

        //Term that goes with -eta (adding all flavors)
        PV_LO +=
                (x + m_eta) / (pow(x + m_eta, 2.) + epsilonAbsEta)
                        * (partonDistributionXXiSummed
                                + m_partonDistributionEtaXiSummed);

        //Analitic term to CFF for +eta (addding all flavors)
        ANALITIC_LO = m_partonDistributionEtaXiSummed
                * log(fabs(1. - 1. / m_eta));

        //Analitic term to CFF for -eta (addding all flavors)
        ANALITIC_LO += -1 * m_partonDistributionEtaXiSummed
                * log(fabs(1. + 1. / m_eta));

        convo = -(PV_LO + ANALITIC_LO);

    } else if (params.at(1) == 1.) {

        convo = M_PI * m_partonDistributionEtaXiSummed;

    }

    return convo;
}

//CFFs for HL, EL are NLO
double DDVCSCFFTEST::convolutionUnpolarizedL(double x,
        std::vector<double> params) {
    return 0.;
}

/*
 double DDVCSCFFTEST::convolution(double x, std::vector<double> params) {
 return x
 * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
 GPDType::H).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistributionPlus();
 }
 */

double DDVCSCFFTEST::getEpsilon() const {
    return m_epsilon;
}

void DDVCSCFFTEST::setEpsilon(double epsilon) {
    m_epsilon = epsilon;
}

} /* namespace PARTONS */

