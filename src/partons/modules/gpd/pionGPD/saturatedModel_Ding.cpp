/**
  * @file    saturatedModel_Ding.cpp
  * @author  José Manuel Morgado Chavez (University Of Huelva)
  * @author  Cédric Mezrag (CEA Saclay)
  * @date    Friday 23rd April 2021 
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
#include <string>

namespace PARTONS {

    const std::string saturatedModel_Ding::COVARIANT_EXTENSION = "RT";

// With this line we "create" the name for our GPD module. This will be integrated into the factory and thus partons knows about it.
const unsigned int saturatedModel_Ding::classId =
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new saturatedModel_Ding("saturatedModel_Ding"));

saturatedModel_Ding::saturatedModel_Ding(const std::string &className) : PARTONS::GPDModule(className)
{    
    // Set reference factorization scale.
    m_MuF2_ref = pow(0.331,2.);                                                                              // TODO: Set equal to value given in reference paper for \alpha_PI: \mu_H = 0,33 GeV.

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

    if (parameters.isAvailable(saturatedModel_Ding::COVARIANT_EXTENSION)) {
        initRT();
        info(__func__, ElemUtils::Formatter() << saturatedModel_Ding::COVARIANT_EXTENSION
	    << "Configured - Radon transform matrix built.");
    }
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

            // Gauged GPD
            uVal = RT.computeGPD( DDt0, m_x, m_xi );
            uValM = RT.computeGPD( DDt0, -m_x, m_xi );

            // Dterm contribution
            uVal += RT.computeDterm( DDt0, m_x, m_xi );
            uValM += RT.computeDterm( DDt0, -m_x, m_xi );

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

            // Gauged GPD
            uVal = RT.computeGPD( DD, m_x, m_xi );
            uValM = RT.computeGPD( DD, -m_x, m_xi );

            // Dterm contribution
            if ( DDt0.isZero() )                                                                               
            {
                Eigen::VectorXd GPD_DGLAP(RT.x.size());
                
                for ( int i = 0; i < RT.x.size(); i ++ )
                {
                    GPD_DGLAP(i) = 30 * pow(1 - RT.x.at(i), 2.) * ( pow(RT.x.at(i),2.) - pow(RT.xi.at(i),2.) ) / pow( 1 - pow(RT.xi.at(i),2.) , 2.);
                }
                
                DDt0 = RT.computeDD( GPD_DGLAP );
            }

            uVal += dt*RT.computeDterm( DDt0, m_x, m_xi );
            uValM += dt*RT.computeDterm( DDt0, -m_x, m_xi );
        
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

void saturatedModel_Ding::initRT()
{
    RT.init();
}

}  
