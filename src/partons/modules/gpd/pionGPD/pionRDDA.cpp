/**
 * @file pionRDDA.cpp
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
    m_MuF2_ref = 1.9 ; // ref scale at which xfitter PDFs are given
    //std::cout << "m_MuF2_ref = " << m_MuF2_ref << std::endl;
    //std::cout << "get m_MuF2_ref = " << getMuF2Ref() << std::endl;

    // Set default parameter for simple RDDA model
    m_valPara = {-0.25,0.95,2.6};
    m_seaPara = {-0.5,8., 0.21 / (std::tgamma(1.5) * std::tgamma( 9. )/ std::tgamma(1.5 + 9. ) )};
    m_gPara = {-1.,3.,0.23*4};
    m_reggeParaVal = {0.9,1.15,1.54} ;
    m_reggeParaSea = {0.9,-1.48,1.25} ;
    mRDDA_Para = 2.;


    //Relate a specific GPD type with the appropriate function
    m_listGPDComputeTypeAvailable.insert(
        std::make_pair(PARTONS::GPDType::H, &PARTONS::GPDModule::computeH));

    initFunctorsForIntegrations();
}

pionRDDAModel::pionRDDAModel(const pionRDDAModel& other) : PARTONS::GPDModule(other)
{
    // Set reference factorization scale.
    m_MuF2_ref = 1.9 ;
    //std::cout << "m_MuF2_ref = " << m_MuF2_ref << std::endl;
    //std::cout << "get m_MuF2_ref = " << getMuF2Ref() << std::endl;

    // Set default parameter for simple RDDA model
    m_valPara = {-0.25,0.95,2.6};
    m_seaPara = {-0.5,8., 0.21 / (std::tgamma(1.5)*std::tgamma(9)/ std::tgamma(1.5+9) )};
    m_gPara = {-1.,3.,0.23*4};
    m_reggeParaVal = {0.9,1.15,1.54} ;
    m_reggeParaSea = {0.9,-1.48,1.25} ;
    mRDDA_Para = 2.;

    MathIntegratorModule();
    initFunctorsForIntegrations();

}

pionRDDAModel::~pionRDDAModel()
{
    if(m_pIntegralDuVal){ delete m_pIntegralDuVal; m_pIntegralDuVal = 0;}
    if(m_pIntegralDuValMx){ delete m_pIntegralDuValMx; m_pIntegralDuValMx = 0;}
    if(m_pIntegralDdVal){ delete m_pIntegralDdVal; m_pIntegralDdVal = 0;}
    if(m_pIntegralDdValMx){ delete m_pIntegralDdValMx; m_pIntegralDdValMx = 0;}

    if(m_pIntegralHuVal){ delete m_pIntegralHuVal; m_pIntegralHuVal = 0;}
    if(m_pIntegralHuValMx){ delete m_pIntegralHuValMx; m_pIntegralHuValMx = 0;}
    if(m_pIntegralHdVal){ delete m_pIntegralHdVal; m_pIntegralHdVal = 0;}
    if(m_pIntegralHdValMx){ delete m_pIntegralHdValMx; m_pIntegralHdValMx = 0;}

    if(m_pIntegralxLargeHsSea){ delete m_pIntegralxLargeHsSea; m_pIntegralxLargeHsSea = 0;}
    if(m_pIntegralxSmallHsSea){ delete m_pIntegralxSmallHsSea; m_pIntegralxSmallHsSea = 0;}
    if(m_pIntegralxSmall1HsSea){ delete m_pIntegralxSmall1HsSea; m_pIntegralxSmall1HsSea = 0;}
    if(m_pIntegralxSmall2HsSea){ delete m_pIntegralxSmall2HsSea; m_pIntegralxSmall2HsSea = 0;}
    if(m_pIntegralxLargeHsSeaMx){ delete m_pIntegralxLargeHsSeaMx; m_pIntegralxLargeHsSeaMx = 0;}

    if(m_pIntegralDSea){delete m_pIntegralDSea; m_pIntegralDSea = 0;};
    if(m_pIntegralDSeaMx){delete m_pIntegralDSeaMx; m_pIntegralDSeaMx = 0;};

    if(m_pIntegralxLargeHg){ delete m_pIntegralxLargeHg; m_pIntegralxLargeHg = 0;}
    if(m_pIntegralxSmall1Hg){ delete m_pIntegralxSmall1Hg; m_pIntegralxSmall1Hg = 0;}
    if(m_pIntegralxSmall2Hg){ delete m_pIntegralxSmall2Hg; m_pIntegralxSmall2Hg = 0;}
    if(m_pIntegralxLargeHgMx){ delete m_pIntegralxLargeHgMx; m_pIntegralxLargeHgMx = 0;}

  //  if(m_pIntegralDGluons){delete m_pIntegralDGluons; m_pIntegralDGluons = 0;};
  //  if(m_pIntegralDGluonsMx){delete m_pIntegralDGluonsMx; m_pIntegralDGluonsMx = 0;};

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
    MathIntegratorModule::setIntegrator(NumA::IntegratorType1D::GK21_ADAPTIVE);

//Integrators for H

    m_pIntegralHuVal = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralHuVal);

    m_pIntegralHuValMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralHuValMx);

    m_pIntegralHdVal = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralHdVal);

    m_pIntegralHdValMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralHdValMx);

    m_pIntegralxLargeHsSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralxLargeHsSea);

    m_pIntegralxSmallHsSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralxSmallHsSea);

    m_pIntegralxSmall1HsSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralxSmall1HsSea);

    m_pIntegralxSmall2HsSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralxSmall2HsSea);

    m_pIntegralxLargeHsSeaMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralxLargeHsSeaMx);

    m_pIntegralxLargeHg = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralxLargeHg);

    m_pIntegralxSmall1Hg = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralxSmall1Hg);

    m_pIntegralxSmall2Hg = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralxSmall2Hg);

    m_pIntegralxLargeHgMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &pionRDDAModel::IntegralxLargeHgMx);

    //Integrators for the D-terms

    m_pIntegralDuVal = NumA::Integrator1D::newIntegrationFunctor(this,
    		&pionRDDAModel::IntegralDuVal);
    m_pIntegralDuValMx = NumA::Integrator1D::newIntegrationFunctor(this,
    		&pionRDDAModel::IntegralDuValMx);
    m_pIntegralDdVal= NumA::Integrator1D::newIntegrationFunctor(this,
    		&pionRDDAModel::IntegralDdVal);
    m_pIntegralDdValMx = NumA::Integrator1D::newIntegrationFunctor(this,
    		&pionRDDAModel::IntegralDdValMx);

    m_pIntegralDSea = NumA::Integrator1D::newIntegrationFunctor(this,
    		&pionRDDAModel::IntegralDSea);
    m_pIntegralDSeaMx = NumA::Integrator1D::newIntegrationFunctor(this,
    		&pionRDDAModel::IntegralDSeaMx);


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

    if(fabs(beta)==1){return 0; }
    else {
    	profile = pow((1. - fabs(beta)) * (1. - fabs(beta)) - alpha * alpha, ProfileShape);
    	profile /= pow(1. - fabs(beta), TwiceProfileShapePlus1);
    	profile *= tgamma(TwiceProfileShapePlus1 + 1.);
    	profile /= (pow(2., TwiceProfileShapePlus1) * tgamma(ProfileShape + 1.)
    			* tgamma(ProfileShape + 1.));
    	return profile;
    }
}

//forward limit ansatz for H
double pionRDDAModel::valencePdfAnsatz(double beta) {
	double pdf;
    pdf = pow(beta, m_valPara.at(0)) * pow((1. - beta), m_valPara.at(1)) * m_valPara.at(2) ;
    return pdf ;
}

double pionRDDAModel::seaPdfAnsatz(double beta) {
	double pdf;
    pdf = pow(beta, m_seaPara.at(0)) * pow((1. - beta), m_seaPara.at(1)) *  m_seaPara.at(2) ;
    return pdf ;
}

double pionRDDAModel::gluonxPdfAnsatz(double beta) {
	double pdf;
    pdf = pow(beta, m_gPara.at(0)+1) * pow((1. - beta), m_gPara.at(1)) *  m_gPara.at(2) ;
    return pdf ;
}

//Reggeized t behaviour
double pionRDDAModel::tReggeizedAnsatzVal(double beta) {
	double tregge;
	double fbeta = fabs(beta) ;
    tregge = exp(m_t * (  pow((1- fbeta ),3. ) * ( m_reggeParaVal.at(0) * log( 1/fbeta ) + m_reggeParaVal.at(1) ) + m_reggeParaVal.at(2) * fbeta* pow ( (1-fbeta) , 2.)  ) ) ;
    return tregge ;
}

double pionRDDAModel::tReggeizedAnsatzSea(double beta) {
	double tregge;
	double fbeta = fabs(beta) ;
    tregge = exp(m_t * (  pow((1- fbeta ),3. ) * ( m_reggeParaSea.at(0) * log( 1/fbeta ) + m_reggeParaSea.at(1) ) + m_reggeParaSea.at(2) * fbeta* pow ( (1-fbeta) , 2.)  ) ) ;
    return tregge ;
}

//integrals for H
/*
#################################
#
# u quark case
#
##################################


*/

////////////////// Valence part /////////////////////
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
        HuValDD = 0.5 * valencePdfAnsatz(beta)* Profile(beta, alpha) * tReggeizedAnsatzVal(absbeta) ;
    } else {
        HuValDD = 0.;
    }

    return HuValDD;
}

//D-term u valence

double pionRDDAModel::IntegralDuVal(double beta, std::vector<double> Par) {
    double z = m_x / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return DtermuValence(z, beta) ;

}

double pionRDDAModel::IntegralDuValMx(double beta, std::vector<double> Par) {
    double z = m_Mx / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return DtermuValence(z, beta);
}


double pionRDDAModel::DtermuValence(double z, double beta)
{
    double absbeta = fabs(beta);
    double DtermuVal;
    /*    if (beta <= 0 || beta > 1.) {
     throwBetaException(__func__, x);
     }*/
    if (beta > 0.) {
    	DtermuVal = 0.5 * valencePdfAnsatz(beta)* Profile(beta, beta-z)  ;
    } else {
    	DtermuVal = 0.;
    }

    return DtermuVal;
}

/*
#################################
#
# d quark case
#
##################################
*/

double pionRDDAModel::IntegralHdVal(double beta, std::vector<double> Par) {
    double alpha = (m_x + beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return HdValDD(-beta, alpha) / m_xi;

}

double pionRDDAModel::IntegralHdValMx(double beta, std::vector<double> Par) {
    double alpha = (m_Mx + beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return HdValDD(-beta, alpha) / m_xi;
}


double pionRDDAModel::HdValDD(double mbeta, double alpha) {
    double absbeta = fabs(mbeta);
    double HdValDD;
    /*    if (beta <= 0 || beta > 1.) {
     throwBetaException(__func__, x);
     }*/
    if (mbeta < 0.) {
        HdValDD = - 0.5 * valencePdfAnsatz(absbeta)* Profile(absbeta, alpha) * tReggeizedAnsatzVal(absbeta) ;
    } else {
        HdValDD = 0.;
    }

    return HdValDD;
}

//////// D-term

double pionRDDAModel::IntegralDdVal(double beta, std::vector<double> Par) {
    double z = m_x / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return DtermdValence(z,-beta);

}

double pionRDDAModel::IntegralDdValMx(double beta, std::vector<double> Par) {
    double z = (m_Mx) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return DtermdValence(z,-beta);
}



double pionRDDAModel::DtermdValence(double z, double mbeta)
{
    double absbeta = fabs(mbeta);
    double DtermdVal;
    /*    if (beta <= 0 || beta > 1.) {
     throwBetaException(__func__, x);
     }*/
    if (mbeta < 0.) {
    	DtermdVal = -0.5 * valencePdfAnsatz(absbeta)* Profile(absbeta, absbeta-z)  ;
    } else {
    	DtermdVal = 0.;
    }

    return DtermdVal;
}

/*
#################################
#
# Sea contribution
#
##################################
*/

double pionRDDAModel::IntegralxLargeHsSea(double beta,
        std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return HsDD(beta, alpha) / m_xi;
}

double pionRDDAModel::IntegralxLargeHsSeaMx(double beta,        std::vector<double> Par) {
    double alpha = (m_x + beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return HsDD(beta, alpha) / m_xi;
}

double pionRDDAModel::IntegralxSmallHsSea(double beta,
        std::vector<double> Par) {
    return (HsDD(beta, (m_x - beta) / m_xi)
            - HsDD(beta, (m_x + beta) / m_xi)) / m_xi;
}

double pionRDDAModel::IntegralxSmall1HsSea(double beta,
        std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return HsDD(beta, alpha) / m_xi;
}

double pionRDDAModel::IntegralxSmall2HsSea(double beta,
        std::vector<double> Par) {
    double Integral;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return HsDD(beta, (m_x + beta) / m_xi) / m_xi;
}

double pionRDDAModel::HsDD(double beta, double alpha) {
    double absbeta = fabs(beta);
        if (beta <= 0 || beta > 1.) {
     throwBetaException(__func__, beta);
     }

    return   1. / 6. * seaPdfAnsatz(absbeta) * Profile(beta, alpha)  * tReggeizedAnsatzSea(absbeta) ;
}


// D-term part

double pionRDDAModel::IntegralDSea(double beta, std::vector<double> Par)
{	    double z = m_x / m_xi;

		if (beta <= 0 || beta > 1.) {
				throwBetaException(__func__, beta);
		}

		return DtermSea(z,beta);

}

double pionRDDAModel::IntegralDSeaMx(double beta, std::vector<double> Par)
{	    double z = m_Mx / m_xi;

		if (beta <= 0 || beta > 1.) {
			throwBetaException(__func__, beta);
		}

		return DtermSea(z,beta);

}

double pionRDDAModel::DtermSea(double z, double beta){

	double absbeta = fabs(beta);
    double Dtermsea;
    /*    if (beta <= 0 || beta > 1.) {
     throwBetaException(__func__, x);
     }*/
    if (beta > 0.) {
    	Dtermsea =  1. / 6. * seaPdfAnsatz(absbeta)* Profile(absbeta, absbeta-z)  ;
    } else {
    	Dtermsea = 0.;
    }

    return Dtermsea;
}



/*
#################################
#
# gluon contribution
#
##################################
*/

double pionRDDAModel::IntegralxLargeHg(double beta, std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return  HgDD(beta, alpha) / m_xi;
}

double pionRDDAModel::IntegralxLargeHgMx(double beta, std::vector<double> Par) {
    double alpha = (m_x + beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return  HgDD(beta, alpha) / m_xi;
}

double pionRDDAModel::IntegralxSmall1Hg(double beta, std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return  HgDD(beta, alpha) / m_xi;
}

double pionRDDAModel::IntegralxSmall2Hg(double beta, std::vector<double> Par) {
    double alpha = (m_x + beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return  HgDD(beta, alpha) / m_xi;
}

double pionRDDAModel::HgDD(double beta, double alpha) {
    double absbeta = fabs(beta);
        if (beta <= 0 || beta > 1.) {
     throwBetaException(__func__, beta);
     }

    return gluonxPdfAnsatz(absbeta) * Profile(absbeta, alpha)  * tReggeizedAnsatzSea(absbeta) ;
}

///////////////////////////////////////////////////////////////////////////////////////


PARTONS::PartonDistribution pionRDDAModel::computeH()
{

    std::vector<double> x(1), y(1), xi(1), xm(1), ym(1), xim(1);                                            // Declare kinematics in a format appropiated for RadonTransform.

    PARTONS::PartonDistribution partonDistribution;

    // Quark distributions
    QuarkDistribution quarkDistributionUp(QuarkFlavor::UP);
    QuarkDistribution quarkDistributionDown(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistributionStrange(QuarkFlavor::STRANGE);
    //QuarkDistribution quarkDistributionCharm(QuarkFlavor::CHARM);
    //QuarkDistribution quarkDistributionBottom(QuarkFlavor::BOTTOM);
    //QuarkDistribution quarkDistributionTop(QuarkFlavor::TOP);

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

    	    /// gluons

    	    double Hg = 0.;

    	    if (m_x >= m_xi) {
    	        // Integration
    	        Hg = integrate(m_pIntegralxLargeHg, Beta1, Beta2, emptyParameters);

    	    }

    	    if (fabs(m_x) < m_xi) {
    	        Hg = integrate(m_pIntegralxSmall1Hg, 0., Beta2, emptyParameters);
    	        Hg += integrate(m_pIntegralxSmall2Hg, 0., Beta2Mx, emptyParameters);
    	    }

    	    if (m_x <= -m_xi) {
    	        // Integration
    	        Hg = integrate(m_pIntegralxLargeHgMx, Beta1Mx, Beta2Mx,
    	                emptyParameters);
    	    }

    	    //Looking for nan
    	    if(std::isnan(Hg)){ std::cout << "Nan detected in gluon gpd computation   for xi = " << m_xi << " and x = " << m_x << std::endl ;}

    	    //Gluon Distribution
    	    GluonDistribution gluonDistribution(Hg);

    	    // s quark
    	        double Hs = 0;

    	        if (m_x >= m_xi) {
    	            Hs = integrate(m_pIntegralxLargeHsSea, Beta1, Beta2, emptyParameters);
    	        }

    	        if (fabs(m_x) < m_xi) {
    	            Hs = integrate(m_pIntegralxSmallHsSea, Eps, Beta2Min, emptyParameters);
    	            Hs += integrate(m_pIntegralxSmall1HsSea, Beta2Min, Beta2,
    	                    emptyParameters);
    	            Hs -= integrate(m_pIntegralxSmall2HsSea, Beta2Min, Beta2Mx,
    	                    emptyParameters);
    	            std::cout <<"Hs no dterm = " << Hs << std::endl;
    	            //Dterm
    	            Hs-= integrate(m_pIntegralDSea,Eps, (1+m_x/m_xi)/2., emptyParameters )- integrate(m_pIntegralDSeaMx,Eps, (1+m_Mx/m_xi)/2., emptyParameters );
    	            std::cout <<"Hs with dterm = " << Hs << std::endl;



    	        }

    	        if (m_x <= -m_xi) {
    	            Hs = -integrate(m_pIntegralxLargeHsSeaMx, Beta1Mx, Beta2Mx,
    	                    emptyParameters);
    	        }

    	        if(std::isnan(Hs)){ std::cout << "Nan detected in s-quark gpd computation for xi = " << m_xi << " and x = " << m_x << std::endl ;}
    	        quarkDistributionStrange.setQuarkDistribution(Hs);




    	    // u and d quark, valence part evaluated at fx

    	        double HuVal = 0.;
    	        double HdVal = 0.;
    	        double HuValMx = 0.;
    	        double HdValMx = 0.;
    	        double Dtermu = 0.;
    	        double Dtermd = 0.;

    	        if (m_x >= m_xi) {
    	            // Integration, u quark
    	            HuVal = integrate(m_pIntegralHuVal, Beta1, Beta2, emptyParameters);

    	            // Integration, d quark
    	            HdValMx = integrate(m_pIntegralHdValMx, Beta1, Beta2, emptyParameters);
    	        }

    	        if (fabs(m_x) < m_xi) {
    	            // Integration, u quark
    	            HuVal = integrate(m_pIntegralHuVal, Eps, Beta2, emptyParameters);
    	            //D-term
    	            HuVal -= 0.5 * ( integrate(m_pIntegralDuVal, Eps, ((1+m_x/m_xi)/2), emptyParameters)
    	            	- integrate(m_pIntegralDuValMx, Eps, ((1+m_Mx/m_xi)/2), emptyParameters));


    	            // Integration, d quark
    	            HdValMx = integrate(m_pIntegralHdValMx, Eps, Beta2, emptyParameters);
    	            //D-term
    	            HdValMx -= -0.5 * ( integrate(m_pIntegralDdValMx, Eps, (1+m_Mx/m_xi)/2, emptyParameters)
    	            		- integrate(m_pIntegralDdVal, Eps, (1+m_x/m_xi)/2, emptyParameters));
    	        }

    	    ///////////////////////////////////////////////////////////////////////
    	    //   u and d quarks, valence part evaluated at fMx (instead of fx)   //
    	    ///////////////////////////////////////////////////////////////////////


    	        if (m_Mx >= m_xi) {

    	            // Integration, u quark
    	            HuValMx = MathIntegratorModule::integrate(m_pIntegralHuValMx, Beta1Mx, Beta2Mx,
    	                    emptyParameters);

    	            // Integration, d quark
    	            HdVal = integrate(m_pIntegralHdVal, Beta1Mx, Beta2Mx,
    	                    emptyParameters);
    	        }

    	        if (fabs(m_Mx) < m_xi) {

    	        	// Integration, u quark
    	            HuValMx =  MathIntegratorModule::integrate(m_pIntegralHuValMx, Eps, Beta2Mx, emptyParameters);
    	            HuValMx -= -0.5 * ( integrate(m_pIntegralDuVal, Eps, ((1+m_x/m_xi)/2), emptyParameters)
        	            	- integrate(m_pIntegralDuValMx, Eps, ((1+m_Mx/m_xi)/2), emptyParameters));

    	            // Integration, d quark
    	            HdVal = integrate(m_pIntegralHdVal, Eps, Beta2Mx, emptyParameters);
    	            HdVal -= -0.5 * ( integrate(m_pIntegralDdVal, Eps, (1+m_x/m_xi)/2, emptyParameters)
    	            		- integrate(m_pIntegralDdValMx, Eps, (1+m_Mx/m_xi)/2, emptyParameters));

    	        }


    	        if(std::isnan(HuVal)){ std::cout << "Nan detected in HuVal computation for xi = " << m_xi << " and x = " << m_x << std::endl ;}
    	        if(std::isnan(HuValMx)){ std::cout << "Nan detected in HuValMx computation for xi = " << m_xi << " and x = " << m_x << std::endl ;}
    	        if(std::isnan(HdVal)){ std::cout << "Nan detected in HdVal computation for xi = " << m_xi << " and x = " << m_x << std::endl ;}
    	        if(std::isnan(HdValMx)){ std::cout << "Nan detected in HdValMx computation  for xi = " << m_xi << " and x = " << m_x << std::endl ;}

    //std::cout << "Huval = " << HuVal << " Hdval = " << HdVal << std::endl;

    // TODO: Check and fix definitions of the the different quark distributions (u, uM, u+, u-, d(...) and s(...)). See Cédric PhD thesis. pp. 56.
    double HuSea  = Hs;
    double HuSeaMx = -Hs ;

    quarkDistributionUp.setQuarkDistribution(HuVal + HuSea);

        // Singlet distribution
        quarkDistributionUp.setQuarkDistributionPlus(HuVal + HuSea - HuValMx - HuSeaMx);
        // Nonsinglet distributiion
        quarkDistributionUp.setQuarkDistributionMinus(HuVal + HuValMx);

    // d-quark
    double dSea = Hs ;
    double dSeaM = -Hs ;

    quarkDistributionDown.setQuarkDistribution(HdVal + dSea);

        // Singlet distribution
        quarkDistributionDown.setQuarkDistributionPlus(HdVal + dSea - HdValMx - dSeaM);
        // Nonsinglet distributiion
        quarkDistributionDown.setQuarkDistributionMinus(HdVal + HdValMx);

    // s-quark
    double sVal = 0.;
    double sValM = 0.;
    double sSea = Hs ;
    double sSeaM = -Hs ;

    quarkDistributionStrange.setQuarkDistribution(sVal + sSea);
    quarkDistributionStrange.setQuarkDistributionPlus(sVal + sSea - sValM - sSeaM);
    quarkDistributionStrange.setQuarkDistributionMinus(sVal + sValM);

    std::cout << "computation completed for (x,xi) = " << m_x << " " << m_xi << std::endl;

    /*
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
    quarkDistributionStrange.setQuarkDistributionMinus(tVal + tValM);*/

    partonDistribution.addQuarkDistribution(quarkDistributionUp);
    partonDistribution.addQuarkDistribution(quarkDistributionDown);
    partonDistribution.addQuarkDistribution(quarkDistributionStrange);
    //partonDistribution.addQuarkDistribution(quarkDistributionCharm);
    //partonDistribution.addQuarkDistribution(quarkDistributionBottom);
    //partonDistribution.addQuarkDistribution(quarkDistributionTop);

    partonDistribution.setGluonDistribution(gluonDistribution);

    return partonDistribution;
}


}
