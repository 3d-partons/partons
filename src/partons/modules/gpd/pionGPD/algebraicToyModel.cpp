/**
  * @file    algebraicToyModel.cpp
  * @author  José Manuel Morgado Chavez (University Of Huelva)
  * @author  Cédric Mezrag (CEA Saclay)
  * @date    23rd March 2021 
  * @version 1.0
  */

#include "../../../../../include/partons/modules/gpd/pionGPD/algebraicToyModel.h"

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
const unsigned int algebraicToyModel::classId =
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new algebraicToyModel("algebraicToyModel"));

algebraicToyModel::algebraicToyModel(const std::string &className) : PARTONS::GPDModule(className)
{    
    // Set reference factorization scale.
    m_MuF2_ref = pow(0.5,2.);                                                                              // TODO: Set equal to value given in reference paper for \alpha_PI: \mu_H = 0,33 GeV.

    //Relate a specific GPD type with the appropriate function
    m_listGPDComputeTypeAvailable.insert(
        std::make_pair(PARTONS::GPDType::H, &PARTONS::GPDModule::computeH));
}

algebraicToyModel::algebraicToyModel(const algebraicToyModel& other) : PARTONS::GPDModule(other) 
{    
    m2 = 0.101;  // Mass parameter algebraic toy model. Eq (30) Physics Letters B 780 (2018) 287–293.
    m2D = 0.099; // D-term t-dependence: Fitting of Phys. Rev. D 97, 014020 (2018) gravitational FFs.
}

algebraicToyModel::~algebraicToyModel() 
{

}

algebraicToyModel* algebraicToyModel::clone() const 
{
    std::cout << "HELLO \n";

    return new algebraicToyModel(*this);
}

void algebraicToyModel::resolveObjectDependencies() 
{

}

void algebraicToyModel::configure(const ElemUtils::Parameters &parameters) 
{
    PARTONS::GPDModule::configure(parameters);
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

    // u-quark
    double uVal, uValM;

    if ( m_t == 0)                                                                                          //! Zero momentum transfer
    {
        if ( m_x > m_xi )                                                                                   // DGLAP>
        {
            uVal  = 30 * pow(1 - m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) / pow( 1 - pow(m_xi,2.) , 2.);
            uValM = 0.;
        } else if ( m_x < -m_xi )                                                                           //DGLAP>
        {
            uVal = 0.;
            uValM = 30 * pow(1 + m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) / pow( 1 - pow(m_xi,2.) , 2.);   
        } else                                                                                              // ERBL
        {           
            if ( DDt0.isZero() )                                                                               
            {
                Eigen::VectorXd GPD_DGLAP(RT.x.size());
                
                for ( int i = 0; i < RT.x.size(); i ++ )
                {
                    GPD_DGLAP(i) = 30 * pow(1 - RT.x.at(i), 2.) * ( pow(RT.x.at(i),2.) - pow(RT.xi.at(i),2.) ) / pow( 1 - pow(RT.xi.at(i),2.) , 2.);
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

            // ============================================================================================
            // Compute ERBL GPD (Analytic computation)
            // ============================================================================================

            // // (Gauged) ERBL GPD t = 0
            // uVal = 7.5 * (1 - m_x) * ( pow(m_xi, 2.) - pow(m_x, 2.) ) * (m_x + 2*m_x*m_xi + pow(m_xi, 2.)) / ( pow(m_xi, 3.)*pow(1 + m_xi, 2.) );
            // uValM = 7.5 * (1 + m_x) * ( pow(m_xi, 2.) - pow(m_x, 2.) ) * (-m_x - 2*m_x*m_xi + pow(m_xi, 2.)) / ( pow(m_xi, 3.)*pow(1 + m_xi, 2.) ); 

            // // D-terms contribution
            // alpha = m_x/m_xi;

            // dplus = 1.125*(1-pow(alpha,2))*(5*pow(alpha,2)-1);
            // dminus = -3.75*alpha*(1-pow(alpha,2));
    
            // // Add D-terms to GPD.
            // dplus /= m_xi;                                                                                     
        
            // if ( m_xi >= 0 )                                                                            // Conditional expression taking into acount the factor sign(\xi) accompanying dminus.
            // {
            //     uVal +=  dplus + dminus;
            //     uValM += dplus - dminus;
            // } else
            // {
            //     uVal += dplus - dminus;
            //     uValM += dplus + dminus;
            // }    
        }
    } else                                                                                                  //! Non-vanishing momentum transfer.
    {
        c  = -m_t*pow(1 - m_x, 2.)/(4*m2*(1 - pow(m_xi,2)));                                                // t-dependence algebraic toy model.
        cM = -m_t*pow(1 + m_x, 2.)/(4*m2*(1 - pow(m_xi,2)));

        dt = 1/(1-0.25*m_t/m2D);                                                                             // D-term t-dependence (monopole parametrization).

        if ( m_x > m_xi )                                                                                   // DGLAP>
        {
            uVal  = 7.5 * pow(1 - m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) * (3 + ((1 - 2 * c) * atanh(sqrt(c/(1+c))))/((1 + c) * sqrt(c/(1 + c))) )
                / ( pow( 1 - pow(m_xi,2.) , 2.) * pow(1 + c,2.) );
            uValM = 0.;
        } else if ( m_x < -m_xi )                                                                           // DGLAP>
        {
            uVal  = 0.;
            uValM = 7.5 * pow(1 + m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) * (3 + ((1 - 2 * cM) * atanh(sqrt(cM/(1+cM))))/((1 + cM) * sqrt(cM/(1 + cM))) )
                / ( pow( 1 - pow(m_xi,2.) , 2.) * pow(1 + cM,2.) );
        } else                                                                                              // ERBL
        {
            // if ( DD.empty() )
            // {
                // ============================================================================================
                // Compute DD (Proper computation)
                // ============================================================================================

                // computeDD( 0 );                                                                                
                
                // ============================================================================================
                // Compute DD (Reading from file)
                // ============================================================================================

                // mesh = setMesh();                                                                        // Set Mesh according to the computations that have been carried out.
                     
                // ifstream DoubleDistribution;                                                                 
                // DoubleDistribution.open("/home/jose/git/partons_computations/data/RadonTransform/DD/DDt06.dat");
 
                // string linedd;
                // double d;

                // if ( DoubleDistribution )
                // {
                //     while( getline(DoubleDistribution,linedd) )
                //     {
                //         istringstream iss(linedd);
                //         if ( !(iss >> d) )
                //         {
                //             throw runtime_error( "DD file does not have the correct format: vector<double>" );
                //         } else
                //         {
                //             DD.push_back(d);
                //         }
                //     }
 
                // } else
                // {
                //     throw runtime_error( "File not found." );
                // }
                
 
                // DoubleDistribution.close();

                // ============================================================================================
                // ============================================================================================
            // }
            
            // ============================================================================================
            // Compute ERBL GPD (Proper computation: RT)
            // ============================================================================================

            // NumA::RadonTransform RT;                                                                        // Radon transform matrix for uVal.
            // NumA::RadonTransform RTminus;                                                                   // Radon transform matrix for uValM.

            // x[0] = m_x; xi[0] = m_xi; y[0] = m_xi/m_x;                                                      // Kinematics written in proper format.
            // xm[0] = -m_x; xim[0] = m_xi; ym[0] = -m_xi/m_x;

            // RT.RTMatrix.clear();                                                                            // Radon transform matrix for uVal.
            // RT.build_RTmatrix(mesh, x, y, xi );

            // RTminus.RTMatrix.clear();                                                                       // Radon transform matrix for uValM.
            // RTminus.build_RTmatrix(mesh, xm, ym, xim);

            // for ( int i = 0; i < DD.size() ; i++ )
            // {
            //     uVal += RT.RTMatrix[0][i]*DD[i];
            //     uValM += RTminus.RTMatrix[0][i]*DD[i];                                                      // !Check if this is correct. Check computation of uValM.
            // }


            // // D-terms contribution                                                                            // TODO: Implement computation of D-terms in RT.
            // if ( DtermsVec.size() == 0 )
            // {                    
            //     DtermsVec = computeDterms();

            //     // Interpolate numerically computed D-terms.
            //     Dminus = new NumA::CubicSpline(DtermsVec[2],DtermsVec[0]);
            //     Dplus = new NumA::CubicSpline(DtermsVec[2],DtermsVec[1]);
                 
            //     Dminus->ConstructSpline();
            //     Dplus->ConstructSpline(); 
            // }

            // // Add D-terms to GPD.
            // alpha = m_x/m_xi;

            // if ( m_xi >= 0 )                                                                                // Conditional expression taking into acount the factor sign(\xi) accompanying dminus.
            // {
            //     uVal  +=  dt*(Dplus->getSplineInsideValue(alpha)/m_xi + Dminus->getSplineInsideValue(alpha));
            //     uValM += dt*(Dplus->getSplineInsideValue(alpha)/m_xi - Dminus->getSplineInsideValue(alpha));
            // } else
            // {
            //     uVal  += dt*(Dplus->getSplineInsideValue(alpha)/m_xi - Dminus->getSplineInsideValue(alpha));
            //     uValM += dt*(Dplus->getSplineInsideValue(alpha)/m_xi + Dminus->getSplineInsideValue(alpha));
            // }

            // ============================================================================================
            // Compute ERBL GPD (Analytic computation)
            // ============================================================================================

            // (Gauged) ERBL GPD t < 0
            uVal  = -3.75 * (pow(m_xi,2) - pow(m_x,2)) * (sqrt(c * (c + 1)) * ( -c*m_xi*(1 - pow(m_xi,2))*(1 - m_x) 
                * (pow(m_xi,4) + 6*m_xi*(1-m_x)*pow(m_x,2)-6*pow(m_xi,3)*(1-m_x)+pow(m_xi,2)*(4-3*(3-m_x)*m_x)+m_x*(4-m_x*(8-5*m_x))) 
                + pow(c,2)*pow(1-pow(m_xi,2),2)*(pow(m_xi,3)*(3*m_xi-2)+3*pow(m_x,4)-4*m_xi*pow(m_x,3)-6*(m_xi-1)*m_xi*pow(m_x,2)+2*m_xi*(pow(m_xi,2)-1)*m_x)
                + pow(m_x-1,3)*(pow(m_xi,5)+3*pow(m_xi,4)*(m_x-1)+pow(m_xi,3)*(2-5*m_x)+2*m_xi*m_x))
                + (1-2*c)*pow( c*(1-pow(m_xi,2))*(pow(m_xi,2)-pow(m_x,2))+pow(m_xi,2)*pow((m_x-1),2),2)
                *(atanh(sqrt(c/(1+c)))-atanh(sqrt(c/(1+c))*(pow(m_xi,2)-m_x)/(m_xi*(1-m_x))))                )
                / (pow(1+c,2.5)*pow(1-pow(m_xi,2),1.5)*pow(1-m_x,2)*sqrt(c*(1-pow(m_xi,2)))*pow(pow(m_xi,2)+c*(1-pow(m_xi,2))*(pow(m_xi,2)-pow(m_x,2))/pow(1-m_x,2),2));

            uValM = -3.75 * (pow(m_xi,2) - pow(m_x,2)) * ( sqrt(cM * (cM + 1)) * ( -cM*m_xi*(1 - pow(m_xi,2))*(1 + m_x) 
                * (pow(m_xi,4) + 6*m_xi*(1+m_x)*pow(m_x,2)-6*pow(m_xi,3)*(1+m_x)+pow(m_xi,2)*(4-3*(3+m_x)*(-1)*m_x)-m_x*(4+m_x*(8+5*m_x))) 
                + pow(cM,2)*pow(1-pow(m_xi,2),2)*(pow(m_xi,3)*(3*m_xi-2)+3*pow(m_x,4)-4*m_xi*pow(-m_x,3)-6*(m_xi-1)*m_xi*pow(m_x,2)+2*m_xi*(pow(m_xi,2)-1)*(-1)*m_x)
                + pow(-m_x-1,3)*(pow(m_xi,5)+3*pow(m_xi,4)*(-m_x-1)+pow(m_xi,3)*(2+5*m_x)+2*m_xi*(-1)*m_x))
                + (1-2*cM)*pow( cM*(1-pow(m_xi,2))*(pow(m_xi,2)-pow(m_x,2))+pow(m_xi,2)*pow((-m_x-1),2),2)
                *(atanh(sqrt(cM/(1+cM)))-atanh(sqrt(cM/(1+cM))*(pow(m_xi,2)-m_x)/(-m_xi*(1+m_x)))))
                / (pow(1+cM,2.5)*pow(1-pow(m_xi,2),1.5)*pow(1+m_x,2)*sqrt(cM*(1-pow(m_xi,2)))*pow(pow(m_xi,2)+cM*(1-pow(m_xi,2))*(pow(m_xi,2)-pow(m_x,2))/pow(1+m_x,2),2)); 

            // D-terms contribution (monopole parametrization)
            alpha = m_x/m_xi;

            dplus = 1.125*(1-pow(alpha,2))*(5*pow(alpha,2)-1)*dt;
            dminus = -3.75*alpha*(1-pow(alpha,2))*dt;  
        
            // Add D-terms to GPD.
            dplus /= m_xi;                                                                                      
        
            if ( m_xi >= 0 )                                                          // Conditional expression taking into acount the factor sign(\xi) accompanying dminus.
            {
                uVal +=  dplus + dminus;
                uValM += dplus - dminus;
            } else
            {
                uVal += dplus - dminus;
                uValM += dplus + dminus;
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

// std::vector<std::vector<double>> algebraicToyModel::computeDterms()
// {
//     // Evaluation points.
//     double mx = 1000;               // Number of points.
//     double st = 2/mx;               // Step.
    
//     std::vector<std::vector<double>> Dterms(3, std::vector<double>(mx) );
    
//     if ( DDt0.empty() )
//     {
//         // ============================================================================================
//         // Compute DD (Proper computation)
//         // ============================================================================================

//         // computeDD();                                                                             
                
//         // ============================================================================================
//         // Compute DD (Reading from file)
//         // ============================================================================================

//         mesh = setMesh();                                                                        // Set Mesh according to the computations that have been carried out.
                     
//         ifstream DoubleDistribution;                                                                 
//         // // DoubleDistribution.open("/usr/local/share/data/DoubleDistribution/AlgebraicToyModel/AverageDD-50it-1e-7-P1.dat");
//         DoubleDistribution.open("/home/jose/codes/PARTONS/data/kinematics/GPD/Evolution_kinematics/DD.dat");//! CREATE A DATA DIRECTORY CONTAINING DDs FOR THIS PURPOSE
//         string linedd;
//         double d;

//         if ( DoubleDistribution )
//         {
//             while( getline(DoubleDistribution,linedd) )
//             {
//                 istringstream iss(linedd);
//                 if ( !(iss >> d) )
//                 {
//                     throw runtime_error( "DD file does not have the correct format: vector<double>" );
//                 } else
//                 {
//                     DDt0.push_back(d);
//                 }
//             }
//             } else
//             {
//                 throw runtime_error( "File not found." );
//             }
                
//         DoubleDistribution.close();
//     }
    
//     for (int i = 0; i < mx; i++ )
//     {
//         Dterms[2][i] = -1.+i*st;
//     }
//     Dterms[0] = computeDminus( mesh, DDt0, Dterms[2] );
//     Dterms[1] = computeDplus( mesh, DDt0, Dterms[2] );

// return Dterms;
// }

// vector<double> algebraicToyModel::computeDminus( NumA::Mesh& mesh, const vector<double>& DDt0, vector<double>& x )
// {
//     // Dminus = 0.5(H(-x,1,0) - H(x,1,0))

//     NumA::RadonTransform RT;
//     NumA::RadonTransform RTminus;

//     std::vector<double> xi(x.size(),1.), y(x.size());

//     for (int i = 0; i < x.size(); i++ )
//     {
//         y[i] = 1./x[i];
//     }

//     RT.RTMatrix.clear();
//     RT.build_RTmatrix(mesh, x, y, xi);

//     for (int i = 0; i < x.size(); i++ )
//     {
//         x[i] = -x[i];
//         y[i] = -y[i];
//     }

//     RTminus.RTMatrix.clear();
//     RTminus.build_RTmatrix(mesh, x, y, xi);

//     // H(x,1,0)
//     std::vector<double> Hx1(x.size());

//     for ( int i = 0; i < x.size(); i++ )
//     {
//         for ( int j = 0; j < DDt0.size(); j++ )
//         {
//             Hx1[i] += RT.RTMatrix[i][j]*DDt0[j];
//         }
//     }

//     //H(-x,1,0)
//     std::vector<double> Hx1minus (x.size());
   
//     for ( int i = 0; i < x.size(); i++ )
//     {
//         for ( int j = 0; j < DDt0.size(); j++ )
//         {
//             Hx1minus[i] += RTminus.RTMatrix[i][j]*DDt0[j];
//         }
//     }

//     // Dminus
//     std::vector<double> DminusVec (x.size());

//     for( int i = 0; i < x.size(); i++)
//     {
//         DminusVec[i] = 0.5*(Hx1minus[i]-Hx1[i]);
//     }

// return DminusVec;
// }

// std::vector<double> algebraicToyModel::computeDplus( NumA::Mesh& mesh, const vector<double>& DDt0, vector<double>& x )
// {
//     // Dplus = 0.5(phi((1+x)/2) - H(-x,1,0) - H(x,1,0))

//     NumA::RadonTransform RT;
//     NumA::RadonTransform RTminus;

//     std::vector<double> xi(x.size(),1.), y(x.size());

//     for (int i = 0; i < x.size(); i++ )
//     {
//         y[i] = 1./x[i];
//     }

//     RT.RTMatrix.clear();
//     RT.build_RTmatrix(mesh, x, y, xi);

//     for (int i = 0; i < x.size(); i++ )
//     {
//         x[i] = -x[i];
//         y[i] = -y[i];
//     }

//     RTminus.RTMatrix.clear();
//     RTminus.build_RTmatrix(mesh, x, y, xi);

//     // H(x,1,0)
//     std::vector<double> Hx1(x.size());

//     for ( int i = 0; i < x.size(); i++ )
//     {
//         for ( int j = 0; j < DDt0.size(); j++ )
//         {
//             Hx1[i] += RT.RTMatrix[i][j]*DDt0[j];
//         }
//     }

//     //H(-x,1,0)
//     std::vector<double> Hx1minus (x.size());
   
//     for ( int i = 0; i < x.size(); i++ )
//     {
//         for ( int j = 0; j < DDt0.size(); j++ )
//         {
//             Hx1minus[i] += RTminus.RTMatrix[i][j]*DDt0[j];
//         }
//     }

//     // Dplus
//     std::vector<double> DplusVec (x.size());

//     for( int i = 0; i < x.size(); i++)
//     {
//         DplusVec[i] = 0.5*(1.5*(1-pow(x[i],2))-Hx1minus[i]-Hx1[i]);
//     }

// return DplusVec;
// }

}
