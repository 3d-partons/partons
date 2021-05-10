/**
  * @file    RunningAlphaStrongPI.cpp
  * @author  Jose Manuel Morgado Chávez (University of Huelva),
  * @date    May, 10th 2021
  * 
  * @brief   Evaluation of the process independent strong running coupling constant presented in e.g.: Eur. Phys. J. C (2020) 80:1064
  * @version 1.0
  *      
  * TODO: Automatize computation of LambdaQCD_nf 
  *
  */

#include "../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongPI.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/root_finding/Brent.h>
#include <cmath>
#include <cstdlib>

#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {

const unsigned int RunningAlphaStrongPI::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new RunningAlphaStrongPI("RunningAlphaStrongPI"));

/**
  * RunningAlphaStrong::RunningAlphaStrong( string pQCD, bool Verbose )
  *
  * Default constructor.
  */
RunningAlphaStrongPI::RunningAlphaStrongPI(const std::string &className) :
        RunningAlphaStrongModule(className), fNc(3), fBeta0(0.), fLambdaQCD4(0.),
            fLambdaQCD5(0.), fLambdaQCD6(0.), fa0(0.104), fa1(0.0975), fb0(0.121), 
                fAlphaS(0.) {
}

RunningAlphaStrongPI::RunningAlphaStrongPI(const RunningAlphaStrongPI &other) :
        RunningAlphaStrongModule(other) {
    fNc = other.fNc;
    fBeta0 = other.fBeta0;
    fLambdaQCD4 = other.fLambdaQCD4;
    fLambdaQCD5 = other.fLambdaQCD5;
    fLambdaQCD6 = other.fLambdaQCD6;
    fa0 = other.fa0;
    fa1 = other.fa1;
    fb0 = other.fb0;
    fAlphaS = other.fAlphaS;
}

RunningAlphaStrongPI* RunningAlphaStrongPI::clone() const {
    return new RunningAlphaStrongPI(*this);
}

/**
  * Default destructor.
  */
RunningAlphaStrongPI::~RunningAlphaStrongPI() {
}

// Quark masses, beta function coefficients, Lambda_QCD and running scale take their default values.
void RunningAlphaStrongPI::initModule() {
    RunningAlphaStrongModule::initModule();
}

// TODO: implement.
void RunningAlphaStrongPI::isModuleWellConfigured() {
    RunningAlphaStrongModule::isModuleWellConfigured();
}


/**
  * RnningAlphaStrongPI::compute()
  *
  * Computation of the strong running coupling at the specified scale, Mu.
  * 
  * 1. Sets the number of active flavours: m_nf.
  * 2. Computes LambdaQCD for every flavour and sets Lambda = LambdaQCDm_nf to be used later on.
  * 3. Computes alphaS( m_Mu, Lambda, m_nf )
  * 
  * TODO: implement - Voir a remplacer ça par un arbre binaire pour les tests si pertinent.
  * TODO: Definir la masse des quarks au carré dans le fichier de constantes.
  */
double RunningAlphaStrongPI::compute() {

    // Set number of active flavours
    if ( m_Mu < Constant::QUARK_BOTTOM_MASS) {
        m_nf = 4;
    }

    else if (Constant::QUARK_BOTTOM_MASS <= m_Mu && m_Mu < Constant::QUARK_TOP_MASS) {
        m_nf = 5;
    }

    else if (Constant::QUARK_TOP_MASS <= m_Mu) {
        m_nf = 6;
    }

    debug(__func__, ElemUtils::Formatter() << "Mu2= " << m_Mu2 << "(GeV^2)   nf= " << m_nf);

    ComputeLambdaQCD();

    double Lambda = 0.;                                 // Lambda_QCD with actual number of active flavours
    
    // Finds the actual value of Lambda_QCD
    switch (m_nf) {

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
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Number of active flavours value must be between 4 and 6. Cannot find actual value of Lambda_QCD.");
    }

    debug(__func__, ElemUtils::Formatter() << "Lambda = " << Lambda);

    Running(m_Mu, Lambda, m_nf);

    if (m_alphaS > 1.0)
        warn(__func__, ElemUtils::Formatter() << "alpha_s = " << m_alphaS);

    return fAlphaS;
}

/**
  * RnningAlphaStrongPI::GetColourNumber()
  *
  * General SU(Nc). It is set in the constructor as fNc.
  */
unsigned int RunningAlphaStrongPI::GetColourNumber() const 
{
    return fNc;
}

// PRIVATE ROUTINES

/**
  * RunningAlphaStrongPI::ComputeExpansionCoefficients(unsigned int NFlavour)
  * 
  * Computes the coefficient of the expansion of the QCD beta function.                 
  */
void RunningAlphaStrongPI::ComputeExpansionCoefficients(unsigned int NFlavour) {

    double CA = fNc;                                    // Casimir operator, adjoint representation
    double TF = 0.5;                                    // Fundamental representation generator

    //TODO replace exit(-1)
    if (NFlavour != 4 && NFlavour != 5 && NFlavour != 6) 
    {
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

    fBeta0 = 11. / 3. * CA - 4. / 3. * TF * NFlavour;
    fBeta0 /= 4.;
}

/**
  * RunningAlphaStrongPI::ComputeLambdaQCD()
  *
  * Computes Lambda_QCD as a function of the number of active flavours 
  * (
  *     Those values have been computed by hand requiring:
  *         alphaS_nf = alphaS_(nf+1)
  *     which implies,     
  *         LambadaQCD2_(nf+1) \propto (LambdaQCD2_nf)^[beta0(nf)/beta0(nf+1)]
  * ) 
  * 
  * TODO: Start from the value Lambda_QCD^2 = 0.234 GeV^2 given in Eur. Phys. J. C (2020) 80:1064, which is valid up to four flavours. Then runs forwards to top quark threshold to get Lambda_QCD for Nf = 6.
  */
void RunningAlphaStrongPI::ComputeLambdaQCD() {
    
    // Set fLambdaQCD4
    fLambdaQCD4 = 0.234;            // GeV
    
    // Find fLambdaQCD5
    fLambdaQCD5 = 0.182;            // GeV

    // Find fLambdaQCD6
    fLambdaQCD6 = 0.095;            //GeV
}

/*!
 * \fn Running( double Mu, double Lambda, unsigned int NFlavour)
 *
 * Running of Alpha_PI, depending on the renormalization scale Mu and Lambda_QCD with NFlavour active flavours.
 *
 */
void RunningAlphaStrongPI::Running(double Mu, double Lambda, unsigned int NFlavour) {

    double t = log( ( pow(fa0, 2.) + fa1 * pow (Mu, 2.) + pow(Mu, 4.) ) / ( pow(Lambda, 2.) * (fb0 + pow(Mu, 2.)) ) );

    ComputeExpansionCoefficients(NFlavour);

    fAlphaS = 1. / (fBeta0 * t);

    fAlphaS *= Constant::PI;
}

//! This is not need with the present implementation. Something similar will be necessary when improving the computation of LambdaQCD

/**
  *  RunningAlphaStrongPI::FindLambda(double Lambda, double* Parameters)
  *
  * Equation alpha_s( Mu, Lambda ) - alpha  = 0 to get Lambda from the value alpha of the strong coupling constant at scale Mu.
  * Parameters is an array of 2 doubles :
  *    - Parameters[ 0 ] is the scale Mu,
  *    - Parameters[ 1 ] is the value alpha_s( Mu ).
  *    - Parameters[ 2 ] is the number of active flavours
  *
  */

// double RunningAlphaStrongPI::FindLambda(double Lambda, std::vector<double> Parameters) {
    
//     unsigned int NFlavour = (unsigned int) Parameters[2];

//     if (NFlavour < 3 && NFlavour > 6) {
//         throw ElemUtils::CustomException(getClassName(), __func__,
//                 ElemUtils::Formatter()
//                         << "Erroneous input in RunningAlphaStrong::FinLambda. Number of active flavours has to be an integer between 3 and 6. Here NFlavour = "
//                         << NFlavour);
//     }

//     Running(Parameters[0], Lambda, NFlavour);

//     return fAlphaS - Parameters[1];
// }

} /* namespace PARTONS */