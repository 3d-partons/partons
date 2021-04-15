/**
  * @file    saturatedModel_Ding.cpp
  * @author  José Manuel Morgado Chavez (University Of Huelva)
  * @author  Cédric Mezrag (CEA Saclay)
  * @date    15th April 2021 
  * @version 1.0
  */

#include "../../../../../include/partons/modules/gpd/pionGPD/saturatedModel_Ding.h"

#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <cln/float.h>
#include <cln/float_class.h>
#include <cln/floatformat.h>
#include <cln/real.h>
#include <cmath>
#include <utility>

// Numerical extension.
#include <NumA/RadonTransform/RadonTransform.h>
#include <eigen3/Eigen/Dense>

#include <random>
#include <ctime>

// NumA interpolation (D-terms)
// #include <NumA/interpolation/CubicSpline.h>

namespace PARTONS {

// With this line we "create" the name for our GPD module. This will be integrated into the factory and thus partons knows about it.
const unsigned int saturatedModel_Ding::classId =
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new saturatedModel_Ding("saturatedModel_Ding"));

saturatedModel_Ding::saturatedModel_Ding(const std::string &className) : PARTONS::GPDModule(className)
{    
    // Set reference factorization scale.
    m_MuF2_ref = pow(0.5,2.);                                                                              // TODO: Set equal to value given in reference paper for \alpha_PI: \mu_H = 0,33 GeV.

    //Relate a specific GPD type with the appropriate function
    m_listGPDComputeTypeAvailable.insert(
        std::make_pair(PARTONS::GPDType::H, &PARTONS::GPDModule::computeH));
}

saturatedModel_Ding::saturatedModel_Ding(const saturatedModel_Ding& other) : PARTONS::GPDModule(other) 
{    
    m2 = 0.101;  // Mass parameter algebraic toy model. Eq (30) Physics Letters B 780 (2018) 287–293.
    m2D = 0.099; // D-term t-dependence: Fitting of Phys. Rev. D 97, 014020 (2018) gravitational FFs.
}

saturatedModel_Ding::~saturatedModel_Ding() 
{

}

saturatedModel_Ding* saturatedModel_Ding::clone() const 
{
    return new saturatedModel_Ding(*this);
}

void saturatedModel_Ding::resolveObjectDependencies() 
{

}

void saturatedModel_Ding::configure(const ElemUtils::Parameters &parameters) 
{
    PARTONS::GPDModule::configure(parameters);
}

void saturatedModel_Ding::isModuleWellConfigured() 
{
    PARTONS::GPDModule::isModuleWellConfigured();
}

void saturatedModel_Ding::initModule() 
{
    PARTONS::GPDModule::initModule();
}

PARTONS::PartonDistribution saturatedModel_Ding::computeH() 
{

    PARTONS::PartonDistribution partonDistribution;

    // Quark distributions
    QuarkDistribution quarkDistributionUp(QuarkFlavor::UP);
    QuarkDistribution quarkDistributionDown(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistributionStrange(QuarkFlavor::STRANGE);

    // u-quark
    double uVal, uValM;

    if ( m_t == 0)                                                                                          //! Zero momentum transfer
    {
        if ( m_x > m_xi )                                                                                   // DGLAP>
        {
            uVal = (213.32*pow(1 - m_x,2.)*(pow(m_x,2.) - pow(m_xi,2.))
                   *sqrt( 1 - 2.9342*sqrt( (1 - m_x)/(1 - m_xi) )*sqrt( (m_x - m_xi)/(1 - m_xi) ) + 2.2911*((1 - m_x)/(1 - m_xi))*((m_x - m_xi)/(1 - m_xi))  )
                   *sqrt( 1 - 2.9342*sqrt( (1 - m_x)/(1 + m_xi) )*sqrt( (m_x + m_xi)/(1 + m_xi) ) + 2.2911*((1 - m_x)/(1 + m_xi))*((m_x + m_xi)/(1 + m_xi)) ))
                   /( pow(1 - pow(m_xi,2.),2.) );
            uValM = 0.;
        } else if ( m_x < -m_xi )                                                                           //DGLAP>
        {
            uVal = 0.;
            uValM = (213.32*pow(1 + m_x,2.)*(pow(m_x,2.) - pow(m_xi,2.))
                    *sqrt( 1 - 2.9342*sqrt( (1 + m_x)/(1 - m_xi) )*sqrt( (-m_x - m_xi)/(1 - m_xi) ) + 2.2911*((1 + m_x)/(1 - m_xi))*((-m_x - m_xi)/(1 - m_xi))  )
                    *sqrt( 1 - 2.9342*sqrt( (1 + m_x)/(1 + m_xi) )*sqrt( (-m_x + m_xi)/(1 + m_xi) ) + 2.2911*((1 + m_x)/(1 + m_xi))*((-m_x + m_xi)/(1 + m_xi)) ))
                    /( pow(1 - pow(m_xi,2.),2.) );   
        } else                                                                                              // ERBL
        {           
            if ( DDt0.isZero() )                                                                               
            {
                Eigen::VectorXd GPD_DGLAP(RT.x.size());
                
                for ( int i = 0; i < RT.x.size(); i ++ )
                {
                    GPD_DGLAP(i) = (213.32*pow(1 - RT.x.at(i),2.)*(pow(RT.x.at(i),2.) - pow(RT.xi.at(i),2.))
                                   *sqrt( 1 - 2.9342*sqrt( (1 - RT.x.at(i))/(1 - RT.xi.at(i)) )*sqrt( (RT.x.at(i) - RT.xi.at(i))/(1 - RT.xi.at(i)) ) + 2.2911*((1 - RT.x.at(i))/(1 - RT.xi.at(i)))*((RT.x.at(i) - RT.xi.at(i))/(1 - RT.xi.at(i)))  )
                                   *sqrt( 1 - 2.9342*sqrt( (1 - RT.x.at(i))/(1 + RT.xi.at(i)) )*sqrt( (RT.x.at(i) + RT.xi.at(i))/(1 + RT.xi.at(i)) ) + 2.2911*((1 - RT.x.at(i))/(1 + RT.xi.at(i)))*((RT.x.at(i) + RT.xi.at(i))/(1 + RT.xi.at(i))) ))
                                   /( pow(1 - pow(RT.xi.at(i),2.),2.) );
                }
                
                DDt0 = RT.computeDD( GPD_DGLAP );
            }

            // ============================================================================================
            // Compute ERBL GPD (Proper computation: RT)
            // ============================================================================================

            uVal = RT.computeGPD( DDt0, m_x, m_xi );
            uValM = RT.computeGPD( DDt0, -m_x, m_xi );

            // // D-terms contribution                                                                         // TODO: Implement computation of D-terms in RT.
            // if ( DtermsVec.size() == 0 )
            // {                    
            //     DtermsVec = computeDterms();
 
            //     // Interpolate numerically computed D-terms.
            //     Dminus = new NumA::CubicSpline(DtermsVec[2],DtermsVec[0]);
            //     Dplus = new NumA::CubicSpline(DtermsVec[2],DtermsVec[1]);
            //     //  
            //     Dminus->ConstructSpline();
            //     Dplus->ConstructSpline(); 
            // }           
 
            // // Add D-terms to GPD.
            // alpha = m_x/m_xi;
 
            // if ( m_xi >= 0 )                                                                                // Conditional expression taking into acount the factor sign(\xi) accompanying dminus.
            // {
            //     uVal +=  Dplus->getSplineInsideValue(alpha)/m_xi + Dminus->getSplineInsideValue(alpha);
            //     uValM += Dplus->getSplineInsideValue(alpha)/m_xi - Dminus->getSplineInsideValue(alpha);
            // } else
            // {
            //     uVal += Dplus->getSplineInsideValue(alpha)/m_xi - Dminus->getSplineInsideValue(alpha);
            //     uValM += Dplus->getSplineInsideValue(alpha)/m_xi + Dminus->getSplineInsideValue(alpha);
            // }
        }
    } else                                                                                                  //! Non-vanishing momentum transfer.
    {
        c  = -m_t*pow(1 - m_x, 2.)/(4*m2*(1 - pow(m_xi,2)));                                                // t-dependence algebraic toy model.
        cM = -m_t*pow(1 + m_x, 2.)/(4*m2*(1 - pow(m_xi,2)));

        dt = 1/(1-0.25*m_t/m2D);                                                                             // D-term t-dependence (monopole parametrization).

        if ( m_x > m_xi )                                                                                   // DGLAP>
        {
            uVal = (213.32*pow(1 - m_x,2.)*(pow(m_x,2.) - pow(m_xi,2.))
                   *sqrt( 1 - 2.9342*sqrt( (1 - m_x)/(1 - m_xi) )*sqrt( (m_x - m_xi)/(1 - m_xi) ) + 2.2911*((1 - m_x)/(1 - m_xi))*((m_x - m_xi)/(1 - m_xi))  )
                   *sqrt( 1 - 2.9342*sqrt( (1 - m_x)/(1 + m_xi) )*sqrt( (m_x + m_xi)/(1 + m_xi) ) + 2.2911*((1 - m_x)/(1 + m_xi))*((m_x + m_xi)/(1 + m_xi)) ))
                   *(3 + ((1 - 2 * c) * atanh(sqrt(c/(1+c))))/((1 + c) * sqrt(c/(1 + c))) )
                   /( pow(1 - pow(m_xi,2.),2.) * pow(1 + c,2.) );
            uValM = 0.;
        } else if ( m_x < -m_xi )                                                                           // DGLAP>
        {
            uVal = 0.;
            uValM = (213.32*pow(1 + m_x,2.)*(pow(m_x,2.) - pow(m_xi,2.))
                    *sqrt( 1 - 2.9342*sqrt( (1 + m_x)/(1 - m_xi) )*sqrt( (-m_x - m_xi)/(1 - m_xi) ) + 2.2911*((1 + m_x)/(1 - m_xi))*((-m_x - m_xi)/(1 - m_xi))  )
                    *sqrt( 1 - 2.9342*sqrt( (1 + m_x)/(1 + m_xi) )*sqrt( (-m_x + m_xi)/(1 + m_xi) ) + 2.2911*((1 + m_x)/(1 + m_xi))*((-m_x + m_xi)/(1 + m_xi)) ))
                    *(3 + ((1 - 2 * cM) * atanh(sqrt(cM/(1+cM))))/((1 + cM) * sqrt(cM/(1 + cM))) )
                    /( pow(1 - pow(m_xi,2.),2.) * pow(1 + cM,2.) ); 
        } else                                                                                              // ERBL
        {
            if ( DD.isZero() )                                                                               
            {
                Eigen::VectorXd GPD_DGLAP(RT.x.size());
                
                for ( int i = 0; i < RT.x.size(); i ++ )
                {
                    GPD_DGLAP(i) = (213.32*pow(1 - m_x,2.)*(pow(m_x,2.) - pow(m_xi,2.))
                                   *sqrt( 1 - 2.9342*sqrt( (1 - m_x)/(1 - m_xi) )*sqrt( (m_x - m_xi)/(1 - m_xi) ) + 2.2911*((1 - m_x)/(1 - m_xi))*((m_x - m_xi)/(1 - m_xi))  )
                                   *sqrt( 1 - 2.9342*sqrt( (1 - m_x)/(1 + m_xi) )*sqrt( (m_x + m_xi)/(1 + m_xi) ) + 2.2911*((1 - m_x)/(1 + m_xi))*((m_x + m_xi)/(1 + m_xi)) ))
                                   *(3 + ((1 - 2 * c) * atanh(sqrt(c/(1+c))))/((1 + c) * sqrt(c/(1 + c))) )
                                   /( pow(1 - pow(m_xi,2.),2.) * pow(1 + c,2.) );
                }
                
                DD = RT.computeDD( GPD_DGLAP );
            }                                                                              
                
            // ============================================================================================
            // Compute ERBL GPD (Proper computation: RT)
            // ============================================================================================

            uVal = RT.computeGPD( DDt0, m_x, m_xi );
            uValM = RT.computeGPD( DDt0, -m_x, m_xi );

            // D-terms contribution                                                                         // TODO: Implement computation of D-terms in RT.
            if ( DtermsVec.size() == 0 )
            {                    
                DtermsVec = computeDterms();
 
                // Interpolate numerically computed D-terms.
                Dminus = new NumA::CubicSpline(DtermsVec[2],DtermsVec[0]);
                Dplus = new NumA::CubicSpline(DtermsVec[2],DtermsVec[1]);
                //  
                Dminus->ConstructSpline();
                Dplus->ConstructSpline(); 
            }           
 
            // Add D-terms to GPD.
            alpha = m_x/m_xi;
 
            if ( m_xi >= 0 )                                                                                // Conditional expression taking into acount the factor sign(\xi) accompanying dminus.
            {
                uVal +=  Dplus->getSplineInsideValue(alpha)/m_xi + Dminus->getSplineInsideValue(alpha);
                uValM += Dplus->getSplineInsideValue(alpha)/m_xi - Dminus->getSplineInsideValue(alpha);
            } else
            {
                uVal += Dplus->getSplineInsideValue(alpha)/m_xi - Dminus->getSplineInsideValue(alpha);
                uValM += Dplus->getSplineInsideValue(alpha)/m_xi + Dminus->getSplineInsideValue(alpha);
            }
        }
    }

    // TODO: Check and fix definitions of the the different quark distributions (u, uM, u+, u-, d(...) and s(...)). See Cédric PhD thesis. pp. 56.
    double uSea  = 0.;
    double uSeaM = 0.;

    quarkDistributionUp.setQuarkDistribution(uVal + uSea);

        // Singlet distribution
        quarkDistributionUp.setQuarkDistributionPlus(uVal + uSea - uValM - uSeaM);
        // Nonsinglet distributiion
        quarkDistributionUp.setQuarkDistributionMinus(uVal + uValM);
        
    // d-quark    
    double dVal = -uValM;
    double dValM = -uVal;
    double dSea = 0.;
    double dSeaM = 0.;

    quarkDistributionDown.setQuarkDistribution(dVal + dSea);

        // Singlet distribution
        quarkDistributionDown.setQuarkDistributionPlus(dVal + dSea - dValM - dSeaM);
        // Nonsinglet distributiion
        quarkDistributionDown.setQuarkDistributionMinus(dVal + dValM);

    // s-quark
    double sVal = 0.;
    double sValM = 0.;
    double sSea = 0.;
    double sSeaM = 0.;

    quarkDistributionStrange.setQuarkDistribution(sVal + sSea);
    quarkDistributionStrange.setQuarkDistributionPlus(sVal + sSea - sValM - sSeaM);
    quarkDistributionStrange.setQuarkDistributionMinus(sVal + sValM);

    partonDistribution.addQuarkDistribution(quarkDistributionUp);
    partonDistribution.addQuarkDistribution(quarkDistributionDown);
    partonDistribution.addQuarkDistribution(quarkDistributionStrange);

    // Gluon distributions 
    GluonDistribution gluonDistribution(0.);
    partonDistribution.setGluonDistribution(gluonDistribution);

    return partonDistribution;
}

std::vector<std::vector<double>> saturatedModel_Ding::computeDterms()
{
    /**
      * Dminus = 0.5*(H(-x,1,0) - H(x,1,0))
      * Dplus = 0.5(phi((1+x)/2) - H(-x,1,0) - H(x,1,0))
      */

    // Computation of DD 
    if ( DDt0.isZero() )                                                                               
    {
        // ============================================================================================
        // Compute DD (Proper computation)
        // ============================================================================================
        
        Eigen::VectorXd GPD_DGLAP(RT.x.size());
                
        for ( int i = 0; i < RT.x.size(); i ++ )
        {
            GPD_DGLAP(i) = 30 * pow(1 - RT.x.at(i), 2.) * ( pow(RT.x.at(i),2.) - pow(RT.xi.at(i),2.) ) / 
                           pow( 1 - pow(RT.xi.at(i),2.) , 2.);
        }
                
        DDt0 = RT.computeDD( GPD_DGLAP ); 
    }

    // Computation of GPDs

    // Evaluation points.
    const double nop = 10000;                                                                   // Number of points.
    const double xstp = 2./nop;                                                                 // Step.

    std::vector<double> x (nop);                                                                // Sampling points.
    std::vector<double> xi (nop,1.);

    std::vector<std::vector<double>> Dterms( 3, std::vector<double> (nop) );                    // Matrix containing the numerical evaluation for the D-terms: DtermsVec[0]: Dminus (Odd D-term)
                                                                                                //                                                             DtermsVec[1]: Dplus (even D-term).
                                                                                                //                                                             DtermsVec[2]: x: Evaluation points of the D-terms.

    for ( int i = 0; i < nop; i++ )
    {
        x.at(i) = -1.+i*xstp;
        Dterms[2].at(i) = x.at(i);                                                              // Update Dterms-matrix for output.
    }
        
    // H(x,1,0)
    std::vector<double> GPD1x (nop);

    GPD1x = RT.computeGPD( DDt0, x, xi);

    // H(x,-1,0)
    std::vector<double> GPD1xM (nop);

    for ( int i = 0; i < nop; i++ )
    {
        x.at(i) *= -1.;
    }

    GPD1xM = RT.computeGPD( DDt0, x, xi );

    // Compute Dterms
    for ( int i = 0; i < nop; i++ )
    {
        Dterms[0].at(i) = 0.5*(GPD1xM.at(i)-GPD1x.at(i)); 
        Dterms[1].at(i) = 0.5*(1.5*(1-pow(x.at(i),2))-GPD1xM.at(i)-GPD1x.at(i));
    }

return Dterms;
}

}  
