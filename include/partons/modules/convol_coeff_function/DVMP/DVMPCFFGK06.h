#ifndef DVMP_CFF_GK06_H
#define DVMP_CFF_GK06_H

/**
 * @file DVMPCFFGK06.h
 * @author Kemal Tezgin
 * @date October 21, 2019
 * @version 1.0
 */

#include <stddef.h>
#include <complex>
#include <string>

#include "../../../beans/QuarkFlavor.h"
#include "DVMPConvolCoeffFunctionModule.h"

namespace PARTONS {

/**
 * @class DVMPCFFGK06
 *
 * This module calculates DVMP Compton Form Factors ...
 *
 * Available CFF types: H, E, Ht, Et, ...
 */
class DVMPCFFGK06: public DVMPConvolCoeffFunctionModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    DVMPCFFGK06(const std::string &className);

    virtual DVMPCFFGK06* clone() const;

    /**
     * Default destructor.
     */
    virtual ~DVMPCFFGK06();

    virtual void resolveObjectDependencies();

    friend double DVMPCFFGK06IntegrationFunctionQuark(double* x, size_t n,
            void* voidParams);
    friend double DVMPCFFGK06IntegrationFunctionGluon(double* x, size_t n,
            void* voidParams);

protected:

    /**
     * Copy constructor.
     * @param other
     */
    DVMPCFFGK06(const DVMPCFFGK06 &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual std::complex<double> computeCFF();

    /**
     * Quark unintegrated amplitude.
     * @param x Nucleon's momentum fraction.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     * @param quarkType Quark type.
     */
    double quarkUnintegratedAmplitude(double x, double tau, double b,
            QuarkFlavor::Type quarkType) const;

    /**
     * Gluon unintegrated amplitude.
     * @param x Nucleon's momentum fraction.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */
    double gluonUnintegratedAmplitude(double x, double tau, double b) const;

private:

    const double m_xbj; ///< Bjorken x.
    const double m_cNf; ///< Number of active flavors.
    const double m_cLambdaQCD; ///< Lambda QCD
    const double m_tmin;
    const double EulerGamma; /// Euler-Mascheroni Constant
    const double PositronCharge; /// Charge of the positron
    const double Nc; /// Number of colors
    const double Cf;
    const double muPi;
    const double decayConstant;
    const double transverseSize3;

    /**
     * Renormalization scale. Taken to be the largest mass scale in the hard process amplitude.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */
    double computeMuR(double tau, double b) const;

    /**
     * Running coupling.
     */
    double alphaS(double MuR) const;

    /**
     * Sudakov factor.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */
    double expSudakovFactor(double tau, double b) const;

    /**
     * Sudakov factor function s.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */
    double sudakovFactorFunctionS(double tau, double b) const;

    /**
     * Meson wave function.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */
    double mesonWF(double tau, double b) const;

    /**
     * Meson twist-2 wave function (Gaussian parameterization).
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     * @param f Decay constant (in GeV).
     * @param a Transverse size parameter (in GeV-1)
     */
    double mesonWFGaussianTwist2(double tau, double b) const;

    /**
     * Meson twist-3 wave function (Gaussian parameterization).
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     * @param f Decay constant (in GeV).
     * @param a Transverse size parameter (in GeV-1)
     */
    double mesonWFGaussianTwist3(double b) const;

    /**
     * Hankel function of the first kind.
     * @param z input of the function.
     */
    std::complex<double> HankelFunctionFirstKind(double z) const;

    /**
     * Hard scattering kernel for Pi0 production.
     * @param x Nucleon's momentum fraction.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */
    std::complex<double> hardKernelPi0(double x, double tau, double b) const;

    /**
     * Hard scattering kernel for Pi^+ production.
     * @param x Nucleon's momentum fraction.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */
    std::complex<double> hardKernelPip(double x, double tau, double b) const;

    /**
     * Gluon propagator.
     * @param x Nucleon's momentum fraction.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */

    std::complex<double> subprocessPi0Twist2(double x, double tau, double b) const;

    /**
     * Unintegrated twist-2 subprocess amplitude for Pi^0 production.
     * @param x Nucleon's momentum fraction.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */

    std::complex<double> subprocessPipTwist2(double x, double tau, double b) const;

    /**
    * Unintegrated twist-2 subprocess amplitude for Pi^+ production.
    * @param x Nucleon's momentum fraction.
    * @param tau Meson's momentum fraction.
    * @param b Impact-space parameter.
    */

    std::complex<double> subprocessPi0Twist3(double x, double tau, double b) const;

    /**
     * Unintegrated twist-3 subprocess amplitude for Pi^0 production.
     * @param x Nucleon's momentum fraction.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */

    std::complex<double> subprocessPipTwist3(double x, double tau, double b) const;

    /**
    * Unintegrated twist-3 subprocess amplitude for Pi^+ production.
    * @param x Nucleon's momentum fraction.
    * @param tau Meson's momentum fraction.
    * @param b Impact-space parameter.
    */

    double HtConvolutionPi0Re(double *xtaub, size_t dim, void *params) const;

    /**
    * The real part of the convolution, to be used in the evaluation of VEGAS Monte Carlo integration, of the GPD \tilde{H} with the subprocess amplitude in Pi^0 production.
    * @param *xtaub takes an array of variables x, tau and b.
    * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
    * @param params is the parameters to be given in the VEGAS Monte Carlo integration, set to be none.
    */

    double HtConvolutionPi0Im(double *xtaub, size_t dim, void *params) const;

    /**
    * The imaginary part of the convolution, to be used in the evaluation of VEGAS Monte Carlo integration, of the GPD \tilde{H} with the subprocess amplitude in Pi^0 production.
    * @param *xtaub takes an array of variables x, tau and b.
    * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
    * @param params is the parameters to be given in the VEGAS Monte Carlo integration, set to be none.
    */

    std::complex<double> HtConvolutionPi0(void) const;

    /**
    * Convolution of the GPD \tilde{H} with the subprocess amplitude in Pi^0 production. Evaluated with 3D VEGAS Monte Carlo, and 1D integration routines implemented in gsl library.
    */

    double EtConvolutionPi0Re(double *xtaub, size_t dim, void *params) const;

    /**
    * The real part of the convolution, to be used in the evaluation of VEGAS Monte Carlo integration, of the GPD \tilde{E} with the subprocess amplitude in Pi^0 production.
    * @param *xtaub takes an array of variables x, tau and b.
    * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
    * @param params is the parameters to be given in the VEGAS Monte Carlo integration, set to be none.
    */

    double EtConvolutionPi0Im(double *xtaub, size_t dim, void *params) const;

    /**
    * The imaginary part of the convolution, to be used in the evaluation of VEGAS Monte Carlo integration, of the GPD \tilde{E} with the subprocess amplitude in Pi^0 production.
    * @param *xtaub takes an array of variables x, tau and b.
    * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
    * @param params is the parameters to be given in the VEGAS Monte Carlo integration, set to be none.
    */

    std::complex<double> EtConvolutionPi0(void) const;

    /**
    * Convolution of the GPD \tilde{E} with the subprocess amplitude in Pi^0 production. Evaluated with 3D VEGAS Monte Carlo, and 1D integration routines implemented in gsl library.
    */

    double HTransConvolutionPi0Re(double *xtaub, size_t dim, void *params) const;

    /**
    * The real part of the convolution, to be used in the evaluation of VEGAS Monte Carlo integration, of the GPD H_T with the subprocess amplitude in Pi^0 production.
    * @param *xtaub takes an array of variables x, tau and b.
    * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
    * @param params is the parameters to be given in the VEGAS Monte Carlo integration, set to be none.
    */

    double HTransConvolutionPi0Im(double *xtaub, size_t dim, void *params) const;

    /**
    * The imaginary part of the convolution, to be used in the evaluation of VEGAS Monte Carlo integration, of the GPD H_T with the subprocess amplitude in Pi^0 production.
    * @param *xtaub takes an array of variables x, tau and b.
    * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
    * @param params is the parameters to be given in the VEGAS Monte Carlo integration, set to be none.
    */

    double HTransConvolutionPi0Analytic (double x, void * params) const;

    /**
    * Analytically computable part of the convolution of the GPD H_T with the subprocess amplitude in Pi^0 production. 3D integration is reduced to a 1D integration.
    * @param x Nucleon's momentum fraction.
    * @param params is the parameters to be given in the 1D integration, set to be none.
    */

    std::complex<double> HTransConvolutionPi0(void) const;

    /**
    * Convolution of the GPD H_T with the subprocess amplitude in Pi^0 production. Evaluated with 3D VEGAS Monte Carlo, and 1D integration routines implemented in gsl library.
    */

    double ETransConvolutionPi0Re(double *xtaub, size_t dim, void *params) const;

    /**
    * The real part of the convolution, to be used in the evaluation of VEGAS Monte Carlo integration, of the GPD \bar{E}_T with the subprocess amplitude in Pi^0 production.
    * @param *xtaub takes an array of variables x, tau and b.
    * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
    * @param params is the parameters to be given in the VEGAS Monte Carlo integration, set to be none.
    */

    double ETransConvolutionPi0Im(double *xtaub, size_t dim, void *params) const;

    /**
    * The imaginary part of the convolution, to be used in the evaluation of VEGAS Monte Carlo integration, of the GPD \bar{E}_T with the subprocess amplitude in Pi^0 production.
    * @param *xtaub takes an array of variables x, tau and b.
    * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
    * @param params is the parameters to be given in the VEGAS Monte Carlo integration, set to be none.
    */

    double ETransConvolutionPi0Analytic (double x, void * params) const;

    /**
    * Analytically computable part of the convolution of the GPD \bar{E}_T with the subprocess amplitude in Pi^0 production. 3D integration is reduced to a 1D integration.
    * @param x Nucleon's momentum fraction.
    * @param params is the parameters to be given in the 1D integration, set to be none.
    */

    std::complex<double> ETransConvolutionPi0(void) const;

    /**
    * Convolution of the GPD \bar{E}_T with the subprocess amplitude in Pi^0 production. Evaluated with 3D VEGAS Monte Carlo, and 1D integration routines implemented in gsl library.
    */

    std::complex<double> convolutionPi0Twist2(double x, double tau, double b, GPDType::Type GPDType) const;
    std::complex<double> convolutionPipTwist2(double x, double tau, double b, GPDType::Type GPDType) const;

    std::complex<double> amplitude0p0pPi0(void) const;
    std::complex<double> amplitude0m0pPi0(void) const;
    std::complex<double> amplitude0mppPi0(void) const;
    std::complex<double> amplitude0pppPi0(void) const;
    std::complex<double> amplitude0pmpPi0(void) const;
    std::complex<double> amplitude0mmpPi0(void) const;

    std::complex<double> amplitude0p0pPip(void) const;
    std::complex<double> amplitude0m0pPip(void) const;
    std::complex<double> amplitude0mppPip(void) const;
    std::complex<double> amplitude0pppPip(void) const;
    std::complex<double> amplitude0pmpPip(void) const;
    std::complex<double> amplitude0mmpPip(void) const;

    double CrossSectionLPi0(void) const;
    double CrossSectionTPi0(void) const;
    double CrossSectionLTPi0(void) const;
    double CrossSectionTTPi0(void) const;

    double CrossSectionLPip(void) const;
    double CrossSectionTPip(void) const;
    double CrossSectionLTPip(void) const;
    double CrossSectionTTPip(void) const;


    double gluonPropagator(double x, double tau, double b) const;

    /**
     * Quark unintegrated amplitude.
     * @param quarkType Quark type.
     */
    double quarkIntegratedAmplitude(QuarkFlavor::Type quarkType) const;

    /**
     * Gluon unintegrated amplitude.
     */
    double gluonIntegratedAmplitude() const;
};

} /* namespace PARTONS */

#endif /* DVMP_CFF_GK06_H */
