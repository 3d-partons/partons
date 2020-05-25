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
#include <utility>

#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/parton_distribution/PartonDistribution.h"
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

    /**
     * GSL wrapper to convolutionFunction().
     */
    static double gslWrapper0(double *xtaub, size_t dim, void *params);

    /**
     * GSL wrapper to convolutionTwist3BFunction().
     */
    static double gslWrapper1(double x, void * params);

    friend double gslWrapper0(double *xtaub, size_t dim, void *params);
    friend double gslWrapper1(double x, void * params);

protected:

    /**
     * Copy constructor.
     * @param other
     */
    DVMPCFFGK06(const DVMPCFFGK06 &other);


    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual std::complex<double> computeCFF();

    double m_tmin; ///< Minimum t value
    double m_xbj;  ///< Bjorken x

//    /**
//     * Handbag helicity amplitude \f$\mathcal{M}_{\mu'\nu',\mu\nu}\f$.
//     */
//    std::complex<double> amplitude(int mup, int nup, int mu, int nu) const;

private:

    const double m_cNf; ///< Number of active flavors.
    const double m_cLambdaQCD; ///< Lambda QCD
    const double m_EulerGamma; ///< Euler-Mascheroni Constant
    const double m_PositronCharge; ///< Charge of the positron
    const double m_Nc; ///< Number of colors
    const double m_Cf; ///< Color factor
    const double m_muPi; ///> Parameter proportional to chiral condensate, see for instance Eq. (21) in arxiv:0906.0460

    PartonDistribution m_gpdResultXiXi; ///< GPD result at x=xi.

    //*** MISC FUNCTIONS *************************************************

    /**
     * Hankel function of the first kind.
     * @param z input of the function.
     */
    std::complex<double> HankelFunctionFirstKind(double z) const;

    /**
     * Renormalization scale. Taken to be the largest mass scale in the hard process amplitude.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */
    double computeMuR(double tau, double b) const;

    //*** AMPLITUDE INGRIDIENTS ******************************************

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
     * Meson wave function (Gaussian parameterization).
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     * @param twist Twist.
     */
    double mesonWF(double tau, double b, size_t twist) const;

    /**
     * Return decay constant and transverse size.
     * @param twist Twist.
     */
    std::pair<double, double> mesonWFParameters(size_t twist) const;

    /**
     * Unintegrated subprocess amplitude.
     * @param x Nucleon's momentum fraction.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     * @param twist Twist.
     */
    std::complex<double> subProcess(double x, double tau, double b,
            size_t twist) const;

    /**
     * Get combination of GPDs as probed by the meson under consideration.
     * @param partonDistribution PartonDistribution object.
     */
    double getMesonGPDCombination(
            const PartonDistribution& partonDistribution) const;

    //*** AMPLITUDE ******************************************************

    /**
     * Evaluate convolution (twist-2).
     * @param gpdType GPD type.
     */
    std::complex<double> convolutionTwist2(GPDType::Type gpdType) const;

    /**
     * Evaluate convolution (twist-3, part A).
     * @param gpdType GPD type.
     */
    std::complex<double> convolutionTwist3A(GPDType::Type gpdType) const;

    /**
     * Evaluate convolution (twist-3, part B).
     * @param gpdType GPD type.
     */
    std::complex<double> convolutionTwist3B(GPDType::Type gpdType) const;

    /**
     * Evaluate convolution (twist-3, part C).
     * @param gpdType GPD type.
     */
    std::complex<double> convolutionTwist3C(GPDType::Type gpdType) const;

    /**
     * Evaluate convolution using 3D MC integration (any twist).
     * @param gpdType GPD type.
     * @param twist Twist.
     */
    std::complex<double> convolution(GPDType::Type gpdType, size_t twist) const;

    /**
     * Function to be used in the evaluation of VEGAS Monte Carlo integration.
     * @param *xtaub takes an array of variables x, tau and b.
     * @param dim is the dimension of VEGAS Monte Carlo integration, set to be 3.
     * @param params is the parameters to be given in the VEGAS Monte Carlo integration.
     */
    double convolutionFunction(double *xtaub, size_t dim, void *params) const;

    /**
     * Function to be used in the evaluation of 1D integration.
     * @param x Variable x.
     * @param params Parameters to be given in the integration.
     */
    double convolutionTwist3BFunction(double x, void * params) const;
};

} /* namespace PARTONS */

#endif /* DVMP_CFF_GK06_H */
