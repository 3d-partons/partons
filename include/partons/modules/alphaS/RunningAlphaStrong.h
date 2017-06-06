/**
 * @file RunningAlphaStrong.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 13th, 2011.
 * @version 1.0
 */

#ifndef RUNNING_ALPHA_STRONG_H
#define RUNNING_ALPHA_STRONG_H

#include <string>
#include <vector>

#include "../RunningAlphaStrongModule.h"

/*!
 * \class RunningAlphaStrong
 * 
 * \brief Evaluation of the strong running coupling constant in the \f$\overline{MS}\f$ scheme.
 *
 * This module evaluates the strong running coupling constant in the \f$\overline{MS}\f$ scheme.
 * It takes \f$\alpha(M_Z)\f$ and quark masses (c, b and t) as inputs and computes the evolution downwards to get \f$\Lambda_{QCD}\f$ for different numbers of active flavors. Then \f$\alpha(M_Z)\f$ is computed at the required scale.
 *
 * For an example of usage of this module see the abstract class documentation.
 * 
 * Note 1: Quark masses and \f$\alpha(M_Z)\f$ are defined in FundamentalPhysicalConstants.h. These values come from Ref. @cite Nakamura:2010zzi
 *
 * Note 2: The convention for the QCD beta function is the following: <br>
 * \f$\frac{d \alpha_{s}(n_{f})}{d \ln(\mu^2)} = \beta ( \alpha_{s} ) = - \sum_{n=1}^\infty \beta_{n-1} \times ( \alpha_{s} / \pi )^{(n+1)}\f$
 */
class RunningAlphaStrong: public RunningAlphaStrongModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    RunningAlphaStrong(const std::string &className);

    virtual RunningAlphaStrong* clone() const;

    /**
     * Destructor.
     */
    virtual ~RunningAlphaStrong();

    virtual double compute();

    /**
     * Get number of colors (SU(\f$n_{c}\f$))
     */
    unsigned int GetColourNumber() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    RunningAlphaStrong(const RunningAlphaStrong &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    /**
     * Evaluate beta function coefficients for a fixed number of flavors.
     * \f$n_{c}\f$-independent expressions (not used) are given in @cite Chetyrkin:1997un.<br>
     * \f$n_{c}\f$-dependent expressions (used) are given in @cite Czakon:2004bu.
     * @param NFlavour Number of flavors.
     */
    void ComputeExpansionCoefficients(unsigned int NFlavour);

    /**
     *  Evaluate \f$\Lambda_{QCD}\f$ for different number of flavors.
     */
    void ComputeLambdaQCD();

    /**
     *  Evaluate \f$\alpha_s\f$ at four loops.
     */
    void Running(double Mu, double Lambda, unsigned int NFlavour);

    /**
     *  Returns \f$\Lambda_{QCD}\f$ from \f$\alpha_s\f$ at given scale.
     */
    double FindLambda(double Lambda, std::vector<double> Parameters);

    /**
     * Number of colors (SU(\f$n_{c}\f$)).
     */
    unsigned int fNc;

    //TODO voir s'il ne serait pas pertinent de faire une classe pour les coeff de renormalisation

    double fBeta0;              ///< 1st coefficient of beta function expansion.
    double fBeta1;              ///< 2nd coefficient of beta function expansion.
    double fBeta2;              ///< 3rd coefficient of beta function expansion.
    double fBeta3;              ///< 4th coefficient of beta function expansion.

    double fB1; ///< Reduced coefficient RunningAlphaStrong::fBeta1 / RunningAlphaStrong::fBeta0.
    double fB2; ///< Reduced coefficient RunningAlphaStrong::fBeta2 / RunningAlphaStrong::fBeta0.
    double fB3; ///< Reduced coefficient RunningAlphaStrong::fBeta3 / RunningAlphaStrong::fBeta0.
    double fLambdaQCD3;                     ///< \f$\Lambda_{QCD}(n_{f} = 3)\f$.
    double fLambdaQCD4;                     ///< \f$\Lambda_{QCD}(n_{f} = 4)\f$.
    double fLambdaQCD5;                     ///< \f$\Lambda_{QCD}(n_{f} = 5)\f$.
    double fLambdaQCD6;                     ///< \f$\Lambda_{QCD}(n_{f} = 6)\f$.
    double fAlphaSMZ;                   ///< Reference value of strong coupling.
    double fAlphaS;                       ///< Current value of strong coupling.
};

#endif /* RUNNING_ALPHA_STRONG_H */

