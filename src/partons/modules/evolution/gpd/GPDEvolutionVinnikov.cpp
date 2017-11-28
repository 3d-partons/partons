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
        GPDEvolutionModule(className), m_gridSize(10), m_xiLast(0.), m_tLast(
                0.), m_MuR2Last(0.), m_MuF2RefLast(0.) {

    //clear vectors
    clearVectors(true);
}

GPDEvolutionVinnikov::GPDEvolutionVinnikov(const GPDEvolutionVinnikov& other) :
        GPDEvolutionModule(other), m_gridSize(other.m_gridSize), m_xiLast(
                other.m_xiLast), m_tLast(other.m_tLast), m_MuR2Last(
                other.m_MuR2Last), m_MuF2RefLast(other.m_MuF2RefLast) {

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

    m_NSXGrid = other.m_NSXGrid;
    m_SXGrid = other.m_SXGrid;
}

GPDEvolutionVinnikov::~GPDEvolutionVinnikov() {
}

GPDEvolutionVinnikov* GPDEvolutionVinnikov::clone() const {
    return new GPDEvolutionVinnikov(*this);
}

void GPDEvolutionVinnikov::configure(const ElemUtils::Parameters& parameters) {

    //run for mother class
    GPDEvolutionModule::configure(parameters);

    //check and set
    if (parameters.isAvailable(GPDEvolutionVinnikov::PARAM_NAME_GRID_SIZE)) {
        setGridSize(parameters.getLastAvailable().toUInt());
    }
}

void GPDEvolutionVinnikov::resolveObjectDependencies() {
    GPDEvolutionModule::resolveObjectDependencies();
}

void GPDEvolutionVinnikov::isModuleWellConfigured() {

    //run for mother class
    GPDEvolutionModule::isModuleWellConfigured();

    //check GPD type
    if (m_currentGPDComputeType != GPDType::H
            && m_currentGPDComputeType != GPDType::E
            && m_currentGPDComputeType != GPDType::Ht
            && m_currentGPDComputeType != GPDType::Et) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "This evolution module is not implemented for GPD type "
                        << GPDType(m_currentGPDComputeType).toString());
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
}

void GPDEvolutionVinnikov::initModule() {

    //run for mother class
    GPDEvolutionModule::initModule();

    //check if last kinematics the same
    bool lastKinematicsDifferent = !((m_xi == m_xiLast) && (m_t == m_tLast)
            && (m_MuR2 == m_MuR2Last) && (m_MuF2_ref == m_MuF2RefLast));

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
                    gpdKinematic, m_currentGPDComputeType, false);

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

                //get singlet combination and gluons
                m_S.push_back(getS(m_nFlavors_ref, partonDistribution));
                m_G.push_back(getG(partonDistribution));
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
}

void GPDEvolutionVinnikov::computeMuF2DerivativeNS(double MuF2, double MuF2Last,
        QuarkNonSingletCombination::Type quarkNonSingletCombination) {

    //check pQCD order
    if (m_qcdOrderType != PerturbativeQCDOrderType::LO) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Evolution not implemented for pQCD order "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
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
                * m_pRunningAlphaStrong->compute(MuF2Last) * one_twopi;
    for (i = 0; i <= 4 * m_gridSize; i++)
        gpd_aux.at(i) = gpd.at(i) + 0.5 * k1.at(i);
    for (i = 0; i <= 4 * m_gridSize; i++)
        k2.at(i) = dt * conv_int_ns(i, m_xi, m_gridSize, gpd_aux, m_NSXGrid)
                * m_pRunningAlphaStrong->compute(exp(log(MuF2Last) + 0.5 * dt))
                * one_twopi;
    for (i = 0; i <= 4 * m_gridSize; i++)
        gpd_aux.at(i) = gpd.at(i) + 0.5 * k2.at(i);
    for (i = 0; i <= 4 * m_gridSize; i++)
        k3.at(i) = dt * conv_int_ns(i, m_xi, m_gridSize, gpd_aux, m_NSXGrid)
                * m_pRunningAlphaStrong->compute(exp(log(MuF2Last) + 0.5 * dt))
                * one_twopi;
    for (i = 0; i <= 4 * m_gridSize; i++)
        gpd_aux.at(i) = gpd.at(i) + k3.at(i);
    for (i = 0; i <= 4 * m_gridSize; i++)
        k4.at(i) = dt * conv_int_ns(i, m_xi, m_gridSize, gpd_aux, m_NSXGrid)
                * m_pRunningAlphaStrong->compute(MuF2) * one_twopi;

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
    if (m_qcdOrderType != PerturbativeQCDOrderType::LO) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Evolution not implemented for pQCD order "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
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

    if (m_currentGPDComputeType == GPDType::H
            || m_currentGPDComputeType == GPDType::E) {

        qq = &GPDEvolutionVinnikov::conv_int_qq;
        gg = &GPDEvolutionVinnikov::conv_int_gg;
        gq = &GPDEvolutionVinnikov::conv_int_gq;
        qg = &GPDEvolutionVinnikov::conv_int_qg;
    } else if (m_currentGPDComputeType == GPDType::Ht
            || m_currentGPDComputeType == GPDType::Et) {

        qq = &GPDEvolutionVinnikov::conv_int_qq_pol;
        gg = &GPDEvolutionVinnikov::conv_int_gg_pol;
        gq = &GPDEvolutionVinnikov::conv_int_gq_pol;
        qg = &GPDEvolutionVinnikov::conv_int_qg_pol;
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Evolution not implemented for GPD type "
                        << GPDType(m_currentGPDComputeType).toString());
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
                        * m_pRunningAlphaStrong->compute(MuF2Last) * one_twopi;
        k1_g.at(i) =
                dt
                        * ((this->*gq)(i, m_xi, m_gridSize, m_SEvolved,
                                m_SXGrid)
                                + (this->*gg)(i, m_xi, m_gridSize, m_GEvolved,
                                        m_SXGrid))
                        * m_pRunningAlphaStrong->compute(MuF2Last) * one_twopi;
    }
    for (i = 0; i <= 2 * m_gridSize; i++) {
        gpd_q_aux.at(i) = m_SEvolved.at(i) + 0.5 * k1_q.at(i);
        gpd_g_aux.at(i) = m_GEvolved.at(i) + 0.5 * k1_g.at(i);
    }
    for (i = 0; i <= 2 * m_gridSize; i++) {
        k2_q.at(i) = dt
                * ((this->*qq)(i, m_xi, m_gridSize, gpd_q_aux, m_SXGrid)
                        + (this->*qg)(i, m_xi, m_gridSize, gpd_g_aux, m_SXGrid))
                * m_pRunningAlphaStrong->compute(exp(log(MuF2Last) + 0.5 * dt))
                * one_twopi;
        k2_g.at(i) = dt
                * ((this->*gq)(i, m_xi, m_gridSize, gpd_q_aux, m_SXGrid)
                        + (this->*gg)(i, m_xi, m_gridSize, gpd_g_aux, m_SXGrid))
                * m_pRunningAlphaStrong->compute(exp(log(MuF2Last) + 0.5 * dt))
                * one_twopi;
    }
    for (i = 0; i <= 2 * m_gridSize; i++) {
        gpd_q_aux.at(i) = m_SEvolved.at(i) + 0.5 * k2_q.at(i);
        gpd_g_aux.at(i) = m_GEvolved.at(i) + 0.5 * k2_g.at(i);
    }
    for (i = 0; i <= 2 * m_gridSize; i++) {
        k3_q.at(i) = dt
                * ((this->*qq)(i, m_xi, m_gridSize, gpd_q_aux, m_SXGrid)
                        + (this->*qg)(i, m_xi, m_gridSize, gpd_g_aux, m_SXGrid))
                * m_pRunningAlphaStrong->compute(exp(log(MuF2Last) + 0.5 * dt))
                * one_twopi;
        k3_g.at(i) = dt
                * ((this->*gq)(i, m_xi, m_gridSize, gpd_q_aux, m_SXGrid)
                        + (this->*gg)(i, m_xi, m_gridSize, gpd_g_aux, m_SXGrid))
                * m_pRunningAlphaStrong->compute(exp(log(MuF2Last) + 0.5 * dt))
                * one_twopi;
    }
    for (i = 0; i <= 2 * m_gridSize; i++) {
        gpd_q_aux.at(i) = m_SEvolved.at(i) + k3_q.at(i);
        gpd_g_aux.at(i) = m_GEvolved.at(i) + k3_g.at(i);
    }
    for (i = 0; i <= 2 * m_gridSize; i++) {
        k4_q.at(i) = dt
                * ((this->*qq)(i, m_xi, m_gridSize, gpd_q_aux, m_SXGrid)
                        + (this->*qg)(i, m_xi, m_gridSize, gpd_g_aux, m_SXGrid))
                * m_pRunningAlphaStrong->compute(MuF2) * one_twopi;
        k4_g.at(i) = dt
                * ((this->*gq)(i, m_xi, m_gridSize, gpd_q_aux, m_SXGrid)
                        + (this->*gg)(i, m_xi, m_gridSize, gpd_g_aux, m_SXGrid))
                * m_pRunningAlphaStrong->compute(MuF2) * one_twopi;
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
    m_pRunningAlphaStrong->setNf(nFlavors);

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
    m_pRunningAlphaStrong->setNf(nFlavors);

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
        if (m_currentGPDComputeType == GPDType::H
                || m_currentGPDComputeType == GPDType::E) {
            result *= -1;
        } else if (m_currentGPDComputeType == GPDType::Ht
                || m_currentGPDComputeType == GPDType::Et) {
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Evolution not implemented for GPD type "
                            << GPDType(m_currentGPDComputeType).toString());
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
    m_pRunningAlphaStrong->setNf(nFlavors);

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
        if (m_currentGPDComputeType == GPDType::H
                || m_currentGPDComputeType == GPDType::E) {
        } else if (m_currentGPDComputeType == GPDType::Ht
                || m_currentGPDComputeType == GPDType::Et) {
            result *= -1;
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Evolution not implemented for GPD type "
                            << GPDType(m_currentGPDComputeType).toString());
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
                            << "Integration is not continues (it changed direction). Try other integration routine. muF2_ref: "
                            << m_MuF2_ref << " muF2_target: " << m_MuF2
                            << " muF2_last: " << MuF2Last << " muF2_current: "
                            << MuF2);
        }

    } else {

        if (MuF2 > MuF2Last) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Integration is not continues (it changed direction). Try other integration routine. muF2_ref: "
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

} /* namespace PARTONS */
