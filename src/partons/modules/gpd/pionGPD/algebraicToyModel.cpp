/**
 * Headers for setMesh to read mesh attributes from a external file
 * To be removed.
 */

#include <fstream>
#include <sstream>

// TODO: Clean list of headers. Check which ones are necessary.

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
#include <include/NumA/RadonTransform/RadonTransform.h>
#include <include/NumA/triangulation/mesh.h>
#include <include/NumA/triangulation/point.h>

#include <include/NumA/linear_algebra/least_squares/LSMRSolver.h>
#include <include/NumA/linear_algebra/matrix/MatrixD.h>
#include <include/NumA/linear_algebra/vector/VectorD.h>

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
#include <include/NumA/triangulation/software/triangle.h>
}

namespace PARTONS {

// With this line we "create" the name for our GPD module. This will be integrated into the factory and thus partons knows about it.
// Usign the assigned name "satyratedModel_Ding" we can make use of this module in subsequent codes. 
const unsigned int algebraicToyModel::classId =
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new algebraicToyModel("algebraicToyModel"));

algebraicToyModel::algebraicToyModel(const std::string &className) : PARTONS::GPDModule(className)
{
    // Set reference factorization scale.
    m_MuF2_ref = 0.4;

    //Relate a specific GPD type with the appropriate function
    m_listGPDComputeTypeAvailable.insert(
        std::make_pair(PARTONS::GPDType::H, &PARTONS::GPDModule::computeH));
}

algebraicToyModel::algebraicToyModel(const algebraicToyModel& other) : PARTONS::GPDModule(other) 
{
    m2 = 0.16;
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
    
    std::vector<double> x(1), y(1), xi(1), xm(1), ym(1), xim(1);                                            // Declare kinematics in a format appropiated for RadonTransform.

    PARTONS::PartonDistribution partonDistribution;

    // Quark distributions
    QuarkDistribution quarkDistributionUp(QuarkFlavor::UP);
    QuarkDistribution quarkDistributionDown(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistributionStrange(QuarkFlavor::STRANGE);

    // u-quark
    double uVal, uValM;

    if ( m_t == 0)                                                                                          // Zero momentum transfer
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
            if ( DD.empty() )                                                                               
            {
                // ============================================================================================
                // Compute DD (Proper computation)
                // ============================================================================================

                computeDD();                                                                                
                
                // ============================================================================================
                // Compute DD (Reading from file)
                // ============================================================================================

                // mesh = setMesh();                                                                        // Set Mesh according to the computations that have been carried out.
                     
                // ifstream DoubleDistribution;                                                                 
                // DoubleDistribution.open("/usr/local/share/data/DoubleDistribution/AlgebraicToyModel/AverageDD-50it-1e-7-P1.dat");
 
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
            }

            NumA::RadonTransform RT;                                                                        // Radon transform matrix for uVal.
            NumA::RadonTransform RTminus;                                                                   // Radon transform matrix for uValM.

            x[0] = m_x; xi[0] = m_xi; y[0] = m_xi/m_x;                                                      // Kienmatics written in proper format.
            xm[0] = -m_x; xim[0] = m_xi; ym[0] = -m_xi/m_x;
                
            RT.RTMatrix.clear();                                                                            // Radon transform matrix for uVal.
            RT.build_RTmatrix(mesh, x, y, xi );

            RTminus.RTMatrix.clear();                                                                       // Radon transform matrix for uValM.
            RTminus.build_RTmatrix(mesh, xm, ym, xim);

            for ( int i = 0; i < DD.size() ; i++ )
            {
                uVal += RT.RTMatrix[0][i]*DD[i];
                uValM += RTminus.RTMatrix[0][i]*DD[i];                                                      // !Check if this is correct. Check computation of uValM.
            }
        }
    } else                                                                                                  // Non-vanishing momentum transfer.
    {

        c  = -m_t*pow(1 - m_x, 2.)/(4*m2*(1 - pow(m_xi,2)));                                                // t-dependence algebraic toy model.
        cM = -m_t*pow(1 + m_x, 2.)/(4*m2*(1 - pow(m_xi,2)));

        if ( m_x > m_xi )                                                                                   // DGLAP>
        {
            uVal  = 7.5 * pow(1 - m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) * (3 + ((1 - 2 * c) * atanh(c/(1+c)))/((1 + c) * sqrt(c/(1 + c))) )
                / ( pow( 1 - pow(m_xi,2.) , 2.) * pow(1 + c,2) );
            uValM = 0.;
        } else if ( m_x < -m_xi )                                                                           // DGLAP>
        {
            uVal  = 0.;
            uValM = 7.5 * pow(1 + m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) * (3 + ((1 - 2 * cM) * atanh(cM/(1+cM)))/((1 + cM) * sqrt(cM/(1 + cM))) )
                / ( pow( 1 - pow(m_xi,2.) , 2.) * pow(1 + cM,2) );
        } else                                                                                              // ERBL
        {
            if ( DD.empty() )
            {
                // ============================================================================================
                // Compute DD (Proper computation)
                // ============================================================================================

                computeDD();                                                                                
                
                // ============================================================================================
                // Compute DD (Reading from file)
                // ============================================================================================

                // mesh = setMesh();                                                                        // Set Mesh according to the computations that have been carried out.
                     
                // ifstream DoubleDistribution;                                                                 
                // DoubleDistribution.open("/usr/local/share/data/DoubleDistribution/AlgebraicToyModel/AverageDD-50it-1e-7-P1.dat"");
 
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
            }
        
            NumA::RadonTransform RT;                                                                        // Radon transform matrix for uVal.
            NumA::RadonTransform RTminus;                                                                   // Radon transform matrix for uValM.

            x[0] = m_x; xi[0] = m_xi; y[0] = m_xi/m_x;                                                      // Kinematics written in proper format.
            xm[0] = -m_x; xim[0] = m_xi; ym[0] = -m_xi/m_x;

            RT.RTMatrix.clear();                                                                            // Radon transform matrix for uVal.
            RT.build_RTmatrix(mesh, x, y, xi );

            RTminus.RTMatrix.clear();                                                                       // Radon transform matrix for uValM.
            RTminus.build_RTmatrix(mesh, xm, ym, xim);

            for ( int i = 0; i < DD.size() ; i++ )
            {
                uVal += RT.RTMatrix[0][i]*DD[i];
                uValM += RTminus.RTMatrix[0][i]*DD[i];                                                      // !Check if this is correct. Check computation of uValM.
            }
        }
    }

            // ERBL t < 0
            // uVal  = -3.75 * (pow(m_xi,2) - pow(m_x,2)) * ( sqrt(c * (c + 1)) * ( -c*m_xi*(1 - pow(m_xi,2))*(1 - m_x) 
                // * (pow(m_xi,4) + 6*m_xi*(1-m_x)*pow(m_x,2)-6*pow(m_xi,3)*(1-m_x)+pow(m_xi,2)*(4-3*(3-m_x)*m_x)+m_x*(4-m_x*(8-5*m_x))) 
                // + pow(c,2)*pow(1-pow(m_xi,2),2)*(pow(m_xi,3)*(6*m_xi-2)+3*pow(m_x,4)-4*m_xi*pow(m_x,3)-6*(m_xi-1)*m_xi*pow(m_x,2)+2*m_xi*(pow(m_xi,2)-1)*m_x)
                // + pow(m_x-1,3)*(pow(m_xi,5)+3*pow(m_xi,4)*(m_x-1)+pow(m_xi,3)*(2-5*m_x)+2*m_xi*m_x))
                // + (1-2*c)*pow( c*(1-pow(m_xi,2))*(pow(m_xi,2)-pow(m_x,2))+pow(m_xi,2)*(m_x-1),2)
                // *(atanh(sqrt(c/(1+c)))-atanh(sqrt(c/(1+c))*(pow(m_xi,2)-m_x)/(m_xi*(1-m_x)))))
                // / (pow(1+c,2.5)*pow(1-pow(m_xi,2),1.5)*pow(1-m_x,2)*sqrt(c*(1-pow(m_xi,2)))*pow(pow(m_xi,2)+c*(1-pow(m_xi,2))*(pow(m_xi,2)-pow(m_x,2))/pow(1-m_x,2),2));
            // uValM = -3.75 * (pow(m_xi,2) - pow(m_x,2)) * ( sqrt(cM * (cM + 1)) * ( -cM*m_xi*(1 - pow(m_xi,2))*(1 + m_x) 
                // * (pow(m_xi,4) + 6*m_xi*(1+m_x)*pow(m_x,2)-6*pow(m_xi,3)*(1+m_x)+pow(m_xi,2)*(4-3*(3+m_x)*(-1)*m_x)-m_x*(4+m_x*(8+5*m_x))) 
                // + pow(cM,2)*pow(1-pow(m_xi,2),2)*(pow(m_xi,3)*(6*m_xi-2)+3*pow(m_x,4)-4*m_xi*pow(-m_x,3)-6*(m_xi-1)*m_xi*pow(m_x,2)+2*m_xi*(pow(m_xi,2)-1)*(-1)*m_x)
                // + pow(-m_x-1,3)*(pow(m_xi,5)+3*pow(m_xi,4)*(-m_x-1)+pow(m_xi,3)*(2+5*m_x)+2*m_xi*(-1)*m_x))
                // + (1-2*cM)*pow( cM*(1-pow(m_xi,2))*(pow(m_xi,2)-pow(m_x,2))+pow(m_xi,2)*(-m_x-1),2)
                // *(atanh(sqrt(cM/(1+cM)))-atanh(sqrt(cM/(1+cM))*(pow(m_xi,2)-m_x)/(-m_xi*(1+m_x)))))
                // / (pow(1+cM,2.5)*pow(1-pow(m_xi,2),1.5)*pow(1+m_x,2)*sqrt(cM*(1-pow(m_xi,2)))*pow(pow(m_xi,2)+cM*(1-pow(m_xi,2))*(pow(m_xi,2)-pow(m_x,2))/pow(1+m_x,2),2));

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

void algebraicToyModel::computeDD()
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

    if ( m_t == 0 )                                                                                         // ? How can I sample the GPD within this code. 
    {
        for( int i = 0; i < x.size(); i++ )
        {
            GPD_DGLAP.at(i)  = 30 * pow(1 - x[i], 2.) * ( pow(x[i],2.) - pow(xi[i],2.) ) / pow( 1 - pow(xi[i],2.) , 2.);
        }
    } else
    {
        double c  = -m_t*pow(1 - m_x, 2.)/(4*m2*(1 - pow(m_xi,2)));
 
        for ( int i = 0; i < x.size(); i++ )
        {
            GPD_DGLAP.at(i)  = 7.5 * pow(1 - x[i], 2.) * ( pow(x[i],2.) - pow(xi[i],2.) ) * (3 + ((1 - 2 * c) * atanh(c/(1+c)))/((1 + c) * sqrt(c/(1 + c))) )
                / ( pow( 1 - pow(xi[i],2.) , 2.) * pow(1 + c,2) );
        }
    }

    // STEP 4: Set LSMRsolver and compute DD.
    NumA::LSMRSolver solver;                                                                                // Set LSMR solver;

    solver.setDamping( 0.0 );                                                                               // Damp. Different from zero only on damped least-squares problems.
    solver.setMaximumIerations( 1e6 );                                                                      // Maximum number of iterations. (There is a misprint in NumA: Ierations instead of Iterations).
    solver.setToleranceA( 1e-7 );                                                                           // Estimate of the relative error in the data defining the matrix A.
    solver.setToleranceB( 1e-7 );                                                                           // Estimate of the relative error in the data defining the vector B.
    solver.setConditionNumberLimit( 1.0 / ( 10 * sqrt( 1e-15 ) ) );                                         // Upper limit on cond(Abar). Prevents small values of A and Abar.  

    RTmatrix.assign( m, nod, 0. );                                                                          // Resize RTmatrix attribute properly.

    // DD.assign( nod, 0. );                                                                                   // Resize DD attribute properly

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

NumA::Mesh algebraicToyModel::setMesh() //! This is a quick fix allowing for practical calculations. We need to fix the problem with Triangle.
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

}
