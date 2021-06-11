/**
  * @file    saturatedModel_Ding.cpp
  * @author  José Manuel Morgado Chavez (University Of Huelva)
  * @author  Cédric Mezrag (CEA Saclay)
  * @date    Friday 11th June 2021 
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

namespace PARTONS {

// With this line we "create" the name for our GPD module. This will be integrated into the factory and thus partons knows about it.
const unsigned int saturatedModel_Ding::classId =
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new saturatedModel_Ding("saturatedModel_Ding"));

saturatedModel_Ding::saturatedModel_Ding(const std::string &className) : PARTONS::GPDModule(className)
{    
    // Set reference factorization scale.
    m_MuF2_ref = pow(0.331,2.);

    //Relate a specific GPD type with the appropriate function
    m_listGPDComputeTypeAvailable.insert(
        std::make_pair(PARTONS::GPDType::H, &PARTONS::GPDModule::computeH));
}

saturatedModel_Ding::saturatedModel_Ding(const saturatedModel_Ding& other) : PARTONS::GPDModule(other) 
{    
    m2 = pow(0.318,2.);                                                                                             // Mass parameter algebraic toy model. Eq (30) Physics Letters B 780 (2018) 287–293.
    m2D = pow(0.318,2.);                                                                                            // D-term t-dependence: Fitting of Phys. Rev. D 97, 014020 (2018) gravitational FFs.
    // RT.init();                                                                                                      // Initialize Radon transform
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

    // Initialise NumA::RadonTransform.
    initRT();

    // Print logger message when NumA::RadonTransform is properly configured.
    info(__func__, ElemUtils::Formatter() << saturatedModel_Ding::COVARIANT_EXTENSION
	<< "Configured - Radon transform matrix built.");
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
    QuarkDistribution quarkDistributionCharm(QuarkFlavor::CHARM);
    QuarkDistribution quarkDistributionBottom(QuarkFlavor::BOTTOM);
    QuarkDistribution quarkDistributionTop(QuarkFlavor::TOP);

    // u-quark
    double uVal, uValM;

    if ( m_t == 0)                                                                                          // Zero momentum transfer
    {
        if ( m_x > m_xi || m_x == m_xi )                                                                    // DGLAP>
        {
            uVal = (213.32*pow(1 - m_x,2.)*(pow(m_x,2.) - pow(m_xi,2.))
                   *sqrt( 1 - 2.9342*sqrt( (1 - m_x)/(1 - m_xi) )*sqrt( (m_x - m_xi)/(1 - m_xi) ) + 2.2911*((1 - m_x)/(1 - m_xi))*((m_x - m_xi)/(1 - m_xi))  )
                   *sqrt( 1 - 2.9342*sqrt( (1 - m_x)/(1 + m_xi) )*sqrt( (m_x + m_xi)/(1 + m_xi) ) + 2.2911*((1 - m_x)/(1 + m_xi))*((m_x + m_xi)/(1 + m_xi)) ))
                   /( pow(1 - pow(m_xi,2.),2.) );
            uValM = 0.;

        } else if ( m_x < -m_xi || m_x == -m_xi )                                                           //DGLAP>
        {
            uVal = 0.;
            uValM = (213.32*pow(1 + m_x,2.)*(pow(m_x,2.) - pow(m_xi,2.))
                    *sqrt( 1 - 2.9342*sqrt( (1 + m_x)/(1 - m_xi) )*sqrt( (-m_x - m_xi)/(1 - m_xi) ) + 2.2911*((1 + m_x)/(1 - m_xi))*((-m_x - m_xi)/(1 - m_xi))  )
                    *sqrt( 1 - 2.9342*sqrt( (1 + m_x)/(1 + m_xi) )*sqrt( (-m_x + m_xi)/(1 + m_xi) ) + 2.2911*((1 + m_x)/(1 + m_xi))*((-m_x + m_xi)/(1 + m_xi)) ))
                    /( pow(1 - pow(m_xi,2.),2.) );  

        } else                                                                                              // ERBL
        {       
            // Compute double distribution.
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

            // Compute "gauged" GPD.
            uVal = RT.computeGPD( DDt0, m_x, m_xi );
            uValM = RT.computeGPD( DDt0, -m_x, m_xi );

            // Compute Dterm contribution.
            uVal += RT.computeDterm( DDt0, m_x, m_xi );
            uValM += RT.computeDterm( DDt0, -m_x, m_xi );
        }

    } else                                                                                                  // Non-vanishing momentum transfer.
    {        
        c  = -m_t*pow(1 - m_x, 2.)/(4*m2*(1 - pow(m_xi,2)));                                                // t-dependence algebraic toy model.
        cM = -m_t*pow(1 + m_x, 2.)/(4*m2*(1 - pow(m_xi,2)));
        // c1  = -m_t/(4*m2);

        dt = 1/(1-0.25*m_t/m2D);                                                                            // D-term t-dependence (monopole parametrization).

        if ( m_x > m_xi || m_x == m_xi )                                                                    // DGLAP>
        {
            uVal = (213.32*pow(1 - m_x,2.)*(pow(m_x,2.) - pow(m_xi,2.))
                   *sqrt( 1 - 2.9342*sqrt( (1 - m_x)/(1 - m_xi) )*sqrt( (m_x - m_xi)/(1 - m_xi) ) + 2.2911*((1 - m_x)/(1 - m_xi))*((m_x - m_xi)/(1 - m_xi))  )
                   *sqrt( 1 - 2.9342*sqrt( (1 - m_x)/(1 + m_xi) )*sqrt( (m_x + m_xi)/(1 + m_xi) ) + 2.2911*((1 - m_x)/(1 + m_xi))*((m_x + m_xi)/(1 + m_xi)) ))
                   *(3 + ((1 - 2 * c) * atanh(sqrt(c/(1+c))))/((1 + c) * sqrt(c/(1 + c))) )
                   /( pow(1 - pow(m_xi,2.),2.) * pow(1 + c,2.) );
            uValM = 0.;

            // if ( m_x == 1 )                                                                                 // Actually this is the limit x->1 (with \xi<1). 
            // {
            //     uVal = 0.;
            //     uValM = 0.;
            // }

        } else if ( m_x < -m_xi || m_x == -m_xi )                                                            // DGLAP<
        {
            uVal = 0.;
            uValM = (213.32*pow(1 + m_x,2.)*(pow(m_x,2.) - pow(m_xi,2.))
                    *sqrt( 1 - 2.9342*sqrt( (1 + m_x)/(1 - m_xi) )*sqrt( (-m_x - m_xi)/(1 - m_xi) ) + 2.2911*((1 + m_x)/(1 - m_xi))*((-m_x - m_xi)/(1 - m_xi))  )
                    *sqrt( 1 - 2.9342*sqrt( (1 + m_x)/(1 + m_xi) )*sqrt( (-m_x + m_xi)/(1 + m_xi) ) + 2.2911*((1 + m_x)/(1 + m_xi))*((-m_x + m_xi)/(1 + m_xi)) ))
                    *(3 + ((1 - 2 * cM) * atanh(sqrt(cM/(1+cM))))/((1 + cM) * sqrt(cM/(1 + cM))) )
                    /( pow(1 - pow(m_xi,2.),2.) * pow(1 + cM,2.) ); 

            // if ( m_x == -1 )                                                                                // Actually this is the limit x->1 (with \xi<1). 
            // {
            //     uVal = 0.;
            //     uValM = 0.;
            // }

        } else                                                                                              // ERBL
        {
                // Compute double distribution.
                if ( DD.isZero() )                                                                          // TODO: Map with DDs for different t.
                {            
                    Eigen::VectorXd GPD_DGLAP(RT.x.size());

                    for ( int i = 0; i < RT.x.size(); i ++ )
                    {
                        ca  = -m_t*pow(1 - RT.x.at(i), 2.)/(4*m2*(1 - pow(RT.xi.at(i),2)));

                        GPD_DGLAP(i) = (213.32*pow(1 - m_x,2.)*(pow(m_x,2.) - pow(m_xi,2.))
                                   *sqrt( 1 - 2.9342*sqrt( (1 - m_x)/(1 - m_xi) )*sqrt( (m_x - m_xi)/(1 - m_xi) ) + 2.2911*((1 - m_x)/(1 - m_xi))*((m_x - m_xi)/(1 - m_xi))  )
                                   *sqrt( 1 - 2.9342*sqrt( (1 - m_x)/(1 + m_xi) )*sqrt( (m_x + m_xi)/(1 + m_xi) ) + 2.2911*((1 - m_x)/(1 + m_xi))*((m_x + m_xi)/(1 + m_xi)) ))
                                   *(3 + ((1 - 2 * ca) * atanh(sqrt(ca/(1+ca))))/((1 + c) * sqrt(ca/(1 + ca))) )
                                   /( pow(1 - pow(m_xi,2.),2.) * pow(1 + ca,2.) );
                    }
                
                    DD = RT.computeDD( GPD_DGLAP );
                }

                // Compute "gauged" GPD.
                uVal = RT.computeGPD( DD, m_x, m_xi );
                uValM = RT.computeGPD( DD, -m_x, m_xi );

                // Compute Dterm contribution.
                if ( DDt0.isZero() )                                                                        // TODO: Overload computeDterm so that it accepts (DDt0, x, xi) and (x, xi) as arguments. In that way, we can here look for DDt0 and choose the function computeDterm to be called.                                                                     
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
            
                uVal += dt*RT.computeDterm( DDt0, m_x, m_xi );
                uValM += dt*RT.computeDterm( DDt0, -m_x, m_xi );
        }
    }

    double uSea  = 0.;
    double uSeaM = 0.;

    quarkDistributionUp.setQuarkDistribution(uVal + uSea);
    quarkDistributionUp.setQuarkDistributionPlus(uVal + uSea - uValM - uSeaM);
    quarkDistributionUp.setQuarkDistributionMinus(uVal + uValM);
        
    // d-quark    
    double dVal = -uValM;
    double dValM = -uVal;
    double dSea = 0.;
    double dSeaM = 0.;

    quarkDistributionDown.setQuarkDistribution(dVal + dSea);
    quarkDistributionDown.setQuarkDistributionPlus(dVal + dSea - dValM - dSeaM);
    quarkDistributionDown.setQuarkDistributionMinus(dVal + dValM);

    // s-quark
    double sVal = 0.;
    double sValM = 0.;
    double sSea = 0.;
    double sSeaM = 0.;

    quarkDistributionStrange.setQuarkDistribution(sVal + sSea);
    quarkDistributionStrange.setQuarkDistributionPlus(sVal + sSea - sValM - sSeaM);
    quarkDistributionStrange.setQuarkDistributionMinus(sVal + sValM);

    // c-quark
    double cVal = 0.;
    double cValM = 0.;
    double cSea = 0.;
    double cSeaM = 0.;

    quarkDistributionCharm.setQuarkDistribution(cVal + cSea);
    quarkDistributionCharm.setQuarkDistributionPlus(cVal + cSea - cValM - cSeaM);
    quarkDistributionCharm.setQuarkDistributionMinus(cVal + cValM);

    // b-quark
    double bVal = 0.;
    double bValM = 0.;
    double bSea = 0.;
    double bSeaM = 0.;

    quarkDistributionBottom.setQuarkDistribution(bVal + bSea);
    quarkDistributionBottom.setQuarkDistributionPlus(bVal + bSea - bValM - bSeaM);
    quarkDistributionBottom.setQuarkDistributionMinus(bVal + bValM);

    // t-quark
    double tVal = 0.;
    double tValM = 0.;
    double tSea = 0.;
    double tSeaM = 0.;

    quarkDistributionTop.setQuarkDistribution(tVal + tSea);
    quarkDistributionTop.setQuarkDistributionPlus(tVal + tSea - tValM - tSeaM);
    quarkDistributionTop.setQuarkDistributionMinus(tVal + tValM);


    // Fill-in parton distribution objects with defined quark and gluon distributions.
    partonDistribution.addQuarkDistribution(quarkDistributionUp);
    partonDistribution.addQuarkDistribution(quarkDistributionDown);
    partonDistribution.addQuarkDistribution(quarkDistributionStrange);
    partonDistribution.addQuarkDistribution(quarkDistributionCharm);
    partonDistribution.addQuarkDistribution(quarkDistributionBottom);
    partonDistribution.addQuarkDistribution(quarkDistributionTop);

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
