#include "../../../../../include/partons/modules/convol_coeff_function/DDVCS/DDVCSCFFTEST.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <utility>

#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/modules/gpd/GPDModule.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"

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

    //set functors
    initFunctorsForIntegrations();
}

DDVCSCFFTEST::DDVCSCFFTEST(const DDVCSCFFTEST &other) :
        DDVCSConvolCoeffFunctionModule(other) {

    //set functors
    initFunctorsForIntegrations();
}

void DDVCSCFFTEST::resolveObjectDependencies() {

    //resolve for parent module
    DDVCSConvolCoeffFunctionModule::resolveObjectDependencies();

    //set integration type
    setIntegrator(NumA::IntegratorType1D::DEXP);
}

DDVCSCFFTEST::~DDVCSCFFTEST() {

    // destroy functors
    if (m_pConvolution) {
        delete m_pConvolution;
        m_pConvolution = 0;
    }

}

DDVCSCFFTEST* DDVCSCFFTEST::clone() const {
    return new DDVCSCFFTEST(*this);
}

void DDVCSCFFTEST::initFunctorsForIntegrations() {
    m_pConvolution = NumA::Integrator1D::newIntegrationFunctor(this,
            &DDVCSCFFTEST::convolution);
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

    //internal variables to be used
    m_xi;
    m_eta;
    m_t;
    m_MuF2;
    m_MuR2;
    m_Q2;
    m_Q2Prim;

    m_pGPDModule;   //pointer to GPD module
    m_currentGPDComputeType; //GPD type
    m_qcdOrderType; //pQCD order

    //parameters
    std::vector<double> parameters;

    double re = integrate(m_pConvolution, 0., 1., parameters);

    return std::complex<double>(re, 0.);
}

std::complex<double> DDVCSCFFTEST::computePolarized() {
    return std::complex<double>(0., 0.);
}

/*
 * DOUBTS:
 * 1) can I use 'params' in convolution() function to pass gpd type and therefore have this function valid for any gpd? This way I can avoid defining one function per gpd type
 * 2) Can I do the same for 'eps' used to compute principal value in CFF?
 * 3) in main.cpp there's a selection of H as the gpd. If I choose 'ALL' type, then the computation done using m_currentGPDComputeType is repeated over all gpd types?
 * 4) is there any way I can define some variable, let's call it myGDP, such that I can write myGPD.getQuarkDistribution(QuarkFlavor: UP)? Something in the lines of myGPD = m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2), GPDType::H)?
 */

double DDVCSCFFTEST::convolution(double x, std::vector<double> params) {

    double PV_LO; //PV integral in CFF at LO
    const double eps = m_eta / pow(10., 3.); //small parameter that helps with the practical realization of PV: PV(1/x) = integral[x/(x^2 + eps)], eps -> 0
    double ANALITIC_LO; // analitic term in CFF at LO

    //Term that goes with +eta (adding all flavors)
    PV_LO =
            (x - m_eta) / (pow(x - m_eta, 2.) + eps)
                    * (Constant::U2_ELEC_CHARGE
                            * (m_pGPDModule->compute(
                                    GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
                                    GPDType::H).getQuarkDistribution(
                                    QuarkFlavor::UP).getQuarkDistributionPlus()
                                    - m_pGPDModule->compute(
                                            GPDKinematic(m_eta, m_xi, m_t,
                                                    m_MuF2, m_MuR2), GPDType::H).getQuarkDistribution(
                                            QuarkFlavor::UP).getQuarkDistributionPlus())
                            + Constant::D2_ELEC_CHARGE
                                    * (m_pGPDModule->compute(
                                            GPDKinematic(x, m_xi, m_t, m_MuF2,
                                                    m_MuR2), GPDType::H).getQuarkDistribution(
                                            QuarkFlavor::DOWN).getQuarkDistributionPlus()
                                            - m_pGPDModule->compute(
                                                    GPDKinematic(m_eta, m_xi,
                                                            m_t, m_MuF2,
                                                            m_MuR2), GPDType::H).getQuarkDistribution(
                                                    QuarkFlavor::DOWN).getQuarkDistributionPlus())
                            + Constant::S2_ELEC_CHARGE
                                    * (m_pGPDModule->compute(
                                            GPDKinematic(x, m_xi, m_t, m_MuF2,
                                                    m_MuR2), GPDType::H).getQuarkDistribution(
                                            QuarkFlavor::STRANGE).getQuarkDistributionPlus()
                                            - m_pGPDModule->compute(
                                                    GPDKinematic(m_eta, m_xi,
                                                            m_t, m_MuF2,
                                                            m_MuR2), GPDType::H).getQuarkDistribution(
                                                    QuarkFlavor::STRANGE).getQuarkDistributionPlus())
                            + Constant::C2_ELEC_CHARGE
                                    * (m_pGPDModule->compute(
                                            GPDKinematic(x, m_xi, m_t, m_MuF2,
                                                    m_MuR2), GPDType::H).getQuarkDistribution(
                                            QuarkFlavor::CHARM).getQuarkDistributionPlus()
                                            - m_pGPDModule->compute(
                                                    GPDKinematic(m_eta, m_xi,
                                                            m_t, m_MuF2,
                                                            m_MuR2), GPDType::H).getQuarkDistribution(
                                                    QuarkFlavor::CHARM).getQuarkDistributionPlus())
                            + Constant::B2_ELEC_CHARGE
                                    * (m_pGPDModule->compute(
                                            GPDKinematic(x, m_xi, m_t, m_MuF2,
                                                    m_MuR2), GPDType::H).getQuarkDistribution(
                                            QuarkFlavor::BOTTOM).getQuarkDistributionPlus()
                                            - m_pGPDModule->compute(
                                                    GPDKinematic(m_eta, m_xi,
                                                            m_t, m_MuF2,
                                                            m_MuR2), GPDType::H).getQuarkDistribution(
                                                    QuarkFlavor::BOTTOM).getQuarkDistributionPlus())
                            + Constant::T2_ELEC_CHARGE
                                    * (m_pGPDModule->compute(
                                            GPDKinematic(x, m_xi, m_t, m_MuF2,
                                                    m_MuR2), GPDType::H).getQuarkDistribution(
                                            QuarkFlavor::TOP).getQuarkDistributionPlus()
                                            - m_pGPDModule->compute(
                                                    GPDKinematic(m_eta, m_xi,
                                                            m_t, m_MuF2,
                                                            m_MuR2), GPDType::H).getQuarkDistribution(
                                                    QuarkFlavor::TOP).getQuarkDistributionPlus()));

    //Term that goes with -eta (adding all flavors)
    m_eta = -m_eta;

    PV_LO +=
            (x - m_eta) / (pow(x - m_eta, 2.) + eps)
                    * (Constant::U2_ELEC_CHARGE
                            * (m_pGPDModule->compute(
                                    GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
                                    GPDType::H).getQuarkDistribution(
                                    QuarkFlavor::UP).getQuarkDistributionPlus()
                                    - m_pGPDModule->compute(
                                            GPDKinematic(m_eta, m_xi, m_t,
                                                    m_MuF2, m_MuR2), GPDType::H).getQuarkDistribution(
                                            QuarkFlavor::UP).getQuarkDistributionPlus())
                            + Constant::D2_ELEC_CHARGE
                                    * (m_pGPDModule->compute(
                                            GPDKinematic(x, m_xi, m_t, m_MuF2,
                                                    m_MuR2), GPDType::H).getQuarkDistribution(
                                            QuarkFlavor::DOWN).getQuarkDistributionPlus()
                                            - m_pGPDModule->compute(
                                                    GPDKinematic(m_eta, m_xi,
                                                            m_t, m_MuF2,
                                                            m_MuR2), GPDType::H).getQuarkDistribution(
                                                    QuarkFlavor::DOWN).getQuarkDistributionPlus())
                            + Constant::S2_ELEC_CHARGE
                                    * (m_pGPDModule->compute(
                                            GPDKinematic(x, m_xi, m_t, m_MuF2,
                                                    m_MuR2), GPDType::H).getQuarkDistribution(
                                            QuarkFlavor::STRANGE).getQuarkDistributionPlus()
                                            - m_pGPDModule->compute(
                                                    GPDKinematic(m_eta, m_xi,
                                                            m_t, m_MuF2,
                                                            m_MuR2), GPDType::H).getQuarkDistribution(
                                                    QuarkFlavor::STRANGE).getQuarkDistributionPlus())
                            + Constant::C2_ELEC_CHARGE
                                    * (m_pGPDModule->compute(
                                            GPDKinematic(x, m_xi, m_t, m_MuF2,
                                                    m_MuR2), GPDType::H).getQuarkDistribution(
                                            QuarkFlavor::CHARM).getQuarkDistributionPlus()
                                            - m_pGPDModule->compute(
                                                    GPDKinematic(m_eta, m_xi,
                                                            m_t, m_MuF2,
                                                            m_MuR2), GPDType::H).getQuarkDistribution(
                                                    QuarkFlavor::CHARM).getQuarkDistributionPlus())
                            + Constant::B2_ELEC_CHARGE
                                    * (m_pGPDModule->compute(
                                            GPDKinematic(x, m_xi, m_t, m_MuF2,
                                                    m_MuR2), GPDType::H).getQuarkDistribution(
                                            QuarkFlavor::BOTTOM).getQuarkDistributionPlus()
                                            - m_pGPDModule->compute(
                                                    GPDKinematic(m_eta, m_xi,
                                                            m_t, m_MuF2,
                                                            m_MuR2), GPDType::H).getQuarkDistribution(
                                                    QuarkFlavor::BOTTOM).getQuarkDistributionPlus())
                            + Constant::T2_ELEC_CHARGE
                                    * (m_pGPDModule->compute(
                                            GPDKinematic(x, m_xi, m_t, m_MuF2,
                                                    m_MuR2), GPDType::H).getQuarkDistribution(
                                            QuarkFlavor::TOP).getQuarkDistributionPlus()
                                            - m_pGPDModule->compute(
                                                    GPDKinematic(m_eta, m_xi,
                                                            m_t, m_MuF2,
                                                            m_MuR2), GPDType::H).getQuarkDistribution(
                                                    QuarkFlavor::TOP).getQuarkDistributionPlus()));

    //Return eta to its original value
    m_eta = -m_eta;

    //Analitic term to CFF for +eta (addding all flavors)
    ANALITIC_LO =
            (m_pGPDModule->compute(
                    GPDKinematic(m_eta, m_xi, m_t, m_MuF2, m_MuR2), GPDType::H).getQuarkDistribution(
                    QuarkFlavor::UP).getQuarkDistributionPlus())
                    * log(abs(1. - 1. / m_eta))
                    + (m_pGPDModule->compute(
                            GPDKinematic(m_eta, m_xi, m_t, m_MuF2, m_MuR2),
                            GPDType::H).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistributionPlus())
                            * log(abs(1. - 1. / m_eta))
                    + (m_pGPDModule->compute(
                            GPDKinematic(m_eta, m_xi, m_t, m_MuF2, m_MuR2),
                            GPDType::H).getQuarkDistribution(
                            QuarkFlavor::STRANGE).getQuarkDistributionPlus())
                            * log(abs(1. - 1. / m_eta))
                    + (m_pGPDModule->compute(
                            GPDKinematic(m_eta, m_xi, m_t, m_MuF2, m_MuR2),
                            GPDType::H).getQuarkDistribution(QuarkFlavor::CHARM).getQuarkDistributionPlus())
                            * log(abs(1. - 1. / m_eta))
                    + (m_pGPDModule->compute(
                            GPDKinematic(m_eta, m_xi, m_t, m_MuF2, m_MuR2),
                            GPDType::H).getQuarkDistribution(
                            QuarkFlavor::BOTTOM).getQuarkDistributionPlus())
                            * log(abs(1. - 1. / m_eta))
                    + (m_pGPDModule->compute(
                            GPDKinematic(m_eta, m_xi, m_t, m_MuF2, m_MuR2),
                            GPDType::H).getQuarkDistribution(QuarkFlavor::TOP).getQuarkDistributionPlus())
                            * log(abs(1. - 1. / m_eta));

    //Analitic term to CFF for -eta (addding all flavors)
    m_eta = -m_eta;
    ANALITIC_LO =
            (m_pGPDModule->compute(
                    GPDKinematic(m_eta, m_xi, m_t, m_MuF2, m_MuR2), GPDType::H).getQuarkDistribution(
                    QuarkFlavor::UP).getQuarkDistributionPlus())
                    * log(abs(1. - 1. / m_eta))
                    + (m_pGPDModule->compute(
                            GPDKinematic(m_eta, m_xi, m_t, m_MuF2, m_MuR2),
                            GPDType::H).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistributionPlus())
                            * log(abs(1. - 1. / m_eta))
                    + (m_pGPDModule->compute(
                            GPDKinematic(m_eta, m_xi, m_t, m_MuF2, m_MuR2),
                            GPDType::H).getQuarkDistribution(
                            QuarkFlavor::STRANGE).getQuarkDistributionPlus())
                            * log(abs(1. - 1. / m_eta))
                    + (m_pGPDModule->compute(
                            GPDKinematic(m_eta, m_xi, m_t, m_MuF2, m_MuR2),
                            GPDType::H).getQuarkDistribution(QuarkFlavor::CHARM).getQuarkDistributionPlus())
                            * log(abs(1. - 1. / m_eta))
                    + (m_pGPDModule->compute(
                            GPDKinematic(m_eta, m_xi, m_t, m_MuF2, m_MuR2),
                            GPDType::H).getQuarkDistribution(
                            QuarkFlavor::BOTTOM).getQuarkDistributionPlus())
                            * log(abs(1. - 1. / m_eta))
                    + (m_pGPDModule->compute(
                            GPDKinematic(m_eta, m_xi, m_t, m_MuF2, m_MuR2),
                            GPDType::H).getQuarkDistribution(QuarkFlavor::TOP).getQuarkDistributionPlus())
                            * log(abs(1. - 1. / m_eta));

    //Return eta to its original value
    m_eta = -m_eta;

    return PV_LO + ANALITIC_LO;

}

/*
 double DDVCSCFFTEST::convolution(double x, std::vector<double> params) {
 return x
 * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
 GPDType::H).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistributionPlus();
 }
 */

} /* namespace PARTONS */
