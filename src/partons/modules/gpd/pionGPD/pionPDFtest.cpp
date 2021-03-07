/**
 * @file pionPDFtest.cpp
 * @author José Manuel Morgado Chavez (University Of Huelva)
 * @date 6th March 2021 
 * @version 1.0
 */

/**
 * Headers for setMesh to read mesh attributes from a external file
 * To be removed.
 */

#include <fstream>
#include <sstream>

// TODO: Clean list of headers. Check which ones are necessary.

#include "../../../../../include/partons/modules/gpd/pionGPD/pionPDFtest.h"

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

namespace PARTONS {

// With this line we "create" the name for our GPD module. This will be integrated into the factory and thus partons knows about it.
const unsigned int pionPDFtest::classId =
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new pionPDFtest("pionPDFtest"));

pionPDFtest::pionPDFtest(const std::string &className) : PARTONS::GPDModule(className)
{
    // Set reference factorization scale.
    m_MuF2_ref = 1.;                                                                              // TODO: Set equal to value given in reference paper for \alpha_PI: \mu_H = 0,33 GeV.


    m_listGPDComputeTypeAvailable.insert(std::make_pair(PARTONS::GPDType::H, &PARTONS::GPDModule::computeH));
}

pionPDFtest::pionPDFtest(const pionPDFtest& other) : PARTONS::GPDModule(other) 
{

}

pionPDFtest::~pionPDFtest() 
{

}

pionPDFtest* pionPDFtest::clone() const 
{
    return new pionPDFtest(*this);
}

void pionPDFtest::resolveObjectDependencies() 
{

}

void pionPDFtest::configure(const ElemUtils::Parameters &parameters) 
{
    PARTONS::GPDModule::configure(parameters);
}

void pionPDFtest::isModuleWellConfigured() 
{
    PARTONS::GPDModule::isModuleWellConfigured();
}

void pionPDFtest::initModule() 
{
    PARTONS::GPDModule::initModule();
}

PARTONS::PartonDistribution pionPDFtest::computeH() 
{

    PARTONS::PartonDistribution partonDistribution;

    // Quark distributions
    QuarkDistribution quarkDistributionUp(QuarkFlavor::UP);
    QuarkDistribution quarkDistributionDown(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistributionStrange(QuarkFlavor::STRANGE);

    // u-quark
    double uVal, uValM;

    uVal  = 30*pow(m_x*(1-m_x),2.);
    uValM = 30*pow(m_x*(1+m_x),2.);

    double uSea  = 0.;
    double uSeaM = 0.;

    quarkDistributionUp.setQuarkDistribution(uVal + uSea);

        // Singlet distribution
        quarkDistributionUp.setQuarkDistributionPlus(uVal + uSea - uValM - uSeaM);
        // Nonsinglet distributiion
        quarkDistributionUp.setQuarkDistributionMinus(uVal + uSea + uValM + uSeaM);
        
    // d-quark    
    double dVal = uVal;
    double dValM = uValM;
    double dSea = 0.;
    double dSeaM = 0.;

    quarkDistributionDown.setQuarkDistribution(dVal + dSea);

        // Singlet distribution
        quarkDistributionDown.setQuarkDistributionPlus(dVal + dSea - dValM - dSeaM);
        // Nonsinglet distributiion
        quarkDistributionDown.setQuarkDistributionMinus(dVal + dSea + dValM + dSeaM);

    // s-quark
    double sVal = 0.;
    double sValM = 0.;
    double sSea = 0.;
    double sSeaM = 0.;

    quarkDistributionStrange.setQuarkDistribution(sVal + sSea);
    quarkDistributionStrange.setQuarkDistributionPlus(sVal + sSea - sValM - sSeaM);
    quarkDistributionStrange.setQuarkDistributionMinus(sVal + sSea + sValM + sSeaM);

    partonDistribution.addQuarkDistribution(quarkDistributionUp);
    partonDistribution.addQuarkDistribution(quarkDistributionDown);
    partonDistribution.addQuarkDistribution(quarkDistributionStrange);

    // Gluon distributions 
    GluonDistribution gluonDistribution(0.);
    partonDistribution.setGluonDistribution(gluonDistribution);
    
    return partonDistribution;
}

}