#include "../../../../../include/partons/modules/convol_coeff_function/JET/JETCFFStandard.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <gsl/gsl_deriv.h>
#include <gsl/gsl_math.h>
#include <utility>

#include "../../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h"
#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/List.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/convol_coeff_function/ConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"

namespace PARTONS {

const unsigned int JETCFFStandard::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new JETCFFStandard("JETCFFStandard"));

JETCFFStandard::JETCFFStandard(const std::string &className) :
        JETConvolCoeffFunctionModule(className) {

    //relate GPD types with functions to be used
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &JETConvolCoeffFunctionModule::computeUnpolarized));

    //this module does not depend on GPDs
    setIsGPDModuleDependent(true);
}

JETCFFStandard::JETCFFStandard(const JETCFFStandard &other) :
        JETConvolCoeffFunctionModule(other) {
}

JETCFFStandard::~JETCFFStandard() {
}

JETCFFStandard* JETCFFStandard::clone() const {
    return new JETCFFStandard(*this);
}

void JETCFFStandard::resetPreviousKinematic() {
	m_lastH = JETConvolCoeffFunctionKinematic();
}

std::complex<double> JETCFFStandard::computeUnpolarized() {

    if (m_qcdOrderType != PerturbativeQCDOrderType::LO) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "QCD order: "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString()
                        << " not implemented");
    }

	JETCFFStandardIntegrationParameters params;

	params.m_pGPDModule = m_pGPDModule;
	params.m_gpdType = GPDType::H;

	params.m_xi = m_xi;
	params.m_t = m_t;
	params.m_MuF2 = m_MuF2;
	params.m_MuR2 = m_MuR2;

	//quark mass
	double m;

	switch(m_jetType){

		case JetType::UP:
		case JetType::DOWN:
		case JetType::STRANGE:{
			m = 0.; break;
		}

		case JetType::CHARM:{
			m = 1.25; break;
		}

		default:{
			throw ElemUtils::CustomException(getClassName(), __func__, "unknown type for assignment");
		}
	}

	//mu2 (Eq. 15 from https://arxiv.org/pdf/hep-ph/0505263)
	double mu2 = pow(m, 2) + m_z * (1. - m_z) * m_Q2;

	//beta (Eq. 16 from https://arxiv.org/pdf/hep-ph/0505263)
	double beta = mu2 / (m_qPerp2 + mu2);

	//number of colors
	constexpr double Nc = 3.;
	constexpr double CF = (Nc * Nc - 1.) / (2 * Nc);

	// basic =====================================================================

	JETConvolCoeffFunctionKinematic thisKin(m_xi, m_t, m_z, m_qPerp2, m_Q2, m_MuF2, m_MuR2, m_jetType, m_jetCFFType);

	if (thisKin != m_lastH) {
		// gluon
		params.m_partonType = JETCFFStandardIntegrationParameters::Parton::GLUON;

		// Hg/(x+xi-i0)^2
		params.m_order = JETCFFStandardIntegrationParameters::Order::Second;
		params.m_xi0 = -1 * m_xi;
		params.m_epsilonSign = JETCFFStandardIntegrationParameters::EpsilonSign::Minus;

		std::complex<double> cff_g2P = params.evaluateConvolution();

		// Hg/(x-xi+i0)^2
		params.m_order = JETCFFStandardIntegrationParameters::Order::Second;
		params.m_xi0 = m_xi;
		params.m_epsilonSign = JETCFFStandardIntegrationParameters::EpsilonSign::Plus;

		std::complex<double> cff_g2M = params.evaluateConvolution();

		// Hg/(x+xi-i0)
		params.m_order = JETCFFStandardIntegrationParameters::Order::First;
		params.m_xi0 = -1 * m_xi;
		params.m_epsilonSign = JETCFFStandardIntegrationParameters::EpsilonSign::Minus;

		std::complex<double> cff_g1P = params.evaluateConvolution();

		// Hg/(x-xi+i0)
		params.m_order = JETCFFStandardIntegrationParameters::Order::First;
		params.m_xi0 = m_xi;
		params.m_epsilonSign = JETCFFStandardIntegrationParameters::EpsilonSign::Plus;

		std::complex<double> cff_g1M = params.evaluateConvolution();

		// quark
		params.m_partonType = JETCFFStandardIntegrationParameters::jetTypeToParton(m_jetType);

		// Hq/(x+xi-i0)
		params.m_order = JETCFFStandardIntegrationParameters::Order::First;
		params.m_xi0 = -1 * m_xi;
		params.m_epsilonSign = JETCFFStandardIntegrationParameters::EpsilonSign::Minus;

		std::complex<double> cff_q1P = params.evaluateConvolution();

		// Hq/(x-xi+i0)
		params.m_order = JETCFFStandardIntegrationParameters::Order::First;
		params.m_xi0 = m_xi;
		params.m_epsilonSign = JETCFFStandardIntegrationParameters::EpsilonSign::Plus;

		std::complex<double> cff_q1M = params.evaluateConvolution();

		// Hq/(x-xi(1-2beta)-i0)
		params.m_order = JETCFFStandardIntegrationParameters::Order::First;
		params.m_xi0 = m_xi * (1 - 2 * beta);
		params.m_epsilonSign = JETCFFStandardIntegrationParameters::EpsilonSign::Minus;

		std::complex<double> cff_q1BM = params.evaluateConvolution();

		// Hq/(x+xi(1-2beta)+i0)
		params.m_order = JETCFFStandardIntegrationParameters::Order::First;
		params.m_xi0 = -1 * m_xi * (1 - 2 * beta);
		params.m_epsilonSign = JETCFFStandardIntegrationParameters::EpsilonSign::Plus;

		std::complex<double> cff_q1BP = params.evaluateConvolution();

		// Braun's =====================================================================

		// ILg (Eq. 30 from https://arxiv.org/pdf/hep-ph/0505263)
		m_ILg = 2 * m_xi * (1. - beta) * (cff_g2P + cff_g2M) - (1. - 2 * beta) * (cff_g1M - cff_g1P);

		// ILq (Eq. 30 from https://arxiv.org/pdf/hep-ph/0505263)
		m_ILq = 2 * m_xi * ((1. - m_z) * cff_g1P + m_z * cff_g1M);

		// ITg (Eq. 32 from https://arxiv.org/pdf/hep-ph/0505263)
		m_ITg = m_xi * (1. - 2 * beta) * (cff_g2P + cff_g2M) + 2 * beta * (cff_g1M - cff_g1P);

		// ITq1 (Eq. 32 from https://arxiv.org/pdf/hep-ph/0505263)
		m_ITq1 = 2 * m_xi * (      m_z * (1. - m_z) * cff_q1M - beta * pow(1. - m_z, 2) / (1. - beta) * cff_q1P + pow(1. - m_z, 2) / (1. - beta) * cff_q1BM);

		// ITq1 (Eq. 32 from https://arxiv.org/pdf/hep-ph/0505263)
		m_ITq2 = 2 * m_xi * (-1 *  m_z * (1. - m_z) * cff_q1P + beta * pow(     m_z, 2) / (1. - beta) * cff_q1M - pow(     m_z, 2) / (1. - beta) * cff_q1BP);

		m_lastH = thisKin;
	}

	// CFFs =====================================================================

	switch (m_jetCFFType) {

		case JetCFFType::LL: {
			return m_ILg + 2 * CF * m_ILq;
		}

		case JetCFFType::TL: {
			return m_ILg;
		}

		case JetCFFType::TT1: {
			return 2 * CF * m_ITq1 + (1. - m_z) * m_ITg;
		}

		case JetCFFType::TT2: {
			return 2 * CF * m_ITq2 - m_z * m_ITg;
		}

		default: {
			throw ElemUtils::CustomException(getClassName(), __func__, "unknown type for return");
		}
	}
}


} /* namespace PARTONS */
