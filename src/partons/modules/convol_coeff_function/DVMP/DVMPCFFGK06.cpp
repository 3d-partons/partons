// Implementation of helicity amplitudes appearing in Goloskokov-Kroll (GK) model in pseudoscalar meson
// (pi+ and pi0) production.

#include "../../../../../include/partons/modules/convol_coeff_function/DVMP/DVMPCFFGK06.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
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
        DVMPConvolCoeffFunctionModule(className), m_cNf(3.), m_cLambdaQCD(0.22), m_tmin(-4. * pow(Constant::PROTON_MASS, 2.) * pow(m_xi, 2.) / (1. - pow(Constant::PROTON_MASS, 2.))),
                EulerGamma(0.577216), PositronCharge(0.3028), Nc(3.), Cf(4. / 3.) {

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
        DVMPConvolCoeffFunctionModule(other), m_cNf(other.m_cNf), m_cLambdaQCD(
                other.m_cLambdaQCD), m_tmin(other.m_tmin), EulerGamma(other.EulerGamma),
                PositronCharge(other.PositronCharge), Nc(other.Nc){
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

    //check meson
    if (m_mesonType == MesonType::RHO0) {

        std::complex<double> cff_g = gluonIntegratedAmplitude()
                + gluonIntegratedAmplitude();
        std::complex<double> cff_u = quarkIntegratedAmplitude(QuarkFlavor::UP)
                + quarkIntegratedAmplitude(QuarkFlavor::UP);
        std::complex<double> cff_d = quarkIntegratedAmplitude(QuarkFlavor::DOWN)
                + quarkIntegratedAmplitude(QuarkFlavor::DOWN);
        std::complex<double> cff_s = quarkIntegratedAmplitude(
                QuarkFlavor::STRANGE)
                + quarkIntegratedAmplitude(QuarkFlavor::STRANGE);

        //TODO implement sum from Eq. (3, 5, 6) from https://arxiv.org/pdf/hep-ph/0611290.pdf
        //TODO integration done in gluonIntegratedAmplitude() and quarkIntegratedAmplitude()

    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Meson type: "
                        << MesonType(m_mesonType).toString()
                        << " not implemented");
    }

    return 0.;
}

double DVMPCFFGK06::Heaviside(double x) const {

    if (x < 0.0)
        return 0.0;
    else
        return 1.0;

}


double DVMPCFFGK06::computeMuR(double tau, double b) const {

    double Q = sqrt(m_Q2);

    double maximum = tau * Q;

    if (1. - tau > tau)
        maximum = (1. - tau) * Q;

    if (1. / b > maximum)
        maximum = 1. / b;

    return maximum;
}


double DVMPCFFGK06::alphaS(double MuR) const {

    //TODO implementation of one loop alpha_s, variables:


    m_MuR2;

    double Q = sqrt(m_Q2);

    double coupling = (12.0 * M_PI) / ((33. - 2. * m_cNf) * log(pow(MuR,2.) / pow(m_cLambdaQCD,2.)));

    return coupling;
}

double DVMPCFFGK06::sudakovFactorFunctionS(double tau, double b) const {

    // sudakov function s is described, for example, in the appendix of https://arxiv.org/pdf/hep-ph/9503418.pdf

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

    //TODO implementation of Sudakov factor function s, like Eq. (14) from https://arxiv.org/pdf/hep-ph/0611290.pdf
}

double DVMPCFFGK06::expSudakovFactor(double tau, double b) const {

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

double DVMPCFFGK06::mesonWF(double tau, double b) const {

    //TODO define parameters and use given parameterization.

    if (m_mesonType == MesonType::RHO0) {

        double f;
        double a;

        if (m_mesonPolarization == MesonPolarization::L) {

            f = 0.216; //from Eq. (49) of https://arxiv.org/pdf/hep-ph/0501242.pdf
            a = 0.52;  //from Eq. (51) of ...
        } else if (m_mesonPolarization == MesonPolarization::T) {

            f = 0.250; //from Eq. (50) of https://arxiv.org/pdf/hep-ph/0501242.pdf
            a = 0.65;  //from Eq. (52) of ...
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "Polarization: "
                            << MesonPolarization(m_mesonPolarization).toString()
                            << " for meson: "
                            << MesonType(m_mesonType).toString()
                            << " not implemented");
        }

        // return mesonWFGaussian(tau, b, f, a);
        return 0.;

    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Meson type: "
                        << MesonType(m_mesonType).toString()
                        << " not implemented");
    }

    return 0.;
}

double DVMPCFFGK06::mesonWFGaussianTwist2(double tau, double b) const {

    double decayConstant = 0.132;

    double transverseSize2 = 1. / (8. * pow(M_PI, 2.0) * pow(decayConstant, 2.));

    double WFtwist2 = 2. * M_PI * decayConstant / sqrt(2.*Nc) * 6. * tau * (1. - tau) *
            exp(-1. * tau * (1. - tau) * pow(b, 2.0) / (4. * transverseSize2));

    return WFtwist2;

}

double DVMPCFFGK06::mesonWFGaussianTwist3(double b) const {

    double muPi = 2.0;

    double decayConstant = 0.132;

    double transverseSize3 = 1.8;

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

    // In pi^0 leptoproduction, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw2 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::Ht).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution()
            - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::Ht).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist2(xtaub[0], xtaub[1], xtaub[2]);

    return real(convolutionPi0Tw2);
}

double DVMPCFFGK06::HtConvolutionPi0Im(double *xtaub, size_t dim, void *params) const {

    // In pi^0 leptoproduction, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw2 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::Ht).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution()
            - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::Ht).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist2(xtaub[0], xtaub[1], xtaub[2]);

    return imag(convolutionPi0Tw2);
}

double DVMPCFFGK06::EtConvolutionPi0Re(double *xtaub, size_t dim, void *params) const {

    // In pi^0 leptoproduction, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw2 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::Et).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution()
            - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::Et).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist2(xtaub[0], xtaub[1], xtaub[2]);

    return real(convolutionPi0Tw2);
}

double DVMPCFFGK06::EtConvolutionPi0Im(double *xtaub, size_t dim, void *params) const {

    // In pi^0 leptoproduction, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw2 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::Et).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution()
            - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::Et).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist2(xtaub[0], xtaub[1], xtaub[2]);

    return imag(convolutionPi0Tw2);
}

double DVMPCFFGK06::HTransConvolutionPi0Re(double *xtaub, size_t dim, void *params) const {

    // In pi^0 leptoproduction, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw3 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::HTrans).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution()
            - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::HTrans).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist3(xtaub[0], xtaub[1], xtaub[2]);

    return real(convolutionPi0Tw3);
}

double DVMPCFFGK06::HTransConvolutionPi0Im(double *xtaub, size_t dim, void *params) const {

    // In pi^0 leptoproduction, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw3 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::HTrans).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution()
            - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::HTrans).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist3(xtaub[0], xtaub[1], xtaub[2]);

    return imag(convolutionPi0Tw3);
}

double DVMPCFFGK06::ETransConvolutionPi0Re(double *xtaub, size_t dim, void *params) const {

    // In pi^0 leptoproduction, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw3 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::ETrans).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution()
            - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::ETrans).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist3(xtaub[0], xtaub[1], xtaub[2]);

    return real(convolutionPi0Tw3);
}

double DVMPCFFGK06::ETransConvolutionPi0Im(double *xtaub, size_t dim, void *params) const {

    // In pi^0 leptoproduction, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw3 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::ETrans).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution()
            - Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(xtaub[0], m_xi, m_t, m_MuF2, m_MuR2), GPDType::ETrans).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist3(xtaub[0], xtaub[1], xtaub[2]);

    return imag(convolutionPi0Tw3);
}


















std::complex<double> DVMPCFFGK06::convolutionPi0Twist2(double x, double tau, double b,
        GPDType::Type GPDType) const {

    // For pi^0, GPDs appear in the combination of 1/sqrt(2) * (e^u * F^u  - e^d * F^d)

    std::complex<double> convolutionPi0Tw2 = 1. / sqrt(2.) * (Constant::U_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            GPDType).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution() -
            Constant::D_ELEC_CHARGE * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2), GPDType).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPi0Twist2(x, tau, b);

    return convolutionPi0Tw2;

}

std::complex<double> DVMPCFFGK06::subprocessPipTwist2(double x, double tau, double b) const {

    double Cf = 4. / 3.;

    std::complex<double> Ts = -1. * 1i / 4. * (gsl_sf_bessel_J0(sqrt((1. - tau) * (x - m_xi) / (2. * m_xi)) * b * sqrt(m_Q2)) + 1i * gsl_sf_bessel_Y0(sqrt((1. - tau) * (x - m_xi) / (2. * m_xi)) * b * sqrt(m_Q2))) * Heaviside(x - m_xi)
            -1. / (2. * M_PI) * gsl_sf_bessel_K0(sqrt((1. - tau) * (m_xi - x) / (2. * m_xi)) * b * sqrt(m_Q2)) * Heaviside(m_xi - x);

    std::complex<double> Tu = -1. / (2. * M_PI) * gsl_sf_bessel_K0(sqrt(tau * (x + m_xi) / (2. * m_xi)) * b * sqrt(m_Q2));

    std::complex<double> subprocessPipTw2 = Cf * sqrt(2. / Nc) * m_Q2 / m_xi * 2. * M_PI *
            b * mesonWFGaussianTwist2(tau, b) * alphaS(computeMuR(tau,b)) * expSudakovFactor(tau, b) * (Constant::U_ELEC_CHARGE * Ts - Constant::D_ELEC_CHARGE * Tu);

    return subprocessPipTw2;

}

std::complex<double> DVMPCFFGK06::convolutionPipTwist2(double x, double tau, double b,
        GPDType::Type GPDType) const {

    // For pi^+, GPDs appear in the combination of (F^u  - F^d)
    std::complex<double> convolutionPipTw2 = (m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            GPDType).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution() -
            m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2), GPDType).getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution())
            * subprocessPipTwist2(x, tau, b);

    return convolutionPipTw2;

}

std::complex<double> DVMPCFFGK06::amplitude0p0pPi0(double x, double tau, double b) const {

    // For pi^0, helicity amplitude of longitudinal polarized photon
    std::complex<double> amplitude0p0p = sqrt(1. - pow(m_xi, 2.)) * PositronCharge / sqrt(m_Q2) *
            (convolutionPi0Twist2(x, tau, b, GPDType::Ht) - pow(m_xi, 2.) / (1. - pow(m_xi, 2.)) * convolutionPi0Twist2(x, tau, b, GPDType::Et));

    return amplitude0p0p;

}

std::complex<double> DVMPCFFGK06::amplitude0p0pPip(double x, double tau, double b) const {

    // For pi^+, helicity amplitude of longitudinal polarized photon
    std::complex<double> amplitude0p0p = sqrt(1. - pow(m_xi, 2.)) * PositronCharge / sqrt(m_Q2) *
            (convolutionPipTwist2(x, tau, b, GPDType::Ht) - pow(m_xi, 2.) / (1. - pow(m_xi, 2.)) * convolutionPipTwist2(x, tau, b, GPDType::Et));

    return amplitude0p0p;
}

std::complex<double> DVMPCFFGK06::amplitude0m0pPi0(double x, double tau, double b) const {

    // For pi^0, helicity amplitude of longitudinal polarized photon
    std::complex<double> amplitude0m0p = PositronCharge / sqrt(m_Q2) * sqrt(-(m_t - m_tmin)) * m_xi / (2. * Constant::PROTON_MASS) *
            convolutionPi0Twist2(x, tau, b, GPDType::Et);

    return amplitude0m0p;

}

std::complex<double> DVMPCFFGK06::amplitude0m0pPip(double x, double tau, double b) const {

    // For pi^+, helicity amplitude of longitudinal polarized photon
    std::complex<double> amplitude0m0p = PositronCharge / sqrt(m_Q2) * sqrt(-(m_t - m_tmin)) * m_xi / (2. * Constant::PROTON_MASS) *
            convolutionPipTwist2(x, tau, b, GPDType::Et);

    return amplitude0m0p;
}

double DVMPCFFGK06::gluonPropagator(double x, double tau, double b) const {

    //TODO propagators - implement cases depending on:
    //TODO meson type (different for vector and pseudoscalar)
    //TODO (TO BE CHECKED) target helicity combination (e.g. for vector mesons: different for H and E)
    //TODO (TO BE CHECKED) meson polarization (e.g. for vector mesons: different for L and T)
    return 0.;
}

double DVMPCFFGK06::quarkUnintegratedAmplitude(double x, double tau, double b,
        QuarkFlavor::Type quarkType) const {

    //Eqs. (6, 10) from from https://arxiv.org/pdf/hep-ph/0611290.pdf
    //here for GPDs we get singlet combination, so we need to integrate between (0, 1) only.
    return mesonWF(tau, b) * alphaS(computeMuR(tau,b))
            * exp(-1. * expSudakovFactor(tau, b))
            * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
                    m_currentGPDComputeType).getQuarkDistribution(quarkType).getQuarkDistributionPlus();
}

double DVMPCFFGK06::gluonUnintegratedAmplitude(double x, double tau,
        double b) const {

    //Eqs (5, 10) from from https://arxiv.org/pdf/hep-ph/0611290.pdf
    return mesonWF(tau, b) * gluonPropagator(x, tau, b) * alphaS(computeMuR(tau,b))
            * exp(-1. * expSudakovFactor(tau, b))
            * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
                    m_currentGPDComputeType).getGluonDistribution().getGluonDistribution();
}

double DVMPCFFGK06::quarkIntegratedAmplitude(
        QuarkFlavor::Type quarkType) const {

    //parameters
    DVMPCFFGK06IntegrationParameters params;

    params.m_pDVMPCFFGK06 = this;
    params.m_quarkType = quarkType;

    //range (x, tau, b)
    //TODO max b?
    double rangeMin[3] = { 0., 0., 0. };
    double rangeMax[3] = { 1., 1., 10. };

    //result and error
    double result, error;

    //number of calls
    const size_t nCalls = 100;

    //random generator
    gsl_rng* gslRnd;
    const gsl_rng_type* gslRndType;

    gsl_rng_env_setup();

    gslRndType = gsl_rng_default;
    gslRnd = gsl_rng_alloc(gslRndType);

    //function
    gsl_monte_function gslFunction;

    gslFunction.f = &DVMPCFFGK06IntegrationFunctionQuark;
    gslFunction.dim = 3;
    gslFunction.params = &params;

    //state
    gsl_monte_vegas_state* gslState = gsl_monte_vegas_alloc(3);

    //integrate
    gsl_monte_vegas_integrate(&gslFunction, rangeMin, rangeMax, 3, nCalls,
            gslRnd, gslState, &result, &error);

    //free
    gsl_monte_vegas_free(gslState);
    gsl_rng_free(gslRnd);

    //return
    return result;
}

double DVMPCFFGK06::gluonIntegratedAmplitude() const {
    //TODO implement
    return 0.;
}

} /* namespace PARTONS */
