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

#include "models/FundamentalPhysicalConstants.h"

/*------------------------------- Public routines ----------------------------*/

/*!
 * \fn RunningAlphaStrong::RunningAlphaStrong( string pQCD, bool Verbose )
 * 
 * Default constructor.
 * 
 */
RunningAlphaStrong::RunningAlphaStrong(unsigned int Nc, bool Verbose) :
        kVerbose(Verbose), fUpdateRunning(false), fNf(4), fNc(Nc), fBeta0(
                2.08333), fBeta1(3.20833), fBeta2(6.34925), fBeta3(31.3874), fB1(
                1.54), fB2(3.04764), fB3(15.066), fLambdaQCD3(0.329939), fLambdaQCD4(
                0.28914), fLambdaQCD5(0.208364), fLambdaQCD6(0.0878108), fAlphaSMZ(
                0.1184), fAlphaS(0.303061), fRunningScale(2.), fStrangeQuarkMass(
                QUARK_STRANGE_MASS), fCharmQuarkMass(QUARK_CHARM_MASS), fBottomQuarkMass(
                QUARK_BOTTOM_MASS), fTopQuarkMass(QUARK_TOP_MASS), fZBosonMass(
                Z_BOSON_MASS) {
}

/*!
 * 
 * Default destructor.
 *
 */
RunningAlphaStrong::~RunningAlphaStrong() {
}

/*!
 * \fn void SetRunningScale( double Mu = 2. )
 * 
 * Set current renormalization scale (in GeV). Same default as for (most) PDFs.
 *
 */
//TODO remettre les tests de contraintes
void RunningAlphaStrong::SetRunningScale(double Mu) {
//   if ( Mu <= fLambdaQCD3 ) {
//      cout << "RunningAlphaStrong : Erroneous input in RunningAlphaStrong::SetRunningScale." << endl;
//      cout << "RunningAlphaStrong : The running scale must be larger than LambdaQCD for three active flavours." << endl;
//      cout << "RunningAlphaStrong : Here Mu = " << Mu << " GeV." << endl;
//      cout << endl;
//      exit( -1 );
//   } else {
//      fRunningScale = Mu;
//   }
//
    if (fStrangeQuarkMass <= fRunningScale && fRunningScale < fCharmQuarkMass) {
        fNf = 3;
    }

    if (fCharmQuarkMass <= fRunningScale && fRunningScale < fBottomQuarkMass) {
        fNf = 4;
    }

    if (fBottomQuarkMass <= fRunningScale && fRunningScale < fTopQuarkMass) {
        fNf = 5;
    }

    if (fTopQuarkMass <= fRunningScale) {
        fNf = 6;
    }

    fRunningScale = Mu;
}

/*!
 * \fn void SetAlphaSMZ( double Alpha = 0.1184 )
 * 
 *  Retrieves the value of the strong coupling constant at the scale MZ = Z^0 boson mass.
 * 
 */
void RunningAlphaStrong::SetAlphaSMZ(double Alpha) {
//    if (Alpha <= 0) {
//        cout
//                << "RunningAlphaStrong : Erroneous input in RunningAlphaStrong::SetAlphaSMZ. "
//                << endl;
//        cout
//                << "RunningAlphaStrong : The strong coupling constant must be positive."
//                << endl;
//        cout << "RunningAlphaStrong : Here Alpha = " << Alpha << endl;
//        cout << endl;
//        exit(-1);
//    } else {
    fAlphaSMZ = Alpha;
    fUpdateRunning = true;
//    }
}

/*!
 * \fn void SetStrangeQuarkMass( double Mass = TConstant::StrangeQuarkMass )
 * 
 *  Retrieves strange quark mass (in GeV).
 * 
 */
void RunningAlphaStrong::SetStrangeQuarkMass(double Mass) {
//    if (Mass <= 0) {
//        cout
//                << "RunningAlphaStrong : Erroneous input in RunningAlphaStrong::SetStrangeQuarkMass. "
//                << endl;
//        cout << "RunningAlphaStrong : The strange quark mass must be positive."
//                << endl;
//        cout << "RunningAlphaStrong : Here Mass = " << Mass << " GeV." << endl;
//        cout << endl;
//        exit(-1);
//    } else {
    fStrangeQuarkMass = Mass;
    fUpdateRunning = true;
    TestMassHierarchy();
//    }
}

/*!
 * \fn void SetCharmQuarkMass( double Mass = TConstant::CharmQuarkMass )
 * 
 *  Retrieves charm quark mass (in GeV).
 * 
 */
void RunningAlphaStrong::SetCharmQuarkMass(double Mass) {
//    if (Mass <= 0) {
//        cout
//                << "RunningAlphaStrong : Erroneous input in RunningAlphaStrong::SetCharmQuarkMass. "
//                << endl;
//        cout << "RunningAlphaStrong : The charm quark mass must be positive."
//                << endl;
//        cout << "RunningAlphaStrong : Here Mass = " << Mass << " GeV." << endl;
//        cout << endl;
//        exit(-1);
//    } else {
    fCharmQuarkMass = Mass;
    fUpdateRunning = true;
    TestMassHierarchy();
//    }
}

/*!
 * \fn void SetBottomQuarkMass( double Mass = TConstant::BottomQuarkMass )
 * 
 *  Retrieves bottom quark mass (in GeV).
 * 
 */
void RunningAlphaStrong::SetBottomQuarkMass(double Mass) {
//    if (Mass <= 0) {
//        cout
//                << "RunningAlphaStrong : Erroneous input in RunningAlphaStrong::SetBottomQuarkMass. "
//                << endl;
//        cout << "RunningAlphaStrong : The bottom quark mass must be positive."
//                << endl;
//        cout << "RunningAlphaStrong : Here Mass = " << Mass << " GeV." << endl;
//        cout << endl;
//        exit(-1);
//    } else {
    fBottomQuarkMass = Mass;
    fUpdateRunning = true;
    TestMassHierarchy();
//    }
}

/*!
 * \fn void SetTopQuarkMass( double Mass = TConstant::TopQuarkMass )
 * 
 *  Retrieves top quark mass (in GeV).
 * 
 */
void RunningAlphaStrong::SetTopQuarkMass(double Mass) {
//    if (Mass <= 0) {
//        cout
//                << "RunningAlphaStrong : Erroneous input in RunningAlphaStrong::SetTopQuarkMass. "
//                << endl;
//        cout << "RunningAlphaStrong : The Top quark mass must be positive."
//                << endl;
//        cout << "RunningAlphaStrong : Here Mass = " << Mass << " GeV." << endl;
//        cout << endl;
//        exit(-1);
//    } else {
    fTopQuarkMass = Mass;
    fUpdateRunning = true;
    TestMassHierarchy();
//    }
}

/*!
 * \fn void SetZBosonMass( double Mass = TConstant::ZBosonMass )
 * 
 *  Retrieves Z boson mass (in GeV).
 * 
 */
void RunningAlphaStrong::SetZBosonMass(double Mass) {
//    if (Mass <= 0) {
//        cout
//                << "RunningAlphaStrong : Erroneous input in RunningAlphaStrong::SetZBosonMass. "
//                << endl;
//        cout << "RunningAlphaStrong : The Z boson mass must be positive."
//                << endl;
//        cout << "RunningAlphaStrong : Here Mass = " << Mass << " GeV." << endl;
//        cout << endl;
//        exit(-1);
//    } else {
    fZBosonMass = Mass;
    fUpdateRunning = true;
    TestMassHierarchy();
//    }
}

/*!
 * \fn double Get AlphaS()
 * 
 * Returns strong coupling constant at scale Mu (in GeV). Mu should be larger than LambdaQCD( NFlavour = 2 ). 
 * TODO : TEST
 * 
 */
double RunningAlphaStrong::GetAlphaS() {
    double Lambda; // Lambda_QCD with actual number of active flavours

    if (fUpdateRunning == true) {
        ComputeLambdaQCD();
        fUpdateRunning = false;
    }

    // Finds the actual value of Lambda_QCD

    switch (fNf) {

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
//        cout << "RunningAlphaStrong : Erroneous input." << endl;
//        cout
//                << "RunningAlphaStrong : There are currently only six known quarks flavours."
//                << endl;
//        cout << "RunningAlphaStrong : Nf must be an integer between 2 and 6."
//                << endl;
//        cout << endl;
        exit(-1);

    }

    // Computes beta function coefficients and running of strong coupling with actual number of active flavours
    // and value of Lambda_QCD at current scale

    Running(fRunningScale, Lambda, fNf);

    return fAlphaS;
}

/*!
 * \fn GetActiveFlavourNumber ()
 * 
 * Returns number of active flavours at scale fRunningScale.
 *
 */
unsigned int RunningAlphaStrong::GetActiveFlavourNumber() const {
    return fNf;
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
 * \fn GetLambdaQCD( double* Lambda )
 *
 * Returns LambdaQCD as a function of number of active flavours :
 *    Lambda[ 0 ] = LambdaQCD( Nf = 3 )
 *    Lambda[ 1 ] = LambdaQCD( Nf = 4 )
 *    Lambda[ 2 ] = LambdaQCD( Nf = 5 )
 *    Lambda[ 3 ] = LambdaQCD( Nf = 6 )
 *
 */
void RunningAlphaStrong::GetLambdaQCD(double* Lambda) {
    if (fUpdateRunning == true) {
        ComputeLambdaQCD();
        fUpdateRunning = false;
    }

    Lambda[0] = fLambdaQCD3;
    Lambda[1] = fLambdaQCD4;
    Lambda[2] = fLambdaQCD5;
    Lambda[3] = fLambdaQCD6;
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
    fRunningScale = 2.;

    fStrangeQuarkMass = QUARK_STRANGE_MASS;
    fCharmQuarkMass = QUARK_CHARM_MASS;
    fBottomQuarkMass = QUARK_BOTTOM_MASS;
    fTopQuarkMass = QUARK_TOP_MASS;
    fZBosonMass = Z_BOSON_MASS;

    fNf = 4;
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
    LambdaFinder.SetParameter(0, fZBosonMass);
    LambdaFinder.SetParameter(1, fAlphaSMZ);
    LambdaFinder.SetParameter(2, 5);

    WrappedLambdaFinder = new ROOT::Math::WrappedTF1(LambdaFinder);

    Brent.SetFunction(*WrappedLambdaFinder, LambdaMin, LambdaMax);
    Brent.Solve();
    fLambdaQCD5 = Brent.Root();

    delete WrappedLambdaFinder;

    // Find fLambdaQCD4 between fLambdaQCD5 and LambdaMax
    // Solve Running( fBottomQuarkMass, fLambdaQCD5, 5 ) = Running( fBottomQuarkMass, fLambdaQCD4, 4 )

    Running(fBottomQuarkMass, fLambdaQCD5, 5);
    AlphaTarget = fAlphaS;
    LambdaFinder.SetParameter(0, fBottomQuarkMass);
    LambdaFinder.SetParameter(1, AlphaTarget);
    LambdaFinder.SetParameter(2, 4);

    WrappedLambdaFinder = new ROOT::Math::WrappedTF1(LambdaFinder);

    Brent.SetFunction(*WrappedLambdaFinder, fLambdaQCD5, LambdaMax);
    Brent.Solve();
    fLambdaQCD4 = Brent.Root();

    delete WrappedLambdaFinder;

    // Find fLambdaQCD3 between fLambdaQCD4 and LambdaMax
    // Solve Running( fCharmQuarkMass, fLambdaQCD4, 4 ) = Running( fCharmQuarkMass, fLambdaQCD3, 3 )

    Running(fCharmQuarkMass, fLambdaQCD4, 4);
    AlphaTarget = fAlphaS;
    LambdaFinder.SetParameter(0, fCharmQuarkMass);
    LambdaFinder.SetParameter(1, AlphaTarget);
    LambdaFinder.SetParameter(2, 3);

    WrappedLambdaFinder = new ROOT::Math::WrappedTF1(LambdaFinder);

    Brent.SetFunction(*WrappedLambdaFinder, fLambdaQCD4, LambdaMax);
    Brent.Solve();
    fLambdaQCD3 = Brent.Root();

    delete WrappedLambdaFinder;

    // Find fLambdaQCD6 between LambdaMin and fLambdaQCD5
    // Solve Running( fTopQuarkMass, fLambdaQCD5, 5 ) = Running( fTopQuarkMass, fLambdaQCD6, 6 )

    Running(fTopQuarkMass, fLambdaQCD5, 5);
    AlphaTarget = fAlphaS;
    LambdaFinder.SetParameter(0, fTopQuarkMass);
    LambdaFinder.SetParameter(1, AlphaTarget);
    LambdaFinder.SetParameter(2, 6);

    WrappedLambdaFinder = new ROOT::Math::WrappedTF1(LambdaFinder);

    Brent.SetFunction(*WrappedLambdaFinder, LambdaMin, fLambdaQCD5);
    Brent.Solve();
    fLambdaQCD6 = Brent.Root();

    delete WrappedLambdaFinder;

}

/*!
 * \fn TestQuarkMassHierarchy()
 *
 * Test : modifying quark masses should not modify quark masses hierarchy. Only minor changes of quark masses are expected.
 *
 */
void RunningAlphaStrong::TestMassHierarchy() {
    bool Hierarchy = (fStrangeQuarkMass < fCharmQuarkMass)
            && (fCharmQuarkMass < fBottomQuarkMass)
            && (fBottomQuarkMass < fZBosonMass)
            && (fZBosonMass < fTopQuarkMass);

    if (Hierarchy == false) {
//        cout
//                << "RunningAlphaStrong : Erroneous input in RunningAlphaStrong::TestMassHierarchy"
//                << endl;
//        cout
//                << "RunningAlphaStrong : Incorrect mass hierarchy of quark flavours and Z boson."
//                << endl;
//        cout << "RunningAlphaStrong :    strange quark mass = "
//                << fStrangeQuarkMass << " GeV" << endl;
//        cout << "RunningAlphaStrong :    charm quark mass = " << fCharmQuarkMass
//                << " GeV" << endl;
//        cout << "RunningAlphaStrong :    bottom quark mass = "
//                << fBottomQuarkMass << " GeV" << endl;
//        cout << "RunningAlphaStrong :    top quark mass = " << fTopQuarkMass
//                << " GeV" << endl;
//        cout << "RunningAlphaStrong :    Z boson mass = " << fZBosonMass
//                << " GeV" << endl;
//        cout << endl;
        exit(-1);
    }
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
