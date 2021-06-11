/**
 * @file Simple_RDDA.cpp
 * @author Cédric Mezrag (CEA Saclay)
 * @date 11th June 2021
 * @version 1.0
 */

/**
 * Headers for setMesh to read mesh attributes from a external file
 * To be removed.
 */

#include <fstream>
#include <sstream>

// TODO: Clean list of headers. Check which ones are necessary.

#include "../../../../../include/partons/modules/gpd/pionGPD/pionRDDA.h"

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
const unsigned int pionRDDAModel::classId =
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new pionRDDAModel("pionRDDAModel"));

pionRDDAModel::pionRDDAModel(const std::string &className) : PARTONS::GPDModule(className)
{
    // Set reference factorization scale.
    m_MuF2_ref = pow(1.,2.);
    //std::cout << "m_MuF2_ref = " << m_MuF2_ref << std::endl;
    //std::cout << "get m_MuF2_ref = " << getMuF2Ref() << std::endl;

    // Set default parameter for simple RDDA model
    mPara = {-0.5,3.};
    mRDDA_Para = 1.;


    //Relate a specific GPD type with the appropriate function
    m_listGPDComputeTypeAvailable.insert(
        std::make_pair(PARTONS::GPDType::H, &PARTONS::GPDModule::computeH));

    initFunctorsForIntegrations();
}

pionRDDAModel::pionRDDAModel(const pionRDDAModel& other) : PARTONS::GPDModule(other)
{
    // Set reference factorization scale.
    m_MuF2_ref = pow(1.,2.);
    //std::cout << "m_MuF2_ref = " << m_MuF2_ref << std::endl;
    //std::cout << "get m_MuF2_ref = " << getMuF2Ref() << std::endl;

    // Set default parameter for simple RDDA model
    mPara = {-0.5,3.};
    mRDDA_Para = 1.;

    MathIntegratorModule();
    initFunctorsForIntegrations();

}

pionRDDAModel::~pionRDDAModel()
{
    if(m_pIntegralHuVal){ delete m_pIntegralHuVal; m_pIntegralHuVal = 0;}
    if(m_pIntegralHuValMx){ delete m_pIntegralHuValMx; m_pIntegralHuValMx = 0;}
}

pionRDDAModel* pionRDDAModel::clone() const
{
    return new pionRDDAModel(*this);
}
void pionRDDAModel::resolveObjectDependencies()
{

}

void pionRDDAModel::configure(const ElemUtils::Parameters &parameters)
{
    PARTONS::GPDModule::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

void pionRDDAModel::isModuleWellConfigured()
{
    PARTONS::GPDModule::isModuleWellConfigured();
    //std::cout << "GPD module well configured" << std::endl;

}

void pionRDDAModel::initModule()
{
    PARTONS::GPDModule::initModule();
    //std::cout << "GPD module initiated" << std::endl;
}


void pionRDDAModel::initFunctorsForIntegrations() {
    MathIntegratorModule::setIntegrator(NumA::IntegratorType1D::DEXP);

//Integrators for H

    m_pIntegralHuVal = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralHuVal);

    m_pIntegralHuValMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralHuValMx);

    m_pIntegralHdVal = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralHdVal);

    m_pIntegralHdValMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralHdValMx);

}




void pionRDDAModel::throwBetaException(const std::string &funcName,
        double betaValue) {
    throw ElemUtils::CustomException(getClassName(), funcName,
            ElemUtils::Formatter()
                    << "Longitudinal momentum fraction should be in ] 0., +1. ]"
                    << '\n' << "Here beta = " << betaValue << '\n');
}

//Set profile function parameter
void pionRDDAModel::setProfileParameter(double N){
	mRDDA_Para = N;
	std::cout << "RDDA parameter set to n = "<< mRDDA_Para << std::endl ;
}

double pionRDDAModel::getProfileParameter(){
	std::cout << "RDDA parameter set to n = "<< mRDDA_Para << std::endl ;
	return mRDDA_Para ;
}


//Profile function
double pionRDDAModel::Profile(double beta, double alpha) {
    double profile = 0.;
    double ProfileShape = mRDDA_Para;
    double TwiceProfileShapePlus1 = 2. * ProfileShape + 1;


    double alphaBeta = fabs(alpha) + fabs(beta);

    //TODO uncomment this test !
//    if (alphaBeta > 1.) {
//
//        error(__func__,
//                ElemUtils::Formatter()
//                        << "GK11TestModel: Parameters of profile function should be in rhombus | alpha | + | beta | <= 1."
//                        << '\n' << "Here alpha = " << alpha << " beta = "
//                        << beta << " | alpha | + | beta | = " << alphaBeta
//                        << " for GPD " << GPDType(m_gpdType).toString()
//                        << '\n');
//    }

    profile = pow((1. - fabs(beta)) * (1. - fabs(beta)) - alpha * alpha,
            ProfileShape);
    profile /= pow(1. - fabs(beta), TwiceProfileShapePlus1);
    profile *= tgamma(TwiceProfileShapePlus1 + 1.);
    profile /= (pow(2., TwiceProfileShapePlus1) * tgamma(ProfileShape + 1.)
            * tgamma(ProfileShape + 1.));
    return profile;
}

//forward limit ansatz for H
double pionRDDAModel::SimplePdfAnsatz(double beta) {
	double pdf;
    pdf = pow(beta, mPara.at(0)) * pow((1. - beta), mPara.at(1))
            * tgamma(2+mPara.at(0)+mPara.at(1))/tgamma(1.+mPara.at(0))/tgamma(1.+mPara.at(1));
    return pdf ;
}


//integrals for H
/*
#################################
#
# u quark case
#
##################################


*/
double pionRDDAModel::IntegralHuVal(double beta, std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return HuValDD(beta, alpha) / m_xi;

}

double pionRDDAModel::IntegralHuValMx(double beta, std::vector<double> Par) {
    double alpha = (m_Mx - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return HuValDD(beta, alpha) / m_xi;
}


double pionRDDAModel::HuValDD(double beta, double alpha) {
    double absbeta = fabs(beta);
    double HuValDD;
    /*    if (beta <= 0 || beta > 1.) {
     throwBetaException(__func__, x);
     }*/
    if (beta > 0.) {
        HuValDD = 2 * SimplePdfAnsatz(beta)* Profile(beta, alpha);
    } else {
        HuValDD = 0.;
    }

    return HuValDD;
}


/*
#################################
#
# d quark case
#
##################################
*/

double pionRDDAModel::IntegralHdVal(double beta, std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return HdValDD(beta, alpha) / m_xi;

}

double pionRDDAModel::IntegralHdValMx(double beta, std::vector<double> Par) {
    double alpha = (m_Mx - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return HdValDD(beta, alpha) / m_xi;
}


double pionRDDAModel::HdValDD(double beta, double alpha) {
    double absbeta = fabs(beta);
    double HdValDD;
    /*    if (beta <= 0 || beta > 1.) {
     throwBetaException(__func__, x);
     }*/
    if (beta > 0.) {
        HdValDD = SimplePdfAnsatz(beta)* Profile(beta, alpha);
    } else {
        HdValDD = 0.;
    }

    return HdValDD;
}



PARTONS::PartonDistribution pionRDDAModel::computeH()
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
    double Eps = 1.e-10;
    m_Mx = -m_x;

    	if (m_xi < 0)
    	{ std::cout << "xi should be positive, here it is xi = " << m_xi << std::endl;

    	}
    	// Integration limits and methods
    	    double Beta1 = (m_x - m_xi) / (1. - m_xi); // eq. (54) in A. Radyushkin's paper
    	    double Beta2 = (m_x + m_xi) / (1. + m_xi); // eq. (54) in A. Radyushkin's paper

    	    double Beta1Mx = (m_Mx - m_xi) / (1. - m_xi); // eq. (54) in A. Radyushkin's paper
    	    double Beta2Mx = (m_Mx + m_xi) / (1. + m_xi); // eq. (54) in A. Radyushkin's paper

    	    double Beta2Min = std::min<double>(Beta2, Beta2Mx);
    	    std::vector<double> emptyParameters;

    	    // u-d quark, valence part evaluated at fx

    	        double HuVal = 0.;
    	        double HdVal = 0.;

    	        if (m_x >= m_xi) {
    	            // Integration, u quark
    	            HuVal = integrate(m_pIntegralHuVal, Beta1, Beta2, emptyParameters);

    	            // Integration, d quark
    	            HdVal = integrate(m_pIntegralHdVal, Beta1, Beta2, emptyParameters);
    	        }

    	        if (fabs(m_x) < m_xi) {
    	            // Integration, u quark
    	            HuVal = integrate(m_pIntegralHuVal, Eps, Beta2, emptyParameters);

    	            // Integration, d quark
    	            HdVal = integrate(m_pIntegralHdVal, Eps, Beta2, emptyParameters);
    	        }

    	    ///////////////////////////////////////////////////////////////////////
    	    //   u and d quarks, valence part evaluated at fMx (instead of fx)   //
    	    ///////////////////////////////////////////////////////////////////////

    	        double HuValMx = 0.;
    	        double HdValMx = 0.;

    	        if (m_Mx >= m_xi) {

    	            // Integration, u quark
    	            HuValMx = MathIntegratorModule::integrate(m_pIntegralHuValMx, Beta1Mx, Beta2Mx,
    	                    emptyParameters);

    	            // Integration, d quark
    	            HdValMx = integrate(m_pIntegralHdValMx, Beta1Mx, Beta2Mx,
    	                    emptyParameters);
    	        }

    	        if (fabs(m_Mx) < m_xi) {

    	        	// Integration, u quark
    	            HuValMx =  MathIntegratorModule::integrate(m_pIntegralHuValMx, Eps, Beta2Mx, emptyParameters);

    	            // Integration, d quark
    	            HdValMx = integrate(m_pIntegralHdValMx, Eps, Beta2Mx, emptyParameters);

    	        }


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
