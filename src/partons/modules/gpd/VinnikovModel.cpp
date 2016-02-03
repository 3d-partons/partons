#include "../../../../include/partons/modules/gpd/VinnikovModel.h"

#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/utils/ParameterList.h"

const unsigned int VinnikovModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new VinnikovModel("VinnikovModel"));

const double VinnikovModel::EPS_BETR = 1.E-8;
const int VinnikovModel::VinnikovModel::SIMP_INT = 100;

VinnikovModel::VinnikovModel(const std::string &className) :
        GPDModule(className) {

    n_par = 0;
    param = NULL;

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Ht, &GPDModule::computeHt));
}

VinnikovModel::~VinnikovModel() {

    if (param != NULL)
        delete param;
}

VinnikovModel* VinnikovModel::clone() const {
    return new VinnikovModel(*this);
}

void VinnikovModel::configure(ParameterList parameters) {
}

std::string VinnikovModel::toString() {
    return GPDModule::toString();
}

VinnikovModel::VinnikovModel(const VinnikovModel& other) :
        GPDModule(other) {

    n_par = other.n_par;
    param = other.param;
}

void VinnikovModel::isModuleWellConfigured() {

    if (n_par == 0)
        throw std::runtime_error("[VinnikovModel] Unknown n_par");

    if (param == NULL)
        throw std::runtime_error("[VinnikovModel] param is NULL");

    GPDModule::isModuleWellConfigured();
}

void VinnikovModel::initModule() {

    n_par = 1;
    param = new double[6 * n_par];

    for (int i = 0; i < 6 * n_par; i++)
        param[i] = (i == 0) ? (2.) : (1.);

    GPDModule::initModule();
}

PartonDistribution VinnikovModel::computeH() {

    double g, uVal, uSea, dVal, dSea, sSea, uValMx, dValMx;

    g = gpdh(0, m_x, m_xi, m_t, n_par, param);

    uVal = gpdh(-1, m_x, m_xi, m_t, n_par, param);
    uSea = gpdh(1, m_x, m_xi, m_t, n_par, param);

    dVal = gpdh(-2, m_x, m_xi, m_t, n_par, param);
    dSea = gpdh(2, m_x, m_xi, m_t, n_par, param);

    sSea = gpdh(3, m_x, m_xi, m_t, n_par, param);

    uValMx = gpdh(-1, -m_x, m_xi, m_t, n_par, param);
    dValMx = gpdh(-2, -m_x, m_xi, m_t, n_par, param);

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);
    quarkDistribution_s.setQuarkDistribution(sSea);

    quarkDistribution_u.setQuarkDistributionPlus(uVal - uValMx + 2 * uSea);
    quarkDistribution_d.setQuarkDistributionPlus(dVal - dValMx + 2 * dSea);
    quarkDistribution_s.setQuarkDistributionPlus(2 * sSea);

    quarkDistribution_u.setQuarkDistributionMinus(uVal + uValMx);
    quarkDistribution_d.setQuarkDistributionMinus(dVal + dValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    GluonDistribution gluonDistribution(g);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

PartonDistribution VinnikovModel::computeHt() {

    double g, uVal, uSea, dVal, dSea, sSea, uValMx, dValMx;

    g = gpdh_pol(0, m_x, m_xi, m_t, n_par, param);

    uVal = gpdh_pol(-1, m_x, m_xi, m_t, n_par, param);
    uSea = gpdh_pol(1, m_x, m_xi, m_t, n_par, param);

    dVal = gpdh_pol(-2, m_x, m_xi, m_t, n_par, param);
    dSea = gpdh_pol(2, m_x, m_xi, m_t, n_par, param);

    sSea = gpdh_pol(3, m_x, m_xi, m_t, n_par, param);

    uValMx = gpdh_pol(-1, -m_x, m_xi, m_t, n_par, param);
    dValMx = gpdh_pol(-2, -m_x, m_xi, m_t, n_par, param);

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);
    quarkDistribution_s.setQuarkDistribution(sSea);

    quarkDistribution_u.setQuarkDistributionPlus(uVal - uValMx + 2 * uSea);
    quarkDistribution_d.setQuarkDistributionPlus(dVal - dValMx + 2 * dSea);
    quarkDistribution_s.setQuarkDistributionPlus(2 * sSea);

    quarkDistribution_u.setQuarkDistributionMinus(uVal + uValMx);
    quarkDistribution_d.setQuarkDistributionMinus(dVal + dValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    GluonDistribution gluonDistribution(g);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

// GPD H =========================================================================================

// The program calculates GPD H at Q^2 = 4 GeV^2 
// at given x, xi and t for the following partons:
// i_part = 0 : gluons
// i_part = 1 : u-quarks
// i_part = 2 : d-quarks
// i_part = 3 : s-quarks

// double distribution model as in Musatov, Radyushkin, hep-ph/9905376
// and Goeke, Polyakov, Vanderhaeghen, hep-ph/0106012
//
// dependence on proton momentum transfer t for valence distributions
// from Diehl, Feldmann, Jakob, Kroll, hep-ph/0408173
// for sea quark and gluon distributions no t dependence is implemented

// The value of the profile parameters for gluons, sea and
// valence quarks is given as folows:
// param[0] = b(gluon),
// param[1] = b(u_val),
// param[2] = b(d_val),
// param[3] = b(u_sea),
// param[4] = b(d_sea),
// param[5] = b(s_sea),

double VinnikovModel::gpdh(int const i_part, double x, double const xi,
        double const t, int const n_par, double* const param) {
    /*
     if(fabs(x-xi) < 10.0*EPS_BETR)
     {
     double f1 = gpdh(i_part, xi-10.0*EPS_BETR,xi,t,n_par,param);
     double f2 = gpdh(i_part, xi+10.0*EPS_BETR,xi,t,n_par,param);
     return f1 + (f2-f1)/(20.0*EPS_BETR)*(x-xi+10.0*EPS_BETR);
     }
     if(fabs(x+xi) < 10.0*EPS_BETR)
     {
     double f1 = gpdh(i_part, -xi-10.0*EPS_BETR,xi,t,n_par,param);
     double f2 = gpdh(i_part, -xi+10.0*EPS_BETR,xi,t,n_par,param);
     return f1 + (f2-f1)/(20.0*EPS_BETR)*(x+xi+10.0*EPS_BETR);
     }
     */

    if (x == xi)
        x -= EPS_BETR;
    if (x == -xi)
        x += EPS_BETR;

    if (x < -1.0) {
        if ((x + EPS_BETR) >= -1.0)
            return 0.0;
        printf(
                "gpdh (gpdh.c): YOUR X SEEMS TO BE < -1\n (x = %20.10e )IT IS ERROR\n",
                x);
        return 0.0;
    }

    if (x < -xi) {
        double bmin = (-x - xi) / (1.0 - xi);
        double bmax = (-x + xi) / (1.0 + xi);
        double h = dd_int_simp(bmin, bmax, &VinnikovModel::dd1, i_part, x, xi,
                t, n_par, param);
        return h / xi;
    }

    if (x <= 0.0) {
        double bmax1 = (xi + x) / (1.0 + xi);
        double bmax2 = (xi - x) / (1.0 + xi);
        double h1 = dd_int_simp(0.0, bmax1, &VinnikovModel::dd2, i_part, x, xi,
                t, n_par, param);
        double h2 = dd_int_simp(bmax1, bmax2, &VinnikovModel::dd1, i_part, x,
                xi, t, n_par, param);
        return (h1 + h2) / xi;
    }
    if (x < xi) {
        double bmax1 = (xi - x) / (1.0 + xi);
        double bmax2 = (xi + x) / (1.0 + xi);
        double h1 = dd_int_simp(0.0, bmax1, &VinnikovModel::dd2, i_part, x, xi,
                t, n_par, param);
        double h2 = dd_int_simp(bmax1, bmax2, &VinnikovModel::dd3, i_part, x,
                xi, t, n_par, param);
        return (h1 + h2) / xi;
    }
    if (x <= 1.0) {
        double bmin = (x - xi) / (1.0 - xi);
        double bmax = (x + xi) / (1.0 + xi);
        double h = dd_int_simp(bmin, bmax, &VinnikovModel::dd3, i_part, x, xi,
                t, n_par, param);
        return h / xi;
    }
    if ((x - EPS_BETR) <= 1.0)
        return 0.0;
    printf(
            "gpdh (gpdh.c): YOUR X SEEMS TO BE > 1\n (x = %20.10e )IT IS ERROR\n",
            x);
    return 0.0;
}

double VinnikovModel::dd1(int const i_part, double const beta, double const x,
        double const xi, double const t, int const n_par, double* const param) {
    double prof_b;
    double alpha = (x + beta) / xi;
    if (0 == i_part)
        prof_b = param[0];
    else
        prof_b = param[n_par * (abs(i_part) + 2)];

    if (alpha >= (1.0 - beta)) {
        if ((alpha + beta - 1.0) > EPS_BETR)
            printf("dd1 (gpdh.c): WARNING1, beta = %15.7e, alpha = %15.7e\n",
                    beta, alpha);
        return 0.0;
    }
    if (alpha <= (-1.0 + beta)) {
        if ((alpha - beta + 1.0) < (-EPS_BETR))
            printf("dd1 (gpdh.c): WARNING2, beta = %15.7e, alpha = %15.7e\n",
                    beta, alpha);
        return 0.0;
    }

    double pf = prof_funct(prof_b, beta, alpha);

    if (0 == i_part)
        return pf * glu(beta, t);
    if (-1 == i_part)
        return 0.;
    if (1 == i_part)
        return -pf * usea(beta, t);
    if (-2 == i_part)
        return 0.;
    if (2 == i_part)
        return -pf * dsea(beta, t);
    if (3 == i_part)
        return -pf * ssea(beta, t);

    printf("dd1 (gpdh.c): WARNING3, ipart = %5d !={0,-1,1,-2,2,3}", i_part);
    return 0.0;
}

double VinnikovModel::dd2(int const i_part, double const beta, double const x,
        double const xi, double const t, int const n_par, double* const param) {
    double prof_b_val = param[n_par * abs(i_part)];
    double prof_b_sea = param[n_par * (abs(i_part) + 2)];

    double alpha1 = (x + beta) / xi;
    double alpha2 = (x - beta) / xi;

    if (alpha1 >= 1.0 - beta) {
        if ((alpha1 - 1.0 + beta) > EPS_BETR)
            printf("dd2 (gpdh.c): WARNING1, beta = %15.7e, alpha = %15.7e\n",
                    beta, alpha1);
        return 0.0;
    }
    if (alpha1 <= -1.0 + beta) {
        if ((alpha1 + 1.0 - beta) < -EPS_BETR)
            printf("dd2 (gpdh.c): WARNING2, beta = %15.7e, alpha = %15.7e\n",
                    beta, alpha1);
        return 0.0;
    }
    if (alpha2 >= 1.0 - beta) {
        if ((alpha2 - 1.0 + beta) > EPS_BETR)
            printf("dd2 (gpdh.c): WARNING3, beta = %15.7e, alpha = %15.7e\n",
                    beta, alpha2);
        return 0.0;
    }
    if (alpha2 <= -1.0 + beta) {
        if ((alpha2 + 1.0 - beta) < -EPS_BETR)
            printf("dd2 (gpdh.c): WARNING4, beta = %15.7e, alpha = %15.7e\n",
                    beta, alpha2);
        return 0.0;
    }
    double pf1_val = prof_funct(prof_b_val, beta, alpha1);
    double pf2_val = prof_funct(prof_b_val, beta, alpha2);
    double pf1_sea = prof_funct(prof_b_sea, beta, alpha1);
    double pf2_sea = prof_funct(prof_b_sea, beta, alpha2);

    if (0 == i_part)
        return (pf2_val + pf1_val) * glu(beta, t);

    if (-1 == i_part)
        return pf2_val * uval(beta, t);

    if (1 == i_part)
        return (pf2_sea - pf1_sea) * usea(beta, t);

    if (-2 == i_part)
        return pf2_val * dval(beta, t);

    if (2 == i_part)
        return (pf2_sea - pf1_sea) * dsea(beta, t);

    if (3 == i_part)
        return (pf2_sea - pf1_sea) * ssea(beta, t);

    printf("dd2 (gpdh.c): WARNING5, ipart = %5d !={0,-1,1,-2,2,3}", i_part);
    return 0.0;
}

double VinnikovModel::dd3(int const i_part, double const beta, double const x,
        double const xi, double const t, int const n_par, double* const param) {
    double prof_b_val = param[n_par * abs(i_part)];
    double prof_b_sea = param[n_par * (abs(i_part) + 2)];

    double alpha = (x - beta) / xi;

    if (alpha >= (1.0 - beta)) {
        if ((alpha + beta - 1.0) > EPS_BETR)
            printf("dd3 (gpdh.c): WARNING1, beta = %15.7e, alpha = %15.7e\n",
                    beta, alpha);
        return 0.0;
    }
    if (alpha <= (-1.0 + beta)) {
        if ((alpha + 1.0 - beta) < (-EPS_BETR))
            printf("dd3 (gpdh.c): WARNING2, beta = %15.7e, alpha = %15.7e\n",
                    beta, alpha);
        return 0.0;
    }

    double pf_val = prof_funct(prof_b_val, beta, alpha);
    double pf_sea = prof_funct(prof_b_sea, beta, alpha);

    if (0 == i_part)
        return pf_val * glu(beta, t);

    if (-1 == i_part)
        return pf_val * uval(beta, t);

    if (1 == i_part)
        return pf_sea * usea(beta, t);

    if (-2 == i_part)
        return pf_val * dval(beta, t);

    if (2 == i_part)
        return pf_sea * dsea(beta, t);

    if (3 == i_part)
        return pf_sea * ssea(beta, t);

    printf("dd3 (gpdh.c): WARNING3, ipart = %5d !={0,-1,1,-2,2,3}", i_part);
    return 0.0;
}

double VinnikovModel::uval(double const x, double const t) {
    if (x <= 0.0) {
        printf("uval (gpdh.c): ERROR: x = %12.3e < 0\n", x);
        return 0.0;
    }
    // Diehl, Feldmann, Jakob, Kroll parameters
    double A_q = 1.22;
    double B_q = 0.59;
    double alp = 0.9;

    double fq = -alp * (1.0 - x) * (1.0 - x) * (1.0 - x) * log(x)
            + B_q * (1.0 - x) * (1.0 - x) * (1.0 - x)
            + A_q * x * (1.0 - x) * (1.0 - x);

    // CTEQ6L at Q2 = 4 GeV^2
    double a0 = 1.7727;
    double a1 = -1.0 + 0.5308;
    double a2 = 3.0142;
    double a3 = -2.0460;
    double a4 = exp(1.8077);
    double a5 = 1.1874;

    return a0
            * exp(
                    log(x) * a1 + log(1.0 - x) * a2 + a3 * x
                            + log(1.0 + a4 * x) * a5 + t * fq);
}

double VinnikovModel::usea(double const x, double const t) {
    if (x <= 0.0 || x > 1.0) {
        printf("usea (gpdh.c): ERROR: x = %12.3e < 0\n", x);
        return 0.0;
    }
    if ((1.0 - x) < EPS_BETR)
        return 0.0;

    //CTEQ6L at Q2 = 2 GeV^2
    double a0 = 0.12650 * 0.5;
    double a1 = -1.25278;
    double a2 = 6.4524;
    double a3 = -1.2810;
    double a4 = exp(4.7667);
    double a5 = 0.60389;

    double b0 = 1.2379;
    double b1 = 2.0374;
    double b2 = 0.23531;
    double b3 = 0.61096;
    double b4 = 23.934;

    return a0
            * exp(
                    log(x) * a1 + log(1.0 - x) * a2 + a3 * x
                            + log(1.0 + a4 * x) * a5)
            + 0.5 * (b0 * exp(b1 * log(x)) - b2 * exp(b3 * log(x)))
                    * exp(b4 * log(1.0 - exp(2.6 * log(x))));
}

double VinnikovModel::dval(double const x, double const t) {
    if (x <= 0.0 || x > 1.0) {
        printf("dval (gpdh.c): ERROR: x = %12.3e < 0\n", x);
        return 0.0;
    }
    if ((1.0 - x) < EPS_BETR)
        return 0.0;

    // Diehl, Feldmann, Jakob, Kroll parameters
    double A_q = 2.59;
    double B_q = 0.59;
    double alp = 0.9;

    double fq = -alp * (1.0 - x) * (1.0 - x) * (1.0 - x) * log(x)
            + B_q * (1.0 - x) * (1.0 - x) * (1.0 - x)
            + A_q * x * (1.0 - x) * (1.0 - x);

    // CTEQ6L at Q2 = 4 GeV^2
    double a0 = 1.4521;
    double a1 = -1.0 + 0.58542;
    double a2 = 6.5839;
    double a3 = 3.3606;
    double a4 = exp(0.2499);
    double a5 = 0.84218;

    return a0
            * exp(
                    log(x) * a1 + log(1.0 - x) * a2 + a3 * x
                            + log(1.0 + a4 * x) * a5 + t * fq);
}

double VinnikovModel::dsea(double const x, double const t) {
    if (x <= 0.0 || x > 1.0) {
        printf("dsea (gpdh.c): ERROR: x = %12.3e < 0\n", x);
        return 0.0;
    }
    if ((1.0 - x) < EPS_BETR)
        return 0.0;

    //CTEQ6L at Q2 = 4 GeV^2
    double a0 = 0.12650 * 0.5;
    double a1 = -1.25278;
    double a2 = 6.4524;
    double a3 = -1.2810;
    double a4 = exp(4.7667);
    double a5 = 0.60389;

    double b0 = 1.2379;
    double b1 = 2.0374;
    double b2 = 0.23531;
    double b3 = 0.61096;
    double b4 = 23.934;

    return a0
            * exp(
                    log(x) * a1 + log(1.0 - x) * a2 + a3 * x
                            + log(1.0 + a4 * x) * a5)
            - 0.5 * (b0 * exp(b1 * log(x)) - b2 * exp(b3 * log(x)))
                    * exp(b4 * log(1.0 - exp(2.6 * log(x))));
}

double VinnikovModel::ssea(double const x, double const t) {
    if (x <= 0.0 || x > 1.0) {
        printf("ssea (gpdh.c): ERROR: x = %12.3e < 0\n", x);
        return 0.0;
    }
    if ((1.0 - x) < EPS_BETR)
        return 0.0;

    //CTEQ6L at Q2 = 4 GeV^2
    double a0 = 0.048779;
    double a1 = -1.22023;
    double a2 = 5.6878;
    double a3 = -1.6059;
    double a4 = 4.3377;
    double a5 = 0.16950;

    return a0
            * exp(
                    log(x) * a1 + log(1.0 - x) * a2 + a3 * x
                            + log(1.0 + a4 * x) * a5);

    return 0.0;
}

double VinnikovModel::glu(double const x, double const t) {
    if (x <= 0.0 || x > 1.0) {
        printf("glu (gpdh.c): ERROR: x = %12.3e < 0\n", x);
        return 0.0;
    }
    if ((1.0 - x) < EPS_BETR)
        return 0.0;

    //CTEQ6L at Q2 = 4 GeV^2
    double a0 = 1.1669;
    double a1 = -0.16168;
    double a2 = 2.3112;
    double a3 = 8.7652;
    double a4 = exp(2.2851);
    double a5 = -3.6050;

    return a0
            * exp(
                    log(x) * a1 + log(1.0 - exp(2.0 / 3.0 * log(x))) * a2
                            + a3 * sqrt(x) + log(1.0 + a4 * x) * a5);
}

// GPD Ht ========================================================================================

// calculates H-tilde at Q^2 = 4 GeV^2 in double distribution model
// for valence distributions includes t-dependence
// see comments in gpdh.c

double VinnikovModel::gpdh_pol(int const i_part, double x, double const xi,
        double const t, int const n_par, double* const param) {
    if (x == xi)
        x -= EPS_BETR;
    if (x == -xi)
        x += EPS_BETR;

    if (x < -1.0) {
        if ((x + EPS_BETR) >= -1.0)
            return 0.0;
        printf(
                "gpdh_pol (gpdh_pol.c): YOUR X SEEMS TO BE < 1\n (x = %20.10e )IT IS ERROR\n",
                x);
        return 0.0;
    }

    if (x < -xi) {
        double bmin = (-x - xi) / (1.0 - xi);
        double bmax = (-x + xi) / (1.0 + xi);
        double h = dd_int_simp(bmin, bmax, &VinnikovModel::dd1_pol, i_part, x,
                xi, t, n_par, param);
        return h / xi;
    }

    if (x <= 0.0) {
        double bmax1 = (xi + x) / (1.0 + xi);
        double bmax2 = (xi - x) / (1.0 + xi);
        double h1 = dd_int_simp(0.0, bmax1, &VinnikovModel::dd2_pol, i_part, x,
                xi, t, n_par, param);
        double h2 = dd_int_simp(bmax1, bmax2, &VinnikovModel::dd1_pol, i_part,
                x, xi, t, n_par, param);
        return (h1 + h2) / xi;
    }
    if (x < xi) {
        double bmax1 = (xi - x) / (1.0 + xi);
        double bmax2 = (xi + x) / (1.0 + xi);
        double h1 = dd_int_simp(0.0, bmax1, &VinnikovModel::dd2_pol, i_part, x,
                xi, t, n_par, param);
        double h2 = dd_int_simp(bmax1, bmax2, &VinnikovModel::dd3_pol, i_part,
                x, xi, t, n_par, param);
        return (h1 + h2) / xi;
    }
    if (x <= 1.0) {
        double bmin = (x - xi) / (1.0 - xi);
        double bmax = (x + xi) / (1.0 + xi);
        double h = dd_int_simp(bmin, bmax, &VinnikovModel::dd3_pol, i_part, x,
                xi, t, n_par, param);
        return h / xi;
    }
    if ((x - EPS_BETR) <= 1.0)
        return 0.0;
    printf(
            "gpdh_pol (gpdh_pol.c): YOUR X SEEMS TO BE > 1\n (x = %20.10e )IT IS ERROR\n",
            x);
    return 0.0;
}

double VinnikovModel::dd1_pol(int const i_part, double const beta,
        double const x, double const xi, double const t, int const n_par,
        double* const param) {
    double alpha = (x + beta) / xi;
    double prof_b = param[n_par * abs(i_part)];

    if (alpha >= (1.0 - beta)) {
        if ((alpha + beta - 1.0) > EPS_BETR)
            printf(
                    "dd1_pol (gpdh_pol.c): WARNING1, beta = %15.7e, alpha = %15.7e\n",
                    beta, alpha);
        return 0.0;
    }
    if (alpha <= (-1.0 + beta)) {
        if ((alpha - beta + 1.0) < (-EPS_BETR))
            printf(
                    "dd1_pol (gpdh_pol.c): WARNING2, beta = %15.7e, alpha = %15.7e\n",
                    beta, alpha);
        return 0.0;
    }

    double pf = prof_funct(prof_b, beta, alpha);

    if (0 == i_part)
        return -pf * glu_pol(beta, t);
    if (-1 == i_part)
        return 0.;
    if (1 == i_part)
        return pf * usea_pol(beta, t);
    if (-2 == i_part)
        return 0.;
    if (2 == i_part)
        return pf * dsea_pol(beta, t);
    if (3 == i_part)
        return pf * ssea_pol(beta, t);

    printf("dd1_pol (gpdh_pol.c): WARNING3, ipart = %5d !={0,-1,1,-2,2,3}",
            i_part);
    return 0.0;
}

double VinnikovModel::dd2_pol(int const i_part, double const beta,
        double const x, double const xi, double const t, int const n_par,
        double* const param) {
    double prof_b_val = param[n_par * abs(i_part)];
    double prof_b_sea = param[n_par * (abs(i_part) + 2)];

    double alpha1 = (x + beta) / xi;
    double alpha2 = (x - beta) / xi;

    if (alpha1 >= 1.0 - beta) {
        if ((alpha1 - 1.0 + beta) > EPS_BETR)
            printf(
                    "dd2_pol (gpdh_pol.c): WARNING1, beta = %15.7e, alpha = %15.7e\n",
                    beta, alpha1);
        return 0.0;
    }
    if (alpha1 <= -1.0 + beta) {
        if ((alpha1 + 1.0 - beta) < -EPS_BETR)
            printf(
                    "dd2_pol (gpdh_pol.c): WARNING2, beta = %15.7e, alpha = %15.7e\n",
                    beta, alpha1);
        return 0.0;
    }
    if (alpha2 >= 1.0 - beta) {
        if ((alpha2 - 1.0 + beta) > EPS_BETR)
            printf(
                    "dd2_pol (gpdh_pol.c): WARNING3, beta = %15.7e, alpha = %15.7e\n",
                    beta, alpha2);
        return 0.0;
    }
    if (alpha2 <= -1.0 + beta) {
        if ((alpha2 + 1.0 - beta) < -EPS_BETR)
            printf(
                    "dd2_pol (gpdh_pol.c): WARNING4, beta = %15.7e, alpha = %15.7e\n",
                    beta, alpha2);
        return 0.0;
    }
    double pf1_val = prof_funct(prof_b_val, beta, alpha1);
    double pf2_val = prof_funct(prof_b_val, beta, alpha2);
    double pf1_sea = prof_funct(prof_b_sea, beta, alpha1);
    double pf2_sea = prof_funct(prof_b_sea, beta, alpha2);

    if (0 == i_part)
        return (pf2_val - pf1_val) * glu_pol(beta, t);

    if (-1 == i_part)
        return pf2_val * uval_pol(beta, t);

    if (1 == i_part)
        return (pf2_sea + pf1_sea) * usea_pol(beta, t);

    if (-2 == i_part)
        return pf2_val * dval_pol(beta, t);

    if (2 == i_part)
        return (pf2_sea + pf1_sea) * dsea_pol(beta, t);

    if (3 == i_part)
        return (pf2_sea + pf1_sea) * ssea_pol(beta, t);

    printf("dd2_pol (gpdh_pol.c): WARNING5, ipart = %5d !={0,-1,1,-2,2,3}",
            i_part);
    return 0.0;
}

double VinnikovModel::dd3_pol(int const i_part, double const beta,
        double const x, double const xi, double const t, int const n_par,
        double* const param) {
    double prof_b_val = param[n_par * abs(i_part)];
    double prof_b_sea = param[n_par * (abs(i_part) + 2)];

    double alpha = (x - beta) / xi;

    if (alpha >= (1.0 - beta)) {
        if ((alpha + beta - 1.0) > EPS_BETR)
            printf(
                    "dd3_pol (gpdh_pol.c): WARNING1, beta = %15.7e, alpha = %15.7e\n",
                    beta, alpha);
        return 0.0;
    }
    if (alpha <= (-1.0 + beta)) {
        if ((alpha + 1.0 - beta) < (-EPS_BETR))
            printf(
                    "dd3_pol (gpdh_pol.c): WARNING2, beta = %15.7e, alpha = %15.7e\n",
                    beta, alpha);
        return 0.0;
    }

    double pf_val = prof_funct(prof_b_val, beta, alpha);
    double pf_sea = prof_funct(prof_b_sea, beta, alpha);

    if (0 == i_part)
        return pf_val * glu_pol(beta, t);

    if (-1 == i_part)
        return pf_val * uval_pol(beta, t);

    if (1 == i_part)
        return pf_sea * usea_pol(beta, t);

    if (-2 == i_part)
        return pf_val * dval_pol(beta, t);

    if (2 == i_part)
        return pf_sea * dsea_pol(beta, t);

    if (3 == i_part)
        return pf_sea * ssea_pol(beta, t);

    printf("dd3_pol (gpdh_pol.c): WARNING3, ipart = %5d !={0,-1,1,-2,2,3}",
            i_part);
    return 0.0;
}

double VinnikovModel::uval_pol(double const x, double const t) {
    if (x <= 0.0) {
        printf("uval_pol (gpdh_pol.c): ERROR: x = %12.3e < 0\n", x);
        return 0.0;
    }
    // Diehl, Feldmann, Jakob, Kroll parameters
    double A_q = 1.22;
    double B_q = 0.59;
    double alp = 0.9;

    double fq = -alp * (1.0 - x) * (1.0 - x) * (1.0 - x) * log(x)
            + B_q * (1.0 - x) * (1.0 - x) * (1.0 - x)
            + A_q * x * (1.0 - x) * (1.0 - x);

    // Blumlein, Bottcher at Q2 = 4 GeV^2
    double eta = 0.926;
    double a = 0.294;
    double b = 3.167;
    double gam = 27.22;

    double coeff = (1.0 + gam * a / (a + b + 1.0)) * gammf(a) * gammf(b + 1.0)
            / gammf(a + b + 1.0);

    return eta / coeff * exp((a - 1.0) * log(x) + b * log(1.0 - x) + t * fq)
            * (1.0 + gam * x);
}

double VinnikovModel::usea_pol(double const x, double const t) {
    if (x <= 0.0 || x > 1.0) {
        printf("usea_pol (gpdh_pol.c): ERROR: x = %12.3e < 0\n", x);
        return 0.0;
    }
    if ((1.0 - x) < EPS_BETR)
        return 0.0;

    // Blumlein, Bottcher at Q2 = 4 GeV^2
    double eta = -0.447 / 6.0;
    double a = 0.424;
    double b = 8.93;
    double gam = 0.0;

    double coeff = (1.0 + gam * a / (a + b + 1.0)) * gammf(a) * gammf(b + 1.0)
            / gammf(a + b + 1.0);

    return eta / coeff * exp((a - 1.0) * log(x) + b * log(1.0 - x))
            * (1.0 + gam * x);
}

double VinnikovModel::dval_pol(double const x, double const t) {
    if (x <= 0.0 || x > 1.0) {
        printf("dval_pol (gpdh_pol.c): ERROR: x = %12.3e < 0\n", x);
        return 0.0;
    }
    if ((1.0 - x) < EPS_BETR)
        return 0.0;

    // Diehl, Feldmann, Jakob, Kroll parameters
    double A_q = 2.59;
    double B_q = 0.59;
    double alp = 0.9;

    double fq = -alp * (1.0 - x) * (1.0 - x) * (1.0 - x) * log(x)
            + B_q * (1.0 - x) * (1.0 - x) * (1.0 - x)
            + A_q * x * (1.0 - x) * (1.0 - x);

    // Blumlein, Bottcher at Q2 = 4 GeV^2
    double eta = -0.341;
    double a = 0.254;
    double b = 3.42;
    double gam = 19.06;

    double coeff = (1.0 + gam * a / (a + b + 1.0)) * gammf(a) * gammf(b + 1.0)
            / gammf(a + b + 1.0);

    return eta / coeff * exp((a - 1.0) * log(x) + b * log(1.0 - x) + t * fq)
            * (1.0 + gam * x);
}

double VinnikovModel::dsea_pol(double const x, double const t) {
    if (x <= 0.0 || x > 1.0) {
        printf("dsea_pol (gpdh_pol.c): ERROR: x = %12.3e < 0\n", x);
        return 0.0;
    }
    if ((1.0 - x) < EPS_BETR)
        return 0.0;

    // Blumlein, Bottcher at Q2 = 4 GeV^2
    double eta = -0.447 / 6.0;
    double a = 0.424;
    double b = 8.93;
    double gam = 0.0;

    double coeff = (1.0 + gam * a / (a + b + 1.0)) * gammf(a) * gammf(b + 1.0)
            / gammf(a + b + 1.0);

    return eta / coeff * exp((a - 1.0) * log(x) + b * log(1.0 - x))
            * (1.0 + gam * x);
}

double VinnikovModel::ssea_pol(double const x, double const t) {
    if (x <= 0.0 || x > 1.0) {
        printf("ssea_pol (gpdh_pol.c): ERROR: x = %12.3e < 0\n", x);
        return 0.0;
    }
    if ((1.0 - x) < EPS_BETR)
        return 0.0;

    // Blumlein, Bottcher at Q2 = 4 GeV^2
    double eta = -0.447 / 6.0;
    double a = 0.424;
    double b = 8.93;
    double gam = 0.0;

    double coeff = (1.0 + gam * a / (a + b + 1.0)) * gammf(a) * gammf(b + 1.0)
            / gammf(a + b + 1.0);

    return eta / coeff * exp((a - 1.0) * log(x) + b * log(1.0 - x))
            * (1.0 + gam * x);
}

double VinnikovModel::glu_pol(double const x, double const t) {
    if (x <= 0.0 || x > 1.0) {
        printf("glu_pol (gpdh_pol.c): ERROR: x = %12.3e < 0\n", x);
        return 0.0;
    }
    if ((1.0 - x) < EPS_BETR)
        return 0.0;

    // Blumlein, Bottcher at Q2 = 4 GeV^2
    double eta = 1.026;
    double a = 1.424;
    double b = 5.51;
    double gam = 0.0;

    double coeff = (1.0 + gam * a / (a + b + 1.0)) * gammf(a) * gammf(b + 1.0)
            / gammf(a + b + 1.0);

    return x * eta / coeff * exp((a - 1.0) * log(x) + b * log(1.0 - x))
            * (1.0 + gam * x);
}

// COMMON FUNCTIONS ===================================================================================

double VinnikovModel::prof_funct(double const b, double const beta,
        double const alpha) {
    double gamma1 = gammf(2.0 * b + 2.0);
    double gamma2 = gammf(b + 1.0);
    double power1 = power(2.0 * (1.0 - beta), -2.0 * b - 1.0);
    //  double power1 = exp(-log(2.0*(1-beta))*(2.0*b+1.0));
    //  double power2 = exp(log((1.0-beta)*(1.0-beta) - alpha*alpha)*b);
    double power2 = power((1.0 - beta) * (1.0 - beta) - alpha * alpha, b);

    return gamma1 * power1 * power2 / (gamma2 * gamma2);
}

double VinnikovModel::power(double const x, double const y) {
    return exp(y * log(x));
}

double VinnikovModel::gammf(double xx) {
    double x, y, tmp, ser;
    static double cof[6] = { 76.18009172947146, -86.50532032941677,
            24.01409824083091, -1.231739572450155, 0.1208650973866179e-2,
            -0.5395239384953e-5 };
    int j;

    y = x = xx;
    tmp = x + 5.5;
    tmp -= (x + 0.5) * log(tmp);
    ser = 1.000000000190015;
    for (j = 0; j <= 5; j++)
        ser += cof[j] / ++y;
    return exp(-tmp) * 2.5066282746310005 * ser / x;
}

double VinnikovModel::dd_int_simp(double const bmin, double const bmax,
        double (VinnikovModel::*dd)(int, double, double, double, double, int,
                double*), int const i_part, double const x, double const xi,
        double const t, int const n_par, double* const param) {
    int i;
    double integr = 0.0;

    double step = log((bmax - EPS_BETR) / (bmin + EPS_BETR))
            / ((double) SIMP_INT);

    if ((bmax - bmin) < EPS_BETR)
        return (bmax - bmin)
                * (this->*dd)(i_part, 0.5 * (bmax + bmin), x, xi, t, n_par,
                        param);

    if ((SIMP_INT % 2) != 0) {
        printf("SIMP_INT (gpd.h) MUST BE EVEN, YOURS IS ODD\n");
        return 0.0;
    }

    for (i = 2; i < SIMP_INT; i += 2) {
        double beta = (bmin + EPS_BETR) * exp(i * step);
        integr += 2.0 * beta * step
                * (this->*dd)(i_part, beta, x, xi, t, n_par, param);
    }

    for (i = 1; i < SIMP_INT; i += 2) {
        double beta = (bmin + EPS_BETR) * exp(i * step);
        integr += 4.0 * beta * step
                * (this->*dd)(i_part, beta, x, xi, t, n_par, param);
    }
    //  integr += dd(bmin+EPS_BETR,x,xi) + dd(bmax-EPS_BETR,x,xi);
    integr += (bmin + EPS_BETR) * step
            * (this->*dd)(i_part, bmin + EPS_BETR, x, xi, t, n_par, param)
            + (bmax - EPS_BETR) * step
                    * (this->*dd)(i_part, bmax - EPS_BETR, x, xi, t, n_par,
                            param);

    integr *= 1.0 / 3.0;

    return integr;
}

