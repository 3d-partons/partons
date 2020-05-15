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
 * This module calculates helicity amplitudes and partial cross sections of exclusive pi0 and pi+
 *   leptoproduction formulated in Goloskokov-Kroll model
 *
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
    const double m_tmin; /// Minimum t value
    const double EulerGamma; /// Euler-Mascheroni Constant
    const double PositronCharge; /// Charge of the positron
    const double Nc; /// Number of colors
    const double Cf; /// Color factor
    const double muPi; /// a parameter proportional to chiral condensate, see for instance Eq. (21) in arxiv:0906.0460
    const double decayConstant; /// Pion decay constant
    const double transverseSize3; /// Twist-3 pion transverse size

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
     * Unintegrated twist-2 subprocess amplitude for Pi^0 production.
     * @param x Nucleon's momentum fraction.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */

    std::complex<double> subprocessPi0Twist2(double x, double tau, double b) const;

    /**
    * Unintegrated twist-2 subprocess amplitude for Pi^+ production.
    * @param x Nucleon's momentum fraction.
    * @param tau Meson's momentum fraction.
    * @param b Impact-space parameter.
    */

    std::complex<double> subprocessPipTwist2(double x, double tau, double b) const;

    /**
     * Unintegrated twist-3 subprocess amplitude for Pi^0 production.
     * @param x Nucleon's momentum fraction.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */

    std::complex<double> subprocessPi0Twist3(double x, double tau, double b) const;

    /**
    * Unintegrated twist-3 subprocess amplitude for Pi^+ production.
    * @param x Nucleon's momentum fraction.
    * @param tau Meson's momentum fraction.
    * @param b Impact-space parameter.
    */

    std::complex<double> subprocessPipTwist3(double x, double tau, double b) const;

    /**
    * The real part of the convolution, to be used in the evaluation of VEGAS Monte Carlo integration, of the GPD \tilde{H} with the subprocess amplitude in Pi^0 production.
    * @param *xtaub takes an array of variables x, tau and b.
    * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
    * @param params is the parameters to be given in the VEGAS Monte Carlo integration, set to be none.
    */

    double HtConvolutionPi0Re(double *xtaub, size_t dim, void *params) const;

    /**
    * The imaginary part of the convolution, to be used in the evaluation of VEGAS Monte Carlo integration, of the GPD \tilde{H} with the subprocess amplitude in Pi^0 production.
    * @param *xtaub takes an array of variables x, tau and b.
    * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
    * @param params is the parameters to be given in the VEGAS Monte Carlo integration, set to be none.
    */

    double HtConvolutionPi0Im(double *xtaub, size_t dim, void *params) const;

    /**
    * Convolution of the GPD \tilde{H} with the subprocess amplitude in Pi^0 production. Evaluated with 3D VEGAS Monte Carlo, and 1D integration routines implemented by using gsl library.
    */

    std::complex<double> HtConvolutionPi0(void) const;

    /**
    * The real part of the convolution, to be used in the evaluation of VEGAS Monte Carlo integration, of the GPD \tilde{E} with the subprocess amplitude in Pi^0 production.
    * @param *xtaub takes an array of variables x, tau and b.
    * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
    * @param params is the parameters to be given in the VEGAS Monte Carlo integration, set to be none.
    */

    double EtConvolutionPi0Re(double *xtaub, size_t dim, void *params) const;

    /**
    * The imaginary part of the convolution, to be used in the evaluation of VEGAS Monte Carlo integration, of the GPD \tilde{E} with the subprocess amplitude in Pi^0 production.
    * @param *xtaub takes an array of variables x, tau and b.
    * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
    * @param params is the parameters to be given in the VEGAS Monte Carlo integration, set to be none.
    */

    double EtConvolutionPi0Im(double *xtaub, size_t dim, void *params) const;

    /**
    * Convolution of the GPD \tilde{E} with the subprocess amplitude in Pi^0 production. Evaluated with 3D VEGAS Monte Carlo, and 1D integration routines implemented by using gsl library.
    */

    std::complex<double> EtConvolutionPi0(void) const;

    /**
    * The real part of the convolution, to be used in the evaluation of VEGAS Monte Carlo integration, of the GPD H_T with the subprocess amplitude in Pi^0 production.
    * @param *xtaub takes an array of variables x, tau and b.
    * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
    * @param params is the parameters to be given in the VEGAS Monte Carlo integration, set to be none.
    */

    double HTransConvolutionPi0Re(double *xtaub, size_t dim, void *params) const;

    /**
    * The imaginary part of the convolution, to be used in the evaluation of VEGAS Monte Carlo integration, of the GPD H_T with the subprocess amplitude in Pi^0 production.
    * @param *xtaub takes an array of variables x, tau and b.
    * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
    * @param params is the parameters to be given in the VEGAS Monte Carlo integration, set to be none.
    */

    double HTransConvolutionPi0Im(double *xtaub, size_t dim, void *params) const;

    /**
    * Analytically computable part of the convolution of the GPD H_T with the subprocess amplitude in Pi^0 production. 3D integration is reduced to a 1D integration.
    * @param x Nucleon's momentum fraction.
    * @param params is the parameters to be given in the 1D integration, set to be none.
    */

    double HTransConvolutionPi0Analytic (double x, void * params) const;

    /**
    * Convolution of the GPD H_T with the subprocess amplitude in Pi^0 production. Evaluated with 3D VEGAS Monte Carlo, and 1D integration routines implemented by using gsl library.
    */

    std::complex<double> HTransConvolutionPi0(void) const;

    /**
    * The real part of the convolution, to be used in the evaluation of VEGAS Monte Carlo integration, of the GPD \bar{E}_T with the subprocess amplitude in Pi^0 production.
    * @param *xtaub takes an array of variables x, tau and b.
    * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
    * @param params is the parameters to be given in the VEGAS Monte Carlo integration, set to be none.
    */

    double ETransConvolutionPi0Re(double *xtaub, size_t dim, void *params) const;

    /**
    * The imaginary part of the convolution, to be used in the evaluation of VEGAS Monte Carlo integration, of the GPD \bar{E}_T with the subprocess amplitude in Pi^0 production.
    * @param *xtaub takes an array of variables x, tau and b.
    * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
    * @param params is the parameters to be given in the VEGAS Monte Carlo integration, set to be none.
    */

    double ETransConvolutionPi0Im(double *xtaub, size_t dim, void *params) const;

    /**
    * Analytically computable part of the convolution of the GPD \bar{E}_T with the subprocess amplitude in Pi^0 production. 3D integration is reduced to a 1D integration.
    * @param x Nucleon's momentum fraction.
    * @param params is the parameters to be given in the 1D integration, set to be none.
    */

    double ETransConvolutionPi0Analytic (double x, void * params) const;

    /**
    * Convolution of the GPD \bar{E}_T with the subprocess amplitude in Pi^0 production. Evaluated with 3D VEGAS Monte Carlo, and 1D integration routines implemented by using gsl library.
    */

    std::complex<double> ETransConvolutionPi0(void) const;

    /**
    * Pi0 handbag helicity amplitude \mathcal{M}_{0+,0+}
    */
    std::complex<double> amplitude0p0pPi0(void) const;

    /**
    * Pi0 handbag helicity amplitude \mathcal{M}_{0-,0+}
    */
    std::complex<double> amplitude0m0pPi0(void) const;

    /**
    * Pi0 handbag helicity amplitude \mathcal{M}_{0-,++}
    */
    std::complex<double> amplitude0mppPi0(void) const;

    /**
    * Pi0 handbag helicity amplitude \mathcal{M}_{0+,++}
    */
    std::complex<double> amplitude0pppPi0(void) const;

    /**
    * Pi0 handbag helicity amplitude \mathcal{M}_{0+,-+}
    */
    std::complex<double> amplitude0pmpPi0(void) const;

    /**
    * Pi0 handbag helicity amplitude \mathcal{M}_{0-,-+}
    */
    std::complex<double> amplitude0mmpPi0(void) const;

    /**
    * Pi+ handbag helicity amplitude \mathcal{M}_{0+,0+}
    */
    std::complex<double> amplitude0p0pPip(void) const;

    /**
    * Pi+ handbag helicity amplitude \mathcal{M}_{0-,0+}
    */
    std::complex<double> amplitude0m0pPip(void) const;

    /**
    * Pi+ handbag helicity amplitude \mathcal{M}_{0-,++}
    */
    std::complex<double> amplitude0mppPip(void) const;

    /**
    * Pi+ handbag helicity amplitude \mathcal{M}_{0+,++}
    */
    std::complex<double> amplitude0pppPip(void) const;

    /**
    * Pi+ handbag helicity amplitude \mathcal{M}_{0+,-+}
    */
    std::complex<double> amplitude0pmpPip(void) const;

    /**
    * Pi+ handbag helicity amplitude \mathcal{M}_{0-,-+}
    */
    std::complex<double> amplitude0mmpPip(void) const;

    /**
    * Pi0 partial longitidunal cross section L
    */
    double CrossSectionLPi0(void) const;

    /**
    * Pi0 partial transverse cross section T
    */
    double CrossSectionTPi0(void) const;

    /**
    * Pi0 partial interference cross section LT
    */
    double CrossSectionLTPi0(void) const;

    /**
    * Pi0 partial interference cross section TT
    */
    double CrossSectionTTPi0(void) const;

    /**
    * Pi+ partial longitidunal cross section L
    */
    double CrossSectionLPip(void) const;

    /**
    * Pi+ partial transverse cross section T
    */
    double CrossSectionTPip(void) const;

    /**
    * Pi+ partial interference cross section LT
    */
    double CrossSectionLTPip(void) const;

    /**
    * Pi+ partial interference cross section TT
    */
    double CrossSectionTTPip(void) const;

};

} /* namespace PARTONS */

#endif /* DVMP_CFF_GK06_H */
