#include "VinnikovEvolQCDModel.h"

#include <Math/InterpolationTypes.h>
#include <Math/Interpolator.h>
#include <stddef.h>
#include <cmath>
#include <cstdio>

#include "../../beans/gpd/GPDResult.h"
#include "../../beans/gpd/GPDType.h"
#include "../../beans/parton_distribution/GluonDistribution.h"
#include "../../beans/parton_distribution/PartonDistribution.h"
#include "../../BaseObjectRegistry.h"
#include "../../FundamentalPhysicalConstants.h"
#include "../../utils/logger/LoggerManager.h"
#include "../../utils/stringUtils/Formatter.h"
#include "../GPDModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int VinnikovEvolQCDModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new VinnikovEvolQCDModel("VinnikovEvolQCDModel"));

VinnikovEvolQCDModel::VinnikovEvolQCDModel(const std::string &className) :
        EvolQCDModule(className), m_nbColor(3.), m_CF(
                0.5 * (m_nbColor - 1. / m_nbColor)), m_CA(
                2. * m_CF + 1. / m_nbColor), m_nbActiveFlavor(3.), m_b0(
                11. * m_nbColor / 3. - 2. * m_nbActiveFlavor / 3.), m_TR(
                m_nbActiveFlavor / 2.), m_LnMuF2_ref(
                log(m_MuF_ref * m_MuF_ref)), m_LnMuF2(log(m_MuF * m_MuF)), m_NonSingletGpd(
                4 * m_nbXPoints + 1, 0.), m_SingletGpd(2 * m_nbXPoints + 1, 0.), m_GluonGpd(
                2 * m_nbXPoints + 1, 0.), m_Singletx(2 * m_nbXPoints + 1, 0.), m_NonSingletx(
                4 * m_nbXPoints + 1, 0.) {
}

VinnikovEvolQCDModel::~VinnikovEvolQCDModel() {

}

//TODO refactoring
VinnikovEvolQCDModel::VinnikovEvolQCDModel(const VinnikovEvolQCDModel &other) :
        EvolQCDModule(other) {
    m_nbColor = other.m_nbColor;
    m_CF = other.m_CF;
    m_CA = other.m_CA;
    m_nbActiveFlavor = other.m_nbActiveFlavor;
    m_b0 = other.m_b0;
    m_TR = other.m_TR;
    m_LnMuF2_ref = other.m_LnMuF2_ref;
    m_LnMuF2 = other.m_LnMuF2;
    m_NonSingletGpd = other.m_NonSingletGpd;
    m_SingletGpd = other.m_SingletGpd;
    m_GluonGpd = other.m_GluonGpd;
    m_Singletx = other.m_Singletx;
    m_NonSingletx = other.m_NonSingletx;
}

VinnikovEvolQCDModel* VinnikovEvolQCDModel::clone() const {
    return new VinnikovEvolQCDModel(*this);
}

void VinnikovEvolQCDModel::initModule() {
    EvolQCDModule::initModule();

    m_LnMuF2_ref = log(m_MuF_ref * m_MuF_ref);
    m_LnMuF2 = log(m_MuF * m_MuF);
}

//TODO implementer
void VinnikovEvolQCDModel::isModuleWellConfigured() {
    EvolQCDModule::isModuleWellConfigured();
}

PartonDistribution VinnikovEvolQCDModel::compute(double x, double xi, double t,
        double MuF, double MuR, GPDModule* pGPDModule,
        PartonDistribution partonDistribution) {

//    m_pLoggerManager->debug(getClassName(), __func__,
//            Formatter() << "x = " << x << "    xi = " << xi << "    t = " << t
//                    << " GeV2    MuF = " << MuF << " GeV    MuR = " << MuR
//                    << " GeV");

// TODO : Why does the statement below not work?
//	m_pLoggerManager->debug(getClassName(), __func__,
//			Formatter() << gpdResultData.toString());

    EvolQCDModule::preCompute(x, xi, t, MuF, MuR, pGPDModule,
            partonDistribution);

    initModule();

    isModuleWellConfigured();

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "m_currentGPDComputeType = "
                    << GPDType(m_currentGPDComputeType).toString());

    m_vectorOfGPDCombination = EvolQCDModule::convertBasis(
            m_vectorOfGPDCombination);

    evolution();

    m_vectorOfGPDCombination = EvolQCDModule::invertBasis(
            m_vectorOfGPDCombination);

    PartonDistribution finalPartonDistribution =
            EvolQCDModule::makeFinalPartonDistribution();

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << finalPartonDistribution.toString());

    return finalPartonDistribution;
}

void VinnikovEvolQCDModel::evolution() {

    double Evol;

    // Compute NonSinglet evolution for the first 2*m_nfMin-1 rows of m_vectorOfGPDCombination
    for (unsigned int iNS = 0; iNS < 2 * m_nfMin - 1; iNS++) {

        m_pLoggerManager->debug(getClassName(), __func__,
                Formatter() << "iNS = " << iNS);

        evolutionNonSingletGPD(iNS);
        Evol = interpolate(m_NonSingletx, m_NonSingletGpd, m_x);

        m_pLoggerManager->debug(getClassName(), __func__,
                Formatter() << "Evol NS (interpolated) = " << Evol);

        m_vectorOfGPDCombination.at(iNS) = Evol;
    }

    // Compute Singlet evolution for the last 2 rows of m_vectorOfGPDCombination
    // TODO: Implement x <--> -x ?
    evolutionSingletGPD();

    // Singlet GPD
    Evol = interpolate(m_Singletx, m_SingletGpd, m_x);

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "Evol S (interpolated) = " << Evol);

    m_vectorOfGPDCombination.at(2 * m_nfMin - 1) = Evol;

    // Gluon GPD
    Evol = interpolate(m_Singletx, m_GluonGpd, m_x);

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "Evol Gluon (interpolated) = " << Evol);

    m_vectorOfGPDCombination.at(2 * m_nfMin) = Evol;
}

void VinnikovEvolQCDModel::evolutionNonSingletGPD(unsigned int iNS) {

    double k1[4 * m_nbXPoints + 1], k2[4 * m_nbXPoints + 1];
    double k3[4 * m_nbXPoints + 1], k4[4 * m_nbXPoints + 1];
    double gpd_aux[4 * m_nbXPoints + 1];
    double dummygpd[4 * m_nbXPoints + 1];
    double dummyx[4 * m_nbXPoints + 1];
    double dt = (m_LnMuF2 - m_LnMuF2_ref) / ((double) m_nbMuFPoints);
    double one_twopi = 1.0 / (2.0 * PI);

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "one_twopi = " << one_twopi);

    buildNonSingletxGrid();

    initNonSingletGPD(iNS);

    // Stupid thing done here to fulfill variable constraints
    for (unsigned int idummy = 0; idummy < 4 * m_nbXPoints + 1; idummy++) {
        dummygpd[idummy] = m_NonSingletGpd.at(idummy);
        dummyx[idummy] = m_NonSingletx.at(idummy);

        m_pLoggerManager->debug(getClassName(), __func__,
                Formatter() << "x = " << m_NonSingletx.at(idummy)
                        << "	initial GPDNS = " << m_NonSingletGpd.at(idummy));
    }

    // Here we start the evolution procedure
    // "i" corresponds to the number of interval in "dt"
    for (unsigned int j = 0; j <= (m_nbMuFPoints - 1); j++) {
        double t = m_LnMuF2_ref + j * dt;

        m_pLoggerManager->debug(getClassName(), __func__,
                Formatter() << "dt = " << dt << "	alpha_s(t) = " << alpha_s(t));

        // Now use Runge-Kutta 4-th order method
        // at each x[i] (0 <= i <= n). In this way, numerical solution
        // of the integro-differential evolution equation
        // is reduced to solution of a system of n+1
        // ordinary differential equations of first order.
        // So, Runge-Kutta can be applied.
        for (unsigned int i = 0; i <= 4 * m_nbXPoints; i++) {
            k1[i] = dt
                    * convolNonSingletKernel(i, m_xi, m_nbXPoints, dummygpd,
                            dummyx) * alpha_s(t) * one_twopi;

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "k1[" << i << "] = " << k1[i]);
        }

        for (unsigned int i = 0; i <= 4 * m_nbXPoints; i++) {
            gpd_aux[i] = m_NonSingletGpd[i] + 0.5 * k1[i];

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "gpd_aux k1[" << i << "] = " << gpd_aux[i]);
        }

        for (unsigned int i = 0; i <= 4 * m_nbXPoints; i++) {
            k2[i] = dt
                    * convolNonSingletKernel(i, m_xi, m_nbXPoints, gpd_aux,
                            dummyx) * alpha_s(t + 0.5 * dt) * one_twopi;

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "k2[" << i << "] = " << k1[i]);
        }

        for (unsigned int i = 0; i <= 4 * m_nbXPoints; i++) {
            gpd_aux[i] = m_NonSingletGpd[i] + 0.5 * k2[i];

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "gpd_aux k2[" << i << "] = " << gpd_aux[i]);
        }

        for (unsigned int i = 0; i <= 4 * m_nbXPoints; i++) {
            k3[i] = dt
                    * convolNonSingletKernel(i, m_xi, m_nbXPoints, gpd_aux,
                            dummyx) * alpha_s(t + 0.5 * dt) * one_twopi;

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "k3[" << i << "] = " << k1[i]);
        }

        for (unsigned int i = 0; i <= 4 * m_nbXPoints; i++) {
            gpd_aux[i] = dummygpd[i] + k3[i];

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "gpd_aux k3[" << i << "] = " << gpd_aux[i]);
        }

        for (unsigned int i = 0; i <= 4 * m_nbXPoints; i++) {
            k4[i] = dt
                    * convolNonSingletKernel(i, m_xi, m_nbXPoints, gpd_aux,
                            dummyx) * alpha_s(t + dt) * one_twopi;

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "k4[" << i << "] = " << k1[i]);
        }

        for (unsigned int i = 0; i <= 4 * m_nbXPoints; i++) {
            m_NonSingletGpd.at(i) += (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i])
                    / 6.0;

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "x = " << m_NonSingletx.at(i)
                            << "	final GPDNS = " << m_NonSingletGpd.at(i));
        }
    }
}

void VinnikovEvolQCDModel::evolutionSingletGPD() {

    double k1_q[2 * m_nbXPoints + 1], k2_q[2 * m_nbXPoints + 1];
    double k3_q[2 * m_nbXPoints + 1], k4_q[2 * m_nbXPoints + 1];
    double k1_g[2 * m_nbXPoints + 1], k2_g[2 * m_nbXPoints + 1];
    double k3_g[2 * m_nbXPoints + 1], k4_g[2 * m_nbXPoints + 1];
    double gpd_q_aux[2 * m_nbXPoints + 1], gpd_g_aux[2 * m_nbXPoints + 1];
    double dt = (m_LnMuF2 - m_LnMuF2_ref) / ((double) m_nbMuFPoints);
    double one_twopi = 1.0 / (2.0 * PI);

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "one_twopi = " << one_twopi);

    //Another stupid thing to get good variable type
    double dummygpd_q[2 * m_nbXPoints + 1];
    double dummygpd_g[2 * m_nbXPoints + 1];
    double dummyx[2 * m_nbXPoints + 1];

    buildSingletxGrid();

    initSingletGPD();

    for (unsigned int i = 0; i < 2 * m_nbXPoints + 1; i++) {
        // Warning: In Vinnikov's code, the singlet combination is not divided
        // by 2 nf. Hence we first multiply it by 2 nf here, and will then
        // divide it by 2 nf at the end of the function.
        // Here it is m_nfMin which is relevant.
        m_SingletGpd.at(i) *= 2. * m_nfMin;

        dummygpd_q[i] = m_SingletGpd.at(i);
        dummygpd_g[i] = m_GluonGpd.at(i);
        dummyx[i] = m_Singletx.at(i);

        m_pLoggerManager->debug(getClassName(), __func__,
                Formatter() << "x = " << m_Singletx.at(i) << "	initial GPDS = "
                        << m_SingletGpd.at(i) << "	initial GPD gluon = "
                        << m_GluonGpd.at(i));
    }

    // Here we start the evolution procedure
    // "i" corresponds to the number of interval in "dt"
    for (unsigned int j = 0; j < m_nbMuFPoints; j++) {
        double t = m_LnMuF2_ref + j * dt;

        m_pLoggerManager->debug(getClassName(), __func__,
                Formatter() << "dt = " << dt << "	alpha_s(t) = " << alpha_s(t));

        // Now use Runge-Kutta 4-th order method
        // at each x[i] (0 <= i <= n). In this way, numerical solution
        // of the integro-differential evolution equation
        // is reduced to solution of a system of n+1
        // ordinary differential equations of first order.
        // So, Runge-Kutta can be applied.
        for (unsigned int i = 0; i <= 2 * m_nbXPoints; i++) {
            k1_q[i] = dt
                    * (convolSingletqqKernel(i, m_xi, m_nbXPoints, dummygpd_q,
                            dummyx)
                            + convolSingletqgKernel(i, m_xi, m_nbXPoints,
                                    dummygpd_g, dummyx)) * alpha_s(t)
                    * one_twopi;

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "k1_q[" << i << "] = " << k1_q[i]);

            k1_g[i] = dt
                    * (convolSingletgqKernel(i, m_xi, m_nbXPoints, dummygpd_q,
                            dummyx)
                            + convolSingletggKernel(i, m_xi, m_nbXPoints,
                                    dummygpd_g, dummyx)) * alpha_s(t)
                    * one_twopi;

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "k1_g[" << i << "] = " << k1_g[i]);
        }

        for (unsigned int i = 0; i <= 2 * m_nbXPoints; i++) {
            gpd_q_aux[i] = dummygpd_q[i] + 0.5 * k1_q[i];
            gpd_g_aux[i] = dummygpd_g[i] + 0.5 * k1_g[i];

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "gpd_q_aux k1[" << i << "] = "
                            << gpd_q_aux[i] << "	gpd_g_aux k1[" << i << "] = "
                            << gpd_g_aux[i]);
        }

        for (unsigned int i = 0; i <= 2 * m_nbXPoints; i++) {
            k2_q[i] = dt
                    * (convolSingletqqKernel(i, m_xi, m_nbXPoints, gpd_q_aux,
                            dummyx)
                            + convolSingletqgKernel(i, m_xi, m_nbXPoints,
                                    gpd_g_aux, dummyx)) * alpha_s(t + 0.5 * dt)
                    * one_twopi;

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "k2_q[" << i << "] = " << k2_q[i]);

            k2_g[i] = dt
                    * (convolSingletgqKernel(i, m_xi, m_nbXPoints, gpd_q_aux,
                            dummyx)
                            + convolSingletggKernel(i, m_xi, m_nbXPoints,
                                    gpd_g_aux, dummyx)) * alpha_s(t + 0.5 * dt)
                    * one_twopi;

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "k2_g[" << i << "] = " << k2_g[i]);
        }

        for (unsigned int i = 0; i <= 2 * m_nbXPoints; i++) {
            gpd_q_aux[i] = dummygpd_q[i] + 0.5 * k2_q[i];
            gpd_g_aux[i] = dummygpd_g[i] + 0.5 * k2_g[i];

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "gpd_q_aux k2[" << i << "] = "
                            << gpd_q_aux[i] << "	gpd_g_aux k2[" << i << "] = "
                            << gpd_g_aux[i]);
        }

        for (unsigned int i = 0; i <= 2 * m_nbXPoints; i++) {
            k3_q[i] = dt
                    * (convolSingletqqKernel(i, m_xi, m_nbXPoints, gpd_q_aux,
                            dummyx)
                            + convolSingletqgKernel(i, m_xi, m_nbXPoints,
                                    gpd_g_aux, dummyx)) * alpha_s(t + 0.5 * dt)
                    * one_twopi;

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "k3_q[" << i << "] = " << k3_q[i]);

            k3_g[i] = dt
                    * (convolSingletgqKernel(i, m_xi, m_nbXPoints, gpd_q_aux,
                            dummyx)
                            + convolSingletggKernel(i, m_xi, m_nbXPoints,
                                    gpd_g_aux, dummyx)) * alpha_s(t + 0.5 * dt)
                    * one_twopi;

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "k3_g[" << i << "] = " << k3_g[i]);
        }

        for (unsigned int i = 0; i <= 2 * m_nbXPoints; i++) {
            gpd_q_aux[i] = dummygpd_q[i] + k3_q[i];
            gpd_g_aux[i] = dummygpd_g[i] + k3_g[i];

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "gpd_q_aux k3[" << i << "] = "
                            << gpd_q_aux[i] << "	gpd_g_aux k3[" << i << "] = "
                            << gpd_g_aux[i]);
        }

        for (unsigned int i = 0; i <= 2 * m_nbXPoints; i++) {
            k4_q[i] = dt
                    * (convolSingletqqKernel(i, m_xi, m_nbXPoints, gpd_q_aux,
                            dummyx)
                            + convolSingletqgKernel(i, m_xi, m_nbXPoints,
                                    gpd_g_aux, dummyx)) * alpha_s(t + dt)
                    * one_twopi;

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "k4_q[" << i << "] = " << k4_q[i]);

            k4_g[i] = dt
                    * (convolSingletgqKernel(i, m_xi, m_nbXPoints, gpd_q_aux,
                            dummyx)
                            + convolSingletggKernel(i, m_xi, m_nbXPoints,
                                    gpd_g_aux, dummyx)) * alpha_s(t + dt)
                    * one_twopi;

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "k4_g[" << i << "] = " << k4_g[i]);
        }

        for (unsigned int i = 0; i <= 2 * m_nbXPoints; i++) {
            m_SingletGpd.at(i) += (k1_q[i] + 2.0 * k2_q[i] + 2.0 * k3_q[i]
                    + k4_q[i]) / 6.0;
            m_GluonGpd.at(i) += (k1_g[i] + 2.0 * k2_g[i] + 2.0 * k3_g[i]
                    + k4_g[i]) / 6.0;

            // Warning: In Vinnikov's code, the singlet combination is not divided
            // by 2 nf. Hence we first multiply it by 2 nf here, and will then
            // divide it by 2 nf at the end of the function.
            // Here it is m_nfMin which is relevant.
            m_SingletGpd.at(i) /= 2. * m_nfMin;

            m_pLoggerManager->debug(getClassName(), __func__,
                    Formatter() << "x = " << m_Singletx.at(i)
                            << "	final GPDS = " << m_SingletGpd.at(i)
                            << "	final GPD gluon = " << m_GluonGpd.at(i));
        }

    }
}

void VinnikovEvolQCDModel::buildNonSingletxGrid() {

    double const gamma = log(
            0.5 + 0.5 / m_xi * sqrt(9.0 * m_xi * m_xi - 12.0 * m_xi + 4.0))
            / (m_nbXPoints);
    double const shift = m_xi * m_xi / (1.0 - 2.0 * m_xi);

    for (unsigned int i = 0; i <= 2 * m_nbXPoints; i++) {
        m_NonSingletx.at(i) = -shift * exp(gamma * (2 * m_nbXPoints - i))
                + shift;

        m_pLoggerManager->debug(getClassName(), __func__,
                Formatter() << "i = " << i << "	m_NonSingletx = "
                        << m_NonSingletx.at(i));
    }

    for (unsigned int i = 2 * m_nbXPoints + 1; i <= 4 * m_nbXPoints; i++) {
        m_NonSingletx.at(i) = shift * exp(gamma * (i - 2 * m_nbXPoints))
                - shift;

        m_pLoggerManager->debug(getClassName(), __func__,
                Formatter() << "i = " << i << "	m_NonSingletx = "
                        << m_NonSingletx.at(i));
    }

    // TODO : Check if necessary
    // Security limit to avoid problem when computing GPDs
//	m_NonSingletx.at(0) = -1+1e-15;
//	m_NonSingletx.at(4*m_sizeXGrid) = +1-1e-15;
}

void VinnikovEvolQCDModel::buildSingletxGrid() {

    double const gamma = log(
            0.5 + 0.5 / m_xi * sqrt(9.0 * m_xi * m_xi - 12.0 * m_xi + 4.0))
            / ((double) m_nbXPoints);
    double const shift = m_xi * m_xi / (1.0 - 2.0 * m_xi);

    for (unsigned int i = 0; i <= 2 * m_nbXPoints; i++) {
        m_Singletx.at(i) = shift * exp(gamma * i) - shift;

        m_pLoggerManager->debug(getClassName(), __func__,
                Formatter() << "i = " << i << "	m_Singletx = "
                        << m_Singletx.at(i));
    }

    // TODO : Check if necessary
    // Security limit to avoid problem when computing GPDs
//	m_Singletx.at(0) = 1e-15;
//	m_Singletx.at(2*m_sizeXGrid) = +1-1e-15;
}

double VinnikovEvolQCDModel::alpha_s(double t) {
    double LOG_LAMBDA_5 = -1.4872;
    double LOG_LAMBDA_4 = -1.1221;
    double LOG_LAMBDA_3 = -0.9883;

    //TODO cast unsigned int (m_nbActiveFlavor) to double
    double B0 = (11.0 - 2.0 * m_nbActiveFlavor / 3.0);
    double B1 = (51.0 - 19.0 * m_nbActiveFlavor / 3.0);

    double alpha, L;

    switch (m_nbActiveFlavor) {
    case 3: {
        L = t - 2.0 * LOG_LAMBDA_3;
        break;
    }
    case 4: {
        L = t - 2.0 * LOG_LAMBDA_4;
        break;
    }
    case 5: {
        L = t - 2.0 * LOG_LAMBDA_5;
        break;
    }
    default:
        printf(
                "WARNING (alpha_s.c): m_nbActiveFlavor not between 3 and 5. Returning alpha_s = 0\n");
        break;
    }

    if ((alpha = 4.0 * M_PI / (B0 * L)
            * (1.0 - 2.0 * B1 * log(L) / (B0 * B0 * L))) > 1.0) {
        printf("WARNING (alpha_s.c): alpha_s =%9.2e\n", alpha);
    }

    return alpha;
}

void VinnikovEvolQCDModel::initNonSingletGPD(unsigned int iNS) {
    std::vector<double> vectorOfGPDCombinations;

    GPDResult gpdResult;

    for (unsigned int iGrid = 0; iGrid < 4 * m_nbXPoints + 1; iGrid++) {
        gpdResult = m_pGPDModule->compute(m_NonSingletx.at(iGrid), m_xi, m_t,
                m_MuF_ref, m_MuR, m_currentGPDComputeType);

        m_pLoggerManager->debug(getClassName(), __func__,
                Formatter() << "gpdOutputData = " << gpdResult.toString());

        PartonDistribution partonDistribution = gpdResult.getPartonDistribution(
                m_currentGPDComputeType);

        m_pLoggerManager->debug(getClassName(), __func__,
                Formatter() << "GpdResultData = "
                        << partonDistribution.toString());

        vectorOfGPDCombinations = EvolQCDModule::MakeVectorOfGPDCombinations(
                partonDistribution);
        // TODO: A whole matrix product is computed while only one line is used... Improve!
        vectorOfGPDCombinations = EvolQCDModule::convertBasis(
                vectorOfGPDCombinations);
        m_NonSingletGpd.at(iGrid) = vectorOfGPDCombinations.at(iNS);

        m_pLoggerManager->debug(getClassName(), __func__,
                Formatter() << "m_NonSingletGpd.at(" << iGrid << ") = "
                        << m_NonSingletGpd.at(iGrid));
    }
}

void VinnikovEvolQCDModel::initSingletGPD() {

    for (unsigned int iGrid = 0; iGrid < 2 * m_nbXPoints + 1; iGrid++) {
        GPDResult gpdResult = m_pGPDModule->compute(m_Singletx.at(iGrid), m_xi,
                m_t, m_MuF_ref, m_MuR, m_currentGPDComputeType);
        PartonDistribution partonDistribution = gpdResult.getPartonDistribution(
                m_currentGPDComputeType);
        m_GluonGpd.at(iGrid) =
                partonDistribution.getGluonDistribution().getGluonDistribution();
        m_SingletGpd.at(iGrid) = partonDistribution.getSinglet();
    }
}

double VinnikovEvolQCDModel::convolNonSingletKernel(const unsigned int ix,
        const double xi, const int n, const double* const gpd,
        const double* const yy) {

    // Dimension of "gpd" array must be 4n+1, i.e. "double gpd[4n+1]"
    // In the code it is assumed that gpd(x=-1) = gpd(x=1) = 0.0 !!

    // This function computes the convolution integral
    // using logarithmic grid, i.e. something like this:

    // \int f(x) dx = \int f(exp(gamma*i)-shift)*gamma*exp(gamma*i) di

    // So that the (shifted) logarithmic scale in "x" is transformed into
    // the linear scale in "i". Thus, the standard Simpson method
    // can be used for integrand which has steep rise around zero.
    // More precise, the interval in "x" [0, 1] is presented as:

    // x = -shift*exp(-gamma*(i-2n)) + shift, i = 0 ... 2n
    // x =  shift*exp( gamma*(i-2n)) - shift, i = 2n+1 ... 4n

    // The coefficients "gamma" and "shift" are chosen so that the point
    // x = +-xi divided the pieces in two equal (in log scale) parts:

    // gamma = log(1/2 + 1/2/xi*sqrt(9*xi^2 - 12*xi+4))/n
    // shift = xi^2/(1-2*xi)

    // which yields:
    // which yields:
    // x[0] = -1, x[n] = -xi, x[2n] = 0, x[3n] = xi, x[4n] = 1

    if (n < 5) {
        printf(
                "It's not a good idea to choose n < 5.\n I am too serious code to even start calculation at such conditions.\n So, all you get now is return 0.0;\n");
        return 0.;
    }

    double const x = yy[ix];
    double const x2 = x * x;
    double const xi2 = xi * xi;
    double const c_1_3 = 1.0 / 3.0;
    double const shift = xi2 / (1.0 - 2.0 * xi);
    double const gamma = log(0.5 + 0.5 / xi * sqrt(9.0 * xi2 - 12.0 * xi + 4.0))
            / static_cast<double>(n);

    // First, consider separately some close to boundary cases
    if (0 == ix)
        return 0.0;
    if (4 * n == ix)
        return 0.0;

    if (1 == ix) {
        double y = yy[2];
        double x2m2xi2 = x2 - 2.0 * xi2;
        double f1 = (shift + 1.0) * gpd[1] * (x2m2xi2 + 1.0)
                / ((-1.0 - x) * (1.0 - xi2));
        double f2 = -(-3.0 * gpd[1] + 6.0 * gpd[2] - gpd[3]) / (3.0 * gamma);
        double f3 = (shift - y) * (gpd[1] - gpd[2]) * (x2m2xi2 + y * y)
                / ((y - x) * (y * y - xi2));
        return m_CF
                * (c_1_3 * gamma * (1.25 * f1 + 2.0 * f2 - 0.25 * f3)
                        + gpd[1]
                                * (1.5 + 2.0 * log(1.0 + x)
                                        + 0.5 * (x - xi) / xi
                                                * log((xi - x) * (1.0 - xi))
                                        - 0.5 * (x + xi) / xi
                                                * log((-xi - x) * (1.0 + xi))));
    }

    if (2 == ix) {
        double y = yy[1];
        double x2m2xi2 = x2 - 2.0 * xi2;
        double f1 = (shift + 1.0) * gpd[2] * (x2m2xi2 + 1.0)
                / ((-1.0 - x) * (1.0 - xi2));
        double f2 = (shift - y) * (gpd[2] - gpd[1]) * (x2m2xi2 + y * y)
                / ((y - x) * (y * y - xi2));
        double f3 = -(-6.0 * gpd[1] + 3.0 * gpd[2] + 2.0 * gpd[3])
                / (3.0 * gamma);
        return m_CF
                * (c_1_3 * gamma * (f1 + 4.0 * f2 + f3)
                        + gpd[2]
                                * (1.5 + 2.0 * log(1.0 + x)
                                        + 0.5 * (x - xi) / xi
                                                * log((xi - x) * (1.0 - xi))
                                        - 0.5 * (x + xi) / xi
                                                * log((-xi - x) * (1.0 + xi))));
    }

    if ((4 * n - 1) == ix) {
        double y = yy[4 * n - 2];
        double f1 = (shift + y) * (gpd[4 * n - 2] - gpd[ix])
                * (y * y + x2 - 2.0 * xi2) / ((y - x) * (y * y - xi2));
        double f2 = (gpd[ix - 2] - 6.0 * gpd[ix - 1] + 3.0 * gpd[ix])
                / (3.0 * gamma);
        double f3 = -(shift + 1.0) * gpd[ix] * (1.0 + x2 - 2.0 * xi2)
                / ((1.0 - x) * (1.0 - xi2));
        return m_CF
                * (c_1_3 * gamma * (-0.25 * f1 + 2.0 * f2 + 1.25 * f3)
                        + gpd[1]
                                * (3.0 / 2.0 + 2.0 * log(1.0 - x)
                                        + 0.5 * (x - xi) / xi
                                                * log((x - xi) * (1.0 + xi))
                                        - 0.5 * (x + xi) / xi
                                                * log((xi + x) * (1.0 - xi))));
    }

    if ((4 * n - 2) == ix) {
        double y = yy[4 * n - 1];
        double f1 = (-2.0 * gpd[ix - 1] - 3.0 * gpd[ix] + 6.0 * gpd[ix + 1])
                / (3.0 * gamma);
        double f2 = (shift + y) * (gpd[4 * n - 1] - gpd[ix])
                * (y * y + x2 - 2.0 * xi2) / ((y - x) * (y * y - xi2));
        double f3 = -(shift + 1.0) * gpd[ix] * (1.0 + x2 - 2.0 * xi2)
                / ((1.0 - x) * (1.0 - xi2));
        return m_CF
                * (c_1_3 * gamma * (f1 + 4.0 * f2 + f3)
                        + gpd[2]
                                * (3.0 / 2.0 + 2.0 * log(1.0 - x)
                                        + 0.5 * (x - xi) / xi
                                                * log((x - xi) * (1.0 + xi))
                                        - 0.5 * (x + xi) / xi
                                                * log((xi + x) * (1.0 - xi))));
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution at x < -xi (2 < i < n).
    if (ix < n) {
        double convint = 0.0;
        double x2m2xi2 = x2 - 2.0 * xi2;
        double gpd_x = gpd[ix];
        unsigned int in;
        if (0 == (ix % 2))
            in = ix;
        else
            in = ix - 1;

        for (unsigned int i = 2; i < in; i += 2) {
            double y = yy[i];
            double y2 = y * y;
            double f1 = (shift - y) * (gpd_x - gpd[i]) * (x2m2xi2 + y2)
                    / ((y - x) * (y2 - xi2));
            convint += 2.0 * f1;
        }

        for (unsigned int i = 1; i < ix; i += 2) {
            double y = yy[i];
            double y2 = y * y;
            double f1 = (shift - y) * (gpd_x - gpd[i]) * (x2m2xi2 + y2)
                    / ((y - x) * (y2 - xi2));
            convint += 4.0 * f1;
        }

        if (ix == in) {
            convint += -(-2.0 * gpd[ix - 3] + 9.0 * gpd[ix - 2]
                    - 18.0 * gpd[ix - 1] + 11.0 * gpd[ix]) / (3.0 * gamma);
        }

        if ((ix - 1) == in) {
            double ya = yy[ix - 2];
            double yb = yy[ix - 1];
            double f1 = (shift - ya) * (gpd_x - gpd[ix - 2])
                    * (x2m2xi2 + ya * ya) / ((ya - x) * (ya * ya - xi2));
            double f2 = (shift - yb) * (gpd_x - gpd[ix - 1])
                    * (x2m2xi2 + yb * yb) / ((yb - x) * (yb * yb - xi2));
            double f3 = -(-2.0 * gpd[ix - 3] + 9.0 * gpd[ix - 2]
                    - 18.0 * gpd[ix - 1] + 11.0 * gpd[ix]) / (3.0 * gamma);
            convint += -0.25 * f1 + 3.0 * f2 + 1.25 * f3;
        }

        // TODO: Unclear, a little dirty. Be explicit !
        {
            // y = -1
            convint += (shift + 1.0) * gpd_x * (x2m2xi2 + 1.0)
                    / ((-1.0 - x) * (1.0 - xi2));
        }

        convint *= gamma * c_1_3;

        convint += gpd[ix]
                * (1.5 + 2.0 * log(1.0 + x)
                        + 0.5 * (x - xi) / xi * log((xi - x) * (1.0 - xi))
                        - 0.5 * (x + xi) / xi * log((-xi - x) * (1.0 + xi)));

        return m_CF * convint;
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution at x = -xi (i = n). If the number of intervals
    // is even, the usual Simpson's formula is used. If it is odd, the first
    // interval is integrated with trapecia.

    if (ix == n) {
        double convint = 0.0;
        double gpd_x = gpd[ix];
        unsigned int in;
        if (0 == (ix % 2))
            in = ix;
        else
            in = ix - 1;

        for (unsigned int i = 2; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift - y) * (gpd_x - gpd[i]) / (y + xi);
            convint += 2.0 * f1;
        }

        for (unsigned int i = 1; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift - y) * (gpd_x - gpd[i]) / (y + xi);
            convint += 4.0 * f1;
        }

        if (ix == in) {
            convint += -(-2.0 * gpd[ix - 3] + 9.0 * gpd[ix - 2]
                    - 18.0 * gpd[ix - 1] + 11.0 * gpd[ix]) / (6.0 * gamma);
        }
        if ((ix - 1) == in) {
            double ya = yy[ix - 2];
            double yb = yy[ix - 1];
            double f1 = (shift - ya) * (gpd_x - gpd[ix - 2]) / (ya + xi);
            double f2 = (shift - yb) * (gpd_x - gpd[ix - 1]) / (yb + xi);
            double f3 = (-2.0 * gpd[ix - 3] + 9.0 * gpd[ix - 2]
                    - 18.0 * gpd[ix - 1] + 11.0 * gpd[ix]) / (6.0 * gamma);
            convint += -0.25 * f1 + 3.0 * f2 - 1.25 * f3;

        }

        // TODO: Unclear again...
        {
            // y = -1
            convint += (shift + 1.0) * gpd_x / (-1.0 + xi);
        }

        convint *= gamma * c_1_3;

        convint += gpd[ix] * (1.5 + log(0.5 * (1.0 - xi) / xi));

        convint *= m_CF;

        return convint;
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution at -xi < x < 0 (n < i <= 2n).
    // The integral is given by a sum of three parts: -1 < y < x,
    // x < y < 0 and 0 < y < 1.
    if (ix <= 2 * n) {
        // First integrate at -1 < y < x
        double convint = 0.0;
        double gpd_x = gpd[ix];
        double xp2xi = x + 2.0 * xi;
        double xm2xi = x - 2.0 * xi;
        unsigned int in;

        if (0 == (ix % 2))
            in = ix;
        else
            in = ix - 1;

        for (unsigned int i = 2; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift - y) * (gpd[i] - gpd_x) * (x - xi) * (y - xp2xi)
                    / (xi * (y - xi) * (y - x));
            convint += 2.0 * f1;
        }

        for (unsigned int i = 1; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift - y) * (gpd[i] - gpd_x) * (x - xi) * (y - xp2xi)
                    / (xi * (y - xi) * (y - x));
            convint += 4.0 * f1;
        }

        if (ix == in) {
            convint += (2.0 * gpd[ix - 3] - 9.0 * gpd[ix - 2]
                    + 18.0 * gpd[ix - 1] - 11.0 * gpd[ix]) / (3.0 * gamma);
        }

        if ((ix - 1) == in) {
            double ya = yy[ix - 2];
            double yb = yy[ix - 1];
            double f1 = (shift - ya) * (gpd[ix - 2] - gpd_x) * (x - xi)
                    * (ya - xp2xi) / (xi * (ya - xi) * (ya - x));
            double f2 = (shift - yb) * (gpd[ix - 1] - gpd_x) * (x - xi)
                    * (yb - xp2xi) / (xi * (yb - xi) * (yb - x));
            double f3 = (2.0 * gpd[ix - 3] - 9.0 * gpd[ix - 2]
                    + 18.0 * gpd[ix - 1] - 11.0 * gpd[ix]) / (3.0 * gamma);
            convint += -0.25 * f1 + 3.0 * f2 + 1.25 * f3;
        }

        // TODO: unclear
        {
            // y = -1
            convint += -(shift + 1.0) * gpd_x * (x - xi) * (-1.0 - xp2xi)
                    / (xi * (-1.0 - xi) * (-1.0 - x));
        }

        // Now integrate at  x < y < 0
        if (ix < (2 * n - 2)) {
            if (0 == (ix % 2))
                in = 2 * n;
            else
                in = 2 * n - 1;
            for (unsigned int i = ix + 2; i < in; i += 2) {
                double y = yy[i];
                double f1 = (shift - y) * (x + xi) * (gpd[i] - gpd_x)
                        * (y - xm2xi) / (xi * (y + xi) * (y - x));
                convint += 2.0 * f1;
            }
            for (unsigned int i = ix + 1; i < in; i += 2) {
                double y = yy[i];
                double f1 = (shift - y) * (x + xi) * (gpd[i] - gpd_x)
                        * (y - xm2xi) / (xi * (y + xi) * (y - x));
                convint += 4.0 * f1;
            }

            // TODO: Unclear.
            {
                double y = yy[in];
                double f1 = (shift - y) * (x + xi) * (gpd[in] - gpd_x)
                        * (y - xm2xi) / (xi * (y + xi) * (y - x));
                if ((2 * n) == in)
                    convint += f1;
                else {
                    double ya = yy[2 * n - 2];
                    double f1a = (shift - ya) * (x + xi)
                            * (gpd[2 * n - 2] - gpd_x) * (ya - xm2xi)
                            / (xi * (ya + xi) * (ya - x));
                    double f1b = shift * (x + xi) * (gpd[2 * n] - gpd_x) * xm2xi
                            / (xi2 * x);
                    convint += -0.25 * f1a + 3.0 * f1 + 1.25 * f1b;
                }
            }
            {
                // y = x
                convint += (-11.0 * gpd[ix] + 18.0 * gpd[ix + 1]
                        - 9.0 * gpd[ix + 2] + 2.0 * gpd[ix + 3])
                        / (3.0 * gamma);
            }
        }

        if ((2 * n - 2) == ix) {
            double y = yy[2 * n - 1];
            double f1 = (-2.0 * gpd[2 * n - 3] - 3.0 * gpd[2 * n - 2]
                    + 6.0 * gpd[2 * n - 1] - gpd[2 * n]) / (3.0 * gamma);
            double f2 = (shift - y) * (x + xi) * (gpd[2 * n - 1] - gpd_x)
                    * (y - xm2xi) / (xi * (y + xi) * (y - x));
            double f3 = shift * (gpd[2 * n] - gpd_x) * xm2xi * (x + xi)
                    / (xi2 * x);
            convint += f1 + 4.0 * f2 + f3;
        }

        if ((2 * n - 1) == ix) {
            double y = yy[2 * n - 2];
            double f1 = (shift - y) * (x + xi) * (gpd[2 * n - 2] - gpd_x)
                    * (y - xm2xi) / (xi * (y + xi) * (y - x));
            double f2 = (gpd[2 * n - 3] - 6.0 * gpd[2 * n - 2]
                    + 3.0 * gpd[2 * n - 1] + 2.0 * gpd[2 * n]) / (3.0 * gamma);
            double f3 = shift * (gpd[2 * n] - gpd_x) * xm2xi * (x + xi)
                    / (xi2 * x);
            convint += -0.25 * f1 + 2.0 * f2 + 1.25 * f3;
        }
        // And if x = 0 (ix = 2*n), this second integral is just zero!

        // Now integrate at 0 < y < 1.
        // The number of intervals is always even (= 2*n)
        double convint1 = 0.0;
        for (unsigned int i = 2 * n + 2; i < 4 * n; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * (x + xi) * (gpd[i] - gpd_x) * (y - xm2xi)
                    / (xi * (y + xi) * (y - x));
            convint += 2.0 * f1;
        }

        for (unsigned int i = 2 * n + 1; i < 4 * n; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * (x + xi) * (gpd[i] - gpd_x) * (y - xm2xi)
                    / (xi * (y + xi) * (y - x));
            convint += 4.0 * f1;
        }

        if (ix < 2 * n) {
            convint += shift * (gpd[2 * n] - gpd_x) * xm2xi * (x + xi)
                    / (xi2 * x);
        } else {
            convint += (-11.0 * gpd[2 * n] + 18.0 * gpd[2 * n + 1]
                    - 9.0 * gpd[2 * n + 2] + 2.0 * gpd[2 * n + 3])
                    / (3.0 * gamma);
        }

        // TODO: Unclear. Suppress block ?
        {
            // y = 1
            convint += -(shift + 1.0) * (x + xi) * gpd_x * (1.0 - xm2xi)
                    / (xi * (1.0 + xi) * (1.0 - x));
        }

        convint *= gamma * c_1_3;

        convint += gpd[ix]
                * (3.0 + 2.0 * log((1.0 - x * x) / (1.0 + xi))
                        + (x - xi) / xi * log(xi - x)
                        - (x + xi) / xi * log(xi + x));
        convint *= m_CF * 0.5;
        return convint;
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution at 0 < x < xi (2*n < i < 3*n).
    // The integral is given by a sum of three parts:  x < y < 1,
    // 0 < y < x and -1 < y < 0.
    if (ix < 3 * n) {
        double convint = 0.0;
        double gpd_x = gpd[ix];
        double xm2xi = x - 2.0 * xi;
        double xp2xi = x + 2.0 * xi;
        unsigned int in;

        // First integrate at x < y < 1
        if (0 == (ix % 2))
            in = 4 * n;
        else
            in = 4 * n - 1;
        for (unsigned int i = ix + 2; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * (x + xi) * (gpd[i] - gpd_x) * (y - xm2xi)
                    / (xi * (y + xi) * (y - x));
            convint += 2.0 * f1;
        }

        for (unsigned int i = ix + 1; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * (x + xi) * (gpd[i] - gpd_x) * (y - xm2xi)
                    / (xi * (y + xi) * (y - x));
            convint += 4.0 * f1;
        }

        // TODO: Suppress block.
        {
            double y = yy[in];
            double f1 = (shift + y) * (x + xi) * (gpd[in] - gpd_x) * (y - xm2xi)
                    / (xi * (y + xi) * (y - x));
            if ((4 * n) == in)
                convint += f1;
            else {
                double ya = yy[4 * n - 2];
                double f1a = (shift + ya) * (x + xi) * (gpd[4 * n - 2] - gpd_x)
                        * (ya - xm2xi) / (xi * (ya + xi) * (ya - x));
                double f1b = -(shift + 1.0) * (x + xi) * gpd_x * (1.0 - xm2xi)
                        / (xi * (1.0 + xi) * (1.0 - x));
                convint += -0.25 * f1a + 3.0 * f1 + 1.25 * f1b;
            }
        }

        // TODO: Suppress block.
        {
            // y = x
            convint += (-11.0 * gpd[ix] + 18.0 * gpd[ix + 1] - 9.0 * gpd[ix + 2]
                    + 2.0 * gpd[ix + 3]) / (3.0 * gamma);
        }

        // Integration at 0 < y < x
        if (ix > (2 * n + 2)) {
            if (0 == (ix % 2))
                in = ix;
            else
                in = ix - 1;
            for (unsigned int i = 2 * n + 2; i < in; i += 2) {
                double y = yy[i];
                double f1 = (shift + y) * (gpd[i] - gpd_x) * (x - xi)
                        * (y - xp2xi) / (xi * (y - xi) * (y - x));
                convint += 2.0 * f1;
            }
            for (unsigned int i = 2 * n + 1; i < in; i += 2) {
                double y = yy[i];
                double f1 = (shift + y) * (gpd[i] - gpd_x) * (x - xi)
                        * (y - xp2xi) / (xi * (y - xi) * (y - x));
                convint += 4.0 * f1;
            }

            if (ix == in) {
                convint += (2.0 * gpd[ix - 3] - 9.0 * gpd[ix - 2]
                        + 18.0 * gpd[ix - 1] - 11.0 * gpd[ix]) / (3.0 * gamma);
            }

            if ((ix - 1) == in) {
                double ya = yy[ix - 2];
                double yb = yy[ix - 1];
                double f1 = (shift + ya) * (gpd[ix - 2] - gpd_x) * (x - xi)
                        * (ya - xp2xi) / (xi * (ya - xi) * (ya - x));
                double f2 = (shift + yb) * (gpd[ix - 1] - gpd_x) * (x - xi)
                        * (yb - xp2xi) / (xi * (yb - xi) * (yb - x));
                double f3 = (2.0 * gpd[ix - 3] - 9.0 * gpd[ix - 2]
                        + 18.0 * gpd[ix - 1] - 11.0 * gpd[ix]) / (3.0 * gamma);
                convint += -0.25 * f1 + 3.0 * f2 + 1.25 * f3;
            }

            // TODO: Suppress block.
            {
                // y = 0
                convint += -shift * (gpd[2 * n] - gpd_x) * (x - xi) * xp2xi
                        / (xi2 * x);
            }
        }

        if ((2 * n + 1) == ix) {
            double y = yy[2 * n + 2];
            double f1 = -shift * (gpd[2 * n] - gpd_x) * (x - xi) * xp2xi
                    / (xi2 * x);
            double f2 = (2.0 * gpd[ix - 1] + 3.0 * gpd[ix] - 6.0 * gpd[ix + 1]
                    + gpd[ix + 2]) / (3.0 * gamma);
            double f3 = (shift + y) * (gpd[2 * n + 2] - gpd_x) * (x - xi)
                    * (y - xp2xi) / (xi * (y - xi) * (y - x));
            convint += 1.25 * f1 + 2.0 * f2 - 0.25 * f3;
        }

        if ((2 * n + 2) == ix) {
            double y = yy[2 * n + 1];
            double f1 = -shift * (gpd[2 * n] - gpd_x) * (x - xi) * xp2xi
                    / (xi2 * x);
            double f2 = (shift + y) * (gpd[2 * n + 1] - gpd_x) * (x - xi)
                    * (y - xp2xi) / (xi * (y - xi) * (y - x));
            double f3 = (2.0 * gpd[ix - 1] + 3.0 * gpd[ix] - 6.0 * gpd[ix + 1]
                    + gpd[ix + 2]) / (3.0 * gamma);
            convint += 1.25 * f1 + 2.0 * f2 - 0.25 * f3;
        }

        // Now integrate at -1 < y < 0
        // The number of intervals is even!!
        for (unsigned int i = 2; i < 2 * n; i += 2) {
            double y = yy[i];
            double f1 = (shift - y) * (gpd[i] - gpd_x) * (x - xi) * (y - xp2xi)
                    / (xi * (y - xi) * (y - x));
            convint += 2.0 * f1;
        }

        for (unsigned int i = 1; i < 2 * n; i += 2) {
            double y = yy[i];
            double f1 = (shift - y) * (gpd[i] - gpd_x) * (x - xi) * (y - xp2xi)
                    / (xi * (y - xi) * (y - x));
            convint += 4.0 * f1;
        }

        // TODO: Suppress block.
        {
            // y = -1
            convint += -(shift + 1.0) * gpd_x * (x - xi) * (-1.0 - xp2xi)
                    / (xi * (-1.0 - xi) * (-1.0 - x));
            ;
        }

        // TODO: Suppress block.
        {
            // y = 0
            double f1 = -shift * (gpd[2 * n] - gpd_x) * (x - xi) * xp2xi
                    / (xi2 * x);
        }

        convint *= gamma * c_1_3;

        convint += gpd[ix]
                * (3.0 + 2.0 * log((1.0 - x * x) / (1.0 + xi))
                        + (x - xi) / xi * log(xi - x)
                        - (x + xi) / xi * log(xi + x));

        convint *= 0.5 * m_CF;
        return convint;
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution at x = xi   (i = 3*n). If the number of intervals
    // is even, the usual Simpson's formula is used. If it is odd, the last
    // interval is integrated with trapecia.
    if (3 * n == ix) {
        double convint = 0.;
        double gpd_x = gpd[ix];
        unsigned int in;
        if (0 == (ix % 2))
            in = 4 * n;
        else
            in = 4 * n - 1;

        for (unsigned int i = ix + 2; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * (gpd[i] - gpd_x) / (y - xi);
            convint += 2.0 * f1;
        }

        for (unsigned int i = ix + 1; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * (gpd[i] - gpd_x) / (y - xi);
            convint += 4.0 * f1;
        }

        // TODO: Suppress block.
        {
            double y = yy[in];
            double f1 = (y + shift) * (gpd[in] - gpd_x) / (y - xi);
            if ((4 * n) == in)
                convint += f1;
            else {
                double ya = yy[4 * n - 2];
                double f1a = (shift + ya) * (gpd[4 * n - 2] - gpd_x)
                        / (ya - xi);
                double f1b = -(shift + 1.0) * gpd_x / (1.0 - xi);
                convint += -0.25 * f1a + 3.0 * f1 + 1.25 * f1b;
            }
        }

        // TODO: Suppress block.
        {
            // y = x
            convint += (-11.0 * gpd[ix] + 18.0 * gpd[ix + 1] - 9.0 * gpd[ix + 2]
                    + 2.0 * gpd[ix + 3]) / (6.0 * gamma);
        }

        convint = gamma * convint * c_1_3
                + gpd_x * (1.5 + log(0.5 * (1.0 - xi) / xi));

        convint *= m_CF;

        return convint;
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution at x > xi   (4*n-n1 <= i <= 4*n). If the number of intervals
    // is even, the usual Simpson's formula is used. If it is odd, the last
    // interval is integrated with trapecia.
    if (ix < (4 * n - 2)) {
        double convint = 0.;
        double x2m2xi2 = x2 - 2.0 * xi2;
        double gpd_x = gpd[ix];
        unsigned int in;
        if (0 == (ix % 2))
            in = 4 * n;
        else
            in = 4 * n - 1;

        for (unsigned int i = ix + 2; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * (gpd[i] - gpd_x) * (y * y + x2m2xi2)
                    / ((y - x) * (y * y - xi2));
            convint += 2.0 * f1;
        }

        for (unsigned int i = ix + 1; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * (gpd[i] - gpd_x) * (y * y + x2m2xi2)
                    / ((y - x) * (y * y - xi2));
            convint += 4.0 * f1;
        }

        // TODO: Suppress block.
        {
            double y = yy[in];
            double f1 = (shift + y) * (gpd[in] - gpd_x) * (y * y + x2m2xi2)
                    / ((y - x) * (y * y - xi2));
            if ((4 * n) == in)
                convint += f1;
            else {
                double ya = yy[4 * n - 2];
                double f1a = (shift + ya) * (gpd[4 * n - 2] - gpd_x)
                        * (ya * ya + x2m2xi2) / ((ya - x) * (ya * ya - xi2));
                double f1b = -(shift + 1.0) * gpd_x * (1.0 + x2m2xi2)
                        / ((1.0 - x) * (1.0 - xi2));
                convint += -0.25 * f1a + 3.0 * f1 + 1.25 * f1b;
            }
        }

        // TODO: Suppress block.
        {
            // y = x
            convint += (-11.0 * gpd[ix] + 18.0 * gpd[ix + 1] - 9.0 * gpd[ix + 2]
                    + 2.0 * gpd[ix + 3]) / (3.0 * gamma);
        }

        convint = gamma * convint * c_1_3
                + gpd[ix]
                        * (3.0 / 2.0 + 2.0 * log(1.0 - x)
                                + 0.5 * (x - xi) / xi
                                        * log((x - xi) * (1.0 + xi))
                                - 0.5 * (x + xi) / xi
                                        * log((xi + x) * (1.0 - xi)));

        convint *= m_CF;

        return convint;
    }

    printf(
            "The convolution code finished weird.\n Looks like ix is not in the right range (-1,1)\n");
    return 0.;
}

double VinnikovEvolQCDModel::convolSingletqqKernel(const unsigned int ix,
        const double xi, const int n, const double* const gpd,
        const double* const yy) {

    // Dimension of "gpd" and "yy" arrays must be 2n+1, i.e. "double gpd[2n+1]"
    // In the code it is assumed that gpd(x=0) = gpd(x=1) = 0.0 !!

    // This function computes the convolution integral
    // using logarithmic grid, i.e. something like this:

    // \int f(x) dx = \int f(exp(gamma*i)-shift)*gamma*exp(gamma*i) di

    // So that the (shifted) logarithmic scale in "x" is transformed into
    // the linear scale in "i". Thus, the standard Simpson method
    // can be used for integrand which has steep rise around zero.

    // More precise, the interval in "x" [0, 1] is presented as:

    // x = shift*exp(gamma*(i-2n)) - shift, i = 0 ... 2n

    // The coefficients "gamma" and "shift" are chosen so that the point
    // x = +-xi divided the pieces in two equal (in log scale) parts:

    // gamma = log(1/2 + 1/2/xi*sqrt(9*xi^2 - 12*xi+4))/n
    // shift = xi^2/(1-2*xi)

    // which yields:
    // which yields:
    // x[0] = 0, x[n] = xi, x[2n] = 1

    //----------------   Ok, lets start  -------------------

    if (n < 5) {
        printf(
                "It's not a good idea to choose n < 5.\n I am too serious code to even start calculation at such conditions.\n So, all you get now is return 0.0;\n");
        return 0.;
    }

    double const x = yy[ix];
    double const x2 = x * x;
    double const xi2 = xi * xi;
    double const c_1_3 = 1.0 / 3.0;
    double const shift = xi2 / (1.0 - 2.0 * xi);
    double const gamma = log(0.5 + 0.5 / xi * sqrt(9.0 * xi2 - 12.0 * xi + 4.0))
            / ((double) n);

    //----------------   Ok, lets start  -------------------
    // First, consider separately some close to boundary cases

    if (0 == ix)
        return 0.;

    if ((2 * n) == ix)
        return 0.;

    if ((2 * n - 1) == ix) {
        double x2m2xi2 = x2 - 2.0 * xi2;
        double y = yy[2 * n - 2];
        double y2 = y * y;
        double f1 = (shift + y) * (gpd[2 * n - 2] - gpd[ix]) * (x2m2xi2 + y2)
                / ((y - x) * (y2 - xi2));
        double f2 = (gpd[2 * n - 3] - 6.0 * gpd[2 * n - 2]
                + 3.0 * gpd[2 * n - 1]) / (3.0 * gamma);
        double f3 = -(shift + 1.0) * gpd[ix] * (x2m2xi2 + 1.0)
                / ((1.0 - x) * (1.0 - xi2));
        return m_CF
                * (c_1_3 * gamma * (-0.25 * f1 + 2.0 * f2 + 1.25 * f3)
                        + gpd[ix]
                                * (1.5 + 2.0 * log(1.0 - x)
                                        + (x - xi) / (2.0 * xi)
                                                * log((x - xi) * (1.0 + xi))
                                        - (x + xi) / (2.0 * xi)
                                                * log((x + xi) * (1.0 - xi))));
    }

    if ((2 * n - 2) == ix) {
        double x2m2xi2 = x2 - 2.0 * xi2;
        double y = yy[2 * n - 1];
        double y2 = y * y;
        double f1 = (-2.0 * gpd[2 * n - 3] - 3.0 * gpd[2 * n - 2]
                + 6.0 * gpd[2 * n - 1]) / (3.0 * gamma);
        double f2 = (shift + y) * (gpd[2 * n - 1] - gpd[ix]) * (x2m2xi2 + y2)
                / ((y - x) * (y2 - xi2));
        double f3 = -(shift + 1.0) * gpd[ix] * (x2m2xi2 + 1.0)
                / ((1.0 - x) * (1.0 - xi2));
        return m_CF
                * (c_1_3 * gamma * (f1 + 4.0 * f2 + f3)
                        + gpd[ix]
                                * (1.5 + 2.0 * log(1.0 - x)
                                        + (x - xi) / (2.0 * xi)
                                                * log((x - xi) * (1.0 + xi))
                                        - (x + xi) / (2.0 * xi)
                                                * log((x + xi) * (1.0 - xi))));
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution for 0 < x < xi (2 < i < n).
    // The integral is given by a sum of three parts: 0 < y < x
    // and x < y < 1
    if (ix < n) {
        double convint = 0.0;
        double gpd_x = gpd[ix];
        unsigned int in, i0;

        // first integrate at 0 < y < x
        if (1 == ix) {
            double y = yy[2];
            double y2 = y * y;

            double f1 = (shift + x)
                    * (-2.0 * (xi + x) * x * (gpd[1] - gpd[n]) / (xi2 - x2))
                    - 2.0 * xi * (-3.0 * gpd[1] + 6.0 * gpd[2] - 1.0 * gpd[3])
                            / (6.0 * gamma);
            double f2 = (shift + y)
                    * (-2.0 * (xi + x) * y * (gpd[2] - gpd[n]) / (xi2 - y2)
                            - 4.0 * xi * y * (gpd[2] - gpd_x) / (y2 - x2));
            convint += 2.0 * f1 - 0.25 * f2;
        }

        if (2 == ix) {
            double y = yy[1];
            double y2 = y * y;

            double f1 = (shift + y)
                    * (-2.0 * (xi + x) * y * (gpd[1] - gpd[n]) / (xi2 - y2)
                            - 4.0 * xi * y * (gpd[1] - gpd_x) / (y2 - x2));
            double f2 = (shift + x)
                    * (-2.0 * (xi + x) * x * (gpd[ix] - gpd[n]) / (xi2 - x2))
                    - 2.0 * xi * (-6.0 * gpd[1] + 3.0 * gpd[2] + 2.0 * gpd[3])
                            / (6.0 * gamma);
            convint += 4.0 * f1 + f2;
        }

        if (ix > 2) {
            if (0 == (ix % 2))
                in = ix;
            else
                in = ix - 1;

            for (unsigned int i = 2; i < in; i += 2) {
                double y = yy[i];
                double y2 = y * y;
                double f1 = (shift + y)
                        * (-2.0 * (xi + x) * y * (gpd[i] - gpd[n]) / (xi2 - y2)
                                - 4.0 * xi * y * (gpd[i] - gpd_x) / (y2 - x2));
                convint += 2.0 * f1;
            }

            for (unsigned int i = 1; i < in; i += 2) {
                double y = yy[i];
                double y2 = y * y;
                double f1 = (shift + y)
                        * (-2.0 * (xi + x) * y * (gpd[i] - gpd[n]) / (xi2 - y2)
                                - 4.0 * xi * y * (gpd[i] - gpd_x) / (y2 - x2));
                convint += 4.0 * f1;
            }

            if (ix == in) {
                convint +=
                        (shift + x)
                                * (-2.0 * (xi + x) * x * (gpd[ix] - gpd[n])
                                        / (xi2 - x2))
                                - 2.0 * xi
                                        * (-2.0 * gpd[ix - 3]
                                                + 9.0 * gpd[ix - 2]
                                                - 18.0 * gpd[ix - 1]
                                                + 11.0 * gpd[ix])
                                        / (6.0 * gamma);
            }

            if ((ix - 1) == in) {
                double ya = yy[ix - 2];
                double ya2 = ya * ya;
                double yb = yy[ix - 1];
                double yb2 = yb * yb;
                double f1 = (shift + ya)
                        * (-2.0 * (xi + x) * ya * (gpd[ix - 2] - gpd[n])
                                / (xi2 - ya2)
                                - 4.0 * xi * ya * (gpd[ix - 2] - gpd_x)
                                        / (ya2 - x2));
                double f2 = (shift + yb)
                        * (-2.0 * (xi + x) * yb * (gpd[ix - 1] - gpd[n])
                                / (xi2 - yb2)
                                - 4.0 * xi * yb * (gpd[ix - 1] - gpd_x)
                                        / (yb2 - x2));
                double f3 =
                        (shift + x)
                                * (-2.0 * (xi + x) * x * (gpd[ix] - gpd[n])
                                        / (xi2 - x2))
                                - 2.0 * xi
                                        * (-2.0 * gpd[ix - 3]
                                                + 9.0 * gpd[ix - 2]
                                                - 18.0 * gpd[ix - 1]
                                                + 11.0 * gpd[ix])
                                        / (6.0 * gamma);
                convint += -0.25 * f1 + 3.0 * f2 + 1.25 * f3;
            }

            {
                // y = 0
                convint += 0.;
            }

        }

        // now integrate at x < y < 1
        if (0 == (ix % 2))
            i0 = ix;
        else
            i0 = ix + 1;

        for (unsigned int i = i0 + 2; i < 2 * n; i += 2) {
            double y = yy[i];
            double f1 = (shift + y)
                    * (2.0 * x * (gpd[i] - gpd_x) / (xi + y)
                            + 4.0 * xi * x * (gpd[i] - gpd_x) / (y * y - x2));
            convint += 2.0 * f1;
        }

        for (unsigned int i = i0 + 1; i < 2 * n; i += 2) {
            double y = yy[i];
            double f1 = (shift + y)
                    * (2.0 * x * (gpd[i] - gpd_x) / (xi + y)
                            + 4.0 * xi * x * (gpd[i] - gpd_x) / (y * y - x2));
            convint += 4.0 * f1;
        }

        {
            convint += (shift + 1.0)
                    * (2.0 * x * (-gpd_x) / (xi + 1.0)
                            + 4.0 * xi * x * (-gpd_x) / (1.0 - x2));
        }

        if (ix == i0) {
            convint += 2.0 * xi
                    * (-11.0 * gpd[ix] + 18.0 * gpd[ix + 1] - 9.0 * gpd[ix + 2]
                            + 2.0 * gpd[ix + 3]) / (6.0 * gamma);
        }

        if ((ix + 1) == i0) {
            double ya = yy[ix + 1];
            double yb = yy[ix + 2];
            double f0 = 2.0 * xi
                    * (-11.0 * gpd[ix] + 18.0 * gpd[ix + 1] - 9.0 * gpd[ix + 2]
                            + 2.0 * gpd[ix + 3]) / (6.0 * gamma);
            double f1 = (shift + ya)
                    * (2.0 * x * (gpd[ix + 1] - gpd_x) / (xi + ya)
                            + 4.0 * xi * x * (gpd[ix + 1] - gpd_x)
                                    / (ya * ya - x2));
            double f2 = (shift + yb)
                    * (2.0 * x * (gpd[ix + 2] - gpd_x) / (xi + yb)
                            + 4.0 * xi * x * (gpd[ix + 2] - gpd_x)
                                    / (yb * yb - x2));
            convint += 1.25 * f0 + 3.0 * f1 - 0.25 * f2;
        }

        convint *= c_1_3 * gamma;
        convint += 2.0 * xi * gpd[ix]
                * (log((1.0 - x) * x2 / (xi2 * (1.0 + x))) + 1.5);
        convint += 2.0 * x * gpd[ix] * log((xi + 1.0) / (xi + x));
        convint += -2.0 * xi * (gpd[n] - gpd[ix]) * log(xi2 / (xi2 - x2));
        convint += (xi - x) * gpd[n] * log(xi2 / (xi2 - x2));
        convint *= m_CF / (2.0 * xi);

        return convint;
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution at x = xi  (i = n)
    if (ix == n) {
        double convint = 0.;
        double const gpd_n = gpd[n];
        unsigned int in;

        if (0 == (ix % 2))
            in = 2 * n;
        else
            in = 2 * n - 1;

        for (unsigned int i = ix + 2; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * (gpd[i] - gpd_n) / (y - xi);
            convint += 2.0 * f1;
        }

        for (unsigned int i = ix + 1; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * (gpd[i] - gpd_n) / (y - xi);
            convint += 4.0 * f1;
        }

        {
            double y = yy[in];
            double f1 = (shift + y) * (gpd[in] - gpd_n) / (y - xi);
            convint += f1;

            if ((2 * n - 1) == in) {
                double ya = yy[2 * n - 2];
                double f1a = (shift + ya) * (gpd[2 * n - 2] - gpd_n)
                        / (ya - xi);
                double f1b = -(shift + 1.0) * gpd_n / (1.0 - xi);
                convint += 2.0 * f1 - 0.25 * f1a + 1.25 * f1b;
            }
        }

        {
            // y = x
            convint += (-11.0 * gpd[ix] + 18.0 * gpd[ix + 1] - 9.0 * gpd[ix + 2]
                    + 2.0 * gpd[ix + 3]) / (6.0 * gamma);
        }

        convint *= gamma * c_1_3;
        convint += gpd[ix] * (1.5 + log((1.0 - xi) / (2.0 * xi)));
        convint *= m_CF;

        return convint;
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution at x > xi   (n < i <= 2*n).
    if (ix < (2 * n - 2)) {
        double convint = 0.;
        double const gpd_x = gpd[ix];
        double const x2m2xi2 = x2 - 2.0 * xi2;
        unsigned int in;

        if (0 == (ix % 2))
            in = 2 * n;
        else
            in = 2 * n - 1;

        for (unsigned int i = ix + 2; i < in; i += 2) {
            double y = yy[i];
            double y2 = y * y;
            double f1 = (shift + y) * (gpd[i] - gpd_x) * (x2m2xi2 + y2)
                    / ((y - x) * (y2 - xi2));
            convint += 2.0 * f1;
        }

        for (unsigned int i = ix + 1; i < in; i += 2) {
            double y = yy[i];
            double y2 = y * y;
            double f1 = (shift + y) * (gpd[i] - gpd_x) * (x2m2xi2 + y2)
                    / ((y - x) * (y2 - xi2));
            convint += 4.0 * f1;
        }

        {
            double y = yy[in];
            double y2 = y * y;
            double f1 = (shift + y) * (gpd[in] - gpd_x) * (x2m2xi2 + y2)
                    / ((y - x) * (y2 - xi2));
            convint += f1;

            if ((2 * n - 1) == in) {
                double ya = yy[2 * n - 2];
                double ya2 = ya * ya;
                double f1a = (shift + ya) * (gpd[2 * n - 2] - gpd_x)
                        * (x2m2xi2 + ya2) / ((ya - x) * (ya2 - xi2));
                double f1b = -(shift + 1.0) * gpd_x * (x2m2xi2 + 1.0)
                        / ((1.0 - x) * (1.0 - xi2));
                convint += 2.0 * f1 - 0.25 * f1a + 1.25 * f1b;
            }
        }

        {
            // y = x
            convint += (-11.0 * gpd[ix] + 18.0 * gpd[ix + 1] - 9.0 * gpd[ix + 2]
                    + 2.0 * gpd[ix + 3]) / (3.0 * gamma);
        }

        convint *= gamma * c_1_3;
        convint += gpd[ix]
                * (1.5 + 2.0 * log(1.0 - x)
                        + (x - xi) / (2.0 * xi) * log((x - xi) * (1.0 + xi))
                        - (x + xi) / (2.0 * xi) * log((x + xi) * (1.0 - xi)));
        convint *= m_CF;

        return convint;
    }

    printf(
            "The convolution code finished weird.\n Looks like ix is not in the right range [0,2n]\n");
    return 0.;
}

double VinnikovEvolQCDModel::convolSingletggKernel(const unsigned int ix,
        const double xi, const int n, const double* const gpd,
        const double* const yy) {

    // Dimension of "gpd" and "yy" arrays must be 2n+1, i.e. "double gpd[2n+1]"
    // In the code it is assumed that gpd(x=1) = 0.0 !!

    // This function computes the convolution integral
    // using logarithmic grid, i.e. something like this:

    // \int f(x) dx = \int f(exp(gamma*i)-shift)*gamma*exp(gamma*i) di

    // So that the (shifted) logarithmic scale in "x" is transformed into
    // the linear scale in "i". Thus, the standard Simpson method
    // can be used for integrand which has steep rise around zero.

    // More precise, the interval in "x" [0, 1] is presented as:

    // x = shift*exp(gamma*(i-2n)) - shift, i = 0 ... 2n

    // The coefficients "gamma" and "shift" are chosen so that the point
    // x = +-xi divided the pieces in two equal (in log scale) parts:

    // gamma = log(1/2 + 1/2/xi*sqrt(9*xi^2 - 12*xi+4))/n
    // shift = xi^2/(1-2*xi)

    // which yields:
    // which yields:
    // x[0] = 0, x[n] = xi, x[2n] = 1

    //----------------   Ok, lets start  -------------------

    if (n < 5) {
        printf(
                "It's not a good idea to choose n < 5.\n I am too serious code to even start calculation at such conditions.\n So, all you get now is return 0.0;\n");
        return 0.;
    }

    double const x = yy[ix];
    double const x2 = x * x;
    double const xi2 = xi * xi;
    double const c_1_3 = 1.0 / 3.0;
    double const shift = xi2 / (1.0 - 2.0 * xi);
    double const gamma = log(0.5 + 0.5 / xi * sqrt(9.0 * xi2 - 12.0 * xi + 4.0))
            / ((double) n);

    //----------------   Ok, lets start  -------------------

    // First, consider separately some close to boundary cases
    if ((2 * n) == ix)
        return 0.0;

    if ((2 * n - 1) == ix) {
        double xi2mx2 = xi2 - x2;
        double y = yy[2 * n - 2];
        double y2 = y * y;
        double f1 = (shift + y) * (gpd[2 * n - 2] - gpd[ix])
                * (xi2mx2 * (xi2 - y2) + (x2 + y2) * (x - y) * (x - y))
                / ((xi2 - y2) * (xi2 - y2) * (y - x));
        double f2 = (gpd[2 * n - 3] - 6.0 * gpd[2 * n - 2]
                + 3.0 * gpd[2 * n - 1]) / (6.0 * gamma);
        double f3 = -(shift + 1.0) * gpd[ix]
                * (xi2mx2 * (xi2 - 1.0) + (x2 + 1.0) * (x - 1.0) * (x - 1.0))
                / ((xi2 - 1.0) * (xi2 - 1.0) * (1.0 - x));
        return 2.0 * m_CA
                * (c_1_3 * gamma * (-0.25 * f1 + 2.0 * f2 + 1.25 * f3)
                        + gpd[ix]
                                * (2.0 * (x2 + xi2) * (1.0 - x)
                                        / (xi * xi2 - xi)
                                        + (2.0 * xi - x) * (xi + x) * (xi + x)
                                                / xi2
                                                * log((1.0 + xi) / (x + xi))
                                        - 2.0 * xi
                                                * log(
                                                        (1.0 - xi2)
                                                                / ((1.0 - x)
                                                                        * (1.0
                                                                                - x)))
                                        + (2.0 * xi + x) * (xi - x) * (xi - x)
                                                / xi2
                                                * log((1.0 - xi) / (x - xi))
                                        + 14.0 * c_1_3 * xi) * 0.25 / xi)
                + (-0.5 * m_CA - 2.0 * c_1_3 * m_TR) * gpd[ix];
    }

    if ((2 * n - 2) == ix) {
        double xi2mx2 = xi2 - x2;
        double y = yy[2 * n - 1];
        double y2 = y * y;
        double f1 = (-2.0 * gpd[2 * n - 3] - 3.0 * gpd[2 * n - 2]
                + 6.0 * gpd[2 * n - 1]) / (6.0 * gamma);
        double f2 = (shift + y) * (gpd[2 * n - 1] - gpd[ix])
                * (xi2mx2 * (xi2 - y2) + (x2 + y2) * (x - y) * (x - y))
                / ((xi2 - y2) * (xi2 - y2) * (y - x));
        double f3 = -(shift + 1.0) * gpd[ix]
                * (xi2mx2 * (xi2 - 1.0) + (x2 + 1.0) * (x - 1.0) * (x - 1.0))
                / ((xi2 - 1.0) * (xi2 - 1.0) * (1.0 - x));
        return 2.0 * m_CA
                * (c_1_3 * gamma * (f1 + 4.0 * f2 + f3)
                        + gpd[ix]
                                * (2.0 * (x2 + xi2) * (1.0 - x)
                                        / (xi * xi2 - xi)
                                        + (2.0 * xi - x) * (xi + x) * (xi + x)
                                                / xi2
                                                * log((1.0 + xi) / (x + xi))
                                        - 2.0 * xi
                                                * log(
                                                        (1.0 - xi2)
                                                                / ((1.0 - x)
                                                                        * (1.0
                                                                                - x)))
                                        + (2.0 * xi + x) * (xi - x) * (xi - x)
                                                / xi2
                                                * log((1.0 - xi) / (x - xi))
                                        + 14.0 / 3.0 * xi) * 0.25 / xi)
                + (-0.5 * m_CA - 2.0 * c_1_3 * m_TR) * gpd[ix];
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution for 0 < x < xi (2 < i < n).
    // The integral is given by a sum of three parts: 0 < y < x
    // and x < y < 1
    double convint = 0.0;
    unsigned int i0, in;

    if (ix < n) {
        double const gpd_x = gpd[ix];
        double const gpd_n = gpd[n];
        double const one_xipx2 = 1.0 / ((xi + x) * (xi + x));
        double const xi2mx2_2 = (xi2 - x2) * (xi2 - x2);
        double const sxi2p4xix = 6.0 * xi2 + 4.0 * xi * x;
        double const xi2x = xi2 * x;
        double const txi2mx2 = 2.0 * xi2 - x2;
        double const txipx = 2.0 * xi + x;
        double const fxi = 4.0 * xi;
        double const sxi2m2x2 = 6.0 * xi2 - 2.0 * x2;
        double const txix2 = 2.0 * xi * x2;
        double const xi2px2 = xi2 + x2;
        double const l_xipx_ximx = log((xi + x) / (xi - x));

        // first integrate at 0 < y < x
        if (1 == ix) {
            double y = yy[2];
            double y2 = y * y;

            double f0 = shift
                    * ((gpd[0] - gpd_n) / (xi2 * xi2) * one_xipx2
                            * (xi2mx2_2 * sxi2p4xix - fxi * txi2mx2 * xi2x)
                            + (gpd[0] - gpd_x) * fxi * one_xipx2 * xi2x / x2);
            double f1 = (shift + x) * (gpd[1] - gpd_n)
                    / ((xi2 - x2) * (xi2 - x2)) * one_xipx2
                    * (xi2mx2_2 * (sxi2p4xix - 2.0 * x2)
                            - fxi * (txi2mx2 - x2) * (xi2x + txipx * x2))
                    - 2.0 * xi
                            * (-2.0 * gpd[0] - 3.0 * gpd[1] + 6.0 * gpd[2]
                                    - 1.0 * gpd[3]) / (6.0 * gamma);
            double f2 =
                    (shift + y)
                            * ((gpd[2] - gpd_n) / ((xi2 - y2) * (xi2 - y2))
                                    * one_xipx2
                                    * (xi2mx2_2 * (sxi2p4xix - 2.0 * y2)
                                            - fxi * (txi2mx2 - y2)
                                                    * (xi2x + txipx * y2))
                                    - (gpd[2] - gpd_x) * fxi * one_xipx2
                                            * (xi2x + txipx * y2) / (y2 - x2));
            convint += 1.25 * f0 + 2.0 * f1 - 0.25 * f2;
        }

        if (2 == ix) {
            double y = yy[1];
            double y2 = y * y;

            double f0 = shift
                    * ((gpd[0] - gpd_n) / (xi2 * xi2) * one_xipx2
                            * (xi2mx2_2 * sxi2p4xix - fxi * txi2mx2 * xi2x)
                            + (gpd[0] - gpd_x) * fxi * one_xipx2 * xi2x / x2);
            double f1 =
                    (shift + y)
                            * ((gpd[1] - gpd_n) / ((xi2 - y2) * (xi2 - y2))
                                    * one_xipx2
                                    * (xi2mx2_2 * (sxi2p4xix - 2.0 * y2)
                                            - fxi * (txi2mx2 - y2)
                                                    * (xi2x + txipx * y2))
                                    - (gpd[1] - gpd_x) * fxi * one_xipx2
                                            * (xi2x + txipx * y2) / (y2 - x2));
            double f2 = (shift + x) * (gpd[2] - gpd_n)
                    / ((xi2 - x2) * (xi2 - x2)) * one_xipx2
                    * (xi2mx2_2 * (sxi2p4xix - 2.0 * x2)
                            - fxi * (txi2mx2 - x2) * (xi2x + txipx * x2))
                    - 2.0 * xi
                            * (gpd[0] - 6.0 * gpd[1] + 3.0 * gpd[2]
                                    + 2.0 * gpd[3]) / (6.0 * gamma);
            convint += f0 + 4.0 * f1 + f2;
        }

        if (ix > 2) {
            if (0 == (ix % 2))
                i0 = 0;
            else
                i0 = 1;

            for (unsigned int i = i0 + 2; i < ix; i += 2) {
                double y = yy[i];
                double y2 = y * y;
                double f1 = (shift + y)
                        * ((gpd[i] - gpd_n) / ((xi2 - y2) * (xi2 - y2))
                                * one_xipx2
                                * (xi2mx2_2 * (sxi2p4xix - 2.0 * y2)
                                        - fxi * (txi2mx2 - y2)
                                                * (xi2x + txipx * y2))
                                - (gpd[i] - gpd_x) * fxi * one_xipx2
                                        * (xi2x + txipx * y2) / (y2 - x2));
                convint += 2.0 * f1;
            }

            for (unsigned int i = i0 + 1; i < ix; i += 2) {
                double y = yy[i];
                double y2 = y * y;
                double f1 = (shift + y)
                        * ((gpd[i] - gpd_n) / ((xi2 - y2) * (xi2 - y2))
                                * one_xipx2
                                * (xi2mx2_2 * (sxi2p4xix - 2.0 * y2)
                                        - fxi * (txi2mx2 - y2)
                                                * (xi2x + txipx * y2))
                                - (gpd[i] - gpd_x) * fxi * one_xipx2
                                        * (xi2x + txipx * y2) / (y2 - x2));
                convint += 4.0 * f1;
            }

            {
                // y = x
                convint += (shift + x) * (gpd_x - gpd_n)
                        / ((xi2 - x2) * (xi2 - x2)) * one_xipx2
                        * (xi2mx2_2 * (sxi2p4xix - 2.0 * x2)
                                - fxi * (txi2mx2 - x2) * (xi2x + txipx * x2))
                        - 2.0 * xi
                                * (-2.0 * gpd[ix - 3] + 9.0 * gpd[ix - 2]
                                        - 18.0 * gpd[ix - 1] + 11.0 * gpd[ix])
                                / (6.0 * gamma);
            }

            if (1 == i0) {
                double ya = yy[1];
                double ya2 = ya * ya;
                double yb = yy[2];
                double yb2 = yb * yb;

                double f0 =
                        shift
                                * ((gpd[0] - gpd_n) / (xi2 * xi2) * one_xipx2
                                        * (xi2mx2_2 * sxi2p4xix
                                                - fxi * txi2mx2 * xi2x)
                                        + (gpd[0] - gpd_x) * fxi * one_xipx2
                                                * xi2x / x2);
                double f1 = (shift + ya)
                        * ((gpd[1] - gpd_n) / ((xi2 - ya2) * (xi2 - ya2))
                                * one_xipx2
                                * (xi2mx2_2 * (sxi2p4xix - 2.0 * ya2)
                                        - fxi * (txi2mx2 - ya2)
                                                * (xi2x + txipx * ya2))
                                - (gpd[1] - gpd_x) * fxi * one_xipx2
                                        * (xi2x + txipx * ya2) / (ya2 - x2));
                double f2 = (shift + yb)
                        * ((gpd[2] - gpd_n) / ((xi2 - yb2) * (xi2 - yb2))
                                * one_xipx2
                                * (xi2mx2_2 * (sxi2p4xix - 2.0 * yb2)
                                        - fxi * (txi2mx2 - yb2)
                                                * (xi2x + txipx * yb2))
                                - (gpd[1] - gpd_x) * fxi * one_xipx2
                                        * (xi2x + txipx * yb2) / (yb2 - x2));
                convint += 1.25 * f0 + 2.0 * f1 - 0.25 * f2;
            }

            {
                // i = i0
                double y = yy[i0];
                double y2 = y * y;
                convint += (shift + y)
                        * ((gpd[i0] - gpd_n) / ((xi2 - y2) * (xi2 - y2))
                                * one_xipx2
                                * (xi2mx2_2 * (sxi2p4xix - 2.0 * y2)
                                        - fxi * (txi2mx2 - y2)
                                                * (xi2x + txipx * y2))
                                - (gpd[i0] - gpd_x) * fxi * one_xipx2
                                        * (xi2x + txipx * y2) / (y2 - x2));
            }

        }

        // now integrate at x < y < 1
        if (0 == (ix % 2))
            i0 = ix;
        else
            i0 = ix + 1;

        for (unsigned int i = i0 + 2; i < 2 * n; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) / ((xi + y) * (xi + y) * (y * y - x2))
                    * (gpd[i] - gpd_x)
                    * ((sxi2m2x2 + fxi * y) * (y * y - x2)
                            + fxi * (xi2px2 * y + txix2));
            convint += 2.0 * f1;
        }

        for (unsigned int i = i0 + 1; i < 2 * n; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) / ((xi + y) * (xi + y) * (y * y - x2))
                    * (gpd[i] - gpd_x)
                    * ((sxi2m2x2 + fxi * y) * (y * y - x2)
                            + fxi * (xi2px2 * y + txix2));
            convint += 4.0 * f1;
        }

        {
            // y = 1
            double f1 = -(shift + 1.0) / ((xi + 1.0) * (xi + 1.0) * (1.0 - x2))
                    * gpd_x
                    * ((sxi2m2x2 + fxi) * (1.0 - x2) + fxi * (xi2px2 + txix2));
            convint += f1;
        }

        if (ix == i0) {
            if (x > 0.95 * xi) {
                double ya = yy[ix + 1];
                double f1 = (shift + ya)
                        / ((xi + ya) * (xi + ya) * (ya * ya - x2))
                        * (gpd[ix + 1] - gpd_x)
                        * ((6.0 * xi2 + 4.0 * xi * ya - 2.0 * x2)
                                * (ya * ya - x2)
                                + 4.0 * xi * ((xi2 + x2) * ya + 2.0 * xi * x2));
                convint += f1;
            } else {
                convint += 2.0 * xi
                        * (-11.0 * gpd[ix] + 18.0 * gpd[ix + 1]
                                - 9.0 * gpd[ix + 2] + 2.0 * gpd[ix + 3])
                        / (6.0 * gamma);
            }
        }

        if ((ix + 1) == i0) {
            double ya = yy[ix + 1];
            double yb = yy[ix + 2];
            double f0;

            if (x > 0.95 * xi) {
                f0 = (shift + ya) / ((xi + ya) * (xi + ya)) * 4.0 * xi
                        * ((xi2 + x2) * ya + 2.0 * xi * x2)
                        * (gpd[ix + 1] - gpd_x) / (ya * ya - x2);
            } else {
                f0 = 2.0 * xi
                        * (-11.0 * gpd[ix] + 18.0 * gpd[ix + 1]
                                - 9.0 * gpd[ix + 2] + 2.0 * gpd[ix + 3])
                        / (6.0 * gamma);
            }
            double f1 = (shift + ya) / ((xi + ya) * (xi + ya))
                    * ((6.0 * xi2 + 4.0 * xi * ya - 2.0 * x2)
                            * (gpd[ix + 1] - gpd_x)
                            + 4.0 * xi * ((xi2 + x2) * ya + 2.0 * xi * x2)
                                    * (gpd[ix + 1] - gpd_x) / (ya * ya - x2));
            double f2 = (shift + yb) / ((xi + yb) * (xi + yb))
                    * ((6.0 * xi2 + 4.0 * xi * yb - 2.0 * x2)
                            * (gpd[ix + 2] - gpd_x)
                            + 4.0 * xi * ((xi2 + x2) * yb + 2.0 * xi * x2)
                                    * (gpd[ix + 2] - gpd_x) / (yb * yb - x2));
            convint += 3.0 * f1 + 1.25 * f0 - 0.25 * f2;
        }

        convint *= c_1_3 * gamma;
        convint += 2.0 * xi * gpd[ix]
                * (log((1.0 - x2) / ((xi + 1.0) * (xi + 1.0)))
                        - 2.0 / (1.0 + xi) + 7.0 * c_1_3);
        convint += -fxi * (gpd_n - gpd_x) / (xi + x)
                * (-0.5 * (xi - x) * l_xipx_ximx + x)
                - fxi / ((xi + x) * (xi + x)) * (gpd_n - gpd_x)
                        * (xi * (x + xi) * l_xipx_ximx - x * (2.0 * xi + x));
        convint += gpd[n]
                * ((xi - x) * 2.0 * x / xi
                        + (xi - x) * (xi - x) / xi2 * (2.0 * xi + x)
                                * l_xipx_ximx);
        convint += gpd_x
                * (2.0 * (xi - x) * (1.0 - x) / (1.0 + xi)
                        + 4.0 * xi * log((xi + 1.0) / (xi + x)));
        convint *= m_CA / (2.0 * xi);
        convint += gpd[ix] * (-0.5 * m_CA - 2.0 * c_1_3 * m_TR);

        return convint;
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution at x = xi  (i = n)
    if (ix == n) {
        double convint = 0.0;
        double const gpd_x = gpd[ix];
        unsigned int i0;

        if (0 == (ix % 2))
            i0 = ix;
        else
            i0 = ix + 1;

        // return 2.0*conv_int_gg(n+1, xi, n, gpd, yy)
        //- conv_int_gg(n+2, xi, n, gpd, yy);

        for (unsigned int i = i0 + 2; i < 2 * n; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * (gpd[i] - gpd[n])
                    / ((xi - y) * (xi + y) * (xi + y)) * 2.0 * y * xi
                    + (shift + y) * (gpd[i] - gpd_x) / (y - xi);
            convint += 2.0 * f1;
        }

        for (unsigned int i = i0 + 1; i < 2 * n; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * (gpd[i] - gpd[n])
                    / ((xi - y) * (xi + y) * (xi + y)) * 2.0 * y * xi
                    + (shift + y) * (gpd[i] - gpd_x) / (y - xi);
            convint += 4.0 * f1;
        }

        {
            // y = 1
            convint += (shift + 1.0) * (-gpd[n])
                    / ((xi - 1.0) * (xi + 1.0) * (xi + 1.0)) * 2.0 * xi
                    + (shift + 1.0) * (-gpd_x) / (1.0 - xi);
        }

        if (ix == i0) {
            convint += (-11.0 * gpd[ix + 1] + 18.0 * gpd[ix + 2]
                    - 9.0 * gpd[ix + 3] + 2.0 * gpd[ix + 4]) / (12.0 * gamma);
        }

        if ((ix + 1) == i0) {
            double ya = yy[ix + 1];
            double yb = yy[ix + 2];
            double f0 = (-11.0 * gpd[ix + 1] + 18.0 * gpd[ix + 2]
                    - 9.0 * gpd[ix + 3] + 2.0 * gpd[ix + 4]) / (12.0 * gamma);
            double f1 = (shift + ya) * (gpd[ix + 1] - gpd[n])
                    / ((xi - ya) * (xi + ya) * (xi + ya)) * 2.0 * ya * xi
                    + (shift + ya) * (gpd[ix + 1] - gpd_x) / (ya - xi);
            double f2 = (shift + yb) * (gpd[ix + 2] - gpd[n])
                    / ((xi - yb) * (xi + yb) * (xi + yb)) * 2.0 * yb * xi
                    + (shift + yb) * (gpd[ix + 2] - gpd_x) / (yb - xi);
            convint += 3.0 * f1 - 0.25 * f2 + 1.25 * f0;
        }

        convint *= gamma * c_1_3;
        convint += gpd[ix]
                * (4.0 * xi2 * (1.0 - xi) / (xi * xi2 - xi)
                        + 4.0 * xi * log((1.0 + xi) / (2.0 * xi))
                        - 2.0 * xi * log((1.0 + xi) / (1.0 - xi))
                        + 14.0 * c_1_3 * xi) * 0.25 / xi;

        // convint += (gpd[n]-gpd[ix])*log((xi-x)*(xi+1.0)/((xi+x)*(xi-1.0)))
        // *(3.0*xi2*x-x2*x)/(4.0*xi2*xi);
        // convint += -(gpd[n]-gpd[ix])*(x2+xi2)*(1.0-x)/((xi+x)*(xi+1.0)*2.0*xi);
        // convint += -(gpd[n]-gpd[ix])*(x2+xi2)
        // *((x-xi)/(xi2-1.0) + x/(xi+x))/(2.0*xi2) ;

        convint *= 2.0 * m_CA;
        convint += (-0.5 * m_CA - 2.0 * c_1_3 * m_TR) * gpd[ix];

        return convint;
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution at x > xi   (n < i <= 2*n).
    if (ix < (2 * n - 2)) {
        double convint = 0.0;
        double const gpd_x = gpd[ix];
        double const gpd_n = gpd[n];
        double const xi2px2 = xi2 + x2;
        double const tx = 2.0 * x;
        double const xxi2mx3 = x * xi2 - x2 * x;

        unsigned int in;

        if (0 == (ix % 2))
            in = 2 * n;
        else
            in = 2 * n - 1;

        for (unsigned int i = ix + 2; i < in; i += 2) {
            double y = yy[i];
            double y2 = y * y;
            double f1 = (shift + y)
                    * ((gpd[i] - gpd_n) / ((xi2 - y2) * (xi2 - y2))
                            * (y * xi2px2 + xxi2mx3 - tx * y2)
                            + (gpd[i] - gpd_x) / (y - x));
            convint += 2.0 * f1;
        }

        for (unsigned int i = ix + 1; i < in; i += 2) {
            double y = yy[i];
            double y2 = y * y;
            double f1 = (shift + y)
                    * ((gpd[i] - gpd_n) / ((xi2 - y2) * (xi2 - y2))
                            * (y * xi2px2 + xxi2mx3 - tx * y2)
                            + (gpd[i] - gpd_x) / (y - x));
            convint += 4.0 * f1;
        }

        {
            double y = yy[in];
            double y2 = y * y;
            double f1 = (shift + y)
                    * ((gpd[in] - gpd_n) / ((xi2 - y2) * (xi2 - y2))
                            * (y * xi2px2 + xxi2mx3 - tx * y2)
                            + (gpd[in] - gpd_x) / (y - x));
            convint += f1;

            if ((2 * n - 1) == in) {
                double ya = yy[2 * n - 2];
                double ya2 = ya * ya;
                double f1a = (shift + ya)
                        * ((gpd[2 * n - 2] - gpd_n)
                                / ((xi2 - ya2) * (xi2 - ya2))
                                * (ya * xi2px2 + xxi2mx3 - tx * ya2)
                                + (gpd[2 * n - 2] - gpd_x) / (ya - x));
                double f1b = -(shift + 1.0)
                        * (gpd_n / ((xi2 - 1.0) * (xi2 - 1.0))
                                * (xi2px2 + xxi2mx3 - tx) + gpd_x / (1.0 - x));
                convint += 2.0 * f1 - 0.25 * f1a + 1.25 * f1b;
            }
        }

        {
            // y = x
            convint += (shift + x) * (gpd_x - gpd_n) / ((xi2 - x2) * (xi2 - x2))
                    * (x * xi2px2 + xxi2mx3 - tx * x2)
                    + (-11.0 * gpd[ix] + 18.0 * gpd[ix + 1] - 9.0 * gpd[ix + 2]
                            + 2.0 * gpd[ix + 3]) / (6.0 * gamma);
        }

        convint *= gamma * c_1_3;
        convint +=
                gpd[ix]
                        * (2.0 * (x2 + xi2) * (1.0 - x) / (xi * xi2 - xi)
                                + (2.0 * xi - x) * (xi + x) * (xi + x) / xi2
                                        * log((1.0 + xi) / (x + xi))
                                - 2.0 * xi
                                        * log(
                                                (1.0 - xi2)
                                                        / ((1.0 - x) * (1.0 - x)))
                                + (2.0 * xi + x) * (xi - x) * (xi - x) / xi2
                                        * log((1.0 - xi) / (x - xi))
                                + 14.0 * c_1_3 * xi) * 0.25 / xi;
        convint += (gpd[n] - gpd[ix])
                * log((xi - x) * (xi + 1.0) / ((xi + x) * (xi - 1.0)))
                * (3.0 * xi2 * x - x2 * x) / (4.0 * xi2 * xi);
        convint += -(gpd[n] - gpd[ix]) * (x2 + xi2) * (1.0 - x)
                / ((xi + x) * (xi + 1.0) * 2.0 * xi);
        convint += -(gpd[n] - gpd[ix]) * (x2 + xi2)
                * ((x - xi) / (xi2 - 1.0) + x / (xi + x)) / (2.0 * xi2);
        convint *= 2.0 * m_CA;
        convint += (-0.5 * m_CA - 2.0 * c_1_3 * m_TR) * gpd[ix];

        return convint;
    }

    printf(
            "The convolution code finished weird.\n Looks like ix is not in the right range [0,2n]\n");
    return 0.;
}

double VinnikovEvolQCDModel::convolSingletgqKernel(const unsigned int ix,
        const double xi, const int n, const double* const gpd,
        const double* const yy) {

    // Dimension of "gpd" and "yy" arrays must be 2n+1, i.e. "double gpd[2n+1]"
    // In the code it is assumed that gpd(x=1) = 0.0 !!

    // This function computes the convolution integral
    // using logarithmic grid, i.e. something like this:

    // \int f(x) dx = \int f(exp(gamma*i)-shift)*gamma*exp(gamma*i) di

    // So that the (shifted) logarithmic scale in "x" is transformed into
    // the linear scale in "i". Thus, the standard Simpson method
    // can be used for integrand which has steep rise around zero.

    // More precise, the interval in "x" [0, 1] is presented as:

    // x = shift*exp(gamma*(i-2n)) - shift, i = 0 ... 2n

    // The coefficients "gamma" and "shift" are chosen so that the point
    // x = +-xi divided the pieces in two equal (in log scale) parts:

    // gamma = log(1/2 + 1/2/xi*sqrt(9*xi^2 - 12*xi+4))/n
    // shift = xi^2/(1-2*xi)

    // which yields:

    // x[0] = 0, x[n] = xi, x[2n] = 1

    //----------------   Ok, lets start  -------------------

    if (n < 5) {
        printf(
                "It's not a good idea to choose n < 5.\n I am too serious code to even start calculation at such conditions.\n So, all you get now is return 0.0;\n");
        return 0.;
    }

    double const x = yy[ix];
    double const x2 = x * x;
    double const xi2 = xi * xi;
    double const c_1_3 = 1.0 / 3.0;
    double const shift = xi2 / (1.0 - 2.0 * xi);
    double const gamma = log(0.5 + 0.5 / xi * sqrt(9.0 * xi2 - 12.0 * xi + 4.0))
            / ((double) n);

    // First, consider separately some close to boundary cases

    if (2 * n == ix)
        return 0.;

    if ((2 * n - 1) == ix) {
        double y = yy[2 * n - 2];
        double y2 = y * y;
        double f1 = gpd[2 * n - 2] * (xi2 - x2 - 2.0 * y2 + 2.0 * x * y)
                / (xi2 - y2);
        return m_CF * c_1_3 * gamma
                * (-0.25 * f1 * (shift + y) + 2.0 * gpd[ix] * (shift + x));
    }

    if ((2 * n - 2) == ix) {
        double y = yy[2 * n - 1];
        double y2 = y * y;
        double f1 = gpd[2 * n - 1] * (xi2 - x2 - 2.0 * y2 + 2.0 * x * y)
                / (xi2 - y2);
        double f2 = gpd[2 * n - 2];
        return m_CF * c_1_3 * gamma
                * (4.0 * f1 * (shift + y) + f2 * (shift + x));
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution at 0 < x < xi (0 <= i < n).
    // The integral is given by a sum of two parts:  0 < y < x
    // and x < y < 1 .

    if (ix < n) {
        double convint = 0.;
        double xi2px2_xi = (xi2 + x2) / xi;
        double gpd_n = gpd[n];
        unsigned int in;

        // First integrate at 0 < y < x

        if (1 == ix) {
            double y = yy[2];
            double f1 = (shift + x) * (gpd[1] - gpd_n) * (xi - x) * x
                    / (xi * (xi + x));
            double f2 = (shift + y) * (gpd[2] - gpd_n) * (xi - x) * (xi - x) * y
                    / (xi * (xi2 - y * y));
            convint += 2.0 * f1 - 0.25 * f2;
        }

        if (2 == ix) {
            double y = yy[1];
            double f1 = (shift + y) * (gpd[1] - gpd_n) * (xi - x) * (xi - x) * y
                    / (xi * (xi2 - y * y));
            double f2 = (shift + x) * (gpd[2] - gpd_n) * (xi - x) * x
                    / (xi * (xi + x));
            convint += 4.0 * f1 + f2;
        }

        if (ix > 2) {
            if (0 == (ix % 2))
                in = ix;
            else
                in = ix - 1;

            for (unsigned int i = 2; i < in; i += 2) {
                double y = yy[i];
                double f1 = (shift + y) * (gpd[i] - gpd_n) * (xi - x) * (xi - x)
                        * y / (xi * (xi2 - y * y));
                convint += 2.0 * f1;
            }

            for (unsigned int i = 1; i < in; i += 2) {
                double y = yy[i];
                double f1 = (shift + y) * (gpd[i] - gpd_n) * (xi - x) * (xi - x)
                        * y / (xi * (xi2 - y * y));
                convint += 4.0 * f1;
            }

            {
                double y = yy[in];
                double f1 = (shift + y) * (gpd[in] - gpd_n) * (xi - x)
                        * (xi - x) * y / (xi * (xi2 - y * y));
                convint += f1;

                if (in == (ix - 1)) {
                    double ya = yy[ix - 2];
                    double f1a = (shift + ya) * (gpd[ix - 2] - gpd_n) * (xi - x)
                            * (xi - x) * ya / (xi * (xi2 - ya * ya));
                    double f2 = (shift + x) * (gpd[ix] - gpd_n) * (xi - x) * x
                            / (xi * (xi + x));
                    convint += 2.0 * f1 - 0.25 * f1a + 1.25 * f2;
                }
            }
        }

        // Now integrate at x < y < 1
        if (0 == (ix % 2))
            in = 2 * n;
        else
            in = 2 * n - 1;

        for (unsigned int i = ix + 2; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * gpd[i] * (2.0 - xi2px2_xi / (xi + y));
            convint += 2.0 * f1;
        }

        for (unsigned int i = ix + 1; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * gpd[i] * (2.0 - xi2px2_xi / (xi + y));
            convint += 4.0 * f1;
        }

        {
            double y = yy[in];
            double f1 = (shift + y) * gpd[in] * (2.0 - xi2px2_xi / (xi + y));
            convint += f1;

            if ((2 * n - 1) == in) {
                double ya = yy[2 * n - 2];
                double f1a = (shift + ya) * gpd[2 * n - 2]
                        * (2.0 - xi2px2_xi / (xi + ya));
                convint += 2.0 * f1 - 0.25 * f1a;
            }
        }

        {
            // y = x
            convint += (shift + x) * gpd[ix] * (2.0 - xi2px2_xi / (xi + x));
        }

        convint *= gamma * c_1_3;

        // convint += gpd_n*(-(xi-x)*(xi-x)*x/xi + (xi-x)*(xi-x)*log(xi/(xi-x)));

        convint += gpd_n
                * ((xi - x) * (xi - x) / xi * log(xi / (xi + x))
                        + (xi - x) * (xi - x) * 0.5 / xi
                                * log((xi + x) / (xi - x)));

        return m_CF * convint;
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution at x = xi   (i = n).
    if (n == ix) {
        double convint = 0.0;
        unsigned int in;

        if (0 == (ix % 2))
            in = 2 * n;
        else
            in = 2 * n - 1;

        for (unsigned int i = ix + 2; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * gpd[i] * y / (xi + y);
            convint += 2.0 * f1;
        }

        for (unsigned int i = ix + 1; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * gpd[i] * y / (xi + y);
            convint += 4.0 * f1;
        }

        {
            double y = yy[in];
            double f1 = (shift + y) * gpd[in] * y / (xi + y);
            convint += f1;

            if ((2 * n - 1) == in) {
                double ya = yy[2 * n - 2];
                double f1a = (shift + ya) * gpd[2 * n - 2] * ya / (xi + ya);
                convint += 2.0 * f1 - 0.25 * f1a;
            }
        }

        {
            // y = x
            convint += (shift + x) * gpd[ix] * 0.5;
        }

        convint *= gamma * c_1_3;

        return 2.0 * m_CF * convint;
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution at x > xi   (n < i <= 2*n).

    if (ix < (2 * n - 2)) {
        double convint = 0.0;
        double xi2mx2 = xi2 - x2;
        double const two_x = 2.0 * x;

        unsigned int in;

        if (0 == (ix % 2))
            in = 2 * n;
        else
            in = 2 * n - 1;

        for (unsigned int i = ix + 2; i < in; i += 2) {
            double y = yy[i];
            double y2 = y * y;
            double f1 = (shift + y) * (xi2mx2 - 2.0 * y2 + two_x * y) * gpd[i]
                    / (xi2 - y2);
            convint += 2.0 * f1;
        }

        for (unsigned int i = ix + 1; i < in; i += 2) {
            double y = yy[i];
            double y2 = y * y;
            double f1 = (shift + y) * (xi2mx2 - 2.0 * y2 + two_x * y) * gpd[i]
                    / (xi2 - y2);
            convint += 4.0 * f1;
        }

        {
            double y = yy[in];
            double y2 = y * y;
            double f1 = (shift + y) * (xi2mx2 - 2.0 * y2 + two_x * y) * gpd[in]
                    / (xi2 - y2);
            convint += f1;

            if ((2 * n - 1) == in) {
                double ya = yy[2 * n - 2];
                double ya2 = ya * ya;
                double f1a = (shift + ya) * (xi2mx2 - 2.0 * ya2 + two_x * ya)
                        * gpd[2 * n - 2] / (xi2 - ya2);
                convint += 2.0 * f1 - 0.25 * f1a;
            }
        }

        {
            // y = x
            convint += (shift + x) * gpd[ix];
        }

        convint *= gamma * c_1_3;

        return m_CF * convint;
    }

    printf(
            "The convolution code finished weird.\n Looks like ix is not in the right range (-1,1)\n");
    return 0.;
}

double VinnikovEvolQCDModel::convolSingletqgKernel(const unsigned int ix,
        const double xi, const int n, const double* const gpd,
        const double* const yy) {

    // Dimension of "gpd" and "yy" arrays must be 2n+1, i.e. "double gpd[2n+1]"
    // In the code it is assumed that gpd(x=1) = 0.0 !!

    // This function computes the convolution integral
    // using logarithmic grid, i.e. something like this:

    // \int f(x) dx = \int f(exp(gamma*i)-shift)*gamma*exp(gamma*i) di

    // So that the (shifted) logarithmic scale in "x" is transformed into
    // the linear scale in "i". Thus, the standard Simpson method
    // can be used for integrand which has steep rise around zero.

    // More precise, the interval in "x" [0, 1] is presented as:

    // x = shift*exp(gamma*(i-2n)) - shift, i = 0 ... 2n

    // The coefficients "gamma" and "shift" are chosen so that the point
    // x = +-xi divided the pieces in two equal (in log scale) parts:

    // gamma = log(1/2 + 1/2/xi*sqrt(9*xi^2 - 12*xi+4))/n
    // shift = xi^2/(1-2*xi)

    // which yields:
    // x[0] = 0, x[n] = xi, x[2n] = 1

    //----------------   Ok, lets start  -------------------

    if (n < 5) {
        printf(
                "It's not a good idea to choose n < 5.\n I am too serious code to even start calculation at such conditions.\n So, all you get now is return 0.0;\n");
        return 0.;
    }

    double const x = yy[ix];
    double const x2 = x * x;
    double const xi2 = xi * xi;
    double const xi2m2x2 = xi2 - 2.0 * x2;
    double const two_x = 2.0 * x;
    double const c_1_3 = 1.0 / 3.0;
    double const shift = xi2 / (1.0 - 2.0 * xi);
    double const gamma = log(0.5 + 0.5 / xi * sqrt(9.0 * xi2 - 12.0 * xi + 4.0))
            / ((double) n);
    //  double const deriv_minus_xi = (-2.0*gpd[n-3] + 9.0*gpd[n-2]
    //				 - 18.0*gpd[n-1] + 11.0*gpd[n])/
    //    (6.0*gamma*(-xi+shift));

    // First, consider separately some close to boundary cases
    if ((2 * n) == ix)
        return 0.;

    if ((2 * n - 1) == ix) {
        double y = yy[2 * n - 2];
        double y2 = y * y;
        double f1 = gpd[2 * n - 2] * (xi2m2x2 - y2 + two_x * y)
                / ((xi2 - y2) * (xi2 - y2));
        double f2 = gpd[ix] / (xi2 - x2);
        return -2.0 * m_TR * c_1_3 * gamma
                * (2.0 * f2 * (shift + x) - 0.25 * f1 * (shift + y));
    }

    if ((2 * n - 2) == ix) {
        double y = yy[2 * n - 1];
        double y2 = y * y;
        double f1 = gpd[2 * n - 1] * (xi2m2x2 - y2 + two_x * y)
                / ((xi2 - y2) * (xi2 - y2));
        double f2 = gpd[2 * n - 2] / (xi2 - x2);
        return -2.0 * m_TR * c_1_3 * gamma
                * (4.0 * f1 * (shift + y) + f2 * (shift + x));
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution at 0 < x < xi (0 < i < n).
    // The integral is given by a sum of two parts:  0 < y < x
    // and x < y < 1 .
    if (ix < n) {
        double convint = 0.0;
        double xi432 = xi2 * xi2 + xi2 * xi * x - 2.0 * xi2 * x2;
        double xi2mxix = xi2 - xi * x;
        double gpd_n = gpd[n];
        unsigned int in;

        // First integrate at 0 < y < x
        if (1 == ix) {
            double y = yy[2];
            double y2 = y * y;
            double f0 = shift * (gpd[0] - gpd_n) * xi432 / (xi2 * xi2);
            double f1 = (shift + x) * (gpd[1] - gpd_n) * (xi432 - xi2mxix * x2)
                    / ((xi2 - x2) * (xi2 - x2));
            double f2 = (shift + y) * (gpd[2] - gpd_n) * (xi432 - xi2mxix * y2)
                    / ((xi2 - y2) * (xi2 - y2));
            convint += 1.25 * f0 + 2.0 * f1 - 0.25 * f2;
        }

        if (2 == ix) {
            double y = yy[1];
            double y2 = y * y;
            double f0 = shift * (gpd[0] - gpd_n) * xi432 / (xi2 * xi2);
            double f1 = (shift + y) * (gpd[1] - gpd_n) * (xi432 - xi2mxix * y2)
                    / ((xi2 - y2) * (xi2 - y2));
            double f2 = (shift + x) * (gpd[2] - gpd_n) * (xi432 - xi2mxix * x2)
                    / ((xi2 - x2) * (xi2 - x2));
            convint += f0 + 4.0 * f1 + f2;
        }

        if (ix > 2) {
            if (0 == (ix % 2))
                in = ix;
            else
                in = ix - 1;

            for (unsigned int i = 2; i < in; i += 2) {
                double y = yy[i];
                double y2 = y * y;
                double f1 = (shift + y) * (gpd[i] - gpd_n)
                        * (xi432 - xi2mxix * y2) / ((xi2 - y2) * (xi2 - y2));
                convint += 2.0 * f1;
            }

            for (unsigned int i = 1; i < in; i += 2) {
                double y = yy[i];
                double y2 = y * y;
                double f1 = (shift + y) * (gpd[i] - gpd_n)
                        * (xi432 - xi2mxix * y2) / ((xi2 - y2) * (xi2 - y2));
                convint += 4.0 * f1;
            }

            {
                double y = yy[in];
                double y2 = y * y;
                double f1 = (shift + y) * (gpd[in] - gpd_n)
                        * (xi432 - xi2mxix * y2) / ((xi2 - y2) * (xi2 - y2));
                convint += f1;

                if ((ix - 1) == in) {
                    double ya = yy[ix - 2];
                    double ya2 = ya * ya;
                    double f1a = (shift + ya) * (gpd[ix - 2] - gpd_n)
                            * (xi432 - xi2mxix * ya2)
                            / ((xi2 - ya2) * (xi2 - ya2));
                    double f1b = (shift + x) * (gpd[ix] - gpd_n)
                            * (xi432 - xi2mxix * x2)
                            / ((xi2 - x2) * (xi2 - x2));
                    convint += 2.0 * f1 - 0.25 * f1a + 1.25 * f1b;
                }
            }

            {
                // y = 0
                convint += shift * (gpd[0] - gpd_n) * xi432 / (xi2 * xi2);
            }
        }

        // Now integrate at x < y < 1
        if (0 == (ix % 2))
            in = 2 * n;
        else
            in = 2 * n - 1;

        for (unsigned int i = ix + 2; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * gpd[i] * xi * x / ((xi + y) * (xi + y));
            convint += 2.0 * f1;
        }

        for (unsigned int i = ix + 1; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * gpd[i] * xi * x / ((xi + y) * (xi + y));
            convint += 4.0 * f1;
        }

        {
            double y = yy[in];
            double f1 = (shift + y) * gpd[in] * xi * x / ((xi + y) * (xi + y));
            convint += f1;

            if ((2 * n - 1) == in) {
                double ya = yy[2 * n - 2];
                double f1a = (shift + ya) * gpd[2 * n - 2] * xi * x
                        / ((xi + ya) * (xi + ya));
                convint += 2.0 * f1 - 0.25 * f1a;
            }
        }

        {
            // y = x
            convint += (shift + x) * gpd[ix] * xi * x / ((xi + x) * (xi + x));
        }

        convint *= gamma * c_1_3;
        convint += gpd_n
                * (x2 / (xi + x)
                        + (xi2 - x2) / 2.0 / xi * log((xi + x) / (xi - x)));
        convint *= 1.0 / xi2;

        return 2.0 * m_TR * convint;
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution at x = xi   (i = 3*n). If the number of intervals
    // is even, the usual Simpson's formula is used. If it is odd, the last
    // interval is integrated with trapecia.
    if (n == ix) {
        double convint = 0.0;
        double gpd_n = gpd[n];
        unsigned int in;

        if (0 == (ix % 2))
            in = 2 * n;
        else
            in = 2 * n - 1;

        for (unsigned int i = ix + 2; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * (gpd[i] - gpd_n) / ((xi + y) * (xi + y));
            convint += 2.0 * f1;
        }

        for (unsigned int i = ix + 1; i < in; i += 2) {
            double y = yy[i];
            double f1 = (shift + y) * (gpd[i] - gpd_n) / ((xi + y) * (xi + y));
            convint += 4.0 * f1;
        }

        {
            double y = yy[in];
            double f1 = (shift + y) * (gpd[in] - gpd_n) / ((xi + y) * (xi + y));
            convint += f1;

            // TODO: Understand who are "i" and "xi" here.
            if ((2 * n - 1) == in) {
                double ya = yy[2 * n - 2];

                // TODO: Check and understand !!! Possible bug in Vinnikov's code...
                //double f1a = (shift+ya)*(gpd[i] - gpd_n)/((xi+ya)*(xi+ya));
                double f1a = (shift + ya) * (gpd[in + 1] - gpd_n)
                        / ((xi + ya) * (xi + ya)); // Test HM, 4/11/2014
                double f1b = -(shift + 1.0) * gpd_n / ((xi + 1.0) * (xi + 1.0));
                convint += 2.0 * f1 - 0.25 * f1a + 1.25 * f1b;
            }
        }

        {
            // y = x
            convint += (shift + x) * (gpd[ix] - gpd_n) / ((xi + x) * (xi + x));
        }

        convint *= gamma * c_1_3;
        convint += -0.5 * gpd[n] * (xi - 1.0) / (xi * (xi + 1.0));
        convint += 0.5 * gpd[n] / xi;

        return 2.0 * m_TR * convint;
    }

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Convolution at x > xi   (4*n-n1 <= i <= 4*n). If the number of intervals
    // is even, the usual Simpson's formula is used. If it is odd, the last
    // interval is integrated with trapecia.
    if (ix < (2 * n - 2)) {
        double convint = 0.;
        double const gpd_3n = gpd[n];
        unsigned int in;

        if (0 == (ix % 2))
            in = 2 * n;
        else
            in = 2 * n - 1;

        for (unsigned int i = ix + 2; i < in; i += 2) {
            double y = yy[i];
            double y2 = y * y;
            double f1 = (shift + y) * (gpd_3n - gpd[i])
                    * (xi2m2x2 - y2 + two_x * y) / ((xi2 - y2) * (xi2 - y2));
            convint += 2.0 * f1;
        }

        for (unsigned int i = ix + 1; i < in; i += 2) {
            double y = yy[i];
            double y2 = y * y;
            double f1 = (shift + y) * (gpd_3n - gpd[i])
                    * (xi2m2x2 - y2 + two_x * y) / ((xi2 - y2) * (xi2 - y2));
            convint += 4.0 * f1;
        }

        {
            double y = yy[in];
            double y2 = y * y;
            double f1 = (shift + y) * (gpd_3n - gpd[in])
                    * (xi2m2x2 - y2 + two_x * y) / ((xi2 - y2) * (xi2 - y2));
            convint += f1;

            if ((2 * n - 1) == in) {
                double ya = yy[2 * n - 2];
                double ya2 = ya * ya;
                double f1a = (shift + ya) * (gpd_3n - gpd[2 * n - 2])
                        * (xi2m2x2 - ya2 + two_x * ya)
                        / ((xi2 - ya2) * (xi2 - ya2));
                double f1b = (shift + 1.0) * gpd_3n * (xi2m2x2 - 1.0 + two_x)
                        / ((xi2 - 1.0) * (xi2 - 1.0));
                convint += 2.0 * f1 - 0.25 * f1a + 1.25 * f1b;
            }
        }

        {
            // y = x
            convint += (shift + x) * (gpd_3n - gpd[ix]) * (xi2m2x2 + x2)
                    / ((xi2 - x2) * (xi2 - x2));

        }

        convint *= gamma * c_1_3;
        convint += -gpd_3n
                * (0.5 * (xi2 - x2) / (xi2 * xi)
                        * log((xi + 1.0) * (xi - x) / ((xi + x) * (xi - 1.0)))
                        + (x - x2) / (xi2 * (xi2 - 1.0)));

        return 2.0 * m_TR * convint;
    }

    printf(
            "The convolution code finished weird.\n Looks like ix is not in the right range (-1,1)\n");
    return 0.;
}

double VinnikovEvolQCDModel::interpolate(std::vector<double> xGrid,
        std::vector<double> yGrid, double x) {

    /*	// TODO : Check that the syntax is indeed correct...
     if (xGrid.size() != yGrid.size() ) throw  std::length_error(
     Formatter()
     << "[VinnikovEvolQCDModel] In interpolate function :"
     << "x- and y- grids should have the same size;"
     << "Here x has size " << xGrid.size() << " and y has size"
     << yGrid.size());

     if (*xGrid.begin() < x || *xGrid.end() > x) throw std::range_error(
     Formatter()
     << "[VinnikovEvolQCDModel] In interpolate function :"
     << "Target x does not lie in x-grid: extrapolation !"
     << "Here target x = " << x);*/

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "x = " << x);

    size_t nDataPointsInGrid = xGrid.size();
    double xArray[nDataPointsInGrid];
    double yArray[nDataPointsInGrid];

    for (unsigned int i = 0; i < nDataPointsInGrid; i++) {
        xArray[i] = xGrid.at(i);
        yArray[i] = yGrid.at(i);

        m_pLoggerManager->debug(getClassName(), __func__,
                Formatter() << "i = " << i << "	 xArray = " << xArray[i]
                        << "	yArray = " << yArray[i]);
    }

    // You can choose among the following methods:
    // CSPLINE, LINEAR, POLYNOMIAL,
    // CSPLINE_PERIODIC, AKIMA, AKIMA_PERIODIC
    ROOT::Math::Interpolator RootInterpolator(nDataPointsInGrid,
            ROOT::Math::Interpolation::kLINEAR);
    RootInterpolator.SetData(nDataPointsInGrid, xArray, yArray);

    return RootInterpolator.Eval(x);
}

void VinnikovEvolQCDModel::setNbMuFPoints(unsigned int nbMuFPoints) {
    m_nbMuFPoints = nbMuFPoints;
}

void VinnikovEvolQCDModel::setNbXPoints(unsigned int nbXPoints) {
    //TODO: Ensure update before actual computation
    m_nbXPoints = nbXPoints;

    m_NonSingletGpd.resize(m_nbXPoints);
    m_SingletGpd.resize(m_nbXPoints);
    m_GluonGpd.resize(m_nbXPoints);
    m_Singletx.resize(m_nbXPoints);
    m_NonSingletx.resize(m_nbXPoints);
}
