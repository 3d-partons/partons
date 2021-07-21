#include "../../../../../include/partons/modules/evolution/gpd/GPDEvolutionVinnikov.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/interpolation/CubicSpline.h>
#include <cmath>
#include <iterator>
#include <utility>

#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/active_flavors_thresholds/ActiveFlavorsThresholdsModule.h"
#include "../../../../../include/partons/modules/gpd/GPDModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongModule.h"

namespace PARTONS {

const std::string GPDEvolutionVinnikov::PARAM_NAME_GRID_SIZE = "gridSize";

const unsigned int GPDEvolutionVinnikov::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDEvolutionVinnikov("GPDEvolutionVinnikov"));

const double GPDEvolutionVinnikov::N_C = 3.0;
const double GPDEvolutionVinnikov::C_F = (0.5 * (N_C - 1.0 / N_C));
const double GPDEvolutionVinnikov::N_F = 3.0;
const double GPDEvolutionVinnikov::B_0 = (11.0 * N_C / 3.0 - 2.0 * N_F / 3.0);
const double GPDEvolutionVinnikov::C_A = (2.0 * C_F + 1.0 / N_C);
const double GPDEvolutionVinnikov::T_R = (N_F / 2.0);

GPDEvolutionVinnikov::GPDEvolutionVinnikov(const std::string& className) :
        GPDEvolutionModule(className), MathIntegratorModule(), m_gridSize(10), m_xiLast(
                0), m_tLast(0), m_MuR2Last(0), m_nFlavors(0), m_nFlavors_ref(0), m_pGPDModule(
                0) {

    //reset
    m_PartonDistributionAtMuF2_ref = PartonDistribution();

    //nf = 1
    conversionMatrices.insert(std::make_pair(1,

    NumA::MatrixD(2, 2,	//
//	GPD		u(-) u(+)
            +1., +1., // u(x)
            -1., +1.  // u(-x)
            ) * (1. / 2.)));

    //nf = 2
    conversionMatrices.insert(std::make_pair(2,

    NumA::MatrixD(4, 4,	//
//	GPD		u(-) d(-) u-d  Sum(+)
            +1., +1., +2., +1., // u(x)
            -3., +1., +2., +1., // u(-x)
            +1., +1., -2., +1., // d(x)
            +1., -3., -2., +1.  // d(-x)
            ) * (1. / 4.)));

    //nf = 3
    conversionMatrices.insert(std::make_pair(3,

    NumA::MatrixD(6, 6, //
//	GPD		u(-) d(-) s(-) u-d  u-s  Sum(+)
            +1., +1., +1., +2., +2., +1., // u(x)
            -5., +1., +1., +2., +2., +1., // u(-x)
            +1., +1., +1., -4., +2., +1., // d(x)
            +1., -5., +1., -4., +2., +1., // d(-x)
            +1., +1., +1., +2., -4., +1., // s(x)
            +1., +1., -5., +2., -4., +1.  // s(-x)
            ) * (1. / 6.)));

    //nf = 4
    conversionMatrices.insert(std::make_pair(4,

    NumA::MatrixD(8, 8, //
//	GPD		u(-) d(-) s(-) c(-) u-d  u-s  u-c  Sum(+)
            +1., +1., +1., +1., +2., +2., +2., +1., // u(x)
            +7., +1., +1., +1., +2., +2., +2., +1., // u(-x)
            +1., +1., +1., +1., -6., +2., +2., +1., // d(x)
            +1., -7., +1., +1., -6., +2., +2., +1., // d(-x)
            +1., +1., +1., +1., +2., -6., +2., +1., // s(x)
            +1., +1., -7., +1., +2., -6., +2., +1., // s(-x)
            +1., +1., +1., +1., +2., +2., -6., +1., // c(x)
            +1., +1., +1., -7., +2., +2., -6., +1.  // c(-x)
            ) * (1. / 8.)));

    //nf = 5
    conversionMatrices.insert(std::make_pair(5,

    NumA::MatrixD(10, 10, //
//	GPD		u(-) d(-) s(-) c(-) t(-) u-d  u-s  u-c  u-t  Sum(+)
            +1., +1., +1., +1., +1., +2., +2., +2., +2., +1., // u(x)
            -9., +1., +1., +1., +1., +2., +2., +2., +2., +1., // u(-x)
            +1., +1., +1., +1., +1., -8., +2., +2., +2., +1., // d(x)
            +1., -9., +1., +1., +1., -8., +2., +2., +2., +1., // d(-x)
            +1., +1., +1., +1., +1., +2., -8., +2., +2., +1., // s(x)
            +1., +1., -9., +1., +1., +2., -8., +2., +2., +1., // s(-x)
            +1., +1., +1., +1., +1., +2., +2., -8., +2., +1., // c(x)
            +1., +1., +1., -9., +1., +2., +2., -8., +2., +1., // c(-x)
            +1., +1., +1., +1., +1., +2., +2., +2., -8., +1., // t(x)
            +1., +1., +1., +1., -9., +2., +2., +2., -8., +1.  // t(-x)
            ) * (1. / 10.)));

    //nf = 6
    conversionMatrices.insert(std::make_pair(6,

    NumA::MatrixD(12, 12, //
//	GPD		u(-) d(-) s(-) c(-) t(-) b(-) u-d  u-s  u-c  u-t  u-b  Sum(+)
            +1., +1., +1., +1., +1., +1., +2., +2., +2., +2., +2., +1., // u(x)
            -11., +1., +1., +1., +1., +1., +2., +2., +2., +2., +2., +1., // u(-x)
            +1., +1., +1., +1., +1., +1., -10., +2., +2., +2., +2., +1., // d(x)
            +1., -11., +1., +1., +1., +1., -10., +2., +2., +2., +2., +1., // d(-x)
            +1., +1., +1., +1., +1., +1., +2., -10., +2., +2., +2., +1., // s(x)
            +1., +1., -11., +1., +1., +1., +2., -10., +2., +2., +2., +1., // s(-x)
            +1., +1., +1., +1., +1., +1., +2., +2., -10., +2., +2., +1., // c(x)
            +1., +1., +1., -11., +1., +1., +2., +2., -10., +2., +2., +1., // c(-x)
            +1., +1., +1., +1., +1., +1., +2., +2., +2., -10., +2., +1., // t(x)
            +1., +1., +1., +1., -11., +1., +2., +2., +2., -10., +2., +1., // t(-x)
            +1., +1., +1., +1., +1., +1., +2., +2., +2., +2., -10., +1., // b(x)
            +1., +1., +1., +1., +1., -11., +2., +2., +2., +2., -10., +1. // b(-x)
            ) * (1. / 12.)));

    //init functors
    initFunctorsForIntegrations();

    //clear vectors
    clearVectors(true);
}

GPDEvolutionVinnikov::GPDEvolutionVinnikov(const GPDEvolutionVinnikov& other) :
        GPDEvolutionModule(other), MathIntegratorModule(other), m_gridSize(
                other.m_gridSize), m_xiLast(other.m_xiLast), m_tLast(
                other.m_tLast), m_MuR2Last(other.m_MuR2Last) {

    //copy vectors
    m_NS = other.m_NS;
    m_S = other.m_S;
    m_G = other.m_G;

    m_NSDiff = other.m_NSDiff;
    m_SDiff = other.m_SDiff;
    m_GDiff = other.m_GDiff;

    m_NSEvolved = other.m_NSEvolved;
    m_SEvolved = other.m_SEvolved;
    m_GEvolved = other.m_GEvolved;

    m_NSEvoledForFirstTime = other.m_NSEvoledForFirstTime;
    m_SEvoledForFirstTime = other.m_SEvoledForFirstTime;
    m_GEvoledForFirstTime = other.m_GEvoledForFirstTime;

    m_NSResultLast = other.m_NSResultLast;
    m_SResultLast = other.m_SResultLast;
    m_GResultLast = other.m_GResultLast;

    m_NSMuF2Last = other.m_NSMuF2Last;
    m_SMuF2Last = other.m_SMuF2Last;
    m_GMuF2Last = other.m_GMuF2Last;

    m_nFlavors = other.m_nFlavors;
    m_nFlavors_ref = other.m_nFlavors_ref;

    m_pGPDModule = other.m_pGPDModule;

    m_NSXGrid = other.m_NSXGrid;
    m_SXGrid = other.m_SXGrid;

    conversionMatrices = other.conversionMatrices;

    m_PartonDistributionAtMuF2_ref = other.m_PartonDistributionAtMuF2_ref;

    //init functors
    initFunctorsForIntegrations();
}

GPDEvolutionVinnikov::~GPDEvolutionVinnikov() {

    if (p_intIntegrateComputeOutputNS) {
        delete p_intIntegrateComputeOutputNS;
        p_intIntegrateComputeOutputNS = 0;
    }

    if (p_intIntegrateComputeOutputS) {
        delete p_intIntegrateComputeOutputS;
        p_intIntegrateComputeOutputS = 0;
    }

    if (p_intIntegrateComputeOutputG) {
        delete p_intIntegrateComputeOutputG;
        p_intIntegrateComputeOutputG = 0;
    }
}

GPDEvolutionVinnikov* GPDEvolutionVinnikov::clone() const {
    return new GPDEvolutionVinnikov(*this);
}

void GPDEvolutionVinnikov::configure(const ElemUtils::Parameters& parameters) {

    //run for mother class
    GPDEvolutionModule::configure(parameters);

    MathIntegratorModule::configureIntegrator(parameters);

    //check and set
    if (parameters.isAvailable(GPDEvolutionVinnikov::PARAM_NAME_GRID_SIZE)) {
        setGridSize(parameters.getLastAvailable().toUInt());
    }
}

void GPDEvolutionVinnikov::resolveObjectDependencies() {
    GPDEvolutionModule::resolveObjectDependencies();

    //set integration type
    setIntegrator(NumA::IntegratorType1D::TRAPEZOIDALLOG);
}

void GPDEvolutionVinnikov::isModuleWellConfigured() {

    if (m_pertOrder == PerturbativeQCDOrderType::UNDEFINED) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "pQCD order is UNDEFINED");
    }

    if (!m_pRunningAlphaStrongModule) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Pointer to RunningAlphaStrong module is NULL");
    }

    if (!m_pActiveFlavorsModule) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Pointer to ActiveFlavorsModule module is NULL");
    }

    if (m_x < -1. || m_x > 1.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "x is out of range: " << m_x);
    }

    if (m_xi < 0. || m_xi > 1.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "xi is out of range: " << m_xi);
    }

    if (m_t > 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "t is out of range: " << m_t);
    }

    if (m_MuF2 <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "muF2 is out of range: " << m_MuF2);
    }

    if (m_MuR2 <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "muR2 is out of range:" << m_MuR2);
    }
}

void GPDEvolutionVinnikov::initModule() {

    //run for mother class
    GPDEvolutionModule::initModule();

    //evaluate nf and muF2 and muF2_ref
    m_nFlavors =
            (m_pActiveFlavorsModule) ?
                    ((m_pActiveFlavorsModule->getNfInterval(m_MuF2)).getNf()) :
                    (0);

    if (m_nFlavors < 1 || m_nFlavors > 6) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "nFlavors at muF2 is out of range: "
                        << m_nFlavors);
    }
}

PartonDistribution GPDEvolutionVinnikov::compute(GPDModule* pGPDModule,
        const GPDType::Type &type) {

    //pre-compute (set internal variables, etc.)
    m_pGPDModule = pGPDModule;
    m_MuF2_ref = (m_pGPDModule) ? (m_pGPDModule->getMuF2Ref()) : (0.);
    m_type = type;
    m_PartonDistributionAtMuF2_ref = m_pGPDModule->compute(
            GPDKinematic(m_x, m_xi, m_t, m_MuF2_ref, m_MuR2), m_type);

    //initialize
    m_nFlavors_ref =
            (m_pActiveFlavorsModule) ?
                    ((m_pActiveFlavorsModule->getNfInterval(m_MuF2_ref)).getNf()) :
                    (0);

    if (m_nFlavors_ref < 1 || m_nFlavors_ref > 6) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "nFlavors at muF2_ref is out of range:"
                        << m_nFlavors_ref);
    }

    //check if last kinematics the same
    bool lastKinematicsDifferent = !((m_xi == m_xiLast) && (m_t == m_tLast)
            && (m_MuR2 == m_MuR2Last));

    //clear vectors
    clearVectors(lastKinematicsDifferent);

    if (lastKinematicsDifferent) {

        //set grids
        m_NSXGrid = std::vector<double>(4 * m_gridSize + 1, 0.);
        m_SXGrid = std::vector<double>(2 * m_gridSize + 1, 0.);

        //populate grids
        gpd_grid_ns(m_gridSize, m_xi, m_NSXGrid);
        gpd_grid_s(m_gridSize, m_xi, m_SXGrid);

        //check if grids valid
        checkIfGridsValid();

        //iterators
        std::map<QuarkNonSingletCombination::Type, double>::iterator itNSSingle;
        std::map<QuarkNonSingletCombination::Type, std::vector<double> >::iterator itNS;
        //compute
        for (size_t iGrid = 0; iGrid <= 4 * m_gridSize; iGrid++) {
            // create kinematics to compute
            GPDKinematic gpdKinematic(m_NSXGrid.at(iGrid), m_xi, m_t,
                    m_MuF2_ref, m_MuR2);

            //compute
            PartonDistribution partonDistribution = m_pGPDModule->compute(
                    gpdKinematic, m_type);

            //get non-singlet combinations
            std::map<QuarkNonSingletCombination::Type, double> NS = getNS(
                    m_nFlavors_ref, partonDistribution);

            for (itNSSingle = NS.begin(); itNSSingle != NS.end();
                    itNSSingle++) {

                //check if valid
                checkIfResultValid(m_NSXGrid.at(iGrid), itNSSingle->second);

                //set m_NS
                itNS = m_NS.find(itNSSingle->first);

                if (itNS == m_NS.end()) {
                    throw ElemUtils::CustomException(getClassName(), __func__,
                            ElemUtils::Formatter() << "No key "
                                    << QuarkNonSingletCombination(
                                            itNSSingle->first).toString()
                                    << " in map m_NS");
                }

                (itNS->second).push_back(itNSSingle->second);
            }

            if (iGrid >= 2 * m_gridSize) {

                //get single ad gluon
                double S = getS(m_nFlavors_ref, partonDistribution);
                double G = getG(partonDistribution);

                //check if valid
                checkIfResultValid(m_NSXGrid.at(iGrid), S);
                checkIfResultValid(m_NSXGrid.at(iGrid), G);

                if (iGrid >= 2 * m_gridSize) {

                    //get singlet combination and gluons
                    m_S.push_back(S);
                    m_G.push_back(G);
                }
            }
        }
    }

    //iterators
    std::map<QuarkNonSingletCombination::Type, std::vector<double> >::iterator itNSDiff;
    std::map<QuarkNonSingletCombination::Type, bool>::iterator itNSEvoledForFirstTime;

    //set grids
    for (itNSDiff = m_NSDiff.begin(); itNSDiff != m_NSDiff.end(); itNSDiff++) {
        itNSDiff->second = std::vector<double>(4 * m_gridSize + 1, 0.);
    }
    m_SDiff = std::vector<double>(2 * m_gridSize + 1, 0.);
    m_GDiff = std::vector<double>(2 * m_gridSize + 1, 0.);

    for (itNSEvoledForFirstTime = m_NSEvoledForFirstTime.begin();
            itNSEvoledForFirstTime != m_NSEvoledForFirstTime.end();
            itNSEvoledForFirstTime++) {
        itNSEvoledForFirstTime->second = true;
    }
    m_SEvoledForFirstTime = true;
    m_GEvoledForFirstTime = true;

    //check GPD type
    if (m_type != GPDType::H && m_type != GPDType::E && m_type != GPDType::Ht
            && m_type != GPDType::Et) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "This evolution module is not implemented for GPD type "
                        << GPDType(m_type).toString());
    }

    //check n flavors
    if (m_nFlavors != m_nFlavors_ref) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "This evolution module is not able to add or drop flavors because of the scale change, therefore nf at muF2 = "
                        << m_nFlavors
                        << " must be the same as that for muF2_ref = "
                        << m_nFlavors_ref);
    }

    if (m_nFlavors != 3) {
        warn(__func__,
                ElemUtils::Formatter()
                        << "This evolution module assumes explicitly nf = 3 in the kernel expressions while you are training to run it with nf = "
                        << m_nFlavors);
    }

    //variables
    std::vector<double> parameters(1,
            static_cast<double>(QuarkNonSingletCombination::UNDEFINED));
    double diff;

    //do the evolution for nf given by the target scale
    std::map<QuarkNonSingletCombination::Type, double> NSDiff;
    double SDiff, GDiff;

    if (m_nFlavors >= 1) {
        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::UP_NONSINGLET);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::UP_NONSINGLET,
                        diff));
    }

    if (m_nFlavors >= 2) {
        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::DOWN_NONSINGLET);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::DOWN_NONSINGLET,
                        diff));

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::UP_MINUS_DOWN);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_DOWN,
                        diff));
    }

    if (m_nFlavors >= 3) {

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::STRANGE_NONSINGLET);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::STRANGE_NONSINGLET,
                        diff));

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::UP_MINUS_STRANGE);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_STRANGE,
                        diff));
    }

    if (m_nFlavors >= 4) {

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::CHARM_NONSINGLET);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::CHARM_NONSINGLET,
                        diff));

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::UP_MINUS_CHARM);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_CHARM,
                        diff));
    }

    if (m_nFlavors >= 5) {

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::TOP_NONSINGLET);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::TOP_NONSINGLET,
                        diff));

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::UP_MINUS_TOP);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_TOP, diff));
    }

    if (m_nFlavors == 6) {

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::BOTTOM_NONSINGLET);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::BOTTOM_NONSINGLET,
                        diff));

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::UP_MINUS_BOTTOM);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_BOTTOM,
                        diff));
    }

    parameters.at(0) =
            static_cast<double>(QuarkNonSingletCombination::UNDEFINED);
    SDiff = integrate(p_intIntegrateComputeOutputS, m_MuF2_ref, m_MuF2,
            parameters);
    GDiff = integrate(p_intIntegrateComputeOutputG, m_MuF2_ref, m_MuF2,
            parameters);

    //apply difference (new flavors can arrive!)

    //NS, S and G for reference scale
    std::map<QuarkNonSingletCombination::Type, double> NS0 = getNS(
            m_nFlavors_ref, m_PartonDistributionAtMuF2_ref);
    double S0 = getS(m_nFlavors_ref, m_PartonDistributionAtMuF2_ref);
    double G0 = getG(m_PartonDistributionAtMuF2_ref);

    //apply (add result at reference scale to the map containing difference: after this operation the map will contain the end result)
    std::map<QuarkNonSingletCombination::Type, double>::iterator it1;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it2;

    for (it1 = NSDiff.begin(); it1 != NSDiff.end(); it1++) {

        it2 = NS0.find(it1->first);

        if (it2 != NS0.end())
            it1->second += it2->second;
    }

    SDiff += S0;
    GDiff += G0;

    //back to flavor basis and return the result
    return getPartonDistribution(m_nFlavors, NSDiff, SDiff, GDiff);
}

void GPDEvolutionVinnikov::computeMuF2DerivativeNS(double MuF2, double MuF2Last,
        QuarkNonSingletCombination::Type quarkNonSingletCombination) {

    //check pQCD order
    if (m_pertOrder != PerturbativeQCDOrderType::LO) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Evolution not implemented for pQCD order "
                        << PerturbativeQCDOrderType(m_pertOrder).toString());
    }

    //variables
    size_t i;
    std::vector<double> k1(4 * m_gridSize + 1, 0.);
    std::vector<double> k2(4 * m_gridSize + 1, 0.);
    std::vector<double> k3(4 * m_gridSize + 1, 0.);
    std::vector<double> k4(4 * m_gridSize + 1, 0.);

    std::vector<double> gpd_aux(4 * m_gridSize + 1, 0.);

    //select NS combination
    std::map<QuarkNonSingletCombination::Type, std::vector<double> >::const_iterator itEvolved =
            m_NSEvolved.find(quarkNonSingletCombination);

    if (itEvolved == m_NSEvolved.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No key "
                        << QuarkNonSingletCombination(
                                quarkNonSingletCombination).toString()
                        << " in map m_NSLast");
    }

    //get vector
    const std::vector<double>& gpd = itEvolved->second;

    //1/2PI factor
    double one_twopi = 1.0 / (2.0 * Constant::PI);

    //dt
    double dt = log(MuF2) - log(MuF2Last);

    //evolution
    for (i = 0; i <= 4 * m_gridSize; i++)
        k1.at(i) = dt * conv_int_ns(i, m_xi, m_gridSize, gpd, m_NSXGrid)
                * m_pRunningAlphaStrongModule->compute(MuF2Last) * one_twopi;
    for (i = 0; i <= 4 * m_gridSize; i++)
        gpd_aux.at(i) = gpd.at(i) + 0.5 * k1.at(i);
    for (i = 0; i <= 4 * m_gridSize; i++)
        k2.at(i) = dt * conv_int_ns(i, m_xi, m_gridSize, gpd_aux, m_NSXGrid)
                * m_pRunningAlphaStrongModule->compute(
                        exp(log(MuF2Last) + 0.5 * dt)) * one_twopi;
    for (i = 0; i <= 4 * m_gridSize; i++)
        gpd_aux.at(i) = gpd.at(i) + 0.5 * k2.at(i);
    for (i = 0; i <= 4 * m_gridSize; i++)
        k3.at(i) = dt * conv_int_ns(i, m_xi, m_gridSize, gpd_aux, m_NSXGrid)
                * m_pRunningAlphaStrongModule->compute(
                        exp(log(MuF2Last) + 0.5 * dt)) * one_twopi;
    for (i = 0; i <= 4 * m_gridSize; i++)
        gpd_aux.at(i) = gpd.at(i) + k3.at(i);
    for (i = 0; i <= 4 * m_gridSize; i++)
        k4.at(i) = dt * conv_int_ns(i, m_xi, m_gridSize, gpd_aux, m_NSXGrid)
                * m_pRunningAlphaStrongModule->compute(MuF2) * one_twopi;

    //apply
    std::map<QuarkNonSingletCombination::Type, std::vector<double> >::iterator itDiff =
            m_NSDiff.find(quarkNonSingletCombination);

    if (itDiff == m_NSDiff.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No key "
                        << QuarkNonSingletCombination(
                                quarkNonSingletCombination).toString()
                        << " in map m_NSDiff");
    }

    for (i = 0; i <= 4 * m_gridSize; i++)
        (itDiff->second).at(i) = (k1.at(i) + 2.0 * k2.at(i) + 2.0 * k3.at(i)
                + k4.at(i)) / 6.0;
}

void GPDEvolutionVinnikov::computeMuF2DerivativeSG(double MuF2,
        double MuF2Last) {

    //check pQCD order
    if (m_pertOrder != PerturbativeQCDOrderType::LO) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Evolution not implemented for pQCD order "
                        << PerturbativeQCDOrderType(m_pertOrder).toString());
    }

    //functions
    double (GPDEvolutionVinnikov::*qq)(int const ix, double const xi,
            int const n, const std::vector<double>& gpd,
            const std::vector<double>& x);
    double (GPDEvolutionVinnikov::*gg)(int const ix, double const xi,
            int const n, const std::vector<double>& gpd,
            const std::vector<double>& x);
    double (GPDEvolutionVinnikov::*gq)(int const ix, double const xi,
            int const n, const std::vector<double>& gpd,
            const std::vector<double>& x);
    double (GPDEvolutionVinnikov::*qg)(int const ix, double const xi,
            int const n, const std::vector<double>& gpd,
            const std::vector<double>& x);

    if (m_type == GPDType::H || m_type == GPDType::E) {

        qq = &GPDEvolutionVinnikov::conv_int_qq;
        gg = &GPDEvolutionVinnikov::conv_int_gg;
        gq = &GPDEvolutionVinnikov::conv_int_gq;
        qg = &GPDEvolutionVinnikov::conv_int_qg;
    } else if (m_type == GPDType::Ht || m_type == GPDType::Et) {

        qq = &GPDEvolutionVinnikov::conv_int_qq_pol;
        gg = &GPDEvolutionVinnikov::conv_int_gg_pol;
        gq = &GPDEvolutionVinnikov::conv_int_gq_pol;
        qg = &GPDEvolutionVinnikov::conv_int_qg_pol;
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Evolution not implemented for GPD type "
                        << GPDType(m_type).toString());
    }

    //variables
    size_t i;
    std::vector<double> k1_q(2 * m_gridSize + 1, 0.);
    std::vector<double> k2_q(2 * m_gridSize + 1, 0.);
    std::vector<double> k3_q(2 * m_gridSize + 1, 0.);
    std::vector<double> k4_q(2 * m_gridSize + 1, 0.);
    std::vector<double> k1_g(2 * m_gridSize + 1, 0.);
    std::vector<double> k2_g(2 * m_gridSize + 1, 0.);
    std::vector<double> k3_g(2 * m_gridSize + 1, 0.);
    std::vector<double> k4_g(2 * m_gridSize + 1, 0.);

    std::vector<double> gpd_q_aux(2 * m_gridSize + 1, 0.);
    std::vector<double> gpd_g_aux(2 * m_gridSize + 1, 0.);

    //1/2PI factor
    double one_twopi = 1.0 / (2.0 * Constant::PI);

    //dt
    double dt = log(MuF2) - log(MuF2Last);

    //evolution
    for (i = 0; i <= 2 * m_gridSize; i++) {
        k1_q.at(i) =
                dt
                        * ((this->*qq)(i, m_xi, m_gridSize, m_SEvolved,
                                m_SXGrid)
                                + (this->*qg)(i, m_xi, m_gridSize, m_GEvolved,
                                        m_SXGrid))
                        * m_pRunningAlphaStrongModule->compute(MuF2Last)
                        * one_twopi;
        k1_g.at(i) =
                dt
                        * ((this->*gq)(i, m_xi, m_gridSize, m_SEvolved,
                                m_SXGrid)
                                + (this->*gg)(i, m_xi, m_gridSize, m_GEvolved,
                                        m_SXGrid))
                        * m_pRunningAlphaStrongModule->compute(MuF2Last)
                        * one_twopi;
    }
    for (i = 0; i <= 2 * m_gridSize; i++) {
        gpd_q_aux.at(i) = m_SEvolved.at(i) + 0.5 * k1_q.at(i);
        gpd_g_aux.at(i) = m_GEvolved.at(i) + 0.5 * k1_g.at(i);
    }
    for (i = 0; i <= 2 * m_gridSize; i++) {
        k2_q.at(i) = dt
                * ((this->*qq)(i, m_xi, m_gridSize, gpd_q_aux, m_SXGrid)
                        + (this->*qg)(i, m_xi, m_gridSize, gpd_g_aux, m_SXGrid))
                * m_pRunningAlphaStrongModule->compute(
                        exp(log(MuF2Last) + 0.5 * dt)) * one_twopi;
        k2_g.at(i) = dt
                * ((this->*gq)(i, m_xi, m_gridSize, gpd_q_aux, m_SXGrid)
                        + (this->*gg)(i, m_xi, m_gridSize, gpd_g_aux, m_SXGrid))
                * m_pRunningAlphaStrongModule->compute(
                        exp(log(MuF2Last) + 0.5 * dt)) * one_twopi;
    }
    for (i = 0; i <= 2 * m_gridSize; i++) {
        gpd_q_aux.at(i) = m_SEvolved.at(i) + 0.5 * k2_q.at(i);
        gpd_g_aux.at(i) = m_GEvolved.at(i) + 0.5 * k2_g.at(i);
    }
    for (i = 0; i <= 2 * m_gridSize; i++) {
        k3_q.at(i) = dt
                * ((this->*qq)(i, m_xi, m_gridSize, gpd_q_aux, m_SXGrid)
                        + (this->*qg)(i, m_xi, m_gridSize, gpd_g_aux, m_SXGrid))
                * m_pRunningAlphaStrongModule->compute(
                        exp(log(MuF2Last) + 0.5 * dt)) * one_twopi;
        k3_g.at(i) = dt
                * ((this->*gq)(i, m_xi, m_gridSize, gpd_q_aux, m_SXGrid)
                        + (this->*gg)(i, m_xi, m_gridSize, gpd_g_aux, m_SXGrid))
                * m_pRunningAlphaStrongModule->compute(
                        exp(log(MuF2Last) + 0.5 * dt)) * one_twopi;
    }
    for (i = 0; i <= 2 * m_gridSize; i++) {
        gpd_q_aux.at(i) = m_SEvolved.at(i) + k3_q.at(i);
        gpd_g_aux.at(i) = m_GEvolved.at(i) + k3_g.at(i);
    }
    for (i = 0; i <= 2 * m_gridSize; i++) {
        k4_q.at(i) = dt
                * ((this->*qq)(i, m_xi, m_gridSize, gpd_q_aux, m_SXGrid)
                        + (this->*qg)(i, m_xi, m_gridSize, gpd_g_aux, m_SXGrid))
                * m_pRunningAlphaStrongModule->compute(MuF2) * one_twopi;
        k4_g.at(i) = dt
                * ((this->*gq)(i, m_xi, m_gridSize, gpd_q_aux, m_SXGrid)
                        + (this->*gg)(i, m_xi, m_gridSize, gpd_g_aux, m_SXGrid))
                * m_pRunningAlphaStrongModule->compute(MuF2) * one_twopi;
    }

    //apply
    for (i = 0; i <= 2 * m_gridSize; i++) {
        m_SDiff.at(i) = (k1_q.at(i) + 2.0 * k2_q.at(i) + 2.0 * k3_q.at(i)
                + k4_q.at(i)) / 6.0;
        m_GDiff.at(i) = (k1_g.at(i) + 2.0 * k2_g.at(i) + 2.0 * k3_g.at(i)
                + k4_g.at(i)) / 6.0;
    }
}

double GPDEvolutionVinnikov::nonSingletMu2FDerivative(double MuF2,
        QuarkNonSingletCombination::Type quarkNonSingletCombination) {

    //check n flavors
    size_t nFlavors = (m_pActiveFlavorsModule->getNfInterval(MuF2)).getNf();

    if (nFlavors != m_nFlavors || nFlavors != m_nFlavors_ref) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "nFlavors at current muF2, "
                        << m_nFlavors << "at " << MuF2
                        << ", must be the same as that for muF2 and muF2_ref, "
                        << m_nFlavors_ref);
    }

    //set alpha_s
    m_pRunningAlphaStrongModule->setNf(nFlavors);

    //iterators
    std::map<QuarkNonSingletCombination::Type, bool>::iterator itEvoledForFirstTime =
            m_NSEvoledForFirstTime.find(quarkNonSingletCombination);

    if (itEvoledForFirstTime == m_NSEvoledForFirstTime.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No key "
                        << QuarkNonSingletCombination(
                                quarkNonSingletCombination).toString()
                        << " in map m_NSEvoledForFirstTime");
    }

    std::map<QuarkNonSingletCombination::Type, double>::iterator itResultLast =
            m_NSResultLast.find(quarkNonSingletCombination);

    if (itResultLast == m_NSResultLast.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No key "
                        << QuarkNonSingletCombination(
                                quarkNonSingletCombination).toString()
                        << " in map m_NSResultLast");
    }

    std::map<QuarkNonSingletCombination::Type, double>::iterator itMuF2Last =
            m_NSMuF2Last.find(quarkNonSingletCombination);

    if (itMuF2Last == m_NSMuF2Last.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No key "
                        << QuarkNonSingletCombination(
                                quarkNonSingletCombination).toString()
                        << " in map m_NSMuF2Last");
    }

    //check if not first point
    if (itEvoledForFirstTime->second) {

        //check
        if (MuF2 != m_MuF2_ref) {
            warn(__func__,
                    ElemUtils::Formatter()
                            << "First evaluated value different than the reference scale, muF2_first: "
                            << MuF2 << ", muF2_ref: " << m_MuF2_ref);
        }

        //set initial
        m_NSEvolved = m_NS;

        itResultLast->second = 0.;
        itMuF2Last->second = m_MuF2_ref;

        itEvoledForFirstTime->second = false;

        return 0.;
    }

    //check if direction did not change
    checkEvolutionDirection(MuF2, itMuF2Last->second);

    //run evolution
    computeMuF2DerivativeNS(MuF2, itMuF2Last->second,
            quarkNonSingletCombination);

    //iterators
    std::map<QuarkNonSingletCombination::Type, std::vector<double> >::const_iterator itDiff =
            m_NSDiff.find(quarkNonSingletCombination);

    if (itDiff == m_NSDiff.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No key "
                        << QuarkNonSingletCombination(
                                quarkNonSingletCombination).toString()
                        << " in map m_NSDiff");

    }

    std::map<QuarkNonSingletCombination::Type, std::vector<double> >::iterator itEvolved =
            m_NSEvolved.find(quarkNonSingletCombination);

    if (itEvolved == m_NSEvolved.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No key "
                        << QuarkNonSingletCombination(
                                quarkNonSingletCombination).toString()
                        << " in map m_NSLast");

    }

    //apply to last
    for (size_t i = 0; i <= 4 * m_gridSize; i++)
        (itEvolved->second).at(i) += (itDiff->second).at(i);

    //set and calculate spline
    NumA::CubicSpline spline(m_NSXGrid, itDiff->second);
    spline.ConstructSpline();

    double result = spline.getSplineInsideValue(m_x);

    //differential result
    double resultDiff = result * 2. / (MuF2 - itMuF2Last->second)
            - itResultLast->second;

    //set last
    itResultLast->second = resultDiff;
    itMuF2Last->second = MuF2;

    //return
    return resultDiff;
}

double GPDEvolutionVinnikov::singletMuF2Derivative(double MuF2) {

    //check n flavors
    size_t nFlavors = (m_pActiveFlavorsModule->getNfInterval(MuF2)).getNf();

    if (nFlavors != m_nFlavors || nFlavors != m_nFlavors_ref) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "nFlavors at current muF2, "
                        << m_nFlavors << "at " << MuF2
                        << ", must be the same as that for muF2 and muF2_ref, "
                        << m_nFlavors_ref);
    }

    //set alpha_s
    m_pRunningAlphaStrongModule->setNf(nFlavors);

    //check if not first point
    if (m_SEvoledForFirstTime) {

        //check
        if (MuF2 != m_MuF2_ref) {
            warn(__func__,
                    ElemUtils::Formatter()
                            << "First evaluated value different than the reference scale, muF2_first: "
                            << MuF2 << ", muF2_ref: " << m_MuF2_ref);
        }

        //set initial
        m_SEvolved = m_S;
        m_GEvolved = m_G;

        m_SResultLast = 0.;
        m_SMuF2Last = m_MuF2_ref;

        m_SEvoledForFirstTime = false;

        return 0.;
    }

    //check if direction did not change
    checkEvolutionDirection(MuF2, m_SMuF2Last);

    //run evolution
    computeMuF2DerivativeSG(MuF2, m_SMuF2Last);

    //apply to last
    for (size_t i = 0; i <= 2 * m_gridSize; i++) {
        m_SEvolved.at(i) += m_SDiff.at(i);
        m_GEvolved.at(i) += m_GDiff.at(i);
    }

    //set and calculate spline
    NumA::CubicSpline spline(m_SXGrid, m_SDiff);
    spline.ConstructSpline();

    double result = spline.getSplineInsideValue(fabs(m_x));

    //symmetry property
    if (m_x < 0.) {
        if (m_type == GPDType::H || m_type == GPDType::E) {
            result *= -1;
        } else if (m_type == GPDType::Ht || m_type == GPDType::Et) {
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Evolution not implemented for GPD type "
                            << GPDType(m_type).toString());
        }
    }

    //differential result
    double resultDiff = result * 2. / (MuF2 - m_SMuF2Last) - m_SResultLast;

    //set last
    m_SResultLast = resultDiff;
    m_SMuF2Last = MuF2;

    //return
    return resultDiff;
}

double GPDEvolutionVinnikov::gluonMuF2Derivative(double MuF2) {

    //check n flavors
    size_t nFlavors = (m_pActiveFlavorsModule->getNfInterval(MuF2)).getNf();

    if (nFlavors != m_nFlavors || nFlavors != m_nFlavors_ref) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "nFlavors at current muF2, "
                        << m_nFlavors << "at " << MuF2
                        << ", must be the same as that for muF2 and muF2_ref, "
                        << m_nFlavors_ref);
    }

    //set alpha_s
    m_pRunningAlphaStrongModule->setNf(nFlavors);

    //check if not first point
    if (m_GEvoledForFirstTime) {

        //check
        if (MuF2 != m_MuF2_ref) {
            warn(__func__,
                    ElemUtils::Formatter()
                            << "First evaluated value different than the reference scale, muF2_first: "
                            << MuF2 << ", muF2_ref: " << m_MuF2_ref);
        }

        //set initial
        m_SEvolved = m_S;
        m_GEvolved = m_G;

        m_GResultLast = 0.;
        m_GMuF2Last = m_MuF2_ref;

        m_GEvoledForFirstTime = false;

        return 0.;
    }

    //check if direction did not change
    checkEvolutionDirection(MuF2, m_GMuF2Last);

    //run evolution
    computeMuF2DerivativeSG(MuF2, m_GMuF2Last);

    //apply to last
    for (size_t i = 0; i <= 2 * m_gridSize; i++) {
        m_SEvolved.at(i) += m_SDiff.at(i);
        m_GEvolved.at(i) += m_GDiff.at(i);
    }

    //set and calculate spline
    NumA::CubicSpline spline(m_SXGrid, m_GDiff);
    spline.ConstructSpline();

    double result = spline.getSplineInsideValue(fabs(m_x));

    //symmetry property
    if (m_x < 0.) {
        if (m_type == GPDType::H || m_type == GPDType::E) {
        } else if (m_type == GPDType::Ht || m_type == GPDType::Et) {
            result *= -1;
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Evolution not implemented for GPD type "
                            << GPDType(m_type).toString());
        }
    }

    //differential result
    double resultDiff = result * 2. / (MuF2 - m_GMuF2Last) - m_GResultLast;

    //set last
    m_GResultLast = resultDiff;
    m_GMuF2Last = MuF2;

    //evaluate and save
    return resultDiff;
}

void GPDEvolutionVinnikov::checkIfGridsValid() {

    //non-singlet case
    if (m_NSXGrid.at(0) < -1.) {

        if (m_NSXGrid.at(1) < -1.) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "Two first elements of x table outside allowed range (non-singlet case)");
        } else {
            m_NSXGrid.at(0) = -1.;
            warn(__func__,
                    "Invalid value of first element in x table corrected (non-singlet case)");
        }
    }

    if (m_NSXGrid.at(4 * m_gridSize) > 1.) {

        if (m_NSXGrid.at(4 * m_gridSize - 1) > 1.) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "Two last elements of x table outside allowed range (non-singlet case)");
        } else {
            m_NSXGrid.at(4 * m_gridSize) = 1.;
            warn(__func__,
                    "Invalid value of last element in x table corrected (non-singlet case)");
        }
    }

    //singlet (and gluons) case
    if (m_SXGrid.at(0) < 0.) {

        if (m_SXGrid.at(1) < 0.) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "Two first elements of x table outside allowed range (singlet case)");
        } else {
            m_SXGrid.at(0) = 0.;
            warn(__func__,
                    "Invalid value of first element in x table corrected (singlet case)");
        }
    }

    if (m_SXGrid.at(2 * m_gridSize) > 1.) {

        if (m_SXGrid.at(2 * m_gridSize - 1) > 1.) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "Two last elements of x table outside allowed range (singlet case)");
        } else {
            m_SXGrid.at(2 * m_gridSize) = 1.;
            warn(__func__,
                    "Invalid value of last element in x table corrected (singlet case)");
        }
    }
}

void GPDEvolutionVinnikov::checkIfResultValid(const double x, double& value) {

    if (std::isnan(value)) {

        if (fabs(x) == 1.) {
            warn(__func__,
                    ElemUtils::Formatter() << "NaN for x = " << x
                            << " due to numerical issues, set 0. instead");
            value = 0.;
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "NaN for x = " << x);
        }
    }
}

void GPDEvolutionVinnikov::clearVectors(bool lastKinematicsDifferent) {

    //vector of combinations
    std::vector<QuarkNonSingletCombination::Type> NS;
    std::vector<QuarkNonSingletCombination::Type>::const_iterator itNS;

    NS.push_back(QuarkNonSingletCombination::UP_NONSINGLET);
    NS.push_back(QuarkNonSingletCombination::DOWN_NONSINGLET);
    NS.push_back(QuarkNonSingletCombination::STRANGE_NONSINGLET);
    NS.push_back(QuarkNonSingletCombination::CHARM_NONSINGLET);
    NS.push_back(QuarkNonSingletCombination::TOP_NONSINGLET);
    NS.push_back(QuarkNonSingletCombination::BOTTOM_NONSINGLET);
    NS.push_back(QuarkNonSingletCombination::UP_MINUS_DOWN);
    NS.push_back(QuarkNonSingletCombination::UP_MINUS_STRANGE);
    NS.push_back(QuarkNonSingletCombination::UP_MINUS_CHARM);
    NS.push_back(QuarkNonSingletCombination::UP_MINUS_TOP);
    NS.push_back(QuarkNonSingletCombination::UP_MINUS_BOTTOM);

    //clear
    if (lastKinematicsDifferent) {
        m_NS.clear();
        m_S.clear();
        m_G.clear();
    }

    m_NSDiff.clear();
    m_SDiff.clear();
    m_GDiff.clear();

    m_NSEvolved.clear();
    m_SEvolved.clear();
    m_GEvolved.clear();

    m_NSEvoledForFirstTime.clear();
    m_SEvoledForFirstTime = true;
    m_GEvoledForFirstTime = true;

    m_NSResultLast.clear();
    m_SResultLast = 0.;
    m_GResultLast = 0.;

    m_NSMuF2Last.clear();
    m_SMuF2Last = 0.;
    m_GMuF2Last = 0.;

    if (lastKinematicsDifferent) {
        m_NSXGrid.clear();
        m_SXGrid.clear();
    }

    //set
    for (itNS = NS.begin(); itNS != NS.end(); itNS++) {

        if (lastKinematicsDifferent) {
            m_NS.insert(std::make_pair(*itNS, std::vector<double>()));
        }
        m_NSDiff.insert(std::make_pair(*itNS, std::vector<double>()));
        m_NSEvolved.insert(std::make_pair(*itNS, std::vector<double>()));
        m_NSEvoledForFirstTime.insert(std::make_pair(*itNS, true));
        m_NSResultLast.insert(std::make_pair(*itNS, 0.));
        m_NSMuF2Last.insert(std::make_pair(*itNS, 0.));
    }
}

void GPDEvolutionVinnikov::checkEvolutionDirection(const double MuF2,
        const double MuF2Last) const {

    if (m_MuF2 > m_MuF2_ref) {

        if (MuF2 < MuF2Last) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Integration is not continuous (it changed direction). Try another integration routine. muF2_ref: "
                            << m_MuF2_ref << " muF2_target: " << m_MuF2
                            << " muF2_last: " << MuF2Last << " muF2_current: "
                            << MuF2);
        }

    } else {

        if (MuF2 > MuF2Last) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Integration is not continuous (it changed direction). Try another integration routine. muF2_ref: "
                            << m_MuF2_ref << " muF2_target: " << m_MuF2
                            << " muF2_last: " << MuF2Last << " muF2_current: "
                            << MuF2);
        }

    }
}

size_t GPDEvolutionVinnikov::getGridSize() const {
    return m_gridSize;
}

void GPDEvolutionVinnikov::setGridSize(size_t gridSize) {

    //info
    info(__func__,
            ElemUtils::Formatter() << "Grid size changed from " << m_gridSize
                    << " to " << gridSize);

    //set
    m_gridSize = gridSize;
}

std::map<QuarkNonSingletCombination::Type, double> GPDEvolutionVinnikov::getNS(
        const size_t nFlavors,
        const PartonDistribution& partonDistribution) const {

    //check if nf is right
    if (nFlavors < 1 || nFlavors > 6) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Illegal number of flavors: "
                        << nFlavors);
    }

    //result
    std::map<QuarkNonSingletCombination::Type, double> result;

    //reference to quark content
    const std::map<QuarkFlavor::Type, QuarkDistribution>& quarkContent =
            partonDistribution.getQuarkDistributions();

    //iterators
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it_u;
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it_d;
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it_s;
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it_c;
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it_t;
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it_b;

    //search and insert
    if (nFlavors >= 1) {

        it_u = findInQuarkFlavorMap(quarkContent, QuarkFlavor::UP,
                "quarkContent");

        result.insert(
                std::make_pair(QuarkNonSingletCombination::UP_NONSINGLET,
                        (it_u->second).getQuarkDistributionMinus()));
    }

    if (nFlavors >= 2) {

        it_d = findInQuarkFlavorMap(quarkContent, QuarkFlavor::DOWN,
                "quarkContent");

        result.insert(
                std::make_pair(QuarkNonSingletCombination::DOWN_NONSINGLET,
                        (it_d->second).getQuarkDistributionMinus()));
        result.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_DOWN,
                        (it_u->second).getQuarkDistribution()
                                - (it_d->second).getQuarkDistribution()));
    }

    if (nFlavors >= 3) {

        it_s = findInQuarkFlavorMap(quarkContent, QuarkFlavor::STRANGE,
                "quarkContent");

        result.insert(
                std::make_pair(QuarkNonSingletCombination::STRANGE_NONSINGLET,
                        (it_s->second).getQuarkDistributionMinus()));
        result.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_STRANGE,
                        (it_u->second).getQuarkDistribution()
                                - (it_s->second).getQuarkDistribution()));
    }

    if (nFlavors >= 4) {

        it_c = findInQuarkFlavorMap(quarkContent, QuarkFlavor::CHARM,
                "quarkContent");

        result.insert(
                std::make_pair(QuarkNonSingletCombination::CHARM_NONSINGLET,
                        (it_c->second).getQuarkDistributionMinus()));
        result.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_CHARM,
                        (it_u->second).getQuarkDistribution()
                                - (it_c->second).getQuarkDistribution()));
    }

    if (nFlavors >= 5) {

        it_t = findInQuarkFlavorMap(quarkContent, QuarkFlavor::TOP,
                "quarkContent");

        result.insert(
                std::make_pair(QuarkNonSingletCombination::TOP_NONSINGLET,
                        (it_t->second).getQuarkDistributionMinus()));
        result.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_TOP,
                        (it_u->second).getQuarkDistribution()
                                - (it_t->second).getQuarkDistribution()));
    }

    if (nFlavors == 6) {

        it_b = findInQuarkFlavorMap(quarkContent, QuarkFlavor::BOTTOM,
                "quarkContent");

        result.insert(
                std::make_pair(QuarkNonSingletCombination::BOTTOM_NONSINGLET,
                        (it_b->second).getQuarkDistributionMinus()));
        result.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_BOTTOM,
                        (it_u->second).getQuarkDistribution()
                                - (it_b->second).getQuarkDistribution()));
    }

    //return
    return result;
}

double GPDEvolutionVinnikov::getS(const size_t nFlavors,
        const PartonDistribution& partonDistribution) const {

    //check if nf is right
    if (nFlavors < 1 || nFlavors > 6) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Illegal number of flavors: "
                        << nFlavors);
    }

    //result
    double result = 0.;

    //reference to quark content
    const std::map<QuarkFlavor::Type, QuarkDistribution>& quarkContent =
            partonDistribution.getQuarkDistributions();

    //iterator
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it;

    //search and insert
    if (nFlavors >= 1) {

        it = findInQuarkFlavorMap(quarkContent, QuarkFlavor::UP,
                "quarkContent");
        result += (it->second).getQuarkDistributionPlus();
    }

    if (nFlavors >= 2) {

        it = findInQuarkFlavorMap(quarkContent, QuarkFlavor::DOWN,
                "quarkContent");
        result += (it->second).getQuarkDistributionPlus();
    }

    if (nFlavors >= 3) {

        it = findInQuarkFlavorMap(quarkContent, QuarkFlavor::STRANGE,
                "quarkContent");
        result += (it->second).getQuarkDistributionPlus();
    }

    if (nFlavors >= 4) {

        it = findInQuarkFlavorMap(quarkContent, QuarkFlavor::CHARM,
                "quarkContent");
        result += (it->second).getQuarkDistributionPlus();
    }

    if (nFlavors >= 5) {

        it = findInQuarkFlavorMap(quarkContent, QuarkFlavor::TOP,
                "quarkContent");
        result += (it->second).getQuarkDistributionPlus();
    }

    if (nFlavors == 6) {

        it = findInQuarkFlavorMap(quarkContent, QuarkFlavor::BOTTOM,
                "quarkContent");
        result += (it->second).getQuarkDistributionPlus();
    }

    //result
    return result;
}

double GPDEvolutionVinnikov::getG(
        const PartonDistribution& partonDistribution) const {
    return partonDistribution.getGluonDistribution().getGluonDistribution();
}

PartonDistribution GPDEvolutionVinnikov::getPartonDistribution(
        const size_t nFlavors,
        const std::map<QuarkNonSingletCombination::Type, double>& NS, double S,
        double G) const {

    //check if nf is right
    if (nFlavors < 1 || nFlavors > 6) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Illegal number of flavors: "
                        << nFlavors);
    }

    //select appropraite conversion matrix
    std::map<size_t, NumA::MatrixD>::const_iterator it =
            conversionMatrices.find(nFlavors);

    if (it == conversionMatrices.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No key " << nFlavors
                        << " in map conversionMatrices");
    }

    //result
    PartonDistribution partonDistribution;

    //values of GPDs for [0]: +x and [1]: -x
    double u[2], d[2], s[2], c[2], t[2], b[2];

    //iteratoes
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSu;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSd;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSs;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSc;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSt;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSb;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSumd;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSums;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSumc;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSumt;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSumb;

    //search
    if (nFlavors >= 1) {

        it_NSu = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::UP_NONSINGLET, "NS");
    }

    if (nFlavors >= 2) {

        it_NSd = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::DOWN_NONSINGLET, "NS");
        it_NSumd = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::UP_MINUS_DOWN, "NS");
    }

    if (nFlavors >= 3) {

        it_NSs = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::STRANGE_NONSINGLET, "NS");
        it_NSums = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::UP_MINUS_STRANGE, "NS");
    }

    if (nFlavors >= 4) {

        it_NSc = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::CHARM_NONSINGLET, "NS");
        it_NSumc = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::UP_MINUS_CHARM, "NS");
    }

    if (nFlavors >= 5) {

        it_NSt = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::TOP_NONSINGLET, "NS");
        it_NSumt = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::UP_MINUS_TOP, "NS");
    }

    if (nFlavors == 6) {

        it_NSb = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::BOTTOM_NONSINGLET, "NS");
        it_NSumb = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::UP_MINUS_BOTTOM, "NS");
    }

    //insert and evaluate
    NumA::VectorD input;

    if (nFlavors == 1) {

        input.push_back(it_NSu->second);
        input.push_back(S);

        u[0] = (it->second).getLine(0) * input;
        u[1] = (it->second).getLine(1) * input;

        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::UP, u[0], u[0] + u[1],
                        u[0] - u[1]));
    }

    if (nFlavors == 2) {

        input.push_back(it_NSu->second);
        input.push_back(it_NSd->second);
        input.push_back(it_NSumd->second);
        input.push_back(S);

        u[0] = (it->second).getLine(0) * input;
        u[1] = (it->second).getLine(1) * input;
        d[0] = (it->second).getLine(2) * input;
        d[1] = (it->second).getLine(3) * input;

        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::UP, u[0], u[0] + u[1],
                        u[0] - u[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::DOWN, d[0], d[0] + d[1],
                        d[0] - d[1]));
    }

    if (nFlavors == 3) {

        input.push_back(it_NSu->second);
        input.push_back(it_NSd->second);
        input.push_back(it_NSs->second);
        input.push_back(it_NSumd->second);
        input.push_back(it_NSums->second);
        input.push_back(S);

        u[0] = (it->second).getLine(0) * input;
        u[1] = (it->second).getLine(1) * input;
        d[0] = (it->second).getLine(2) * input;
        d[1] = (it->second).getLine(3) * input;
        s[0] = (it->second).getLine(4) * input;
        s[1] = (it->second).getLine(5) * input;

        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::UP, u[0], u[0] + u[1],
                        u[0] - u[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::DOWN, d[0], d[0] + d[1],
                        d[0] - d[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::STRANGE, s[0], s[0] + s[1],
                        s[0] - s[1]));
    }

    if (nFlavors == 4) {

        input.push_back(it_NSu->second);
        input.push_back(it_NSd->second);
        input.push_back(it_NSs->second);
        input.push_back(it_NSc->second);
        input.push_back(it_NSumd->second);
        input.push_back(it_NSums->second);
        input.push_back(it_NSumc->second);
        input.push_back(S);

        u[0] = (it->second).getLine(0) * input;
        u[1] = (it->second).getLine(1) * input;
        d[0] = (it->second).getLine(2) * input;
        d[1] = (it->second).getLine(3) * input;
        s[0] = (it->second).getLine(4) * input;
        s[1] = (it->second).getLine(5) * input;
        c[0] = (it->second).getLine(6) * input;
        c[1] = (it->second).getLine(7) * input;

        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::UP, u[0], u[0] + u[1],
                        u[0] - u[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::DOWN, d[0], d[0] + d[1],
                        d[0] - d[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::STRANGE, s[0], s[0] + s[1],
                        s[0] - s[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::CHARM, c[0], c[0] + c[1],
                        c[0] - c[1]));
    }

    if (nFlavors == 5) {

        input.push_back(it_NSu->second);
        input.push_back(it_NSd->second);
        input.push_back(it_NSs->second);
        input.push_back(it_NSc->second);
        input.push_back(it_NSt->second);
        input.push_back(it_NSumd->second);
        input.push_back(it_NSums->second);
        input.push_back(it_NSumc->second);
        input.push_back(it_NSumt->second);
        input.push_back(S);

        u[0] = (it->second).getLine(0) * input;
        u[1] = (it->second).getLine(1) * input;
        d[0] = (it->second).getLine(2) * input;
        d[1] = (it->second).getLine(3) * input;
        s[0] = (it->second).getLine(4) * input;
        s[1] = (it->second).getLine(5) * input;
        c[0] = (it->second).getLine(6) * input;
        c[1] = (it->second).getLine(7) * input;
        t[0] = (it->second).getLine(8) * input;
        t[1] = (it->second).getLine(9) * input;

        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::UP, u[0], u[0] + u[1],
                        u[0] - u[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::DOWN, d[0], d[0] + d[1],
                        d[0] - d[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::STRANGE, s[0], s[0] + s[1],
                        s[0] - s[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::CHARM, c[0], c[0] + c[1],
                        c[0] - c[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::TOP, t[0], t[0] + t[1],
                        t[0] - t[1]));
    }

    if (nFlavors == 6) {

        input.push_back(it_NSu->second);
        input.push_back(it_NSd->second);
        input.push_back(it_NSs->second);
        input.push_back(it_NSc->second);
        input.push_back(it_NSt->second);
        input.push_back(it_NSb->second);
        input.push_back(it_NSumd->second);
        input.push_back(it_NSums->second);
        input.push_back(it_NSumc->second);
        input.push_back(it_NSumt->second);
        input.push_back(it_NSumb->second);
        input.push_back(S);

        u[0] = (it->second).getLine(0) * input;
        u[1] = (it->second).getLine(1) * input;
        d[0] = (it->second).getLine(2) * input;
        d[1] = (it->second).getLine(3) * input;
        s[0] = (it->second).getLine(4) * input;
        s[1] = (it->second).getLine(5) * input;
        c[0] = (it->second).getLine(6) * input;
        c[1] = (it->second).getLine(7) * input;
        t[0] = (it->second).getLine(8) * input;
        t[1] = (it->second).getLine(9) * input;
        b[0] = (it->second).getLine(10) * input;
        b[1] = (it->second).getLine(11) * input;

        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::UP, u[0], u[0] + u[1],
                        u[0] - u[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::DOWN, d[0], d[0] + d[1],
                        d[0] - d[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::STRANGE, s[0], s[0] + s[1],
                        s[0] - s[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::CHARM, c[0], c[0] + c[1],
                        c[0] - c[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::TOP, t[0], t[0] + t[1],
                        t[0] - t[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::BOTTOM, b[0], b[0] + b[1],
                        b[0] - b[1]));
    }

    //add gluons
    partonDistribution.setGluonDistribution(GluonDistribution(G));

    //return
    return partonDistribution;
}

inline std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator GPDEvolutionVinnikov::findInQuarkFlavorMap(
        const std::map<QuarkFlavor::Type, QuarkDistribution>& map,
        QuarkFlavor::Type key, const std::string& mapName) const {

    //search
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it =
            map.find(key);

    //check
    if (it == map.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No key "
                        << QuarkFlavor(key).toString() << " in map "
                        << mapName);
    }

    //return
    return it;
}

std::map<QuarkNonSingletCombination::Type, double>::const_iterator GPDEvolutionVinnikov::findInQuarkNonSingletCombinationMap(
        const std::map<QuarkNonSingletCombination::Type, double>& map,
        QuarkNonSingletCombination::Type key,
        const std::string& mapName) const {

    //search
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it =
            map.find(key);

    //check
    if (it == map.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No key "
                        << QuarkNonSingletCombination(key).toString()
                        << " in map " << mapName);
    }

    //return
    return it;
}

void GPDEvolutionVinnikov::initFunctorsForIntegrations() {
    //set functors
    p_intIntegrateComputeOutputNS = NumA::Integrator1D::newIntegrationFunctor(
            this, &GPDEvolutionVinnikov::integrateComputeOutputNS);
    p_intIntegrateComputeOutputS = NumA::Integrator1D::newIntegrationFunctor(
            this, &GPDEvolutionVinnikov::integrateComputeOutputS);
    p_intIntegrateComputeOutputG = NumA::Integrator1D::newIntegrationFunctor(
            this, &GPDEvolutionVinnikov::integrateComputeOutputG);
}

double GPDEvolutionVinnikov::integrateComputeOutputNS(double MuF2,
        std::vector<double> par) {
    return nonSingletMu2FDerivative(MuF2,
            static_cast<QuarkNonSingletCombination::Type>(par.at(0)));
}

double GPDEvolutionVinnikov::integrateComputeOutputS(double MuF2,
        std::vector<double> par) {
    return singletMuF2Derivative(MuF2);
}

double GPDEvolutionVinnikov::integrateComputeOutputG(double MuF2,
        std::vector<double> par) {
    return gluonMuF2Derivative(MuF2);
}

} /* namespace PARTONS */
