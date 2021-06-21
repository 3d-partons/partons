/**
  * @file    interpolatedGPD.cpp
  * @author  José Manuel Morgado Chavez (University Of Huelva)
  * @date    Friday 10th June 2021 
  * @version 1.0
  */

#include "../../../../../include/partons/modules/gpd/pionGPD/interpolatedGPD.h"

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

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <NumA/interpolation/CubicSpline.h>
#include <math.h>


namespace PARTONS {

// With this line we "create" the name for our GPD module. This will be integrated into the factory and thus partons knows about it.
const unsigned int interpolatedGPD::classId =
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new interpolatedGPD("interpolatedGPD"));

interpolatedGPD::interpolatedGPD(const std::string &className) : PARTONS::GPDModule(className)
{    
    // Set reference factorization scale.
    m_MuF2_ref = pow(0.331,2.);

    //Relate a specific GPD type with the appropriate function
    m_listGPDComputeTypeAvailable.insert(
        std::make_pair(PARTONS::GPDType::H, &PARTONS::GPDModule::computeH));
}

interpolatedGPD::interpolatedGPD(const interpolatedGPD& other) : PARTONS::GPDModule(other) 
{    

}

interpolatedGPD::~interpolatedGPD() 
{

}

interpolatedGPD* interpolatedGPD::clone() const 
{
    return new interpolatedGPD(*this);
}

void interpolatedGPD::resolveObjectDependencies() 
{

}

void interpolatedGPD::configure(const ElemUtils::Parameters &parameters) 
{
    PARTONS::GPDModule::configure(parameters);
}

void interpolatedGPD::isModuleWellConfigured() 
{
    PARTONS::GPDModule::isModuleWellConfigured();
}

void interpolatedGPD::initModule() 
{
    PARTONS::GPDModule::initModule();
}

PARTONS::PartonDistribution interpolatedGPD::computeH() 
{
    // u-quark GPDs.
    double uVal, uValM;

    read_file();

    // Interpolate GPD model
    uValInt = new NumA::CubicSpline(x,GPD);
    
    uValInt->ConstructSpline();

    if ( m_x > m_xi || m_x == m_xi )
    {
        if ( m_t == 0)
        {
            if ( m_x == 1 )
            {
                std::cout << "HELLO \n";
                uVal = 0.;
                uValM = 0.;
            } else
            {
                uVal  = 30 * pow(1 - m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) / pow( 1 - pow(m_xi,2.) , 2.);
                uValM = 0.;
            }
        } else
        {
            // t-dependence
            const double m2 = pow(0.318,2.);    
            const double c  = -m_t*pow(1 - m_x, 2.)/(4*m2*(1 - pow(m_xi,2)));                                                // t-dependence algebraic toy model.

            uVal  = 7.5 * pow(1 - m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) * (3 + ((1 - 2 * c) * atanh(sqrt(c/(1+c))))/((1 + c) * sqrt(c/(1 + c))) )
                / ( pow( 1 - pow(m_xi,2.) , 2.) * pow(1 + c,2.) );
            uValM = 0.;

            if ( m_x == 1 )                                                                                 
            {
                uVal = 0.;
                uValM = 0.;
            }
        }
    } else if ( m_x < -m_xi || m_x == -m_xi )
    {
        if ( m_t == 0 )
        {
            if ( m_x == -1 )
            {
                uVal = 0.;
                uValM = 0.;
            } else
            {
                uVal = 0.;
                uValM = 30 * pow(1 + m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) / pow( 1 - pow(m_xi,2.) , 2.);
            }

        } else
        {
            // t-dependence
            const double m2 = pow(0.318,2.);    
            const double cM = -m_t*pow(1 + m_x, 2.)/(4*m2*(1 - pow(m_xi,2)));

            uVal  = 0.;
            uValM = 7.5 * pow(1 + m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) * (3 + ((1 - 2 * cM) * atanh(sqrt(cM/(1+cM))))/((1 + cM) * sqrt(cM/(1 + cM))) )
                / ( pow( 1 - pow(m_xi,2.) , 2.) * pow(1 + cM,2.) );

            if ( m_x == -1 )                                                                                
            {
                uVal = 0.;
                uValM = 0.;
            }
        }
    } else
    {
        uVal = uValInt->getSplineInsideValue(m_x);
        uValM = uValInt->getSplineInsideValue(-m_x);    
    }

    delete uValInt;
    uValInt = NULL;

    // summary.close();

    PARTONS::PartonDistribution partonDistribution;

    // Quark distributions
    QuarkDistribution quarkDistributionUp(QuarkFlavor::UP);
    QuarkDistribution quarkDistributionDown(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistributionStrange(QuarkFlavor::STRANGE);
    QuarkDistribution quarkDistributionCharm(QuarkFlavor::CHARM);
    QuarkDistribution quarkDistributionBottom(QuarkFlavor::BOTTOM);
    QuarkDistribution quarkDistributionTop(QuarkFlavor::TOP);

    // u-quark
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

void interpolatedGPD::read_file()
{
    // Read GPD model
    path_to_inputfile = "data/CovariantExtension/AlgebraicModel/FinnerMesh-t";
    path_to_inputfile.append( std::to_string(m_t) );
    path_to_inputfile.append( "-xi" );
    path_to_inputfile.append( std::to_string(m_xi) );
    path_to_inputfile.append( ".csv" );

    std::ifstream inputfile;
    inputfile.open(path_to_inputfile);

    if ( inputfile )
    {
        // Clear vectors
        x.clear();
        GPD.clear();
    
        std::string line;
    
        double v1, v2;
    
        while( getline(inputfile, line) )
        {
            std::stringstream iss(line);
    
            if ( !(iss >> v1 >> v2) )
            {
                throw std::runtime_error("Input file is not appropriately formated.");
            }else
            {
                x.push_back(v1);                // Value of x.
                GPD.push_back(v2);              // GPD.
            }
        }
    } else
    {
        throw std::runtime_error("File not found.");
    }

    inputfile.close();
}

}