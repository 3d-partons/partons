#include "DVCSCFFModel.h"

#include <Math/AllIntegrationTypes.h>
#include <Math/Integrator.h>
#include <Math/WrappedTF1.h>
#include <TF1.h>
#include <TMath.h>
#include <cmath>
#include <map>
#include <stdexcept>
#include <utility>
#include <complex>

#include "../../beans/cff/CFFInputData.h"
#include "../../beans/cff/CFFOutputData.h"
#include "../../beans/gpd/GPDOutputData.h"
#include "../../beans/gpd/GPDQuarkFlavorData.h"
#include "../../beans/gpd/GPDResultData.h"
#include "../../beans/QCDOrderType.h"
#include "../../beans/QuarkFlavor.h"
#include "../../FundamentalPhysicalConstants.h"
#include "../../services/ModuleObjectFactory.h"
#include "../../utils/stringUtils/Formatter.h"
#include "../../utils/logger/LoggerManager.h"
#include "../alphaS/RunningAlphaStrong.h"
#include "../GPDModule.h"

// Initialise GK11GPDModule::moduleID with a unique name.
const std::string DVCSCFFModel::moduleID = "DVCSCFFModel";

// Define a useless static boolean variable to enable registerModule() to be executed before the main() function.
// Because global variables have program scope, and are initialised before main() is called.
// !!! CARE !!! variable name must be unique.
static bool isDVCSCFFModuleRegistered =
		ModuleObjectFactory::getInstance()->registerModule(new DVCSCFFModel());

DVCSCFFModel::DVCSCFFModel() :
		CFFModule(DVCSCFFModel::moduleID), m_CF(4. / 3.)/*, m_integrator(
 ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR, 0., 1.e-3)*/{

	m_listOfCFFComputeFunctionAvailable.insert(
			std::make_pair(GPDComputeType::H, &CFFModule::computeUnpolarized));
	m_listOfCFFComputeFunctionAvailable.insert(
			std::make_pair(GPDComputeType::E, &CFFModule::computeUnpolarized));
	m_listOfCFFComputeFunctionAvailable.insert(
			std::make_pair(GPDComputeType::Ht, &CFFModule::computePolarized));
	m_listOfCFFComputeFunctionAvailable.insert(
			std::make_pair(GPDComputeType::Et, &CFFModule::computePolarized));
}

DVCSCFFModel::DVCSCFFModel(const DVCSCFFModel &other) :
		CFFModule(other) {
	m_Zeta = other.m_Zeta;
	m_logQ2OverMu2 = other.m_logQ2OverMu2;
	m_Q = other.m_Q;
	m_nbOfActiveFlavour = other.m_nbOfActiveFlavour;
	m_alphaSOver2Pi = other.m_alphaSOver2Pi;
	m_quarkDiagonal = other.m_quarkDiagonal;
	m_gluonDiagonal = other.m_gluonDiagonal;

	m_realPartSubtractQuark = other.m_realPartSubtractQuark;
	m_imaginaryPartSubtractQuark = other.m_imaginaryPartSubtractQuark;
	m_realPartSubtractGluon = other.m_realPartSubtractGluon;
	m_imaginaryPartSubtractGluon = other.m_imaginaryPartSubtractGluon;

	m_CF = other.m_CF;
}

//TODO implement clone function
DVCSCFFModel* DVCSCFFModel::clone() const {
	return new DVCSCFFModel(*this);
}

DVCSCFFModel::~DVCSCFFModel() {

}

void DVCSCFFModel::initModule() {
	m_xi = m_xB / (2 - m_xB);
	m_Q = sqrt(m_Q2);
	m_Zeta = 2. * m_xi / (1 + m_xi);
	m_logQ2OverMu2 = 2. * log(m_Q / m_MuF);
	m_nbOfActiveFlavour = 3;

	RunningAlphaStrong* Alpha = new RunningAlphaStrong();
	Alpha->SetRunningScale(m_MuR);
	m_alphaSOver2Pi = Alpha->GetAlphaS() / (2. * PI);
	delete Alpha;
}

void DVCSCFFModel::isModuleWellConfigured() {
	if (m_pGPDModule == 0) {
		throw std::runtime_error("[DVCSCFFModel] GPDModule* is NULL");
	}
	if (m_qcdOrderType == QCDOrderType::UNDEFINED) {
		throw std::runtime_error("[DVCSCFFModel] QCDOrderType is UNDEFINED");
	}
}

//TODO voir pourquoi CFFInoutData se retrouve NULL lors de la copie de CFFOutputData
CFFOutputData DVCSCFFModel::compute(const double &xB, const double &t,
		const double Q2, const double &MuF, const double &MuR,
		GPDComputeType::Type gpdComputeType) {

	CFFOutputData cffOutputData(new CFFInputData(xB, t, Q2));

	m_xB = xB;
	m_t = t;
	m_Q2 = Q2;
	m_MuF = MuF;
	m_MuR = MuR;

	isModuleWellConfigured();

	initModule();

	switch (gpdComputeType) {
	case GPDComputeType::ALL: {

		for (m_it = m_listOfCFFComputeFunctionAvailable.begin();
				m_it != m_listOfCFFComputeFunctionAvailable.end(); m_it++) {
			m_currentGPDComputeType = m_it->first;

			m_pLoggerManager->debug(getClassName(), __func__,
					Formatter() << "m_currentGPDComputeType = "
							<< GPDComputeType(m_currentGPDComputeType).toString());

			// call function store in the base class map
			cffOutputData.add(m_currentGPDComputeType,
					((*this).*(m_it->second))());
		}
		break;
	}
	default: {
		m_it = m_listOfCFFComputeFunctionAvailable.find(gpdComputeType);
		if (m_it != m_listOfCFFComputeFunctionAvailable.end()) {
			m_currentGPDComputeType = gpdComputeType;
			// call function store in the base class map
			cffOutputData.add(m_currentGPDComputeType,
					((*this).*(m_it->second))());
		} else {
			//TODO can't compute this
		}
		break;
	}
	}

	return cffOutputData;
}

std::complex<double> DVCSCFFModel::computeUnpolarized() {

	computeDiagonalGPD();
	computeSubtractionFunctionsV();
	return computeIntegralsV();
}

std::complex<double> DVCSCFFModel::computePolarized() {

	computeDiagonalGPD();
	computeSubtractionFunctionsA();
	return computeIntegralsV();
}

void DVCSCFFModel::computeDiagonalGPD() {
	GPDOutputData gpdOutputData = m_pGPDModule->compute(m_xi, m_xi, m_t, m_MuF,
			m_MuR, m_currentGPDComputeType);
	GPDResultData* pGPDResultData = gpdOutputData.getGPDResultData(
			m_currentGPDComputeType);

	//TODO compute CFF singlet; FAIT; vérifier le résultat du calcul
	m_quarkDiagonal = computeSquareChargeAveragedGPD(gpdOutputData);
	m_gluonDiagonal = 2. * pGPDResultData->getGluon();
}

double DVCSCFFModel::computeSquareChargeAveragedGPD(
		GPDOutputData &gpdOutputData) {
//TODO comment faire evoluer le calcul si de nouvelles saveurs de quark entrent en jeux
	double result = 0.;
	GPDResultData* gpdResultData = gpdOutputData.getGPDResultData(
			m_currentGPDComputeType);
	result +=
			gpdResultData->getGPDQuarkFlavorData(QuarkFlavor::UP)->getPartonDistributionSinglet()
					* U2_ELEC_CHARGE;
	result +=
			gpdResultData->getGPDQuarkFlavorData(QuarkFlavor::DOWN)->getPartonDistributionSinglet()
					* D2_ELEC_CHARGE;
	result +=
			gpdResultData->getGPDQuarkFlavorData(QuarkFlavor::STRANGE)->getPartonDistributionSinglet()
					* S2_ELEC_CHARGE;

//    std::vector<> = gpdResultData.listQuarkTypeComputed()
//
//    for()
//    {
//        if( MuF || Q >< c.getSinglet() )
//    }

	return result;
}

void DVCSCFFModel::computeSubtractionFunctionsV() {
	double LogZeta = log(m_Zeta);
	double LogInvZeta = log((1. - m_Zeta) / m_Zeta);
	double LogInvZeta2 = LogInvZeta * LogInvZeta;
	double DiLogInvZeta = TMath::DiLog(1. - 1. / m_Zeta);
	double Pi2 = PI * PI;

	double RealPartSubtractQuarkLO; // Real part of eq. (B2)
	double ImaginaryPartSubtractQuarkLO; // Imaginary part of eq. (B2)
	double RealPartSubtractQuarkNLOV; // Real part of eq. (B4)
	double ImaginaryPartSubtractQuarkNLOV; // Imaginary part of eq. (B4)
	double RealPartSubtractGluonNLOV; // Real part of eq. (B6)
	double ImaginaryPartSubtractGluonNLOV; // Imaginary part of eq. (B6)

	// LO, 1 / 1 - z

	RealPartSubtractQuarkLO = -LogInvZeta;

	ImaginaryPartSubtractQuarkLO = PI;

	// NLO, quark, vector, eq. (B4)

	RealPartSubtractQuarkNLOV = Pi2 / 2. - 3. * DiLogInvZeta
			+ LogInvZeta * (Pi2 + 9. + 3. * LogZeta - LogInvZeta2 / 3.);
	RealPartSubtractQuarkNLOV += m_logQ2OverMu2
			* (Pi2 - 3. * LogInvZeta - LogInvZeta2);
	RealPartSubtractQuarkNLOV *= m_CF / 2.;

	ImaginaryPartSubtractQuarkNLOV = Pi2 / 3. + 9. + 3. * LogZeta - LogInvZeta2
			- m_logQ2OverMu2 * (2. * LogInvZeta + 3);
	ImaginaryPartSubtractQuarkNLOV *= -PI * m_CF / 2.;

	// NLO, gluon, vector, eq. (B6)

	RealPartSubtractGluonNLOV = -1. + Pi2 / 3. * (1. - 3. / 4. * m_Zeta)
			+ DiLogInvZeta - LogZeta * LogInvZeta;
	RealPartSubtractGluonNLOV += (2. - m_Zeta) * LogInvZeta
			* (1. - LogInvZeta / 4.);
	RealPartSubtractGluonNLOV += m_logQ2OverMu2 / 2.
			* (1. - (2. - m_Zeta) * LogInvZeta);
	RealPartSubtractGluonNLOV *= 1 / (2. * m_xi);

	ImaginaryPartSubtractGluonNLOV = (2. - m_Zeta)
			* (2. - m_logQ2OverMu2 - LogInvZeta) - 2. * LogZeta;
	ImaginaryPartSubtractGluonNLOV *= -PI / (4. * m_xi);

	// Computation of subtraction terms in eq. (8) and (9)
	// Sums up previous contributions and takes care of LO / NLO and P(olarized) / U(npolarized) subtleties

	// LO, real and imaginary parts

	m_realPartSubtractQuark = RealPartSubtractQuarkLO;
	m_imaginaryPartSubtractQuark = ImaginaryPartSubtractQuarkLO;

	m_realPartSubtractGluon = 0.;
	m_imaginaryPartSubtractGluon = 0.;

	// NLO, real and imaginary parts

	if (m_qcdOrderType == QCDOrderType::NLO) {

		// Real part, quark contribution

		m_realPartSubtractQuark += m_alphaSOver2Pi * RealPartSubtractQuarkNLOV;

		// Real part, gluon contribution

		m_realPartSubtractGluon += m_alphaSOver2Pi * RealPartSubtractGluonNLOV;

		// Imaginary part, quark contribution

		m_imaginaryPartSubtractQuark += m_alphaSOver2Pi
				* ImaginaryPartSubtractQuarkNLOV;

		// Imaginary part, gluon contribution

		m_imaginaryPartSubtractGluon += m_alphaSOver2Pi
				* ImaginaryPartSubtractGluonNLOV;

	}
}

void DVCSCFFModel::computeSubtractionFunctionsA() {
	double LogZeta = log(m_Zeta);
	double LogInvZeta = log((1. - m_Zeta) / m_Zeta);
	double LogInvZeta2 = LogInvZeta * LogInvZeta;
	double DiLogInvZeta = TMath::DiLog(1. - 1. / m_Zeta);
	double Pi2 = PI * PI;

	double RealPartSubtractQuarkLO; // Real part of eq. (B2)
	double ImaginaryPartSubtractQuarkLO; // Imaginary part of eq. (B2)
	double RealPartSubtractQuarkNLOA; // Real part of eq. (B4)
	double ImaginaryPartSubtractQuarkNLOA; // Imaginary part of eq. (B4)
	double RealPartSubtractGluonNLOA; // Real part of eq. (B6)
	double ImaginaryPartSubtractGluonNLOA; // Imaginary part of eq. (B6)

	// LO, 1 / 1 - z

	RealPartSubtractQuarkLO = -LogInvZeta;

	ImaginaryPartSubtractQuarkLO = PI;

	// NLO, quark, axial, eq. (B3)

	RealPartSubtractQuarkNLOA = Pi2 / 6. - DiLogInvZeta
			+ LogInvZeta * (Pi2 + 9. + LogZeta - LogInvZeta2 / 3.);
	RealPartSubtractQuarkNLOA += m_logQ2OverMu2
			* (Pi2 - 3. * LogInvZeta - LogInvZeta2);
	RealPartSubtractQuarkNLOA *= m_CF / 2.;

	ImaginaryPartSubtractQuarkNLOA = Pi2 / 3. + 9. + LogZeta - LogInvZeta2
			- m_logQ2OverMu2 * (2. * LogInvZeta + 3);
	ImaginaryPartSubtractQuarkNLOA *= -PI * m_CF / 2.;

	// NLO, gluon, axial, eq. (B5)

	RealPartSubtractGluonNLOA = 1. + Pi2 / 4. * m_Zeta
			+ m_Zeta * LogInvZeta * (1. - LogInvZeta / 4.);
	RealPartSubtractGluonNLOA += -m_logQ2OverMu2 / 2.
			* (1. + m_Zeta * LogInvZeta);
	RealPartSubtractGluonNLOA *= 1 / (2. * m_xi);

	ImaginaryPartSubtractGluonNLOA = 2. - LogInvZeta - m_logQ2OverMu2;
	ImaginaryPartSubtractGluonNLOA *= -PI * m_Zeta / (4. * m_xi);

	// Computation of subtraction terms in eq. (8) and (9)
	// Sums up previous contributions and takes care of LO / NLO and P(olarized) / U(npolarized) subtleties

	// LO, real and imaginary parts

	m_realPartSubtractQuark = RealPartSubtractQuarkLO;
	m_imaginaryPartSubtractQuark = ImaginaryPartSubtractQuarkLO;

	m_realPartSubtractGluon = 0.;
	m_imaginaryPartSubtractGluon = 0.;

	// NLO, real and imaginary parts

	if (m_qcdOrderType == "NLO") {

		// Real part, quark contribution

		m_realPartSubtractQuark += m_alphaSOver2Pi * RealPartSubtractQuarkNLOA;

		// Real part, gluon contribution

		m_realPartSubtractGluon += m_alphaSOver2Pi * RealPartSubtractGluonNLOA;

		// Imaginary part, quark contribution

		m_imaginaryPartSubtractQuark += m_alphaSOver2Pi
				* ImaginaryPartSubtractQuarkNLOA;

		// Imaginary part, gluon contribution

		m_imaginaryPartSubtractGluon += m_alphaSOver2Pi
				* ImaginaryPartSubtractGluonNLOA;

	}
}

std::complex<double> DVCSCFFModel::computeIntegralsV() {
	double IntegralRealPartKernelQuark1; // Integral between 0 and fZeta in real part of amplitude
	double IntegralRealPartKernelQuark2; // Integral between fZeta and 1 in real part of amplitude
	double IntegralImaginaryPartKernelQuark; // Integral between 0 and fZeta in imaginary part of amplitude

	double IntegralRealPartKernelGluon1; // Integral between 0 and fZeta in real part of amplitude
	double IntegralRealPartKernelGluon2; // Integral between fZeta and 1 in real part of amplitude
	double IntegralImaginaryPartKernelGluon; // Integral between 0 and fZeta in imaginary part of amplitude

	double SumSqrCharges; // Sum of square of electric charges of active quark flavours

	// Sums up integrals and subtraction constants to get real and imaginary parts of CFF.

	if (m_qcdOrderType != QCDOrderType::LO
			&& m_qcdOrderType != QCDOrderType::NLO) {

		Formatter()
				<< "[DVCSCFFModule::computeIntegrals] Erroneous input perturbative QCD order can only be LO or NLO. Here Order = "
				/*<< qcdOrderType.toString()*/;
		throw std::runtime_error("");
	}

	// Compute sum of active quark electric charges squared

	switch (m_nbOfActiveFlavour) {

	case 3:
		SumSqrCharges = 2. / 3.;
		break;

	case 4:
		SumSqrCharges = 10. / 9.;
		break;

	case 5:
		SumSqrCharges = 11. / 9.;
		break;

	case 6:
		SumSqrCharges = 15. / 9.;
		break;

	default:
		throw std::runtime_error(
				Formatter()
						<< "[DVCSCFFModule::computeIntegrals] Erroneous input number of active quark flavours should be an integer between 3 and 6. Number of active quark flavours = "
						<< m_nbOfActiveFlavour);
	}

	// Define functions and compute integrals

	double* DummyPointer = NULL; // dummy variable for parameters in Integral method of TF1 (no parameters for us)
	ROOT::Math::Integrator Integrator(
			ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR, 0., 1.e-3);

	//  ROOT::Math::Integrator Integrator( ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR, 0., 1.e-9 );

	//  Integrator.SetAbsTolerance( 0. );
	//   Integrator.SetRelTolerance( 0.001 );
	//   Integrator.SetNPoints( 1 );

	// Quark sector

	TF1 ConvolRealPartKernelQuark1("ConvolReKernelQuark1V", this,
			&DVCSCFFModel::ConvolReKernelQuark1V, 0., 1., 0, "DVCSCFFModel",
			"ConvolReKernelQuark1V");
	ROOT::Math::WrappedTF1 WrappedConvolRealPartKernelQuark1(
			ConvolRealPartKernelQuark1);
	Integrator.SetFunction(WrappedConvolRealPartKernelQuark1);
	//  IntegralRealPartKernelQuark1 =  Integrator.Integral( 1.e-8, +fXi );
	IntegralRealPartKernelQuark1 = Integrator.Integral(0., +m_xi);

	TF1 ConvolRealPartKernelQuark2("ConvolReKernelQuark2V", this,
			&DVCSCFFModel::ConvolReKernelQuark2V, 0., 1., 0, "DVCSCFFModel",
			"ConvolReKernelQuark2V");
	ROOT::Math::WrappedTF1 WrappedConvolRealPartKernelQuark2(
			ConvolRealPartKernelQuark2);
	Integrator.SetFunction(WrappedConvolRealPartKernelQuark2);
	IntegralRealPartKernelQuark2 = Integrator.Integral(+m_xi, +1.);

	TF1 ConvolImaginaryPartKernelQuark("ConvolImKernelQuarkV", this,
			&DVCSCFFModel::ConvolImKernelQuarkV, 0., 1., 0, "DVCSCFFModel",
			"ConvolImKernelQuarkV");
	ROOT::Math::WrappedTF1 WrappedConvolImaginaryPartKernelQuark(
			ConvolImaginaryPartKernelQuark);
	Integrator.SetFunction(WrappedConvolImaginaryPartKernelQuark);
	IntegralImaginaryPartKernelQuark = Integrator.Integral(+m_xi, +1.);

	// Gluon sector

	IntegralRealPartKernelGluon1 = 0.;
	IntegralRealPartKernelGluon2 = 0.;
	IntegralImaginaryPartKernelGluon = 0.;

	if (m_qcdOrderType == QCDOrderType::NLO) {

		TF1 ConvolRealPartKernelGluon1("ConvolReKernelGluon1V", this,
				&DVCSCFFModel::ConvolReKernelGluon1V, 0., 1., 0, "DVCSCFFModel",
				"ConvolReKernelGluon1V");
		ROOT::Math::WrappedTF1 WrappedConvolRealPartKernelGluon1(
				ConvolRealPartKernelGluon1);
		Integrator.SetFunction(WrappedConvolRealPartKernelGluon1);
		//     IntegralRealPartKernelGluon1 =  Integrator.Integral( 1.e-8, +fXi );
		IntegralRealPartKernelGluon1 = Integrator.Integral(0., +m_xi);

		TF1 ConvolRealPartKernelGluon2("ConvolReKernelGluon2V", this,
				&DVCSCFFModel::ConvolReKernelGluon2V, 0., 1., 0, "DVCSCFFModel",
				"ConvolReKernelGluon2V");
		ROOT::Math::WrappedTF1 WrappedConvolRealPartKernelGluon2(
				ConvolRealPartKernelGluon2);
		Integrator.SetFunction(WrappedConvolRealPartKernelGluon2);
		IntegralRealPartKernelGluon2 = Integrator.Integral(+m_xi, +1.);

		TF1 ConvolImaginaryPartKernelGluon("ConvolImKernelGluonV", this,
				&DVCSCFFModel::ConvolImKernelGluonV, 0., 1., 0, "DVCSCFFModel",
				"ConvolImKernelGluonV");
		ROOT::Math::WrappedTF1 WrappedConvolImaginaryPartKernelGluon(
				ConvolImaginaryPartKernelGluon);
		Integrator.SetFunction(WrappedConvolImaginaryPartKernelGluon);
		IntegralImaginaryPartKernelGluon = Integrator.Integral(+m_xi, +1.);
	}

	// Compute Subtraction constants (different at LO or NLO)

	computeSubtractionFunctionsV();

	// Compute real and imaginary part of CFF according to eq. (8) and eq. (9)

	double fRealPartCFF = IntegralRealPartKernelQuark1
			+ IntegralRealPartKernelQuark2
			+ m_quarkDiagonal * m_realPartSubtractQuark;
	fRealPartCFF += SumSqrCharges
			* (IntegralRealPartKernelGluon1 + IntegralRealPartKernelGluon2
					+ m_gluonDiagonal * m_realPartSubtractGluon);
	// Multiplication by SumSqrCharges corrects in mistake in eq. (9)

	double fImaginaryPartCFF = IntegralImaginaryPartKernelQuark
			+ m_quarkDiagonal * m_imaginaryPartSubtractQuark;
	fImaginaryPartCFF += SumSqrCharges
			* (IntegralImaginaryPartKernelGluon
					+ m_gluonDiagonal * m_imaginaryPartSubtractGluon);
	// Multiplication by SumSqrCharges corrects in mistake in eq. (9)

	delete DummyPointer;

	return std::complex<double>(fRealPartCFF, fImaginaryPartCFF);
}

//TODO implement pour KernelA
std::complex<double> DVCSCFFModel::computeIntegralsA() {
	double IntegralRealPartKernelQuark1; // Integral between 0 and fZeta in real part of amplitude
	double IntegralRealPartKernelQuark2; // Integral between fZeta and 1 in real part of amplitude
	double IntegralImaginaryPartKernelQuark; // Integral between 0 and fZeta in imaginary part of amplitude

	double IntegralRealPartKernelGluon1; // Integral between 0 and fZeta in real part of amplitude
	double IntegralRealPartKernelGluon2; // Integral between fZeta and 1 in real part of amplitude
	double IntegralImaginaryPartKernelGluon; // Integral between 0 and fZeta in imaginary part of amplitude

	double SumSqrCharges; // Sum of square of electric charges of active quark flavours

	// Sums up integrals and subtraction constants to get real and imaginary parts of CFF.

	if (m_qcdOrderType != QCDOrderType::LO
			&& m_qcdOrderType != QCDOrderType::NLO) {

		throw std::runtime_error(
				Formatter()
						<< "[DVCSCFFModule::computeIntegralsA] Erroneous input perturbative QCD order can only be LO or NLO. Here Order = "
						<< QCDOrderType(m_qcdOrderType).toString());

	}

	// Compute sum of active quark electric charges squared

	switch (m_nbOfActiveFlavour) {

	case 3:
		SumSqrCharges = 2. / 3.;
		break;

	case 4:
		SumSqrCharges = 10. / 9.;
		break;

	case 5:
		SumSqrCharges = 11. / 9.;
		break;

	case 6:
		SumSqrCharges = 15. / 9.;
		break;

	default:
		throw std::runtime_error(
				Formatter()
						<< "[DVCSCFFModule::computeIntegralsA] Erroneous input number of active quark flavours should be an integer between 3 and 6. Number of active quark flavours = "
						<< m_nbOfActiveFlavour);
	}

	// Define functions and compute integrals

	double* DummyPointer = NULL; // dummy variable for parameters in Integral method of TF1 (no parameters for us)
	ROOT::Math::Integrator Integrator(
			ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR, 0., 1.e-3);

	//  ROOT::Math::Integrator Integrator( ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR, 0., 1.e-9 );

	//  Integrator.SetAbsTolerance( 0. );
	//   Integrator.SetRelTolerance( 0.001 );
	//   Integrator.SetNPoints( 1 );

	// Quark sector

	TF1 ConvolRealPartKernelQuark1("ConvolReKernelQuark1A", this,
			&DVCSCFFModel::ConvolReKernelQuark1A, 0., 1., 0, "DVCSCFFModel",
			"ConvolReKernelQuark1A");
	ROOT::Math::WrappedTF1 WrappedConvolRealPartKernelQuark1(
			ConvolRealPartKernelQuark1);
	Integrator.SetFunction(WrappedConvolRealPartKernelQuark1);
	//	IntegralRealPartKernelQuark1 =  Integrator.Integral( 1.e-8, +fXi );
	IntegralRealPartKernelQuark1 = Integrator.Integral(0., +m_xi);

	TF1 ConvolRealPartKernelQuark2("ConvolReKernelQuark2A", this,
			&DVCSCFFModel::ConvolReKernelQuark2A, 0., 1., 0, "DVCSCFFModel",
			"ConvolReKernelQuark2A");
	ROOT::Math::WrappedTF1 WrappedConvolRealPartKernelQuark2(
			ConvolRealPartKernelQuark2);
	Integrator.SetFunction(WrappedConvolRealPartKernelQuark2);
	IntegralRealPartKernelQuark2 = Integrator.Integral(+m_xi, +1.);

	TF1 ConvolImaginaryPartKernelQuark("ConvolImKernelQuarkA", this,
			&DVCSCFFModel::ConvolImKernelQuarkA, 0., 1., 0, "DVCSCFFModel",
			"ConvolImKernelQuarkA");
	ROOT::Math::WrappedTF1 WrappedConvolImaginaryPartKernelQuark(
			ConvolImaginaryPartKernelQuark);
	Integrator.SetFunction(WrappedConvolImaginaryPartKernelQuark);
	IntegralImaginaryPartKernelQuark = Integrator.Integral(+m_xi, +1.);

	// Gluon sector

	IntegralRealPartKernelGluon1 = 0.;
	IntegralRealPartKernelGluon2 = 0.;
	IntegralImaginaryPartKernelGluon = 0.;

	if (m_qcdOrderType == QCDOrderType::NLO) {

		TF1 ConvolRealPartKernelGluon1("ConvolReKernelGluon1A", this,
				&DVCSCFFModel::ConvolReKernelGluon1A, 0., 1., 0, "DVCSCFFModel",
				"ConvolReKernelGluon1A");
		ROOT::Math::WrappedTF1 WrappedConvolRealPartKernelGluon1(
				ConvolRealPartKernelGluon1);
		Integrator.SetFunction(WrappedConvolRealPartKernelGluon1);
		//	   IntegralRealPartKernelGluon1 =  Integrator.Integral( 1.e-8, +fXi );
		IntegralRealPartKernelGluon1 = Integrator.Integral(0., +m_xi);

		TF1 ConvolRealPartKernelGluon2("ConvolReKernelGluon2A", this,
				&DVCSCFFModel::ConvolReKernelGluon2A, 0., 1., 0, "DVCSCFFModel",
				"ConvolReKernelGluon2A");
		ROOT::Math::WrappedTF1 WrappedConvolRealPartKernelGluon2(
				ConvolRealPartKernelGluon2);
		Integrator.SetFunction(WrappedConvolRealPartKernelGluon2);
		IntegralRealPartKernelGluon2 = Integrator.Integral(+m_xi, +1.);

		TF1 ConvolImaginaryPartKernelGluon("ConvolImKernelGluonA", this,
				&DVCSCFFModel::ConvolImKernelGluonA, 0., 1., 0, "DVCSCFFModel",
				"ConvolImKernelGluonA");
		ROOT::Math::WrappedTF1 WrappedConvolImaginaryPartKernelGluon(
				ConvolImaginaryPartKernelGluon);
		Integrator.SetFunction(WrappedConvolImaginaryPartKernelGluon);
		IntegralImaginaryPartKernelGluon = Integrator.Integral(+m_xi, +1.);
	}

	// Compute Subtraction constants (different at LO or NLO)

	computeSubtractionFunctionsA();

	// Compute real and imaginary part of CFF according to eq. (8) and eq. (9)

	double realPartCFF = IntegralRealPartKernelQuark1
			+ IntegralRealPartKernelQuark2
			+ m_quarkDiagonal * m_realPartSubtractQuark;
	//	cout << fpQCDOrder << "      RealPartCFF Quark = " << fRealPartCFF << endl ;
	realPartCFF += SumSqrCharges
			* (IntegralRealPartKernelGluon1 + IntegralRealPartKernelGluon2
					+ m_gluonDiagonal * m_realPartSubtractGluon);
	//	cout << fpQCDOrder << "      RealPartCFF Gluon = " << SumSqrCharges * ( IntegralRealPartKernelGluon1 + IntegralRealPartKernelGluon2 + fGluonDiagonal * fRealPartSubtractGluon ) << endl ;
	// Multiplication by SumSqrCharges corrects in mistake in eq. (9)

	double imaginaryPartCFF = IntegralImaginaryPartKernelQuark
			+ m_quarkDiagonal * m_imaginaryPartSubtractQuark;
	//	cout << fpQCDOrder << "      ImaginaryPartCFF Quark = " << fImaginaryPartCFF << endl ;
	imaginaryPartCFF += SumSqrCharges
			* (IntegralImaginaryPartKernelGluon
					+ m_gluonDiagonal * m_imaginaryPartSubtractGluon);
	//	cout << fpQCDOrder << "      ImaginaryPartCFF Gluon = " << SumSqrCharges * ( IntegralImaginaryPartKernelGluon + fGluonDiagonal * fImaginaryPartSubtractGluon ) << endl ;
	// Multiplication by SumSqrCharges corrects in mistake in eq. (9)

	delete DummyPointer;

	return std::complex<double>(realPartCFF, imaginaryPartCFF);
}

std::complex<double> DVCSCFFModel::KernelQuarkNLOV(double x) {
	double z = x / m_xi;

	std::complex<double> LogOneMinusz(0., 0.);
	if (x < m_xi) {
// remplacer z par x/xi
		LogOneMinusz = std::complex<double>(0., 0.);
		LogOneMinusz = std::complex<double>(TMath::Log((1. - z) / 2.), 0.);
	}
	if (x > m_xi) {
		LogOneMinusz = std::complex<double>(TMath::Log((z - 1.) / 2.), -PI);
	}

	return KernelQuarkNLOA(x) - (m_CF / (1. + z)) * LogOneMinusz;
}

/*!
 * \fn KernelQuark( double x )
 *
 * T^{q, V/A} in appendix A, eq. (A1).
 *
 */
std::complex<double> DVCSCFFModel::KernelQuarkV(double x) {
	//std::complex<double> z = std::complex<double>(x / m_xi, 0.);
	//std::complex<double> quark();

	double z = x / m_xi;

//    TComplex z = TComplex(x / fXi, 0.);
//    TComplex Quark;

	std::complex<double> quark(1. / (1. - z), 0.);

	if (m_qcdOrderType == QCDOrderType::NLO) {

		quark += m_alphaSOver2Pi * KernelQuarkNLOV(x);

		if (m_qcdOrderType != QCDOrderType::LO
				&& m_qcdOrderType != QCDOrderType::NLO) {

			throw std::runtime_error(
					Formatter()
							<< "[DVCSCFFModule::KernelQuarkV] Erroneous input, perturbative QCD order can only be LO or NLO. Here Order = "
							<< QCDOrderType(m_qcdOrderType).toString());
			//TODO throw an exception
		}
	}

	return quark;
}

std::complex<double> DVCSCFFModel::KernelQuarkA(double x) {
	//std::complex<double> z = std::complex<double>(x / m_xi, 0.);
	//std::complex<double> quark();

	double z = x / m_xi;

//    TComplex z = TComplex(x / fXi, 0.);
//    TComplex Quark;

	std::complex<double> quark(1. / (1. - z), 0.);

	if (m_qcdOrderType == QCDOrderType::NLO) {

		quark += m_alphaSOver2Pi * KernelQuarkNLOA(x);

		if (m_qcdOrderType != QCDOrderType::LO
				&& m_qcdOrderType != QCDOrderType::NLO) {

			throw std::runtime_error(
					Formatter()
							<< "[DVCSCFFModule::KernelQuarkA] Erroneous input, perturbative QCD order can only be LO or NLO. Here Order = "
							<< QCDOrderType(m_qcdOrderType).toString());
			//TODO throw an exception
		}
	}

	return quark;
}

/*!
 * \fn KernelGluon( double x )
 *
 * T^{g, V/A} in appendix A, eq. (A1).
 *
 */
std::complex<double> DVCSCFFModel::KernelGluonV(double x) {

	std::complex<double> gluon(0., 0.);

	if (m_qcdOrderType == QCDOrderType::NLO) {

		gluon += m_alphaSOver2Pi * KernelGluonNLOV(x);

		if (m_qcdOrderType != QCDOrderType::LO
				&& m_qcdOrderType != QCDOrderType::NLO) {
			Formatter()
					<< "[DVCSCFFModule::KernelGluonV] Erroneous input, perturbative QCD order can only be LO or NLO. Here Order = "
					<< QCDOrderType(m_qcdOrderType).toString();
			//TODO throw an exception
		}
	}

	return gluon;
}

std::complex<double> DVCSCFFModel::KernelGluonA(double x) {

	std::complex<double> gluon(0., 0.);

	if (m_qcdOrderType == QCDOrderType::NLO) {

		gluon += m_alphaSOver2Pi * KernelGluonNLOA(x);

		if (m_qcdOrderType != QCDOrderType::LO
				&& m_qcdOrderType != QCDOrderType::NLO) {
			Formatter()
					<< "[DVCSCFFModule::KernelGluonA] Erroneous input, perturbative QCD order can only be LO or NLO. Here Order = "
					<< QCDOrderType(m_qcdOrderType).toString();
			//TODO throw an exception
		}
	}

	return gluon;
}

/*
 * \fn double ConvolReKernelQuark1( double* x, double* Parameters );
 *
 * eq. (8), real part of amplitude, terms integrated for X between 0 and fZeta.
 * Equivalently x integration domain ranges between -fXi and +fXi.
 * Warning : grid is defined for x > 0.
 *
 * Expressions are modified in order to integrate between 0 and fXi, hence explicitely avoiding GPD behaviour at x = 0.
 *
 */
double DVCSCFFModel::ConvolReKernelQuark1V(double* x, double* Parameters) {

	GPDOutputData gpdOutputData = m_pGPDModule->compute(x[0], m_xi, m_t, m_MuF,
			m_MuR, m_currentGPDComputeType);

	// GPD evaluated at x = x[ 0 ]
	double EvalGPD = computeSquareChargeAveragedGPD(gpdOutputData);

	// Integrated function
	double Convol = (EvalGPD - m_quarkDiagonal) * KernelQuarkV(x[0]).real();
	Convol += (-EvalGPD - m_quarkDiagonal) * KernelQuarkV(-x[0]).real();

	Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

	return Convol;
}

/*
 * \fn double ConvolReKernelQuark2( double* x, double* Parameters );
 *
 * eq. (8), real part of amplitude, terms integrated between fZeta and 1.
 * Equivalently x integration domain ranges between fXi and 1.
 *
 */
double DVCSCFFModel::ConvolReKernelQuark2V(double* x, double* Parameters) {
	GPDOutputData gpdOutputData = m_pGPDModule->compute(x[0], m_xi, m_t, m_MuF,
			m_MuR, m_currentGPDComputeType);

	// GPD evaluated at x = x[ 0 ]
	double EvalGPD = computeSquareChargeAveragedGPD(gpdOutputData);

	double Convol = EvalGPD - m_quarkDiagonal;
	Convol *= KernelQuarkV(x[0]).real();

	Convol += -KernelQuarkV(-x[0]).real() * EvalGPD;

	Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

	return Convol;
}

/*
 * \fn double ConvolImKernelQuark( double* x, double* Parameters );
 *
 * eq. (8), imaginary part of amplitude, terms integrated between fZeta and 1.
 * Equivalently x integration domain ranges between fXi and 1.
 *
 */
double DVCSCFFModel::ConvolImKernelQuarkV(double* x, double* Parameters) {
	GPDOutputData gpdOutputData = m_pGPDModule->compute(x[0], m_xi, m_t, m_MuF,
			m_MuR, m_currentGPDComputeType);

	// GPD evaluated at x = x[ 0 ]
	double EvalGPD = computeSquareChargeAveragedGPD(gpdOutputData);

	double Convol = EvalGPD - m_quarkDiagonal;
	Convol *= KernelQuarkV(x[0]).imag();
	Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

	return Convol;
}

/*
 * \fn double ConvolReKernelGluon1( double* x, double* Parameters );
 *
 * eq. (9), real part of amplitude, terms integrated between 0 and fZeta.
 * Equivalently x integration domain ranges between -fXi and +fXi.
 * Warning : grid is defined for x > 0.
 *
 * Expressions are modified in order to integrate between 0 and fXi, hence explicitely avoiding GPD behaviour at x = 0.
 *
 */
double DVCSCFFModel::ConvolReKernelGluon1V(double* x, double* Parameters) {
	GPDOutputData gpdOutputData = m_pGPDModule->compute(x[0], m_xi, m_t, m_MuF,
			m_MuR, m_currentGPDComputeType);

	// GPD evaluated at x = x[ 0 ]
	double EvalGPD =
			2.
					* (gpdOutputData.getGPDResultData(m_currentGPDComputeType))->getGluon();

	double Convol = (EvalGPD - m_gluonDiagonal) * KernelGluonV(x[0]).real();
	Convol += (+EvalGPD - m_gluonDiagonal) * KernelGluonV(-x[0]).real();

	Convol /= (m_xi * m_xi * m_nbOfActiveFlavour);

	return Convol;
}

/*
 * \fn double ConvolReKernelGluon2( double* x, double* Parameters );
 *
 * eq. (8), real part of amplitude, terms integrated between fZeta and 1.
 * Equivalently x integration domain ranges between fXi and 1.
 *
 */
double DVCSCFFModel::ConvolReKernelGluon2V(double* x, double* Parameters) {
	GPDOutputData gpdOutputData = m_pGPDModule->compute(x[0], m_xi, m_t, m_MuF,
			m_MuR, m_currentGPDComputeType);

	double EvalGPD =
			2.
					* (gpdOutputData.getGPDResultData(m_currentGPDComputeType))->getGluon();

	double Convol = EvalGPD - m_gluonDiagonal;
	Convol *= KernelGluonV(x[0]).real();

	Convol += +KernelGluonV(-x[0]).real() * EvalGPD;

	Convol /= (m_xi * m_xi * m_nbOfActiveFlavour); // In eq. (8), ( ( 2 - fZeta ) / fZeta )^2 = 1 / fXi^2

	return Convol;
}

/*
 * \fn double ConvolImKernelGluon( double* x, double* Parameters );
 *
 * eq. (9), imaginary part of amplitude, terms integrated between fZeta and 1.
 * Equivalently x integration domain ranges between fXi and 1.
 *
 */
double DVCSCFFModel::ConvolImKernelGluonV(double* x, double* Parameters) {
	GPDOutputData gpdOutputData = m_pGPDModule->compute(x[0], m_xi, m_t, m_MuF,
			m_MuR, m_currentGPDComputeType);

	double EvalGPD =
			2.
					* (gpdOutputData.getGPDResultData(m_currentGPDComputeType))->getGluon();

	double Convol = EvalGPD - m_gluonDiagonal;
	Convol *= KernelGluonV(x[0]).imag();
	Convol /= (m_xi * m_xi * m_nbOfActiveFlavour); // In eq. (8), ( ( 2 - fZeta ) / fZeta )^2 = 1 / fXi^2

	return Convol;
}

std::complex<double> DVCSCFFModel::KernelGluonNLOV(double x) {

	double z = x / m_xi;

	std::complex<double> LogOneMinusz(0., 0.);
	if (x < m_xi) {
		// remplacer z par x/xi
		LogOneMinusz = std::complex<double>(TMath::Log((1. - z) / 2.), 0.);
	}
	if (x > m_xi) {
		LogOneMinusz = std::complex<double>(TMath::Log((z - 1.) / 2.), -PI);
	}

	std::complex<double> GluonNLOV(LogOneMinusz);
	GluonNLOV += (m_logQ2OverMu2 - 2.);
	GluonNLOV /= 1. - z;
	GluonNLOV += LogOneMinusz / (1. + z);
	GluonNLOV *= (m_nbOfActiveFlavour / 2.);
	GluonNLOV += -KernelGluonNLOA(x);

	return GluonNLOV;
}

std::complex<double> DVCSCFFModel::KernelGluonNLOA(double x) {
	double z = x / m_xi;
	std::complex<double> LogOneMinusz(0., 0.);
	if (x < m_xi) {
		// remplacer z par x/xi
		LogOneMinusz = std::complex<double>(TMath::Log((1. - z) / 2.), 0.);
	}
	if (x > m_xi) {
		LogOneMinusz = std::complex<double>(TMath::Log((z - 1.) / 2.), -PI);
	}

	std::complex<double> GluonNLOA(LogOneMinusz);
	GluonNLOA += (m_logQ2OverMu2 - 2.);
	GluonNLOA *= 1 / (1 - z * z);
	GluonNLOA *= (1. / (1. - (z * z)) + LogOneMinusz / ((1. + z) * (1. + z)));
	GluonNLOA += -LogOneMinusz * LogOneMinusz / (2. * (1. + z) * (1. + z));
	GluonNLOA *= (m_nbOfActiveFlavour / 2.);

	return GluonNLOA;
}

std::complex<double> DVCSCFFModel::KernelQuarkNLOA(double x) {

	double z = x / m_xi;
	std::complex<double> LogOneMinusz(0., 0.);
	if (x < m_xi) {
		// remplacer z par x/xi
		LogOneMinusz = std::complex<double>(TMath::Log((1. - z) / 2.), 0.);
	}
	if (x > m_xi) {
		LogOneMinusz = std::complex<double>(TMath::Log((z - 1.) / 2.), -PI);
	}

	std::complex<double> QuarkNLOA(m_logQ2OverMu2);
	QuarkNLOA += LogOneMinusz / 2. - (3. / 4.);
	QuarkNLOA *= 2. * LogOneMinusz + 3.;
	QuarkNLOA += -(27. / 4.) - (1. - z) / (1. + z) * LogOneMinusz;
	QuarkNLOA *= m_CF / 2. * (1. - z);

	return QuarkNLOA;
}

double DVCSCFFModel::ConvolReKernelQuark1A(double* x, double* Parameters) {
	GPDOutputData gpdOutputData = m_pGPDModule->compute(x[0], m_xi, m_t, m_MuF,
			m_MuR, m_currentGPDComputeType);

	// GPD evaluated at x = x[ 0 ]
	double EvalGPD = computeSquareChargeAveragedGPD(gpdOutputData);

	double Convol = (EvalGPD - m_quarkDiagonal) * KernelQuarkA(x[0]).real();
	Convol += (+EvalGPD - m_quarkDiagonal) * KernelQuarkA(-x[0]).real();

	Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

	return Convol;
}

double DVCSCFFModel::ConvolReKernelQuark2A(double* x, double* Parameters) {
	GPDOutputData gpdOutputData = m_pGPDModule->compute(x[0], m_xi, m_t, m_MuF,
			m_MuR, m_currentGPDComputeType);

	// GPD evaluated at x = x[ 0 ]
	double EvalGPD = computeSquareChargeAveragedGPD(gpdOutputData);

	double Convol = EvalGPD - m_quarkDiagonal;
	Convol *= KernelQuarkA(x[0]).real();

	Convol += +KernelQuarkA(-x[0]).real() * EvalGPD;

	Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

	return Convol;
}

double DVCSCFFModel::ConvolImKernelQuarkA(double* x, double* Parameters) {
	GPDOutputData gpdOutputData = m_pGPDModule->compute(x[0], m_xi, m_t, m_MuF,
			m_MuR, m_currentGPDComputeType);

	// GPD evaluated at x = x[ 0 ]
	double EvalGPD = computeSquareChargeAveragedGPD(gpdOutputData);

	double Convol = EvalGPD - m_quarkDiagonal;
	Convol *= KernelQuarkA(x[0]).imag();
	Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

	return Convol;
}

double DVCSCFFModel::ConvolReKernelGluon1A(double* x, double* Parameters) {
	GPDOutputData gpdOutputData = m_pGPDModule->compute(x[0], m_xi, m_t, m_MuF,
			m_MuR, m_currentGPDComputeType);

	double EvalGPD =
			2.
					* (gpdOutputData.getGPDResultData(m_currentGPDComputeType))->getGluon();

	double Convol = (EvalGPD - m_gluonDiagonal) * KernelGluonA(x[0]).real();
	Convol += (-EvalGPD - m_gluonDiagonal) * KernelGluonA(-x[0]).real();

	Convol /= (m_xi * m_xi * m_nbOfActiveFlavour);

	return Convol;
}

double DVCSCFFModel::ConvolReKernelGluon2A(double* x, double* Parameters) {
	GPDOutputData gpdOutputData = m_pGPDModule->compute(x[0], m_xi, m_t, m_MuF,
			m_MuR, m_currentGPDComputeType);

	double EvalGPD =
			2.
					* (gpdOutputData.getGPDResultData(m_currentGPDComputeType))->getGluon();

	double Convol = EvalGPD - m_gluonDiagonal;
	Convol *= KernelGluonA(x[0]).real();

	Convol += -KernelGluonA(-x[0]).real() * EvalGPD;

	Convol /= (m_xi * m_xi * m_nbOfActiveFlavour); // In eq. (8), ( ( 2 - fZeta ) / fZeta )^2 = 1 / fXi^2

	return Convol;
}

double DVCSCFFModel::ConvolImKernelGluonA(double* x, double* Parameters) {
	GPDOutputData gpdOutputData = m_pGPDModule->compute(x[0], m_xi, m_t, m_MuF,
			m_MuR, m_currentGPDComputeType);

	double EvalGPD =
			2.
					* (gpdOutputData.getGPDResultData(m_currentGPDComputeType))->getGluon();

	double Convol = EvalGPD - m_gluonDiagonal;
	Convol *= KernelGluonA(x[0]).imag();
	Convol /= (m_xi * m_xi * m_nbOfActiveFlavour); // In eq. (8), ( ( 2 - fZeta ) / fZeta )^2 = 1 / fXi^2

	return Convol;
}
