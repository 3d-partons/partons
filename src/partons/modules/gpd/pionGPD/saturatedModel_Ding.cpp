/**
 * @file saturatedModel_Ding.cpp
 * @author José Manuel Morgado Chavez (University Of Huelva)
 * @author Cédric Mezrag (CEA Saclay)
 * @date 12th February 2021 
 * @version 1.0
 */

/**
 * Headers for setMesh to read mesh attributes from a external file
 * To be removed.
 */

#include <fstream>
#include <sstream>

// TODO: Clean list of headers. Check which ones are necessary.

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
#include <NumA/triangulation/mesh.h>
#include <NumA/triangulation/point.h>

#include <NumA/linear_algebra/least_squares/LSMRSolver.h>
#include <NumA/linear_algebra/matrix/MatrixD.h>
#include <NumA/linear_algebra/vector/VectorD.h>

#include <random>
#include <ctime>

// Definitions required by triangle external software
#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */

#define VOID int

#define ANSI_DECLARATORS

extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <NumA/triangulation/software/triangle.h>
}

// NumA interpolation (D-terms)
#include <NumA/interpolation/CubicSpline.h>

namespace PARTONS {

// With this line we "create" the name for our GPD module. This will be integrated into the factory and thus partons knows about it.
const unsigned int saturatedModel_Ding::classId =
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new saturatedModel_Ding("saturatedModel_Ding"));

saturatedModel_Ding::saturatedModel_Ding(const std::string &className) : PARTONS::GPDModule(className)
{
    // Set reference factorization scale.
    m_MuF2_ref = 0.4;                                                                              // TODO: Set equal to value given in reference paper for \alpha_PI: \mu_H = 0,33 GeV.

    //Relate a specific GPD type with the appropriate function
    m_listGPDComputeTypeAvailable.insert(
        std::make_pair(PARTONS::GPDType::H, &PARTONS::GPDModule::computeH));
}

saturatedModel_Ding::saturatedModel_Ding(const saturatedModel_Ding& other) : PARTONS::GPDModule(other) 
{
    m2 = 0.101; // Mass paramter monopole parametrization for t-dependence.
                // TODO: Fix value (this is the one for the algebraic model.)
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
    
    std::vector<double> x(1), y(1), xi(1), xm(1), ym(1), xim(1);                                            // Declare kinematics in a format appropiated for RadonTransform.

    PARTONS::PartonDistribution partonDistribution;

    // Quark distributions
    QuarkDistribution quarkDistributionUp(QuarkFlavor::UP);
    QuarkDistribution quarkDistributionDown(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistributionStrange(QuarkFlavor::STRANGE);

    // u-quark
    double uVal, uValM;

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
        if ( DDt0.empty() )                                                                               
        {
            // ============================================================================================
            // Compute DD (Proper computation)
            // ============================================================================================

            // computeDD();                                                                             
                
            // ============================================================================================
            // Compute DD (Reading from file)
            // ============================================================================================

            mesh = setMesh();                                                                        // Set Mesh according to the computations that have been carried out.
                     
            ifstream DoubleDistribution;                                                                 
            DoubleDistribution.open("/home/jose/codes/PARTONS/data/kinematics/GPD/Evolution_kinematics/DD.dat");    //! CREATE A DATA DIRECTORY CONTAINING DDs FOR THIS PURPOSE
            string linedd;
            double d;

            if ( DoubleDistribution )
            {
                while( getline(DoubleDistribution,linedd) )
                {
                    istringstream iss(linedd);
                    if ( !(iss >> d) )
                    {
                        throw runtime_error( "DD file does not have the correct format: vector<double>" );
                    } else
                    {
                        DDt0.push_back(d);
                    }
                }
            } else
            {
                throw runtime_error( "File not found." );
            }         
            
            DoubleDistribution.close();
        }

        // ============================================================================================
        // Compute ERBL GPD (Proper computation: RT)
        // ============================================================================================

        NumA::RadonTransform RT;                                                                        // Radon transform matrix for uVal.
        NumA::RadonTransform RTminus;                                                                   // Radon transform matrix for uValM.
 
        x[0] = m_x; xi[0] = m_xi; y[0] = m_xi/m_x;                                                      // Kienmatics written in proper format.
        xm[0] = -m_x; xim[0] = m_xi; ym[0] = -m_xi/m_x;
 
        RT.RTMatrix.clear();                                                                            // Radon transform matrix for uVal.
        RT.build_RTmatrix(mesh, x, y, xi );
 
        RTminus.RTMatrix.clear();                                                                       // Radon transform matrix for uValM.
        RTminus.build_RTmatrix(mesh, xm, ym, xim);
 
        for ( int i = 0; i < DDt0.size() ; i++ )
        {
            uVal += RT.RTMatrix[0][i]*DDt0[i];
            uValM += RTminus.RTMatrix[0][i]*DDt0[i];
        }

        // D-terms contribution                                                                         // TODO: Implement computation of D-terms in RT.
        if ( DtermsVec.size() == 0 )
        {                    
            DtermsVec = computeDterms();

            // Interpolate numerically computed D-terms.
            Dminus = new NumA::CubicSpline(DtermsVec[2],DtermsVec[0]);
            Dplus = new NumA::CubicSpline(DtermsVec[2],DtermsVec[1]);
                 
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

    // Global factorized t-dependece.
    if ( !(m_t == 0) )
    {
        dt = 1/(1-0.25*m_t/m2);
        
        uVal  *= dt;
        uValM *= dt;
    }

    // TODO: Check and fix definitions of the the different quark distributions (u, uM, u+, u-, d(...) and s(...)).
    double uSea  = 0.;
    double uSeaM = 0.;

    quarkDistributionUp.setQuarkDistribution(uVal + uSea);

        // Singlet distribution
        quarkDistributionUp.setQuarkDistributionPlus(uVal - uValM);
        // Nonsinglet distributiion
        quarkDistributionUp.setQuarkDistributionMinus(uVal + uSea + uValM + uSeaM);
        
    // s-quark    
    double dVal = -uValM;
    double dValM = -uVal;
    double dSea = 0.;
    double dSeaM = 0.;

    quarkDistributionDown.setQuarkDistribution(dVal + dSea);

        // Singlet distribution
        quarkDistributionUp.setQuarkDistributionPlus(dVal - dValM);
        // Nonsinglet distributiion
        quarkDistributionUp.setQuarkDistributionMinus(dVal + dSea + dValM + dSeaM);

    // s-quark
    double sVal = 0.;
    double sValM = 0.;
    double sSea = 0.;
    double sSeaM = 0.;

    quarkDistributionStrange.setQuarkDistribution(sVal + sSea);

    partonDistribution.addQuarkDistribution(quarkDistributionUp);
    partonDistribution.addQuarkDistribution(quarkDistributionDown);
    partonDistribution.addQuarkDistribution(quarkDistributionStrange);

        // Singlet distribution
        quarkDistributionUp.setQuarkDistributionPlus(sVal - sValM);
        // Nonsinglet distributiion
        quarkDistributionUp.setQuarkDistributionMinus(sVal + sSea + sValM + sSeaM);

    // Gluon distributions 
    GluonDistribution gluonDistribution(0.);
    partonDistribution.setGluonDistribution(gluonDistribution);

    return partonDistribution;
}

void saturatedModel_Ding::computeDD()
{
    // STEP 1: Build triangulation.
    mesh = setMesh();

    // STEP 2: Compute Radon transform matrix.
    NumA::RadonTransform RT;                                                                                // Radon transform.

    int m = 4*mesh.elements.size();                                                                         // No. sampling lines.
    int nod = mesh.nodes.size();                                                                            // No. interpolation nodes.

    std::vector<double> x(m), y(m), xi(m);                                                                  // Sampling lines (random sampling).

    default_random_engine re( time(NULL) );		                                                            // Random generator por sampling points 
    uniform_real_distribution<double> unif(0.,1.);		 													

    for ( int i = 0; i < m; i++ )                                                                           // Definition of sampling lines.
    {
        x[i]  = unif(re);
        y[i]  = unif(re);    
        xi[i] = x[i] * y[i];									                                            // Randon sampling for \xi (DGLAP region).
    }

    RT.build_RTmatrix(mesh, x, y, xi );                                                                     // Build Radon transform matrix.

    // STEP 3: Set b-vector by sampling DGLAP GPD.
    NumA::VectorD GPD_DGLAP( m );      

    for( int i = 0; i < x.size(); i++ )
    {
        GPD_DGLAP.at(i)  = (213.32*pow(1 - x[i],2.)*(pow(x[i],2.) - pow(xi[i],2.))
                           *sqrt( 1 - 2.9342*sqrt( (1 - x[i])/(1 - xi[i]) )*sqrt( (x[i] - xi[i])/(1 - xi[i]) ) + 2.2911*((1 - x[i])/(1 - xi[i]))*((x[i] - xi[i])/(1 - xi[i]))  )
                           *sqrt( 1 - 2.9342*sqrt( (1 - x[i])/(1 + xi[i]) )*sqrt( (x[i] + xi[i])/(1 + xi[i]) ) + 2.2911*((1 - x[i])/(1 + xi[i]))*((x[i] + xi[i])/(1 + xi[i])) ))
                           /( pow(1 - pow(xi[i],2.),2.) );
    }

    // STEP 4: Set LSMRsolver and compute DD.
    NumA::LSMRSolver solver;                                                                                // Set LSMR solver;

    solver.setDamping( 0.0 );                                                                               // Damp. Different from zero only on damped least-squares problems.
    solver.setMaximumIerations( 1e6 );                                                                      // Maximum number of iterations. (There is a misprint in NumA: Ierations instead of Iterations).
    solver.setToleranceA( 1e-7 );                                                                           // Estimate of the relative error in the data defining the matrix A.
    solver.setToleranceB( 1e-7 );                                                                           // Estimate of the relative error in the data defining the vector B.
    solver.setConditionNumberLimit( 1.0 / ( 10 * sqrt( 1e-15 ) ) );                                         // Upper limit on cond(Abar). Prevents small values of A and Abar.  

    RTmatrix.assign( m, nod, 0. );                                                                          // Resize RTmatrix attribute properly.

    for ( int i = 0; i < m; i++ )                                                                           // Set Radon transform matrix as a NumA::VectorD for NumA::LSMR to handle it.
    {
        for ( int j = 0; j < nod; j++ )
        {
            RTmatrix.at(i,j) = RT.RTMatrix[i][j];
        }
    }

    NumA::VectorD DoubleDistribution( nod );                                                                // Declare double distribution as a NumA vector.

    DoubleDistribution = solver.solve( RTmatrix, GPD_DGLAP);                                                // Computation of the double distribution.  

    // STEP 5: Update DD attribute with computed values.
    DD.resize( nod );                                                                                       // Resize DD attribute to match the number of interpolation nodes.

    for ( int i = 0; i < nod; i++ )                                                                         // Update DD attribute with values in DoubleDistribution.
    {
        DD.at(i) = DoubleDistribution.at(i);
    }

return;
}

NumA::Mesh saturatedModel_Ding::setMesh() //! This is a quick fix allowing for practical calculations. We need to fix the problem with Triangle.
{
    NumA::Mesh MESH;                                                                                        // Create an object of class Mesh.                                         
    
    // ============================================================================================
    // Set mesh (Proper implementation)
    // ============================================================================================
    
    // MESH.SetMinimalArea ( 0.001 );                                                                          // Set minimal area for mesh elements.
    // MESH.GenerateMesh();                                                                                    // Generate triangulation for object mesh.
    // MESH.Nodes( 1 );                                                                                        // Nodes for a constant interpolating polynomial.
    // MESH.BasisFunctions( 0 );                                                                               // Definition of interpolating basis functions.
    // MESH.Nodes( 1 );                                                                                        // Nodes for a constant interpolating polynomial.
    // MESH.BasisFunctions( 0 );                                                                               // Definition of interpolating basis functions.

    // ============================================================================================
    // Set mesh (Reading from file)
    // ============================================================================================

    // Set elements
    ifstream elem;
    elem.open( "/usr/local/share/data/mesh/mesh1/elements.dat" );                                           // TODO: CONVERT TO A RELATIVE PATH.

    if ( elem )
    {            
        string lineelem, headelem;

        int v1, v2, v3;

        std::vector<int> ele1(3);
            
        while ( getline(elem, lineelem) )
        {
            istringstream isse(lineelem);
            if ( !(isse >> headelem >> v1 >> v2 >> v3) ) 
            {
                throw runtime_error("Input file 'elements.dat' has not the correct format: vertex0 \t vertex1 \t vertex2");
            } else
            {
                ele1[0] = v1;
                ele1[1] = v2;
                ele1[2] = v3;

                MESH.elements.push_back(ele1);
            } 
        }

    } else
    {
        throw runtime_error("Missing input file 'elements.dat'. It should be located in 'partons_root_directory/data/mesh/mesh1'");
    }

    elem.close();

    // Set vertices
    ifstream vert;
    vert.open( "/usr/local/share/data/mesh/mesh1/vertices.dat" );                                           // TODO: CONVERT TO A RELATIVE PATH.

    if ( vert )
    {            
        string line, head;

        double a, b;
        NumA::point vertex;
            
        while ( getline(vert, line) )
        {
            istringstream iss(line);
            if ( !(iss >> head >> a >> b) ) 
            {
                throw runtime_error("Input file 'vertices.dat' has not the correct format: x-coord. \t y-coord.");
            } else
            {
                vertex.x = a;   vertex.y = b;

                MESH.vertices.push_back(vertex);
            } 
        }

    } else
    {
        throw runtime_error("Missing input file 'vertices.dat'. It should be located in 'partons_root_directory/data/mesh/mesh1'");
    }

    vert.close();

    // Set vertices' neighbors.
    ifstream vneighbors;
    vneighbors.open( "/usr/local/share/data/mesh/mesh1/neighborsv.dat" );                                   // TODO: CONVERT TO A RELATIVE PATH.                          

    if ( vneighbors )
    {            
        string linen;

        int n1, n2, n3, n4, n5, n6, n7, n8, n9;

        std::vector<int> vn;

        while ( getline(vneighbors, linen) )
        {
            vn.clear();
            
            istringstream ss(linen);
            ss >> n1;
            
            vn.push_back(n1);

            if ( ss >> n2 )
            {
                vn.push_back(n2);
                
                if ( ss >> n3 )
                {
                    vn.push_back(n3);

                    if ( ss >> n4 )
                    {
                        vn.push_back(n4);

                        if ( ss >> n5 )
                        {
                            vn.push_back(n5);

                            if ( ss >> n6 )
                            {
                                vn.push_back(n6);

                                if ( ss >> n7 )
                                {
                                    vn.push_back(n7);

                                    if ( ss >> n8 )
                                    {
                                        vn.push_back(n8);    
                                    }    
                                }    
                            }    
                        }    
                    }
                }
            }

            MESH.vneighbors.push_back(vn);
        }
    } else
    {
        throw runtime_error("Missing input file 'neighborsv.dat'. It should be located in 'partons_root_directory/data/mesh/mesh1'");
    }
    
    vneighbors.close();

    MESH.Nodes( 1 );                                                                                        // Nodes for a constant interpolating polynomial.
    MESH.BasisFunctions( 0 );                                                                               // Definition of interpolating basis functions.
    
return MESH;
}

std::vector<std::vector<double>> saturatedModel_Ding::computeDterms()
{
    // Evaluation points.
    double mx = 1000;               // Number of points.
    double st = 2/mx;               // Step.
    
    std::vector<std::vector<double>> Dterms(3, std::vector<double>(mx) );
    
    if ( DDt0.empty() )
    {
        // ============================================================================================
        // Compute DD (Proper computation)
        // ============================================================================================

        // computeDD();                                                                             
                
        // ============================================================================================
        // Compute DD (Reading from file)
        // ============================================================================================

        mesh = setMesh();                                                                        // Set Mesh according to the computations that have been carried out.
                     
        ifstream DoubleDistribution;                                                                 
        // // DoubleDistribution.open("/usr/local/share/data/DoubleDistribution/saturatedModel_Ding/AverageDD-50it-1e-7-P1.dat");
        DoubleDistribution.open("/home/jose/codes/PARTONS/data/kinematics/GPD/Evolution_kinematics/DD.dat");//! CREATE A DATA DIRECTORY CONTAINING DDs FOR THIS PURPOSE
        string linedd;
        double d;

        if ( DoubleDistribution )
        {
            while( getline(DoubleDistribution,linedd) )
            {
                istringstream iss(linedd);
                if ( !(iss >> d) )
                {
                    throw runtime_error( "DD file does not have the correct format: vector<double>" );
                } else
                {
                    DDt0.push_back(d);
                }
            }
            } else
            {
                throw runtime_error( "File not found." );
            }
                
        DoubleDistribution.close();
    }
    
    for (int i = 0; i < mx; i++ )
    {
        Dterms[2][i] = -1.+i*st;
    }
    Dterms[0] = computeDminus( mesh, DDt0, Dterms[2] );
    Dterms[1] = computeDplus( mesh, DDt0, Dterms[2] );

return Dterms;
}

vector<double> saturatedModel_Ding::computeDminus( NumA::Mesh& mesh, const vector<double>& DD, vector<double>& x )
{
    // Dminus = 0.5(H(-x,1,0) - H(x,1,0))

    NumA::RadonTransform RT;
    NumA::RadonTransform RTminus;

    std::vector<double> xi(x.size(),1.), y(x.size());

    for (int i = 0; i < x.size(); i++ )
    {
        y[i] = 1./x[i];
    }

    RT.RTMatrix.clear();
    RT.build_RTmatrix(mesh, x, y, xi);

    for (int i = 0; i < x.size(); i++ )
    {
        x[i] = -x[i];
        y[i] = -y[i];
    }

    RTminus.RTMatrix.clear();
    RTminus.build_RTmatrix(mesh, x, y, xi);

    // H(x,1,0)
    std::vector<double> Hx1(x.size());

    for ( int i = 0; i < x.size(); i++ )
    {
        for ( int j = 0; j < DD.size(); j++ )
        {
            Hx1[i] += RT.RTMatrix[i][j]*DD[j];
        }
    }

    //H(-x,1,0)
    std::vector<double> Hx1minus (x.size());
   
    for ( int i = 0; i < x.size(); i++ )
    {
        for ( int j = 0; j < DD.size(); j++ )
        {
            Hx1minus[i] += RTminus.RTMatrix[i][j]*DD[j];
        }
    }

    // Dminus
    std::vector<double> DminusVec (x.size());

    for( int i = 0; i < x.size(); i++)
    {
        DminusVec[i] = 0.5*(Hx1minus[i]-Hx1[i]);
    }

return DminusVec;
}

std::vector<double> saturatedModel_Ding::computeDplus( NumA::Mesh& mesh, const vector<double>& DD, vector<double>& x )
{
    // Dplus = 0.5(phi((1+x)/2) - H(-x,1,0) - H(x,1,0))

    NumA::RadonTransform RT;
    NumA::RadonTransform RTminus;

    std::vector<double> xi(x.size(),1.), y(x.size());

    for (int i = 0; i < x.size(); i++ )
    {
        y[i] = 1./x[i];
    }

    RT.RTMatrix.clear();
    RT.build_RTmatrix(mesh, x, y, xi);

    for (int i = 0; i < x.size(); i++ )
    {
        x[i] = -x[i];
        y[i] = -y[i];
    }

    RTminus.RTMatrix.clear();
    RTminus.build_RTmatrix(mesh, x, y, xi);

    // H(x,1,0)
    std::vector<double> Hx1(x.size());

    for ( int i = 0; i < x.size(); i++ )
    {
        for ( int j = 0; j < DD.size(); j++ )
        {
            Hx1[i] += RT.RTMatrix[i][j]*DD[j];
        }
    }

    //H(-x,1,0)
    std::vector<double> Hx1minus (x.size());
   
    for ( int i = 0; i < x.size(); i++ )
    {
        for ( int j = 0; j < DD.size(); j++ )
        {
            Hx1minus[i] += RTminus.RTMatrix[i][j]*DD[j];
        }
    }

    // Dplus
    std::vector<double> DplusVec (x.size());

    for( int i = 0; i < x.size(); i++)
    {
        DplusVec[i] = 0.5*(1.5*(1-pow(x[i],2))-Hx1minus[i]-Hx1[i]);
    }

return DplusVec;
}

}
