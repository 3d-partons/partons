/*--------------------------- RunningAlphaStrong.cxx ------------------------*/

/*!
 * \file RunningAlphaStrong.cxx
 * \class RunningAlphaStrong
 * \brief C++ class evaluating the ruuning of the strong coupling constant.
 * \author H. Moutarde (Irfu/SPhN, CEA-Saclay),
 *		First version.
 * \date July 13th, 2011.
 * 
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
 * 1. Matching of alpha at an heavy quark theshold after Schroder, Chetyrkin et al.
 *
 * 2. Making the verbose mode really verbose.
 *
 * 3. Today evolution is NNNLO. For consistency LO, NLO and NNLO should be available.
 *
 */

/*-------------------------------- Includes ----------------------------------*/

#include "RunningAlphaStrong.h"

#include <Math/BrentRootFinder.h>
#include <Math/WrappedTF1.h>
#include <TF1.h>
#include <cmath>
#include <cstdlib>
#include <stdexcept>

#include "../../FundamentalPhysicalConstants.h"
#include "../../ModuleObjectFactory.h"

// Initialise [class]::moduleID with a unique name.
const std::string RunningAlphaStrong::moduleID =
        ModuleObjectFactory::getInstance()->registerModule(
                new RunningAlphaStrong("RunningAlphaStrong"));

/*------------------------------- Public routines ----------------------------*/

/*!
 * \fn RunningAlphaStrong::RunningAlphaStrong( string pQCD, bool Verbose )
 * 
 * Default constructor.
 * 
 */
RunningAlphaStrong::RunningAlphaStrong(const std::string &className)
        : RunningAlphaStrongModule(className), fNc(3), fBeta0(2.08333), fBeta1(
                3.20833), fBeta2(6.34925), fBeta3(31.3874), fB1(1.54), fB2(
                3.04764), fB3(15.066), fLambdaQCD3(0.329939), fLambdaQCD4(
                0.28914), fLambdaQCD5(0.208364), fLambdaQCD6(0.0878108), fAlphaSMZ(
                0.1184), fAlphaS(0.303061) {
}

//TODO implement
RunningAlphaStrong::RunningAlphaStrong(const RunningAlphaStrong &other)
        : RunningAlphaStrongModule(other) {

}

RunningAlphaStrong* RunningAlphaStrong::clone() const {
    return new RunningAlphaStrong(*this);
}

/**
 * 
 * Default destructor.
 *
 */
RunningAlphaStrong::~RunningAlphaStrong() {
}

//TODO implement
void RunningAlphaStrong::initModule() {

}

//TODO implement
void RunningAlphaStrong::isModuleWellConfigured() {

}

//TODO implement - Voir a remplacer ça par un arbre binaire pour les tests si pertinent
double RunningAlphaStrong::compute(double Mu) {
    m_Mu = Mu;

    if (QUARK_STRANGE_MASS <= m_Mu && m_Mu < QUARK_CHARM_MASS) {
        m_nf = 3;
    }

    else if (QUARK_CHARM_MASS <= m_Mu && m_Mu < QUARK_BOTTOM_MASS) {
        m_nf = 4;
    }

    else if (QUARK_BOTTOM_MASS <= m_Mu && m_Mu < QUARK_TOP_MASS) {
        m_nf = 5;
    }

    else if (QUARK_TOP_MASS <= m_Mu) {
        m_nf = 6;
    }

    ComputeLambdaQCD();

    double Lambda = 0.; // Lambda_QCD with actual number of active flavours
    // Finds the actual value of Lambda_QCD

    switch (m_nf) {

    case 3:
        Lambda = fLambdaQCD3;
        break;

    case 4:
        Lambda = fLambdaQCD4;
        break;

    case 5:
        Lambda = fLambdaQCD5;
        break;

    case 6:
        Lambda = fLambdaQCD6;
        break;

    default:
        //TODO implement
        throw std::runtime_error("RunningAlphaStrong::compute(Mu)");
    }

    Running(m_Mu, Lambda, m_nf);

    return fAlphaS;
}

/*!
 * \fn GetColourNumber ()
 * 
 * General SU(Nc) theory even if using experimental input values (Alpha_S(M_Z), quark masses) becomes dubious if Nc != 3.
 *
 */
unsigned int RunningAlphaStrong::GetColourNumber() const {
    return fNc;
}

/*!
 * \fn ResetToDefault()
 *
 * Strong coupling at M_Z, quark masses, beta function coefficients, Lambda_QCD and running scale take their default values.
 *
 */
void RunningAlphaStrong::ResetToDefault() {
    fBeta0 = 2.25;
    fBeta1 = 4.;
    fBeta2 = 10.0599;
    fBeta3 = 47.228;

    fB1 = 1.77778;
    fB2 = 4.47106;
    fB3 = 20.9902;

    fLambdaQCD3 = 0.373;
    fLambdaQCD4 = 0.305;
    fLambdaQCD5 = 0.205;
    fLambdaQCD6 = 0.100;

    fAlphaS = 1.;
    fAlphaSMZ = 0.1184;

    m_nf = 4;
}

/*------------------------------ Private routines ----------------------------*/

/*!
 * \fn ComputeExpansionCoefficients( unsigned int NFlavour )
 * 
 * Computes the coefficient of the expansion of the QCD beta function. 
 * 
 * Nc-independent expressions (comments) are given in K.G. Chetyrkin et al., "Decoupling relations to O(AlphaS^3) 
 * and their connection to low-energy theorems", Nucl. Phys. B 510 (1998) 61, 
 * arXiv:hep-ph/9708255.   
 *
 * Nc-dependent expressions (used) are given in M. Czakon, "The four-loop QCD beta function and anomalous dimensions",
 * Nucl. Phys. B710 (2005) 485, arXiv:hep-ph/0411261.                 
 * 
 */
void RunningAlphaStrong::ComputeExpansionCoefficients(unsigned int NFlavour) {
    double Zeta3 = ZETA_3;
    double NFlavour2 = NFlavour * NFlavour;
    double NFlavour3 = NFlavour2 * NFlavour;

    double CA = fNc; // Casimir operator, ajoint representation
    double NC2 = fNc * fNc;
    double NA = NC2 - 1.; // Dimension of adjoint representation
    double CF = NA / (2. * fNc); // Casimir operator, fundamental representation
    double TF = 0.5; // Fundamental representation generator
    double CA2 = CA * CA;
    double CA3 = CA2 * CA;
    double CA4 = CA3 * CA;
    double CF2 = CF * CF;
    double CF3 = CF2 * CF;
    double TF2 = TF * TF;
    double TF3 = TF2 * TF;

    if (NFlavour != 3 && NFlavour != 4 && NFlavour != 5 && NFlavour != 6) {
//        cout
//                << "RunningAlphaStrong : Erroneous input in RunningAlphaStrong::ComputeExpansionCoefficients."
//                << endl;
//        cout
//                << "RunningAlphaStrong : Number of active flavours has to be an integer between 3 and 6."
//                << endl;
//        cout << "RunningAlphaStrong : Here NFlavour = " << NFlavour << endl;
//        cout << endl;
        exit(-1);
    }

//   fBeta0 = 1./4. * ( 11. - 2. / 3. * NFlavour );
    fBeta0 = 11. / 3. * CA - 4. / 3. * TF * NFlavour;
    fBeta0 /= 4.;

//   fBeta1 = 1./16. * ( 102. - 38. / 3. * NFlavour );
    fBeta1 = 34. / 3. * CA2 - 20. / 3. * CA * TF * NFlavour
            - 4. * CF * TF * NFlavour;
    fBeta1 /= 16.;

//   fBeta2 = 1./64. * ( 2857. / 2. - 5033. / 18. * NFlavour + 325. / 54. * NFlavour2 );	
    fBeta2 = 2857. / 54. * CA3 - 1415. / 27. * CA2 * TF * NFlavour
            + 158. / 27. * CA * TF2 * NFlavour2;
    fBeta2 += 44. / 9. * CF * TF2 * NFlavour2
            - 205. / 9. * CF * CA * TF * NFlavour + 2. * CF2 * TF * NFlavour;
    fBeta2 /= 64.;

//   fBeta3 = 1./256. * ( 149753. / 6. + 3564 * Zeta3 + ( - 1078361. / 162. - 6508. / 27. * Zeta3 ) * NFlavour + ( 50065. / 162. + 6472. / 81. * Zeta3 ) * NFlavour2 + 1093. / 729. * NFlavour3 );
    fBeta3 = CA * CF * TF2 * NFlavour2 * (17152. / 243. + 448. / 9. * Zeta3);
    fBeta3 += CA * CF2 * TF * NFlavour * (-4204. / 27. + 352. / 9. * Zeta3);
    fBeta3 += 424. / 243. * CA * TF3 * NFlavour3
            + CA2 * CF * TF * NFlavour * (7073. / 243. - 656. / 9. * Zeta3);
    fBeta3 += CA2 * TF2 * NFlavour2 * (7930. / 81. + 224. / 9. * Zeta3)
            + 1232. / 243. * CF * TF3 * NFlavour3;
    fBeta3 += CA3 * TF * NFlavour * (-39143. / 81. + 136. / 3. * Zeta3)
            + CA4 * (150653. / 486. - 44. / 9. * Zeta3);
    fBeta3 += CF2 * TF2 * NFlavour2 * (1352. / 27. - 704. / 9. * Zeta3)
            + 46. * CF3 * TF * NFlavour;
    fBeta3 += NFlavour * fNc * (NC2 + 6.) / 48.
            * (512. / 9. - 1664. / 3. * Zeta3);
    fBeta3 += NFlavour2 * (NC2 * NC2 - 6. * NC2 + 18.) / (96. * NC2)
            * (-704. / 9. + 512. / 3. * Zeta3);
    fBeta3 += NC2 * (NC2 + 36.) / 24. * (-80. / 9. + 704. / 3. * Zeta3);
    fBeta3 /= 256.;

    fB1 = fBeta1 / fBeta0;
    fB2 = fBeta2 / fBeta0;
    fB3 = fBeta3 / fBeta0;
}

/*!
 * \fn ComputeLambdaQCD()
 *
 * Computes Lambda_QCD as a function of the number of active flavours. 
 * Starts from the value of Alpha_S at MZ and runs downwards to strange quark threshold to get Lambda_QCD for Nf = 2, ... 5. then runs forwards to top quark threshold to get Lambda_QCD for Nf = 6.
 *
 * TODO : clean matching of Chetyrkin for heavy flavours.
 *
 */
void RunningAlphaStrong::ComputeLambdaQCD() {
    double LambdaMin = 0.05; // From current knowledge of LambdaQCD in MSbar scheme
    double LambdaMax = 0.5; // Idem
    double AlphaTarget; // Solve at scale mu Running( mu, LambdaQCD(Nf), Nf ) == AlphaTarget

    ROOT::Math::WrappedTF1* WrappedLambdaFinder;

    TF1 LambdaFinder("FindLambda", this, &RunningAlphaStrong::FindLambda,
            LambdaMin, LambdaMax, 3, "RunningAlphaStrong", "FindLambda");
    ROOT::Math::BrentRootFinder Brent;

    // Find fLambdaQCD5 between LambdaMin and LambdaMax
    // Solve Running( fZBosonMass, fLambdaQCD5, 5 ) = fAlphaSMZ

    AlphaTarget = fAlphaSMZ;
    LambdaFinder.SetParameter(0, Z_BOSON_MASS);
    LambdaFinder.SetParameter(1, fAlphaSMZ);
    LambdaFinder.SetParameter(2, 5);

    WrappedLambdaFinder = new ROOT::Math::WrappedTF1(LambdaFinder);

    Brent.SetFunction(*WrappedLambdaFinder, LambdaMin, LambdaMax);
    Brent.Solve();
    fLambdaQCD5 = Brent.Root();

    delete WrappedLambdaFinder;

    // Find fLambdaQCD4 between fLambdaQCD5 and LambdaMax
    // Solve Running( fBottomQuarkMass, fLambdaQCD5, 5 ) = Running( fBottomQuarkMass, fLambdaQCD4, 4 )

    Running(QUARK_BOTTOM_MASS, fLambdaQCD5, 5);
    AlphaTarget = fAlphaS;
    LambdaFinder.SetParameter(0, QUARK_BOTTOM_MASS);
    LambdaFinder.SetParameter(1, AlphaTarget);
    LambdaFinder.SetParameter(2, 4);

    WrappedLambdaFinder = new ROOT::Math::WrappedTF1(LambdaFinder);

    Brent.SetFunction(*WrappedLambdaFinder, fLambdaQCD5, LambdaMax);
    Brent.Solve();
    fLambdaQCD4 = Brent.Root();

    delete WrappedLambdaFinder;

    // Find fLambdaQCD3 between fLambdaQCD4 and LambdaMax
    // Solve Running( fCharmQuarkMass, fLambdaQCD4, 4 ) = Running( fCharmQuarkMass, fLambdaQCD3, 3 )

    Running(QUARK_CHARM_MASS, fLambdaQCD4, 4);
    AlphaTarget = fAlphaS;
    LambdaFinder.SetParameter(0, QUARK_CHARM_MASS);
    LambdaFinder.SetParameter(1, AlphaTarget);
    LambdaFinder.SetParameter(2, 3);

    WrappedLambdaFinder = new ROOT::Math::WrappedTF1(LambdaFinder);

    Brent.SetFunction(*WrappedLambdaFinder, fLambdaQCD4, LambdaMax);
    Brent.Solve();
    fLambdaQCD3 = Brent.Root();

    delete WrappedLambdaFinder;

    // Find fLambdaQCD6 between LambdaMin and fLambdaQCD5
    // Solve Running( fTopQuarkMass, fLambdaQCD5, 5 ) = Running( fTopQuarkMass, fLambdaQCD6, 6 )

    Running(QUARK_TOP_MASS, fLambdaQCD5, 5);
    AlphaTarget = fAlphaS;
    LambdaFinder.SetParameter(0, QUARK_TOP_MASS);
    LambdaFinder.SetParameter(1, AlphaTarget);
    LambdaFinder.SetParameter(2, 6);

    WrappedLambdaFinder = new ROOT::Math::WrappedTF1(LambdaFinder);

    Brent.SetFunction(*WrappedLambdaFinder, LambdaMin, fLambdaQCD5);
    Brent.Solve();
    fLambdaQCD6 = Brent.Root();

    delete WrappedLambdaFinder;

}

/*!
 * \fn Running( double Mu, double Lambda, unsigned int NFlavour)
 *
 * Running of Alpha_S at four loops, depending on the renormalization scale Mu and Lambda_QCD with NFlavour active flavours.
 *
 */
void RunningAlphaStrong::Running(double Mu, double Lambda,
        unsigned int NFlavour) {
    double t = 2. * log(Mu / Lambda);
    double lt = log(t);

    ComputeExpansionCoefficients(NFlavour);

    fAlphaS =
            1. / (fBeta0 * t) - fB1 * lt / pow(fBeta0 * t, 2)
                    + 1. / pow(fBeta0 * t, 3)
                            * (pow(fB1, 2) * (pow(lt, 2) - lt - 1) + fB2)
                    + 1. / pow(fBeta0 * t, 4)
                            * (pow(fB1, 3)
                                    * (-pow(lt, 3) + 5. / 2. * pow(lt, 2)
                                            + 2. * lt - 1. / 2.)
                                    - 3. * fB1 * fB2 * lt + fB3 / 2.);

    fAlphaS *= PI;
}

/*!
 * \fn FindLambda( double Lambda, double* Parameters )
 *
 * Equation alpha_s( Mu, Lambda ) - alpha  = 0 to get Lambda from the value alpha of the strong coupling constant at scale Mu.
 * Parameters is an array of 2 doubles :
 *    - Parameters[ 0 ] is the scale Mu,
 *    - Parameters[ 1 ] is the value alpha_s( Mu ).
 *    - Parameters[ 2 ] is the number of active flavours
 *
 */
double RunningAlphaStrong::FindLambda(double* Lambda, double* Parameters) {
    unsigned int NFlavour = (unsigned int) Parameters[2];

    if ((NFlavour != 3) && (NFlavour != 4) && (NFlavour != 5)
            && (NFlavour != 6)) {
//        cout
//                << "RunningAlphaStrong : Erroneous input in RunningAlphaStrong::FinLambda."
//                << endl;
//        cout
//                << "RunningAlphaStrong : Number of active flavours has to be an integer between 3 and 6."
//                << endl;
//        cout << "RunningAlphaStrong : Here NFlavour = " << NFlavour << endl;
//        cout << endl;
        exit(-1);
    }

    Running(Parameters[0], Lambda[0], NFlavour);

    return fAlphaS - Parameters[1];
}
