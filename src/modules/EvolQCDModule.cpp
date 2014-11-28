#include "EvolQCDModule.h"

#include <math.h>
#include <stddef.h>
//#include <iostream>
#include <stdexcept>

#include "../beans/gpd/GPDQuarkFlavorData.h"
#include "../beans/QuarkFlavor.h"
#include "../utils/logger/LoggerManager.h"
#include "../utils/stringUtils/Formatter.h"
#include "GPDModule.h"

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

MatrixD EvolQCDModule::invertMatrix1(3, 3, 1., 0., 0., 0., 0., 0., 0., 0., 1.);

MatrixD EvolQCDModule::invertMatrix2(5, 5, 1., 0., 0., 0., 0., 0., 0., 1. / 2.,
        0., 0., 0., 1., 0., 0., 0., 0., 0., -1. / 2., 0., 0., 0., 0., 0., 0.,
        1.);

MatrixD EvolQCDModule::invertMatrix3(7, 7, 1., 0., 0., 0., 0., 0., 0., 0., 0.,
        0., 1. / 2., 1. / 6., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0.,
        -1. / 2., 1. / 6., 0., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0.,
        -1. / 3., 0., 0., 0., 0., 0., 0., 0., 0., 1.);

MatrixD EvolQCDModule::invertMatrix4(9, 9, 1., 0., 0., 0., 0., 0., 0., 0., 0.,
        0., 0., 0., 0., 1. / 2., 1. / 6., 1. / 12., 0., 0., 0., 1., 0., 0., 0.,
        0., 0., 0., 0., 0., 0., 0., 0., -1. / 2., 1. / 6., 1. / 12., 0., 0., 0.,
        0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., -1. / 3., 1. / 12.,
        0., 0., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
        -1. / 4., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1.);

MatrixD EvolQCDModule::invertMatrix5(11, 11, 1., 0., 0., 0., 0., 0., 0., 0., 0.,
        0., 0., 0., 0., 0., 0., 0., 1. / 2., 1. / 6., 1. / 12., 1. / 20., 0.,
        0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
        -1. / 2., 1. / 6., 1. / 12., 1. / 20., 0., 0., 0., 0., 1., 0., 0., 0.,
        0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., -1. / 3., 1. / 12.,
        1. / 20., 0., 0., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
        0., 0., 0., 0., 0., -1. / 4., 1. / 20., 0., 0., 0., 0., 0., 0., 1., 0.,
        0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0, 0., -1. / 5., 0., 0., 0.,
        0., 0., 0., 0., 0., 0., 0., 0., 0., 1.);

MatrixD EvolQCDModule::invertMatrix6(13, 13, 1., 0., 0., 0., 0., 0., 0., 0., 0.,
        0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1. / 2., 1. / 6., 1. / 12.,
        1. / 20., 1. / 30., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
        0., 0., 0., 0., 0., 0., 0., 0., -1. / 2., 1. / 6., 1. / 12., 1. / 20.,
        1. / 30., 0., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
        0., 0., 0., 0., 0., 0., 0., -1. / 3., 1. / 12., 1. / 20., 1. / 30., 0.,
        0., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
        0., 0., 0., 0., -1. / 4., 1. / 20., 1. / 30., 0., 0., 0., 0., 0., 0.,
        1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0, 0., 0.,
        -1. / 5., 1. / 30., 0., 0., 0., 0., 0., 0., 0., 1., 0., 0., 0., 0., 0.,
        0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., -1. / 6., 0., 0., 0.,
        0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1.);

//TODO quelles sont les valeurs par défauts lors de l'initialisation ?
EvolQCDModule::EvolQCDModule(const std::string &moduleID)
        : ModuleObject(moduleID), m_x(0), m_xi(0), m_t(0), m_MuF(0), m_MuR(0), m_MuF_ref(
                0), m_pGPDModule(0), m_qcdOrderType(QCDOrderType::UNDEFINED), m_currentGPDComputeType(
                GPDComputeType::H), m_alphaS(0), m_scaleDistinction(0), m_nfEvol(
                -1), m_nfMin(-1), m_nbXPoints(20), m_nbMuFPoints(1), m_epsilon(
                0.01), m_alpha(0.1) {
}

EvolQCDModule::EvolQCDModule(const EvolQCDModule &other)
        : ModuleObject(other) {
    m_x = other.m_x;
    m_xi = other.m_xi;
    m_t = other.m_t;
    m_MuF = other.m_MuF;
    m_MuR = other.m_MuR;
    m_MuF_ref = other.m_MuF_ref;

    m_nfMin = other.m_nfMin;
    m_nfEvol = other.m_nfEvol;
    m_nbXPoints = other.m_nbXPoints;
    m_nbMuFPoints = other.m_nbMuFPoints;
    m_epsilon = other.m_epsilon;
    m_alpha = other.m_alpha;

    m_qcdOrderType = other.m_qcdOrderType;
    //m_runningAlphaS = other.m_runningAlphaS;
    m_alphaS = other.m_alphaS;
    m_scaleDistinction = other.m_scaleDistinction;

    m_currentGPDComputeType = other.m_currentGPDComputeType;
    m_qcdOrderType = other.m_qcdOrderType;

    if (other.m_pGPDModule != 0) {
        // GPDModule is an abstract class, so it's impossible to use copy constructor to get a new instance of the object
        m_pGPDModule = (other.m_pGPDModule)->clone();
    } else {
        m_pGPDModule = 0;
    }

    //TODO clone ou surcharger l'opérateur =
    //m_gpdResultData = other.m_gpdResultData;
    //m_gpdResultData = other.m_gpdResultData->clone();

    m_currentConvertMatrix = other.m_currentConvertMatrix;
    m_currentInvertMatrix = other.m_currentInvertMatrix;

    m_vectorOfGPDCombination = other.m_vectorOfGPDCombination;
}

//EvolQCDModule::EvolQCDModule* clone() const {
//    return new EvolQCDModule(*this);
//}

EvolQCDModule::~EvolQCDModule() {
}

void EvolQCDModule::initModule() {
//do nothing; the definition is just mandatory for enable the use of the clone function

//TODO MAJ des matrices de changement de base en fonction du Nf_EVOL choisi par l'utilisateur
    initNfMin();
    initMatrixValue();
    initVectorOfGPDCombinations();

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "NfMin = " << m_nfMin);
    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "Convert Matrix = "
                    << m_currentConvertMatrix.toString());
    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "Invert Matrix = "
                    << m_currentInvertMatrix.toString());
}

//TODO ajouter les tests manquants
void EvolQCDModule::isModuleWellConfigured() {
    if (m_pGPDModule == 0) {
        throw std::runtime_error("[EvolQCDModule] GPDModule* is NULL");
    }

    if (m_qcdOrderType == QCDOrderType::UNDEFINED) {
        throw std::runtime_error("[EvolQCDModule] QCDOrderType is UNDEFINED");
    }

    // test contrainte sur les bornes du nf_evol
    if (m_nfEvol < 1 || m_nfEvol > 6) {
        throw std::runtime_error(
                Formatter()
                        << "[EvolQCDModule] nfEvol is out of range ; a good value is between [1 : 6]");
    }

    // Test range in MuF and MuF_ref
    if (m_MuF <= 0.) {
        throw std::runtime_error(
                Formatter() << "[EvolQCDModule] m_MuF is out of range ;"
                        << "m_MuF should be >0. Here m_MuF = " << m_MuF);
    }

    // Test range in MuF and MuF_ref
    if (m_MuF_ref <= 0.) {
        throw std::runtime_error(
                Formatter() << "[EvolQCDModule] m_MuF_ref is out of range ;"
                        << "m_MuF_ref should be >0. Here m_MuF_ref = "
                        << m_MuF_ref);
    }
}

void EvolQCDModule::initNfMin() {

    size_t nfGPDModel = m_gpdResultData.sizeOfListOfQuarkFlavorData();

    // si nf_evol = -1 alors nf_evol = nf_gpd
    if (m_nfEvol == -1) {
        m_nfEvol = nfGPDModel;
        m_nfMin = nfGPDModel;
        // si nf_evol < nf_gpd alors nf_min = nf_evol
    } else if (m_nfEvol < nfGPDModel) {
        m_nfMin = m_nfEvol;
        // si nf_evol >= nf_gpd alors nf_min = nf_gpd
    } else {
        m_nfMin = nfGPDModel;
    }
}

void EvolQCDModule::initMatrixValue() {
    // care about m_nfEvol = 0 ; divided by ZERO
    double missingCoef = 1. / (2. * (double) m_nfEvol);
    double missingInvertCoef = 0.;

    switch (m_nfMin) {
    case 1:
        m_currentConvertMatrix = EvolQCDModule::conversionMatrix1;
        m_currentConvertMatrix.update(1, 1, missingCoef);

        m_currentInvertMatrix = EvolQCDModule::invertMatrix1;
        m_currentInvertMatrix.update(1, 1, 2. * m_nfEvol);
        break;

    case 2:
        m_currentConvertMatrix = EvolQCDModule::conversionMatrix2;
        m_currentConvertMatrix.update(3, 1, missingCoef);
        m_currentConvertMatrix.update(3, 3, missingCoef);

        m_currentInvertMatrix = EvolQCDModule::invertMatrix2;
        m_currentInvertMatrix.update(1, 3, m_nfEvol);
        m_currentInvertMatrix.update(3, 3, m_nfEvol);
        break;

    case 3:
        m_currentConvertMatrix = EvolQCDModule::conversionMatrix3;
        m_currentConvertMatrix.update(5, 1, missingCoef);
        m_currentConvertMatrix.update(5, 3, missingCoef);
        m_currentConvertMatrix.update(5, 5, missingCoef);

        missingInvertCoef = (2. * m_nfEvol) / 3.;
        m_currentInvertMatrix = EvolQCDModule::invertMatrix3;
        m_currentInvertMatrix.update(1, 5, missingInvertCoef);
        m_currentInvertMatrix.update(3, 5, missingInvertCoef);
        m_currentInvertMatrix.update(5, 5, missingInvertCoef);
        break;

    case 4:
        m_currentConvertMatrix = EvolQCDModule::conversionMatrix4;
        m_currentConvertMatrix.update(7, 1, missingCoef);
        m_currentConvertMatrix.update(7, 3, missingCoef);
        m_currentConvertMatrix.update(7, 5, missingCoef);
        m_currentConvertMatrix.update(7, 7, missingCoef);

        missingInvertCoef = m_nfEvol / 2.;
        m_currentInvertMatrix = EvolQCDModule::invertMatrix4;
        m_currentInvertMatrix.update(1, 7, missingInvertCoef);
        m_currentInvertMatrix.update(3, 7, missingInvertCoef);
        m_currentInvertMatrix.update(5, 7, missingInvertCoef);
        m_currentInvertMatrix.update(7, 7, missingInvertCoef);
        break;

    case 5:
        m_currentConvertMatrix = EvolQCDModule::conversionMatrix5;
        m_currentConvertMatrix.update(9, 1, missingCoef);
        m_currentConvertMatrix.update(9, 3, missingCoef);
        m_currentConvertMatrix.update(9, 5, missingCoef);
        m_currentConvertMatrix.update(9, 7, missingCoef);
        m_currentConvertMatrix.update(9, 9, missingCoef);

        missingInvertCoef = (2. * m_nfEvol) / 5.;
        m_currentInvertMatrix = EvolQCDModule::invertMatrix5;
        m_currentInvertMatrix.update(1, 9, missingInvertCoef);
        m_currentInvertMatrix.update(3, 9, missingInvertCoef);
        m_currentInvertMatrix.update(5, 9, missingInvertCoef);
        m_currentInvertMatrix.update(7, 9, missingInvertCoef);
        m_currentInvertMatrix.update(9, 9, missingInvertCoef);
        break;

    case 6:
        m_currentConvertMatrix = EvolQCDModule::conversionMatrix6;
        m_currentConvertMatrix.update(11, 1, missingCoef);
        m_currentConvertMatrix.update(11, 3, missingCoef);
        m_currentConvertMatrix.update(11, 5, missingCoef);
        m_currentConvertMatrix.update(11, 7, missingCoef);
        m_currentConvertMatrix.update(11, 9, missingCoef);
        m_currentConvertMatrix.update(11, 11, missingCoef);

        missingInvertCoef = m_nfEvol / 3.;
        m_currentInvertMatrix = EvolQCDModule::invertMatrix6;
        m_currentInvertMatrix.update(1, 11, missingInvertCoef);
        m_currentInvertMatrix.update(3, 11, missingInvertCoef);
        m_currentInvertMatrix.update(5, 11, missingInvertCoef);
        m_currentInvertMatrix.update(7, 11, missingInvertCoef);
        m_currentInvertMatrix.update(9, 11, missingInvertCoef);
        m_currentInvertMatrix.update(11, 11, missingInvertCoef);
        break;

    default:
        break;
    }
}

void EvolQCDModule::initVectorOfGPDCombinations() {
    m_vectorOfGPDCombination = MakeVectorOfGPDCombinations(m_gpdResultData);
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
    return (fabs(MuF - MuF_ref) > (m_epsilon * MuF_ref)) ? true : false;
}

bool EvolQCDModule::isAbsoluteTest(const double &MuF, const double &MuF_ref) {
    return (fabs(MuF - MuF_ref) > m_alpha) ? true : false;
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

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "x = " << x << "    xi = " << xi << "    t = " << t
                    << " GeV2    MuF = " << MuF << " GeV    MuR = " << MuR
                    << " GeV");

//	EvolQCDModule::initModule();
//	EvolQCDModule::isModuleWellConfigured();
}

//TODO ajouter les commentaires qui vont bien et les références au papier
std::vector<double> EvolQCDModule::convertBasis(
        std::vector<double> vectorToConvert) {
    std::vector<double> tempVector = m_currentConvertMatrix * vectorToConvert;
    return tempVector;
}

std::vector<double> EvolQCDModule::invertBasis(
        std::vector<double> vectorToInvert) {
    std::vector<double> tempVector = m_currentInvertMatrix * vectorToInvert;
    return tempVector;
}

//TODO automatiser les setters
GPDResultData EvolQCDModule::makeGPDResultData() {
    GPDResultData gpdResultData(m_currentGPDComputeType);

    // set gluon
    gpdResultData.setGluon(
            m_vectorOfGPDCombination[m_vectorOfGPDCombination.size() - 1]);

    GPDQuarkFlavorData quarkFlavorData(m_currentGPDComputeType,
            QuarkFlavor::UNDEFINED);

    // TODO documenter le calcul si contre
    switch ((m_vectorOfGPDCombination.size() - 1) / 2) {
    case 6:
        quarkFlavorData = GPDQuarkFlavorData(m_currentGPDComputeType,
                QuarkFlavor::TOP);
        quarkFlavorData.setPartonDistributionMinus(
                m_vectorOfGPDCombination[10]);
        quarkFlavorData.setPartonDistributionPlus(m_vectorOfGPDCombination[11]);
        quarkFlavorData.setPartonDistribution(
                calculateFq(m_vectorOfGPDCombination[10],
                        m_vectorOfGPDCombination[11]));
        gpdResultData.addGPDQuarkFlavorData(quarkFlavorData);
    case 5:
        quarkFlavorData = GPDQuarkFlavorData(m_currentGPDComputeType,
                QuarkFlavor::BOTTOM);
        quarkFlavorData.setPartonDistributionMinus(m_vectorOfGPDCombination[8]);
        quarkFlavorData.setPartonDistributionPlus(m_vectorOfGPDCombination[9]);
        quarkFlavorData.setPartonDistribution(
                calculateFq(m_vectorOfGPDCombination[8],
                        m_vectorOfGPDCombination[9]));
        gpdResultData.addGPDQuarkFlavorData(quarkFlavorData);
    case 4:
        quarkFlavorData = GPDQuarkFlavorData(m_currentGPDComputeType,
                QuarkFlavor::CHARM);
        quarkFlavorData.setPartonDistributionMinus(m_vectorOfGPDCombination[6]);
        quarkFlavorData.setPartonDistributionPlus(m_vectorOfGPDCombination[7]);
        quarkFlavorData.setPartonDistribution(
                calculateFq(m_vectorOfGPDCombination[6],
                        m_vectorOfGPDCombination[7]));
        gpdResultData.addGPDQuarkFlavorData(quarkFlavorData);
    case 3:
        quarkFlavorData = GPDQuarkFlavorData(m_currentGPDComputeType,
                QuarkFlavor::STRANGE);
        quarkFlavorData.setPartonDistributionMinus(m_vectorOfGPDCombination[4]);
        quarkFlavorData.setPartonDistributionPlus(m_vectorOfGPDCombination[5]);
        quarkFlavorData.setPartonDistribution(
                calculateFq(m_vectorOfGPDCombination[4],
                        m_vectorOfGPDCombination[5]));
        gpdResultData.addGPDQuarkFlavorData(quarkFlavorData);
    case 2:
        quarkFlavorData = GPDQuarkFlavorData(m_currentGPDComputeType,
                QuarkFlavor::DOWN);
        quarkFlavorData.setPartonDistributionMinus(m_vectorOfGPDCombination[2]);
        quarkFlavorData.setPartonDistributionPlus(m_vectorOfGPDCombination[3]);
        quarkFlavorData.setPartonDistribution(
                calculateFq(m_vectorOfGPDCombination[2],
                        m_vectorOfGPDCombination[3]));
        gpdResultData.addGPDQuarkFlavorData(quarkFlavorData);
    case 1:
        quarkFlavorData = GPDQuarkFlavorData(m_currentGPDComputeType,
                QuarkFlavor::UP);
        quarkFlavorData.setPartonDistributionMinus(m_vectorOfGPDCombination[0]);
        quarkFlavorData.setPartonDistributionPlus(m_vectorOfGPDCombination[1]);
        quarkFlavorData.setPartonDistribution(
                calculateFq(m_vectorOfGPDCombination[0],
                        m_vectorOfGPDCombination[1]));
        gpdResultData.addGPDQuarkFlavorData(quarkFlavorData);
        break;
    default:
        break;
    }

    return gpdResultData;
}

double EvolQCDModule::calculateFq(double FMinus, double FPlus) {
    return (FPlus + FMinus) / 2.;
}

QCDOrderType::Type EvolQCDModule::getQcdOrderType() const {
    return m_qcdOrderType;
}

void EvolQCDModule::setQcdOrderType(QCDOrderType::Type qcdOrderType) {
    m_qcdOrderType = qcdOrderType;
}

std::vector<double> EvolQCDModule::MakeVectorOfGPDCombinations(
        GPDResultData gpdResultData) {
    std::vector<GPDQuarkFlavorData> listOfQuarkFlavorData =
            gpdResultData.getListOfQuarkFlavorData();

    //TODO creation en fonction de nfMin pour tronquer ce qui doit l'être

    // q+ et q- (HNonSinglet) pour chaque saveur de quark, HSinglet et Hg
    //m_vectorOfGPDCombination.resize(listOfQuarkFlavorData.size() * 2 + 1);

    std::vector<double> vectorOfGPDCombination;

    for (unsigned int i = 0; i != m_nfMin; i++) {
        vectorOfGPDCombination.push_back(
                listOfQuarkFlavorData[i].getPartonDistributionMinus());
        vectorOfGPDCombination.push_back(
                listOfQuarkFlavorData[i].getPartonDistributionPlus());
    }

    vectorOfGPDCombination.push_back(m_gpdResultData.getGluon());

    return vectorOfGPDCombination;
}

const GPDModule* EvolQCDModule::getGpdModule() const {
    return m_pGPDModule;
}

void EvolQCDModule::setGpdModule(GPDModule* gpdModule) {
    m_pGPDModule = gpdModule;
    m_MuF_ref = m_pGPDModule->getMuFRef();

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "GPDModule = " << m_pGPDModule->getClassName());

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "MuF_ref = " << m_MuF_ref << " GeV");
}

int EvolQCDModule::getEvolutionActiveFlavors() const {
    return m_nfEvol;
}

void EvolQCDModule::setEvolutionActiveFlavors(int nfEvol) {
    m_nfEvol = nfEvol;
}

unsigned int EvolQCDModule::getNbMuFPoints() const {
    return m_nbMuFPoints;
}

unsigned int EvolQCDModule::getNbXPoints() const {
    return m_nbXPoints;
}
