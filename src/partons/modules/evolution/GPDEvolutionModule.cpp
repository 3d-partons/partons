#include "../../../../include/partons/modules/evolution/GPDEvolutionModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <math.h>
#include <NumA/linear_algebra/matrix/MatrixD.h>
#include <iostream>

#include "../../../../include/partons/beans/List.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/modules/active_flavors/NfFunctionExample.h"
#include "../../../../include/partons/modules/alphaS/RunningAlphaStrong.h"
#include "../../../../include/partons/modules/GPDModule.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

NumA::MatrixD GPDEvolutionModule::conversionMatrix1(3, 3, //
        1., 0., 0., //
        0., 1., 0., //
        0., 0., 1.); //

NumA::MatrixD GPDEvolutionModule::conversionMatrix2(5, 5, //
        1., 0., 0., 0., 0., //
        0., 1., 0., 1., 0., //
        0., 0., 1., 0., 0., //
        0., 1., 0., -1., 0., //
        0., 0., 0., 0., 1.); //

NumA::MatrixD GPDEvolutionModule::conversionMatrix3(7, 7, //
        1., 0., 0., 0., 0., 0., 0., //
        0., 1., 0., 1., 0., 1., 0., //
        0., 0., 1., 0., 0., 0., 0., //
        0., 1., 0., -1., 0., 0., 0., //
        0., 0., 0., 0., 1., 0., 0., //
        0., 1., 0., 1., 0., -2., 0., //
        0., 0., 0., 0., 0., 0., 1.); //

NumA::MatrixD GPDEvolutionModule::conversionMatrix4(9, 9, //
        1., 0., 0., 0., 0., 0., 0., 0., 0., //
        0., 1., 0., 1., 0., 1., 0., 1., 0., //
        0., 0., 1., 0., 0., 0., 0., 0., 0., //
        0., 1., 0., -1., 0., 0., 0., 0., 0., //
        0., 0., 0., 0., 1., 0., 0., 0., 0., //
        0., 1., 0., 1., 0., -2., 0., 0., 0., //
        0., 0., 0., 0., 0., 0., 1., 0., 0., //
        0., 1., 0., 1., 0., 1., 0., -3., 0., //
        0., 0., 0., 0., 0., 0., 0., 0., 1.); //

NumA::MatrixD GPDEvolutionModule::conversionMatrix5(11, 11, //
        1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., //
        0., 1., 0., 1., 0., 1., 0., 1., 0., 1., 0., //
        0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., //
        0., 1., 0., -1., 0., 0., 0., 0., 0., 0., 0., //
        0., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0., //
        0., 1., 0., 1., 0., -2., 0., 0., 0., 0., 0., //
        0., 0., 0., 0., 0., 0., 1., 0., 0., 0., 0., //
        0., 1., 0., 1., 0., 1., 0., -3., 0., 0., 0., //
        0., 0., 0., 0., 0., 0., 0., 0., 1., 0., 0., //
        0., 1., 0., 1., 0., 1., 0., 1., 0., -4., 0., //
        0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1.); //

NumA::MatrixD GPDEvolutionModule::conversionMatrix6(13, 13, //
        1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., //
        0., 1., 0., 1., 0., 1., 0., 1., 0., 1., 0., 1., 0., //
        0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., //
        0., 1., 0., -1., 0., 0., 0., 0., 0., 0., 0., 0., 0., //
        0., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., //
        0., 1., 0., 1., 0., -2., 0., 0., 0., 0., 0., 0., 0., //
        0., 0., 0., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0., //
        0., 1., 0., 1., 0., 1., 0., -3., 0., 0., 0., 0., 0., //
        0., 0., 0., 0., 0., 0., 0., 0., 1., 0., 0., 0., 0., //
        0., 1., 0., 1., 0., 1., 0., 1., 0., -4., 0., 0., 0., //
        0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1., 0., 0., //
        0., 1., 0., 1., 0., 1., 0., 1., 0., 1., 0., -5., 0., //
        0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1.); //

NumA::MatrixD GPDEvolutionModule::invertMatrix1(3, 3, //
        1., 0., 0., //
        0., 1., 0., //
        0., 0., 1.); //

NumA::MatrixD GPDEvolutionModule::invertMatrix2(5, 5, //
        1., 0., 0., 0., 0., //
        0., 1. / 2., 0., 1. / 2., 0., //
        0., 0., 1., 0., 0., //
        0., 1. / 2., 0., -1. / 2., 0., //
        0., 0., 0., 0., 1.); //

NumA::MatrixD GPDEvolutionModule::invertMatrix3(7, 7, //
        1., 0., 0., 0., 0., 0., 0., //
        0., 1. / 3., 0., 1. / 2., 0., 1. / 6., 0., //
        0., 0., 1., 0., 0., 0., 0., //
        0., 1. / 3., 0., -1. / 2., 0., 1. / 6., 0., //
        0., 0., 0., 0., 1., 0., 0., //
        0., 1. / 3., 0., 0., 0., -1. / 3., 0., //
        0., 0., 0., 0., 0., 0., 1.); //

NumA::MatrixD GPDEvolutionModule::invertMatrix4(9, 9, //
        1., 0., 0., 0., 0., 0., 0., 0., 0., //
        0., 1. / 4., 0., 1. / 2., 0., 1. / 6., 0, 1. / 12., //
        0., 0., 1., 0., 0., 0., 0., 0., 0., //
        0., 1. / 4., 0., -1. / 2., 0., 1. / 6., 0, 1. / 12., //
        0., 0., 0., 0., 0., 1., 0., 0., 0., //
        0., 1. / 4., 0., 0., 0., -1. / 3., 0, 1. / 12., //
        0., 0., 0., 0., 0., 0., 1., 0., 0., //
        0., 1. / 4., 0., 0., 0., 0., 0., -1. / 4., 0., //
        0., 0., 0., 0., 0., 0., 0., 0., 1.); //

NumA::MatrixD GPDEvolutionModule::invertMatrix5(11, 11, //
        1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., //
        0., 1. / 5., 0., 1. / 2., 0., 1. / 6., 0., 1. / 12., 0., 1. / 20., 0., //
        0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., //
        0., 1. / 5., 0., -1. / 2., 0., 1. / 6., 0., 1. / 12., 0., 1. / 20., 0., //
        0., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0., //
        0., 1. / 5., 0., 0., 0., -1. / 3., 0., 1. / 12., 0., 1. / 20., 0., //
        0., 0., 0., 0., 0., 0., 1., 0., 0., 0., 0., //
        0., 1. / 5., 0., 0., 0., 0., 0., -1. / 4., 0., 1. / 20., 0., //
        0., 0., 0., 0., 0., 0., 0., 0., 1., 0., 0., //
        0., 1. / 5., 0., 0., 0., 0., 0., 0, 0., -1. / 5., 0., //
        0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1.); //

NumA::MatrixD GPDEvolutionModule::invertMatrix6(13,
        13, //
        1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
        0., //
        0., 1. / 6., 0., 1. / 2., 0., 1. / 6., 0., 1. / 12., 0., 1. / 20., 0.,
        1. / 30.,
        0., //
        0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
        0., //
        0., 1. / 6., 0., -1. / 2., 0., 1. / 6., 0., 1. / 12., 0., 1. / 20., 0.,
        1. / 30.,
        0., //
        0., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0., 0.,
        0., //
        0., 1. / 6., 0., 0., 0., -1. / 3., 0., 1. / 12., 0., 1. / 20., 0.,
        1. / 30.,
        0., //
        0., 0., 0., 0., 0., 0., 1., 0., 0., 0., 0., 0.,
        0., //
        0., 1. / 6., 0., 0., 0., 0., 0., -1. / 4., 0., 1. / 20., 0., 1. / 30.,
        0., //
        0., 0., 0., 0., 0., 0., 0., 0., 1., 0., 0., 0., 0., //
        0., 1. / 6., 0., 0., 0., 0., 0., 0., 0., -1. / 5., 0., 1. / 30., 0., //
        0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1., 0., 0., //
        0., 1. / 6., 0., 0., 0., 0., 0., 0., 0., 0., 0., -1. / 6., 0., //
        0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1.); //

const std::string GPDEvolutionModule::GPD_EVOLUTION_MODULE_CLASS_NAME =
        "GPDEvolutionModule";

//TODO quelles sont les valeurs par défauts lors de l'initialisation ?
GPDEvolutionModule::GPDEvolutionModule(const std::string &className) :
        ModuleObject(className), m_x(0), m_xi(0), m_t(0), m_MuF2(0), m_MuR2(0), m_pGPDModule(
                0), m_qcdOrderType(PerturbativeQCDOrderType::UNDEFINED), m_currentGPDComputeType(
                GPDType::UNDEFINED), m_pNfFunction(0), m_pNfEvolFunction(0), m_pRunningAlphaStrong(
                0), m_scaleDistinction(0), m_nbXPoints(20), m_nbMuFPoints(1), m_epsilon(
                0.01), m_alpha(0.1), m_currentNf(0), m_currentNonSingletIndex(0) {
}

GPDEvolutionModule::GPDEvolutionModule(const GPDEvolutionModule &other) :
        ModuleObject(other) {
    m_x = other.m_x;
    m_xi = other.m_xi;
    m_t = other.m_t;
    m_MuF2 = other.m_MuF2;
    m_MuR2 = other.m_MuR2;

    //TODO sert à quoi ?
    m_nbXPoints = other.m_nbXPoints;
    m_nbMuFPoints = other.m_nbMuFPoints;
    m_epsilon = other.m_epsilon;
    m_alpha = other.m_alpha;
    //TODO sert à quoi ?

    m_qcdOrderType = other.m_qcdOrderType;
    m_scaleDistinction = other.m_scaleDistinction;
    m_currentGPDComputeType = other.m_currentGPDComputeType;

    m_currentNf = other.m_currentNf;
    m_currentNonSingletIndex = other.m_currentNonSingletIndex;

    m_partonDistributionFlavorBase = other.m_partonDistributionFlavorBase;
    m_partonDistributionEvolutionBase = other.m_partonDistributionEvolutionBase;

    if (other.m_pGPDModule != 0) {
        // GPDModule is an abstract class, so it's impossible to use copy constructor to get a new instance of the object
        m_pGPDModule = (other.m_pGPDModule)->clone();
    } else {
        m_pGPDModule = 0;
    }

    if (other.m_pNfFunction != 0) {
        // ActiveFlavorsModule is an abstract class, so it's impossible to use copy constructor to get a new instance of the object
        m_pNfFunction = (other.m_pNfFunction)->clone();
    } else {
        m_pNfFunction = 0;
    }

    if (other.m_pNfEvolFunction != 0) {
        // ActiveFlavorsModule is an abstract class, so it's impossible to use copy constructor to get a new instance of the object
        m_pNfEvolFunction = (other.m_pNfEvolFunction)->clone();
    } else {
        m_pNfEvolFunction = 0;
    }

    if (other.m_pRunningAlphaStrong != 0) {
        // RunningAlphaStrong is an abstract class, so it's impossible to use copy constructor to get a new instance of the object
        m_pRunningAlphaStrong = (other.m_pRunningAlphaStrong)->clone();
    } else {
        m_pRunningAlphaStrong = 0;
    }
}

GPDEvolutionModule::~GPDEvolutionModule() {
}

//TODO replace hardcoded active flavors module
void GPDEvolutionModule::resolveObjectDependencies() {
    m_pRunningAlphaStrong =
            Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                    RunningAlphaStrong::classId);

    m_pNfFunction =
            Partons::getInstance()->getModuleObjectFactory()->newActiveFlavorsModule(
                    NfFunctionExample::classId);

    m_pNfEvolFunction =
            Partons::getInstance()->getModuleObjectFactory()->newActiveFlavorsModule(
                    NfFunctionExample::classId);
}

//TODO logger
void GPDEvolutionModule::preCompute(double x, double xi, double t, double MuF2,
        double MuR2, GPDModule* pGPDModule, GPDType::Type gpdType) {
    debug(__func__, "");

    m_pGPDModule = pGPDModule;

    m_x = x;
    m_xi = xi;
    m_t = t;
    m_MuF2 = MuF2;
    m_MuR2 = MuR2;

    m_currentGPDComputeType = gpdType;
}

//TODO logger
void GPDEvolutionModule::initModule() {
    debug(__func__, "Processing ...");
}

//TODO ajouter les tests manquants
void GPDEvolutionModule::isModuleWellConfigured() {
    debug(__func__, "");

    if (m_pNfFunction == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "m_pNfFunction* is NULL");
    }

    if (m_pNfEvolFunction == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "m_pNfEvolFunction* is NULL");
    }

    if (m_pGPDModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "GPDModule* is NULL");
    }

    if (m_qcdOrderType == PerturbativeQCDOrderType::UNDEFINED) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "QCDOrderType is UNDEFINED");
    }

    // Test range in MuF and MuF_ref
    if (m_MuF2 <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "m_MuF2 is out of range ;"
                        << "m_MuF2 should be >0. Here m_MuF2 = " << m_MuF2);
    }
}

bool GPDEvolutionModule::isRunnable(double MuF2, double MuF2_ref,
        GPDEvolutionModule::Type testType) {
    debug(__func__, "entered");

    //TODO MuF de l'utilisateur par rapport au MuF_Ref du modèle de GPD
    bool result = false;
    switch (testType) {
    case GPDEvolutionModule::RELATIVE:
        result = isRelativeTest(MuF2, MuF2_ref);
        break;
    case GPDEvolutionModule::ABSOLUTE:
        result = isAbsoluteTest(MuF2, MuF2_ref);
        break;
    case GPDEvolutionModule::BOTH:
        result = (isRelativeTest(MuF2, MuF2_ref)
                && isAbsoluteTest(MuF2, MuF2_ref));
        break;
    default:
        break;
    }

    return result;
}

bool GPDEvolutionModule::isRelativeTest(double MuF2, double MuF2_ref) {
    debug(__func__,
            ElemUtils::Formatter() << "MuF2 = " << MuF2 << "   MuF2_ref = "
                    << MuF2_ref);

    return (fabs(MuF2 - MuF2_ref) > (m_epsilon * MuF2_ref)) ? true : false;
}

bool GPDEvolutionModule::isAbsoluteTest(double MuF2, double MuF2_ref) {
    debug(__func__, "");

    return (fabs(MuF2 - MuF2_ref) > m_alpha) ? true : false;
}

//TODO ajouter les commentaires qui vont bien et les références au papier
double GPDEvolutionModule::convertBasis(const NumA::VectorD &vectorToConvert,
        unsigned short currentNf, unsigned short nonSingletIndex) {

    double result;

    switch (currentNf) {
    case 1:
        result = GPDEvolutionModule::conversionMatrix1.getLine(nonSingletIndex)
                * vectorToConvert;
        break;

    case 2:
        result = GPDEvolutionModule::conversionMatrix2.getLine(nonSingletIndex)
                * vectorToConvert;
        break;

    case 3:
        result = GPDEvolutionModule::conversionMatrix3.getLine(nonSingletIndex)
                * vectorToConvert;
        break;

    case 4:
        result = GPDEvolutionModule::conversionMatrix4.getLine(nonSingletIndex)
                * vectorToConvert;
        break;

    case 5:
        result = GPDEvolutionModule::conversionMatrix5.getLine(nonSingletIndex)
                * vectorToConvert;
        break;

    case 6:
        result = GPDEvolutionModule::conversionMatrix6.getLine(nonSingletIndex)
                * vectorToConvert;
        break;

    default:
        break;
    }

    return result;
}

//TODO ajouter les commentaires qui vont bien et les références au papier
NumA::VectorD GPDEvolutionModule::convertBasis(
        const NumA::VectorD &vectorToConvert, unsigned short matrixNum) {

    NumA::VectorD result;

    switch (matrixNum) {
    case 1:
        result = GPDEvolutionModule::conversionMatrix1 * vectorToConvert;
        break;

    case 2:
        result = GPDEvolutionModule::conversionMatrix2 * vectorToConvert;
        break;

    case 3:
        result = GPDEvolutionModule::conversionMatrix3 * vectorToConvert;
        break;

    case 4:
        result = GPDEvolutionModule::conversionMatrix4 * vectorToConvert;
        break;

    case 5:
        result = GPDEvolutionModule::conversionMatrix5 * vectorToConvert;
        break;

    case 6:
        result = GPDEvolutionModule::conversionMatrix6 * vectorToConvert;
        break;

    default:
        break;
    }

    return result;
}

NumA::VectorD GPDEvolutionModule::invertBasis(
        const NumA::VectorD &vectorToInvert, unsigned short matrixNum) {
    NumA::VectorD result;

    switch (matrixNum) {
    case 1:
        result = GPDEvolutionModule::invertMatrix1 * vectorToInvert;
        break;

    case 2:
        result = GPDEvolutionModule::invertMatrix2 * vectorToInvert;
        break;

    case 3:
        result = GPDEvolutionModule::invertMatrix3 * vectorToInvert;
        break;

    case 4:
        result = GPDEvolutionModule::invertMatrix4 * vectorToInvert;
        break;

    case 5:
        result = GPDEvolutionModule::invertMatrix5 * vectorToInvert;
        break;

    case 6:
        result = GPDEvolutionModule::invertMatrix6 * vectorToInvert;
        break;

    default:
        break;
    }

    return result;
}

NumA::VectorD GPDEvolutionModule::makeVectorOfGPDCombinations(
        const PartonDistribution &partonDistribution) {

    NumA::VectorD vectorOfGPDCombinations;

    List<QuarkDistribution> quarkDistributions =
            partonDistribution.getListOfQuarkDistribution();

    // first entry of vector = gluon value
    vectorOfGPDCombinations.push_back(
            partonDistribution.getGluonDistribution().getGluonDistribution());

    // TODO add comment : see note n° blabla how to make vector of GPD combination
    for (unsigned short i = 0; i != quarkDistributions.size(); i++) {
        vectorOfGPDCombinations.push_back(
                quarkDistributions[i].getQuarkDistributionPlus());
        vectorOfGPDCombinations.push_back(
                quarkDistributions[i].getQuarkDistributionMinus());
    }

    return vectorOfGPDCombinations;
}

PartonDistribution GPDEvolutionModule::makeFinalPartonDistribution() {
    debug(__func__, "");

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(
            GluonDistribution(m_partonDistributionFlavorBase[0]));

    QuarkDistribution quarkDistribution(QuarkFlavor::UNDEFINED);

    // TODO documenter le calcul si contre ; first entry in vector = gluon value
    switch ((m_partonDistributionFlavorBase.size() - 1) / 2) {
    case 6:
        quarkDistribution = makeFinalQuarkDistribution(QuarkFlavor::TOP,
                m_partonDistributionFlavorBase[11],
                m_partonDistributionFlavorBase[12]);
        partonDistribution.addQuarkDistribution(quarkDistribution);
        /* no break */
    case 5:
        quarkDistribution = makeFinalQuarkDistribution(QuarkFlavor::BOTTOM,
                m_partonDistributionFlavorBase[9],
                m_partonDistributionFlavorBase[10]);
        partonDistribution.addQuarkDistribution(quarkDistribution);
        /* no break */
    case 4:
        quarkDistribution = makeFinalQuarkDistribution(QuarkFlavor::CHARM,
                m_partonDistributionFlavorBase[7],
                m_partonDistributionFlavorBase[8]);
        partonDistribution.addQuarkDistribution(quarkDistribution);
        /* no break */
    case 3:
        quarkDistribution = makeFinalQuarkDistribution(QuarkFlavor::STRANGE,
                m_partonDistributionFlavorBase[5],
                m_partonDistributionFlavorBase[6]);
        partonDistribution.addQuarkDistribution(quarkDistribution);
        /* no break */
    case 2:
        quarkDistribution = makeFinalQuarkDistribution(QuarkFlavor::DOWN,
                m_partonDistributionFlavorBase[3],
                m_partonDistributionFlavorBase[4]);
        partonDistribution.addQuarkDistribution(quarkDistribution);
        /* no break */
    case 1:
        quarkDistribution = makeFinalQuarkDistribution(QuarkFlavor::UP,
                m_partonDistributionFlavorBase[1],
                m_partonDistributionFlavorBase[2]);
        partonDistribution.addQuarkDistribution(quarkDistribution);
        break;
    default:
        break;
    }

    return partonDistribution;
}

QuarkDistribution GPDEvolutionModule::makeFinalQuarkDistribution(
        QuarkFlavor::Type quarkFlavor, double quarkDistributionPlus,
        double quarkDistributionMinus) {
    QuarkDistribution quarkDistribution = QuarkDistribution(quarkFlavor);
    quarkDistribution.setQuarkDistributionPlus(quarkDistributionPlus);
    quarkDistribution.setQuarkDistributionMinus(quarkDistributionMinus);
    quarkDistribution.setQuarkDistribution(
            calculateFq(quarkDistributionPlus, quarkDistributionMinus));

    return quarkDistribution;
}

double GPDEvolutionModule::calculateFq(double FPlus, double FMinus) {
    debug(__func__,
            ElemUtils::Formatter() << " FMinus = " << FMinus << "   FPlus = "
                    << FPlus);

    return (FMinus + FPlus) / 2.;
}

PartonDistribution GPDEvolutionModule::compute(double x, double xi, double t,
        double MuF2, double MuR2, GPDModule* pGPDModule,
        GPDType::Type gpdType) {
    preCompute(x, xi, t, MuF2, MuR2, pGPDModule, gpdType);

    // 1. retrieve intervals from function nf over MuF in descending order nf ; from MuF_ref to MuF
    std::vector<NfInterval> nfIntervals = m_pNfFunction->getNfIntervals(
            m_pGPDModule->getMuF2Ref(), m_MuF2);

    std::cerr << "nfIntervals = " << nfIntervals.size() << " for MuF2_ref = "
            << m_pGPDModule->getMuF2Ref() << " and MuF2 = " << m_MuF2
            << std::endl;

//    // sort descending order
//    for (size_t i = nfIntervals.size() - 1; i != -1; i--) {
//        m_invertedIntervals.push_back(nfIntervals[i]);
//    }

    m_invertedIntervals = nfIntervals;

    std::cerr << "m_invertedIntervals = " << m_invertedIntervals.size()
            << std::endl;

    // 2.
    evolutionR(x, nfIntervals.size() - 1);

    return makeFinalPartonDistribution();
}

void GPDEvolutionModule::evolutionR(double x,
        unsigned int indexCurrentInterval) {
    if (indexCurrentInterval == 0) {
        // end of recursivity ; compute GPD at Muf_ref
        double MuF2_ref =
                m_invertedIntervals[indexCurrentInterval].getLowerBound();
        m_partonDistributionFlavorBase = makeVectorOfGPDCombinations(
                m_pGPDModule->compute(x, m_xi, m_t, MuF2_ref, MuF2_ref,
                        m_currentGPDComputeType));
    } else {
        evolutionR(x, indexCurrentInterval - 1);
    }

    // add new flavor if needed ; convert basis ; evolution ; invert basis
    evolution(m_invertedIntervals[indexCurrentInterval]);
}

//PartonDistribution GPDEvolutionModule::computeGPDModelAtMufRef() {
//    return m_pGPDModule->compute(m_x, m_xi, m_t, m_pGPDModule->getMuF2Ref(),
//            m_MuR2, m_currentGPDComputeType).getPartonDistribution(
//            m_currentGPDComputeType);
//}

//PartonDistribution GPDEvolutionModule::evolution(
//        const PartonDistribution &partonDistribution) {
//
//    // create the first vector of GPD combination to init evolution process ; at MuF_ref
//    m_partonDistributionFlavorBase = makeVectorOfGPDCombinations(
//            partonDistribution);
//
//    // perform evolution interval by interval ; from MuF_ref to MuF_final = MuF
//    for (unsigned short i = 0; i != nfIntervals.size(); i++) {
//        evolution (nfIntervals[i]);
//    }
//
//    return makeFinalPartonDistribution();
//}

void GPDEvolutionModule::evolution(const NfInterval &nfInterval) {
    m_currentNf = nfInterval.getNf();

    // compare nfInterval to nf of vector of GPD combination and resize vector of GPD combination to init evolution process
    resizeVectorOfGPDCombination(m_partonDistributionFlavorBase, m_currentNf);

    // convert from basis to handle Singlet and NonSinglet values
    m_partonDistributionEvolutionBase = convertBasis(
            m_partonDistributionFlavorBase, m_currentNf);

    computeNonSinglet(nfInterval);
    computeSingletGluon(nfInterval);

// invert from basis to handle quark distribution values
    m_partonDistributionFlavorBase = invertBasis(
            m_partonDistributionEvolutionBase, m_currentNf);
}

void GPDEvolutionModule::computeNonSinglet(const NfInterval &nfInterval) {
    // For each entry of the vector we compute FNS,i ; We started the loop at 2 because vector[0] = FG and vector[1] = FS
    for (unsigned int i = 2; i != m_partonDistributionEvolutionBase.size();
            i++) {
        m_currentNonSingletIndex = i;

        m_partonDistributionEvolutionBase[i] +=
                integratedNonSingletMuFDerivative(nfInterval);
    }
}

void GPDEvolutionModule::computeSingletGluon(const NfInterval &nfInterval) {
    // m_vectorOfNonSingletSingletGluon[0] = FG
    // m_vectorOfNonSingletSingletGluon[1] = FS
    m_partonDistributionEvolutionBase[0] = integratedGluonMuFDerivative(
            nfInterval);
    m_partonDistributionEvolutionBase[1] = integratedSingletMuFDerivative(
            nfInterval);
}

void GPDEvolutionModule::resizeVectorOfGPDCombination(
        NumA::VectorD &vectorOfGPDCombination, unsigned short nfInterval) {

    // -1 because first entry = gluon
    unsigned short nfGPDModel = (vectorOfGPDCombination.size() - 1) / 2;

    // if nfInterval < nfGPDModel troncate vectorOfGPDCombination
    if (nfInterval < nfGPDModel) {
        vectorOfGPDCombination.resize(
                vectorOfGPDCombination.size()
                        - ((nfGPDModel - nfInterval) * 2));
    }
    // else add new quark flavor(s) for each missing flavor ; init to 0.
    else {
        for (unsigned short i = nfGPDModel; i != nfInterval; i++) {
            // new qMinus = 0.
            vectorOfGPDCombination.push_back(0.);
            // new qPlus = 0.
            vectorOfGPDCombination.push_back(0.);
        }
    }
}

double GPDEvolutionModule::nonSingletGPD(unsigned short nonSingletIndex,
        unsigned short currentNf, double y, double MuF2) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(y, m_xi, m_t,
            MuF2, m_MuR2, m_currentGPDComputeType);

    NumA::VectorD vectorOfQuarkDistribution = makeVectorOfGPDCombinations(
            partonDistribution);

    // compare nfInterval to nf of vector of GPD combination and resize vector of GPD combination
    resizeVectorOfGPDCombination(vectorOfQuarkDistribution, currentNf);

    return convertBasis(vectorOfQuarkDistribution, currentNf, nonSingletIndex);
}

void GPDEvolutionModule::configure(const ElemUtils::Parameters &parameters) {
    if (parameters.isAvailable(
            PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE)) {
        m_qcdOrderType = PerturbativeQCDOrderType(
                parameters.getLastAvailable().getString()).getType();

        info(__func__,
                ElemUtils::Formatter()
                        << PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE
                        << " configured with value = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }

    ModuleObject::configure(parameters);
}

PerturbativeQCDOrderType::Type GPDEvolutionModule::getQcdOrderType() const {
    return m_qcdOrderType;
}

void GPDEvolutionModule::setQcdOrderType(
        PerturbativeQCDOrderType::Type qcdOrderType) {
    m_qcdOrderType = qcdOrderType;
}

void GPDEvolutionModule::setGpdModule(GPDModule* gpdModule) {
    m_pGPDModule = gpdModule;

    debug(__func__,
            ElemUtils::Formatter() << "GPDModule = "
                    << m_pGPDModule->getClassName());
}

void GPDEvolutionModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "TODO : implement");
}
//
//double GPDEvolutionModule::nonSingletMuFDerivative(
//        const NfInterval &nfInterval) {
//    error(__func__, "Must be implemented in daugther class");
//    return 0.;
//}
//
//double GPDEvolutionModule::singletMuFDerivative(const NfInterval &nfInterval) {
//    error(__func__, "Must be implemented in daugther class");
//    return 0.;
//}
//
//double GPDEvolutionModule::gluonMuFDerivative(const NfInterval &nfInterval) {
//    error(__func__, "Must be implemented in daugther class");
//    return 0.;
//}

