#include "../../../../../include/partons/modules/process/DDVCS/DDVCSProcessDMSW22.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <cmath>

#include "../../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DDVCSProcessDMSW22::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSProcessDMSW22("DDVCSProcessDMSW22"));

DDVCSProcessDMSW22::DDVCSProcessDMSW22(const std::string &className) :
        DDVCSProcessModule(className) {
    m_DMSW_tMin = 0.;
    m_DMSW_Mnucleon = 0.;
    m_DMSW_charge_e = 0.;
    m_DMSW_Q2 = 0.;

    int i;
    for (i = 0; i < 4; i++) {
        m_DMSW_r1[i] = 0.;
        m_DMSW_r2[i] = 0.;
        m_DMSW_rPrime1[i] = 0.;
        m_DMSW_rPrime2[i] = 0.;
        m_DMSW_p1[i] = 0.;
        m_DMSW_p2[i] = 0.;
        m_DMSW_k[i] = 0.;
        m_DMSW_Delta[i] = 0.;
        m_DMSW_q2[i] = 0.;
        m_DMSW_q1[i] = 0.;
        m_DMSW_kPrime[i] = 0.;
        m_DMSW_lminus[i] = 0.;
        m_DMSW_lplus[i] = 0.;
        m_DMSW_nminus[i] = 0.;
        m_DMSW_nplus[i] = 0.;
        m_DMSW_nBM[i] = 0.;
        m_DMSW_nstarBM[i] = 0.;
        m_DMSW_HATnBM[i] = 0.;
    }

    m_DMSW_k0[0] = +1.;
    m_DMSW_k0[1] = +1.;
    m_DMSW_k0[2] = 0.;
    m_DMSW_k0[3] = 0.;

    int a, b, c, d;
    for (a = 0; a < 4; a++) {
        for (b = 0; b < 4; b++) {
            for (c = 0; c < 4; c++) {
                for (d = 0; d < 4; d++) {
                    m_DMSW_LCtensor[a][b][c][d] = 0.;
                }
            }
        }
    }

    m_DMSW_LCtensor[0][1][2][3] = +1.;
    m_DMSW_LCtensor[0][3][1][2] = +1.;
    m_DMSW_LCtensor[0][2][3][1] = +1.;
    m_DMSW_LCtensor[0][1][3][2] = -1.;
    m_DMSW_LCtensor[0][3][2][1] = -1.;
    m_DMSW_LCtensor[0][2][1][3] = -1.;

    m_DMSW_LCtensor[3][1][0][2] = +1.;
    m_DMSW_LCtensor[2][1][3][0] = +1.;
    m_DMSW_LCtensor[3][1][2][0] = -1.;
    m_DMSW_LCtensor[2][1][0][3] = -1.;
    m_DMSW_LCtensor[2][3][1][0] = -1.;
    m_DMSW_LCtensor[3][2][1][0] = +1.;

    m_DMSW_LCtensor[3][0][2][1] = +1.;
    m_DMSW_LCtensor[1][3][2][0] = +1.;
    m_DMSW_LCtensor[3][0][1][2] = -1.;
    m_DMSW_LCtensor[1][3][0][2] = -1.;
    m_DMSW_LCtensor[3][2][0][1] = -1.;
    m_DMSW_LCtensor[1][0][3][2] = +1.;

    m_DMSW_LCtensor[2][0][1][3] = +1.;
    m_DMSW_LCtensor[1][2][0][3] = +1.;
    m_DMSW_LCtensor[2][0][3][1] = -1.;
    m_DMSW_LCtensor[1][2][3][0] = -1.;
    m_DMSW_LCtensor[1][0][2][3] = -1.;
    m_DMSW_LCtensor[2][3][0][1] = +1.;

    for (a = 0; a < 4; a++) {
        for (b = 0; b < 4; b++) {
            if (a == b) {
                if (a == 0) {
                    m_DMSW_metric_[a][b] = +1.;
                } else if (a == 1 || a == 2 || a == 3) {
                    m_DMSW_metric_[a][b] = -1.;
                }
            } else {
                m_DMSW_metric_[a][b] = 0.;
            }
        }
    }

    m_DMSW_epsilon2 = 0.;
    m_DMSW_Q2 = 0.;
    m_DMSW_F1 = 0.;
    m_DMSW_F2 = 0.;
//    m_DMSW_eta = 0.;
    m_DMSW_xi = 0.;
    m_DMSW_y = 0.;
    m_DMSW_pq = 0.;

}

DDVCSProcessDMSW22::DDVCSProcessDMSW22(const DDVCSProcessDMSW22& other) :
        DDVCSProcessModule(other) {

//    m_DMSW_r1[4] = other.m_DMSW_r1[4];
//    m_DMSW_r2[4] = other.m_DMSW_r2[4];
//    m_DMSW_rPrime1[4] = other.m_DMSW_rPrime1[4];
//    m_DMSW_rPrime2[4] = other.m_DMSW_rPrime2[4];
//    m_DMSW_p1[4] = other.m_DMSW_p1[4];
//    m_DMSW_p2[4] = other.m_DMSW_p2[4];
//    p2I[4] = other.p2I[4];

    m_DMSW_tMin = other.m_DMSW_tMin;
    m_DMSW_Mnucleon = other.m_DMSW_Mnucleon;
    m_DMSW_charge_e = other.m_DMSW_charge_e;
    m_DMSW_Q2 = other.m_DMSW_Q2;

    int i;
    for (i = 0; i < 4; i++) {
        m_DMSW_r1[i] = other.m_DMSW_r1[i];
        m_DMSW_r2[i] = other.m_DMSW_r2[i];

    }
    m_DMSW_epsilon2 = other.m_DMSW_epsilon2;
    m_DMSW_Q2 = other.m_DMSW_Q2;

    for (i = 0; i < 4; i++) {
        m_DMSW_p2[i] = other.m_DMSW_p2[i];
    }

    for (i = 0; i < 4; i++) {
        m_DMSW_rPrime1[i] = other.m_DMSW_rPrime1[i];
        m_DMSW_rPrime2[i] = other.m_DMSW_rPrime2[i];
        m_DMSW_k[i] = other.m_DMSW_k[i];
        m_DMSW_Delta[i] = other.m_DMSW_Delta[i];
        m_DMSW_q2[i] = other.m_DMSW_q2[i];
        m_DMSW_q1[i] = other.m_DMSW_q1[i];
        m_DMSW_kPrime[i] = other.m_DMSW_kPrime[i];
        m_DMSW_lminus[i] = other.m_DMSW_lminus[i];
        m_DMSW_lplus[i] = other.m_DMSW_lplus[i];
        m_DMSW_nminus[i] = other.m_DMSW_nminus[i];
        m_DMSW_nplus[i] = other.m_DMSW_nplus[i];
        m_DMSW_nBM[i] = other.m_DMSW_nBM[i];
        m_DMSW_nstarBM[i] = other.m_DMSW_nstarBM[i];
        m_DMSW_HATnBM[i] = other.m_DMSW_HATnBM[i];
        m_DMSW_k0[i] = other.m_DMSW_k0[i];
    }

    int a, b, c, d;
    for (a = 0; a < 4; a++) {
        for (b = 0; b < 4; b++) {
            for (c = 0; c < 4; c++) {
                for (d = 0; d < 4; d++) {
                    m_DMSW_LCtensor[a][b][c][d] =
                            other.m_DMSW_LCtensor[a][b][c][d];
                }
            }
        }
    }

    for (a = 0; a < 4; a++) {
        for (b = 0; b < 4; b++) {
            m_DMSW_metric_[a][b] = other.m_DMSW_metric_[a][b];
        }
    }

    m_DMSW_F1 = other.m_DMSW_F1;
    m_DMSW_F2 = other.m_DMSW_F2;
//    m_DMSW_eta = other.m_DMSW_eta;
    m_DMSW_xi = other.m_DMSW_xi;
    m_DMSW_y = other.m_DMSW_y;
    m_DMSW_pq = other.m_DMSW_pq;

    m_cffH = other.m_cffH;
    m_cffE = other.m_cffE;
    m_cffHt = other.m_cffHt;
    m_cffEt = other.m_cffEt;
    m_cffHL = other.m_cffHL;
    m_cffEL = other.m_cffEL;
}

DDVCSProcessDMSW22::~DDVCSProcessDMSW22() {
}

DDVCSProcessDMSW22* DDVCSProcessDMSW22::clone() const {
    return new DDVCSProcessDMSW22(*this);
}

void DDVCSProcessDMSW22::initModule() {

    //init mother class
    DDVCSProcessModule::initModule();

    //compute internal variables
    computeInternalVariables(Constant::PROTON_MASS, m_E, m_t, m_xB, m_Q2,
            m_Q2Prim, m_phi, m_phiL, m_thetaL);

}

void DDVCSProcessDMSW22::isModuleWellConfigured() {

    //check mother class
    DDVCSProcessModule::isModuleWellConfigured();

    //TODO check kinematic limits

}

PhysicalType<double> DDVCSProcessDMSW22::CrossSectionBH() {

    //check target polarization
    int polariz;

    if (m_targetPolarization.getX() == 0. && m_targetPolarization.getY() == 0.
            && m_targetPolarization.getZ() == 0.) {
        polariz = 0;
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Not able to run for polarization "
                        << m_targetPolarization.toString());
    }

    double xsec = crossSectionBH(polariz, m_xB, m_Q2, m_Q2Prim, m_t, m_thetaL); //phi and phiL dependence is hidden in the momenta that are called in ampliBH1() and similar functions inside crossSectionBH(). They are managed by computeInternalVariables()

    return PhysicalType<double>(xsec, PhysicalUnit::GEVm2);
}

PhysicalType<double> DDVCSProcessDMSW22::CrossSectionVCS() {

    //check target polarization
    int polariz;

    if (m_targetPolarization.getX() == 0. && m_targetPolarization.getY() == 0.
            && m_targetPolarization.getZ() == 0.) {
        polariz = 0;
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Not able to run for polarization "
                        << m_targetPolarization.toString());
    }

    double xsec = crossSectionVCS(polariz, m_xB, m_Q2, m_Q2Prim, m_t, m_thetaL);

    return PhysicalType<double>(xsec, PhysicalUnit::GEVm2);
}

PhysicalType<double> DDVCSProcessDMSW22::CrossSectionInterf() {

    //check target polarization
    int polariz;

    if (m_targetPolarization.getX() == 0. && m_targetPolarization.getY() == 0.
            && m_targetPolarization.getZ() == 0.) {
        polariz = 0;
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Not able to run for polarization "
                        << m_targetPolarization.toString());
    }

    double xsec = crossSectionInterf(polariz, m_xB, m_Q2, m_Q2Prim, m_t,
            m_thetaL);

    return PhysicalType<double>(xsec, PhysicalUnit::GEVm2);

}

//***************** Amplitudes for BH1, BH2 (with their 'crossed' counterparts) and VCS *****************//

std::complex<double> DDVCSProcessDMSW22::ampliBH1(int s2, int s1, int sl,
        double lminus[4], double lplus[4], int s, double kPrime[4], double k[4],
        double Mll2, double t) {

    std::complex<double> tBH1_J1 = (0., 0.);
    std::complex<double> tBH1_J2 = (0., 0.);
    std::complex<double> tBH1 = (0., 0.);
    double L[3][4]; // array that will contain lminus, lplus, kPrime
    double rPrime[2][4];
    double r[2][4];
    int h1, h2, minush1, minush2;

    //Term in J1:

    int i, j;
    for (i = 0; i < 4; i++) {
        L[0][i] = lminus[i];
        L[1][i] = lplus[i];
        L[2][i] = kPrime[i];
    }

    for (i = 0; i < 4; i++) {
        rPrime[0][i] = m_DMSW_rPrime2[i];
        rPrime[1][i] = m_DMSW_rPrime1[i];
        r[0][i] = m_DMSW_r2[i];
        r[1][i] = m_DMSW_r1[i];
    }

    //Transform physical values of helicity s1, s2 = -1 and +1, to integers 0 and 1; respectively
    if (s2 == +1) {
        h2 = +1;
        minush2 = 0;
    } else if (s2 == -1) {
        h2 = 0;
        minush2 = 1;
    }
    if (s1 == +1) {
        h1 = +1;
        minush1 = 0;
    } else if (s1 == -1) {
        h1 = 0;
        minush1 = 1;
    }

    for (j = 0; j < 3; j++) {
        tBH1_J1 += fFunction(sl, lminus, lplus, s, kPrime, L[j])
                * (Yfunction(s2, s1)
                        * fFunction(s, L[j], k, +1, rPrime[h2], r[h1])
                        + Zfunction(s2, s1)
                                * fFunction(s, L[j], k, -1, rPrime[minush2],
                                        r[minush1]));
    }

    tBH1_J1 = (m_DMSW_F1 + m_DMSW_F2) * tBH1_J1;

    //Adding term in J2:
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 2; i++) {

            tBH1_J2 += fFunction(sl, lminus, lplus, s, kPrime, L[j])
                    * gFunction(s, L[j], rPrime[i], k);
            tBH1_J2 += fFunction(sl, lminus, lplus, s, kPrime, L[j])
                    * gFunction(s, L[j], r[i], k);

        }
    }

    tBH1_J2 = -m_DMSW_F2 * J2function(s2, s1) * tBH1_J2
            / (2. * m_DMSW_Mnucleon);

    double kMinusDelta[4];
    for (i = 0; i < 4; i++) {
        kMinusDelta[i] = m_DMSW_k[i] - m_DMSW_Delta[i];
    }

    tBH1 = pow(m_DMSW_charge_e, 4.) * (tBH1_J1 + tBH1_J2)
            / (Mll2 * t * MinkProd(kMinusDelta, kMinusDelta));

    return tBH1;

}

std::complex<double> DDVCSProcessDMSW22::ampliBH1crossed(int s2, int s1, int sl,
        double lminus[4], double lplus[4], int s, double kPrime[4], double k[4],
        double Mll2, double t) {

    std::complex<double> tBH1_J1 = (0., 0.);
    std::complex<double> tBH1_J2 = (0., 0.);
    std::complex<double> tBH1 = (0., 0.);
    double L[3][4]; // array that will contain lminus, lplus, k
    double rPrime[2][4];
    double r[2][4];
    int h1, h2, minush1, minush2;

    //Term in J1:

    int i, j;
    for (i = 0; i < 4; i++) {
        L[0][i] = lminus[i];
        L[1][i] = lplus[i];
        L[2][i] = k[i];
    }

    for (i = 0; i < 4; i++) {
        rPrime[0][i] = m_DMSW_rPrime2[i];
        rPrime[1][i] = m_DMSW_rPrime1[i];
        r[0][i] = m_DMSW_r2[i];
        r[1][i] = m_DMSW_r1[i];
    }

    //Define 'sigma' to include appropriate signs in the sum over L
    double sigma[3] = { -1., -1., +1. };

    //Transform physical values of helicity s1, s2 = -1 andr +1, to integers 0 and 1; respectively
    if (s2 == +1) {
        h2 = +1;
        minush2 = 0;
    } else if (s2 == -1) {
        h2 = 0;
        minush2 = 1;
    }
    if (s1 == +1) {
        h1 = +1;
        minush1 = 0;
    } else if (s1 == -1) {
        h1 = 0;
        minush1 = 1;
    }

    for (j = 0; j < 3; j++) {
        tBH1_J1 += sigma[j] * fFunction(sl, lminus, lplus, s, L[j], k)
                * (Yfunction(s2, s1)
                        * fFunction(s, kPrime, L[j], +1, rPrime[h2], r[h1])
                        + Zfunction(s2, s1)
                                * fFunction(s, kPrime, L[j], -1,
                                        rPrime[minush2], r[minush1]));
    }

    tBH1_J1 = (m_DMSW_F1 + m_DMSW_F2) * tBH1_J1;

    //Adding term in J2:
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 2; i++) {

            tBH1_J2 += sigma[j] * fFunction(sl, lminus, lplus, s, L[j], k)
                    * gFunction(s, kPrime, rPrime[i], L[j]);
            tBH1_J2 += sigma[j] * fFunction(sl, lminus, lplus, s, L[j], k)
                    * gFunction(s, kPrime, r[i], L[j]);

        }
    }

    tBH1_J2 = -m_DMSW_F2 * J2function(s2, s1) * tBH1_J2
            / (2. * m_DMSW_Mnucleon);

    double kPrimePlusDelta[4]; // kPrimePlusDelta = k' + Delta = k - q2
    for (i = 0; i < 4; i++) {
        kPrimePlusDelta[i] = m_DMSW_k[i] - m_DMSW_q2[i];
    }

    tBH1 = pow(m_DMSW_charge_e, 4.) * (tBH1_J1 + tBH1_J2)
            / (Mll2 * t * MinkProd(kPrimePlusDelta, kPrimePlusDelta));

    return tBH1;

}

std::complex<double> DDVCSProcessDMSW22::ampliBH2(int s2, int s1, int sl,
        double lminus[4], double lplus[4], int s, double kPrime[4], double k[4],
        double Qcal2, double t) {

    std::complex<double> tBH2_J1 = (0., 0.);
    std::complex<double> tBH2_J2 = (0., 0.);
    std::complex<double> tBH2 = (0., 0.);
    double L[3][4]; // array that will contain kPrime, lminus, k
    double rPrime[2][4];
    double r[2][4];
    int h1, h2, minush1, minush2;

    //Term in J1:

    int i, j;
    for (i = 0; i < 4; i++) {
        L[0][i] = kPrime[i];
        L[1][i] = lminus[i];
        L[2][i] = k[i];
    }

    for (i = 0; i < 4; i++) {
        rPrime[0][i] = m_DMSW_rPrime2[i];
        rPrime[1][i] = m_DMSW_rPrime1[i];
        r[0][i] = m_DMSW_r2[i];
        r[1][i] = m_DMSW_r1[i];
    }

    //Define 'sigma' to include appropriate signs in the sum over L
    double sigma[3] = { -1., -1., +1. };

    //Transform physical values of helicity s1, s2 = -1 and +1, to integers 0 and 1; respectively
    if (s2 == +1) {
        h2 = +1;
        minush2 = 0;
    } else if (s2 == -1) {
        h2 = 0;
        minush2 = 1;
    }
    if (s1 == +1) {
        h1 = +1;
        minush1 = 0;
    } else if (s1 == -1) {
        h1 = 0;
        minush1 = 1;
    }

    for (j = 0; j < 3; j++) {
        tBH2_J1 += sigma[j] * fFunction(sl, lminus, L[j], s, kPrime, k)
                * (Yfunction(s2, s1)
                        * fFunction(sl, L[j], lplus, +1, rPrime[h2], r[h1])
                        + Zfunction(s2, s1)
                                * fFunction(sl, L[j], lplus, -1,
                                        rPrime[minush2], r[minush1]));
    }

    tBH2_J1 = (m_DMSW_F1 + m_DMSW_F2) * tBH2_J1;

    //Adding term in J2:
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 2; i++) {

            tBH2_J2 += sigma[j] * fFunction(sl, lminus, L[j], s, kPrime, k)
                    * gFunction(sl, L[j], rPrime[i], lplus);
            tBH2_J2 += sigma[j] * fFunction(sl, lminus, L[j], s, kPrime, k)
                    * gFunction(sl, L[j], r[i], lplus);

        }
    }

    tBH2_J2 = -m_DMSW_F2 * J2function(s2, s1) * tBH2_J2
            / (2. * m_DMSW_Mnucleon);

    double q1Minuslminus[4];
    for (i = 0; i < 4; i++) {
        q1Minuslminus[i] = m_DMSW_q1[i] - m_DMSW_lminus[i];
    }

    tBH2 = -pow(m_DMSW_charge_e, 4.) * (tBH2_J1 + tBH2_J2)
            / (-Qcal2 * t * MinkProd(q1Minuslminus, q1Minuslminus));

    return tBH2;

}

std::complex<double> DDVCSProcessDMSW22::ampliBH2crossed(int s2, int s1, int sl,
        double lminus[4], double lplus[4], int s, double kPrime[4], double k[4],
        double Qcal2, double t) {

    std::complex<double> tBH2_J1 = (0., 0.);
    std::complex<double> tBH2_J2 = (0., 0.);
    std::complex<double> tBH2 = (0., 0.);
    double L[3][4]; // array that will contain kPrime, lplus, k
    double rPrime[2][4];
    double r[2][4];
    int h1, h2, minush1, minush2;

    //Term in J1:

    int i, j;
    for (i = 0; i < 4; i++) {
        L[0][i] = kPrime[i];
        L[1][i] = lplus[i];
        L[2][i] = k[i];
    }

    for (i = 0; i < 4; i++) {
        rPrime[0][i] = m_DMSW_rPrime2[i];
        rPrime[1][i] = m_DMSW_rPrime1[i];
        r[0][i] = m_DMSW_r2[i];
        r[1][i] = m_DMSW_r1[i];
    }

    //Define 'sigma' to include appropriate signs in the sum over L
    double sigma[3] = { -1., -1., +1. };

    //Transform physical values of helicity s1, s2 = -1 and +1, to integers 0 and 1; respectively
    if (s2 == +1) {
        h2 = +1;
        minush2 = 0;
    } else if (s2 == -1) {
        h2 = 0;
        minush2 = 1;
    }
    if (s1 == +1) {
        h1 = +1;
        minush1 = 0;
    } else if (s1 == -1) {
        h1 = 0;
        minush1 = 1;
    }

    for (j = 0; j < 3; j++) {
        tBH2_J1 += sigma[j] * fFunction(sl, L[j], lplus, s, kPrime, k)
                * (Yfunction(s2, s1)
                        * fFunction(sl, lminus, L[j], +1, rPrime[h2], r[h1])
                        + Zfunction(s2, s1)
                                * fFunction(sl, lminus, L[j], -1,
                                        rPrime[minush2], r[minush1]));
    }

    tBH2_J1 = (m_DMSW_F1 + m_DMSW_F2) * tBH2_J1;

    //Adding term in J2:
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 2; i++) {

            tBH2_J2 += sigma[j] * fFunction(sl, L[j], lplus, s, kPrime, k)
                    * gFunction(sl, lminus, rPrime[i], L[j]);
            tBH2_J2 += sigma[j] * fFunction(sl, L[j], lplus, s, kPrime, k)
                    * gFunction(sl, lminus, r[i], L[j]);

        }
    }

    tBH2_J2 = -m_DMSW_F2 * J2function(s2, s1) * tBH2_J2
            / (2. * m_DMSW_Mnucleon);

    double q1Minuslplus[4];
    for (i = 0; i < 4; i++) {
        q1Minuslplus[i] = m_DMSW_q1[i] - m_DMSW_lplus[i];
    }

    tBH2 = pow(m_DMSW_charge_e, 4.) * (tBH2_J1 + tBH2_J2)
            / (-Qcal2 * t * MinkProd(q1Minuslplus, q1Minuslplus));

    return tBH2;

}

std::complex<double> DDVCSProcessDMSW22::ampliVCS(int s2, int s1, int sl,
        double lminus[4], double lplus[4], int s, double kPrime[4], double k[4],
        double Qcal2, double Mll2) {

    //save t value
    double tOrig = m_t;

    //Evaluation at t = tMin, update vectors
    computeInternalVariables(Constant::PROTON_MASS, m_E, m_DMSW_tMin, m_xB,
            m_Q2, m_Q2Prim, m_phi, m_phiL, m_thetaL);

    double r[2][4], rPrime[2][4]; //arrays containing r1, r2, rPrime1, rPrime2
    int i, j, h1, minush1, h2, minush2;
    std::complex<double> tVCS, tVCS_T1, tVCS_T2;

    for (i = 0; i < 4; i++) {
        rPrime[0][i] = m_DMSW_rPrime2[i];
        rPrime[1][i] = m_DMSW_rPrime1[i];
        r[0][i] = m_DMSW_r2[i];
        r[1][i] = m_DMSW_r1[i];
    }

    //Transform physical values of helicity s1, s2 = -1 and +1, to integers 0 and 1
    if (s2 == +1) {
        h2 = +1;
        minush2 = 0;
    } else if (s2 == -1) {
        h2 = 0;
        minush2 = 1;
    }
    if (s1 == +1) {
        h1 = +1;
        minush1 = 0;
    } else if (s1 == -1) {
        h1 = 0;
        minush1 = 1;
    }

    //Checking sign of the zeroth component of m_DMSW_nstarBM and m_DMSW_HATnBM and saving it in s0star and s0n, respectively

    double s0star = m_DMSW_nstarBM[0] / fabs(m_DMSW_nstarBM[0]);
    double s0n = m_DMSW_HATnBM[0] / fabs(m_DMSW_HATnBM[0]);

    //Let's make sure we get only +/-1 with no decimals:
    if (s0star > 0.) {
        s0star = +1.;
    } else if (s0star < 0.) {
        s0star = -1.;
    }

    if (s0n > 0.) {
        s0n = +1.;
    } else if (s0n < 0.) {
        s0n = -1.;
    }

    double snstar[4], sHATn[4];
    for (i = 0; i < 4; i++) {
        snstar[i] = s0star * m_DMSW_nstarBM[i];
        sHATn[i] = s0n * m_DMSW_HATnBM[i];
    }

//Term in T^(1):
    tVCS_T1 = fFunction(sl, lminus, lplus, s, kPrime, k)
            - (s0n * s0star / m_DMSW_pq) * gFunction(sl, lminus, snstar, lplus)
                    * gFunction(s, kPrime, sHATn, k)
            - (s0n * s0star / m_DMSW_pq) * gFunction(sl, lminus, sHATn, lplus)
                    * gFunction(s, kPrime, snstar, k);

    tVCS_T1 *= -0.5;

    tVCS_T1 *= (s0n / m_DMSW_pq) * (m_cffH + m_cffE)
            * (Yfunction(s2, s1) * gFunction(+1, rPrime[h2], sHATn, r[h1])
                    + Zfunction(s2, s1)
                            * gFunction(-1, rPrime[minush2], sHATn, r[minush1]))
            - m_cffE * J2function(s2, s1) / m_DMSW_Mnucleon;

//Adding the term in T^(2):
    int mu, nu;

    for (mu = 0; mu < 4; mu++) {
        for (nu = 0; nu < 4; nu++) {
            tVCS_T2 +=
                    -0.5 * Constant::COMPLEX_UNIT * LCperp(mu, nu)
                            * m_DMSW_metric_[mu][mu]
                            * jFunction(sl, mu, lminus, lplus)
                            * m_DMSW_metric_[nu][nu]
                            * jFunction(s, nu, kPrime, k)
                            * (m_cffHt * J15plus(s2, s1)
                                    + m_cffEt * J25plus(s2, s1)
                                            / (2. * m_DMSW_Mnucleon));
        }
    }

//Complete amplitude for VCS at LO in alpha strong and LT:
    tVCS = pow(m_DMSW_charge_e, 4.) * (tVCS_T1 + tVCS_T2) / (-Qcal2 * Mll2);

//back to original t
    m_t = tOrig;
//    computeInternalVariables(Constant::PROTON_MASS, m_E, m_t, m_xB, m_Q2, m_Q2Prim,
//            m_phi, m_phiL, m_thetaL);

    return tVCS;
}

//************ Cross-sections for BH, VCS and their interference ************//

double DDVCSProcessDMSW22::crossSectionBH(int polariz, double xB, double Qcal2,
        double Mll2, double t, double thetal) {

    /*
     * polariz = 0 for unpolarized target, beam and muon pair
     * polariz = 1 for polarized target but unpolarized beam and muon pair
     */

    double XSEC;
    int s2, s1, sl, s;
    std::complex<double> Abh1 = (0., 0.);
    std::complex<double> Abh1crossed = (0., 0.);
    std::complex<double> Abh2 = (0., 0.);
    std::complex<double> Abh2crossed = (0., 0.);
    std::complex<double> T2_bh1 = (0., 0.);
    std::complex<double> T2_bh2 = (0., 0.);
    std::complex<double> T2_bh12 = (0., 0.);
    std::complex<double> T2_bh = (0., 0.);

    if (polariz == 0) {

        //****** Unpolarized case: average on spins s1, s and sum over s2 and sl ******//

        for (s1 = -1; s1 < 2; s1 = s1 + 2) {
            for (s = -1; s < 2; s = s + 2) {
                for (s2 = -1; s2 < 2; s2 = s2 + 2) {
                    for (sl = -1; sl < 2; sl = sl + 2) {

                        Abh1 = ampliBH1(s2, s1, sl, m_DMSW_lminus, m_DMSW_lplus,
                                s, m_DMSW_kPrime, m_DMSW_k, Mll2, t);

                        Abh1crossed = ampliBH1crossed(s2, s1, sl, m_DMSW_lminus,
                                m_DMSW_lplus, s, m_DMSW_kPrime, m_DMSW_k, Mll2,
                                t);

                        Abh2 = ampliBH2(s2, s1, sl, m_DMSW_lminus, m_DMSW_lplus,
                                s, m_DMSW_kPrime, m_DMSW_k, Qcal2, t);

                        Abh2crossed = ampliBH2crossed(s2, s1, sl, m_DMSW_lminus,
                                m_DMSW_lplus, s, m_DMSW_kPrime, m_DMSW_k, Qcal2,
                                t);

                        // |BH1|^2
                        T2_bh1 += (Abh1 + Abh1crossed)
                                * conj(Abh1 + Abh1crossed);

                        // |BH2|^2
                        T2_bh2 += (Abh2 + Abh2crossed)
                                * conj(Abh2 + Abh2crossed);

                        // BH1-BH2 interference
                        T2_bh12 += (Abh1 + Abh1crossed)
                                * conj(Abh2 + Abh2crossed)
                                + conj(
                                        (Abh1 + Abh1crossed)
                                                * conj(Abh2 + Abh2crossed));
                    }
                }
            }
        }
        //Modulus squared of the whole amplitude for BH process
        T2_bh = T2_bh1 + T2_bh2 + T2_bh12;

        //7-fold differential cross-section
        XSEC =
                pow(Constant::FINE_STRUCTURE_CONSTANT, 4.) * xB
                        * pow(m_DMSW_y, 2.) * real(T2_bh)
                        * (1. / pow(m_DMSW_charge_e, 8.)) * sin(thetal)
                        / (16 * pow(2. * M_PI, 3.) * pow(Qcal2, 2.)
                                * sqrt(
                                        1.
                                                + pow(2. * xB * m_DMSW_Mnucleon,
                                                        2.) / Qcal2));

        XSEC = XSEC / 4.; // 1/4 factor coming from averaging in initial spin states of proton and electron (unpolarized xsec)

    } else if (polariz == 1) {
        //TODO

        XSEC = 0.;
        std::cout << "TO DO POLARIZED CASE" << std::endl;

    }

    return XSEC;
}

double DDVCSProcessDMSW22::crossSectionVCS(int polariz, double xB, double Qcal2,
        double Mll2, double t, double thetal) {

    double XSEC;
    std::complex<double> T2_vcs = (0., 0.);
    std::complex<double> Avcs = (0., 0.);
    int s2, s1, sl, s;

    if (polariz == 0) {
        for (s1 = -1; s1 < 2; s1 = s1 + 2) {
            for (s = -1; s < 2; s = s + 2) {
                for (s2 = -1; s2 < 2; s2 = s2 + 2) {
                    for (sl = -1; sl < 2; sl = sl + 2) {

                        Avcs = ampliVCS(s2, s1, sl, m_DMSW_lminus, m_DMSW_lplus,
                                s, m_DMSW_kPrime, m_DMSW_k, Qcal2, Mll2);

                        T2_vcs += Avcs * conj(Avcs);

                    }
                }
            }
        }

        //7-fold differential cross-section
        XSEC =
                pow(Constant::FINE_STRUCTURE_CONSTANT, 4.) * xB
                        * pow(m_DMSW_y, 2.) * real(T2_vcs)
                        * (1. / pow(m_DMSW_charge_e, 8.)) * sin(thetal)
                        / (16 * pow(2. * M_PI, 3.) * pow(Qcal2, 2.)
                                * sqrt(
                                        1.
                                                + pow(2. * xB * m_DMSW_Mnucleon,
                                                        2.) / Qcal2));

        XSEC = XSEC / 4.; // 1/4 factor coming from averaging in initial spin states of proton and electron (unpolarized xsec)

    } else if (polariz == 1) {
        //TODO

        XSEC = 0.;
        std::cout << "TO DO POLARIZED CASE" << std::endl;

    }

    return XSEC;
}

double DDVCSProcessDMSW22::crossSectionInterf(int polariz, double xB,
        double Qcal2, double Mll2, double t, double thetal) {

    double XSEC;
    std::complex<double> T2_interf = (0., 0.);
    std::complex<double> Avcs, Abh1, Abh1crossed, Abh2, Abh2crossed;
    int s2, s1, sl, s;

    if (polariz == 0) {
        for (s1 = -1; s1 < 2; s1 = s1 + 2) {
            for (s = -1; s < 2; s = s + 2) {
                for (s2 = -1; s2 < 2; s2 = s2 + 2) {
                    for (sl = -1; sl < 2; sl = sl + 2) {

                        Avcs = ampliVCS(s2, s1, sl, m_DMSW_lminus, m_DMSW_lplus,
                                s, m_DMSW_kPrime, m_DMSW_k, Qcal2, Mll2);
                        Abh1 = ampliBH1(s2, s1, sl, m_DMSW_lminus, m_DMSW_lplus,
                                s, m_DMSW_kPrime, m_DMSW_k, Mll2, t);

                        Abh1crossed = ampliBH1crossed(s2, s1, sl, m_DMSW_lminus,
                                m_DMSW_lplus, s, m_DMSW_kPrime, m_DMSW_k, Mll2,
                                t);

                        Abh2 = ampliBH2(s2, s1, sl, m_DMSW_lminus, m_DMSW_lplus,
                                s, m_DMSW_kPrime, m_DMSW_k, Qcal2, t);

                        Abh2crossed = ampliBH2crossed(s2, s1, sl, m_DMSW_lminus,
                                m_DMSW_lplus, s, m_DMSW_kPrime, m_DMSW_k, Qcal2,
                                t);

                        T2_interf += (Abh1 + Abh1crossed) * conj(Avcs)
                                + (Abh2 + Abh2crossed) * conj(Avcs)
                                + conj(
                                        (Abh1 + Abh1crossed) * conj(Avcs)
                                                + (Abh2 + Abh2crossed)
                                                        * conj(Avcs));

                    }
                }
            }
        }

        //7-fold differential cross-section
        XSEC =
                pow(Constant::FINE_STRUCTURE_CONSTANT, 4.) * xB
                        * pow(m_DMSW_y, 2.) * real(T2_interf)
                        * (1. / pow(m_DMSW_charge_e, 8.)) * sin(thetal)
                        / (16 * pow(2. * M_PI, 3.) * pow(Qcal2, 2.)
                                * sqrt(
                                        1.
                                                + pow(2. * xB * m_DMSW_Mnucleon,
                                                        2.) / Qcal2));

        XSEC = XSEC / 4.; // 1/4 factor coming from averaging in initial spin states of proton and electron (unpolarized xsec)

    } else if (polariz == 1) {
        //TODO

        XSEC = 0.;
        std::cout << "TO DO POLARIZED CASE" << std::endl;

    }

    return XSEC;
}

//********************************* Auxiliary functions*****************************//

std::complex<double> DDVCSProcessDMSW22::sKS(const double r1[4],
        const double r2[4]) const {

    std::complex<double> sValue = (r1[2] + Constant::COMPLEX_UNIT * r1[3])
            * sqrt(std::complex<double>(r2[0] - r2[1]) / (r1[0] - r1[1]))
            - (r2[2] + Constant::COMPLEX_UNIT * r2[3])
                    * sqrt(
                            std::complex<double>(r1[0] - r1[1])
                                    / (r2[0] - r2[1])); //KS1985, eq 3.11

    return sValue;

}

std::complex<double> DDVCSProcessDMSW22::tKS(const double r1[4],
        const double r2[4]) const {

    std::complex<double> tValue = conj(sKS(r2, r1)); //KS1985, eq 3.8

    return tValue;

}

std::complex<double> DDVCSProcessDMSW22::fFunction(int lambda, double k0[4],
        double k1[4], int lambdaPrime, double k2[4], double k3[4]) const {

    std::complex<double> fValue;

    if (lambda == -1 && lambdaPrime == +1) {
        fValue = sKS(k2, k1) * tKS(k0, k3);
    } else if (lambda == +1 && lambdaPrime == -1) {
        fValue = tKS(k2, k1) * sKS(k0, k3);
    } else if (lambda == +1 && lambdaPrime == +1) {
        fValue = sKS(k2, k0) * tKS(k1, k3);
    } else if (lambda == -1 && lambdaPrime == -1) {
        fValue = tKS(k2, k0) * sKS(k1, k3);
    }

    return 2. * fValue;
}

std::complex<double> DDVCSProcessDMSW22::gFunction(int lambda, double L[4],
        double R[4], double k[4]) const {

    std::complex<double> gValue;

    if (lambda == +1) {
        gValue = sKS(L, R) * tKS(R, k);
    } else if (lambda == -1) {
        gValue = tKS(L, R) * sKS(R, k);
    }

    return gValue;

}

std::complex<double> DDVCSProcessDMSW22::Yfunction(int s2, int s1) const {

    std::complex<double> Yvalue;

    if (s2 == +1 && s1 == +1) {
        Yvalue = tKS(m_DMSW_rPrime2, m_DMSW_rPrime1) * sKS(m_DMSW_r1, m_DMSW_r2)
                / pow(m_DMSW_Mnucleon, 2.);
    } else if (s2 == +1 && s1 == -1) {
        Yvalue = tKS(m_DMSW_rPrime2, m_DMSW_rPrime1) / m_DMSW_Mnucleon;
    } else if (s2 == -1 && s1 == +1) {
        Yvalue = sKS(m_DMSW_r1, m_DMSW_r2) / m_DMSW_Mnucleon;
    } else if (s2 == -1 && s1 == -1) {
        Yvalue = +1.;
    }

    return Yvalue;
}

std::complex<double> DDVCSProcessDMSW22::Zfunction(int s2, int s1) const {

    std::complex<double> Zvalue;

    if (s2 == -1 && s1 == -1) {
        Zvalue = sKS(m_DMSW_rPrime2, m_DMSW_rPrime1) * tKS(m_DMSW_r1, m_DMSW_r2)
                / pow(m_DMSW_Mnucleon, 2.);
    } else if (s2 == -1 && s1 == +1) {
        Zvalue = sKS(m_DMSW_rPrime2, m_DMSW_rPrime1) / m_DMSW_Mnucleon;
    } else if (s2 == +1 && s1 == -1) {
        Zvalue = tKS(m_DMSW_r1, m_DMSW_r2) / m_DMSW_Mnucleon;
    } else if (s2 == +1 && s1 == +1) {
        Zvalue = +1.;
    }

    return Zvalue;
}

std::complex<double> DDVCSProcessDMSW22::J2function(int s2, int s1) const {

    std::complex<double> J2value;

    if (s2 == +1 && s1 == +1) {
        J2value = (tKS(m_DMSW_rPrime2, m_DMSW_rPrime1)
                * sKS(m_DMSW_rPrime1, m_DMSW_r2)
                + tKS(m_DMSW_rPrime2, m_DMSW_r1) * sKS(m_DMSW_r1, m_DMSW_r2))
                / m_DMSW_Mnucleon;
    } else if (s2 == +1 && s1 == -1) {
        J2value = tKS(m_DMSW_rPrime2, m_DMSW_rPrime1)
                * tKS(m_DMSW_r1, m_DMSW_r2) * sKS(m_DMSW_rPrime1, m_DMSW_r1)
                / pow(m_DMSW_Mnucleon, 2.) + tKS(m_DMSW_rPrime2, m_DMSW_r2);
    } else if (s2 == -1 && s1 == +1) {
        J2value = sKS(m_DMSW_rPrime2, m_DMSW_rPrime1)
                * tKS(m_DMSW_rPrime1, m_DMSW_r1) * sKS(m_DMSW_r1, m_DMSW_r2)
                / pow(m_DMSW_Mnucleon, 2.) + sKS(m_DMSW_rPrime2, m_DMSW_r2);
    } else if (s2 == -1 && s1 == -1) {
        J2value = (sKS(m_DMSW_rPrime2, m_DMSW_rPrime1)
                * tKS(m_DMSW_rPrime1, m_DMSW_r2)
                + tKS(m_DMSW_r1, m_DMSW_r2) * sKS(m_DMSW_rPrime2, m_DMSW_r1))
                / m_DMSW_Mnucleon;
    }

    return J2value;
}

double DDVCSProcessDMSW22::MinkProd(const double p[4],
        const double q[4]) const {

    double prod = 0.;
    int i;

    for (i = 0; i < 4; i++) {
        prod += m_DMSW_metric_[i][i] * p[i] * q[i];
    }

    return prod; //Minkowsky product of two 4-vectors p and q
}

std::complex<double> DDVCSProcessDMSW22::jFunction(int mu, int helic,
        double p1[4], double p2[4]) const {

    std::complex<double> jValue;
    int a, b, c;

    double Np1p2 = sqrt((p1[0] - p1[1]) * (p2[0] - p2[1]));

    double LCp1p2k0 = 0.; //product of LCtensor with p1, p2 and k0 with one upper index free (mu)

    for (a = 0; a < 4; a++) {
        for (b = 0; b < 4; b++) {
            for (c = 0; c < 4; c++) {
                LCp1p2k0 += m_DMSW_LCtensor[mu][a][b][c] * m_DMSW_metric_[a][a]
                        * m_DMSW_metric_[b][b] * m_DMSW_metric_[c][c] * p1[a]
                        * p2[b] * m_DMSW_k0[c];
            }
        }
    }

    jValue = p1[mu] * MinkProd(p2, m_DMSW_k0) + p2[mu] * MinkProd(p1, m_DMSW_k0)
            - m_DMSW_k0[mu] * MinkProd(p1, p2)
            + std::complex<double>(0., helic * LCp1p2k0);

    return jValue / Np1p2;
}

std::complex<double> DDVCSProcessDMSW22::J15plus(int s2, int s1) const {

    std::complex<double> J15value;

//Checking sign of zeroth component of m_DMSW_HATnBM and saving it in s0n
    double s0n = m_DMSW_HATnBM[0] / fabs(m_DMSW_HATnBM[0]);
    double sHATn[4];
    int i;

    for (i = 0; i < 4; i++) {
        sHATn[i] = s0n * m_DMSW_HATnBM[i];
    }

    if (s2 == +1 && s1 == +1) {

        J15value = tKS(m_DMSW_rPrime2, m_DMSW_rPrime1)
                * sKS(m_DMSW_rPrime1, sHATn) * sKS(m_DMSW_r1, m_DMSW_r2)
                * tKS(sHATn, m_DMSW_r1) / pow(m_DMSW_Mnucleon, 2.)
                - tKS(m_DMSW_rPrime2, sHATn) * sKS(sHATn, m_DMSW_r2);

    } else if (s2 == -1 && s1 == -1) {

        J15value = sKS(m_DMSW_rPrime2, m_DMSW_rPrime1)
                * tKS(m_DMSW_rPrime1, sHATn) * tKS(m_DMSW_r1, m_DMSW_r2)
                * sKS(sHATn, m_DMSW_r1) / pow(m_DMSW_Mnucleon, 2.)
                - sKS(m_DMSW_rPrime2, sHATn) * tKS(sHATn, m_DMSW_r2);
        J15value = -J15value;

    } else if (s2 == +1 && s1 == -1) {

        J15value = tKS(m_DMSW_rPrime2, m_DMSW_rPrime1)
                * sKS(m_DMSW_rPrime1, sHATn) * tKS(sHATn, m_DMSW_r2)
                - tKS(m_DMSW_rPrime2, sHATn) * tKS(m_DMSW_r1, m_DMSW_r2)
                        * sKS(sHATn, m_DMSW_r1);
        J15value /= m_DMSW_Mnucleon;

    } else if (s2 == -1 && s1 == +1) {

        J15value = sKS(m_DMSW_rPrime2, m_DMSW_rPrime1)
                * tKS(m_DMSW_rPrime1, sHATn) * sKS(sHATn, m_DMSW_r2)
                - sKS(m_DMSW_rPrime2, sHATn) * sKS(m_DMSW_r1, m_DMSW_r2)
                        * tKS(sHATn, m_DMSW_r1);
        J15value /= m_DMSW_Mnucleon;
        J15value = -J15value;

    }

    return s0n * J15value / m_DMSW_pq;
}

std::complex<double> DDVCSProcessDMSW22::J25plus(int s2, int s1) const {

    std::complex<double> J25value;
    double q[4], p[4];
    int i;

    for (i = 0; i < 4; i++) {
        q[i] = (m_DMSW_q1[i] + m_DMSW_q2[i]) / 2.;
        p[i] = m_DMSW_p1[i] + m_DMSW_p2[i];
    }

    double DeltaPlus = MinkProd(m_DMSW_nBM, m_DMSW_Delta);

    if (s2 == +1 && s1 == +1) {
        J25value = pow(m_DMSW_Mnucleon, -1.)
                * (sKS(m_DMSW_r1, m_DMSW_r2) * tKS(m_DMSW_rPrime2, m_DMSW_r1)
                        - tKS(m_DMSW_rPrime2, m_DMSW_rPrime1)
                                * sKS(m_DMSW_rPrime1, m_DMSW_r2));
    } else if (s2 == -1 && s1 == -1) {
        J25value = -pow(m_DMSW_Mnucleon, -1.)
                * (tKS(m_DMSW_r1, m_DMSW_r2) * sKS(m_DMSW_rPrime2, m_DMSW_r1)
                        - sKS(m_DMSW_rPrime2, m_DMSW_rPrime1)
                                * tKS(m_DMSW_rPrime1, m_DMSW_r2));
    } else if (s2 == +1 && s1 == -1) {
        J25value = tKS(m_DMSW_rPrime2, m_DMSW_r2)
                - pow(m_DMSW_Mnucleon, -2.)
                        * tKS(m_DMSW_rPrime2, m_DMSW_rPrime1)
                        * tKS(m_DMSW_r1, m_DMSW_r2)
                        * sKS(m_DMSW_rPrime1, m_DMSW_r1);
    } else if (s2 == -1 && s1 == +1) {
        J25value = -(sKS(m_DMSW_rPrime2, m_DMSW_r2)
                - pow(m_DMSW_Mnucleon, -2.)
                        * sKS(m_DMSW_rPrime2, m_DMSW_rPrime1)
                        * sKS(m_DMSW_r1, m_DMSW_r2)
                        * tKS(m_DMSW_rPrime1, m_DMSW_r1));
    }

    return DeltaPlus * J25value;
}

double DDVCSProcessDMSW22::LCperp(int mu, int nu) {

    double LCvalue = 0.;

    int a, b;
    for (a = 0; a < 4; a++) {
        for (b = 0; b < 4; b++) {
            LCvalue += m_DMSW_LCtensor[mu][nu][a][b] * m_DMSW_metric_[a][a]
                    * m_DMSW_metric_[b][b] * m_DMSW_nstarBM[a] * m_DMSW_nBM[b];

        }
    }

    return LCvalue;
}

void DDVCSProcessDMSW22::computeInternalVariables(double Mnucleon, double Ebeam,
        double t, double xB, double Qcal2, double Mll2, double phi, double phil,
        double thetal) {

//We change to Belitsky's phi: LHS is phi in Belitsky2003 notation, RHS' phi is in Trento's
    phi = M_PI - phi;

//Evaluation of tMin:
    m_DMSW_epsilon2 = pow(2. * xB * Mnucleon, 2.) / Qcal2;

    m_DMSW_tMin = -1. / (4. * xB * (1. - xB) + m_DMSW_epsilon2);
    m_DMSW_tMin *= (2. * ((1. - xB) * Qcal2 - xB * Mll2)
            + m_DMSW_epsilon2 * (Qcal2 - Mll2)
            - 2. * sqrt(1. + m_DMSW_epsilon2)
                    * sqrt(
                            pow((1. - xB) * Qcal2 - xB * Mll2, 2.)
                                    - m_DMSW_epsilon2 * Qcal2 * Mll2));

//Basic numbers:
    m_DMSW_Mnucleon = Mnucleon;
    m_DMSW_charge_e = sqrt(Constant::FINE_STRUCTURE_CONSTANT * 4. * M_PI);
    m_DMSW_Q2 = 0.5 * (Qcal2 - Mll2 + t / 2.);

//Light-like vectors (r1, r2, r'1, r'2) to build proton momenta (p1, p2)
    m_DMSW_r1[0] = 0.5 * Mnucleon;
    m_DMSW_r1[1] = 0.;
    m_DMSW_r1[2] = 0.;
    m_DMSW_r1[3] = 0.5 * Mnucleon;

    m_DMSW_r2[0] = 0.5 * Mnucleon;
    m_DMSW_r2[1] = 0.;
    m_DMSW_r2[2] = 0.;
    m_DMSW_r2[3] = -0.5 * Mnucleon;

    m_DMSW_p1[0] = Mnucleon;
    m_DMSW_p1[1] = 0.;
    m_DMSW_p1[2] = 0.;
    m_DMSW_p1[3] = 0.;

    double modp2I = sqrt(-t * (1. - t / pow(2. * Mnucleon, 2.))); //modulus of 3D component of p2 in TRF-I frame, eq 12 in BM2003
    double cosThetaN = -(m_DMSW_epsilon2 * 0.5 * (Qcal2 + Mll2 - t) - xB * t)
            / (2 * xB * Mnucleon * sqrt(1. + m_DMSW_epsilon2) * modp2I); //cosine of the angle of p2I with respect to z-axis of TRF-I frame, eq 13 in BM2003
    double sinThetaN = sqrt(1. - pow(cosThetaN, 2.));;

    //Because of eq 19 at t = tMin (sinGamma = 0), q2 = (w2, 0, 0, w2*modv) and momentum conservation:
    if (t == m_DMSW_tMin) {
        sinThetaN = 0.;
    }

    m_DMSW_Q2 = 0.5 * (Qcal2 - Mll2 + t / 2.);

    double w2 = sqrt(Qcal2 / m_DMSW_epsilon2) + t / (2. * Mnucleon); //eq 17 in BM2003
    double modv = sqrt(1. - Mll2 / pow(w2, 2.)); //modulus of outgoing photon's speed in TRF-I frame, eq 17 in BM2003

    double cosGamma = -(sqrt(m_DMSW_epsilon2) * 0.5 * (Qcal2 - Mll2 + t)
            + sqrt(Qcal2) * w2)
            / (sqrt(Qcal2 * (1. + m_DMSW_epsilon2)) * modv * w2); //cosine of angle between outgoing photon and z-axis in TRF-I frame, eq 18 in BM2003
    double sinGamma = sqrt(1. - pow(cosGamma, 2.)); //this expression can produce NaNs when t = tMin (probably because numerics since for t = tMin, cosGamma == -1 so sinGamma == 0)

    //Because of eq 21 in BM2003:
    if (t == m_DMSW_tMin) {
        sinGamma = 0.;
    }

    double p2I[4]; //p2 in TRF-I frame, eqs 12 and 13 in BM2003
    p2I[0] = Mnucleon - t / (2. * Mnucleon);
    p2I[1] = modp2I * sinThetaN * cos(phi);
    p2I[2] = modp2I * sinThetaN * sin(phi);
    p2I[3] = modp2I * cosThetaN;

    m_DMSW_p2[0] = p2I[0];
    m_DMSW_p2[1] = cosGamma
            * (cos(M_PI + phi) * p2I[1] + sin(M_PI + phi) * p2I[2])
            - sinGamma * p2I[3];
    m_DMSW_p2[2] = -sin(M_PI + phi) * p2I[1] + cos(M_PI + phi) * p2I[2];
    m_DMSW_p2[3] = sinGamma
            * (cos(M_PI + phi) * p2I[1] + sin(M_PI + phi) * p2I[2])
            + cosGamma * p2I[3];

    double Alpha = 0.5 * (m_DMSW_p2[0] + m_DMSW_p2[3]);
    double Beta = m_DMSW_p2[0] - m_DMSW_p2[3];
    double Gamma = Alpha - 0.5 * pow(Mnucleon, 2.) / Beta;

    m_DMSW_rPrime1[0] = Alpha - Gamma;
    m_DMSW_rPrime1[1] = 0.;
    m_DMSW_rPrime1[2] = 0.;
    m_DMSW_rPrime1[3] = Alpha - Gamma;

    m_DMSW_rPrime2[0] = Gamma + Beta / 2.;
    m_DMSW_rPrime2[1] = m_DMSW_p2[1];
    m_DMSW_rPrime2[2] = m_DMSW_p2[2];
    m_DMSW_rPrime2[3] = Gamma - Beta / 2.;

//Other momenta
    m_DMSW_y = Qcal2 / (Ebeam * 2. * xB * Mnucleon);

    double cosTheta_e = -(1. + m_DMSW_y * m_DMSW_epsilon2 / 2.)
            / sqrt(1. + m_DMSW_epsilon2);
    double sinTheta_e = sqrt(1. - pow(cosTheta_e, 2.));
    m_DMSW_k[0] = Ebeam;
    m_DMSW_k[1] = Ebeam
            * (sinTheta_e * cosGamma * cos(M_PI + phi) - cosTheta_e * sinGamma);
    m_DMSW_k[2] = -Ebeam * sinTheta_e * sin(M_PI + phi);
    m_DMSW_k[3] = Ebeam
            * (sinTheta_e * sinGamma * cos(M_PI + phi) + cosTheta_e * cosGamma);

    int i;
    for (i = 0; i < 4; i++) {
        m_DMSW_Delta[i] = m_DMSW_p2[i] - m_DMSW_p1[i];
    }

    m_DMSW_q2[0] = w2;
    m_DMSW_q2[1] = 0.;
    m_DMSW_q2[2] = 0.;
    m_DMSW_q2[3] = w2 * modv;

    double w1 = sqrt(Qcal2 / m_DMSW_epsilon2);
    m_DMSW_q1[0] = w1;
    m_DMSW_q1[1] = w1 * sqrt(1. + m_DMSW_epsilon2) * sinGamma;
    m_DMSW_q1[2] = 0.;
    m_DMSW_q1[3] = -w1 * sqrt(1. + m_DMSW_epsilon2) * cosGamma;

    m_DMSW_lminus[0] = 0.5 * w2 * (1. + modv * cos(thetal));
    m_DMSW_lminus[1] = 0.5 * sqrt(Mll2) * sin(thetal) * cos(phil);
    m_DMSW_lminus[2] = 0.5 * sqrt(Mll2) * sin(thetal) * sin(phil);
    m_DMSW_lminus[3] = 0.5 * w2 * (modv + cos(thetal));

    for (i = 0; i < 4; i++) {
        m_DMSW_lplus[i] = m_DMSW_q2[i] - m_DMSW_lminus[i];
        m_DMSW_kPrime[i] = m_DMSW_k[i] - m_DMSW_q1[i];
    }

    double auxVector[4], pbar[4];
    auxVector[0] = 1.;
    auxVector[1] = 0.;
    auxVector[2] = 0.;
    auxVector[3] = -1;

    for (i = 0; i < 4; i++) {
        pbar[i] = 0.5 * (m_DMSW_p1[i] + m_DMSW_p2[i]);
    }

    double N = 0.5 * MinkProd(auxVector, pbar); //normalization constant for nminus and nplus such that nminus * nplus = 1 and nminus * pbar = 1
    m_DMSW_nplus[0] = N;
    m_DMSW_nplus[1] = 0.;
    m_DMSW_nplus[2] = 0.;
    m_DMSW_nplus[3] = N;

    for (i = 0; i < 4; i++) {
        m_DMSW_nminus[i] = (1. / (2. * N)) * auxVector[i];
    }

// xi and pq:
    m_DMSW_xi = 2. * m_DMSW_Q2 / (2. * Qcal2 / xB - Qcal2 - Mll2 + t); //xi variable, the first of the two eqs in BM2003's eq 31 (REMEMBER: we take positive eta)
    m_DMSW_pq = m_DMSW_Q2 / m_DMSW_xi;

// n and n^\star light-like vectors from BM2000 and BM2003. Here, n = m_DMSW_nBM and n^\star = m_DMSW_nstarBM

    double delta2 = (Mnucleon * Mnucleon - t / 4.); //delta2 = \delta^2 * m_DMSW_Q2 with \delta^2 as defined in BM2003 paper (eq A1)
    double root = sqrt(1. + 4. * m_DMSW_xi * delta2 / m_DMSW_pq);

    for (i = 0; i < 4; i++) {
        m_DMSW_nstarBM[i] = -(delta2 / m_DMSW_pq) * (1. / root)
                * (m_DMSW_q1[i] + m_DMSW_q2[i]) / 2.
                + ((1. + root) / (4. * root)) * 2. * pbar[i];
        m_DMSW_nBM[i] = (2. / (m_DMSW_pq * root))
                * (m_DMSW_q1[i] + m_DMSW_q2[i]) / 2.
                - ((1. - root) / (2. * delta2 * root)) * 2. * pbar[i];
        m_DMSW_HATnBM[i] = m_DMSW_pq * m_DMSW_nBM[i];
    }

//EM form factors for proton
    m_DMSW_F1 = (4. * pow(Mnucleon, 2.) - t * 2.7928)
            * pow((1. - t / 0.71), -2.) / (4. * pow(Mnucleon, 2.) - t); //2.7928 = Born's magneton for proton

    m_DMSW_F2 = 4. * pow(Mnucleon, 2.) * (2.7928 - 1.)
            * pow((1. - t / 0.71), -2.) / (4. * pow(Mnucleon, 2.) - t);

//Get CFFs

    if (m_dvcsConvolCoeffFunctionResult.isAvailable(GPDType::H)) {
        m_cffH = m_dvcsConvolCoeffFunctionResult.getLastAvailable();
    }

    if (m_dvcsConvolCoeffFunctionResult.isAvailable(GPDType::E)) {
        m_cffE = m_dvcsConvolCoeffFunctionResult.getLastAvailable();
    }

    if (m_dvcsConvolCoeffFunctionResult.isAvailable(GPDType::Ht)) {
        m_cffHt = m_dvcsConvolCoeffFunctionResult.getLastAvailable();
    }

    if (m_dvcsConvolCoeffFunctionResult.isAvailable(GPDType::Et)) {
        m_cffEt = m_dvcsConvolCoeffFunctionResult.getLastAvailable();
    }

    if (m_dvcsConvolCoeffFunctionResult.isAvailable(GPDType::HL)) {
        m_cffHL = m_dvcsConvolCoeffFunctionResult.getLastAvailable();
    }

    if (m_dvcsConvolCoeffFunctionResult.isAvailable(GPDType::EL)) {
        m_cffEL = m_dvcsConvolCoeffFunctionResult.getLastAvailable();
    }

}

}

/* namespace PARTONS */
