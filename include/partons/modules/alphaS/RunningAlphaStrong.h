/*---------------------------- TRunningAlphaStrong.h -------------------------*/

/*!
 * @file RunningAlphaStrong.h
 * @class RunningAlphaStrong
 * @brief C++ class evaluating the ruuning of the strong coupling constant.
 * @author H. Moutarde (Irfu/SPhN, CEA-Saclay),
 *		First version.
 * @date July 13th, 2011.
 * Last update : 17 September 2014
 *
 * Remarks :
 *
 * 1. Unit is GeV.
 * 
 * 2. Since perturbation theory is not reliable below 1 GeV, the running scale should be higher than this limit. However computations in quark models often require evolution from a very low scale, comparable to (and a little higher than) Lambda_QCD with three active flavours. Hence in this program the running scale should be greater than Lambda_QCD( Nf = 3 ).
 * 
 * 3. The program takes alpha_s( MZ ) and quark masses (c, b and t) as inputs and computes the evolution downwards to get LambdaQCD for different numbers of active flavours. Then alpha_s is computed at the required scale.
 *
 * 4. See Particle Data Group for quark masses and alpha_s(MZ) :
 *    K. Nakamura et al, J. Phys. G37 (2010) 075021.
 *
 * 5. The matching of the evaluations of the strong coupling constant with different numbers of active flavours at a given threshold is described in :
 *    - Y. Schroder and M. Steinhauser, "Four-Loop Decoupling Relations for the Strong Coupling", JHEP 0601 (2006) 051.
 *    - K.G. Chetyrkin, J.H. Kuhn and C. Sturm, "QCD Decoupling at Four Loops", Nucl. Phys. B744 (2006) 121. 
 *    CAUTION : NOT YET !!!  
 *
 * 6. The convention for the QCD beta function is the following :
 *    d alpha_s(nf)/d ln(mu^2) = Beta ( alpha_s ) = - sum_{n=1}^\infty Beta_{n-1} * ( alpha_s / pi )^(n+1)
 *
 * 7. One can vary the number of colours to work with YM SU(Nc) with Nc != 3.
 *
 *
 * TODO : 
 * 
 * 1. Matching of alpha at an heavy quark theshold after SChroder, Chetyrkin et al.
 *
 * 2. Making the verbose mode really verbose.
 *
 * 3. Today evolution is NNNLO. For consistency LO, NLO and NNLO should be available.
 *
 */

/*------------------------- Compilation of the class -------------------------*/

#ifndef RUNNING_ALPHA_STRONG_H
#define RUNNING_ALPHA_STRONG_H

#include <string>
#include <vector>

#include "../RunningAlphaStrongModule.h"

/*---------------- Definition of the class TRunningAlphaStrong ---------------*/

/*!
 * \class TRunningAlphaStrong
 * 
 * \brief Computation of the strong running coupling constant in the MSbar scheme.
 * 
 */

class RunningAlphaStrong: public RunningAlphaStrongModule {
public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    RunningAlphaStrong(const std::string &className);

    virtual RunningAlphaStrong* clone() const;

    virtual ~RunningAlphaStrong();

    virtual double compute(double Mu2);

    unsigned int GetColourNumber() const; ///< Returns number of colours (SU(Nc))

    void ResetToDefault(); ///< Back to default (PDG) values

protected:
    /**
     * Copy constructor
     *
     * @param other
     */
    RunningAlphaStrong(const RunningAlphaStrong &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    void ComputeExpansionCoefficients(unsigned int NFlavour); ///< Beta function coefficients
    void ComputeLambdaQCD();                 ///< Lambda_QCD as a function of Nf
    void Running(double Mu, double Lambda, unsigned int NFlavour); ///< Strong coupling at four loops
    double FindLambda(std::vector<double> Lambda,
            std::vector<double> Parameters); ///< Returns Lambda from alpha_s at scale Mu

    unsigned int fNc;                      ///< Number of colours (SU(Nc))

    //TODO voir s'il ne serait pas pertinent de faire une classe pour les coeff de renormalisation

    double fBeta0;               ///< 1st coefficient of beta function expansion
    double fBeta1;               ///< 2nd coefficient of beta function expansion
    double fBeta2;               ///< 3rd coefficient of beta function expansion
    double fBeta3;               ///< 4th coefficient of beta function expansion

    //

    double fB1;                         ///< Reduced coefficient fBeta1 / fBeta0
    double fB2;                         ///< Reduced coefficient fBeta2 / fBeta0
    double fB3;                         ///< Reduced coefficient fBeta3 / fBeta0
    double fLambdaQCD3;                                ///< Lambda_QCD( Nf = 3 )
    double fLambdaQCD4;                                ///< Lambda_QCD( Nf = 4 )
    double fLambdaQCD5;                                ///< Lambda_QCD( Nf = 5 )
    double fLambdaQCD6;                                ///< Lambda_QCD( Nf = 6 )
    double fAlphaSMZ;                         ///< Ref. value of strong coupling
    double fAlphaS;                        ///< Current value of strong coupling
};

#endif /* RUNNING_ALPHA_STRONG_H */

