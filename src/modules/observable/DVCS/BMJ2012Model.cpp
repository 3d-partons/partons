/*
 * BMJ2012Model.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: debian
 */

#include "BMJ2012Model.h"

#include <algorithm>
#include <cmath>

#include "../../../BaseObjectRegistry.h"
#include "../../../FundamentalPhysicalConstants.h"
#include "../../../utils/vector/Vector3D.h"

// Initialise [class]::classId with a unique name.
const unsigned int BMJ2012Model::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new BMJ2012Model("BMJ2012Model"));

/*--------------------------------------- Constructors ---------------------------------*/

BMJ2012Model::BMJ2012Model(const std::string &className) :
        DVCSModule(className) {
    m_E = 5.77;
    m_phi1BMK = 0.;
    m_phi2BMK = 0.;
    m_PhiBMK = 0.;
    m_theta = 0.;
    m_Lambda = 0.;
    m_lambda = 0.;
    m_xB2 = 0.;
    m_y[0] = 0.;
    m_Q[0] = 0;
    m_M[0] = 0;
    m_epsilon[0] = 0.;
    m_epsroot = 0.;
    m_K[0] = 0.;
    m_Delta2[0] = 0.;
    m_Delta2_min = 0.;
    std::fill_n(m_cBH[0], 3 * 3, 0.);
    m_s1BHTP = 0.;
}

/*-------------------------------------- Destructor ------------------------------------*/

BMJ2012Model::~BMJ2012Model() {
    // TODO Auto-generated destructor stub
}

BMJ2012Model::BMJ2012Model(const BMJ2012Model& other) :
        DVCSModule(other) {
}

BMJ2012Model* BMJ2012Model::clone() const {
    return new BMJ2012Model(*this);
}

void BMJ2012Model::initModule() {

    //init mother class
    DVCSModule::initModule();

    m_xB2 = pow(m_xB, 2);
    m_Q[0] = sqrt(m_Q2);
    m_Q[1] = m_Q2;
    m_Q[2] = m_Q[0] * m_Q2;
    m_Q[3] = pow(m_Q2, 2);
    m_Delta2[0] = m_t;
    m_Delta2[1] = pow(m_t, 2);
    m_M[0] = PROTON_MASS;
    m_M[1] = pow(PROTON_MASS, 2);
    m_y[0] = m_Q2 / (2 * m_xB * m_M[0] * m_E);
    m_y[1] = m_y[0] * m_y[0];
    m_epsilon[0] = 2 * m_xB * PROTON_MASS / m_Q[0];
    m_epsilon[1] = pow(m_epsilon[0], 2);
    m_epsroot = sqrt(1 + m_epsilon[1]);
    m_Delta2_min = -m_Q2 * (2 * (1 - m_xB) * (1 - m_epsroot) + m_epsilon[1])
            / (4 * m_xB * (1 - m_xB) + m_epsilon[1]);
    m_K[1] = -(m_t / m_Q2) * (1 - m_xB)
            * (1 - m_y[0] - m_y[1] * m_epsilon[1] / 4)
            * (1 - m_Delta2_min / m_t)
            * (m_epsroot
                    + (4 * m_xB * (1 - m_xB) + m_epsilon[1]) / (4 * (1 - m_xB))
                            * (m_t - m_Delta2_min) / m_Q2);
    m_K[0] = sqrt(m_K[1]);

}

void BMJ2012Model::initModule(double beamHelicity, double beamCharge,
        Vector3D targetPolarization) {

    //init mother class
    DVCSModule::initModule(beamHelicity, beamCharge, targetPolarization);

    // define the angles and Lambda
    defineAngles(targetPolarization);

    //define the Lepton propagators
    double Delta2Q2 = m_t / m_Q2;
    double yeps = m_y[0] * (1 + m_epsilon[1]);
    double J = (1 - m_y[0] - m_y[0] * m_epsilon[1]) * (1 + Delta2Q2)
            - (1 - m_xB) * (2 - m_y[0]) * Delta2Q2;
    m_P1 = -(J + 2 * m_K[0] * cos(m_phi1BMK)) / yeps;
    m_P2 = 1 + Delta2Q2 - m_P1;

    // beam helicity
    m_lambda = beamHelicity;

    // Compute Fourier coeffs of BH
    computeFourierCoeffsBH();
}

void BMJ2012Model::isModuleWellConfigured() {

    //check mother class
    DVCSModule::isModuleWellConfigured();
}

void BMJ2012Model::defineAngles(Vector3D targetPolarization) {
    double Px = targetPolarization.getX();
    double Py = targetPolarization.getY();
    double Pz = targetPolarization.getZ();

    //TODO BMK polarization rotation
    double Sx = Px;
    double Sy = Py;
    double Sz = Pz;

    double sintheta = sqrt(pow(Sx, 2) + pow(Sy, 2));
    m_theta = asin(sintheta);
    if (Sz < 0.) {
        m_theta = PI - m_theta;
    }

    if (sintheta != 0.) {
        double cosPhi = Sx / sintheta;
        double sinPhi = Sy / sintheta;
        if (cosPhi == 0.) {
            m_PhiBMK = ((sinPhi > 0.) - (sinPhi < 0.)) * PI / 2;
        } else {
            m_PhiBMK = atan(Sy / Sx);
            if (cosPhi <= 0.) {
                if (sinPhi <= 0.) {
                    m_PhiBMK = m_PhiBMK - PI;
                } else {
                    m_PhiBMK = m_PhiBMK + PI;
                }
            }
        }

    }

    double costheta = cos(m_theta);
    if (costheta != 0.) {
        m_Lambda = Sz / costheta;
    }

    m_phi1BMK = PI - m_phi;
    m_phi2BMK = m_PhiBMK - m_phi1BMK;
}

void BMJ2012Model::computeFourierCoeffsBH() {
    //TODO Get the value of F1(t) and F2(t) from another module
    double F1 = 0;
    double F2 = 0.;
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
    m_cBH[0][0] = 8 * m_K[1]
            * ((2 + 3 * m_epsilon[1]) * m_Q2 / m_t * F1MinusDeltaF2
                    + 2 * m_xB2 * F1PlusF22)
            + pow((2 - m_y[0]), 2)
                    * ((2 + m_epsilon[1])
                            * ((m_xB2 / Delta2M2) * pow((1 + Delta2Q2), 2)
                                    + 4 * (1 - m_xB) * (1 + m_xB * Delta2Q2))
                            * F1MinusDeltaF2
                            + (4 * m_xB2)
                                    * (m_xB
                                            + (1 - m_xB + m_epsilon[1] / 2)
                                                    * pow((1 - m_t / m_Q2), 2)
                                            - m_xB * (1 - 2 * m_xB)
                                                    * m_Delta2[1] / m_Q[3])
                                    * F1PlusF22)
            + 8 * (1 + m_epsilon[1]) * (1 - m_y[0] - m_epsilon[1] * m_y[1] / 4)
                    * (2 * m_epsilon[1] * (1 - Delta2M2) * F1MinusDeltaF2
                            - m_xB2 * pow((1 - Delta2Q2), 2) * F1PlusF22);
    m_cBH[0][1] = 8 * m_K[0] * (2 - m_y[0])
            * ((m_xB2 / Delta2M2 - 2 * m_xB - m_epsilon[1]) * F1MinusDeltaF2
                    + 2 * m_xB2 * (1 - (1 - 2 * m_xB2) * Delta2Q2) * F1PlusF22);
    m_cBH[0][2] = 8 * m_xB2 * m_K[1]
            * (F1MinusDeltaF2 / Delta2M2 + 2 * F1PlusF22);

    // LP Fourier coefficients
    double C1 = 8 * m_lambda * m_Lambda * m_xB * m_y[0] * m_epsroot * F1PlusF2
            / (1 - Delta2M2);
    m_cBH[1][0] = C1 * (2 - m_y[0])
            * (0.5 * (m_xB / 2 * (1 - Delta2Q2) - Delta2M2)
                    * (2 - m_xB - 2 * pow((1 - m_xB), 2) * Delta2Q2
                            + m_epsilon[1] * (1 - Delta2Q2)
                            - m_xB * (1 - 2 * m_xB) * m_Delta2[1] / m_Q[3])
                    * F1PlusF2
                    + (1 - (1 - m_xB) * Delta2Q2)
                            * (m_xB2 / (4 * Delta2M2) * pow(1 + Delta2Q2, 2)
                                    + (1 - m_xB) * (1 + m_xB * Delta2Q2))
                            * F1PlusDeltaF2);
    m_cBH[1][1] = -C1 * m_K[0]
            * ((2 * Delta2M2 - m_xB * (1 - Delta2Q2))
                    * (1 - m_xB + m_xB * Delta2Q2) * F1PlusF2
                    + (1 + m_xB - (3 - 2 * m_xB) * (1 + m_xB * Delta2Q2)
                            - m_xB2 / Delta2M2 * (1 + m_Delta2[1] / m_Q[3]))
                            * F1PlusDeltaF2);

    // TP Fourier coefficients
    double C2 = sqrt(1 - m_y[0] - m_epsilon[1] * m_y[1] / 4);
    m_cBH[2][0] = -8 * m_lambda * cos(m_phi2BMK) * (2 - m_y[0]) * m_y[0] / MQ
            * m_epsroot * m_K[0] / C2 * F1PlusF2
            * (m_xB * m_xB2 * pow(MQ, 2) * (1 - Delta2Q2) * F1PlusF2
                    + (1 - (1 - m_xB) * Delta2Q2)
                            * (m_xB2 / (4 * Delta2M2) * (1 - Delta2Q2) * F1
                                    + m_xB / 2 * F2));
    m_cBH[2][1] =
            -16 * m_lambda * cos(m_phi2BMK) * m_xB * m_y[0] * C2 * MQ
                    * m_epsroot * F1PlusF2
                    * (2 * m_K[1] / (Delta2Q2 * pow(C2, 2))
                            * (m_xB * (1 - Delta2Q2) * F1 + Delta2M2 * F2)
                            + (1 + m_epsilon[1]) * m_xB * (1 - Delta2Q2)
                                    * F1PlusDeltaF2);
    m_s1BHTP = 16 * m_lambda * sin(m_phi2BMK) * m_y[0] * m_xB2 * C2 * MQ
            * pow(m_epsroot, 3) * (1 - Delta2Q2) * F1PlusF2 * F1PlusDeltaF2;
}

double BMJ2012Model::SqrAmplBH(double beamHelicity, double beamCharge,
        Vector3D targetPolarization) {

    double A = pow(POSITRON_CHARGE, 6)
            / (m_xB2 * m_y[1] * pow(1 + m_epsilon[1], 2) * m_t * m_P1 * m_P2);

    double result = 0;
    for (int n(0); n < 3; n++) {
        result += (m_cBH[0][n] + cos(m_theta) * m_cBH[1][n]
                + sin(m_theta) * m_cBH[2][n]) * cos(n * m_phi1BMK);
    }
    result += m_s1BHTP * sin(m_phi1BMK);
    result *= A;

    return result;
}

double BMJ2012Model::SqrAmplVCS(double beamHelicity, double beamCharge,
        Vector3D targetPolarization) {
}

double BMJ2012Model::SqrAmplInterf(double beamHelicity, double beamCharge,
        Vector3D targetPolarization) {
}

double BMJ2012Model::CrossSectionBH(double beamHelicity, double beamCharge,
        Vector3D targetPolarization) {
    return SqrAmplBH(beamHelicity, beamCharge, targetPolarization)
            * m_phaseSpace;
}

double BMJ2012Model::CrossSectionVCS(double beamHelicity, double beamCharge,
        Vector3D targetPolarization) {
    return SqrAmplVCS(beamHelicity, beamCharge, targetPolarization)
            * m_phaseSpace;
}

double BMJ2012Model::CrossSectionInterf(double beamHelicity, double beamCharge,
        Vector3D targetPolarization) {
    return SqrAmplInterf(beamHelicity, beamCharge, targetPolarization)
            * m_phaseSpace;
}
