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
    m_DMSW_Mnucleon = 0.;
    m_DMSW_charge_e = 0.;

    int i;
    for (i = 0; i < 4; i++) {
        m_DMSW_r1[i] = 0.;
        m_DMSW_r2[i] = 0.;
        m_DMSW_rPrime1[i] = 0.;
        m_DMSW_rPrime2[i] = 0.;
        m_DMSW_p1[i] = 0.;
        m_DMSW_p2[i] = 0.;
        m_DMSW_p2I[i] = 0.;
        m_DMSW_k[i] = 0.;
        m_DMSW_Delta[i] = 0.;
        m_DMSW_q2[i] = 0.;
        m_DMSW_q1[i] = 0.;
        m_DMSW_kPrime[i] = 0.;
        m_DMSW_lminus[i] = 0.;
        m_DMSW_lplus[i] = 0.;
    }

    m_DMSW_modp2I = 0.;
    m_DMSW_epsilon2 = 0.;
    m_DMSW_Q2 = 0.;
    m_DMSW_F1 = 0.;
    m_DMSW_F2 = 0.;
//    m_DMSW_eta = 0.;
//    m_DMSW_xi = 0.;
    m_DMSW_y = 0.;

}

DDVCSProcessDMSW22::DDVCSProcessDMSW22(const DDVCSProcessDMSW22& other) :
        DDVCSProcessModule(other) {

//    m_DMSW_r1[4] = other.m_DMSW_r1[4];
//    m_DMSW_r2[4] = other.m_DMSW_r2[4];
//    m_DMSW_rPrime1[4] = other.m_DMSW_rPrime1[4];
//    m_DMSW_rPrime2[4] = other.m_DMSW_rPrime2[4];
//    m_DMSW_p1[4] = other.m_DMSW_p1[4];
//    m_DMSW_p2[4] = other.m_DMSW_p2[4];
//    m_DMSW_p2I[4] = other.m_DMSW_p2I[4];

    m_DMSW_Mnucleon = other.m_DMSW_Mnucleon;
    m_DMSW_charge_e = other.m_DMSW_charge_e;

    int i;
    for (i = 0; i < 4; i++) {
        m_DMSW_r1[i] = other.m_DMSW_r1[i];
        m_DMSW_r2[i] = other.m_DMSW_r2[i];

    }
    m_DMSW_epsilon2 = other.m_DMSW_epsilon2;
    m_DMSW_modp2I = other.m_DMSW_modp2I;
    m_DMSW_Q2 = other.m_DMSW_Q2;

    for (i = 0; i < 4; i++) {
        m_DMSW_p2I[i] = other.m_DMSW_p2I[i];
    }

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
    }

    m_DMSW_F1 = other.m_DMSW_F1;
    m_DMSW_F2 = other.m_DMSW_F2;
//    m_DMSW_eta = other.m_DMSW_eta;
//    m_DMSW_xi = other.m_DMSW_xi;
    m_DMSW_y = other.m_DMSW_y;

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

    double xsec = 0.;

    return PhysicalType<double>(xsec, PhysicalUnit::GEVm2);
}

PhysicalType<double> DDVCSProcessDMSW22::CrossSectionInterf() {

    double xsec = 0.;

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

    //DEBUG
    std::cout << sKS(rPrime[1], kPrime) << " =s(r'1, k'); " << tKS(k, r[0]) << " =t(k, r2)" << std::endl;
    std::cout << sKS(m_DMSW_rPrime1, m_DMSW_kPrime) << " =s(r'1, k'); " << tKS(m_DMSW_k, m_DMSW_r2) << " =t(k, r2)" << std::endl;
//    for (s2 = -1; s2 < 2; s2=s2+2) {
//        for (s1 = -1; s1 < 2; s1=s1+2) {
//            std::cout << Yfunction(s2, s1) << " =Y(" << s2 << ", " << s1 << ")" << std::endl;
//        }
//    }
//    for (s2 = -1; s2 < 2; s2=s2+2) {
//        for (s1 = -1; s1 < 2; s1=s1+2) {
//            std::cout << Zfunction(s2, s1) << " =Z(" << s2 << ", " << s1 << ")" << std::endl;
//        }
//    }
//    std::cout << (1. / (Mll2 * t * MinkProd(kMinusDelta, kMinusDelta)))*(m_DMSW_F1 + m_DMSW_F2) << " =(F1+F2)/propagators \n\n" << std::endl;
//    for (i = 0; i < 4; i++) {
//        std::cout << rPrime[0][i] << " =r'2[" << i << "]= " << m_DMSW_rPrime2[i] << std::endl;
//    }
//    for (i = 0; i < 4; i++) {
//        std::cout << rPrime[1][i] << " =r'1[" << i << "]= " << m_DMSW_rPrime1[i] << std::endl;
//    }
//    for (i = 0; i < 4; i++) {
//        std::cout << r[0][i] << " =r2[" << i << "]= " << m_DMSW_r2[i] << std::endl;
//    }
//    for (i = 0; i < 4; i++) {
//        std::cout << r[1][i] << " =r1[" << i << "]= " << m_DMSW_r1[i] << std::endl;
//    }
//    std::cout << "\n\n" << std::endl;
//    for (i = 0; i < 4; i++) {
//        std::cout << lplus[i] << " =l+[" << i << "]= " << m_DMSW_lplus[i] << std::endl;
//    }
//    std::cout << "\n\n" << std::endl;
//    std::cout << fFunction(+1, lminus, lplus, +1, kPrime, kPrime)
//            << " =f(+, l-, l+; +, k', k')" << std::endl;
//    std::cout << fFunction(+1, kPrime, k, +1, rPrime[1], r[1])
//            << " =f(+, k', k; +, r'1, r1)" << std::endl;
//    std::cout << fFunction(1, kPrime, k, -1, rPrime[0], r[0])
//            << " =f(+, k',k; -, r'2, r2)\n\n" << std::endl;
    //std::cout << tBH1_J1 << " =tBH1_J1 F2=0; " << s2 << " " << s1 << " " << s << " " << sl << " =s2 s1 s sl" << std::endl;
//    std::cout << tBH1/pow(m_DMSW_charge_e, 4.) << " =ampliBH1/e⁴ with F2=0; " << s2 << " " << s1 << " " << s << " " << sl << " =s2 s1 s sl" << std::endl;
//    std::cout << "\n" << std::endl;
    //END DEBUG

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
            / (Qcal2 * t * MinkProd(q1Minuslminus, q1Minuslminus));

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
            / (Qcal2 * t * MinkProd(q1Minuslplus, q1Minuslplus));

    return tBH2;

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
        //T2_bh = T2_bh1 + T2_bh2 + T2_bh12;

        T2_bh = T2_bh1; //DEBUG

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

//********************************* Auxiliary functions*****************************//

std::complex<double> DDVCSProcessDMSW22::sKS(double r1[4], double r2[4]) const {

    std::complex<double> sValue = (r1[2] + Constant::COMPLEX_UNIT * r1[3])
            * sqrt(std::complex<double>(r2[0] - r2[1]) / (r1[0] - r1[1]))
            - (r2[2] + Constant::COMPLEX_UNIT * r2[3])
                    * sqrt(
                            std::complex<double>(r1[0] - r1[1])
                                    / (r2[0] - r2[1])); //KS1985, eq 3.11

    return sValue;

}

std::complex<double> DDVCSProcessDMSW22::tKS(double r1[4], double r2[4]) const {

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
    double rPrime1[4];
    double rPrime2[4];
    double r1[4];
    double r2[4];
    int i;

    for (i = 0; i < 4; i++) {
        rPrime1[i] = m_DMSW_rPrime1[i];
        rPrime2[i] = m_DMSW_rPrime2[i];
        r1[i] = m_DMSW_r1[i];
        r2[i] = m_DMSW_r2[i];
    }

    if (s2 == +1 && s1 == +1) {
        Yvalue = tKS(rPrime2, rPrime1) * sKS(r1, r2) / pow(m_DMSW_Mnucleon, 2.);
    } else if (s2 == +1 && s1 == -1) {
        Yvalue = tKS(rPrime2, rPrime1) / m_DMSW_Mnucleon;
    } else if (s2 == -1 && s1 == +1) {
        Yvalue = sKS(r1, r2) / m_DMSW_Mnucleon;
    } else if (s2 == -1 && s1 == -1) {
        Yvalue = +1.;
    }

    return Yvalue;
}

std::complex<double> DDVCSProcessDMSW22::Zfunction(int s2, int s1) const {

    std::complex<double> Zvalue;
    double rPrime1[4];
    double rPrime2[4];
    double r1[4];
    double r2[4];
    int i;

    for (i = 0; i < 4; i++) {
        rPrime1[i] = m_DMSW_rPrime1[i];
        rPrime2[i] = m_DMSW_rPrime2[i];
        r1[i] = m_DMSW_r1[i];
        r2[i] = m_DMSW_r2[i];
    }

    if (s2 == -1 && s1 == -1) {
        Zvalue = sKS(rPrime2, rPrime1) * tKS(r1, r2) / pow(m_DMSW_Mnucleon, 2.);
    } else if (s2 == -1 && s1 == +1) {
        Zvalue = sKS(rPrime2, rPrime1) / m_DMSW_Mnucleon;
    } else if (s2 == +1 && s1 == -1) {
        Zvalue = tKS(r1, r2) / m_DMSW_Mnucleon;
    } else if (s2 == +1 && s1 == +1) {
        Zvalue = +1.;
    }

    return Zvalue;
}

std::complex<double> DDVCSProcessDMSW22::J2function(int s2, int s1) const {

    std::complex<double> J2value;
    double rPrime1[4];
    double rPrime2[4];
    double r1[4];
    double r2[4];
    int i;

    for (i = 0; i < 4; i++) {
        rPrime1[i] = m_DMSW_rPrime1[i];
        rPrime2[i] = m_DMSW_rPrime2[i];
        r1[i] = m_DMSW_r1[i];
        r2[i] = m_DMSW_r2[i];
    }

    if (s2 == +1 && s1 == +1) {
        J2value = (tKS(rPrime2, rPrime1) * sKS(rPrime1, r2)
                + tKS(rPrime2, r1) * sKS(r1, r2)) / m_DMSW_Mnucleon;
    } else if (s2 == +1 && s1 == -1) {
        J2value = tKS(rPrime2, rPrime1) * tKS(r1, r2) * sKS(rPrime1, r1)
                / pow(m_DMSW_Mnucleon, 2.) + tKS(rPrime2, r2);
    } else if (s2 == -1 && s1 == +1) {
        J2value = sKS(rPrime2, rPrime1) * tKS(rPrime1, r1) * sKS(r1, r2)
                / pow(m_DMSW_Mnucleon, 2.) + sKS(rPrime2, r2);
    } else if (s2 == -1 && s1 == -1) {
        J2value = (sKS(rPrime2, rPrime1) * tKS(rPrime1, r2)
                + tKS(r1, r2) * sKS(rPrime2, r1)) / m_DMSW_Mnucleon;
    }

    return J2value;
}

double DDVCSProcessDMSW22::MinkProd(double p[4], double q[4]) const {

    double prod;
    int i;
    prod = p[0] * q[0];

    for (i = 1; i < 4; i++) {
        prod -= p[i] * q[i];
    }

    return prod; //Minkowsky product of two 4-vectors p and q
}

void DDVCSProcessDMSW22::computeInternalVariables(double Mnucleon, double Ebeam,
        double t, double xB, double Qcal2, double Mll2, double phi, double phil,
        double thetal) {

    //We change to Belitsky's phi: LHS is phi in Belitsky2003 notation, RHS' phi is in Trento's
    phi = M_PI - phi;

    m_DMSW_Mnucleon = Mnucleon;
    m_DMSW_charge_e = sqrt(Constant::FINE_STRUCTURE_CONSTANT * 4. * M_PI);

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

    m_DMSW_epsilon2 = pow(2. * xB * Mnucleon, 2.) / Qcal2;
    m_DMSW_modp2I = sqrt(-t * (1. - t / pow(2. * Mnucleon, 2.)));
    double cosThetaN = -(m_DMSW_epsilon2 * 0.5 * (Qcal2 + Mll2 - t) - xB * t)
            / (2 * xB * Mnucleon * sqrt(1. + m_DMSW_epsilon2) * m_DMSW_modp2I); //cosine of the angle of p2I with respect to z-axis of TRF-I frame, eq 13 in BM2003
    double sinThetaN = sqrt(1. - pow(cosThetaN, 2.));
    m_DMSW_Q2 = 0.5 * (Qcal2 - Mll2 + t / 2.);

    double w2 = sqrt(Qcal2 / m_DMSW_epsilon2) + t / (2. * Mnucleon); //eq 17 in BM2003
    double modv = sqrt(1. - Mll2 / pow(w2, 2.)); //modulus of outgoing photon's speed in TRF-I frame, eq 17 in BM2003
    double cosGamma = -(sqrt(m_DMSW_epsilon2) * 0.5 * (Qcal2 - Mll2 + t)
            + sqrt(Qcal2) * w2)
            / (sqrt(Qcal2 * (1. + m_DMSW_epsilon2)) * modv * w2); //cosine of angle between outgoing photon and z-axis in TRF-I frame, eq 18 in BM2003
    double sinGamma = sqrt(1. - pow(cosGamma, 2.));

    m_DMSW_p2I[0] = Mnucleon - t / (2. * Mnucleon);
    m_DMSW_p2I[1] = m_DMSW_modp2I * sinThetaN * cos(phi);
    m_DMSW_p2I[2] = m_DMSW_modp2I * sinThetaN * sin(phi);
    m_DMSW_p2I[3] = m_DMSW_modp2I * cosThetaN;

    m_DMSW_p2[0] = m_DMSW_p2I[0];
    m_DMSW_p2[1] =
            cosGamma
                    * (cos(M_PI + phi) * m_DMSW_p2I[1]
                            + sin(M_PI + phi) * m_DMSW_p2I[2])
                    - sinGamma * m_DMSW_p2I[3];
    m_DMSW_p2[2] = -sin(M_PI + phi) * m_DMSW_p2I[1]
            + cos(M_PI + phi) * m_DMSW_p2I[2];
    m_DMSW_p2[3] =
            sinGamma
                    * (cos(M_PI + phi) * m_DMSW_p2I[1]
                            + sin(M_PI + phi) * m_DMSW_p2I[2])
                    + cosGamma * m_DMSW_p2I[3];

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

    //EM form factors for proton
    m_DMSW_F1 = (4. * pow(Mnucleon, 2.) - t * 2.7928)
            * pow((1. - t / 0.71), -2.) / (4. * pow(Mnucleon, 2.) - t); //2.7928 = Born's magneton for proton
//    m_DMSW_F2 = 4. * pow(Mnucleon, 2.) * (2.7928 - 1.)
//            * pow((1. - t / 0.71), -2.) / (4. * pow(Mnucleon, 2.) - t);

    m_DMSW_F2 = 0.; //DEBUG

    //DEBUG-CHECKING:
//    std::cout << phi << " =phi   CHECKS" << std::endl;
//    double eta = (Qcal2 + Mll2) / (2. * Qcal2 / xB - Qcal2 - Mll2 + t); //eta = -Delta*q/(p*q), p = p1+p2, q = (q1+q2)/2
//    double xi = eta * 2. * m_DMSW_Q2 / (Qcal2 + Mll2); //xi = -q^2/(p*q)
//    double tmin = -4. * pow(m_DMSW_Mnucleon * eta, 2.) / (1. - eta * eta);
//    double K = (0.5 / eta) * sqrt(-xi * t / m_DMSW_Q2) * sqrt(1. - tmin / t)
//            * sqrt((1. - eta) / (1. + eta))
//            * sqrt((1. - m_DMSW_y) * (xi + eta));
//    double ytilde = 2. / (1. + cos(thetal));
//    double Ktilde = (0.5 / eta) * sqrt(-xi * t / m_DMSW_Q2)
//            * sqrt(1. - tmin / t) * sqrt((1. - eta) / (1. + eta))
//            * sqrt((1. - ytilde) * (xi - eta));
//    double ytildePlus = 2. / (1 - cos(thetal));
//    double KtildePlus = (0.5 / eta) * sqrt(-xi * t / m_DMSW_Q2)
//            * sqrt(1. - tmin / t) * sqrt((1. - eta) / (1. + eta))
//            * sqrt((1. - ytildePlus) * (xi - eta));
//    std::cout << "myResult =variableBeingComputed= BM2003Results   CHECKS"
//            << std::endl;
//    std::cout << MinkProd(m_DMSW_r1, m_DMSW_r1) << " =r1^2= 0   CHECKS"
//            << std::endl;
//    std::cout << MinkProd(m_DMSW_r2, m_DMSW_r2) << " =r2^2= 0   CHECKS"
//            << std::endl;
//    std::cout << MinkProd(m_DMSW_rPrime1, m_DMSW_rPrime1)
//            << " =r'1^2= 0   CHECKS" << std::endl;
//    std::cout << MinkProd(m_DMSW_rPrime2, m_DMSW_rPrime2)
//            << " =r'2^2= 0   CHECKS" << std::endl;
//    std::cout << MinkProd(m_DMSW_p1, m_DMSW_p1) << " =p1^2= "
//            << pow(m_DMSW_Mnucleon, 2.) << "   CHECKS" << std::endl;
//    std::cout << MinkProd(m_DMSW_p2, m_DMSW_p2) << " =p2^2= "
//            << pow(m_DMSW_Mnucleon, 2.) << "   CHECKS" << std::endl;
//    double r1Plusr2[4], rPrime1PlusrPrime2[4];
//    for (i = 0; i < 4; i++) {
//        r1Plusr2[i] = m_DMSW_r1[i] + m_DMSW_r2[i];
//        rPrime1PlusrPrime2[i] = m_DMSW_rPrime1[i] + m_DMSW_rPrime2[i];
//    }
//    std::cout << MinkProd(r1Plusr2, r1Plusr2) << " =(r1+r2)^2= "
//            << pow(m_DMSW_Mnucleon, 2.) << "   CHECKS" << std::endl;
//    std::cout << MinkProd(rPrime1PlusrPrime2, rPrime1PlusrPrime2)
//            << " =(r'1+r'2)^2= " << pow(m_DMSW_Mnucleon, 2.) << "   CHECKS"
//            << std::endl;
//    std::cout << MinkProd(m_DMSW_k, m_DMSW_k) << " =k^2= 0   CHECKS"
//            << std::endl;
//    std::cout << MinkProd(m_DMSW_kPrime, m_DMSW_kPrime) << " =k'^2= 0   CHECKS"
//            << std::endl;
//    std::cout << MinkProd(m_DMSW_lminus, m_DMSW_lminus) << " =l-^2= 0   CHECKS"
//            << std::endl;
//    std::cout << MinkProd(m_DMSW_lplus, m_DMSW_lplus) << " =l+^2= 0   CHECKS"
//            << std::endl;
//    std::cout << MinkProd(m_DMSW_q1, m_DMSW_q1) << " =q1^2= " << -Qcal2
//            << "   CHECKS" << std::endl;
//    std::cout << MinkProd(m_DMSW_q2, m_DMSW_q2) << " =q2^2= " << Mll2
//            << "   CHECKS" << std::endl;
//    std::cout << MinkProd(m_DMSW_Delta, m_DMSW_Delta) << " =Delta^2= " << t
//            << "   CHECKS" << std::endl;
//    std::cout << MinkProd(m_DMSW_k, m_DMSW_Delta) << " =k·Delta= "
//            << -m_DMSW_Q2 * eta * (1. - 2. * K * cos(M_PI + m_phi))
//                    / (m_DMSW_y * xi) << " (up to 1/(pq))   CHECKS"
//            << std::endl;
//    double tmaxExact = -(1. / (4. * xB * (1. - xB) + m_DMSW_epsilon2))
//            * (2. * ((1. - xB) * Qcal2 - xB * Mll2)
//                    + m_DMSW_epsilon2 * (Qcal2 - Mll2)
//                    + 2. * sqrt(1. + m_DMSW_epsilon2)
//                            * sqrt(
//                                    pow((1. - xB) * Qcal2 - xB * Mll2, 2.)
//                                            - m_DMSW_epsilon2 * Qcal2 * Mll2)); //eq 22 in BM2003
//    double tminExact = -(1. / (4. * xB * (1. - xB) + m_DMSW_epsilon2))
//            * (2. * ((1. - xB) * Qcal2 - xB * Mll2)
//                    + m_DMSW_epsilon2 * (Qcal2 - Mll2)
//                    - 2. * sqrt(1. + m_DMSW_epsilon2)
//                            * sqrt(
//                                    pow((1. - xB) * Qcal2 - xB * Mll2, 2.)
//                                            - m_DMSW_epsilon2 * Qcal2 * Mll2)); //eq 22 in BM2003
//    double Kexact = (1. / (2. * (Qcal2 + Mll2)))
//            * sqrt(
//                    -(1. - m_DMSW_y - pow(m_DMSW_y, 2.) * m_DMSW_epsilon2 / 4.)
//                            * (4. * xB * (1. - xB) + m_DMSW_epsilon2)
//                            * (t - tminExact) * (t - tmaxExact)); //eq 26 in BM2003
//    double BM_lminusDelta =
//            (-1. / (4. * modv))
//                    * ((Qcal2 + Mll2)
//                            * (modv + cos(thetal)
//                                    + 2. * sqrt(Qcal2 * Mll2) * Kexact
//                                            * sin(thetal) * cos(phil)
//                                            / ((Qcal2 + xB * t)
//                                                    * sqrt(
//                                                            1. - m_DMSW_y
//                                                                    - m_DMSW_y
//                                                                            * m_DMSW_y
//                                                                            * m_DMSW_epsilon2
//                                                                            / 4.)))
//                            + t
//                                    * (modv
//                                            + (Qcal2 - 2. * xB * Mll2 + xB * t)
//                                                    * cos(thetal)
//                                                    / (Qcal2 + xB * t))); //eq 25 in BM2003
//    double BM_lplusDelta =
//            (-1. / (4. * modv))
//                    * ((Qcal2 + Mll2)
//                            * (modv + cos(M_PI - thetal)
//                                    + 2. * sqrt(Qcal2 * Mll2) * Kexact
//                                            * sin(M_PI - thetal)
//                                            * cos(phil + M_PI)
//                                            / ((Qcal2 + xB * t)
//                                                    * sqrt(
//                                                            1. - m_DMSW_y
//                                                                    - m_DMSW_y
//                                                                            * m_DMSW_y
//                                                                            * m_DMSW_epsilon2
//                                                                            / 4.)))
//                            + t
//                                    * (modv
//                                            + (Qcal2 - 2. * xB * Mll2 + xB * t)
//                                                    * cos(M_PI - thetal)
//                                                    / (Qcal2 + xB * t)));
//    std::cout << MinkProd(m_DMSW_lminus, m_DMSW_Delta) << " =l-·Delta= "
//            << -m_DMSW_Q2 * eta * (1. + 2. * Ktilde * cos(m_phiL))
//                    / (ytilde * xi) << " (up to 1/(pq)) = " << BM_lminusDelta
//            << " (exact)   CHECKS" << std::endl;
//    std::cout << MinkProd(m_DMSW_lplus, m_DMSW_Delta) << " =l+·Delta= "
//            << -m_DMSW_Q2 * eta * (1. - 2. * KtildePlus * cos(m_phiL))
//                    / (ytildePlus * xi) << " (up to 1/(pq)) = "
//            << -BM_lminusDelta - 2. * m_DMSW_Q2 + 0.5 * Qcal2 - (3. / 2.) * Mll2
//            << " = " << BM_lplusDelta << " (exact)   CHECKS" << std::endl;
//    std::cout << fabs(sKS(m_DMSW_r1, m_DMSW_r2)) / m_DMSW_Mnucleon << " = "
//            << fabs(tKS(m_DMSW_r1, m_DMSW_r2)) / m_DMSW_Mnucleon
//            << " =|sKS(r1, r2)|/M = |tKS(r1, r2)|/M= 1   CHECKS" << std::endl;
//    std::cout << fabs(sKS(m_DMSW_rPrime1, m_DMSW_rPrime2)) / m_DMSW_Mnucleon
//            << " = "
//            << fabs(tKS(m_DMSW_rPrime1, m_DMSW_rPrime2)) / m_DMSW_Mnucleon
//            << " =|sKS(r'1, r'2)|/M =|tKS(r'1, r'2)|/M= 1   CHECKS"
//            << std::endl;
//    std::complex<double> my_gPlus, Jakub_gPlus;
//    int s = +1;
//    my_gPlus = gFunction(s, m_DMSW_kPrime, m_DMSW_r1, m_DMSW_k)
//            + gFunction(s, m_DMSW_kPrime, m_DMSW_r2, m_DMSW_k)
//            + gFunction(s, m_DMSW_kPrime, m_DMSW_rPrime1, m_DMSW_k)
//            + gFunction(s, m_DMSW_kPrime, m_DMSW_rPrime2, m_DMSW_k);
//    my_gPlus += gFunction(s, m_DMSW_lminus, m_DMSW_r1, m_DMSW_k)
//            + gFunction(s, m_DMSW_lminus, m_DMSW_r2, m_DMSW_k)
//            + gFunction(s, m_DMSW_lminus, m_DMSW_rPrime1, m_DMSW_k)
//            + gFunction(s, m_DMSW_lminus, m_DMSW_rPrime2, m_DMSW_k);
//    my_gPlus += gFunction(s, m_DMSW_lplus, m_DMSW_r1, m_DMSW_k)
//            + gFunction(s, m_DMSW_lplus, m_DMSW_r2, m_DMSW_k)
//            + gFunction(s, m_DMSW_lplus, m_DMSW_rPrime1, m_DMSW_k)
//            + gFunction(s, m_DMSW_lplus, m_DMSW_rPrime2, m_DMSW_k);
//    double P[4] = { Mnucleon, 0., 0., Mnucleon };
//    double N[4] = { Mnucleon, 0., 0., -Mnucleon };
//    Jakub_gPlus = gFunction(s, m_DMSW_kPrime, P, m_DMSW_k)
//            + gFunction(s, m_DMSW_kPrime, N, m_DMSW_k)
//            + gFunction(s, m_DMSW_kPrime, m_DMSW_k, m_DMSW_k)
//            - gFunction(s, m_DMSW_kPrime, m_DMSW_kPrime, m_DMSW_k)
//            - gFunction(s, m_DMSW_kPrime, m_DMSW_lplus, m_DMSW_k)
//            - gFunction(s, m_DMSW_kPrime, m_DMSW_lminus, m_DMSW_k);
//    Jakub_gPlus += gFunction(s, m_DMSW_kPrime, P, m_DMSW_k)
//            + gFunction(s, m_DMSW_lminus, N, m_DMSW_k)
//            + gFunction(s, m_DMSW_lminus, m_DMSW_k, m_DMSW_k)
//            - gFunction(s, m_DMSW_lminus, m_DMSW_kPrime, m_DMSW_k)
//            - gFunction(s, m_DMSW_lminus, m_DMSW_lplus, m_DMSW_k)
//            - gFunction(s, m_DMSW_lminus, m_DMSW_lminus, m_DMSW_k);
//    Jakub_gPlus += gFunction(s, m_DMSW_lplus, P, m_DMSW_k)
//            + gFunction(s, m_DMSW_lplus, N, m_DMSW_k)
//            + gFunction(s, m_DMSW_lplus, m_DMSW_k, m_DMSW_k)
//            - gFunction(s, m_DMSW_lplus, m_DMSW_kPrime, m_DMSW_k)
//            - gFunction(s, m_DMSW_lplus, m_DMSW_lplus, m_DMSW_k)
//            - gFunction(s, m_DMSW_lplus, m_DMSW_lminus, m_DMSW_k);
//    std::cout << my_gPlus << " =g(+, {k',l-,l+}, {r1,r2,r'1,r'2}, k) mine; "
//            << Jakub_gPlus
//            << " =g(+, {k',l-,l+}, {P,N,k,-k',-l+,-l-}, k) Jakub's   CHECKS"
//            << std::endl;
//    std::complex<double> my_gMinus, Jakub_gMinus;
//    s = -1;
//    my_gMinus = gFunction(s, m_DMSW_kPrime, m_DMSW_r1, m_DMSW_k)
//            + gFunction(s, m_DMSW_kPrime, m_DMSW_r2, m_DMSW_k)
//            + gFunction(s, m_DMSW_kPrime, m_DMSW_rPrime1, m_DMSW_k)
//            + gFunction(s, m_DMSW_kPrime, m_DMSW_rPrime2, m_DMSW_k);
//    my_gMinus += gFunction(s, m_DMSW_lminus, m_DMSW_r1, m_DMSW_k)
//            + gFunction(s, m_DMSW_lminus, m_DMSW_r2, m_DMSW_k)
//            + gFunction(s, m_DMSW_lminus, m_DMSW_rPrime1, m_DMSW_k)
//            + gFunction(s, m_DMSW_lminus, m_DMSW_rPrime2, m_DMSW_k);
//    my_gMinus += gFunction(s, m_DMSW_lplus, m_DMSW_r1, m_DMSW_k)
//            + gFunction(s, m_DMSW_lplus, m_DMSW_r2, m_DMSW_k)
//            + gFunction(s, m_DMSW_lplus, m_DMSW_rPrime1, m_DMSW_k)
//            + gFunction(s, m_DMSW_lplus, m_DMSW_rPrime2, m_DMSW_k);
//    Jakub_gMinus = gFunction(s, m_DMSW_kPrime, P, m_DMSW_k)
//            + gFunction(s, m_DMSW_kPrime, N, m_DMSW_k)
//            + gFunction(s, m_DMSW_kPrime, m_DMSW_k, m_DMSW_k)
//            - gFunction(s, m_DMSW_kPrime, m_DMSW_kPrime, m_DMSW_k)
//            - gFunction(s, m_DMSW_kPrime, m_DMSW_lplus, m_DMSW_k)
//            - gFunction(s, m_DMSW_kPrime, m_DMSW_lminus, m_DMSW_k);
//    Jakub_gMinus += gFunction(s, m_DMSW_kPrime, P, m_DMSW_k)
//            + gFunction(s, m_DMSW_lminus, N, m_DMSW_k)
//            + gFunction(s, m_DMSW_lminus, m_DMSW_k, m_DMSW_k)
//            - gFunction(s, m_DMSW_lminus, m_DMSW_kPrime, m_DMSW_k)
//            - gFunction(s, m_DMSW_lminus, m_DMSW_lplus, m_DMSW_k)
//            - gFunction(s, m_DMSW_lminus, m_DMSW_lminus, m_DMSW_k);
//    Jakub_gMinus += gFunction(s, m_DMSW_lplus, P, m_DMSW_k)
//            + gFunction(s, m_DMSW_lplus, N, m_DMSW_k)
//            + gFunction(s, m_DMSW_lplus, m_DMSW_k, m_DMSW_k)
//            - gFunction(s, m_DMSW_lplus, m_DMSW_kPrime, m_DMSW_k)
//            - gFunction(s, m_DMSW_lplus, m_DMSW_lplus, m_DMSW_k)
//            - gFunction(s, m_DMSW_lplus, m_DMSW_lminus, m_DMSW_k);
//    std::cout << my_gMinus << " =g(+, {k',l-,l+}, {r1,r2,r'1,r'2}, k) mine; "
//            << Jakub_gMinus
//            << " =g(+, {k',l-,l+}, {P,N,k,-k',-l+,-l-}, k) Jakub's   CHECKS"
//            << std::endl;
//    for (i = 0; i < 4; i++) {
//        std::cout << m_DMSW_p2[i] << " =p2[" << i << "]= "
//                << m_DMSW_rPrime1[i] + m_DMSW_rPrime2[i] << " =(r'1+r'2)[" << i
//                << "]   CHECKS" << std::endl;
//    }
//    //END OF DEBUG-CHECKING

}

}

/* namespace PARTONS */
