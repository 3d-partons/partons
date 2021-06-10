/**
  * @file    algebraicToyModel.cpp
  * @author  José Manuel Morgado Chavez (University Of Huelva)
  * @author  Cédric Mezrag (CEA Saclay)
  * @date    Friday 16th April 2021 
  * @version 1.0
  */

#include "../../../../../include/partons/modules/gpd/pionGPD/algebraicToyModel.h"

#include "../../../../../include/partons/BaseObjectRegistry.h"
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

    const std::string algebraicToyModel::COVARIANT_EXTENSION = "RT";

// With this line we "create" the name for our GPD module. This will be integrated into the factory and thus partons knows about it.
const unsigned int algebraicToyModel::classId =
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new algebraicToyModel("algebraicToyModel"));

algebraicToyModel::algebraicToyModel(const std::string &className) : PARTONS::GPDModule(className)
{    
    // Set reference factorization scale.
    m_MuF2_ref = pow(0.331,2.);

    //Relate a specific GPD type with the appropriate function
    m_listGPDComputeTypeAvailable.insert(
        std::make_pair(PARTONS::GPDType::H, &PARTONS::GPDModule::computeH));
}

algebraicToyModel::algebraicToyModel(const algebraicToyModel& other) : PARTONS::GPDModule(other) 
{    
    m2 = pow(0.318,2.);                                                                                             // Mass parameter algebraic toy model. Eq (30) Physics Letters B 780 (2018) 287–293.
    m2D = pow(0.318,2.);                                                                                            // D-term t-dependence: Fitting of Phys. Rev. D 97, 014020 (2018) gravitational FFs.
    // RT.init();                                                                                              // Initialize Radon transform
}

algebraicToyModel::~algebraicToyModel() 
{

}

algebraicToyModel* algebraicToyModel::clone() const 
{
    return new algebraicToyModel(*this);
}

void algebraicToyModel::resolveObjectDependencies() 
{

}

void algebraicToyModel::configure(const ElemUtils::Parameters &parameters) 
{
    PARTONS::GPDModule::configure(parameters);

    if (parameters.isAvailable(algebraicToyModel::COVARIANT_EXTENSION)) {
        initRT();
        info(__func__, ElemUtils::Formatter() << algebraicToyModel::COVARIANT_EXTENSION
	    << "Configured - Radon transform matrix built.");
    }

}

void algebraicToyModel::isModuleWellConfigured() 
{
    PARTONS::GPDModule::isModuleWellConfigured();
}

void algebraicToyModel::initModule() 
{
    PARTONS::GPDModule::initModule();
}

PARTONS::PartonDistribution algebraicToyModel::computeH() 
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
    double uVal1, uValM1;

    if ( m_t == 0)                                                                                          //! Zero momentum transfer
    {
        if ( m_x > m_xi || m_x == m_xi )                                                                    // DGLAP>
        {
            uVal  = 30 * pow(1 - m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) / pow( 1 - pow(m_xi,2.) , 2.);
            uValM = 0.;

            uVal1 = uVal;
            uValM1 = uValM; 

        } else if ( m_x < -m_xi || m_x == -m_xi )                                                           //DGLAP>
        {
            uVal = 0.;
            uValM = 30 * pow(1 + m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) / pow( 1 - pow(m_xi,2.) , 2.);

            uVal1 = uVal;
            uValM1 = uValM;
        } else                                                                                              // ERBL
        {           
            // ============================================================================================
            // Compute ERBL GPD (Proper computation: RT)
            // ============================================================================================
            
            // // Compute double distribution.
            // if ( DDt0.isZero() )                                                                               
            // {
            //     Eigen::VectorXd GPD_DGLAP(RT.x.size());

            //     for ( int i = 0; i < RT.x.size(); i ++ )
            //     {
            //         GPD_DGLAP(i) = 30 * pow(1 - RT.x.at(i), 2.) * ( pow(RT.x.at(i),2.) - pow(RT.xi.at(i),2.) ) / pow( 1 - pow(RT.xi.at(i),2.) , 2.);
            //     }
                
            //     DDt0 = RT.computeDD( GPD_DGLAP );
            // }

            // // Compute "gauged" GPD.
            // uVal = RT.computeGPD( DDt0, m_x, m_xi );
            // uValM = RT.computeGPD( DDt0, -m_x, m_xi );

            // // Compute Dterm contribution.
            // uVal += RT.computeDterm( DDt0, m_x, m_xi );
            // uValM += RT.computeDterm( DDt0, -m_x, m_xi );

            // ============================================================================================
            // Compute ERBL GPD (Analytic computation)
            // ============================================================================================

            // (Gauged) ERBL GPD t = 0
            uVal1 = 7.5 * (1 - m_x) * ( pow(m_xi, 2.) - pow(m_x, 2.) ) * (m_x + 2*m_x*m_xi + pow(m_xi, 2.)) / ( pow(m_xi, 3.)*pow(1 + m_xi, 2.) );
            uValM1 = 7.5 * (1 + m_x) * ( pow(m_xi, 2.) - pow(m_x, 2.) ) * (-m_x - 2*m_x*m_xi + pow(m_xi, 2.)) / ( pow(m_xi, 3.)*pow(1 + m_xi, 2.) ); 

            // D-terms contribution
            alpha1 = m_x/m_xi;

            dplus1 = 1.125*(1-pow(alpha1,2))*(5*pow(alpha1,2)-1);
            dminus1 = -3.75*alpha1*(1-pow(alpha1,2));
    
            // Add D-terms to GPD.
            dplus1 /= m_xi;                                                                                     
        
            if ( m_xi >= 0 )                                                                                // Conditional expression taking into acount the factor sign(\xi) accompanying dminus.
            {
                uVal1 +=  dplus1 + dminus1;
                uValM1 += dplus1 - dminus1;
            } else
            {
                uVal1 += dplus1 - dminus1;
                uValM1 += dplus1 + dminus1;
            }
        }
    } else                                                                                                  //! Non-vanishing momentum transfer. CHECK!!!!!!!!!!!!!!!!!!!
    {
        c  = -m_t*pow(1 - m_x, 2.)/(4*m2*(1 - pow(m_xi,2)));                                                // t-dependence algebraic toy model.
        cM = -m_t*pow(1 + m_x, 2.)/(4*m2*(1 - pow(m_xi,2)));
        c1  = -m_t/(4*m2);

        dt = 1/(1-0.25*m_t/m2D);                                                                            // D-term t-dependence (monopole parametrization).

        if ( m_x > m_xi || m_x == m_xi )                                                                    // DGLAP>
        {
            uVal  = 7.5 * pow(1 - m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) * (3 + ((1 - 2 * c) * atanh(sqrt(c/(1+c))))/((1 + c) * sqrt(c/(1 + c))) )
                / ( pow( 1 - pow(m_xi,2.) , 2.) * pow(1 + c,2.) );
            uValM = 0.;

            uVal1 = uVal;
            uValM1 = uValM;

            if ( m_x == 1 )                                                                                 // Actually this is the limit x->1 (with \xi<1). 
            {
                uVal = 0.;
                uValM = 0.;

                uVal1 = uVal;
                uValM1 = uValM;
            }

        } else if ( m_x < -m_xi || m_x == -m_xi )                                                            // DGLAP<
        {
            uVal  = 0.;
            uValM = 7.5 * pow(1 + m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) * (3 + ((1 - 2 * cM) * atanh(sqrt(cM/(1+cM))))/((1 + cM) * sqrt(cM/(1 + cM))) )
                / ( pow( 1 - pow(m_xi,2.) , 2.) * pow(1 + cM,2.) );

            uVal1 = uVal;
            uValM1 = uValM;

            if ( m_x == -1 ) // Actually this is the limit x->1 (with \xi<1). 
            {
                uVal = 0.;
                uValM = 0.;

                uVal1 = uVal;
                uValM1 = uValM;
            }

        } else                                                                                              // ERBL
        {
            // ============================================================================================
            // Compute ERBL GPD (Proper computation: RT)
            // ============================================================================================
            
            // Compute double distribution.
            // if ( DD.isZero() )                                                                              // TODO: Map with DDs for different t.
            // {            
            //     Eigen::VectorXd GPD_DGLAP(RT.x.size());

            //     for ( int i = 0; i < RT.x.size(); i ++ )
            //     {
            //         ca  = -m_t*pow(1 - RT.x.at(i), 2.)/(4*m2*(1 - pow(RT.xi.at(i),2)));

            //         GPD_DGLAP(i) = 7.5 * pow(1 - RT.x.at(i), 2.) * ( pow(RT.x.at(i),2.) - pow(RT.xi.at(i),2.) ) * (3 + ((1 - 2 * ca) * atanh(sqrt(ca/(1+ca))))/((1 + ca) * sqrt(ca/(1 + ca))) )
            //                        / ( pow( 1 - pow(RT.xi.at(i),2.) , 2.) * pow(1 + ca,2.) );
            //     }
                
            //     DD = RT.computeDD( GPD_DGLAP );
            // }

            // // Compute "gauged" GPD.
            // uVal = RT.computeGPD( DD, m_x, m_xi );
            // uValM = RT.computeGPD( DD, -m_x, m_xi );

            // // Compute Dterm contribution.
            // if ( DDt0.isZero() )                                                                             // TODO: Overload computeDterm so that it accepts (DDt0, x, xi) and (x, xi) as arguments. In that way, we can here look for DDt0 and choose the function computeDterm to be called.                                                                     
            // {
            //     Eigen::VectorXd GPD_DGLAP(RT.x.size());
                
            //     for ( int i = 0; i < RT.x.size(); i ++ )
            //     {
            //         GPD_DGLAP(i) = 30 * pow(1 - RT.x.at(i), 2.) * ( pow(RT.x.at(i),2.) - pow(RT.xi.at(i),2.) ) / pow( 1 - pow(RT.xi.at(i),2.) , 2.);
            //     }
                
            //     DDt0 = RT.computeDD( GPD_DGLAP );
            // }
            
            // uVal += dt*RT.computeDterm( DDt0, m_x, m_xi );
            // uValM += dt*RT.computeDterm( DDt0, -m_x, m_xi );

            // ============================================================================================
            // Compute ERBL GPD (Analytic computation)
            // ============================================================================================

            if ( m_xi == 1 )
            {
                uVal1 = -3.75*(1-pow(m_x,2))*( (((1-pow(m_x,2))+c1*(1-4*pow(m_x,3)+3*pow(m_x,4)))/pow(1+c1*(1-pow(m_x,2)),2)) - log(1+c1*(1-pow(m_x,2)))/c1 )
                    / (c1*pow(1-m_x,2));
                
                uValM1 = -3.75*(1-pow(m_x,2))*( (((1-pow(m_x,2))+c1*(1+4*pow(m_x,3)+3*pow(m_x,4)))/pow(1+c1*(1-pow(m_x,2)),2)) - log(1+c1*(1-pow(m_x,2)))/c1 )
                    / (c1*pow(1+m_x,2));

                if ( m_x == 1 )
                {
                    uVal1 = 0.;
                    uValM1 = 0.; // Verify this limit.
                }

            } else                                                                                  // The following expression shows a "fake" divergence in the limit \xi->1.
            {
                // (Gauged) ERBL GPD t < 0
                uVal1  = -3.75 * (pow(m_xi,2) - pow(m_x,2)) * (sqrt(c * (c + 1)) * ( -c*m_xi*(1 - pow(m_xi,2))*(1 - m_x) 
                    * (pow(m_xi,4) + 6*m_xi*(1-m_x)*pow(m_x,2)-6*pow(m_xi,3)*(1-m_x)+pow(m_xi,2)*(4-3*(3-m_x)*m_x)+m_x*(4-m_x*(8-5*m_x))) 
                    + pow(c,2)*pow(1-pow(m_xi,2),2)*(pow(m_xi,3)*(3*m_xi-2)+3*pow(m_x,4)-4*m_xi*pow(m_x,3)-6*(m_xi-1)*m_xi*pow(m_x,2)+2*m_xi*(pow(m_xi,2)-1)*m_x)
                    + pow(m_x-1,3)*(pow(m_xi,5)+3*pow(m_xi,4)*(m_x-1)+pow(m_xi,3)*(2-5*m_x)+2*m_xi*m_x))
                    + (1-2*c)*pow( c*(1-pow(m_xi,2))*(pow(m_xi,2)-pow(m_x,2))+pow(m_xi,2)*pow((m_x-1),2),2)
                    *(atanh(sqrt(c/(1+c)))-atanh(sqrt(c/(1+c))*(pow(m_xi,2)-m_x)/(m_xi*(1-m_x))))                )
                    / (pow(1+c,2.5)*pow(1-pow(m_xi,2),1.5)*pow(1-m_x,2)*sqrt(c*(1-pow(m_xi,2)))*pow(pow(m_xi,2)+c*(1-pow(m_xi,2))*(pow(m_xi,2)-pow(m_x,2))/pow(1-m_x,2),2));

                uValM1 = -3.75 * (pow(m_xi,2) - pow(m_x,2)) * ( sqrt(cM * (cM + 1)) * ( -cM*m_xi*(1 - pow(m_xi,2))*(1 + m_x) 
                    * (pow(m_xi,4) + 6*m_xi*(1+m_x)*pow(m_x,2)-6*pow(m_xi,3)*(1+m_x)+pow(m_xi,2)*(4+3*(3+m_x)*m_x)-m_x*(4+m_x*(8+5*m_x))) 
                    + pow(cM,2)*pow(1-pow(m_xi,2),2)*(pow(m_xi,3)*(3*m_xi-2)+3*pow(m_x,4)+4*m_xi*pow(m_x,3)-6*(m_xi-1)*m_xi*pow(m_x,2)-2*m_xi*(pow(m_xi,2)-1)*m_x)
                    - pow(m_x+1,3)*(pow(m_xi,5)-3*pow(m_xi,4)*(m_x+1)+pow(m_xi,3)*(2+5*m_x)-2*m_xi*m_x))
                    + (1-2*cM)*pow( cM*(1-pow(m_xi,2))*(pow(m_xi,2)-pow(m_x,2))+pow(m_xi,2)*pow(m_x+1,2),2)
                    *(atanh(sqrt(cM/(1+cM)))-atanh(sqrt(cM/(1+cM))*(pow(m_xi,2)+m_x)/(m_xi*(1+m_x)))))
                    / (pow(1+cM,2.5)*pow(1-pow(m_xi,2),1.5)*pow(1+m_x,2)*sqrt(cM*(1-pow(m_xi,2)))*pow(pow(m_xi,2)+cM*(1-pow(m_xi,2))*(pow(m_xi,2)-pow(m_x,2))/pow(1+m_x,2),2)); 
            }

            // D-terms contribution (monopole parametrization)
            alpha1 = m_x/m_xi;

            dplus1 = 1.125*(1-pow(alpha1,2))*(5*pow(alpha1,2)-1)*dt;
            dminus1 = -3.75*alpha1*(1-pow(alpha1,2))*dt;  
        
            // Add D-terms to GPD.
            dplus1 /= m_xi;                                                                                      
        
            if ( m_xi >= 0 )                                                          // Conditional expression taking into acount the factor sign(\xi) accompanying dminus.
            {
                uVal1 +=  dplus1 + dminus1;
                uValM1 += dplus1 - dminus1;
            } else
            {
                uVal1 += dplus1 - dminus1;
                uValM1 += dplus1 + dminus1;
            }
        }
    }

    double uSea  = 0.;
    double uSeaM = 0.;

    quarkDistributionUp.setQuarkDistribution(uVal1 + uSea);
    quarkDistributionUp.setQuarkDistributionPlus(uVal1 + uSea - uValM1 - uSeaM);
    quarkDistributionUp.setQuarkDistributionMinus(uVal1 + uValM1);
        
    // d-quark    
    double dVal = -uValM1;
    double dValM = -uVal1;
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

void algebraicToyModel::initRT()
{
    RT.init();
}

}
