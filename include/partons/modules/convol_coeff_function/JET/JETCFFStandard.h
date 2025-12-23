#ifndef JET_CFF_FROM_DVCS_H
#define JET_CFF_FROM_DVCS_H

/**
 * @file JETCFFStandard.h
 * @author Pawel Sznajder (NCBJ)
 * @date December 23, 2025
 * @version 1.0
 */

#include <complex>
#include <string>
#include <gsl/gsl_integration.h>

#include "JETConvolCoeffFunctionModule.h"
#include "../../../beans/gpd/GPDKinematic.h"

namespace PARTONS {

/**
 * Structure for evaluation of JET CFFs.
 */
struct JETCFFStandardIntegrationParameters{

    enum Parton{
        GLUON = 0,
        UP = 1,
        DOWN = 2,
        STRANGE = 3,
    	CHARM = 4
    };

    static Parton jetTypeToParton(JetType::Type jetType){

    	switch(jetType){

    	case JetType::GLUON:{
    		return Parton::GLUON;
    	}

    	case JetType::UP:{
    		return Parton::UP;
    	}

    	case JetType::DOWN:{
    		return Parton::DOWN;
    	}

    	case JetType::STRANGE:{
    		return Parton::STRANGE;
    	}

    	case JetType::CHARM:{
    		return Parton::CHARM;
    	}

    	default:{
    		throw ElemUtils::CustomException("JETCFFStandardIntegrationParameters", __func__,
    			"unknown type for conversion");
    	}

    	}
    }

	double computeGPD(double x) {

    	PartonDistribution pd = m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2), m_gpdType);

    	switch(m_partonType) {
    		case GLUON:{
    			return pd.getGluonDistribution().getGluonDistribution();
    		}

    		case UP:{
    			return pd.getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution();
    		}

    		case DOWN:{
    			return pd.getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistribution();
    		}

    		case STRANGE:{
    			return pd.getQuarkDistribution(QuarkFlavor::STRANGE).getQuarkDistribution();
    		}

    		case CHARM:{

    			if (pd.getQuarkDistributions().find(QuarkFlavor::CHARM) == pd.getQuarkDistributions().end()) {
    				return 0.;
    			}

    			return pd.getQuarkDistribution(QuarkFlavor::CHARM).getQuarkDistribution();
    		}

    		default:{
    			throw ElemUtils::CustomException("JETCFFStandardIntegrationParameters", __func__,
					"unknown type for evaluation");
    		}
    	}
    }

	void computeSubtractionTerms(){

		constexpr double eps = 1.E-6;

    	m_valueDer0AtXi0 = computeGPD(m_xi0);
		if (m_order == Second) m_valueDer1AtXi0 = (computeGPD(m_xi0 + eps) - m_valueDer0AtXi0) / eps;
    }

	static double evaluateConvolutionIntegrand(double x, void* par){

    	JETCFFStandardIntegrationParameters* params = static_cast<JETCFFStandardIntegrationParameters*>(par);

    	double gpd = params->computeGPD(x);

    	switch(params->m_order){

    		case First : {
    			return (gpd - params->m_valueDer0AtXi0) / (x - params->m_xi0);
    		}
    		case Second : {
    			return (gpd - params->m_valueDer0AtXi0 - (x - params->m_xi0) * params->m_valueDer1AtXi0) / pow(x - params->m_xi0, 2);
    		}
    	}

    	return 0.;
    }

	std::complex<double> evaluateConvolution(){

    	//compute
    	computeSubtractionTerms();

    	//real part
    	double re = 0.;
    	double thisRe = 0.;
    	double thisReD = 0.;
    	int status = -1;

    	gsl_function F;
    	F.function = &evaluateConvolutionIntegrand;
    	F.params   = this;

    	const size_t workspace_size = 1000;
    	gsl_integration_workspace* workspace = gsl_integration_workspace_alloc(workspace_size);

    	constexpr double eps = 1.E-2;

    	status = gsl_integration_qag(
			&F,
			-1. + eps,
			 m_xi0,
			1.E-3,
			1.E-3,
			workspace_size,
			GSL_INTEG_GAUSS15,
			workspace,
			&thisRe,
			&thisReD
		);

    	re += thisRe;

    	status = gsl_integration_qag(
			&F,
			 m_xi0,
			 1. - eps,
			1.E-3,
			1.E-3,
			workspace_size,
			GSL_INTEG_GAUSS15,
			workspace,
			&thisRe,
			&thisReD
		);

    	re += thisRe;

    	gsl_integration_workspace_free(workspace);

    	//imaginary part and border terms
    	double im = 0.;

    	switch(m_order){

    		case JETCFFStandardIntegrationParameters::Order::First : {

    			re += m_valueDer0AtXi0 * log(fabs(1. - m_xi0) / fabs(-1. - m_xi0));

    			im = M_PI * m_valueDer0AtXi0;
    			if(m_epsilonSign == JETCFFStandardIntegrationParameters::EpsilonSign::Plus) im *= -1;

    			break;
    		}

    		case JETCFFStandardIntegrationParameters::Order::Second : {

    			re += m_valueDer0AtXi0 * (1. / (-1. - m_xi0) - 1/(1. - m_xi0));
    			re += m_valueDer1AtXi0 * log(fabs(1. - m_xi0) / fabs(-1. - m_xi0));

    			im = M_PI * m_valueDer1AtXi0;
    			if(m_epsilonSign == JETCFFStandardIntegrationParameters::EpsilonSign::Plus) im *= -1;

    			break;
    		}
    	}

    	//return
    	return std::complex<double>(re, im);
    }

    enum Order{
        First = 0,
        Second = 1
    };

    enum EpsilonSign{
        Minus = 0,
        Plus = 1
    };

    GPDModule* m_pGPDModule;
    GPDType::Type m_gpdType;
    Parton m_partonType;

	double m_xi, m_t, m_MuF2, m_MuR2;

    Order m_order;
    EpsilonSign m_epsilonSign;

    double m_xi0;

    double m_valueDer0AtXi0;
    double m_valueDer1AtXi0;
};

/**
 * @class JETCFFStandard
 *
 * This module calculates JET Compton Form Factors.
 *
 * Available CFF types: H.
 */
class JETCFFStandard: public JETConvolCoeffFunctionModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    JETCFFStandard(const std::string &className);
    virtual ~JETCFFStandard();

    virtual JETCFFStandard* clone() const;

    /**
     * Reset previous kinematics.
     */
    void resetPreviousKinematic();

protected:

    /**
     * Copy constructor.
     * @param other
     */
    JETCFFStandard(const JETCFFStandard &other);

    virtual std::complex<double> computeUnpolarized();

	//Basic CFFs stored to avoid recalculation
	JETConvolCoeffFunctionKinematic m_lastH;
	std::complex<double> m_ILg, m_ILq, m_ITg, m_ITq1, m_ITq2;
};

} /* namespace PARTONS */

#endif /* JET_CFF_FROM_DVCS_H */
