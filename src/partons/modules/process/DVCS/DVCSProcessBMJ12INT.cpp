#include <NumA/linear_algebra/vector/Vector3D.h>
#include <cmath>
#include <cstdlib>

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/process/DVCS/DVCSProcessBMJ12INT.h"

// Initialise [class]::classId with a unique name.
const unsigned int DVCSProcessBMJ12INT::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSProcessBMJ12INT("DVCSProcessBMJ12INT"));

/*--------------------------------------- Constructors ---------------------------------*/

DVCSProcessBMJ12INT::DVCSProcessBMJ12INT(const std::string &className) :
        DVCSModule(className) {
    m_phi1BMK = 0.;
    m_phi2BMK = 0.;
    m_PhiBMK = 0.;
    m_theta = 0.;
    m_Lambda = 0.;
    m_lambda = 0.;
    m_xB2 = 0.;
    m_yBMJ.assign(3, 0.);
    m_Q.assign(4, 0.);
    m_xBtQ2.assign(3, 0.);
    m_M.assign(2, 0.);
    m_epsilonBMJ.assign(2, 0.);
    m_epsroot.assign(6, 0.);
    m_K.assign(2, 0.);
    m_Kt.assign(2, 0.);
    m_Delta2.assign(2, 0.);
    m_cBH.assign(3, std::vector<double>(3, 0.));
    m_s1BHTP = 0.;
    m_P1 = 0.;
    m_P2 = 0.;
    m_F1 = 0.;
    m_F2 = 0.;
    m_cVCS.assign(3, std::vector<double>(3, 0.));
    m_sVCS.assign(3, std::vector<double>(3, 0.));
    m_cI.assign(3, std::vector<double>(4, 0.));
    m_sI.assign(3, std::vector<double>(4, 0.));
    m_C.assign(3,
            std::vector<std::vector<double> >(3, std::vector<double>(4, 0.)));
    m_S.assign(3,
            std::vector<std::vector<double> >(3, std::vector<double>(4, 0.)));
    m_dC.assign(3,
            std::vector<std::vector<double> >(3, std::vector<double>(4, 0.)));
    m_dS.assign(3,
            std::vector<std::vector<double> >(3, std::vector<double>(4, 0.)));
    m_cF.assign(3, std::vector<double>(3, 0.));
    m_CFF.assign(4, std::vector<std::complex<double> >(3, 0.));
}

/*-------------------------------------- Destructor ------------------------------------*/

DVCSProcessBMJ12INT::~DVCSProcessBMJ12INT() {
}

DVCSProcessBMJ12INT::DVCSProcessBMJ12INT(const DVCSProcessBMJ12INT& other) :
        DVCSModule(other) {
    m_phi1BMK = other.m_phi1BMK;
    m_phi2BMK = other.m_phi2BMK;
    m_PhiBMK = other.m_PhiBMK;
    m_theta = other.m_theta;
    m_Lambda = other.m_Lambda;
    m_lambda = other.m_lambda;
    m_xB2 = other.m_xB2;
    m_yBMJ = other.m_yBMJ;
    m_Q = other.m_Q;
    m_xBtQ2 = other.m_xBtQ2;
    m_M = other.m_M;
    m_epsilonBMJ = other.m_epsilonBMJ;
    m_epsroot = other.m_epsroot;
    m_K = other.m_K;
    m_Kt = other.m_Kt;
    m_Delta2 = other.m_Delta2;
    m_cBH = other.m_cBH;
    m_s1BHTP = other.m_s1BHTP;
    m_P1 = other.m_P1;
    m_P2 = other.m_P2;
    m_F1 = other.m_F1;
    m_F2 = other.m_F2;
    m_cVCS = other.m_cVCS;
    m_sVCS = other.m_sVCS;
    m_cI = other.m_cI;
    m_sI = other.m_sI;
    m_C = other.m_C;
    m_S = other.m_S;
    m_dC = other.m_dC;
    m_dS = other.m_dS;
    m_cF = other.m_cF;
    m_CFF = other.m_CFF;
}

DVCSProcessBMJ12INT* DVCSProcessBMJ12INT::clone() const {
    return new DVCSProcessBMJ12INT(*this);
}

void DVCSProcessBMJ12INT::initModule() {

    //init mother class
    DVCSModule::initModule();

    m_xB2 = m_xB * m_xB;
    m_Q[0] = sqrt(m_Q2);
    m_Q[1] = m_Q2;
    m_Q[2] = m_Q[0] * m_Q2;
    m_Q[3] = m_Q2 * m_Q2;
    m_Delta2[0] = m_t;
    m_Delta2[1] = m_t * m_t;
    m_xBtQ2[0] = 2 - m_xB + m_xB * m_t / m_Q2;
    m_xBtQ2[1] = m_xBtQ2[0] * m_xBtQ2[0];
    m_xBtQ2[2] = m_xBtQ2[1] * m_xBtQ2[0];
    m_M[0] = Constant::PROTON_MASS; // 0.938271998 // Test Maxime;
    m_M[1] = m_M[0] * m_M[0];
    m_yBMJ[0] = m_y;
    m_yBMJ[1] = m_yBMJ[0] * m_yBMJ[0];
    m_yBMJ[2] = m_yBMJ[0] * m_yBMJ[1];
    m_epsilonBMJ[0] = m_epsilon;
    m_epsilonBMJ[1] = m_epsilonBMJ[0] * m_epsilonBMJ[0];
    m_epsroot[1] = 1 + m_epsilonBMJ[1];
    m_epsroot[0] = sqrt(m_epsroot[1]);
    m_epsroot[2] = m_epsroot[1] * m_epsroot[0];
    m_epsroot[3] = m_epsroot[1] * m_epsroot[1];
    m_epsroot[4] = m_epsroot[2] * m_epsroot[1];
    m_epsroot[5] = m_epsroot[2] * m_epsroot[2];
    m_K[1] = -(m_t / m_Q2) * (1 - m_xB)
            * (1 - m_yBMJ[0] - m_yBMJ[1] * m_epsilonBMJ[1] / 4)
            * (1 - m_tmin / m_t)
            * (m_epsroot[0]
                    + (4 * m_xB * (1 - m_xB) + m_epsilonBMJ[1]) / (4 * (1 - m_xB))
                            * (m_t - m_tmin) / m_Q2);
    m_K[0] = sqrt(m_K[1]);
    m_Kt[1] = ((1 - m_xB) * m_xB + m_epsilonBMJ[1] / 4.) * (m_tmin - m_t)
            * (m_t - m_tmax) / m_Q2;
    m_Kt[0] = sqrt(m_Kt[1]);
    // double K = sqrt(1 - m_yBMJ[0] - m_epsilonBMJ[1] * m_yBMJ[1] / 4.) * m_Kt[0] / m_Q[0]; // Test

    // Dirac and Pauli form factors
    computeFormFactors();

    // Compute CFFs F+b and F0+
    computeCFFs();

    // Compute Angular coeffs for Interf cross section
    computeAngularCoeffsInterf();

    // Phase space
    m_phaseSpace = m_xB * m_yBMJ[1] / (1024 * pow(Constant::PI, 5) * m_Q[3] * m_epsroot[0]);

}

void DVCSProcessBMJ12INT::initModule(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {

    //init mother class
    DVCSModule::initModule(beamHelicity, beamCharge, targetPolarization);

    // define the angles and Lambda
    defineAngles(targetPolarization);

    //define the Lepton propagators
    double Delta2Q2 = m_t / m_Q2;
    double yeps = m_yBMJ[0] * (1. + m_epsilonBMJ[1]);
    double J = (1. - m_yBMJ[0] - m_yBMJ[0] * m_epsilonBMJ[1] / 2.) * (1. + Delta2Q2)
            - (1. - m_xB) * (2. - m_yBMJ[0]) * Delta2Q2;
    m_P1 = -(J + 2. * m_K[0] * cos(m_phi1BMK)) / yeps;
    m_P2 = 1. + Delta2Q2 - m_P1;

    // beam helicity
    m_lambda = beamHelicity;

    // Compute Fourier coeffs of BH
    computeFourierCoeffsBH();
    // Compute Fourier coeffs of VCS
    computeFourierCoeffsVCS();
    // Compute Fourier coeffs of Interference
    computeFourierCoeffsInterf();
}

void DVCSProcessBMJ12INT::isModuleWellConfigured() {

    //check mother class
    DVCSModule::isModuleWellConfigured();
}

void DVCSProcessBMJ12INT::defineAngles(const NumA::Vector3D &targetPolarization) {
    double Px = targetPolarization.getX();
    double Py = targetPolarization.getY();
    double Pz = targetPolarization.getZ();

    // m_phi is considered in the Trento convention here.
    double Sx = Px * cos(m_phi) - Py * sin(m_phi);
    double Sy = -(Px * sin(m_phi) + Py * cos(m_phi));
    double Sz = -Pz;

    m_Lambda = sqrt(Sx * Sx + Sy * Sy + Sz * Sz);

    if (m_Lambda != 0.) {
        double costheta = Sz / m_Lambda;
        m_theta = acos(costheta);

        double sintheta = sin(m_theta);
        if (sintheta != 0.) {
            double sinPhi = Sy / sintheta / m_Lambda;
            double cosPhi = Sx / sintheta / m_Lambda;
            if (cosPhi == 0.) {
                m_PhiBMK = ((sinPhi > 0.) - (sinPhi < 0.)) * Constant::PI / 2;
            } else {
                m_PhiBMK = atan(Sy / Sx);
                if (cosPhi <= 0.) {
                    if (sinPhi < 0.) {
                        m_PhiBMK = m_PhiBMK - Constant::PI;
                    } else {
                        m_PhiBMK = m_PhiBMK + Constant::PI;
                    }
                }
            }
        }
    }

    m_phi1BMK = Constant::PI - m_phi;
    m_phi2BMK = m_PhiBMK - m_phi1BMK;
}

void DVCSProcessBMJ12INT::computeFormFactors() {
    //TODO Get the value of F1(t) and F2(t) from another module
    m_F1 =
            (4. * m_M[1] - 2.79285 * m_t)
                    / (pow(1. - 1.4084507042253522 * m_t, 2)
                            * (4. * m_M[1] - 1. * m_t));
    m_F2 = (7.1714 * m_M[1])
            / (pow(1 - 1.4084507042253522 * m_t, 2) * (4 * m_M[1] - m_t));
}

void DVCSProcessBMJ12INT::computeCFFs() {
    // Compute coefficients in CFFs F+b and F0+
    for (int j = 0; j < 2; j++) {
        m_cF[j][0] = (1. + (1 - 2 * j) * m_epsroot[0]) / (2. * m_epsroot[0])
                + (1 - m_xB) * m_xB2 * (4 * m_M[1] - m_t) * (1 + m_t / m_Q2)
                        / (m_Q2 * m_epsroot[0] * m_xBtQ2[1]);
        m_cF[j][1] = (1. - (1 - 2 * j) * m_epsroot[0]) / (2. * m_epsroot[0])
                * 2. * m_Kt[1] / (m_M[1] * m_xBtQ2[1]);
        m_cF[j][2] = 4 * m_xB2 * m_Kt[1] / (m_Q2 * m_epsroot[0] * m_xBtQ2[2]);
    }
    double A = -sqrt(2.) * m_Kt[0] / (m_epsroot[0] * m_Q[0] * m_xBtQ2[0]);
    m_cF[2][0] = A * m_xB
            * (1 + 2. * m_xB * (4 * m_M[1] - m_t) / (m_Q2 * m_xBtQ2[0]));
    m_cF[2][1] = A * (4. * m_xB2 * m_M[1] - (2. * m_xB + m_epsilonBMJ[1]) * m_t)
            / (2. * m_M[1] * m_xBtQ2[0]);
    m_cF[2][2] = A * 2. * m_xB / m_xBtQ2[0]
            * (1 + 2. * m_xB2 * (4 * m_M[1] - m_t) / (m_Q2 * m_xBtQ2[0]));

    // Gets CFFs H, Ht, etc.
    std::vector<std::complex<double> > CFF_F(4, 0.);
    std::vector<std::complex<double> > CFF_FT(4, 0.);
    std::vector<std::complex<double> > CFF_FLT(4, 0.);

    CFF_F[0] = getConvolCoeffFunctionValue(GPDType::H);
    CFF_FT[0] = getConvolCoeffFunctionValue(GPDType::HTrans);
    CFF_FLT[0] = getConvolCoeffFunctionValue(GPDType::H3p)
            - getConvolCoeffFunctionValue(GPDType::H3m);

    CFF_F[1] = getConvolCoeffFunctionValue(GPDType::E);
    CFF_FT[1] = getConvolCoeffFunctionValue(GPDType::ETrans);
    CFF_FLT[1] = getConvolCoeffFunctionValue(GPDType::E3p)
            - getConvolCoeffFunctionValue(GPDType::E3m);

    CFF_F[2] = getConvolCoeffFunctionValue(GPDType::Ht);
    CFF_FT[2] = getConvolCoeffFunctionValue(GPDType::HtTrans);
    CFF_FLT[2] = getConvolCoeffFunctionValue(GPDType::Ht3p)
            - getConvolCoeffFunctionValue(GPDType::Ht3m);

    CFF_F[3] = getConvolCoeffFunctionValue(GPDType::Et);
    CFF_FT[3] = getConvolCoeffFunctionValue(GPDType::EtTrans);
    CFF_FLT[3] = getConvolCoeffFunctionValue(GPDType::Et3p)
            - getConvolCoeffFunctionValue(GPDType::Et3m);

    // Compute CFFs F+b and F0+
    for (unsigned int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < 3; j++) {
            m_CFF[i][j] = m_cF[j][0] * CFF_F[i] + m_cF[j][1] * CFF_FT[i]
                    + m_cF[j][2] * CFF_FLT[i];
        }
    }
}

void DVCSProcessBMJ12INT::computeFourierCoeffsBH() {
    //TODO Get the value of F1(t) and F2(t) from another module
    double F1 = m_F1; // Dirac form factor
    double F2 = m_F2; // Pauli form factor
//    F1 =
//            (4. * m_M[1] - 2.79285 * m_t)
//                    / (pow(1. - 1.4084507042253522 * m_t, 2)
//                            * (4. * m_M[1] - 1. * m_t));
//    F2 = (7.1714 * m_M[1])
//            / (pow(1 - 1.4084507042253522 * m_t, 2) * (4 * m_M[1] - m_t));

    //F1 = 0.7341366837442705; // Test avec Maxime
    //F2 = 1.204141183854909;

    double F12 = pow(F1, 2);
    double F22 = pow(F2, 2);
    double F1PlusF2 = F1 + F2;
    double F1PlusF22 = pow(F1PlusF2, 2);
    double Delta2M2 = m_t / (4 * m_M[1]);
    double Delta2Q2 = m_t / m_Q2;
    double MQ = m_M[0] / m_Q[0];
    double F1PlusDeltaF2 = F1 + Delta2M2 * F2;
    double F1MinusDeltaF2 = F12 - Delta2M2 * F22;

    // Unpolarized Fourier coefficients
    m_cBH[0][0] = 8. * m_K[1]
            * ((2. + 3. * m_epsilonBMJ[1]) * F1MinusDeltaF2 / Delta2Q2
                    + 2. * m_xB2 * F1PlusF22)
            + pow((2. - m_yBMJ[0]), 2)
                    * ((2. + m_epsilonBMJ[1])
                            * ((m_xB2 / Delta2M2) * pow((1. + Delta2Q2), 2)
                                    + 4. * (1. - m_xB) * (1. + m_xB * Delta2Q2))
                            * F1MinusDeltaF2
                            + (4. * m_xB2)
                                    * (m_xB
                                            + (1. - m_xB + m_epsilonBMJ[1] / 2.)
                                                    * pow((1. - Delta2Q2), 2)
                                            - m_xB * (1. - 2. * m_xB)
                                                    * m_Delta2[1] / m_Q[3])
                                    * F1PlusF22)
            + 8. * (1. + m_epsilonBMJ[1])
                    * (1. - m_yBMJ[0] - m_epsilonBMJ[1] * m_yBMJ[1] / 4.)
                    * (2. * m_epsilonBMJ[1] * (1. - Delta2M2) * F1MinusDeltaF2
                            - m_xB2 * pow((1. - Delta2Q2), 2) * F1PlusF22);
    m_cBH[0][1] = 8. * m_K[0] * (2. - m_yBMJ[0])
            * ((m_xB2 / Delta2M2 - 2. * m_xB - m_epsilonBMJ[1]) * F1MinusDeltaF2
                    + 2. * m_xB2 * (1. - (1. - 2. * m_xB) * Delta2Q2)
                            * F1PlusF22);
    m_cBH[0][2] = 8. * m_xB2 * m_K[1]
            * (F1MinusDeltaF2 / Delta2M2 + 2. * F1PlusF22);

    // LP Fourier coefficients
    double C1 = 8. * m_lambda * m_xB * m_yBMJ[0] * m_epsroot[0] * F1PlusF2
            / (1. - Delta2M2);
    m_cBH[1][0] = C1 * (2. - m_yBMJ[0])
            * (0.5 * (m_xB / 2. * (1. - Delta2Q2) - Delta2M2)
                    * (2. - m_xB - 2. * pow((1 - m_xB), 2) * Delta2Q2
                            + m_epsilonBMJ[1] * (1. - Delta2Q2)
                            - m_xB * (1. - 2. * m_xB) * m_Delta2[1] / m_Q[3])
                    * F1PlusF2
                    + (1. - (1. - m_xB) * Delta2Q2)
                            * (m_xB2 / (4. * Delta2M2) * pow(1. + Delta2Q2, 2)
                                    + (1. - m_xB) * (1. + m_xB * Delta2Q2))
                            * F1PlusDeltaF2);
    m_cBH[1][1] = -C1 * m_K[0]
            * ((2. * Delta2M2 - m_xB * (1. - Delta2Q2))
                    * (1. - m_xB + m_xB * Delta2Q2) * F1PlusF2
                    + (1. + m_xB - (3. - 2. * m_xB) * (1. + m_xB * Delta2Q2)
                            - m_xB2 / Delta2M2 * (1. + m_Delta2[1] / m_Q[3]))
                            * F1PlusDeltaF2);

    // TP Fourier coefficients
    double C2 = sqrt(1. - m_yBMJ[0] - m_epsilonBMJ[1] * m_yBMJ[1] / 4.);
    m_cBH[2][0] = -8. * m_lambda * cos(m_phi2BMK) * (2. - m_yBMJ[0]) * m_yBMJ[0] / MQ
            * m_epsroot[0] * m_K[0] / C2 * F1PlusF2
            * (m_xB * m_xB2 * pow(MQ, 2) * (1. - Delta2Q2) * F1PlusF2
                    + (1. - (1. - m_xB) * Delta2Q2)
                            * (m_xB2 / (4. * Delta2M2) * (1. - Delta2Q2) * F1
                                    + m_xB / 2. * F2));
    m_cBH[2][1] = -16 * m_lambda * cos(m_phi2BMK) * m_xB * m_yBMJ[0] * C2 * MQ
            * m_epsroot[0] * F1PlusF2
            * (2. * m_K[1] / (Delta2Q2 * pow(C2, 2))
                    * (m_xB * (1. - Delta2Q2) * F1 + Delta2M2 * F2)
                    + (1. + m_epsilonBMJ[1]) * m_xB * (1. - Delta2Q2)
                            * F1PlusDeltaF2);
    m_s1BHTP = 16. * m_lambda * sin(m_phi2BMK) * m_yBMJ[0] * m_xB2 * C2 * MQ
            * m_epsroot[2] * (1. - Delta2Q2) * F1PlusF2 * F1PlusDeltaF2;
}

void DVCSProcessBMJ12INT::computeFourierCoeffsVCS() {
    double C1, C2;
    C2 = 1. - m_yBMJ[0] - m_epsilonBMJ[1] * m_yBMJ[1] / 4.;
    C1 = sqrt(C2);

    // Unpolarized Fourier coeffs
    m_cVCS[0][0] = (2 * (2 - 2 * m_yBMJ[0] + m_yBMJ[1] * (1. + m_epsilonBMJ[1] / 2.))
            * C_VCS(0, 1, 1, 1, 1, -1, 1, -1, 1).real()
            + 8 * C2 * C_VCS(0, 0, 1, 0, 1).real()) / m_epsroot[1];
    m_cVCS[0][1] = 4 * sqrt(2.) * C1 / m_epsroot[1] * (2 - m_yBMJ[0])
            * C_VCS(0, 0, 1, 1, 1, -1, 1).real();
    m_sVCS[0][1] = 4 * sqrt(2.) * C1 / m_epsroot[1]
            * (-m_lambda * m_yBMJ[0] * m_epsroot[0])
            * C_VCS(0, 0, 1, 1, 1, -1, 1).imag();
    m_cVCS[0][2] = 8 * C2 / m_epsroot[1] * C_VCS(0, -1, 1, 1, 1).real();

    // Longitudinally polarized Fourier coeffs
    m_cVCS[1][0] = 2 * m_lambda * m_yBMJ[0] * (2 - m_yBMJ[0]) / m_epsroot[1]
            * C_VCS(1, 1, 1, 1, 1, -1, 1, -1, 1).real();
    m_cVCS[1][1] = -4 * sqrt(2.) * C1 / m_epsroot[2]
            * (-m_lambda * m_yBMJ[0] * m_epsroot[0])
            * C_VCS(1, 0, 1, 1, 1, -1, 1).real();
    m_sVCS[1][1] = -4 * sqrt(2.) * C1 / m_epsroot[2] * (2 - m_yBMJ[0])
            * C_VCS(1, 0, 1, 1, 1, -1, 1).imag();
    m_sVCS[1][2] = -8 * C2 / m_epsroot[2] * C_VCS(1, -1, 1, 1, 1).imag();

    // Transversally polarized Fourier coeffs
    m_cVCS[2][0] =
            m_Kt[0] / m_M[0] / m_epsroot[1]
                    * (-4 * C2 * sin(m_phi2BMK) * C_VCS(3, 0, 1, 0, 1).imag()
                            + (2 - m_yBMJ[0])
                                    * (m_lambda * cos(m_phi2BMK) * m_yBMJ[0]
                                            * C_VCS(2, 1, 1, 1, 1, -1, 1, -1, 1).real()
                                            - sin(m_phi2BMK)
                                                    * (2 - 2 * m_yBMJ[0]
                                                            + m_yBMJ[1]
                                                                    * (1.
                                                                            + m_epsilonBMJ[1]
                                                                                    / 2.))
                                                    / (2. - m_yBMJ[0])
                                                    * C_VCS(3, 1, 1, 1, 1, -1,
                                                            1, -1, 1).imag()));
    m_cVCS[2][1] = -2 * sqrt(2.) * C1 / m_epsroot[1] * m_Kt[0] / m_M[0]
            * (cos(m_phi2BMK) * (-m_lambda * m_yBMJ[0])
                    * C_VCS(2, 0, 1, 1, 1, -1, 1).real()
                    + sin(m_phi2BMK) * (2 - m_yBMJ[0])
                            * C_VCS(3, 0, 1, 1, 1, -1, 1).imag());
    m_sVCS[2][1] = -2 * sqrt(2.) * C1 / m_epsroot[1] * m_Kt[0] / m_M[0]
            * (cos(m_phi2BMK) * (2 - m_yBMJ[0]) / m_epsroot[0]
                    * C_VCS(2, 0, 1, 1, 1, -1, 1).imag()
                    + sin(m_phi2BMK) * (m_lambda * m_yBMJ[0] * m_epsroot[0])
                            * C_VCS(3, 0, 1, 1, 1, -1, 1).real());
    m_cVCS[2][2] = -4 * C2 / m_epsroot[2] * m_Kt[0] / m_M[0] * m_epsroot[0]
            * sin(m_phi2BMK) * C_VCS(3, -1, 1, 1, 1).imag();
    m_sVCS[2][2] = -4 * C2 / m_epsroot[2] * m_Kt[0] / m_M[0] * cos(m_phi2BMK)
            * C_VCS(2, -1, 1, 1, 1).imag();
}

void DVCSProcessBMJ12INT::computeAngularCoeffsInterf() {
    double tQ2 = m_t / m_Q2;
    double Kt2Q2 = m_Kt[1] / m_Q2;
    double tp = m_t - m_tmin;
    double tpQ2 = tp / m_Q2;
    double C2 = 1. - m_yBMJ[0] - m_epsilonBMJ[1] * m_yBMJ[1] / 4.;
    double C1 = sqrt(C2);
    double B2 = 2. - 2 * m_yBMJ[0] + m_yBMJ[1] + m_epsilonBMJ[1] * m_yBMJ[1] / 2.;

    /* Unpolarized and TP- target
     * Angulars coeffs C and S
     */

    // C++ coefficients
    m_C[0][0][0] =
            -4 * (2. - m_yBMJ[0]) * (1. + m_epsroot[0]) / m_epsroot[3]
                    * (Kt2Q2 * (2. - m_yBMJ[0]) * (2. - m_yBMJ[0]) / m_epsroot[0]
                            + tQ2 * C2 * (2 - m_xB)
                                    * (1.
                                            + (2 * m_xB
                                                    * (2 - m_xB
                                                            + (m_epsroot[0] - 1.)
                                                                    / 2.
                                                            + m_epsilonBMJ[1]
                                                                    / (2 * m_xB))
                                                    * tQ2 + m_epsilonBMJ[1])
                                                    / ((2. - m_xB)
                                                            * (1 + m_epsroot[0]))));
    m_C[0][1][0] = 8 * (2. - m_yBMJ[0]) * m_xB * tQ2 / m_epsroot[3]
            * (Kt2Q2 * (2. - m_yBMJ[0]) * (2. - m_yBMJ[0]) / m_epsroot[0]
                    + C2 * (1 + m_epsroot[0]) / 2. * (1. + tQ2)
                            * (1.
                                    + (m_epsroot[0] - 1. + 2 * m_xB) * tQ2
                                            / (1 + m_epsroot[0])));
    m_C[0][2][0] =
            8 * (2. - m_yBMJ[0]) * tQ2 / m_epsroot[3]
                    * (Kt2Q2 * (2. - m_yBMJ[0]) * (2. - m_yBMJ[0]) / m_epsroot[0]
                            * (1 + m_epsroot[0] - 2 * m_xB) / 2.
                            - C2
                                    * (2 * Kt2Q2
                                            - (1 + m_epsroot[0]) / 2.
                                                    * (1 + m_epsroot[0] - m_xB
                                                            + (m_epsroot[0] - 1.
                                                                    + m_xB
                                                                            * (3.
                                                                                    + m_epsroot[0]
                                                                                    - 2
                                                                                            * m_xB)
                                                                            / (1
                                                                                    + m_epsroot[0]))
                                                                    * tQ2)));
    m_C[0][0][1] = -16 * m_K[0] * C2 / m_epsroot[4]
            * ((1. + (1. - m_xB) * (m_epsroot[0] - 1.) / (2 * m_xB)
                    + m_epsilonBMJ[1] / (4 * m_xB)) * m_xB * tQ2
                    - 3 * m_epsilonBMJ[1] / 4.)
            - 4 * m_K[0] * B2 * (1. + m_epsroot[0] - m_epsilonBMJ[1])
                    / m_epsroot[4]
                    * (1. - (1. - 3 * m_xB) * tQ2
                            + (1. - m_epsroot[0] + 3 * m_epsilonBMJ[1]) * m_xB
                                    * tQ2 / (1. + m_epsroot[0] - m_epsilonBMJ[1]));
    m_C[0][1][1] = 16 * m_K[0] * m_xB * tQ2 / m_epsroot[4]
            * ((2. - m_yBMJ[0]) * (2. - m_yBMJ[0]) * (1. - (1. - 2 * m_xB) * tQ2)
                    + C2 * (1 + m_epsroot[0] - 2 * m_xB) * tpQ2 / 2.);
    m_C[0][2][1] = -16 * m_K[0] * tQ2 / m_epsroot[3]
            * (C2
                    * (1. - (1. - 2 * m_xB) * tQ2
                            + (4 * m_xB * (1. - m_xB) + m_epsilonBMJ[1]) * tpQ2
                                    / (4 * m_epsroot[0]))
                    - (2. - m_yBMJ[0]) * (2. - m_yBMJ[0])
                            * (1. - m_xB / 2.
                                    + (1 + m_epsroot[0] - 2 * m_xB) / 4.
                                            * (1. - tQ2)
                                    + (4 * m_xB * (1. - m_xB) + m_epsilonBMJ[1])
                                            * tpQ2 / (2 * m_epsroot[0])));
    m_C[0][0][2] = 8 * (2. - m_yBMJ[0]) * C2 / m_epsroot[3]
            * (2 * m_epsilonBMJ[1] * Kt2Q2 / (m_epsroot[0] + m_epsroot[1])
                    + m_xB * tQ2 * tpQ2
                            * (1. - m_xB - (m_epsroot[0] - 1.) / 2.
                                    + m_epsilonBMJ[1] / (2 * m_xB)));
    m_C[0][1][2] = 8 * (2. - m_yBMJ[0]) * C2 * m_xB * tQ2 / m_epsroot[3]
            * (4 * Kt2Q2 / m_epsroot[0]
                    + (1. + m_epsroot[0] - 2 * m_xB) / 2. * (1 + tQ2) * tpQ2);
    m_C[0][2][2] = 4 * (2. - m_yBMJ[0]) * C2 * tQ2 / m_epsroot[3]
            * (4 * (1. - 2 * m_xB) * Kt2Q2 / m_epsroot[0]
                    - (3. - m_epsroot[0] - 2 * m_xB + m_epsilonBMJ[1] / m_xB)
                            * m_xB * tpQ2);
    m_C[0][0][3] = -8 * m_K[0] * C2 * (m_epsroot[0] - 1.) / m_epsroot[4]
            * ((1. - m_xB) * tQ2 + (m_epsroot[0] - 1.) / 2. * (1 + tQ2));
    m_C[0][1][3] = -8 * m_K[0] * C2 * m_xB * tQ2 / m_epsroot[4]
            * (m_epsroot[0] - 1. + (1. + m_epsroot[0] - 2 * m_xB) * tQ2);
    m_C[0][2][3] = 16 * m_K[0] * C2 * tQ2 * tpQ2 / m_epsroot[4]
            * (m_xB * (1. - m_xB) + m_epsilonBMJ[1] / 4.);

    // S++ coefficients
    m_S[0][0][1] =
            8 * m_K[0] * (2. - m_yBMJ[0]) * m_yBMJ[0] / m_epsroot[1]
                    * (1.
                            + (1. - m_xB + (m_epsroot[0] - 1.) / 2.)
                                    / m_epsroot[1] * tpQ2);
    m_S[0][1][1] = -8 * m_K[0] * (2. - m_yBMJ[0]) * m_yBMJ[0] * m_xB * tQ2
            / m_epsroot[3]
            * (m_epsroot[0] - 1. + (1. + m_epsroot[0] - 2 * m_xB) * tQ2);
    m_S[0][2][1] = 8 * m_K[0] * (2. - m_yBMJ[0]) * m_yBMJ[0] * tQ2 / m_epsroot[1]
            * (1.
                    - (1. - 2 * m_xB) * (1. + m_epsroot[0] - 2 * m_xB)
                            / (2 * m_epsroot[1]) * tpQ2);
    m_S[0][0][2] = -4 * C2 * m_yBMJ[0] / m_epsroot[2]
            * (1. + m_epsroot[0] - 2 * m_xB) * tpQ2
            * ((m_epsilonBMJ[1] - m_xB * (m_epsroot[0] - 1.))
                    / (1. + m_epsroot[0] - 2 * m_xB)
                    - (2 * m_xB + m_epsilonBMJ[1]) * tpQ2 / (2 * m_epsroot[0]));
    m_S[0][1][2] = -4 * C2 * m_yBMJ[0] * m_xB * tQ2 / m_epsroot[3]
            * (1. - (1. - 2 * m_xB) * tQ2)
            * (m_epsroot[0] - 1. + (1. + m_epsroot[0] - 2 * m_xB) * tQ2);
    m_S[0][2][2] = -8 * C2 * m_yBMJ[0] * tQ2 * tpQ2 / m_epsroot[3]
            * (1. - m_xB / 2. + 3 * m_epsilonBMJ[1] / 4.)
            * (1. + m_epsroot[0] - 2 * m_xB)
            * (1.
                    + (4 * (1 - m_xB) * m_xB + m_epsilonBMJ[1])
                            / (4. - 2 * m_xB + 3 * m_epsilonBMJ[1]) * tQ2);

    // C0+
    m_C[2][0][0] = 12 * sqrt(2.) * m_K[0] * (2. - m_yBMJ[0]) * C1 / m_epsroot[4]
            * (m_epsilonBMJ[1] + (2. - 6 * m_xB - m_epsilonBMJ[1]) / 3. * tQ2);
    m_C[2][1][0] = 24 * sqrt(2.) * m_K[0] * (2. - m_yBMJ[0]) * C1 * m_xB * tQ2
            / m_epsroot[4] * (1. - (1. - 2 * m_xB) * tQ2);
    m_C[2][2][0] = 4 * sqrt(2.) * m_K[0] * (2. - m_yBMJ[0]) * C1 * tQ2
            / m_epsroot[4] * (8. - 6 * m_xB + 5 * m_epsilonBMJ[1])
            * (1.
                    - tQ2 * (2. - 12 * m_xB * (1. - m_xB) - m_epsilonBMJ[1])
                            / (8. - 6 * m_xB + 5 * m_epsilonBMJ[1]));
    m_C[2][0][1] = 8 * sqrt(2.) * C1 / m_epsroot[3]
            * ((2. - m_yBMJ[0]) * (2. - m_yBMJ[0]) * tpQ2
                    * (1. - m_xB
                            + ((1. - m_xB) * m_xB + m_epsilonBMJ[1] / 4.)
                                    / m_epsroot[0] * tpQ2)
                    + C2 / m_epsroot[0] * (1. - (1. - 2 * m_xB) * tQ2)
                            * (m_epsilonBMJ[1]
                                    - 2 * (1. + m_epsilonBMJ[1] / (2 * m_xB))
                                            * m_xB * tQ2));
    m_C[2][1][1] = 16 * sqrt(2.) * C1 * m_xB * tQ2 / m_epsroot[4]
            * (Kt2Q2 * (2. - m_yBMJ[0]) * (2. - m_yBMJ[0])
                    + pow(1. - (1. - 2 * m_xB) * tQ2, 2) * C2);
    m_C[2][2][1] = 8 * sqrt(2.) * C1 * tQ2 / m_epsroot[4]
            * (Kt2Q2 * (1. - 2 * m_xB) * (2. - m_yBMJ[0]) * (2. - m_yBMJ[0])
                    + (1. - (1. - 2 * m_xB) * tQ2) * C2
                            * (4. - 2 * m_xB + 3 * m_epsilonBMJ[1]
                                    + tQ2
                                            * (4 * m_xB * (1. - m_xB)
                                                    + m_epsilonBMJ[1])));
    m_C[2][0][2] = -8 * sqrt(2.) * m_K[0] * (2. - m_yBMJ[0]) * C1 / m_epsroot[4]
            * (1. + m_epsilonBMJ[1] / 2.)
            * (1.
                    + (1. + m_epsilonBMJ[1] / (2 * m_xB))
                            / (1. + m_epsilonBMJ[1] / 2.) * m_xB * tQ2);
    m_C[2][1][2] = 8 * sqrt(2.) * m_K[0] * (2. - m_yBMJ[0]) * C1 * m_xB * tQ2
            / m_epsroot[4] * (1. - (1. - 2 * m_xB) * tQ2);
    m_C[2][2][2] = 8 * sqrt(2.) * m_K[0] * (2. - m_yBMJ[0]) * C1 * tQ2
            / m_epsroot[3]
            * (1. - m_xB
                    + tpQ2 / 2. * (4 * m_xB * (1. - m_xB) + m_epsilonBMJ[1])
                            / m_epsroot[0]);

    // S0+
    m_S[2][0][1] = 8 * sqrt(2.) * (2. - m_yBMJ[0]) * m_yBMJ[0] * C1 * Kt2Q2
            / m_epsroot[3];
    m_S[2][1][1] = 4 * sqrt(2.) * (2. - m_yBMJ[0]) * m_yBMJ[0] * C1 * m_xB * tQ2
            / m_epsroot[3]
            * (4 * (1. - m_xB) * tQ2 * (1 + m_xB * tQ2)
                    + m_epsilonBMJ[1] * (1 + tQ2) * (1 + tQ2));
    m_S[2][2][1] = -8 * sqrt(2.) * (2. - m_yBMJ[0]) * m_yBMJ[0] * (1. - 2 * m_xB) * C1
            * tQ2 * Kt2Q2 / m_epsroot[3];
    m_S[2][0][2] = 8 * sqrt(2.) * m_K[0] * m_yBMJ[0] * C1
            * (1. + m_epsilonBMJ[1] / 2.) / m_epsroot[3]
            * (1.
                    + (1. + m_epsilonBMJ[1] / (2 * m_xB))
                            / (1. + m_epsilonBMJ[1] / 2.) * m_xB * tQ2);
    m_S[2][1][2] = -8 * sqrt(2.) * m_K[0] * m_yBMJ[0] * C1 * m_xB * tQ2
            / m_epsroot[3] * (1. - (1. - 2 * m_xB) * tQ2);
    m_S[2][2][2] = -2 * sqrt(2.) * m_K[0] * m_yBMJ[0] * C1 * tQ2 / m_epsroot[3]
            * (4. - 4 * m_xB + 2 * m_epsilonBMJ[1]
                    + 2 * tQ2 * (4 * m_xB * (1. - m_xB) + m_epsilonBMJ[1]));

    // C-+
    m_C[1][0][0] = 8 * (2. - m_yBMJ[0]) / m_epsroot[2]
            * ((2. - m_yBMJ[0]) * (2. - m_yBMJ[0]) * (m_epsroot[0] - 1.)
                    / (2 * m_epsroot[1]) * Kt2Q2
                    + C2 / m_epsroot[0]
                            * (1. - m_xB - (m_epsroot[0] - 1.) / 2.
                                    + m_epsilonBMJ[1] / (2 * m_xB)) * m_xB * tQ2
                            * tpQ2);
    m_C[1][1][0] = 4 * (2. - m_yBMJ[0]) * m_xB * tQ2 / m_epsroot[4]
            * (2 * Kt2Q2 * B2
                    - (1. - (1. - 2 * m_xB) * tQ2) * C2
                            * (m_epsroot[0] - 1.
                                    + (m_epsroot[0] + 1. - 2 * m_xB) * tQ2));
    m_C[1][2][0] =
            4 * (2. - m_yBMJ[0]) * tQ2 / m_epsroot[3]
                    * (tpQ2 * C2
                            * (2 * m_xB2 - m_epsilonBMJ[1] - 3 * m_xB
                                    + m_xB * m_epsroot[0])
                            + Kt2Q2 / m_epsroot[0]
                                    * (4.
                                            - 2 * m_xB * (2. - m_yBMJ[0])
                                                    * (2. - m_yBMJ[0]) - 4 * m_yBMJ[0]
                                            + m_yBMJ[1] - m_yBMJ[1] * m_epsroot[2]));
    m_C[1][0][1] =
            8 * m_K[0] / m_epsroot[2]
                    * ((2. - m_yBMJ[0]) * (2. - m_yBMJ[0]) * (2. - m_epsroot[0])
                            / m_epsroot[1]
                            * ((m_epsroot[0] - 1. + m_epsilonBMJ[1])
                                    / (2 * (2. - m_epsroot[0])) * (1. - tQ2)
                                    - m_xB * tQ2)
                            + 2 * C2 / m_epsroot[0]
                                    * ((1. - m_epsroot[0] + m_epsilonBMJ[1] / 2.)
                                            / (2 * m_epsroot[0])
                                            + tQ2
                                                    * (1. - 3 * m_xB / 2.
                                                            + (m_xB
                                                                    + m_epsilonBMJ[1]
                                                                            / 2.)
                                                                    / (2
                                                                            * m_epsroot[0]))));
    m_C[1][1][1] =
            8 * m_K[0] * m_xB * tQ2 / m_epsroot[4]
                    * (2 * (1. - (1. - 2 * m_xB) * tQ2) * B2
                            + C2
                                    * (3. - m_epsroot[0]
                                            - (3 * (1. - 2 * m_xB)
                                                    + m_epsroot[0]) * tQ2));
    m_C[1][2][1] =
            4 * m_K[0] * tQ2 / m_epsroot[4]
                    * (B2
                            * (5. - 4 * m_xB + 3 * m_epsilonBMJ[1] - m_epsroot[0]
                                    - tQ2
                                            * (1. - m_epsilonBMJ[1] - m_epsroot[0]
                                                    - 2 * m_xB
                                                            * (4. - 4 * m_xB
                                                                    - m_epsroot[0])))
                            + C2
                                    * (8. + 5 * m_epsilonBMJ[1] - 6 * m_xB
                                            + 2 * m_xB * m_epsroot[0]
                                            - tQ2
                                                    * (2. - m_epsilonBMJ[1]
                                                            + 2 * m_epsroot[0]
                                                            - 4 * m_xB
                                                                    * (3.
                                                                            - 3
                                                                                    * m_xB
                                                                            + m_epsroot[0]))));
    m_C[1][0][2] =
            4 * (2. - m_yBMJ[0]) * C2 * (1. + m_epsroot[0]) / m_epsroot[4]
                    * ((2. - 3 * m_xB) * tQ2
                            + (1. - 2 * m_xB
                                    + 2 * (1. - m_xB) / (1. + m_epsroot[0]))
                                    * m_xB * tQ2 * tQ2
                            + (1.
                                    + (m_epsroot[0] + m_xB + (1. - m_xB) * tQ2)
                                            / (1. + m_epsroot[0]) * tQ2)
                                    * m_epsilonBMJ[1]);
    m_C[1][1][2] = 4 * (2. - m_yBMJ[0]) * C2 * m_xB * tQ2 / m_epsroot[4]
            * (4 * Kt2Q2 + 1. + m_epsroot[0]
                    + tQ2
                            * ((1. - 2 * m_xB) * (1. - 2 * m_xB - m_epsroot[0])
                                    * tQ2 - 2. + 4 * m_xB
                                    + 2 * m_xB * m_epsroot[0]));
    m_C[1][2][2] =
            16 * (2. - m_yBMJ[0]) * C2 * tQ2 / m_epsroot[2]
                    * (Kt2Q2 * (1. - 2 * m_xB) / m_epsroot[1]
                            - (1. - m_xB)
                                    / (4 * m_xB * (1. - m_xB) + m_epsilonBMJ[1])
                                    * (2 * m_xB2 - m_epsilonBMJ[1] - 3 * m_xB
                                            - m_xB * m_epsroot[0])
                            - tpQ2
                                    * (2 * m_xB2 - m_epsilonBMJ[1] - 3 * m_xB
                                            - m_xB * m_epsroot[0])
                                    / (4 * m_epsroot[0]));
    m_C[1][0][3] = -8 * m_K[0] * C2 * (1. + m_epsroot[0] + m_epsilonBMJ[1] / 2.)
            / m_epsroot[4]
            * (1.
                    + (1. + m_epsroot[0] + m_epsilonBMJ[1] / (2 * m_xB))
                            / (1. + m_epsroot[0] + m_epsilonBMJ[1] / 2) * m_xB
                            * tQ2);
    m_C[1][1][3] = 8 * m_K[0] * C2 * m_xB * tQ2 / m_epsroot[4]
            * (1. + m_epsroot[0])
            * (1. - tQ2 * (1. - 2 * m_xB - m_epsroot[0]) / (1. + m_epsroot[0]));
    m_C[1][2][3] = 16 * m_K[0] * C2 * tQ2 / m_epsroot[3]
            * (1. - m_xB
                    + tpQ2 * (4 * m_xB * (1. - m_xB) + m_epsilonBMJ[1])
                            / (4 * m_epsroot[0]));

    // S-+
    m_S[1][0][1] = 4 * m_K[0] * (2. - m_yBMJ[0]) * m_yBMJ[0] / m_epsroot[3]
            * (1. - m_epsroot[0] + 2 * m_epsilonBMJ[1]
                    - 2 * (1. + (m_epsroot[0] - 1.) / (2 * m_xB)) * m_xB * tQ2);
    m_S[1][1][1] = 8 * m_K[0] * (2. - m_yBMJ[0]) * m_yBMJ[0] * m_xB * tQ2
            / m_epsroot[3] * (1. + m_epsroot[0])
            * (1. - tQ2 * (1. - 2 * m_xB - m_epsroot[0]) / (1. + m_epsroot[0]));
    m_S[1][2][1] = 4 * m_K[0] * (2. - m_yBMJ[0]) * m_yBMJ[0] * tQ2 / m_epsroot[3]
            * (3. + 2 * m_epsilonBMJ[1] + m_epsroot[0]
                    - 2 * m_xB * (1. + m_epsroot[0])
                    - tQ2 * (1. - 2 * m_xB) * (1. - 2 * m_xB - m_epsroot[0]));
    m_S[1][0][2] = 2 * m_yBMJ[0] * C2 * (1. + m_epsroot[0]) / m_epsroot[3]
            * (m_epsilonBMJ[1] - 2 * (1. + m_epsilonBMJ[1] / (2 * m_xB)) * m_xB * tQ2)
            * (1. + (m_epsroot[0] - 1. + 2 * m_xB) / (1. + m_epsroot[0]) * tQ2);
    m_S[1][1][2] = 4 * m_yBMJ[0] * C2 * m_xB * tQ2 / m_epsroot[3]
            * (1. + m_epsroot[0]) * (1. - (1. - 2 * m_xB) * tQ2)
            * (1. - tQ2 * (1. - 2 * m_xB - m_epsroot[0]) / (1. + m_epsroot[0]));
    m_S[1][2][2] = 2 * m_yBMJ[0] * C2 * tQ2 / m_epsroot[3]
            * (4. - 2 * m_xB + 3 * m_epsilonBMJ[1]
                    + tQ2 * (4 * m_xB * (1. - m_xB) + m_epsilonBMJ[1]))
            * (1. + m_epsroot[0] - tQ2 * (1. - 2 * m_xB - m_epsroot[0]));

    /* LP and TP+ target
     * Angulars coeffs dC and dS
     */

    // dC++ coefficients
    m_dC[0][0][0] = -4 * m_yBMJ[0] * (1. + m_epsroot[0]) / m_epsroot[4]
            * ((2. - m_yBMJ[0]) * (2. - m_yBMJ[0]) * Kt2Q2
                    + C2 * (m_xB * tQ2 - (1. - tQ2) * m_epsilonBMJ[1] / 2.)
                            * (1.
                                    + (m_epsroot[0] - 1. + 2 * m_xB)
                                            / (1. + m_epsroot[0]) * tQ2));
    m_dC[0][1][0] = 4 * m_yBMJ[0] * (1. + m_epsroot[0]) * tQ2 / m_epsroot[4]
            * ((2. - m_yBMJ[0]) * (2. - m_yBMJ[0]) * (1. + m_epsroot[0] - 2 * m_xB)
                    * Kt2Q2 / (1. + m_epsroot[0])
                    + C2 * (2. - m_xB + 3 * m_epsilonBMJ[1] / 2.)
                            * (1.
                                    + (4 * (1. - m_xB) * m_xB + m_epsilonBMJ[1])
                                            / (4. - 2 * m_xB + 3 * m_epsilonBMJ[1])
                                            * tQ2)
                            * (1.
                                    + (m_epsroot[0] - 1. + 2 * m_xB)
                                            / (1. + m_epsroot[0]) * tQ2));
    m_dC[0][2][0] = 4 * m_yBMJ[0] * m_xB * tQ2 / m_epsroot[4]
            * (2 * (2. - m_yBMJ[0]) * (2. - m_yBMJ[0]) * Kt2Q2
                    + C2 * (1. + m_epsroot[0]) * (1. - (1. - 2 * m_xB) * tQ2)
                            * (1.
                                    + (m_epsroot[0] - 1. + 2 * m_xB)
                                            / (1. + m_epsroot[0]) * tQ2));
    m_dC[0][0][1] =
            -4 * m_K[0] * m_yBMJ[0] * (2. - m_yBMJ[0])
                    * (1. + m_epsroot[0] - m_epsilonBMJ[1]) / m_epsroot[4]
                    * (1.
                            - (1.
                                    - 2 * m_xB * (2. + m_epsroot[0])
                                            / (1. + m_epsroot[0] - m_epsilonBMJ[1]))
                                    * tQ2);
    m_dC[0][1][1] = 8 * m_K[0] * m_yBMJ[0] * (2. - m_yBMJ[0])
            * (m_epsroot[0] + 2 * (1. - m_xB)) * tQ2 / m_epsroot[3]
            * (1.
                    - (1. + (1. - m_epsilonBMJ[1]) / m_epsroot[0]
                            - 2 * m_xB * (1. + 4 * (1. - m_xB) / m_epsroot[0]))
                            * tpQ2 / (2 * (m_epsroot[0] + 2 * (1. - m_xB))));
    m_dC[0][2][1] = 16 * m_K[0] * m_yBMJ[0] * (2. - m_yBMJ[0]) * m_xB * tQ2
            / m_epsroot[4] * (1. - (1. - 2 * m_xB) * tQ2);
    m_dC[0][0][2] = -4 * m_yBMJ[0] * C2 / m_epsroot[4]
            * (m_xB * tQ2 - (1. - tQ2) * m_epsilonBMJ[1] / 2.)
            * (1. - m_epsroot[0] - (1. + m_epsroot[0] - 2 * m_xB) * tQ2);
    m_dC[0][1][2] = -2 * m_yBMJ[0] * C2 / m_epsroot[4]
            * (4. - 2 * m_xB + 3 * m_epsilonBMJ[1]) * tQ2
            * (1.
                    + (4 * (1. - m_xB) * m_xB + m_epsilonBMJ[1])
                            / (4. - 2 * m_xB + 3 * m_epsilonBMJ[1]) * tQ2)
            * (m_epsroot[0] - 1. + (1. + m_epsroot[0] - 2 * m_xB) * tQ2);
    m_dC[0][2][2] = 4 * m_yBMJ[0] * C2 * m_xB * tQ2 / m_epsroot[4]
            * (1. - (1. - 2 * m_xB) * tQ2)
            * (1. - m_epsroot[0] - (1. + m_epsroot[0] - 2 * m_xB) * tQ2);

    // dS++ coefficients
    m_dS[0][0][1] =
            4 * m_K[0] * B2 * (1. + m_epsroot[0]) / m_epsroot[5]
                    * (2 * m_epsroot[0] - 1.
                            + (1. + m_epsroot[0] - 2 * m_xB)
                                    / (1. + m_epsroot[0]) * tQ2)
                    + 8 * m_K[0] * C2 / m_epsroot[5]
                            * (3 * m_epsilonBMJ[1] / 2.
                                    + (1. - m_epsroot[0] - m_epsilonBMJ[1] / 2.
                                            - m_xB * (3. - m_epsroot[0])) * tQ2);
    m_dS[0][1][1] = 8 * m_K[0] * B2 * tQ2 / m_epsroot[3]
            * (1.
                    - (1. - 2 * m_xB) * (1. + m_epsroot[0] - 2 * m_xB) * tpQ2
                            / (2 * m_epsroot[1]))
            + 32 * m_K[0] * C2 / m_epsroot[5]
                    * (1. - (3. + m_epsroot[0]) * m_xB / 4.
                            + 5 * m_epsilonBMJ[1] / 8.) * tQ2
                    * (1.
                            - (1. - m_epsroot[0] - m_epsilonBMJ[1] / 2.
                                    - 2 * m_xB
                                            * (3 * (1. - m_xB) - m_epsroot[0]))
                                    * tQ2
                                    / (4. - m_xB * (m_epsroot[0] + 3.)
                                            + 5 * m_epsilonBMJ[1] / 2.));
    m_dS[0][2][1] = -8 * m_K[0] * B2 * m_xB * tQ2 / m_epsroot[5]
            * (m_epsroot[0] - 1. + (1. + m_epsroot[0] - 2 * m_xB) * tQ2)
            + 8 * m_K[0] * C2 * (3. + m_epsroot[0]) * m_xB * tQ2 / m_epsroot[5]
                    * (1.
                            - (3. - m_epsroot[0] - 6 * m_xB) * tQ2
                                    / (3. + m_epsroot[0]));
    m_dS[0][0][2] = -4 * (2. - m_yBMJ[0]) * C2 / m_epsroot[4]
            * (4 * Kt2Q2 / m_epsroot[0]
                    + (1. + m_epsroot[0] - 2 * m_xB)
                            * (1. + m_epsroot[0] + m_xB * tQ2) * tpQ2);
    m_dS[0][1][2] = 4 * (2. - m_yBMJ[0]) * C2 * tQ2 / m_epsroot[4]
            * (4 * (1. - 2 * m_xB) * Kt2Q2 / m_epsroot[0]
                    - (3. - m_epsroot[0] - 2 * m_xB + m_epsilonBMJ[1] / m_xB)
                            * m_xB * tpQ2);
    m_dS[0][2][2] = 4 * (2. - m_yBMJ[0]) * C2 * m_xB * tQ2 / m_epsroot[5]
            * (4 * Kt2Q2
                    - (1. + m_epsroot[0] - 2 * m_xB)
                            * (1. - (1. - 2 * m_xB) * tQ2) * tpQ2);
    m_dS[0][0][3] = -4 * m_K[0] * C2 / m_epsroot[5]
            * (1. + m_epsroot[0] - 2 * m_xB) / (1. + m_epsroot[0])
            * m_epsilonBMJ[1] * tpQ2;
    m_dS[0][1][3] = 4 * m_K[0] * C2 / m_epsroot[5]
            * (4 * (1. - m_xB) * m_xB + m_epsilonBMJ[1]) * tQ2 * tpQ2;
    m_dS[0][2][3] = -8 * m_K[0] * C2 / m_epsroot[5]
            * (1. + m_epsroot[0] - 2 * m_xB) * m_xB * tQ2 * tpQ2;

    // dC0+
    m_dC[2][0][0] = 8 * sqrt(2.) * m_K[0] * (1. - m_xB) * m_yBMJ[0] * C1 * tQ2
            / m_epsroot[3];
    m_dC[2][1][0] = 8 * sqrt(2.) * m_K[0] * m_yBMJ[0] * C1 * tQ2 / m_epsroot[3]
            * (m_xB - tQ2 * (1. - 2 * m_xB));
    m_dC[2][2][0] = -8 * sqrt(2.) * m_K[0] * m_yBMJ[0] * C1 * m_xB * tQ2
            / m_epsroot[3] * (1. + tQ2);
    m_dC[2][0][1] = -8 * sqrt(2.) * m_yBMJ[0] * (2. - m_yBMJ[0]) * C1 * Kt2Q2
            / m_epsroot[3];
    m_dC[2][1][1] = 8 * sqrt(2.) * m_yBMJ[0] * (2. - m_yBMJ[0]) * C1 * tQ2 * Kt2Q2
            / m_epsroot[3];
    m_dC[2][2][1] = 0.;
    m_dC[2][0][2] = -8 * sqrt(2.) * m_K[0] * m_yBMJ[0] * C1 * (1. + m_xB * tQ2)
            / m_epsroot[3];
    m_dC[2][1][2] = 8 * sqrt(2.) * m_K[0] * m_yBMJ[0] * (1. - m_xB) * C1 * tQ2
            / m_epsroot[3];
    m_dC[2][2][2] = 8 * sqrt(2.) * m_K[0] * m_yBMJ[0] * C1 * m_xB * tQ2
            / m_epsroot[3] * (1. + tQ2);

    // dS0+
    m_dS[2][0][1] = 8 * sqrt(2.) * C1 / m_epsroot[4]
            * (Kt2Q2 * (2. - m_yBMJ[0]) * (2. - m_yBMJ[0])
                    + (1. + tQ2) * C2
                            * (2 * m_xB * tQ2 - (1. - tQ2) * m_epsilonBMJ[1]));
    m_dS[2][1][1] = -8 * sqrt(2.) * C1 * tQ2 / m_epsroot[4]
            * (Kt2Q2 * (2. - m_yBMJ[0]) * (2. - m_yBMJ[0])
                    + (1. + tQ2) * C2
                            * (4. - 2 * m_xB + 3 * m_epsilonBMJ[1]
                                    + tQ2
                                            * (4 * m_xB * (1. - m_xB)
                                                    + m_epsilonBMJ[1])));
    m_dS[2][2][1] = -16 * sqrt(2.) * C1 * C2 * m_xB * tQ2 / m_epsroot[4]
            * (1. + tQ2) * (1. - (1. - 2 * m_xB) * tQ2);
    m_dS[2][0][2] = 8 * sqrt(2.) * m_K[0] * (2. - m_yBMJ[0]) * C1 / m_epsroot[4]
            * (1. + m_xB * tQ2);
    m_dS[2][1][2] = -8 * sqrt(2.) * m_K[0] * (2. - m_yBMJ[0]) * (1. - m_xB) * C1
            * tQ2 / m_epsroot[4];
    m_dS[2][2][2] = -8 * sqrt(2.) * m_K[0] * (2. - m_yBMJ[0]) * C1 * m_xB * tQ2
            * (1. + tQ2) / m_epsroot[4];

    // dC-+
    m_dC[1][0][0] = 4 * m_yBMJ[0] / m_epsroot[4]
            * (Kt2Q2 * (2. - m_yBMJ[0]) * (2. - m_yBMJ[0]) * (1. - m_epsroot[0])
                    + C2 / 2. * (2 * m_xB * tQ2 - (1. - tQ2) * m_epsilonBMJ[1])
                            * (1. - m_epsroot[0]
                                    - tQ2 * (1. - 2 * m_xB + m_epsroot[0])));
    m_dC[1][1][0] = 2 * m_yBMJ[0] * tQ2 / m_epsroot[4]
            * ((4. - 2 * m_xB + 3 * m_epsilonBMJ[1]) * C2
                    * (1.
                            + tQ2 * (4 * m_xB * (1. - m_xB) + m_epsilonBMJ[1])
                                    / (4. - 2 * m_xB + 3 * m_epsilonBMJ[1]))
                    * (m_epsroot[0] - 1. + tQ2 * (1. - 2 * m_xB + m_epsroot[0]))
                    + 2 * (2. - m_yBMJ[0]) * (2. - m_yBMJ[0])
                            * (m_epsroot[0] - 1. + 2 * m_xB) * Kt2Q2);
    m_dC[1][2][0] = 4 * m_xB * m_yBMJ[0] * tQ2 / m_epsroot[4]
            * (2 * (2. - m_yBMJ[0]) * (2. - m_yBMJ[0])
                    * ((1. - m_xB) * tQ2 * (1. + m_xB * tQ2)
                            + (1. + tQ2) * (1. + tQ2) * m_epsilonBMJ[1] / 4.)
                    - C2 * (1. - (1. - 2 * m_xB) * tQ2)
                            * (1. - m_epsroot[0]
                                    - tQ2 * (1. + m_epsroot[0] - 2 * m_xB)));
    m_dC[1][0][1] = 4 * m_K[0] * m_yBMJ[0] * (2. - m_yBMJ[0]) / m_epsroot[4]
            * (1. - m_epsilonBMJ[1] - m_epsroot[0]
                    - tQ2
                            * (1. - m_epsilonBMJ[1] - m_epsroot[0]
                                    - 2 * m_xB * (2. - m_epsroot[0])));
    m_dC[1][1][1] =
            -4 * m_K[0] * m_yBMJ[0] * (2. - m_yBMJ[0]) * tQ2 / m_epsroot[4]
                    * (5. - 4 * m_xB + 3 * m_epsilonBMJ[1] - m_epsroot[0]
                            - tQ2
                                    * (1. - m_epsilonBMJ[1] - m_epsroot[0]
                                            - 2 * m_xB
                                                    * (4. - 4 * m_xB
                                                            - m_epsroot[0])));
    m_dC[1][2][1] = -16 * m_K[0] * m_xB * m_yBMJ[0] * (2. - m_yBMJ[0]) * tQ2
            / m_epsroot[4] * (1. - (1. - 2 * m_xB) * tQ2);
    m_dC[1][0][2] = -2 * m_yBMJ[0] * C2 / m_epsroot[4]
            * (m_epsilonBMJ[1] * (1. + m_epsroot[0])
                    - 2 * tQ2
                            * ((1. - m_xB) * m_epsilonBMJ[1]
                                    + m_xB * (1. + m_epsroot[0]))
                    + tQ2 * tQ2 * (2 * m_xB + m_epsilonBMJ[1])
                            * (1. - 2 * m_xB - m_epsroot[0]));
    m_dC[1][1][2] = -2 * m_yBMJ[0] * C2 * tQ2 / m_epsroot[4]
            * (4. - 2 * m_xB + 3 * m_epsilonBMJ[1]
                    + tQ2 * (4 * m_xB * (1. - m_xB) + m_epsilonBMJ[1]))
            * (1. + m_epsroot[0] - tQ2 * (1. - m_epsroot[0] - 2 * m_xB));
    m_dC[1][2][2] = -4 * m_xB * m_yBMJ[0] * C2 * tQ2 / m_epsroot[4]
            * (1. - (1. - 2 * m_xB) * tQ2)
            * (1. + m_epsroot[0] - tQ2 * (1. - m_epsroot[0] - 2 * m_xB));

    // dS-+
    m_dS[1][0][1] =
            -4 * m_K[0] / m_epsroot[5]
                    * ((2. - m_yBMJ[0]) * (2. - m_yBMJ[0])
                            * (1 + 2 * m_epsilonBMJ[1] - m_epsroot[0]
                                    + tQ2 * (1. - 2 * m_xB - m_epsroot[0]))
                            - C2
                                    * (2. + m_epsilonBMJ[1] - 2 * m_epsroot[0]
                                            + tQ2
                                                    * (m_epsilonBMJ[1]
                                                            - 4 * m_epsroot[0]
                                                            + 2 * m_xB
                                                                    * (1.
                                                                            + m_epsroot[0]))));
    m_dS[1][1][1] =
            -4 * m_K[0] * tQ2 / m_epsroot[5]
                    * (B2
                            * (3. + 2 * m_epsilonBMJ[1] + m_epsroot[0]
                                    - 2 * m_xB * (1. + m_epsroot[0])
                                    - tQ2 * (1. - 2 * m_xB)
                                            * (1. - 2 * m_xB - m_epsroot[0]))
                            + C2
                                    * (8. + 5 * m_epsilonBMJ[1]
                                            - 2 * m_xB * (3. - m_epsroot[0])
                                            - tQ2
                                                    * (2. - m_epsilonBMJ[1]
                                                            + 2 * m_epsroot[0]
                                                            - 12 * m_xB
                                                                    * (1. - m_xB)
                                                            - 4 * m_xB
                                                                    * m_epsroot[0])));
    m_dS[1][2][1] =
            -8 * m_K[0] * B2 * (1. + m_epsroot[0]) * m_xB * tQ2 / m_epsroot[5]
                    * (1.
                            - tQ2 * (1. - m_epsroot[0] - 2 * m_xB)
                                    / (1. + m_epsroot[0]))
                    - 8 * m_K[0] * C2 * m_xB * tQ2 / m_epsroot[5]
                            * (3. - m_epsroot[0]
                                    - tQ2 * (3. + m_epsroot[0] - 6 * m_xB));
    m_dS[1][0][2] = -4 * (2. - m_yBMJ[0]) * C2 / m_epsroot[5]
            * (tQ2 * tQ2
                    * (m_epsilonBMJ[1] - 2 * m_xB2 * (2. + m_epsroot[0])
                            + m_xB * (3. - m_epsilonBMJ[1] + m_epsroot[0]))
                    + m_epsilonBMJ[1] * (1. + m_epsroot[0])
                    + tQ2
                            * (2. + 2 * m_epsroot[0]
                                    + m_epsilonBMJ[1] * m_epsroot[0]
                                    - m_xB
                                            * (3. - m_epsilonBMJ[1]
                                                    + 3 * m_epsroot[0])));
    m_dS[1][1][2] = -4 * (2. - m_yBMJ[0]) * C2 * tQ2 / m_epsroot[4]
            * ((2. - m_xB) * (1. + m_epsroot[0]) + m_epsilonBMJ[1]
                    + 4 * Kt2Q2 * (1. - 2 * m_xB) / m_epsroot[0]
                    + tQ2
                            * (m_epsilonBMJ[1]
                                    + m_xB * (3. - 2 * m_xB + m_epsroot[0])));
    m_dS[1][2][2] = -4 * (2. - m_yBMJ[0]) * C2 * m_xB * tQ2 / m_epsroot[5]
            * (1. + 4 * Kt2Q2 + m_epsroot[0]
                    - 2 * tQ2 * (1. - 2 * m_xB - m_xB * m_epsroot[0])
                    + tQ2 * tQ2 * (1. - 2 * m_xB)
                            * (1. - 2 * m_xB - m_epsroot[0]));
    m_dS[1][0][3] = 4 * m_K[0] * C2 / m_epsroot[5]
            * (2. + m_epsilonBMJ[1] + 2 * m_epsroot[0]
                    + tQ2 * (m_epsilonBMJ[1] + 2 * m_xB * (1. + m_epsroot[0])));
    m_dS[1][1][3] = -4 * m_K[0] * C2 * tQ2 / m_epsroot[4]
            * (4. - 4 * m_xB
                    + tpQ2 * (4 * m_xB * (1. - m_xB) + m_epsilonBMJ[1])
                            / m_epsroot[0]);
    m_dS[1][2][3] = -8 * m_K[0] * C2 * m_xB * tQ2 / m_epsroot[5]
            * (1. + m_epsroot[0] - tQ2 * (1. - 2 * m_xB - m_epsroot[0]));
}

void DVCSProcessBMJ12INT::computeFourierCoeffsInterf() {
// Unpolarized Fourier coeffs
    for (unsigned n = 0; n < 4; n++) {
        m_cI[0][n] = C_I(0, n, 1, 1).real() + C_I(0, n, -1, 1).real()
                + C_I(0, n, 0, 1).real();
    }
    for (unsigned n = 1; n < 3; n++) {
        m_sI[0][n] = m_lambda
                * (S_I(0, n, 1, 1).imag() + S_I(0, n, -1, 1).imag()
                        + S_I(0, n, 0, 1).imag());
    }

// LP Fourier coeffs
    for (unsigned n = 0; n < 3; n++) {
        m_cI[1][n] = m_lambda
                * (C_I(1, n, 1, 1).real() + C_I(1, n, -1, 1).real()
                        + C_I(1, n, 0, 1).real());
    }
    for (unsigned n = 1; n < 4; n++) {
        m_sI[1][n] = (S_I(1, n, 1, 1).imag() + S_I(1, n, -1, 1).imag()
                + S_I(1, n, 0, 1).imag());
    }

// TP Fourier coeffs
    for (unsigned n = 0; n < 4; n++) {
        m_cI[2][n] = m_M[0] / m_Kt[0]
                * ((n != 3) * m_lambda * cos(m_phi2BMK)
                        * (C_I(2, n, 1, 1).real() + C_I(2, n, -1, 1).real()
                                + C_I(2, n, 0, 1).real())
                        - sin(m_phi2BMK)
                                * (C_I(3, n, 1, 1).imag()
                                        + C_I(3, n, -1, 1).imag()
                                        + C_I(3, n, 0, 1).imag()));
    }
    for (unsigned n = 1; n < 4; n++) {
        m_sI[2][n] = m_M[0] / m_Kt[0]
                * (cos(m_phi2BMK)
                        * (S_I(2, n, 1, 1).imag() + S_I(2, n, -1, 1).imag()
                                + S_I(2, n, 0, 1).imag())
                        + (n != 3) * m_lambda * sin(m_phi2BMK)
                                * (S_I(3, n, 1, 1).real()
                                        + S_I(3, n, -1, 1).real()
                                        + S_I(3, n, 0, 1).real()));
    }

}

std::complex<double> DVCSProcessBMJ12INT::CFF(GPDType::Type F, int a, int b) {
    unsigned int j;

    if (a == -1) {
        a = 1;
        b = -1 * b;
    }

    if (a == 1 and b == 1) {
        j = 0;
    } else if (a == 1 and b == -1) {
        j = 1;
    } else if (a == 0 and abs(b) == 1) {
        j = 2;
    } else {
        return 0.;
    }

    if (F == GPDType::H) {
        return m_CFF[0][j];
    } else if (F == GPDType::E) {
        return m_CFF[1][j];
    } else if (F == GPDType::Ht) {
        return m_CFF[2][j];
    } else if (F == GPDType::Et) {
        return m_CFF[3][j];
    } else {
        return 0.;
    }

}

std::complex<double> DVCSProcessBMJ12INT::C_VCS(unsigned int S, int a1, int b1, int a2,
        int b2) {
    std::complex<double> H1(0., 0.), Ht1(0., 0.), E1(0., 0.), Et1(0., 0.);
    std::complex<double> H2(0., 0.), Ht2(0., 0.), E2(0., 0.), Et2(0., 0.);
    H1 = CFF(GPDType::H, a1, b1);
    Ht1 = CFF(GPDType::Ht, a1, b1);
    E1 = CFF(GPDType::E, a1, b1);
    Et1 = CFF(GPDType::Et, a1, b1);
    H2 = CFF(GPDType::H, a2, b2);
    Ht2 = CFF(GPDType::Ht, a2, b2);
    E2 = CFF(GPDType::E, a2, b2);
    Et2 = CFF(GPDType::Et, a2, b2);

    double tQ2 = m_t / m_Q2;
    if (S == 0) {
        return 4. * (1 - m_xB) * (1 + m_xB * tQ2) / m_xBtQ2[1]
                * (H1 * conj(H2) + Ht1 * conj(Ht2))
                + (2 + tQ2) * m_epsilonBMJ[1] / m_xBtQ2[1] * Ht1 * conj(Ht2)
                - m_t / (4 * m_M[1]) * E1 * conj(E2)
                - m_xB2 / m_xBtQ2[1]
                        * (pow(1 + tQ2, 2)
                                * (H1 * conj(E2) + E1 * conj(H2) + E1 * conj(E2))
                                + Ht1 * conj(Et2) + Et1 * conj(Ht2)
                                + m_t / (4 * m_M[1]) * Et1 * conj(Et2));
    } else if (S == 1) {
        return (4 * (1 - m_xB) * (1 + m_xB * tQ2)
                + 2 * (3. / 2. - m_xB + tQ2 / 2.) * m_epsilonBMJ[1]) / m_xBtQ2[1]
                * (H1 * conj(Ht2) + Ht1 * conj(H2))
                - m_xB2 * (1. - (1 - 2 * m_xB) * tQ2) / m_xBtQ2[1]
                        * (H1 * conj(Et2) + Et1 * conj(H2) + Ht1 * conj(E2)
                                + E1 * conj(Ht2))
                - (4 * m_xB * (1. - m_xB) * (1 + m_xB * tQ2) * tQ2
                        + m_xB * (1 + tQ2) * (1 + tQ2) * m_epsilonBMJ[1])
                        / (2 * m_xBtQ2[1]) * (Ht1 * conj(E2) + E1 * conj(Ht2))
                - m_xB / m_xBtQ2[0]
                        * (m_xB2 * (1 + tQ2) * (1 + tQ2) / (2 * m_xBtQ2[0])
                                + m_t / (4 * m_M[1]))
                        * (E1 * conj(Et2) + Et1 * conj(E2));
    } else if (S == 2) {
        return 2. / m_xBtQ2[1]
                * (m_xB * (H1 * conj(Et2) + Et1 * conj(H2))
                        + 4 * m_xB * (1 - 2 * m_xB) * m_M[1] / m_Q2
                                * (H1 * conj(Ht2) + Ht1 * conj(H2))
                        - (m_xBtQ2[0] + (3. + tQ2) * m_epsilonBMJ[1] / 2.)
                                * (Ht1 * conj(E2) + E1 * conj(Ht2))
                        + m_xB2 / 2. * (1 - tQ2)
                                * (E1 * conj(Et2) + Et1 * conj(E2)));
    } else if (S == 3) {
        return 2. / m_xBtQ2[0] * (H1 * conj(E2) - E1 * conj(H2))
                - 2 * m_xB / m_xBtQ2[1] * (Ht1 * conj(Et2) - Et1 * conj(Ht2));
    } else {
        return 0.;
    }
}

std::complex<double> DVCSProcessBMJ12INT::C_VCS(unsigned int S, int a1, int b1, int a2,
        int b2, int a3, int b3) {
    int signe;
    if (S == 0 or S == 3) {
        signe = +1;
    } else if (S == 1 or S == 2) {
        signe = -1;
    } else {
        return 0.;
    }

    return C_VCS(S, a1, b1, a2, b2) + signe * 1. * C_VCS(S, a1, b1, a3, b3);
}

std::complex<double> DVCSProcessBMJ12INT::C_VCS(unsigned int S, int a1, int b1, int a2,
        int b2, int a3, int b3, int a4, int b4) {
    int signe;
    if (S == 0 or S == 3) {
        signe = +1;
    } else if (S == 1 or S == 2) {
        signe = -1;
    } else {
        return 0.;
    }

    return C_VCS(S, a1, b1, a2, b2) + signe * 1. * C_VCS(S, a3, b3, a4, b4);

}

std::complex<double> DVCSProcessBMJ12INT::C_I(unsigned int S, int a, int b,
        const std::string& VA) {
    std::complex<double> H(0., 0.), Ht(0., 0.), E(0., 0.), Et(0., 0.);
    H = CFF(GPDType::H, a, b);
    Ht = CFF(GPDType::Ht, a, b);
    E = CFF(GPDType::E, a, b);
    Et = CFF(GPDType::Et, a, b);

    double F1PlusF2 = m_F1 + m_F2;
    double xBF1PlusF2 = m_xB * F1PlusF2 / m_xBtQ2[0];

    if (S == 0) {
        if (VA == "V") {
            return xBF1PlusF2 * (H + E);
        } else if (VA == "A") {
            return xBF1PlusF2 * Ht;
        } else {
            return m_F1 * H - m_t / (4 * m_M[1]) * m_F2 * E + xBF1PlusF2 * Ht;
        }
    } else if (S == 1) {
        if (VA == "V") {
            return xBF1PlusF2 * (H + m_xB / 2. * (1 - m_t / m_Q2) * E);
        } else if (VA == "A") {
            return xBF1PlusF2
                    * (Ht + 2 * m_xB * m_M[1] / m_Q2 * Ht + m_xB / 2. * Et);
        } else {
            return 2. / m_xBtQ2[0] * m_F1
                    * (((1 - m_xB) * (1 + m_xB * m_t / m_Q2) + m_xB / 2.
                            + m_xB2 * m_M[1] / m_Q2 * (3 + m_t / m_Q2)) * Ht
                            + m_xB / 2.
                                    * (m_t / (4 * m_M[1])
                                            - m_xB / 2. * (1 - m_t / m_Q2)) * Et)
                    + xBF1PlusF2
                            * (H + m_xB / 2. * (1 - m_t / m_Q2) * E
                                    - (1 - 2 * m_xB) * m_t / m_Q2 * Ht
                                    - m_t / (4 * m_M[1]) * Et);
        }
    } else if (S == 2) {
        if (VA == "V") {
            return xBF1PlusF2
                    * (m_xB * (1 - m_t / m_Q2 * (1 - 2 * m_xB))
                            * (H + m_t / (4 * m_M[1]) * E)
                            - m_Kt[1] / (2 * m_M[1]) * E);
        } else if (VA == "A") {
            return -xBF1PlusF2
                    * ((2 - m_xB + 2 * m_xB * m_t / m_Q2
                            + (3 + m_t / m_Q2 - m_t / m_M[1]) * m_epsilonBMJ[1]
                                    / 2.) * Ht
                            - m_xB / 2.
                                    * (m_xB * (1 - m_t / m_Q2)
                                            - m_t / (2 * m_M[1])) * Et);
        } else {
            return m_xB * (1 - (1 - 2 * m_xB) * m_t / m_Q2) * xBF1PlusF2
                    * (H - Ht + m_t / (4 * m_M[1]) * (E - Et))
                    - 1. / m_xBtQ2[0] * m_Kt[1] / m_M[1]
                            * (m_xB / 2. * m_F1
                                    * (E - Et - 4 * m_M[1] / m_Q2 * Ht)
                                    + m_F2 * (m_xB / 2. * E + Ht));
        }
    } else if (S == 3) {
        if (VA == "V") {
            return m_xB * F1PlusF2 * (H + m_t / (4 * m_M[1]) * E);
        } else if (VA == "A") {
            return -m_xB * xBF1PlusF2 * (Ht + m_t / (4 * m_M[1]) * Et);
        } else {
            return 1. / m_xBtQ2[0]
                    * (m_Kt[1] / m_M[1] * (m_F2 * H - m_F1 * E)
                            + m_xB2 * F1PlusF2
                                    * (pow(1 + m_t / m_Q2, 2)
                                            * (H + m_t / (4 * m_M[1]) * E) - Ht
                                            - m_t / (4 * m_M[1]) * Et));
        }
    } else {
        return 0.;
    }
}

std::complex<double> DVCSProcessBMJ12INT::C_I(unsigned int S, unsigned int n, int a,
        int b) {
    unsigned int i;
    if (a == -1) {
        a = 1;
        b = -1 * b;
    }
    if (a == 1 and b == 1) {
        i = 0;
    } else if (a == 1 and b == -1) {
        i = 1;
    } else if (a == 0 and abs(b) == 1) {
        i = 2;
    } else {
        return 0.;
    }

    if (n < 4) {
        if (S == 0 or S == 3) {
            return m_C[i][0][n] * C_I(S, a, b, "")
                    + m_C[i][1][n] * C_I(S, a, b, "V")
                    + m_C[i][2][n] * C_I(S, a, b, "A");
        } else if (S == 1 or S == 2) {
            return m_dC[i][0][n] * C_I(S, a, b, "")
                    + m_dC[i][1][n] * C_I(S, a, b, "V")
                    + m_dC[i][2][n] * C_I(S, a, b, "A");
        } else {
            return 0.;
        }
    } else {
        return 0.;
    }
}

std::complex<double> DVCSProcessBMJ12INT::S_I(unsigned int S, unsigned int n, int a,
        int b) {
    unsigned int i;
    if (a == -1) {
        a = 1;
        b = -1 * b;
    }
    if (a == 1 and b == 1) {
        i = 0;
    } else if (a == 1 and b == -1) {
        i = 1;
    } else if (a == 0 and abs(b) == 1) {
        i = 2;
    } else {
        return 0.;
    }

    if (n < 4) {
        if (S == 0 or S == 3) {
            return m_S[i][0][n] * C_I(S, a, b, "")
                    + m_S[i][1][n] * C_I(S, a, b, "V")
                    + m_S[i][2][n] * C_I(S, a, b, "A");
        } else if (S == 1 or S == 2) {
            return m_dS[i][0][n] * C_I(S, a, b, "")
                    + m_dS[i][1][n] * C_I(S, a, b, "V")
                    + m_dS[i][2][n] * C_I(S, a, b, "A");
        } else {
            return 0.;
        }
    } else {
        return 0.;
    }
}

double DVCSProcessBMJ12INT::SqrAmplBH(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {

    double A = pow(Constant::POSITRON_CHARGE, 6)
            / (m_xB2 * m_yBMJ[1] * m_epsroot[3] * m_t * m_P1 * m_P2);

    double result = 0.;
    for (int n = 0; n < 3; n++) {
        result += (m_cBH[0][n] + m_Lambda * cos(m_theta) * m_cBH[1][n]
                + m_Lambda * sin(m_theta) * m_cBH[2][n]) * cos(n * m_phi1BMK);
    }
    result += m_s1BHTP * m_Lambda * sin(m_theta) * sin(m_phi1BMK);
    result *= A;

    return result;
}

double DVCSProcessBMJ12INT::SqrAmplVCS(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {

    double A = pow(Constant::POSITRON_CHARGE, 6) / (m_yBMJ[1] * m_Q2);

    double result = 0.;
    for (int n(0); n < 3; n++) {
        result += (m_cVCS[0][n] + m_Lambda * cos(m_theta) * m_cVCS[1][n]
                + m_Lambda * sin(m_theta) * m_cVCS[2][n]) * cos(n * m_phi1BMK);
        result += (m_sVCS[0][n] + m_Lambda * cos(m_theta) * m_sVCS[1][n]
                + m_Lambda * sin(m_theta) * m_sVCS[2][n]) * sin(n * m_phi1BMK);
    }
    result *= A;

    return result;
}

double DVCSProcessBMJ12INT::SqrAmplInterf(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {

    double A = -beamCharge * pow(Constant::POSITRON_CHARGE, 6)
            / (m_xB * m_yBMJ[2] * m_t * m_P1 * m_P2);

    double result = 0.;
    for (int n(0); n < 4; n++) {
        result += (m_cI[0][n] + m_Lambda * cos(m_theta) * m_cI[1][n]
                + m_Lambda * sin(m_theta) * m_cI[2][n]) * cos(n * m_phi1BMK);
        result += (m_sI[0][n] + m_Lambda * cos(m_theta) * m_sI[1][n]
                + m_Lambda * sin(m_theta) * m_sI[2][n]) * sin(n * m_phi1BMK);
    }
    result *= A;

    return result;
}

double DVCSProcessBMJ12INT::CrossSectionBH(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {
    return 0;
}

double DVCSProcessBMJ12INT::CrossSectionVCS(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {
    return 0;
}

double DVCSProcessBMJ12INT::CrossSectionInterf(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {
    return SqrAmplInterf(beamHelicity, beamCharge, targetPolarization)
            * m_phaseSpace;
}
