// Implementation of helicity amplitudes and partial cross sections appearing in Goloskokov-Kroll (GK) model
// in pseudoscalar meson (pi+ and pi0) production.

#include "../../../../../include/partons/modules/convol_coeff_function/DVMP/DVMPCFFGK06.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_vegas.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_sf_bessel.h>
#include <cmath>
#include <utility>

#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/MesonPolarization.h"
#include "../../../../../include/partons/beans/MesonType.h"
#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/modules/convol_coeff_function/DVMP/DVMPCFFGK06Integration.h"
#include "../../../../../include/partons/modules/gpd/GPDModule.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {

const unsigned int DVMPCFFGK06::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPCFFGK06("DVMPCFFGK06"));

DVMPCFFGK06::DVMPCFFGK06(const std::string &className) :
        DVMPConvolCoeffFunctionModule(className), m_xbj(2. * m_xi / (1. + m_xi)), m_cNf(3.), m_cLambdaQCD(0.22), m_tmin(-4. * pow(Constant::PROTON_MASS, 2.) * pow(m_xi, 2.) / (1. - pow(Constant::PROTON_MASS, 2.))),
                EulerGamma(0.577216), PositronCharge(0.3028), Nc(3.), Cf(4. / 3.), muPi(2.0), decayConstant(0.132), transverseSize3(1.8) {

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
            std::make_pair(GPDType::ETrans,
                    &DVMPConvolCoeffFunctionModule::computeCFF));
}

DVMPCFFGK06::DVMPCFFGK06(const DVMPCFFGK06 &other) :
        DVMPConvolCoeffFunctionModule(other), m_xbj(other.m_xbj), m_cNf(other.m_cNf), m_cLambdaQCD(
                other.m_cLambdaQCD), m_tmin(other.m_tmin), EulerGamma(other.EulerGamma),
                PositronCharge(other.PositronCharge), Nc(other.Nc), Cf(other.Cf), muPi(other.muPi),
                decayConstant(other.decayConstant), transverseSize3(other.transverseSize3){
}

DVMPCFFGK06* DVMPCFFGK06::clone() const {
    return new DVMPCFFGK06(*this);
}

DVMPCFFGK06::~DVMPCFFGK06() {
}

void DVMPCFFGK06::resolveObjectDependencies() {
    DVMPConvolCoeffFunctionModule::resolveObjectDependencies();
}

void DVMPCFFGK06::initModule() {
    DVMPConvolCoeffFunctionModule::initModule();
}

void DVMPCFFGK06::isModuleWellConfigured() {
    DVMPConvolCoeffFunctionModule::isModuleWellConfigured();
}

std::complex<double> DVMPCFFGK06::computeCFF() {

    //internal variables to be used in the code
    m_xi;
    m_t;
    m_Q2;
    m_MuF2;
    m_MuR2;

    m_mesonType;
    m_mesonPolarization;

    m_currentGPDComputeType;

    m_qcdOrderType;

    //check pQCD
    if (m_qcdOrderType != PerturbativeQCDOrderType::LO) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "QCD order: "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString()
                        << " not implemented");
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

double DVMPCFFGK06::alphaS(double MuR) const {

    // Running coupling constant

    double Q = sqrt(m_Q2);

    double coupling = (12.0 * M_PI) / ((33. - 2. * m_cNf) * log(pow(MuR,2.) / pow(m_cLambdaQCD,2.)));

    return coupling;
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
    double A2 = 67. / 9. - pow(M_PI, 2.) / 3. - 10. / 27. * m_cNf + 2. * beta0 / 3. * log(exp(EulerGamma) / 2.);

    if (b - sqrt(2.) / (tau * Q) <= 0.)
        sudakov =0.;
    else
        sudakov = 8. / (3. * beta0) * (qHat * log(qHat / bHat) - qHat + bHat)
                    + (4. * beta1) / (3. * pow(beta0,3.)) * (qHat * ((log(2.*qHat)+ 1.)/qHat - (log(2.*bHat)+1.0)/bHat)
                            + 1. / 2. * (pow(log(2. * qHat),2.) - pow(log(2. * bHat),2.)))
                    + 4. / (3. * beta0) * log(exp(2. * EulerGamma - 1.) / 2.) * log(qHat / bHat)
                            + A2 * 4. / pow(beta0, 2.) * ((qHat - bHat) / bHat - log(qHat / bHat));

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
    double sudakovFactor =  sudakovFactorFunctionS(tau, b) + sudakovFactorFunctionS(1. - tau, b)
            - (4. / beta0) * log(log(computeMuR(tau, b) / m_cLambdaQCD) / bHat);

    if (exp(-1. * sudakovFactor) >= 1.)
        expSudakov = 1.;
    else
        expSudakov = exp(-1. * sudakovFactor);

    return expSudakov;
}

double DVMPCFFGK06::mesonWFGaussianTwist2(double tau, double b) const {

    // Twist-2 Gaussian meson wave function

    double transverseSize2 = 1. / (8. * pow(M_PI, 2.0) * pow(decayConstant, 2.));

    double WFtwist2 = 2. * M_PI * decayConstant / sqrt(2.*Nc) * 6. * tau * (1. - tau) *
            exp(-1. * tau * (1. - tau) * pow(b, 2.0) / (4. * transverseSize2));

    return WFtwist2;

}

double DVMPCFFGK06::mesonWFGaussianTwist3(double b) const {

    // Twist-3 Gaussian meson wave function

    double WFtwist3 = 4. * M_PI * decayConstant / sqrt(2.*Nc) * muPi * pow(transverseSize3, 2.) *
            exp(-1.0 * pow(b, 2.) / (8. * pow(transverseSize3, 2.0)) * gsl_sf_bessel_In(0, pow(b, 2.) / (8. * pow(transverseSize3, 2.0))));

    return WFtwist3;

}

std::complex<double> DVMPCFFGK06::HankelFunctionFirstKind(double z) const {

    //This function defines the Hankel Function of the first kind H_0^{(1)}(z) = J_0(z) + i * Y_0(z)

    std::complex<double> Hankel0 = gsl_sf_bessel_J0(z) + 1i * gsl_sf_bessel_Y0(z);

    return Hankel0;
}

std::complex<double> DVMPCFFGK06::subprocessPi0Twist2(double x, double tau, double b) const {

    // Unintegrated twist-2 subprocess amplitude in Pi0 production.

    std::complex<double> Ts, Tu, subprocessPi0Tw2;

    if (x >= m_xi)
        Ts = -1. * 1i / 4. * (gsl_sf_bessel_J0(sqrt((1. - tau) * (x - m_xi) / (2. * m_xi)) * b * sqrt(m_Q2)) +
             1i * gsl_sf_bessel_Y0(sqrt((1. - tau) * (x - m_xi) / (2. * m_xi)) * b * sqrt(m_Q2)));
    else
        Ts = -1. / (2. * M_PI) * gsl_sf_bessel_K0(sqrt((1. - tau) * (m_xi - x) / (2. * m_xi)) * b * sqrt(m_Q2));

    Tu = -1. / (2. * M_PI) * gsl_sf_bessel_K0(sqrt(tau * (x + m_xi) / (2. * m_xi)) * b * sqrt(m_Q2));

    subprocessPi0Tw2 = Cf * sqrt(2. / Nc) * m_Q2 / m_xi * 2. * M_PI *
            b * mesonWFGaussianTwist2(tau, b) * alphaS(computeMuR(tau,b)) * expSudakovFactor(tau, b) * (Ts - Tu);

    return subprocessPi0Tw2;

}

std::complex<double> DVMPCFFGK06::subprocessPi0Twist3(double x, double tau, double b) const {

    // Unintegrated twist-3 subprocess amplitude in Pi0 production.

    std::complex<double> Ts, Tu, subprocessPi0Tw3;

    if (x >= m_xi)
        Ts = -1. * 1i / 4. * (gsl_sf_bessel_J0(sqrt((1. - tau) * (x - m_xi) / (2. * m_xi)) * b * sqrt(m_Q2)) +
             1i * gsl_sf_bessel_Y0(sqrt((1. - tau) * (x - m_xi) / (2. * m_xi)) * b * sqrt(m_Q2)));
    else
        Ts = -1. / (2. * M_PI) * gsl_sf_bessel_K0(sqrt((1. - tau) * (m_xi - x) / (2. * m_xi)) * b * sqrt(m_Q2));

    Tu = -1. / (2. * M_PI) * gsl_sf_bessel_K0(sqrt(tau * (x + m_xi) / (2. * m_xi)) * b * sqrt(m_Q2));

    subprocessPi0Tw3 = 4.0 * Cf / sqrt(2. * Nc) * m_Q2 / m_xi * 2. * M_PI *
            b * mesonWFGaussianTwist3(b) * alphaS(computeMuR(tau,b)) * expSudakovFactor(tau, b) * ((1. - tau) * Ts + tau * Tu);

    return subprocessPi0Tw3;

}

double DVMPCFFGK06::HtConvolutionPi0Re(double *xtaub, size_t dim, void *params) const {

    // Convolution of \tilde{H} with the subprocess amplitude (real part to be returned). To be used in 3D integration. In pi^0 leptoproduction, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw2 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::Ht).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution()
            - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::Ht).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist2(xtaub[0], xtaub[1], xtaub[2]);

    return real(convolutionPi0Tw2);
}

double DVMPCFFGK06::HtConvolutionPi0Im(double *xtaub, size_t dim, void *params) const {

    // Convolution of \tilde{H} with the subprocess amplitude (imaginary part to be returned). To be used in 3D integration. In pi^0 leptoproduction, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw2 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::Ht).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution()
            - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::Ht).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist2(xtaub[0], xtaub[1], xtaub[2]);

    return imag(convolutionPi0Tw2);
}

std::complex<double> DVMPCFFGK06::HtConvolutionPi0(void) const{

    // Convolution of \tilde{H} with the subprocess amplitude. VEGAS Monte-Carlo integration has been called by using the gsl library.

    double rangeMin[3] = { -m_xi, 0.0, 0.0 }; // lower bounds of the 3D integral: with respect to 1) x,  2) tau, and 3) b
    double rangeMax[3] = { 1.0, 1.0, 1.0/m_cLambdaQCD }; // upper bounds of the 3D integral: with respect to 1) x,  2) tau, and 3) b
    double resultHtRe, errorHtRe, resultHtIm, errorHtIm;

    const size_t nWarmUp = 10000; //warm-up the Monte Carlo integral
    const size_t nCalls = 100000; // number of calls of the integral

    gsl_rng* gslRndHtRe;
    const gsl_rng_type* gslRndTypeHtRe;

    gsl_rng* gslRndHtIm;
    const gsl_rng_type* gslRndTypeHtIm;

    gsl_rng_env_setup();

    gslRndTypeHtRe = gsl_rng_default;
    gslRndTypeHtIm = gsl_rng_default;

    gslRndHtRe = gsl_rng_alloc(gslRndTypeHtRe);
    gslRndHtIm = gsl_rng_alloc(gslRndTypeHtIm);

    gsl_monte_function gslFunctionHtRe = {&HtConvolutionPi0Re, 3, 0};
    gsl_monte_function gslFunctionHtIm = {&HtConvolutionPi0Im, 3, 0};

    gsl_monte_vegas_state* gslStateHtRe = gsl_monte_vegas_alloc(3);
    gsl_monte_vegas_state* gslStateHtIm = gsl_monte_vegas_alloc(3);

    //Warm-up
    gsl_monte_vegas_integrate(&gslFunctionHtRe, rangeMin, rangeMax, 3, nWarmUp, gslRndHtRe, gslStateHtRe, &resultHtRe, &errorHtRe);
    gsl_monte_vegas_integrate(&gslFunctionHtIm, rangeMin, rangeMax, 3, nWarmUp, gslRndHtIm, gslStateHtIm, &resultHtIm, &errorHtIm);

    //integrate

    do {

      gsl_monte_vegas_integrate(&gslFunctionHtRe, rangeMin, rangeMax, 3, nCalls, gslRndHtRe, gslStateHtRe, &resultHtRe, &errorHtRe);

    } while (fabs(gsl_monte_vegas_chisq (gslStateHtRe) - 1.0) > 0.5); // run VEGAS Monte-Carlo until you reach a \chi^2 value below the specified value

    do {

      gsl_monte_vegas_integrate(&gslFunctionHtIm, rangeMin, rangeMax, 3, nCalls, gslRndHtIm, gslStateHtIm, &resultHtIm, &errorHtIm);

    } while (fabs(gsl_monte_vegas_chisq (gslStateHtIm) - 1.0) > 0.5); // run VEGAS Monte-Carlo until you reach a \chi^2 value below the specified value

    //free
    gsl_monte_vegas_free(gslStateHtRe);
    gsl_monte_vegas_free(gslStateHtIm);
    gsl_rng_free(gslRndHtRe);
    gsl_rng_free(gslRndHtIm);

    std::complex<double> resultHt = resultHtRe + 1i * resultHtIm; // Add the real and imaginary part of the convolution

    return resultHt;

}

double DVMPCFFGK06::EtConvolutionPi0Re(double *xtaub, size_t dim, void *params) const {

    // Convolution of \tilde{E} with the subprocess amplitude (real part to be returned). To be used in 3D integration. In pi^0 leptoproduction, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw2 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::Et).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution()
            - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::Et).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist2(xtaub[0], xtaub[1], xtaub[2]);

    return real(convolutionPi0Tw2);
}

double DVMPCFFGK06::EtConvolutionPi0Im(double *xtaub, size_t dim, void *params) const {

    // Convolution of \tilde{E} with the subprocess amplitude (imaginary part to be returned). To be used in 3D integration. In pi^0 leptoproduction, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw2 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::Et).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution()
            - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::Et).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist2(xtaub[0], xtaub[1], xtaub[2]);

    return imag(convolutionPi0Tw2);
}

std::complex<double> DVMPCFFGK06::EtConvolutionPi0(void) const{

    // Convolution of \tilde{E} with the subprocess amplitude. VEGAS Monte-Carlo integration has been called by using the gsl library.

    double rangeMin[3] = { -m_xi, 0.0, 0.0 };
    double rangeMax[3] = { 1.0, 1.0, 1.0/m_cLambdaQCD };
    double resultEtRe, errorEtRe, resultEtIm, errorEtIm;

    const size_t nWarmUp = 10000;
    const size_t nCalls = 100000;

    gsl_rng* gslRndEtRe;
    const gsl_rng_type* gslRndTypeEtRe;

    gsl_rng* gslRndEtIm;
    const gsl_rng_type* gslRndTypeEtIm;

    gsl_rng_env_setup();

    gslRndTypeEtRe = gsl_rng_default;
    gslRndTypeEtIm = gsl_rng_default;

    gslRndEtRe = gsl_rng_alloc(gslRndTypeEtRe);
    gslRndEtIm = gsl_rng_alloc(gslRndTypeEtIm);

    gsl_monte_function gslFunctionEtRe = {&EtConvolutionPi0Re, 3, 0};
    gsl_monte_function gslFunctionEtIm = {&EtConvolutionPi0Im, 3, 0};

    gsl_monte_vegas_state* gslStateEtRe = gsl_monte_vegas_alloc(3);
    gsl_monte_vegas_state* gslStateEtIm = gsl_monte_vegas_alloc(3);

    //Warm-up
    gsl_monte_vegas_integrate(&gslFunctionEtRe, rangeMin, rangeMax, 3, nWarmUp, gslRndEtRe, gslStateEtRe, &resultEtRe, &errorEtRe);
    gsl_monte_vegas_integrate(&gslFunctionEtIm, rangeMin, rangeMax, 3, nWarmUp, gslRndEtIm, gslStateEtIm, &resultEtIm, &errorEtIm);

    //integrate

    do {

      gsl_monte_vegas_integrate(&gslFunctionEtRe, rangeMin, rangeMax, 3, nCalls, gslRndEtRe, gslStateEtRe, &resultEtRe, &errorEtRe);

    } while (fabs(gsl_monte_vegas_chisq (gslStateEtRe) - 1.0) > 0.5);

    do {

      gsl_monte_vegas_integrate(&gslFunctionEtIm, rangeMin, rangeMax, 3, nCalls, gslRndEtIm, gslStateEtIm, &resultEtIm, &errorEtIm);

    } while (fabs(gsl_monte_vegas_chisq (gslStateEtIm) - 1.0) > 0.5);

    //free
    gsl_monte_vegas_free(gslStateEtRe);
    gsl_monte_vegas_free(gslStateEtIm);
    gsl_rng_free(gslRndEtRe);
    gsl_rng_free(gslRndEtIm);

    std::complex<double> resultEt = resultEtRe + 1i * resultEtIm;

    return resultEt;

}

double DVMPCFFGK06::HTransConvolutionPi0Re(double *xtaub, size_t dim, void *params) const {

    // Convolution of H_T with the subprocess amplitude (real part to be returned). To be used in 3D integration. In pi^0 leptoproduction, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw3 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::HTrans).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution()
            - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::HTrans).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist3(xtaub[0], xtaub[1], xtaub[2]);

    return real(convolutionPi0Tw3);
}

double DVMPCFFGK06::HTransConvolutionPi0Im(double *xtaub, size_t dim, void *params) const {

    // Convolution of H_T with the subprocess amplitude (imaginary part to be returned). To be used in 3D integration. In pi^0 leptoproduction, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw3 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::HTrans).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution()
            - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::HTrans).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist3(xtaub[0], xtaub[1], xtaub[2]);

    return imag(convolutionPi0Tw3);
}

double DVMPCFFGK06::HTransConvolutionPi0Analytic (double x, void * params) const {

    // Analytically calculable part of the 3D integral. 1D integration remains after evaluation, and this integral will be calculated by using the gsl library.

    double alpha = *(double *) params;
    double convolution = 1. / (x + m_xi) * (1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2), GPDType::HTrans).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution() - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2), GPDType::HTrans).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution()))
                          - 1. / (x - m_xi) * (1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2), GPDType::HTrans).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution() - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2), GPDType::HTrans).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
                                - 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(m_xi, m_xi, m_t, m_MuF2, m_MuR2), GPDType::HTrans).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution() - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(m_xi, m_xi, m_t, m_MuF2, m_MuR2), GPDType::HTrans).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution()));

    return 16. * M_PI * Cf / Nc * alphaS(sqrt(m_Q2 / 2.)) * decayConstant * muPi * pow(transverseSize3, 2.) * convolution;
}


std::complex<double> DVMPCFFGK06::HTransConvolutionPi0(void) const{

    // Convolution of H_T with the subprocess amplitude. VEGAS Monte-Carlo integration has been called by using the gsl library.

    std::complex<double> convolutionPi0Tw3;

    std::complex<double> convolutionPi0Tw3Analytic = 16. * M_PI * Cf / Nc * alphaS(sqrt(m_Q2 / 2.)) * decayConstant * muPi
        * pow(transverseSize3, 2.) * (1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(m_xi, m_xi, m_t, m_MuF2, m_MuR2), GPDType::HTrans).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution() - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(m_xi, m_xi, m_t, m_MuF2, m_MuR2), GPDType::HTrans).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
        * (1i * M_PI - log((1.-m_xi)/(2.*m_xi)))); // First two terms of the convolution, the full analytic part. Does not need to be integrated.

    double rangeMin[3] = { -m_xi, 0.0, 0.0 };
    double rangeMax[3] = { 1.0, 1.0, 1.0/m_cLambdaQCD };
    double resultHTRe, errorHTRe, resultHTIm, errorHTIm;

    const size_t nWarmUp = 10000;
    const size_t nCalls = 100000;

    gsl_rng* gslRndHTRe;
    const gsl_rng_type* gslRndTypeHTRe;

    gsl_rng* gslRndHTIm;
    const gsl_rng_type* gslRndTypeHTIm;

    gsl_rng_env_setup();

    gslRndTypeHTRe = gsl_rng_default;
    gslRndTypeHTIm = gsl_rng_default;

    gslRndHTRe = gsl_rng_alloc(gslRndTypeHTRe);
    gslRndHTIm = gsl_rng_alloc(gslRndTypeHTIm);

    gsl_monte_function gslFunctionHTRe = {&HTransConvolutionPi0Re, 3, 0};
    gsl_monte_function gslFunctionHTIm = {&HTransConvolutionPi0Im, 3, 0};

    gsl_monte_vegas_state* gslStateHTRe = gsl_monte_vegas_alloc(3);
    gsl_monte_vegas_state* gslStateHTIm = gsl_monte_vegas_alloc(3);

    //Warm-up
    gsl_monte_vegas_integrate(&gslFunctionHTRe, rangeMin, rangeMax, 3, nWarmUp, gslRndHTRe, gslStateHTRe, &resultHTRe, &errorHTRe);
    gsl_monte_vegas_integrate(&gslFunctionHTIm, rangeMin, rangeMax, 3, nWarmUp, gslRndHTIm, gslStateHTIm, &resultHTIm, &errorHTIm);

    //integrate

    do {

      gsl_monte_vegas_integrate(&gslFunctionHTRe, rangeMin, rangeMax, 3, nCalls, gslRndHTRe, gslStateHTRe, &resultHTRe, &errorHTRe);

    } while (fabs(gsl_monte_vegas_chisq (gslStateHTRe) - 1.0) > 0.5);

    do {

      gsl_monte_vegas_integrate(&gslFunctionHTIm, rangeMin, rangeMax, 3, nCalls, gslRndHTIm, gslStateHTIm, &resultHTIm, &errorHTIm);

    } while (fabs(gsl_monte_vegas_chisq (gslStateHTIm) - 1.0) > 0.5);

    //free
    gsl_monte_vegas_free(gslStateHTRe);
    gsl_monte_vegas_free(gslStateHTIm);
    gsl_rng_free(gslRndHTRe);
    gsl_rng_free(gslRndHTIm);

    std::complex<double> resultHT = resultHTRe + 1i * resultHTIm;

    gsl_integration_workspace * w = gsl_integration_workspace_alloc (10000); // Here we start the 1D integration to evaluate the function HTransConvolutionPi0Analytic above
    double integration1D, error1D;
    double alpha = 1.0;

    gsl_function gslFunctionHT1D;
    gslFunctionHT1D.function = &HTransConvolutionPi0Analytic;
    gslFunctionHT1D.params = &alpha;

    gsl_integration_qags (&gslFunctionHT1D, -m_xi, 1.0, 0, 1e-5, 10000, w, &integration1D, &error1D);

    gsl_integration_workspace_free (w);


    convolutionPi0Tw3 = convolutionPi0Tw3Analytic + integration1D + resultHT;

    return convolutionPi0Tw3;

}

double DVMPCFFGK06::ETransConvolutionPi0Re(double *xtaub, size_t dim, void *params) const {

    // Convolution of \bar{E}_T with the subprocess amplitude (real part to be returned). To be used in 3D integration. In pi^0 leptoproduction, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw3 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::ETrans).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution()
            - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::ETrans).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist3(xtaub[0], xtaub[1], xtaub[2]);

    return real(convolutionPi0Tw3);
}

double DVMPCFFGK06::ETransConvolutionPi0Im(double *xtaub, size_t dim, void *params) const {

    // Convolution of \bar{E}_T with the subprocess amplitude (imaginary part to be returned). To be used in 3D integration. In pi^0 leptoproduction, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw3 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::ETrans).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution()
            - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::ETrans).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist3(xtaub[0], xtaub[1], xtaub[2]);

    return imag(convolutionPi0Tw3);
}

double DVMPCFFGK06::ETransConvolutionPi0Analytic (double x, void * params) const {

    // Analytically calculable part of the 3D integral. 1D integration remains after evaluation, and this integral will be calculated by using the gsl library.

    double alpha = *(double *) params;
    double convolution = 1. / (x + m_xi) * (1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2), GPDType::ETrans).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution() - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2), GPDType::ETrans).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution()))
                          - 1. / (x - m_xi) * (1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2), GPDType::ETrans).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution() - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2), GPDType::ETrans).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
                                - 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(m_xi, m_xi, m_t, m_MuF2, m_MuR2), GPDType::ETrans).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution() - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(m_xi, m_xi, m_t, m_MuF2, m_MuR2), GPDType::ETrans).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution()));

  return 16. * M_PI * Cf / Nc * alphaS(sqrt(m_Q2 / 2.)) * decayConstant * muPi * pow(transverseSize3, 2.) * convolution;
}

std::complex<double> DVMPCFFGK06::ETransConvolutionPi0(void) const{

    // Convolution of \bar{E}_T with the subprocess amplitude. VEGAS Monte-Carlo integration has been called by using the gsl library.

    std::complex<double> convolutionPi0Tw3;

    std::complex<double> convolutionPi0Tw3Analytic = 16. * M_PI * Cf / Nc * alphaS(sqrt(m_Q2 / 2.)) * decayConstant * muPi
        * pow(transverseSize3, 2.) * (1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(m_xi, m_xi, m_t, m_MuF2, m_MuR2), GPDType::ETrans).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution() - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(m_xi, m_xi, m_t, m_MuF2, m_MuR2), GPDType::ETrans).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
        * (1i * M_PI - log((1.-m_xi)/(2.*m_xi)))); // First two terms of the convolution, the full analytic part. Does not need to be integrated.

    double rangeMin[3] = { -m_xi, 0.0, 0.0 };
    double rangeMax[3] = { 1.0, 1.0, 1.0/m_cLambdaQCD };
    double resultEbarRe, errorEbarRe, resultEbarIm, errorEbarIm;

    const size_t nWarmUp = 10000;
    const size_t nCalls = 100000;

    gsl_rng* gslRndEbarRe;
    const gsl_rng_type* gslRndTypeEbarRe;

    gsl_rng* gslRndEbarIm;
    const gsl_rng_type* gslRndTypeEbarIm;

    gsl_rng_env_setup();

    gslRndTypeEbarRe = gsl_rng_default;
    gslRndTypeEbarIm = gsl_rng_default;

    gslRndEbarRe = gsl_rng_alloc(gslRndTypeEbarRe);
    gslRndEbarIm = gsl_rng_alloc(gslRndTypeEbarIm);

    gsl_monte_function gslFunctionEbarRe = {&ETransConvolutionPi0Re, 3, 0};
    gsl_monte_function gslFunctionEbarIm = {&ETransConvolutionPi0Im, 3, 0};

    gsl_monte_vegas_state* gslStateEbarRe = gsl_monte_vegas_alloc(3);
    gsl_monte_vegas_state* gslStateEbarIm = gsl_monte_vegas_alloc(3);

    //Warm-up
    gsl_monte_vegas_integrate(&gslFunctionEbarRe, rangeMin, rangeMax, 3, nWarmUp, gslRndEbarRe, gslStateEbarRe, &resultEbarRe, &errorEbarRe);
    gsl_monte_vegas_integrate(&gslFunctionEbarIm, rangeMin, rangeMax, 3, nWarmUp, gslRndEbarIm, gslStateEbarIm, &resultEbarIm, &errorEbarIm);

    //integrate

    do {

      gsl_monte_vegas_integrate(&gslFunctionEbarRe, rangeMin, rangeMax, 3, nCalls, gslRndEbarRe, gslStateEbarRe, &resultEbarRe, &errorEbarRe);

    } while (fabs(gsl_monte_vegas_chisq (gslStateEbarRe) - 1.0) > 0.5);

    do {

      gsl_monte_vegas_integrate(&gslFunctionEbarIm, rangeMin, rangeMax, 3, nCalls, gslRndEbarIm, gslStateEbarIm, &resultEbarIm, &errorEbarIm);

    } while (fabs(gsl_monte_vegas_chisq (gslStateEbarIm) - 1.0) > 0.5);

    //free
    gsl_monte_vegas_free(gslStateEbarRe);
    gsl_monte_vegas_free(gslStateEbarIm);
    gsl_rng_free(gslRndEbarRe);
    gsl_rng_free(gslRndEbarIm);

    std::complex<double> resultEbar = resultEbarRe + 1i * resultEbarIm;

    gsl_integration_workspace * w = gsl_integration_workspace_alloc (10000); // Here we start the 1D integration to evaluate the function ETransConvolutionPi0Analytic above
    double integration1D, error1D;
    double alpha = 1.0;

    gsl_function gslFunctionEbar1D;
    gslFunctionEbar1D.function = &ETransConvolutionPi0Analytic;
    gslFunctionEbar1D.params = &alpha;

    gsl_integration_qags (&gslFunctionEbar1D, -m_xi, 1.0, 0, 1e-5, 10000,
                        w, &integration1D, &error1D);

    gsl_integration_workspace_free (w);


    convolutionPi0Tw3 = convolutionPi0Tw3Analytic + integration1D + resultEbar;

    return convolutionPi0Tw3;

}

std::complex<double> DVMPCFFGK06::amplitude0p0pPi0(void) const {

    // The helicity amplitude \mathcal{M}_{0+,0+} in pi0 leptoproduction (handbag contribution only). See Eq. (2) in arxiv:1106.4897

    std::complex<double> amplitude0p0p = sqrt(1. - pow(m_xi, 2.)) * PositronCharge / sqrt(m_Q2) *
            (HtConvolutionPi0() - pow(m_xi, 2.) / (1. - pow(m_xi, 2.)) * EtConvolutionPi0());

    return amplitude0p0p;

}

std::complex<double> DVMPCFFGK06::amplitude0m0pPi0(void) const {

    // The helicity amplitude \mathcal{M}_{0-,0+} in pi0 leptoproduction (handbag contribution only). See Eq. (2) in arxiv:1106.4897

    std::complex<double> amplitude0m0p = PositronCharge / sqrt(m_Q2) * sqrt(-(m_t - m_tmin)) * m_xi / (2. * Constant::PROTON_MASS) *
            EtConvolutionPi0();

    return amplitude0m0p;

}

std::complex<double> DVMPCFFGK06::amplitude0mppPi0(void) const {

    // The helicity amplitude \mathcal{M}_{0-,++} in pi0 leptoproduction (handbag contribution only). See Eq. (8) in arxiv:1106.4897

    std::complex<double> amplitude0mpp = PositronCharge * sqrt(1. - pow(m_xi, 2.)) * HTransConvolutionPi0();

    return amplitude0mpp;
}

std::complex<double> DVMPCFFGK06::amplitude0pppPi0(void) const {

    // The helicity amplitude \mathcal{M}_{0+,++} in pi0 leptoproduction (handbag contribution only). See Eq. (11) in arxiv:1106.4897

    std::complex<double> amplitude0ppp = -1.0 * PositronCharge * sqrt(-(m_t - m_tmin)) / (4. * Constant::PROTON_MASS) * ETransConvolutionPi0();

    return amplitude0ppp;
}

std::complex<double> DVMPCFFGK06::amplitude0pmpPi0(void) const {

    // The helicity amplitude \mathcal{M}_{0+,-+} in pi0 leptoproduction (handbag contribution only). See Eq. (11) in arxiv:1106.4897

    std::complex<double> amplitude0pmp = -1.0 * PositronCharge * sqrt(-(m_t - m_tmin)) / (4. * Constant::PROTON_MASS) * ETransConvolutionPi0();

    return amplitude0pmp;
}

std::complex<double> DVMPCFFGK06::amplitude0mmpPi0(void) const {

    // The helicity amplitude \mathcal{M}_{0-,-+} in pi0 leptoproduction which is zero.

    std::complex<double> amplitude0mmp = 0.0;

    return amplitude0mmp;
}

double DVMPCFFGK06::CrossSectionLPi0(void) const {

    // Longitudinal partial cross section. See Eq. (43) in arxiv:0906.0460

    double W = sqrt(m_Q2 / m_xbj + pow(Constant::PROTON_MASS, 2.0) - m_Q2);

    double factor = 0.3894 * pow(10.0, 6.0) / (16.0 * M_PI * (pow(W, 2.0) - pow(Constant::PROTON_MASS, 2.0)) *
                        sqrt(pow(W, 4.0) + pow(m_Q2, 2.0) + pow(Constant::PROTON_MASS, 4.0) + 2.0 * pow(W, 2.0) * m_Q2
                             - 2.0 * pow(W, 2.0) * pow(Constant::PROTON_MASS, 2.0) + 2.0 * m_Q2 * pow(Constant::PROTON_MASS, 2.0))); //converted to nanobarn

    double CSL = (pow(abs(amplitude0p0pPi0()), 2.) + pow(abs(amplitude0m0pPi0()), 2.)) * factor;

    return CSL;
}

double DVMPCFFGK06::CrossSectionTPi0(void) const {

    // Transverse partial cross section. See Eq. (43) in arxiv:0906.0460

    double W = sqrt(m_Q2 / m_xbj + pow(Constant::PROTON_MASS, 2.0) - m_Q2);

    double factor = 0.3894 * pow(10.0, 6.0) / (16.0 * M_PI * (pow(W, 2.0) - pow(Constant::PROTON_MASS, 2.0)) *
                        sqrt(pow(W, 4.0) + pow(m_Q2, 2.0) + pow(Constant::PROTON_MASS, 4.0) + 2.0 * pow(W, 2.0) * m_Q2
                             - 2.0 * pow(W, 2.0) * pow(Constant::PROTON_MASS, 2.0) + 2.0 * m_Q2 * pow(Constant::PROTON_MASS, 2.0))); //converted to nanobarn

    double CST = (pow(abs(amplitude0mppPi0()), 2.) + pow(abs(amplitude0mmpPi0()), 2.) + pow(abs(amplitude0pppPi0()), 2.) + pow(abs(amplitude0pmpPi0()), 2.)) * factor / 2.;

    return CST;
}

double DVMPCFFGK06::CrossSectionLTPi0(void) const {

    // Partial cross section of the interference part LT. See Eq. (43) in arxiv:0906.0460

    double W = sqrt(m_Q2 / m_xbj + pow(Constant::PROTON_MASS, 2.0) - m_Q2);

    double factor = 0.3894 * pow(10.0, 6.0) / (16.0 * M_PI * (pow(W, 2.0) - pow(Constant::PROTON_MASS, 2.0)) *
                        sqrt(pow(W, 4.0) + pow(m_Q2, 2.0) + pow(Constant::PROTON_MASS, 4.0) + 2.0 * pow(W, 2.0) * m_Q2
                             - 2.0 * pow(W, 2.0) * pow(Constant::PROTON_MASS, 2.0) + 2.0 * m_Q2 * pow(Constant::PROTON_MASS, 2.0))); //converted to nanobarn

    double CSLT = -1.0 * real(conj(amplitude0m0pPi0()) * (amplitude0mppPi0() - amplitude0mmpPi0()) + conj(amplitude0p0pPi0()) * (amplitude0pppPi0() - amplitude0pmpPi0())) * factor / sqrt(2.);

    return CSLT;
}

double DVMPCFFGK06::CrossSectionTTPi0(void) const {

    // Partial cross section of the interference part TT. See Eq. (43) in arxiv:0906.0460

    double W = sqrt(m_Q2 / m_xbj + pow(Constant::PROTON_MASS, 2.0) - m_Q2);

    double factor = 0.3894 * pow(10.0, 6.0) / (16.0 * M_PI * (pow(W, 2.0) - pow(Constant::PROTON_MASS, 2.0)) *
                        sqrt(pow(W, 4.0) + pow(m_Q2, 2.0) + pow(Constant::PROTON_MASS, 4.0) + 2.0 * pow(W, 2.0) * m_Q2
                             - 2.0 * pow(W, 2.0) * pow(Constant::PROTON_MASS, 2.0) + 2.0 * m_Q2 * pow(Constant::PROTON_MASS, 2.0))); //converted to nanobarn

    double CSTT = -1.0 * real(conj(amplitude0mppPi0()) * amplitude0mmpPi0() + conj(amplitude0pppPi0()) * amplitude0pmpPi0()) * factor;

    return CSTT;
}



} /* namespace PARTONS */
