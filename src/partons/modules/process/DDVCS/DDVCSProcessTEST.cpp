#include "../../../../../include/partons/modules/process/DDVCS/DDVCSProcessTEST.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <cmath>

#include "../../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DDVCSProcessTEST::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSProcessTEST("DDVCSProcessTEST"));

DDVCSProcessTEST::DDVCSProcessTEST(const std::string &className) :
        DDVCSProcessModule(className) {

    m_BM_Q2 = 0.;
    m_BM_eta = 0.;
    m_BM_xi = 0.;
    m_BM_y = 0.;
    m_BM_ytilde = 0.;
    m_BM_ytildePlus = 0.;
    m_BM_F1 = 0.;
    m_BM_F2 = 0.;
    m_BM_charge_e = 0.;

    m_BM_tmin = 0.;
    m_BM_tmax = 0.;

    m_BM_edgeFactor = 0.;

    m_BM_Kcap = 0.;
    m_BM_Ktildecap = 0.;
    m_BM_KtildecapPlus = 0.;

    m_BM_kDelta = 0.;
    m_BM_lminusDelta = 0.;
    m_BM_lplusDelta = 0.;

    m_BM_P1 = 0.;
    m_BM_P2 = 0.;
    m_BM_P3 = 0.;
    m_BM_P4 = 0.;

    m_BM_beta = 0.;

    m_BM_xBmin = 0.;
    m_BM_xBmax = 0.;
    m_BM_Qcal2min = 0.;
    m_BM_Qcal2max = 0.;
    m_BM_Mll2min = 0.;
    m_BM_Mll2max = 0.;
}

DDVCSProcessTEST::DDVCSProcessTEST(const DDVCSProcessTEST& other) :
        DDVCSProcessModule(other) {

    m_BM_Q2 = other.m_BM_Q2;
    m_BM_eta = other.m_BM_eta;
    m_BM_xi = other.m_BM_xi;
    m_BM_y = other.m_BM_y;
    m_BM_ytilde = other.m_BM_ytilde;
    m_BM_ytildePlus = other.m_BM_ytildePlus;
    m_BM_F1 = other.m_BM_F1;
    m_BM_F2 = other.m_BM_F2;
    m_BM_charge_e = other.m_BM_charge_e;

    m_BM_tmin = other.m_BM_tmin;
    m_BM_tmax = other.m_BM_tmax;

    m_BM_edgeFactor = other.m_BM_edgeFactor;

    m_BM_Kcap = other.m_BM_Kcap;
    m_BM_Ktildecap = other.m_BM_Ktildecap;
    m_BM_KtildecapPlus = other.m_BM_KtildecapPlus;

    m_BM_kDelta = other.m_BM_kDelta;
    m_BM_lminusDelta = other.m_BM_lminusDelta;
    m_BM_lplusDelta = other.m_BM_lplusDelta;

    m_BM_P1 = other.m_BM_P1;
    m_BM_P2 = other.m_BM_P2;
    m_BM_P3 = other.m_BM_P3;
    m_BM_P4 = other.m_BM_P4;

    m_BM_beta = other.m_BM_beta;

    m_BM_xBmin = other.m_BM_xBmin;
    m_BM_xBmax = other.m_BM_xBmax;
    m_BM_Qcal2min = other.m_BM_Qcal2min;
    m_BM_Qcal2max = other.m_BM_Qcal2max;
    m_BM_Mll2min = other.m_BM_Mll2min;
    m_BM_Mll2max = other.m_BM_Mll2max;

    m_cffH = other.m_cffH;
    m_cffE = other.m_cffE;
    m_cffHt = other.m_cffHt;
    m_cffEt = other.m_cffEt;
    m_cffHL = other.m_cffHL;
    m_cffEL = other.m_cffEL;
}

DDVCSProcessTEST::~DDVCSProcessTEST() {
}

DDVCSProcessTEST* DDVCSProcessTEST::clone() const {
    return new DDVCSProcessTEST(*this);
}

void DDVCSProcessTEST::initModule() {

    //init mother class
    DDVCSProcessModule::initModule();

    //compute internal variables
    computeInternalVariables(0., m_E, Constant::PROTON_MASS,
            m_xB, m_t, m_Q2, m_Q2Prim, m_phi, m_phiL, m_thetaL); //1st parameter is muon mass
    //TODO
//                m_phil, m_thetal);
}

void DDVCSProcessTEST::isModuleWellConfigured() {

    //check mother class
    DDVCSProcessModule::isModuleWellConfigured();

    //check kinematic limits
    if (m_xB < m_BM_xBmin || m_xB > m_BM_xBmax) {

        warn(__func__,
                ElemUtils::Formatter() << "Invalid kinematic limits, xB: "
                        << m_xB << " not between " << m_BM_xBmin << " and "
                        << m_BM_xBmax);
    }

    if (m_Q2 < m_BM_Qcal2min || m_Q2 > m_BM_Qcal2max) {

        warn(__func__,
                ElemUtils::Formatter() << "Invalid kinematic limits, Q2: "
                        << m_Q2 << " not between " << m_BM_Qcal2min << " and "
                        << m_BM_Qcal2max);
    }

    if (m_Q2Prim < m_BM_Mll2min || m_Q2Prim > m_BM_Mll2max) {

        warn(__func__,
                ElemUtils::Formatter() << "Invalid kinematic limits, Q2': "
                        << m_Q2Prim << " not between " << m_BM_Mll2min
                        << " and " << m_BM_Mll2max);
    }

    if (-m_t < -m_BM_tmin || -m_t > -m_BM_tmax) {

        warn(__func__,
                ElemUtils::Formatter() << "Invalid kinematic limits, -t: "
                        << -m_t << " not between " << -m_BM_tmin << " and "
                        << -m_BM_tmax);
    }
}

PhysicalType<double> DDVCSProcessTEST::CrossSectionBH() {

    //availible variables
//    m_xB;
//    m_t;
//    m_Q2;
//    m_Q2Prim;
//    m_E;
//    m_phi;
//    m_theta;
//
//    m_dvcsConvolCoeffFunctionResult;
//
//    m_beamHelicity;
//    m_beamCharge;
//    m_targetPolarization;

//check target polarisation
    int polariz;

    if (m_targetPolarization.getX() == 0. && m_targetPolarization.getY() == 0.
            && m_targetPolarization.getZ() == 0.) {
        polariz = 0;
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Not able to run for polarisation "
                        << m_targetPolarization.toString());
    }

    double xsec = crossSectionBH(m_E, Constant::PROTON_MASS, m_xB, m_t, m_Q2,
            m_Q2Prim, m_phi, m_phiL, m_thetaL, m_beamCharge, polariz);

    return PhysicalType<double>(xsec, PhysicalUnit::GEVm2);
}

PhysicalType<double> DDVCSProcessTEST::CrossSectionVCS() {

    //check target polarisation
    int polariz;

    if (m_targetPolarization.getX() == 0. && m_targetPolarization.getY() == 0.
            && m_targetPolarization.getZ() == 0.) {
        polariz = 0;
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Not able to run for polarisation "
                        << m_targetPolarization.toString());
    }

    double xsec = crossSectionVCS(m_E, Constant::PROTON_MASS, m_xB, m_t, m_Q2,
            m_Q2Prim, m_phi, m_phiL, m_thetaL, m_beamCharge, polariz);

    return PhysicalType<double>(xsec, PhysicalUnit::GEVm2);
}

PhysicalType<double> DDVCSProcessTEST::CrossSectionInterf() {

    //check target polarisation
    int polariz;

    if (m_targetPolarization.getX() == 0. && m_targetPolarization.getY() == 0.
            && m_targetPolarization.getZ() == 0.) {
        polariz = 0;
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Not able to run for polarisation "
                        << m_targetPolarization.toString());
    }

    double xsec = crossSectionInterf(m_E, Constant::PROTON_MASS, m_xB, m_t,
            m_Q2, m_Q2Prim, m_phi, m_phiL, m_thetaL, m_beamCharge, polariz,
            m_beamHelicity);

    return PhysicalType<double>(xsec, PhysicalUnit::GEVm2);

}

double DDVCSProcessTEST::bh_squared(double Mnucleon, double t, double phi,
        double phil, double Q2, double eta, double xi, double y, double ytilde,
        double F1, double F2, double tmin, double tmax, double edgeFactor,
        double P1, double P2, double P3, double P4, int beamSign,
        double charge_e) const {

    double cc11[5][3] = { 0. }, cc12[5][3] = { 0. }, cc22[5][3] = { 0. }; //Fourier coeffs for squared BH amplitude
    double cs11[5][3] = { 0. }, cs12[5][3] = { 0. }, cs22[5][3] = { 0. };
    double sc11[5][3] = { 0. }, sc12[5][3] = { 0. }, sc22[5][3] = { 0. };
    double ss11[5][3] = { 0. }, ss12[5][3] = { 0. }, ss22[5][3] = { 0. };

    int sigma;

    //(all indices are swifted with respect to Belitsky2003: eqs 129 to 144)

    // |BH1|^2 coeffs

    sigma = 1;

    cc11[0][0] = -2. * ((1. - eta) / (1. + eta)) * (1. + xi / eta) * (1. - y)
            * ((2. - 2. * y + pow(y, 2.)) * (2. - 2. * ytilde + pow(ytilde, 2.))
                    * (1. + pow(xi / eta, 2.))
                    - 8. * (1. - y) * (1. - ytilde) * (1. - pow(xi / eta, 2.)))
            * ((1. - tmin / t)
                    * (pow(F1, 2.) - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)))
                    + 2. * pow(eta * (F1 + F2), 2.) / (1. - pow(eta, 2.)));

    cc11[2][0] = 2. * ((1. - eta) / (1. + eta)) * (1. + xi / eta)
            * (1. - pow(xi / eta, 2.)) * (1. - tmin / t) * (1. - y)
            * ((2. - 2. * y + pow(y, 2.)) * (2. - 2. * ytilde + pow(ytilde, 2.))
                    + 8. * (1. - y) * (1. - ytilde))
            * (pow(F1, 2.) - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)));

    cc11[1][1] = 4. * ((1. - eta) / (1. + eta)) * (1. + xi / eta)
            * (-sigma / eta)
            * sqrt((1. - y) * (1. - ytilde) * (pow(xi, 2.) - pow(eta, 2.)))
            * (1. - y) * (2. - y) * (2. - ytilde)
            * ((1. - 3. * xi / eta) * (1. - tmin / t)
                    * (pow(F1, 2.) - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)))
                    - 4. * xi * eta * pow((F1 + F2), 2.) / (1. - pow(eta, 2.)));

    cc11[3][1] = -4. * ((1. - eta) / (1. + eta)) * pow((1. + xi / eta), 2.)
            * (1 - tmin / t) * (-sigma / eta)
            * sqrt((1. - y) * (1. - ytilde) * (pow(xi, 2.) - pow(eta, 2.)))
            * (1. - y) * (2. - y) * (2. - ytilde)
            * (pow(F1, 2.) - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)));

    cc11[0][2] = -4. * ((1. - eta) / (1. + eta)) * (1. - xi / eta)
            * (1. - pow(xi / eta, 2.)) * (1 - tmin / t) * pow((1. - y), 2.)
            * (1. - ytilde)
            * (pow(F1, 2.) - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)));

    cc11[2][2] = 8. * ((1. - eta) / (1. + eta)) * (1. + xi / eta)
            * (1. - pow(xi / eta, 2.)) * pow((1. - y), 2.) * (1. - ytilde)
            * ((1 - tmin / t)
                    * (pow(F1, 2.) - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)))
                    + 2. * pow(eta * (F1 + F2), 2.) / (1. - pow(eta, 2.)));

    cc11[4][2] = pow((xi + eta) / (xi - eta), 2.) * cc11[0][2];

    ss11[1][1] = cc11[1][1] + 2. * cc11[3][1] * (eta - xi) / (eta + xi);

    ss11[3][1] = cc11[3][1];

    ss11[2][2] = cc11[2][2];

    ss11[4][2] = cc11[4][2];

    // |BH2|^2 coeffs

    //Saving original values of y, ytilde and xi, since I'll modify them below
    double ySave, ytildeSave, xiSave;
    ySave = y;
    ytildeSave = ytilde;
    xiSave = xi;

    //I use the property (cc, ss)22 = -(cc, ss)11 upon y <-> ytilde and xi -> -xi => sigma = -1
    ytilde = ySave;
    y = ytildeSave;
    xi = -xiSave;
    sigma = -1;

    // 1st: variable changes made above allows us to simply copy the expressions for (cc, ss)^11
    cc22[0][0] = -2. * ((1. - eta) / (1. + eta)) * (1. + xi / eta) * (1. - y)
            * ((2. - 2. * y + pow(y, 2.)) * (2. - 2. * ytilde + pow(ytilde, 2.))
                    * (1. + pow(xi / eta, 2.))
                    - 8. * (1. - y) * (1. - ytilde) * (1. - pow(xi / eta, 2.)))
            * ((1. - tmin / t)
                    * (pow(F1, 2.) - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)))
                    + 2. * pow(eta * (F1 + F2), 2.) / (1. - pow(eta, 2.)));

    cc22[2][0] = 2. * ((1. - eta) / (1. + eta)) * (1. + xi / eta)
            * (1. - pow(xi / eta, 2.)) * (1. - tmin / t) * (1. - y)
            * ((2. - 2. * y + pow(y, 2.)) * (2. - 2. * ytilde + pow(ytilde, 2.))
                    + 8. * (1. - y) * (1. - ytilde))
            * (pow(F1, 2.) - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)));

    cc22[1][1] = 4. * ((1. - eta) / (1. + eta)) * (1. + xi / eta)
            * (-sigma / eta)
            * sqrt((1. - y) * (1. - ytilde) * (pow(xi, 2.) - pow(eta, 2.)))
            * (1. - y) * (2. - y) * (2. - ytilde)
            * ((1. - 3. * xi / eta) * (1. - tmin / t)
                    * (pow(F1, 2.) - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)))
                    - 4. * xi * eta * pow((F1 + F2), 2.) / (1. - pow(eta, 2.)));

    cc22[3][1] = -4. * ((1. - eta) / (1. + eta)) * pow((1. + xi / eta), 2.)
            * (1 - tmin / t) * (-sigma / eta)
            * sqrt((1. - y) * (1. - ytilde) * (pow(xi, 2.) - pow(eta, 2.)))
            * (1. - y) * (2. - y) * (2. - ytilde)
            * (pow(F1, 2.) - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)));

    cc22[0][2] = -4. * ((1. - eta) / (1. + eta)) * (1. - xi / eta)
            * (1. - pow(xi / eta, 2.)) * (1 - tmin / t) * pow((1. - y), 2.)
            * (1. - ytilde)
            * (pow(F1, 2.) - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)));

    cc22[2][2] = 8. * ((1. - eta) / (1. + eta)) * (1. + xi / eta)
            * (1. - pow(xi / eta, 2.)) * pow((1. - y), 2.) * (1. - ytilde)
            * ((1 - tmin / t)
                    * (pow(F1, 2.) - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)))
                    + 2. * pow(eta * (F1 + F2), 2.) / (1. - pow(eta, 2.)));

    cc22[4][2] = pow((xi + eta) / (xi - eta), 2.) * cc22[0][2];

    ss22[1][1] = cc22[1][1] + 2. * cc22[3][1] * (eta - xi) / (eta + xi);

    ss22[3][1] = cc22[3][1];

    ss22[2][2] = cc22[2][2];

    ss22[4][2] = cc22[4][2];

    // 2nd: add the global minus sign
    int i, j;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 3; j++) {
            cc22[i][j] = -cc22[i][j];
            ss22[i][j] = -ss22[i][j];
        }
    }

    //Let's get back original values for y, ytilde, xi and sigma
    y = ySave;
    ytilde = ytildeSave;
    xi = xiSave;
    sigma = 1;

    // conj(BH1)*BH2 + conj(BH2)*BH1 coeffs

    cc12[0][0] = -8. * ((1. - eta) / (1. + eta)) * (-xi / eta)
            * (1. - pow(xi / eta, 2.)) * (1. - y) * (2. - y) * (1. - ytilde)
            * (2. - ytilde)
            * ((1. - tmin / t)
                    * (pow(F1, 2.) - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)))
                    + 2. * pow(eta * (F1 + F2), 2.) / (1. - pow(eta, 2.)));

    cc12[2][0] = 8. * ((1. - eta) / (1. + eta)) * (1. + xi / eta)
            * (1. - pow(xi / eta, 2.)) * (1. - y) * (2. - y) * (1. - ytilde)
            * (2. - ytilde) * (1. - tmin / t)
            * (pow(F1, 2.) - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)));

    cc12[1][1] =
            -8. * ((1. - eta) / (1. + eta)) * (-sigma / eta)
                    * sqrt(
                            (1. - y) * (1. - ytilde)
                                    * (pow(xi, 2.) - pow(eta, 2.)))
                    * ((2. - 2. * y + pow(y, 2.))
                            * (2. - 2. * ytilde + pow(ytilde, 2.))
                            * (pow(xi / eta, 2.) * (1. - tmin / t)
                                    * (pow(F1, 2.)
                                            - pow(F2, 2.) * t
                                                    / (4. * pow(Mnucleon, 2.)))
                                    + (pow(xi, 2.) + pow(eta, 2.))
                                            * pow(F1 + F2, 2.)
                                            / (1. - pow(eta, 2.)))
                            - (1. - y) * (1. - ytilde)
                                    * (1. - pow(xi / eta, 2.))
                                    * (9. * (1. - tmin / t)
                                            * (pow(F1, 2.)
                                                    - pow(F2, 2.) * t
                                                            / (4.
                                                                    * pow(
                                                                            Mnucleon,
                                                                            2.)))
                                            + 10. * pow(eta * (F1 + F2), 2.)
                                                    / (1. - pow(eta, 2.))));

    cc12[0][2] = -8. * ((1. - eta) / (1. + eta)) * (1. - xi / eta)
            * (1. - pow(xi / eta, 2.)) * (1. - tmin / t) * (1. - y) * (2. - y)
            * (1. - ytilde) * (2. - ytilde)
            * (pow(F1, 2.) - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)));

    cc12[2][2] = -8. * ((1. - eta) / (1. + eta)) * (-xi / eta)
            * (1. - pow(xi / eta, 2.)) * (1. - y) * (2. - y) * (1. - ytilde)
            * (2. - ytilde)
            * ((1. - tmin / t)
                    * (pow(F1, 2.) - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)))
                    + 2. * pow(eta * (F1 + F2), 2.) / (1. - pow(eta, 2.)));

    ss12[1][1] =
            cc12[1][1]
                    - 8. * ((1. - eta) / (1. + eta)) * (-sigma / eta)
                            * sqrt(
                                    (1. - y) * (1. - ytilde)
                                            * (pow(xi, 2.) - pow(eta, 2.)))
                            * (1. - pow(xi / eta, 2.))
                            * ((2. - 2. * y + pow(y, 2.))
                                    * (2. - 2. * ytilde + pow(ytilde, 2.))
                                    + 8. * (1. - y) * (1. - ytilde))
                            * (1. - tmin / t)
                            * (pow(F1, 2.)
                                    - pow(F2, 2.) * t / (4. * pow(Mnucleon, 2.)));

    ss12[2][2] = cc12[2][2];

    double T2_bh, prefactor, series11 = 0., series12 = 0., series22 = 0.,
            series = 0.;

    prefactor = edgeFactor * xi * pow(charge_e, 8.)
            / (pow(y * ytilde, 4.) * t * Q2 * eta); // eq 99 in Belitsky2003. This prefactor goes in front of the whole Fourier series

    int n, m;
    for (n = 0; n < 5; n++) {
        for (m = 0; m < 3; m++) {

            series11 +=
                    (cc11[n][m] * cos(m * phil) + cs11[n][m] * sin(m * phil))
                            * cos(n * phi)
                            + (sc11[n][m] * cos(m * phil)
                                    + ss11[n][m] * sin(m * phil))
                                    * sin(n * phi); //series with coeffs (c, s)^11 in eq 99 of Belitsky2003
            series22 += (cc22[n][m] * cos(m * phi) + cs22[n][m] * sin(m * phi))
                    * cos(n * phil)
                    + (sc22[n][m] * cos(m * phi) + ss22[n][m] * sin(m * phi))
                            * sin(n * phil); //series with coeffs (c, s)^22 in eq 99 of Belitsky2003
            series12 +=
                    (cc12[n][m] * cos(m * phil) + cs12[n][m] * sin(m * phil))
                            * cos(n * phi)
                            + (sc12[n][m] * cos(m * phil)
                                    + ss12[n][m] * sin(m * phil))
                                    * sin(n * phi); //series with coeffs (c, s)^12 in eq 99 of Belitsky2003

        }
    }

    series22 = 0.;//DEBUG
    series12 = 0.;//DEBUG

    series = pow(ytilde / (P1 * P2), 2.) * series11
            + pow(y / (P3 * P4), 2.) * series22
            - beamSign * y * ytilde * series12 / (P1 * P2 * P3 * P4); //the 3 series above with their correspondent prefactors as in eq 99 in Belitsky2003

            //Final squared BH amplitude from eq 99 in Belitsky2003:
    T2_bh = prefactor * series;

    return T2_bh;
}

double DDVCSProcessTEST::crossSectionBH(double Ebeam, double Mnucleon,
        double xB, double t, double Qcal2, double Mll2, double phi, double phil,
        double thetal, int beamSign, int polariz) const {

    //We change to Belitsky's phi: LHS is phi in Belitsky2003 notation, RHS' phi is in Trento's that must be the input in crossSection()
    phi = M_PI - phi;

    //phi entering these expressions is in Belitsky's convention already:

    double T2_BH = bh_squared(Mnucleon, t, phi, phil, m_BM_Q2, m_BM_eta,
            m_BM_xi, m_BM_y, m_BM_ytilde, m_BM_F1, m_BM_F2, m_BM_tmin,
            m_BM_tmax, m_BM_edgeFactor, m_BM_P1, m_BM_P2, m_BM_P3, m_BM_P4,
            beamSign, m_BM_charge_e);

    //7-(or 8- if polarized target)fold differential cross-section in pbarn/GeV^6 (ALL ENERGY QUANTITIES IN GeV !!)
    double xsec;

    if (polariz == 0) {

        xsec = pow(Constant::FINE_STRUCTURE_CONSTANT, 4.) * xB * pow(m_BM_y, 2.)
                * m_BM_beta * T2_BH * (1. / pow(m_BM_charge_e, 8.))
                * sin(thetal)
                / (16 * pow(2. * M_PI, 3.) * pow(Qcal2, 2.)
                        * sqrt(1. + pow(2. * xB * Mnucleon, 2.) / Qcal2));

    } else {

        xsec = (1. / (2. * M_PI)) * pow(Constant::FINE_STRUCTURE_CONSTANT, 4.)
                * xB * pow(m_BM_y, 2.) * m_BM_beta * T2_BH
                * (1. / pow(m_BM_charge_e, 8.)) * sin(thetal)
                / (16 * pow(2. * M_PI, 3.) * pow(Qcal2, 2.)
                        * sqrt(1. + pow(2. * xB * Mnucleon, 2.) / Qcal2));
    }

    return xsec;

}

double DDVCSProcessTEST::vcs_squared(double Mnucleon, double t, double phi,
        double phil, double Q2, double eta, double xi, double y, double ytilde,
        double edgeFactor, const std::complex<double>& cffH,
        const std::complex<double>& cffE, const std::complex<double>& cffHL,
        const std::complex<double>& cffEL,
        const std::complex<double>& cffHtilde,
        const std::complex<double>& cffEtilde, double charge_e) const {

    //VCS' Fourier coeffs
    std::complex<double> cc[3][3];
    std::complex<double> ss[3][3];
    std::complex<double> cs[3][3];
    std::complex<double> sc[3][3];

    int sigma;

    std::complex<double> CVV_FF, CVV_FLF, CVV_FFL, CVV_FLFL, CAA_FF, CVA_FF;

    //Functions (107)-(109) from Belitsky2003, at LT:
    CVV_FF = (1. - pow(eta, 2.)) * cffH * conj(cffH)
            - pow(eta, 2.) * (cffH * conj(cffE) + cffE * conj(cffH))
            - (t / pow(2. * Mnucleon, 2.) + pow(eta, 2.)) * cffE * conj(cffE);
    CVV_FLF = (1. - pow(eta, 2.)) * cffHL * conj(cffH)
            - pow(eta, 2.) * (cffHL * conj(cffE) + cffEL * conj(cffH))
            - (t / pow(2. * Mnucleon, 2.) + pow(eta, 2.)) * cffEL * conj(cffE);
    CVV_FFL = (1. - pow(eta, 2.)) * cffH * conj(cffHL)
            - pow(eta, 2.) * (cffH * conj(cffEL) + cffE * conj(cffHL))
            - (t / pow(2. * Mnucleon, 2.) + pow(eta, 2.)) * cffE * conj(cffEL);
    CVV_FLFL = (1. - pow(eta, 2.)) * cffHL * conj(cffHL)
            - pow(eta, 2.) * (cffHL * conj(cffEL) + cffEL * conj(cffHL))
            - (t / pow(2. * Mnucleon, 2.) + pow(eta, 2.)) * cffEL * conj(cffEL);
    CAA_FF =
            (1. - pow(eta, 2.)) * cffHtilde * conj(cffHtilde)
                    - pow(eta, 2.)
                            * (cffHtilde * conj(cffEtilde)
                                    + cffEtilde * conj(cffHtilde))
                    - (pow(eta, 2.) * t / pow(2. * Mnucleon, 2.)) * cffEtilde
                            * conj(cffEtilde);
    CVA_FF = std::complex<double>(0., 0.);

    sigma = 1; //I assume this value because in VCS there's no coeff to which we apply xi -> -xi. Thereby, kinematics fixes sigma = 1

    //eqs 110-112 in Belitsky2003
    cc[0][0] = 2. * (2. - 2. * y + pow(y, 2.))
            * (2. - 2. * ytilde + pow(ytilde, 2.)) * (CVV_FF + CAA_FF)
            + (16. / pow(xi, 2.)) * (1. - y) * (1. - ytilde)
                    * (pow(xi, 2.) - pow(eta, 2.)) * CVV_FLFL;
    cc[1][1] = (4. * sigma / xi)
            * sqrt((1. - y) * (1. - ytilde) * (pow(xi, 2.) - pow(eta, 2.)))
            * (2. - y) * (2. - ytilde) * (CVV_FFL + CVV_FLF);
    cc[2][2] = 8. * (1. - y) * (1. - ytilde) * (CVV_FF - CAA_FF);

    //eqs 105 in Belitsky2003
    ss[0][0] = cc[0][0];
    ss[1][1] = cc[1][1];
    ss[2][2] = cc[2][2];

    int i, j;
    std::complex<double> series = std::complex<double>(0., 0.);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            series += (cc[i][j] * cos(j * phil) + cs[i][j] * sin(j * phil))
                    * cos(i * phi)
                    + (sc[i][j] * cos(j * phil) + ss[i][j] * sin(j * phil))
                            * sin(i * phi); //series with coeffs (c, s)^VCS in eq 97 from Belitsky2003
        };
    };

    double prefactor;
    std::complex<double> T2_vcs;

    prefactor = edgeFactor * 2. * pow(xi / (1. + eta), 2.) * pow(charge_e, 8.)
            / (pow(Q2 * y * ytilde, 2.)); //prefactor preceding the whole Fourier series in eq 97 from Belitsky2003

    T2_vcs = prefactor * series;

    return real(T2_vcs); //cffs are computed numerically in PARTONS, so imaginary part of cff*conj(cff) could be slightly different from zero. This small error is removed by taking only real part of T2_vcs

}

double DDVCSProcessTEST::crossSectionVCS(double Ebeam, double Mnucleon,
        double xB, double t, double Qcal2, double Mll2, double phi, double phil,
        double thetal, int beamSign, int polariz) const {

    //We change to Belitsky's phi: LHS is phi in Belitsky2003 notation, RHS' phi is in Trento's that must be the input in crossSection()
    phi = M_PI - phi;

    //phi entering these expressions is in Belitsky's convention already:
    double T2_VCS = vcs_squared(Mnucleon, t, phi, phil, m_BM_Q2, m_BM_eta,
            m_BM_xi, m_BM_y, m_BM_ytilde, m_BM_edgeFactor, m_cffH, m_cffE,
            m_cffHL, m_cffEL, m_cffHt, m_cffEt, m_BM_charge_e);

    //7-(or 8- if polarized target)fold differential cross-section in pbarn/GeV^6 (ALL ENERGY QUANTITIES IN GeV !!)
    double xsec;

    if (polariz == 0) {

        xsec = pow(Constant::FINE_STRUCTURE_CONSTANT, 4.) * xB * pow(m_BM_y, 2.)
                * m_BM_beta * T2_VCS * (1. / pow(m_BM_charge_e, 8.))
                * sin(thetal)
                / (16 * pow(2. * M_PI, 3.) * pow(Qcal2, 2.)
                        * sqrt(1. + pow(2. * xB * Mnucleon, 2.) / Qcal2));

    } else {

        xsec = (1. / (2. * M_PI)) * pow(Constant::FINE_STRUCTURE_CONSTANT, 4.)
                * xB * pow(m_BM_y, 2.) * m_BM_beta * T2_VCS
                * (1. / pow(m_BM_charge_e, 8.)) * sin(thetal)
                / (16 * pow(2. * M_PI, 3.) * pow(Qcal2, 2.)
                        * sqrt(1. + pow(2. * xB * Mnucleon, 2.) / Qcal2));
    }

    return xsec;

}

double DDVCSProcessTEST::int_squared(double Mnucleon, double t, double phi,
        double phil, double Q2, double eta, double xi, double y, double ytilde,
        double F1, double F2, double edgeFactor, double Kcap, double Ktildecap,
        const std::complex<double>& cffH, const std::complex<double>& cffE,
        const std::complex<double>& cffHL, const std::complex<double>& cffEL,
        const std::complex<double>& cffHtilde, double P1, double P2, double P3,
        double P4, double charge_e, int beamSign, int polariz,
        double lambda) const {

    //Fourier coeffs
    double cc1[4][3] = { 0. }, ss1[4][3] = { 0. }, cs1[4][3] = { 0. },
            sc1[4][3] = { 0. };
    double cc2[4][3] = { 0. }, ss2[4][3] = { 0. }, cs2[4][3] = { 0. },
            sc2[4][3] = { 0. };

    //Universal electric- and magnetic-like form factors (eq. (118) from Belitsky2003):
    std::complex<double> CV_F, CV_FL, CA_F;

    CV_F = F1 * cffH - t * F2 * cffE / pow(2. * Mnucleon, 2.);
    CV_FL = F1 * cffHL - t * F2 * cffEL / pow(2. * Mnucleon, 2.);
    CA_F = eta * (F1 + F2) * cffHtilde;

    //BH_1, VCS interference at LT (eqs 119-126 in Belitsky2003)
    cc1[0][1] = -8. * Ktildecap * (1. - y) * (2. - y) * (2. - ytilde)
            * ((xi + eta) / (-eta))
            * real(CV_F + CA_F - (xi - eta) * CV_FL / xi);
    cc1[1][0] = 8. * Kcap
            * real(
                    (2. - 2. * y + pow(y, 2.))
                            * (2. - 2. * ytilde + pow(ytilde, 2.))
                            * ((-xi / eta) * CV_F - CA_F)
                            - 8. * (1. - y) * (1. - ytilde)
                                    * (pow(xi, 2.) - pow(eta, 2.)) * CV_FL
                                    / (-eta * xi));
    cc1[1][2] = 16. * Kcap * (1. - y) * (1. - ytilde) * ((xi - eta) / xi)
            * real(CV_F + CA_F);
    cc1[2][1] = -8. * Ktildecap * (1. - y) * (2. - y) * (2. - ytilde)
            * ((xi + eta) / (-eta))
            * real(CV_F - CA_F - (xi + eta) * CV_FL / xi);
    cc1[3][2] = 16. * Kcap * (1. - y) * (1. - ytilde) * ((xi + eta) / (-eta))
            * real(CV_F - CA_F);

    cs1[0][1] = -8. * lambda * Ktildecap * y * (1. - y) * (2. - ytilde)
            * ((xi + eta) / (-eta))
            * real(CV_F + CA_F + (xi - eta) * CV_FL / xi);

    sc1[1][0] = -8. * lambda * Kcap * y * (2. - y)
            * (2. - 2. * ytilde + pow(ytilde, 2.))
            * imag(CV_F + xi * CA_F / eta);
    sc1[2][1] = 8. * lambda * Ktildecap * y * (1. - y) * (2. - ytilde)
            * ((xi + eta) / (-eta))
            * imag(CV_F - CA_F + (xi + eta) * CV_FL / xi);

    cs1[2][1] = -sc1[2][1];

    ss1[1][2] = cc1[1][2];
    ss1[2][1] = cc1[2][1];
    ss1[3][2] = cc1[3][2];

    //BH_2, VCS interference at LT (see eqs 127 and 128 in Belitsky2003)

    //I apply the relation cc2 = cc1 upon y <-> ytilde, xi -> -xi, which implies: CA_F -> -CA_F, Ktildecap <-> Kcap because Kcap(-xi, y = ytilde) = Ktildecap(xi, ytilde)

    double ySave, ytildeSave, xiSave, KcapSave, KtildecapSave;
    std::complex<double> CA_FSave;
    //We store the original values of y, ytilde, xi, CA_F and Ks since I'll need them later
    ySave = y;
    ytildeSave = ytilde;
    xiSave = xi;
    CA_FSave = CA_F;
    KcapSave = Kcap; //evaluated at xi and y
    KtildecapSave = Ktildecap; //evaluated at xi and ytilde

    y = ytildeSave;
    ytilde = ySave;
    xi = -xiSave;
    CA_F = -CA_FSave;
    Kcap = KtildecapSave;
    Ktildecap = KcapSave;

    cc2[0][1] = -8. * Ktildecap * (1. - y) * (2. - y) * (2. - ytilde)
            * ((xi + eta) / (-eta))
            * real(CV_F + CA_F - (xi - eta) * CV_FL / xi);
    cc2[1][0] = 8. * Kcap
            * real(
                    (2. - 2. * y + pow(y, 2.))
                            * (2. - 2. * ytilde + pow(ytilde, 2.))
                            * ((-xi / eta) * CV_F - CA_F)
                            - 8. * (1. - y) * (1. - ytilde)
                                    * (pow(xi, 2.) - pow(eta, 2.)) * CV_FL
                                    / (-eta * xi));
    cc2[1][2] = 16. * Kcap * (1. - y) * (1. - ytilde) * ((xi - eta) / xi)
            * real(CV_F + CA_F);
    cc2[2][1] = -8. * Ktildecap * (1. - y) * (2. - y) * (2. - ytilde)
            * ((xi + eta) / (-eta))
            * real(CV_F - CA_F - (xi + eta) * CV_FL / xi);
    cc2[3][2] = 16. * Kcap * (1. - y) * (1. - ytilde) * ((xi + eta) / (-eta))
            * real(CV_F - CA_F);

    //cs2_01, sc2_21,10 carry extra factors and only change xi -> -xi: CA_F -> -CA_F and Ktildecap(-xi, ytilde) = sqrt((ytilde - 1)/(1 - y))·Kcap(xi, y)

    y = ySave;
    ytilde = ytildeSave;
    Kcap = sqrt((1. - y) / (ytilde - 1.)) * KtildecapSave; //Kcap(-xi, y)
    Ktildecap = sqrt((ytilde - 1.) / (1. - y)) * KcapSave; //Ktildecap(-xi, ytilde)

    cs2[0][1] = sqrt((y - 1.) / (1. - ytilde))
            * (-8. * lambda * Ktildecap * y * (1. - y) * (2. - ytilde)
                    * ((xi + eta) / (-eta))
                    * real(CV_F + CA_F + (xi - eta) * CV_FL / xi));
    sc2[2][1] = sqrt((y - 1.) / (1. - ytilde))
            * (8. * lambda * Ktildecap * y * (1. - y) * (2. - ytilde)
                    * ((xi + eta) / (-eta))
                    * imag(CV_F - CA_F + (xi + eta) * CV_FL / xi));
    sc2[1][0] = -sqrt((y - 1.) / (1. - ytilde))
            * (-8. * lambda * Kcap * y * (2. - y)
                    * (2. - 2. * ytilde + pow(ytilde, 2.))
                    * imag(CV_F + xi * CA_F / eta));

    cs2[2][1] = -sc2[2][1];
    ss2[1][2] = cc2[1][2];
    ss2[2][1] = cc2[2][1];
    ss2[3][2] = cc2[3][2];

    //Returning all variables to original values:
    xi = xiSave;
    CA_F = CA_FSave;
    Kcap = KcapSave;
    Ktildecap = KtildecapSave;

    double prefactor;
    prefactor = edgeFactor * 2. * xi * pow(charge_e, 8.)
            / ((1. + eta) * pow(y * ytilde, 3.) * Q2 * t); //factor that precedes the whole Fourier series in eq 98 from Belitsky2003

    int n, m;
    double T2_int, series1 = 0., series2 = 0.;
    for (n = 0; n < 4; n++) {
        for (m = 0; m < 3; m++) {

            series1 += (cc1[n][m] * cos(m * phil) + cs1[n][m] * sin(m * phil))
                    * cos(n * phi)
                    + (sc1[n][m] * cos(m * phil) + ss1[n][m] * sin(m * phil))
                            * sin(n * phi); //series with coeffs (c, s)^1 in eq 98 from Belitsky2003
            series2 += (cc2[n][m] * cos(m * phi) + cs2[n][m] * sin(m * phi))
                    * cos(n * phil)
                    + (sc2[n][m] * cos(m * phi) + ss2[n][m] * sin(m * phi))
                            * sin(n * phil); //series with (c, s)^2 in eq 98 from Belitsky2003

        };
    };

    //Final squared amplitude for interference between VCS and BH diagrams as in eq 98 in Belitsky2003:
    T2_int = prefactor
            * (-(beamSign * ytilde / (P1 * P2)) * series1
                    + (y / (P3 * P4)) * series2);

    return T2_int;
}

double DDVCSProcessTEST::crossSectionInterf(double Ebeam, double Mnucleon,
        double xB, double t, double Qcal2, double Mll2, double phi, double phil,
        double thetal, int beamSign, int polariz, double lambda) const {

    //We change to Belitsky's phi: LHS is phi in Belitsky2003 notation, RHS' phi is in Trento's that must be the input in crossSection()
    phi = M_PI - phi;

    //phi entering these expressions is in Belitsky's convention already:
    double T2_INT = int_squared(Mnucleon, t, phi, phil, m_BM_Q2, m_BM_eta,
            m_BM_xi, m_BM_y, m_BM_ytilde, m_BM_F1, m_BM_F2, m_BM_edgeFactor,
            m_BM_Kcap, m_BM_Ktildecap, m_cffH, m_cffE, m_cffHL, m_cffEL,
            m_cffHt, m_BM_P1, m_BM_P2, m_BM_P3, m_BM_P4, m_BM_charge_e,
            beamSign, polariz, lambda);

    //7-(or 8- if polarized target)fold differential cross-section in pbarn/GeV^6 (ALL ENERGY QUANTITIES IN GeV !!)
    double xsec;

    if (polariz == 0) {

        xsec = pow(Constant::FINE_STRUCTURE_CONSTANT, 4.) * xB * pow(m_BM_y, 2.)
                * m_BM_beta * T2_INT * (1. / pow(m_BM_charge_e, 8.))
                * sin(thetal)
                / (16 * pow(2. * M_PI, 3.) * pow(Qcal2, 2.)
                        * sqrt(1. + pow(2. * xB * Mnucleon, 2.) / Qcal2));

    } else {

        xsec = (1. / (2. * M_PI)) * pow(Constant::FINE_STRUCTURE_CONSTANT, 4.)
                * xB * pow(m_BM_y, 2.) * m_BM_beta * T2_INT
                * (1. / pow(m_BM_charge_e, 8.)) * sin(thetal)
                / (16 * pow(2. * M_PI, 3.) * pow(Qcal2, 2.)
                        * sqrt(1. + pow(2. * xB * Mnucleon, 2.) / Qcal2));
    }

    return xsec;

}

void DDVCSProcessTEST::computeInternalVariables(double ml, double Ebeam,
        double Mnucleon, double xB, double t, double Qcal2, double Mll2,
        double phi, double phil, double thetal) {

    //We change to Belitsky's phi: LHS is phi in Belitsky2003 notation, RHS' phi is in Trento's that must be the input in crossSection()
    phi = M_PI - phi;

    //phil, thetal are polar coord. of lminus 4-vector in the CM-frame of the (created) lepton pair

    //-----------------------------------------------
    //For the following, cf. eqs (2) to (5) in overleaf
    //Basic kinematical variables:
    m_BM_Q2 = 0.5 * (Qcal2 - Mll2 + t / 2.); //Q2 is the squared of the incoming and outgoing photon momenta's average
    m_BM_eta = (Qcal2 + Mll2) / (2. * Qcal2 / xB - Qcal2 - Mll2 + t);
    m_BM_xi = m_BM_eta * 2. * m_BM_Q2 / (Qcal2 + Mll2);
    m_BM_y = Qcal2 / (Ebeam * 2. * xB * Mnucleon);
    m_BM_ytilde = 2. / (1. + cos(thetal));
    m_BM_ytildePlus = 2. / (1 - cos(thetal)); // ytilde under exchange thetal -> pi - thetal (polar coord. for lplus 4-vector)
//    m_BM_F1 = (4. * pow(Mnucleon, 2.) - t * 2.7928) * pow((1. - t / 0.71), -2.)
//            / (4. * pow(Mnucleon, 2.) - t); //EM form factor F1 for proton, 2.7928 = Born's magneton for proton

    m_BM_F1 = 0.;//DEBUG

    m_BM_F2 = 4. * pow(Mnucleon, 2.) * (2.7928 - 1.) * pow((1. - t / 0.71), -2.)
     / (4. * pow(Mnucleon, 2.) - t); //EM form factor F2 for proton

    m_BM_charge_e = sqrt(Constant::FINE_STRUCTURE_CONSTANT * 4. * M_PI);

    m_BM_tmin = -4. * pow(Mnucleon * m_BM_eta, 2.) / (1. - m_BM_eta * m_BM_eta);
    m_BM_tmax = -m_BM_Q2 * (1. - m_BM_eta * m_BM_eta)
            / (m_BM_xi * (1. - m_BM_xi));

    //Prefactor in amplitude terms that must be changed when y -> 1
    m_BM_edgeFactor = pow(1. + m_BM_eta, 2.)
            / (m_BM_eta * m_BM_eta - m_BM_xi * m_BM_xi); //for y << 1, eq 101 in Belitsky2003

    //Functions that appear in the scalar products
    m_BM_Kcap = (0.5 / m_BM_eta) * sqrt(-m_BM_xi * t / m_BM_Q2)
            * sqrt(1. - m_BM_tmin / t) * sqrt((1. - m_BM_eta) / (1. + m_BM_eta))
            * sqrt((1. - m_BM_y) * (m_BM_xi + m_BM_eta));
    m_BM_Ktildecap = (0.5 / m_BM_eta) * sqrt(-m_BM_xi * t / m_BM_Q2)
            * sqrt(1. - m_BM_tmin / t) * sqrt((1. - m_BM_eta) / (1. + m_BM_eta))
            * sqrt((1. - m_BM_ytilde) * (m_BM_xi - m_BM_eta));
    m_BM_KtildecapPlus = (0.5 / m_BM_eta) * sqrt(-m_BM_xi * t / m_BM_Q2)
            * sqrt(1. - m_BM_tmin / t) * sqrt((1. - m_BM_eta) / (1. + m_BM_eta))
            * sqrt((1. - m_BM_ytildePlus) * (m_BM_xi - m_BM_eta)); //Ktildecap under exchange thetal -> pi - thetal (polar coord. for lplus 4-vector)

            //Basic scalar products of momenta
    m_BM_kDelta = -m_BM_Q2 * m_BM_eta * (1. - 2. * m_BM_Kcap * cos(M_PI + phi))
            / (m_BM_y * m_BM_xi);
    m_BM_lminusDelta = -m_BM_Q2 * m_BM_eta
            * (1. + 2. * m_BM_Ktildecap * cos(phil)) / (m_BM_ytilde * m_BM_xi);
    m_BM_lplusDelta = -m_BM_Q2 * m_BM_eta
            * (1. - 2. * m_BM_KtildecapPlus * cos(phil))
            / (m_BM_ytildePlus * m_BM_xi); // lplus is the same as lminus under thetal -> pi - thetal and phi -> phi + pi (that's why we use ytildePlus and KtildecapPlus)

    //BH denominators
    m_BM_P1 = (2. * m_BM_kDelta + Qcal2 + Mll2) / (Qcal2 + Mll2);
    m_BM_P2 = (t - 2. * m_BM_kDelta) / (Qcal2 + Mll2);
    m_BM_P3 = -(pow(ml, 2.) + 2. * m_BM_lplusDelta + t) / (Qcal2 + Mll2);
    m_BM_P4 = -(pow(ml, 2.) + 2. * m_BM_lminusDelta + t) / (Qcal2 + Mll2);

    //beta variable
    m_BM_beta = sqrt(1. - pow(2. * ml, 2.) / Mll2);

    m_BM_xBmin = Qcal2 / (2. * Mnucleon * Ebeam);
    m_BM_xBmax = 1.;
    m_BM_Qcal2min = 4. * xB * ml * (ml + Mnucleon) / (1. - xB);
    m_BM_Qcal2max = 4. * pow(Ebeam, 2.) * Mnucleon * xB
            / (Mnucleon * xB + 2. * Ebeam);
    m_BM_Mll2min = 4. * pow(ml, 2.);
    m_BM_Mll2max = pow(
            sqrt(pow(Mnucleon, 2.) + Qcal2 * (1. / xB - 1.)) - Mnucleon, 2.);

    //get CFFs

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

} /* namespace PARTONS */
