#include "EvolQCDModule.h"

#include <stdexcept>
#include <math.h>

#include "GPDModule.h"
#include "../utils/stringUtils/Formatter.h"

MatrixD EvolQCDModule::conversionMatrix1(3, 3, 1., 0., 0., 0., 0., 0., 0., 0.,
		1.);

MatrixD EvolQCDModule::conversionMatrix2(5, 5, 1., 0., 0., 0., 0., 0., 0., 1.,
		0., 0., 0., 1., 0., -1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1.);

MatrixD EvolQCDModule::conversionMatrix3(7, 7, 1., 0., 0., 0., 0., 0., 0., 0.,
		0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 1., 0., 0., 0., 1., 0., -1., 0.,
		0., 0., 0., 1., 0., 1., 0., -2., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
		0., 0., 0., 0., 1.);

MatrixD EvolQCDModule::conversionMatrix4(9, 9, 1., 0., 0., 0., 0., 0., 0., 0.,
		0., 0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1., 0., 0., 0.,
		0., 0., 0., 0., 0., 0., 0., 1., 0., 0., 0., 1., 0., -1., 0., 0., 0., 0.,
		0., 0., 1., 0., 1., 0., -2., 0., 0., 0., 0., 1., 0., 1., 0., 1., 0.,
		-3., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
		0., 1.);

MatrixD EvolQCDModule::conversionMatrix5(11, 11, 1., 0., 0., 0., 0., 0., 0., 0.,
		0., 0., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
		1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1., 0., 0., 0., 0.,
		0., 0., 0., 0., 0., 0., 0., 0., 1., 0., 0., 0., 1., 0., -1., 0., 0., 0.,
		0., 0., 0., 0., 0., 1., 0., 1., 0., -2., 0., 0., 0., 0., 0., 0., 1., 0.,
		1., 0., 1., 0., -3., 0., 0., 0., 0., 1., 0., 1., 0., 1., 0., 1., 0.,
		-4., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
		0., 0., 0., 0., 0., 1.);

MatrixD EvolQCDModule::conversionMatrix6(13, 13, 1., 0., 0., 0., 0., 0., 0., 0.,
		0., 0., 0., 0., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
		0., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
		0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1., 0.,
		0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1.,
		0., -1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1., 0., 1., 0., -2.,
		0., 0., 0., 0., 0., 0., 0., 0., 1., 0., 1., 0., 1., 0., -3., 0., 0., 0.,
		0., 0., 0., 1., 0., 1., 0., 1., 0., 1., 0., -4., 0., 0., 0., 0., 1., 0.,
		1., 0., 1., 0., 1., 0., 1., 0., -5., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
		0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1.);

EvolQCDModule::EvolQCDModule(const std::string &moduleID) :
		ModuleObject(moduleID), m_nfEvol(-1), m_nfMin(-1) {
}

EvolQCDModule::EvolQCDModule(const EvolQCDModule &other) :
		ModuleObject(other) {
	m_x = other.m_x;
	m_xi = other.m_xi;
	m_t = other.m_t;
	m_MuF = other.m_MuF;
	m_MuR = other.m_MuR;

	m_nfMin = other.m_nfMin;
	m_nfEvol = other.m_nfEvol;
	m_epsilon = other.m_epsilon;
	m_alpha = other.m_alpha;

	m_qcdOrderType = other.m_qcdOrderType;
	//m_runningAlphaS = other.m_runningAlphaS;
	m_alphaS = other.m_alphaS;
	m_scaleDistinction = other.m_scaleDistinction;

	m_singletQuark = other.m_singletQuark;
	m_gluon = other.m_gluon;

	//TODO comment cloner ça
	if (m_pGPDModule != 0) {

	}
}

EvolQCDModule::~EvolQCDModule() {
}

//TODO ajouter les tests manquants
void EvolQCDModule::isModuleWellConfigured() {
	if (m_pGPDModule == 0) {
		throw std::runtime_error("[EvolQCDModule] GPDModule* is NULL");
	}
	if (m_qcdOrderType == QCDOrderType::UNDEFINED) {
		throw std::runtime_error("[EvolQCDModule] QCDOrderType is UNDEFINED");
	}
}

void EvolQCDModule::initModule() {
//do nothing; the definition is just mandatory for enable the use of the clone function

//TODO MAJ des matrices de changement de base en fonction du Nf_EVOL choisi par l'utilisateur
	initNfMin();
	initMatrixValue();
	initVectorOfGPDCombinations();
}

void EvolQCDModule::initNfMin() {

	size_t nfGPDModel = m_gpdResultData.sizeOfListOfQuarkFlavorData();

	// si nf_evol = -1 alors nf_evol = nf_gpd
	if (m_nfEvol == -1) {
		m_nfEvol = nfGPDModel;
		// si nf_evol < nf_gpd alors nf_min = nf_evol
	} else if (m_nfEvol < nfGPDModel) {
		m_nfMin = m_nfEvol;
		// si nf_evol >= nf_gpd alors nf_min = nf_gpd
	} else {
		m_nfMin = nfGPDModel;
	}

	// test contrainte sur les bornes du nf_evol
	if (m_nfEvol < 1 || m_nfEvol > 6) {
		throw std::runtime_error(
				Formatter()
						<< "[EvolQCDModule] nfEvol is out of range ; a good value is between [1 : 6]");
	}
}

void EvolQCDModule::initMatrixValue() {
	double missingCoef = 1. / (2. * m_nfEvol);

	switch (m_nfMin) {
	case 1:
		m_currentConvertMatrix = conversionMatrix1;
		m_currentConvertMatrix.update(1, 1, missingCoef);
		break;
	case 2:
		m_currentConvertMatrix = conversionMatrix2;
		m_currentConvertMatrix.update(1, 3, missingCoef);
		m_currentConvertMatrix.update(3, 3, missingCoef);
		break;
	case 3:
		m_currentConvertMatrix = conversionMatrix3;
		m_currentConvertMatrix.update(1, 5, missingCoef);
		m_currentConvertMatrix.update(3, 5, missingCoef);
		m_currentConvertMatrix.update(5, 5, missingCoef);
		break;
	case 4:
		m_currentConvertMatrix = conversionMatrix4;
		m_currentConvertMatrix.update(1, 7, missingCoef);
		m_currentConvertMatrix.update(3, 7, missingCoef);
		m_currentConvertMatrix.update(5, 7, missingCoef);
		m_currentConvertMatrix.update(7, 7, missingCoef);
		break;
	case 5:
		m_currentConvertMatrix = conversionMatrix5;
		m_currentConvertMatrix.update(1, 9, missingCoef);
		m_currentConvertMatrix.update(3, 9, missingCoef);
		m_currentConvertMatrix.update(5, 9, missingCoef);
		m_currentConvertMatrix.update(7, 9, missingCoef);
		m_currentConvertMatrix.update(9, 9, missingCoef);
		break;
	case 6:
		m_currentConvertMatrix = conversionMatrix6;
		m_currentConvertMatrix.update(1, 11, missingCoef);
		m_currentConvertMatrix.update(3, 11, missingCoef);
		m_currentConvertMatrix.update(5, 11, missingCoef);
		m_currentConvertMatrix.update(7, 11, missingCoef);
		m_currentConvertMatrix.update(9, 11, missingCoef);
		m_currentConvertMatrix.update(11, 11, missingCoef);
		break;
	default:
		break;
	}
}

void EvolQCDModule::initVectorOfGPDCombinations() {
	std::vector<GPDQuarkFlavorData> listOfQuarkFlavorData =
			m_gpdResultData.getListOfQuarkFlavorData();

	//TODO creation en fonction de nfMin pour tronquer ce qui doit l'être

	// q+ et q- (HNonSinglet) pour chaque saveur de quark, HSinglet et Hg
	m_vectorOfGPDCombination.resize(listOfQuarkFlavorData.size() * 2 + 1);

	for (unsigned int i = 0; i != m_nfMin; i++) {
		m_vectorOfGPDCombination.push_back(
				listOfQuarkFlavorData[i].getPartonDistributionMinus());
		m_vectorOfGPDCombination.push_back(
				listOfQuarkFlavorData[i].getPartonDistributionPlus());
	}

	m_vectorOfGPDCombination.push_back(m_gpdResultData.getGluon());
}

//TODO implement
bool EvolQCDModule::isRunnable(const double &MuF, const double &MuF_ref,
		EvolQCDModule::Type testType) {
	//TODO MuF de l'utilisateur par rapport au MuF_Ref du modèle de GPD
	bool result = false;
	switch (testType) {
	case EvolQCDModule::RELATIVE:
		result = isRelativeTest(MuF, MuF_ref);
		break;
	case EvolQCDModule::ABSOLUTE:
		result = isAbsoluteTest(MuF, MuF_ref);
		break;
	case EvolQCDModule::BOTH:
		result = (isRelativeTest(MuF, MuF_ref) && isAbsoluteTest(MuF, MuF_ref));
		break;
	default:
		break;
	}

	return result;
}

bool EvolQCDModule::isRelativeTest(const double &MuF, const double &MuF_ref) {
	return (fabs(MuF - MuF_ref) < (m_epsilon * MuF_ref)) ? true : false;
}

bool EvolQCDModule::isAbsoluteTest(const double &MuF, const double &MuF_ref) {
	return (fabs(MuF - MuF_ref) < m_alpha) ? true : false;
}

void EvolQCDModule::preCompute(const double &x, const double &xi,
		const double &t, const double &MuF, const double &MuR,
		const GPDResultData &gpdResultData) {
	m_x = x;
	m_xi = xi;
	m_t = t;
	m_MuF = MuF;
	m_MuR = MuR;
	m_gpdResultData = gpdResultData;

	initModule();
	isModuleWellConfigured();
}

//TODO faire valider le calcul par Hervé
//TODO ajouter les commentaires qui vont bien et les références au papier
void EvolQCDModule::convertBasis() {
	std::vector<double> tempVector = m_currentConvertMatrix
			* m_vectorOfGPDCombination;
	m_vectorOfGPDCombination = tempVector;
}

void EvolQCDModule::invertBasis() {
	std::vector<double> tempVector = m_currentInvertMatrix
			* m_vectorOfGPDCombination;
	m_vectorOfGPDCombination = tempVector;
}

GPDResultData EvolQCDModule::makeGPDResultData() {

	//TODO vérifier comment construire par copie
	GPDResultData gpdResultData(m_gpdResultData);

	gpdResultData.setGluon(
			m_vectorOfGPDCombination[m_vectorOfGPDCombination.size() - 1]);



	return GPDResultData();
}
