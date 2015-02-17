/*
 * DVCSCFFHeavyQuarkModel.cpp
 *
 *  Created on: Feb 11, 2015
 *      Author: kuba
 */

#include "DVCSCFFHeavyQuarkModel.h"
#include "../../services/ModuleObjectFactory.h"
#include "../../FundamentalPhysicalConstants.h"
#include "../../utils/logger/LoggerManager.h"
#include "../../utils/stringUtils/Formatter.h"
#include "../alphaS/RunningAlphaStrong.h"
#include "../math/RootIntegrationMode.h"
#include <iostream>

// Initialise [class]::moduleID with a unique name.
const std::string DVCSCFFHeavyQuarkModel::moduleID =
        ModuleObjectFactory::getInstance()->registerModule(
                new DVCSCFFHeavyQuarkModel("DVCSCFFHeavyQuarkModel"));

DVCSCFFHeavyQuarkModel::DVCSCFFHeavyQuarkModel(const std::string& className):DVCSCFFModel(className),
		m_betas(std::complex<double>(0.0,0.0)),m_rs(std::complex<double>(0.0,0.0)),
		m_betaq(std::complex<double>(0.0,0.0)),m_rq(std::complex<double>(0.0,0.0)),
		m_TF(std::complex<double>(0.5,0.0)),m_eps(1.e-5) {

}


DVCSCFFHeavyQuarkModel* DVCSCFFHeavyQuarkModel::clone() const {
	return new DVCSCFFHeavyQuarkModel(*this);
}

DVCSCFFHeavyQuarkModel::~DVCSCFFHeavyQuarkModel() {
	// TODO Auto-generated destructor stub
}

DVCSCFFHeavyQuarkModel::DVCSCFFHeavyQuarkModel(const DVCSCFFHeavyQuarkModel& other)
	: DVCSCFFModel(other) {
	 m_betas = other.m_betas;
	 m_betaq = other.m_betaq;
	 m_rs = other.m_rs;
	 m_rq = other.m_rq;
	 m_TF = other.m_TF;
	 m_eps = other.m_eps;
}

/*void DVCSCFFHeavyQuarkModel::initModule() {
	 DVCSCFFModel::initModule();

	    m_Q = sqrt(m_Q2);
	    m_Zeta = 2. * m_xi / (1 + m_xi);
	    m_logQ2OverMu2 = 2. * log(m_Q / m_MuF);
	    m_nbOfActiveFlavour = 1;

	    RunningAlphaStrong* Alpha = new RunningAlphaStrong();
	    Alpha->SetRunningScale(m_MuR);
	    m_alphaSOver2Pi = Alpha->GetAlphaS() / (2. * PI);
	    delete Alpha;
}*/

//void DVCSCFFHeavyQuarkModel::isModuleWellConfigured() {
//}

std::complex<double> DVCSCFFHeavyQuarkModel::KernelGluonNLOA(double x) {
//    m_pLoggerManager->debug(getClassName(), __func__, "entered");
// TODO Add a massless part
	  double z =  m_xi/x;
	  double mq = QUARK_CHARM_MASS;
	  double s = -m_Q2*(z-1.)/2./z;
	  m_betas = beta(s,mq);
	  m_rs = r(s,mq);
	  m_betaq = beta(-m_Q2,mq);
	  m_rq = r(-m_Q2,mq);
	    std::complex<double> GluonNLOA(m_betas*std::log(m_rs)-m_betaq*std::log(m_rq));
	    GluonNLOA *= (2.*z-6.)/ (z-1.);
	    GluonNLOA +=  std::log(m_rs)*std::log(m_rs) - std::log(m_rq)*std::log(m_rq);
	    GluonNLOA *= (m_TF/ 2./(z+1.)/(z+1.));
	    GluonNLOA /= x*x;
	    m_pLoggerManager->debug(getClassName(), __func__,
	                Formatter() << "x= " << x <<"    GluonNLOA RE = "<< GluonNLOA.real()<<"   GluonNLOA IM = "<< GluonNLOA.imag());

	    return GluonNLOA;
}

std::complex<double> DVCSCFFHeavyQuarkModel::KernelGluonNLOV(double x) {
	std::complex<double> z =  -std::complex<double>(m_xi,-m_eps)/x;
	double mq = QUARK_CHARM_MASS;
	std::complex<double> s = -m_Q2*(z-1.)/2./z;
	double eta = mq*mq/m_Q2;

	m_betas = beta(s,mq);
	m_rs = r(s,mq);
	m_betaq = beta(-m_Q2,mq);
	m_rq = r(-m_Q2,mq);

	std::complex<double> GluonNLOV(m_betas*std::log(m_rs)-m_betaq*std::log(m_rq));
		    GluonNLOV *= (2.-z*(6.-16.*eta))/ (z-1.);
		    GluonNLOV -= (1.+8.*eta*z*(1.-(1.+4.*eta)*z)/(z-1.)/(z-1.))*std::log(m_rs)*std::log(m_rs);
		    GluonNLOV += (1.-2.*eta*(1.+(3.+8.*eta)*z*z)/(z-1.)/(z-1.))*std::log(m_rq)*std::log(m_rq);
		    GluonNLOV *= (m_TF/ 2./(z+1.)/(z+1.));
		    GluonNLOV /= x*x;

		    m_pLoggerManager->debug(getClassName(), __func__,
		                Formatter() << "x= " << x <<"    GluonNLOV RE = "<< GluonNLOV.real()<<"   GluonNLOV IM = "<< GluonNLOV.imag());

	return GluonNLOV;
}

std::complex<double> DVCSCFFHeavyQuarkModel::computeUnpolarized() {
	return DVCSCFFModel::computeUnpolarized();
}

std::complex<double> DVCSCFFHeavyQuarkModel::computePolarized() {
	return DVCSCFFModel::computePolarized();
}

std::complex<double> DVCSCFFHeavyQuarkModel::beta(std::complex<double> s, double mq) {
//	std::complex<double> beta(1.-4.*mq*mq/(s+m_eps*m_eps)*s, 4.*mq*mq/(s+m_eps*m_eps)*m_eps);
	std::complex<double> beta(1.-4.*mq*mq/(s+std::complex<double>(0.0,m_eps)));
	return std::sqrt(beta);
}

std::complex<double> DVCSCFFHeavyQuarkModel::r(std::complex<double> s, double mq) {
	std::complex<double> tmp_beta = beta(s,mq);
	return (tmp_beta-1.)/(tmp_beta+1.);
}

void DVCSCFFHeavyQuarkModel::computeSubtractionFunctionsV() {
	    m_realPartSubtractQuark = 0.;
	    m_imaginaryPartSubtractQuark =0. ;
	    m_realPartSubtractGluon = 0.;
	    m_imaginaryPartSubtractGluon = 0.;
}
