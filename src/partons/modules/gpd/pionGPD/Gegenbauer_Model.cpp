/**
 * @file Gegenbauer_Model.cpp
 * @author Cédric Mezrag (CEA Saclay)
 * @date 2nd June 2021
 * @version 1.0
 */

/**
 * Headers for setMesh to read mesh attributes from a external file
 * To be removed.
 */

#include <fstream>
#include <sstream>

// TODO: Clean list of headers. Check which ones are necessary.

#include "../../../../../include/partons/modules/gpd/pionGPD/Gegenbauer_Model.h"

#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/logger/LoggerManager.h>

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <algorithm>
#include <cmath>
#include <utility>

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
const unsigned int gegenbauerModel::classId =
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new gegenbauerModel("gegenbauerModel"));

gegenbauerModel::gegenbauerModel(const std::string &className) : PARTONS::GPDModule(className)
{
    // Set reference factorization scale.
    m_MuF2_ref = pow(1.,2.);
    //std::cout << "m_MuF2_ref = " << m_MuF2_ref << std::endl;
    //std::cout << "get m_MuF2_ref = " << getMuF2Ref() << std::endl;
    m_Mx = -m_x;
    m_n = 4 ;
    // Set default parameter for simple RDDA model

    //Relate a specific GPD type with the appropriate function
    m_listGPDComputeTypeAvailable.insert(
        std::make_pair(PARTONS::GPDType::H, &PARTONS::GPDModule::computeH));

}

gegenbauerModel::gegenbauerModel(const gegenbauerModel& other) : PARTONS::GPDModule(other)
{
    // Set reference factorization scale.
    m_MuF2_ref = pow(1.,2.);
    m_Mx = -m_x;
    m_n = 4 ;
    //std::cout << "m_MuF2_ref = " << m_MuF2_ref << std::endl;
    //std::cout << "get m_MuF2_ref = " << getMuF2Ref() << std::endl;

    // Set default parameter for simple RDDA model

    //MathIntegratorModule();

}

gegenbauerModel::~gegenbauerModel()
{

}

gegenbauerModel* gegenbauerModel::clone() const
{
    return new gegenbauerModel(*this);
}

void gegenbauerModel::configure(const ElemUtils::Parameters &parameters)
{
    PARTONS::GPDModule::configure(parameters);
}

void gegenbauerModel::isModuleWellConfigured()
{
    PARTONS::GPDModule::isModuleWellConfigured();
    //std::cout << "GPD module well configured" << std::endl;

}

void gegenbauerModel::initModule()
{
    PARTONS::GPDModule::initModule();
    //std::cout << "GPD module initiated" << std::endl;
}

void gegenbauerModel::resolveObjectDependencies()
{

}



PARTONS::PartonDistribution gegenbauerModel::computeH()
{

    std::vector<double> x(1), y(1), xi(1), xm(1), ym(1), xim(1);                                            // Declare kinematics in a format appropiated for RadonTransform.

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
    double Eps = 1.e-9;
    m_Mx = -m_x;

    	if (m_xi != 1)
    	{ std::cout << "xi should equal to 1, here it is xi = " << m_xi << std::endl;

    	}

    	// u-d quark, valence part evaluated at fx

    	        double HuVal = 0.;
    	        double HdVal = 0.;

    	        const double alpha = 1.5;
    	        HuVal = ( 1 - pow(m_x, 2) ) * boost::math::gegenbauer(m_n, alpha, m_x);

    	    ///////////////////////////////////////////////////////////////////////
    	    //   u and d quarks, valence part evaluated at fMx (instead of fx)   //
    	    ///////////////////////////////////////////////////////////////////////

    	        double HuValMx = 0.;
    	        double HdValMx = 0.;

    	        HuValMx = ( 1 - pow(m_Mx, 2) ) * boost::math::gegenbauer(m_n, alpha, m_Mx);


    //std::cout << "Huval = " << HuVal << " Hdval = " << HdVal << std::endl;

    // TODO: Check and fix definitions of the the different quark distributions (u, uM, u+, u-, d(...) and s(...)). See Cédric PhD thesis. pp. 56.
    double HuSea  = 0.;
    double HuSeaMx = 0.;

    quarkDistributionUp.setQuarkDistribution(HuVal + HuSea);

        // Singlet distribution
        quarkDistributionUp.setQuarkDistributionPlus(HuVal + HuSea - HuValMx - HuSeaMx);
        // Nonsinglet distributiion
        quarkDistributionUp.setQuarkDistributionMinus(HuVal + HuValMx);

    // d-quark
    double dSea = 0.;
    double dSeaM = 0.;

    quarkDistributionDown.setQuarkDistribution(HdVal + dSea);

        // Singlet distribution
        quarkDistributionDown.setQuarkDistributionPlus(HdVal + dSea - HdValMx - dSeaM);
        // Nonsinglet distributiion
        quarkDistributionDown.setQuarkDistributionMinus(HdVal + HdValMx);

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

    quarkDistributionStrange.setQuarkDistribution(cVal + cSea);
    quarkDistributionStrange.setQuarkDistributionPlus(cVal + cSea - cValM - cSeaM);
    quarkDistributionStrange.setQuarkDistributionMinus(cVal + cValM);

    // b-quark
    double bVal = 0.;
    double bValM = 0.;
    double bSea = 0.;
    double bSeaM = 0.;

    quarkDistributionStrange.setQuarkDistribution(bVal + bSea);
    quarkDistributionStrange.setQuarkDistributionPlus(bVal + bSea - bValM - bSeaM);
    quarkDistributionStrange.setQuarkDistributionMinus(bVal + bValM);

    // t-quark
    double tVal = 0.;
    double tValM = 0.;
    double tSea = 0.;
    double tSeaM = 0.;

    quarkDistributionStrange.setQuarkDistribution(tVal + tSea);
    quarkDistributionStrange.setQuarkDistributionPlus(tVal + tSea - tValM - tSeaM);
    quarkDistributionStrange.setQuarkDistributionMinus(tVal + tValM);

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


}
