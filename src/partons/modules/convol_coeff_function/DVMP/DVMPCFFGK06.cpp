// Implementation of helicity amplitudes and partial cross sections appearing in Goloskokov-Kroll (GK) model
// in pseudoscalar meson (pi+ and pi0) production.

#include "../../../../../include/partons/modules/convol_coeff_function/DVMP/DVMPCFFGK06.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_vegas.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_sf_bessel.h>
#include <cmath>

#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/beans/MesonType.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/convol_coeff_function/DVMP/DVMPCFFGK06IntegrationParameters.h"
#include "../../../../../include/partons/modules/gpd/GPDModule.h"
#include "../../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongGK.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"

namespace PARTONS {

const unsigned int DVMPCFFGK06::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPCFFGK06("DVMPCFFGK06"));

const std::string DVMPCFFGK06::PARAMETER_NAME_DVMPCFFGK06_MC_NWARMUP =
        "nWarmUps";
const std::string DVMPCFFGK06::PARAMETER_NAME_DVMPCFFGK06_MC_NCALLS = "nCalls";
const std::string DVMPCFFGK06::PARAMETER_NAME_DVMPCFFGK06_MC_CHI2LIMIT =
        "chi2Limit";

double DVMPCFFGK06::gslWrapper0(double *xtaub, size_t dim, void *params) {
    return (static_cast<DVMPCFFGK06IntegrationParameters*>(params))->m_pDVMPCFFGK06->convolutionFunction(
            xtaub, dim, params);
}

double DVMPCFFGK06::gslWrapper1(double x, void * params) {
    return (static_cast<DVMPCFFGK06IntegrationParameters*>(params))->m_pDVMPCFFGK06->convolutionTwist3BFunction(
            x, params);
}

DVMPCFFGK06::DVMPCFFGK06(const std::string &className) :
        DVMPConvolCoeffFunctionModule(className), m_cNf(3.), m_cLambdaQCD(0.22), m_EulerGamma(
                0.577216), m_PositronCharge(0.3028), m_Nc(3.), m_Cf(4. / 3.), m_muPi(
                2.0), m_tmin(0.), m_pRunningAlphaStrongModule(0), m_MCNWarmUp(
                2E4), m_MCCalls(2E5), m_MCChi2Limit(0.8) {

    //relate GPD types with functions to be used
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Ht,
                    &DVMPConvolCoeffFunctionModule::computeCFF));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Et,
                    &DVMPConvolCoeffFunctionModule::computeCFF));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::HTrans,
                    &DVMPConvolCoeffFunctionModule::computeCFF));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::EbarTrans,
                    &DVMPConvolCoeffFunctionModule::computeCFF));
}

DVMPCFFGK06::DVMPCFFGK06(const DVMPCFFGK06 &other) :
        DVMPConvolCoeffFunctionModule(other), m_cNf(other.m_cNf), m_cLambdaQCD(
                other.m_cLambdaQCD), m_EulerGamma(other.m_EulerGamma), m_PositronCharge(
                other.m_PositronCharge), m_Nc(other.m_Nc), m_Cf(other.m_Cf), m_muPi(
                other.m_muPi), m_tmin(other.m_tmin), m_MCNWarmUp(
                other.m_MCNWarmUp), m_MCCalls(other.m_MCCalls), m_MCChi2Limit(
                other.m_MCChi2Limit) {

    //clone alpaS module
    if (other.m_pRunningAlphaStrongModule != 0) {
        m_pRunningAlphaStrongModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pRunningAlphaStrongModule);
    } else {
        m_pRunningAlphaStrongModule = 0;
    }
}

DVMPCFFGK06* DVMPCFFGK06::clone() const {
    return new DVMPCFFGK06(*this);
}

DVMPCFFGK06::~DVMPCFFGK06() {

    // destroy alphaS module
    if (m_pRunningAlphaStrongModule != 0) {
        setRunningAlphaStrongModule(0);
        m_pRunningAlphaStrongModule = 0;
    }
}

void DVMPCFFGK06::configure(const ElemUtils::Parameters &parameters) {

    //run for mother
    DVMPConvolCoeffFunctionModule::configure(parameters);

    if (parameters.isAvailable(
            DVMPCFFGK06::PARAMETER_NAME_DVMPCFFGK06_MC_NWARMUP)) {
        setMcnWarmUp(parameters.getLastAvailable().toUInt());
    }

    if (parameters.isAvailable(
            DVMPCFFGK06::PARAMETER_NAME_DVMPCFFGK06_MC_NCALLS)) {
        setMcCalls(parameters.getLastAvailable().toUInt());
    }

    if (parameters.isAvailable(
            DVMPCFFGK06::PARAMETER_NAME_DVMPCFFGK06_MC_CHI2LIMIT)) {
        setMcChi2Limit(parameters.getLastAvailable().toDouble());
    }
}

void DVMPCFFGK06::resolveObjectDependencies() {

    //run for mother
    DVMPConvolCoeffFunctionModule::resolveObjectDependencies();

    //set alpha_s module
    m_pRunningAlphaStrongModule =
            Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                    RunningAlphaStrongGK::classId);
}

void DVMPCFFGK06::initModule() {

    //run for mother
    DVMPConvolCoeffFunctionModule::initModule();

    //evaluate tmin
    m_tmin = -4. * pow(Constant::PROTON_MASS, 2.) * pow(m_xi, 2.)
            / (1. - pow(m_xi, 2.));
}

void DVMPCFFGK06::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    DVMPConvolCoeffFunctionModule::prepareSubModules(subModulesData);

    std::map<std::string, BaseObjectData>::const_iterator it;

    it = subModulesData.find(
            RunningAlphaStrongModule::RUNNING_ALPHA_STRONG_MODULE_CLASS_NAME);

    if (it != subModulesData.end()) {

        if (m_pRunningAlphaStrongModule != 0) {
            setRunningAlphaStrongModule(0);
            m_pRunningAlphaStrongModule = 0;
        }

        if (!m_pRunningAlphaStrongModule) {
            m_pRunningAlphaStrongModule =
                    Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                            (it->second).getModuleClassName());
            info(__func__,
                    ElemUtils::Formatter()
                            << "Configure with RunningAlphaStrongModule = "
                            << m_pRunningAlphaStrongModule->getClassName());
            m_pRunningAlphaStrongModule->configure(
                    (it->second).getParameters());
        }
    }
}

RunningAlphaStrongModule* DVMPCFFGK06::getRunningAlphaStrongModule() const {
    return m_pRunningAlphaStrongModule;
}

void DVMPCFFGK06::setRunningAlphaStrongModule(
        RunningAlphaStrongModule* pRunningAlphaStrongModule) {
    m_pModuleObjectFactory->updateModulePointerReference(
            m_pRunningAlphaStrongModule, pRunningAlphaStrongModule);
    m_pRunningAlphaStrongModule = pRunningAlphaStrongModule;
}

void DVMPCFFGK06::isModuleWellConfigured() {
    DVMPConvolCoeffFunctionModule::isModuleWellConfigured();
}

std::complex<double> DVMPCFFGK06::computeCFF() {

    //check pQCD
    if (m_qcdOrderType != PerturbativeQCDOrderType::LO) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "QCD order: "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString()
                        << " not implemented");
    }

    //evaluate GPD at xixi line
    m_gpdResultXiXi = m_pGPDModule->compute(
            GPDKinematic(m_xi, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    //GPD
    std::complex<double> convolution;

    //switch over mesons
    switch (m_currentGPDComputeType) {

    //Ht
    case GPDType::Ht: {

        convolution = convolutionTwist2(m_currentGPDComputeType);

        return m_PositronCharge * sqrt(1. - pow(m_xi, 2.)) / sqrt(m_Q2)
                * convolution;
    }

        break;

        //Et
    case GPDType::Et: {

        convolution = convolutionTwist2(m_currentGPDComputeType);

        return m_PositronCharge * sqrt(1. - pow(m_xi, 2.)) / sqrt(m_Q2)
                * (-pow(m_xi, 2.) / (1. - pow(m_xi, 2.)) * convolution)
                + m_PositronCharge / sqrt(m_Q2) * sqrt(-(m_t - m_tmin)) * m_xi
                        / (2. * Constant::PROTON_MASS) * convolution;
    }

        break;

        //HTrans
    case GPDType::HTrans: {

        convolution = convolutionTwist3A(m_currentGPDComputeType)
                + convolutionTwist3B(m_currentGPDComputeType)
                + convolutionTwist3C(m_currentGPDComputeType);

        return m_PositronCharge * sqrt(1. - pow(m_xi, 2.)) * convolution;
    }

        break;

        //EbarTrans
    case GPDType::EbarTrans: {

        convolution = convolutionTwist3A(m_currentGPDComputeType)
                + convolutionTwist3B(m_currentGPDComputeType)
                + convolutionTwist3C(m_currentGPDComputeType);

        return -1.0 * m_PositronCharge * sqrt(-(m_t - m_tmin))
                / (4. * Constant::PROTON_MASS) * convolution;
    }

        break;

    default: {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No implementation for CFF "
                        << GPDType(m_currentGPDComputeType).toString());
    }

        break;
    }

    return 0.;
}

double DVMPCFFGK06::computeMuR(double tau, double b) const {

    // Computation of the renormalization scale. See the paragraph below Eq. (13) in hep-ph/0611290

    double Q = sqrt(m_Q2);

    double maximum = tau * Q;

    if (1. - tau > tau)
        maximum = (1. - tau) * Q;

    if (1. / b > maximum)
        maximum = 1. / b;

    return maximum;
}

double DVMPCFFGK06::sudakovFactorFunctionS(double tau, double b) const {

    // Sudakov function s is described, for example, in the appendix of https://arxiv.org/pdf/hep-ph/9503418.pdf

    double Q = sqrt(m_Q2);

    double sudakov;

    //beta0 factor
    double beta0 = 11. - 2. * m_cNf / 3.;

    //beta1 factor
    double beta1 = 102. - 38. * m_cNf / 3.;

    //b^
    double bHat = -1. * log(b * m_cLambdaQCD);

    //q^
    double qHat = log(tau * Q / (sqrt(2.) * m_cLambdaQCD));

    //A^(2) factor
    double A2 = 67. / 9. - pow(M_PI, 2.) / 3. - 10. / 27. * m_cNf
            + 2. * beta0 / 3. * log(exp(m_EulerGamma) / 2.);

    if (b - sqrt(2.) / (tau * Q) <= 0.)
        sudakov = 0.;
    else
        sudakov = 8. / (3. * beta0) * (qHat * log(qHat / bHat) - qHat + bHat)
                + (4. * beta1) / (3. * pow(beta0, 3.))
                        * (qHat
                                * ((log(2. * qHat) + 1.) / qHat
                                        - (log(2. * bHat) + 1.0) / bHat)
                                + 1. / 2.
                                        * (pow(log(2. * qHat), 2.)
                                                - pow(log(2. * bHat), 2.)))
                + 4. / (3. * beta0) * log(exp(2. * m_EulerGamma - 1.) / 2.)
                        * log(qHat / bHat)
                + A2 * 4. / pow(beta0, 2.)
                        * ((qHat - bHat) / bHat - log(qHat / bHat));

    return sudakov;

}

double DVMPCFFGK06::expSudakovFactor(double tau, double b) const {

    // Computation of the Sudakov exponent. See, for instance, Eq. (12) and the footnote on the same page in hep-ph/0611290

    //sqrt of Q2
    double Q = sqrt(m_Q2);

    double expSudakov;

    //beta0 factor
    double beta0 = 11. - 2. * m_cNf / 3.;

    //b^
    double bHat = -1. * log(b * m_cLambdaQCD);

    //Eq. (12) from https://arxiv.org/pdf/hep-ph/0611290.pdf
    double sudakovFactor = sudakovFactorFunctionS(tau, b)
            + sudakovFactorFunctionS(1. - tau, b)
            - (4. / beta0) * log(log(computeMuR(tau, b) / m_cLambdaQCD) / bHat);

    if (exp(-1. * sudakovFactor) >= 1.) {
        expSudakov = 1.;
    } else {
        expSudakov = exp(-1. * sudakovFactor);
    }

    return expSudakov;
}

std::pair<double, double> DVMPCFFGK06::mesonWFParameters(size_t twist) const {

    //variables
    double transverseSize;
    double decayConstant;

    //switch over mesons
    switch (m_mesonType) {

    //pi0, pi+
    case MesonType::PI0:
    case MesonType::PIPLUS: {

        //twist 2
        if (twist == 2) {
            return std::pair<double, double>(0.132,
                    1. / (8. * pow(M_PI, 2.0) * pow(0.132, 2.)));
        }
        //twist 3
        else if (twist == 3) {
            return std::pair<double, double>(0.132, 1.8);
        }
        //???
        else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "Twist " << twist
                            << " not implemented here for meson "
                            << MesonType(m_mesonType).toString());
        }
    }

        break;

        //???
    default: {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No implementation for meson: "
                        << MesonType(m_mesonType).toString());
    }

        break;
    }

    return std::pair<double, double>(0., 0.);
}

double DVMPCFFGK06::mesonWF(double tau, double b, size_t twist) const {

    //get parameters
    std::pair<double, double> par = mesonWFParameters(twist);

    //twist 2
    if (twist == 2) {

        return 2. * M_PI * par.first / sqrt(2. * m_Nc) * 6. * tau * (1. - tau)
                * exp(-1. * tau * (1. - tau) * pow(b, 2.0) / (4. * par.second));
    }
    //twist 3
    else if (twist == 3) {

        return 4. * M_PI * par.first / sqrt(2. * m_Nc) * m_muPi
                * pow(par.second, 2.)
                * exp(
                        -1.0 * pow(b, 2.) / (8. * pow(par.second, 2.0))
                                * gsl_sf_bessel_In(0,
                                        pow(b, 2.)
                                                / (8. * pow(par.second, 2.0))));
    }
    //???
    else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Twist " << twist
                        << " not implemented here");
    }

    return 0.;
}

double DVMPCFFGK06::getMesonGPDCombination(
        const PartonDistribution& partonDistribution) const {

    //switch over mesons
    switch (m_mesonType) {

    //pi0
    case MesonType::PI0: {
        return 1. / sqrt(2.)
                * (Constant::U_ELEC_CHARGE
                        * partonDistribution.getQuarkDistribution(
                                QuarkFlavor::UP).getQuarkDistributionPlus()
                        - Constant::D_ELEC_CHARGE
                                * partonDistribution.getQuarkDistribution(
                                        QuarkFlavor::DOWN).getQuarkDistributionPlus());
    }

        break;

        //pi+
    case MesonType::PIPLUS: {
        return 1. / sqrt(2.)
                * (Constant::U_ELEC_CHARGE
                        * partonDistribution.getQuarkDistribution(
                                QuarkFlavor::UP).getQuarkDistributionPlus()
                        - Constant::D_ELEC_CHARGE
                                * partonDistribution.getQuarkDistribution(
                                        QuarkFlavor::DOWN).getQuarkDistributionPlus());
    }

        break;

        //???
    default: {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No implementation for meson: "
                        << MesonType(m_mesonType).toString());
    }

        break;
    }

    return 0.;
}

std::complex<double> DVMPCFFGK06::subProcess(double x, double tau, double b,
        size_t twist) const {

    //switch over mesons
    switch (m_mesonType) {

    //pi0 pi+
    case MesonType::PI0:
    case MesonType::PIPLUS: {

        std::complex<double> Ts, Tu;

        //twist 2
        if (twist == 2) {

            if (x >= m_xi) {
                Ts = -1. * std::complex<double>(0., 1.) / 4.
                        * (gsl_sf_bessel_J0(
                                sqrt((1. - tau) * (x - m_xi) / (2. * m_xi)) * b
                                        * sqrt(m_Q2))
                                + std::complex<double>(0., 1.)
                                        * gsl_sf_bessel_Y0(
                                                sqrt(
                                                        (1. - tau) * (x - m_xi)
                                                                / (2. * m_xi))
                                                        * b * sqrt(m_Q2)));
            } else {
                Ts = -1. / (2. * M_PI)
                        * gsl_sf_bessel_K0(
                                sqrt((1. - tau) * (m_xi - x) / (2. * m_xi)) * b
                                        * sqrt(m_Q2));
            }

            Tu = -1. / (2. * M_PI)
                    * gsl_sf_bessel_K0(
                            sqrt(tau * (x + m_xi) / (2. * m_xi)) * b
                                    * sqrt(m_Q2));

            return m_Cf * sqrt(2. / m_Nc) * m_Q2 / m_xi * 2. * M_PI * b
                    * mesonWF(tau, b, twist)
                    * m_pRunningAlphaStrongModule->compute(
                            pow(computeMuR(tau, b), 2))
                    * expSudakovFactor(tau, b) * (Ts - Tu);
        }
        //twist 3
        else if (twist == 3) {

            if (x >= m_xi) {
                Ts = -1. * std::complex<double>(0., 1.) / 4.
                        * (gsl_sf_bessel_J0(
                                sqrt((1. - tau) * (x - m_xi) / (2. * m_xi)) * b
                                        * sqrt(m_Q2))
                                + std::complex<double>(0., 1.)
                                        * gsl_sf_bessel_Y0(
                                                sqrt(
                                                        (1. - tau) * (x - m_xi)
                                                                / (2. * m_xi))
                                                        * b * sqrt(m_Q2)));
            } else {
                Ts = -1. / (2. * M_PI)
                        * gsl_sf_bessel_K0(
                                sqrt((1. - tau) * (m_xi - x) / (2. * m_xi)) * b
                                        * sqrt(m_Q2));
            }

            Tu = -1. / (2. * M_PI)
                    * gsl_sf_bessel_K0(
                            sqrt(tau * (x + m_xi) / (2. * m_xi)) * b
                                    * sqrt(m_Q2));

            return 4.0 * m_Cf / sqrt(2. * m_Nc) * m_Q2 / m_xi * 2. * M_PI * b
                    * mesonWF(tau, b, twist)
                    * m_pRunningAlphaStrongModule->compute(
                            pow(computeMuR(tau, b), 2))
                    * expSudakovFactor(tau, b) * ((1. - tau) * Ts + tau * Tu);

        }
        //???
        else {

            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "Twist " << twist
                            << " not implemented here for meson "
                            << MesonType(m_mesonType).toString());
        }
    }

        break;

        //???
    default: {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No implementation for meson: "
                        << MesonType(m_mesonType).toString());
    }

        break;
    }

    return std::complex<double>(0., 0.);
}

double DVMPCFFGK06::convolutionFunction(double *xtaub, size_t dim,
        void *params) const {

    //parameters
    DVMPCFFGK06IntegrationParameters* p =
            static_cast<DVMPCFFGK06IntegrationParameters*>(params);

    //evaluate sub-process
    std::complex<double> subProcessResult = subProcess(xtaub[0], xtaub[1],
            xtaub[2], p->m_twist);

    //result depending on the meson type
    double mesonGPDCombination = getMesonGPDCombination(
            m_pGPDModule->compute(
                    GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2),
                    p->m_gpdType));

    //return
    std::complex<double> convolution = mesonGPDCombination * subProcessResult;

    return (p->m_isReal) ? (convolution.real()) : (convolution.imag());
}

std::complex<double> DVMPCFFGK06::convolution(GPDType::Type gpdType,
        size_t twist) const {

    double resultRe[2], errorRe[2], resultIm[2], errorIm[2]; //results and errors

    double rangeMin[3] = { 0., 0.0, 0.0 }; // lower bounds of the 3D integral: with respect to 1) x,  2) tau, and 3) b
    double rangeMax[3] = { 1.0, 1.0, 1.0 / m_cLambdaQCD }; // upper bounds of the 3D integral: with respect to 1) x,  2) tau, and 3) b

    gsl_rng_env_setup(); //random generator

    const gsl_rng_type* gslRndType = gsl_rng_default;
    gsl_rng* gslRnd = gsl_rng_alloc(gslRndType);

    DVMPCFFGK06IntegrationParameters dvmpCFFGK06IntegrationParameters; // parameters
    dvmpCFFGK06IntegrationParameters.m_pDVMPCFFGK06 = this;
    dvmpCFFGK06IntegrationParameters.m_gpdType = gpdType;
    dvmpCFFGK06IntegrationParameters.m_twist = twist;

    gsl_monte_function gslFunction = { &gslWrapper0, 3,
            &dvmpCFFGK06IntegrationParameters }; //set function

    gsl_monte_vegas_state* gslState; // gsl state

    //Re
    dvmpCFFGK06IntegrationParameters.m_isReal = true;

    for (size_t i = 0; i < 2; i++) {

        //range
        rangeMin[0] = (i == 0) ? (0.) : (m_xi);
        rangeMax[0] = (i == 0) ? (m_xi) : (1.);

        //state
        gslState = gsl_monte_vegas_alloc(3);

        //warm-up
        gsl_monte_vegas_integrate(&gslFunction, rangeMin, rangeMax, 3,
                m_MCNWarmUp, gslRnd, gslState, &resultRe[i], &errorRe[i]);

        info(__func__,
                ElemUtils::Formatter() << "i: " << i << " (initialization) Re "
                        << GPDType(m_currentGPDComputeType).toString()
                        << " result: " << resultRe[i] << " error: "
                        << errorRe[i]);

        //integrate
        do {

            if (resultRe[i] == 0. && errorRe[i] == 0.)
                break;

            gsl_monte_vegas_integrate(&gslFunction, rangeMin, rangeMax, 3,
                    m_MCCalls, gslRnd, gslState, &resultRe[i], &errorRe[i]);

            info(__func__,
                    ElemUtils::Formatter() << "i: " << i
                            << " (loop) Re: result: " << resultRe[i]
                            << " error: " << errorRe[i]);

        } while (fabs(gsl_monte_vegas_chisq(gslState) - 1.0) > m_MCChi2Limit); // run VEGAS Monte-Carlo until you reach a \chi^2 value below the specified value

        info(__func__,
                ElemUtils::Formatter() << "i: " << i << " (final) Re "
                        << GPDType(m_currentGPDComputeType).toString()
                        << " result: " << resultRe[i] << " error: "
                        << errorRe[i]);

        //Re free state
        gsl_monte_vegas_free(gslState);
    }

    //Im parameters
    dvmpCFFGK06IntegrationParameters.m_isReal = false;

    for (size_t i = 0; i < 2; i++) {

        //range
        rangeMin[0] = (i == 0) ? (0.) : (m_xi);
        rangeMax[0] = (i == 0) ? (m_xi) : (1.);

        //state
        gslState = gsl_monte_vegas_alloc(3);

        //Im warm-up
        gsl_monte_vegas_integrate(&gslFunction, rangeMin, rangeMax, 3,
                m_MCNWarmUp, gslRnd, gslState, &resultIm[i], &errorIm[i]);

        info(__func__,
                ElemUtils::Formatter() << "i: " << i << " (initialization) Im "
                        << GPDType(m_currentGPDComputeType).toString()
                        << " result: " << resultIm[i] << " error: "
                        << errorIm[i]);

        //Im integrate
        do {

            if (resultIm[i] == 0. && errorIm[i] == 0.)
                break;

            gsl_monte_vegas_integrate(&gslFunction, rangeMin, rangeMax, 3,
                    m_MCCalls, gslRnd, gslState, &resultIm[i], &errorIm[i]);

            info(__func__,
                    ElemUtils::Formatter() << "i: " << i
                            << " (loop) Im: result: " << resultIm[i]
                            << " error: " << errorIm[i]);

        } while (fabs(gsl_monte_vegas_chisq(gslState) - 1.0) > m_MCChi2Limit); // run VEGAS Monte-Carlo until you reach a \chi^2 value below the specified value

        info(__func__,
                ElemUtils::Formatter() << "i: " << i << " (final) Im "
                        << GPDType(m_currentGPDComputeType).toString()
                        << " result: " << resultIm[i] << " error: "
                        << errorIm[i]);

        //Im free state
        gsl_monte_vegas_free(gslState);
    }

    // Free
    gsl_rng_free(gslRnd);

    // Return
    return (resultRe[0] + resultRe[1])
            + std::complex<double>(0., 1.) * (resultIm[0] + resultIm[1]);
}

std::complex<double> DVMPCFFGK06::convolutionTwist2(
        GPDType::Type gpdType) const {
    return convolution(gpdType, 2);
}

std::complex<double> DVMPCFFGK06::convolutionTwist3A(
        GPDType::Type gpdType) const {
    return convolution(gpdType, 3);
}

double DVMPCFFGK06::convolutionTwist3BFunction(double x, void * params) const {

    //cast parameters
    DVMPCFFGK06IntegrationParameters* p =
            static_cast<DVMPCFFGK06IntegrationParameters*>(params);

    //get WF parameters
    std::pair<double, double> par = mesonWFParameters(3);

    //GPD
    PartonDistribution gpdResult = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2), p->m_gpdType);

    //evaluate
    double convolution = 1. / (x + m_xi) * (getMesonGPDCombination(gpdResult))
            - 1.
                    * (getMesonGPDCombination(gpdResult)
                            - getMesonGPDCombination(m_gpdResultXiXi))
                    / (x - m_xi);

    return 16. * M_PI * m_Cf / m_Nc
            * m_pRunningAlphaStrongModule->compute(m_Q2 / 2.) * par.first
            * m_muPi * pow(par.second, 2.) * convolution;
}

std::complex<double> DVMPCFFGK06::convolutionTwist3B(
        GPDType::Type gpdType) const {

    gsl_integration_workspace* w; // workspace

    double result[2], error[2]; //result

    DVMPCFFGK06IntegrationParameters dvmpCFFGK06IntegrationParameters; // parameters
    dvmpCFFGK06IntegrationParameters.m_pDVMPCFFGK06 = this;
    dvmpCFFGK06IntegrationParameters.m_gpdType = gpdType;

    gsl_function gslFunction; // gsl function
    gslFunction.function = &gslWrapper1;
    gslFunction.params = &dvmpCFFGK06IntegrationParameters;

    for (size_t i = 0; i < 2; i++) {

        w = gsl_integration_workspace_alloc(10000); //workspace

        gsl_integration_qag(&gslFunction, (i == 0) ? (0.) : (m_xi),
                (i == 0) ? (m_xi) : (1.), 0.0, GSL_INTEG_GAUSS61, 1e-5, 10000,
                w, &result[i], &error[i]); //evaluate

        gsl_integration_workspace_free(w); //free
    }

    return std::complex<double>(result[0] + result[1], 0.);
}

std::complex<double> DVMPCFFGK06::convolutionTwist3C(
        GPDType::Type gpdType) const {

    //get WF parameters
    std::pair<double, double> par = mesonWFParameters(3);

    return 16. * M_PI * m_Cf / m_Nc
            * m_pRunningAlphaStrongModule->compute(m_Q2 / 2.) * par.first
            * m_muPi * pow(par.second, 2.)
            * (getMesonGPDCombination(m_gpdResultXiXi)
                    * (std::complex<double>(0., 1.) * M_PI
                            - log((1. - m_xi) / (2. * m_xi))));
}

std::complex<double> DVMPCFFGK06::HankelFunctionFirstKind(double z) const {

    //This function defines the Hankel Function of the first kind H_0^{(1)}(z) = J_0(z) + i * Y_0(z)

    std::complex<double> Hankel0 = gsl_sf_bessel_J0(z)
            + std::complex<double>(0., 1.) * gsl_sf_bessel_Y0(z);

    return Hankel0;
}

size_t DVMPCFFGK06::getMcCalls() const {
    return m_MCCalls;
}

void DVMPCFFGK06::setMcCalls(size_t mcCalls) {
    info(__func__,
            ElemUtils::Formatter() << "MC number of calls changed from "
                    << m_MCCalls << " to " << mcCalls);
    m_MCCalls = mcCalls;
}

double DVMPCFFGK06::getMcChi2Limit() const {
    return m_MCChi2Limit;
}

void DVMPCFFGK06::setMcChi2Limit(double mcChi2Limit) {
    info(__func__,
            ElemUtils::Formatter() << "MC chi2 limit changed from "
                    << m_MCChi2Limit << " to " << mcChi2Limit);
    m_MCChi2Limit = mcChi2Limit;
}

size_t DVMPCFFGK06::getMcnWarmUp() const {
    return m_MCNWarmUp;
}

void DVMPCFFGK06::setMcnWarmUp(size_t mcnWarmUp) {
    info(__func__,
            ElemUtils::Formatter()
                    << "MC number of calls (warm-up) changed from "
                    << m_MCNWarmUp << " to " << mcnWarmUp);
    m_MCNWarmUp = mcnWarmUp;
}

} /* namespace PARTONS */

