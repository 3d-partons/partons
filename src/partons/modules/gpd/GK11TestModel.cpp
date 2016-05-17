/*
 * GKK11TestModule.cpp
 *
 *  Created on: May 17, 2016
 *      Author: debian
 */

#include "../../../../include/partons/modules/gpd/GK11TestModel.h"

#include "../../../../include/partons/modules/gpd/GK11TestModel.h"

#include <cln/float.h>
#include <cln/float_class.h>
#include <cln/floatformat.h>
#include <cln/real.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <cmath>
#include <map>
#include <utility>
#include <NumA/integration/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int GK11TestModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GK11TestModel("GK11TestModel"));

//TODO initialise missing members
GK11TestModel::GK11TestModel(const std::string &className) :
        GPDModule(className) {
    m_nf = 3;
    fL = 0.;
    m_MuF2_ref = 4.;
    Huval1tab = std::vector<double>(3, 0.);
    Hdval1tab = std::vector<double>(3, 0.);
    Huval1mtab = std::vector<double>(3, 0.);
    Hdval1mtab = std::vector<double>(3, 0.);
    Hs1tab = std::vector<double>(4, 0.);
    Hi1tab = std::vector<double>(4, 0.);
    Euval1tab = std::vector<double>(2, 0.);
    Edval1tab = std::vector<double>(8, 0.);
    Euval1mtab = std::vector<double>(2, 0.);
    Edval1mtab = std::vector<double>(8, 0.);
    Es1tab = std::vector<double>(3, 0.);
    Ei1tab = std::vector<double>(2, 0.);
    Htuval1tab = std::vector<double>(3, 0.);
    Htdval1tab = std::vector<double>(3, 0.);
    Htuval1mtab = std::vector<double>(3, 0.);
    Htdval1mtab = std::vector<double>(3, 0.);
    Hti1tab = std::vector<double>(3, 0.);
    Etuval1tab = std::vector<double>(4, 0.);
    Etdval1tab = std::vector<double>(4, 0.);
    Etuval1mtab = std::vector<double>(4, 0.);
    Etdval1mtab = std::vector<double>(4, 0.);

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::E, &GPDModule::computeE));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Ht, &GPDModule::computeHt));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Et, &GPDModule::computeEt));

    MathIntegratorModule();
    initFunctorsForIntegrations();
}

GK11TestModel::GK11TestModel(const GK11TestModel& other) :
        GPDModule(other) {
    c1 = other.getC1();
    c2 = other.getC2();
    c3 = other.getC3();
    c4 = other.getC4();
    c5 = other.getC5();
    c6 = other.getC6();
    c7 = other.getC7();
    c8 = other.getC8();
    b0 = other.getB0();
    kappa_s = other.getKappaS();
    fL = other.getL();
    fHuValMx = other.getHuValMx();
    fHdValMx = other.getHdValMx();
    fEuValMx = other.getEuValMx();
    fEdValMx = other.getEdValMx();
    fHtuValMx = other.getHtuValMx();
    fHtdValMx = other.getHtdValMx();
    fEtuValMx = other.getEtuValMx();
    fEtdValMx = other.getEtdValMx();
    kHgluon = other.getHgluon();
    kHsea = other.getHsea();
    kHuval = other.getHuval();
    kHdval = other.getHdval();
    kEgluon = other.getEgluon();
    kEsea = other.getEsea();
    kEuval = other.getEuval();
    kEdval = other.getEdval();
    kHtgluon = other.getHtgluon();
    kHtsea = other.getHtsea();
    kHtuval = other.getHtuval();
    kHtdval = other.getHtdval();
    kEtgluon = other.getEtgluon();
    kEtsea = other.getEtsea();
    kEtuval = other.getEtuval();
    kEtdval = other.getEtdval();
    Huval1tab = other.getHuval1tab();
    Hdval1tab = other.getHdval1tab();
    Huval1mtab = other.getHuval1mtab();
    Hdval1mtab = other.getHdval1mtab();
    Hs1tab = other.getHs1tab();
    Hi1tab = other.getHi1tab();
    Euval1tab = other.getEuval1tab();
    Edval1tab = other.getEdval1tab();
    Euval1mtab = other.getEuval1mtab();
    Edval1mtab = other.getEdval1mtab();
    Es1tab = other.getEs1tab();
    Ei1tab = other.getEi1tab();
    Htuval1tab = other.getHtuval1tab();
    Htdval1tab = other.getHtdval1tab();
    Htuval1mtab = other.getHtuval1mtab();
    Htdval1mtab = other.getHtdval1mtab();
    Hti1tab = other.getHti1tab();
    Etuval1tab = other.getEtuval1tab();
    Etdval1tab = other.getEtdval1tab();
    Etuval1mtab = other.getEtuval1mtab();
    Etdval1mtab = other.getEtdval1mtab();

    MathIntegratorModule();
    initFunctorsForIntegrations();
}

GK11TestModel* GK11TestModel::clone() const {
    return new GK11TestModel(*this);
}

GK11TestModel::~GK11TestModel() {
}

void GK11TestModel::configure(const ElemUtils::Parameters &parameters) {
    GPDModule::configure(parameters);
}

//TODO implement
void GK11TestModel::isModuleWellConfigured() {
    GPDModule::isModuleWellConfigured();

}

void GK11TestModel::initModule() {
    GPDModule::initModule();

    fL = log(m_MuF2 / m_MuF2_ref); // Logarithmic dependence on the scale

    /*   debug(__func__,
     ElemUtils::Formatter() << "fMuF2 = " << fMuF2 << " fL = " << fL);*/
}

void GK11TestModel::initFunctorsForIntegrations() {
    setIntegrator(NumA::IntegratorType1D::DEXP);

    m_pIntegralHuVal = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK11TestModel::IntegralHuVal);

    m_pIntegralHdVal = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK11TestModel::IntegralHdVal);

    m_pIntegralHg = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK11TestModel::IntegralHg);

    m_pIntegralHs = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK11TestModel::IntegralHs);

}

double GK11TestModel::Profile(double N, double beta, double alpha) {
    double profile;
    double ProfileShape = N;
    double TwiceProfileShapePlus1 = 2. * ProfileShape + 1;

//    double alphaBeta = fabs(alpha) + fabs(beta);
//    if (alphaBeta > 1.) {
//        error(__func__,
//                ElemUtils::Formatter()
//                        << "MPSSW13Model : Parameters of profile function should be in rhombus | alpha | + | beta | <= 1."
//                        << '\n' << "Here alpha = " << alpha << " beta = "
//                        << beta << " | alpha | + | beta | = " << alphaBeta
//                        << '\n');
//    }

    profile = pow((1. - fabs(beta)) * (1. - fabs(beta)) - alpha * alpha,
            ProfileShape);
    profile /= pow(1. - fabs(beta), TwiceProfileShapePlus1);
    profile *= tgamma(TwiceProfileShapePlus1 + 1.);
    profile /= (pow(2., TwiceProfileShapePlus1) * tgamma(ProfileShape + 1.)
            * tgamma(ProfileShape + 1.));

    return profile;
}



double GK11TestModel::PDFexp(double N, double beta, double* coeff) {
    double sum = 0;
    for (int i = 0; i < 4; i++) {
        sum = sum + coeff[i] * pow(beta, i / 2);
    }
    return pow(beta, -coeff[4]) * pow((1. - beta), 2 * N + 1) * sum;
}

double GK11TestModel::IntegralHuVal(double x, std::vector<double> Par){return 3;}
double GK11TestModel::IntegralHdVal(double x, std::vector<double> Par){return 3;}
double GK11TestModel::IntegralHs(double x, std::vector<double> Par){return 3;}

double GK11TestModel::IntegralHg(double x, std::vector<double> Par){
    double Integral;
    double pdf, beta, absbeta, alpha;

    beta = x;
    alpha =(beta - m_x)/m_xi;
    absbeta = fabs(beta);


//TODO implement function
   /* if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }*/
    c1 = 2.23 + 0.362 * fL; // See table 1 p. 12
    c2 = 5.43 - 7.0 * fL; // See table 1 p. 12
    c3 = -34.0 + 22.5 * fL; // See table 1 p. 12
    c4 = 40.6 - 21.6 * fL; // See table 1 p. 12
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + m_MuF2)); // See eq. (39) p. 14
    d0 = 0.10 + 0.06 * fL - 0.0027 * fL * fL;
    alphaP = 0.15;
    double coeff[5] = { c1, c2, c3, c4, d0 };

    Integral = exp(b0*m_t)*pow(absbeta, -alphaP*m_t)*PDFexp(2,beta,coeff)*Profile(2,beta,alpha);

    return Integral;

}
PartonDistribution GK11TestModel::computeH() {
    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    double Eps = 1.e-9;
    m_Mx = -m_x;

    // Integration limits and methods
    double Beta1 = (m_x - m_xi) / (1. - m_xi); // eq. (54) in A. Radyushkin's paper
    double Beta2 = (m_x + m_xi) / (1. + m_xi); // eq. (54) in A. Radyushkin's paper

    double Beta1Mx = (m_Mx - m_xi) / (1. - m_xi); // eq. (54) in A. Radyushkin's paper
    double Beta2Mx = (m_Mx + m_xi) / (1. + m_xi); // eq. (54) in A. Radyushkin's paper

    std::vector<double> emptyParameters;
// Scales

///    calculateHCoefs(); // Calculate the K's and the coefficients

// Gluons

    c1 = 2.23 + 0.362 * fL; // See table 1 p. 12
    c2 = 5.43 - 7.0 * fL; // See table 1 p. 12
    c3 = -34.0 + 22.5 * fL; // See table 1 p. 12
    c4 = 40.6 - 21.6 * fL; // See table 1 p. 12
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + m_MuF2)); // See eq. (39) p. 14
    d0 = 0.10 + 0.06 * fL - 0.0027 * fL * fL;
    alphaP = 0.15;
    double coeff[5] = { c1, c2, c3, c4, d0 };

    double Hg = 0.;

    if (m_x >= m_xi) {
        // Integration
        Hg = integrate(m_pIntegralHg, Beta1, Beta2, emptyParameters);
    }

    if (fabs(m_x) < m_xi) {
        // Integration
        Hg = integrate(m_pIntegralHg, 0., Beta2, emptyParameters);

        Hg += integrate(m_pIntegralHg, 0., Beta2Mx, emptyParameters);
    }

    if (m_x <= -m_xi) {
        // Integration
        Hg = integrate(m_pIntegralHg, Beta1Mx, Beta2Mx, emptyParameters);
    }

    GluonDistribution gluonDistribution(Hg);

// s quark

    c1 = 0.123 + 0.0003 * fL; // See table 1 p. 12
    c2 = -0.327 - 0.004 * fL; // See table 1 p. 12
    c3 = 0.692 - 0.068 * fL; // See table 1 p. 12
    c4 = -0.486 + 0.038 * fL; // See table 1 p. 12
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + m_MuF2)); // See eq. (39) p. 14
    d0 = 1 + coeff[4];
    alphaP = 0.15;
   //  coeff = {c1,c2,c3,c4,d0};

    /*quarkDistribution_s.setQuarkDistribution(
     exp(b0 * m_t)
     * (c1 * Hs1tab.at(0) + c2 * Hs1tab.at(1) + c3 * Hs1tab.at(2)
     + c4 * Hs1tab.at(3))); // See eq. (27)*/

// u quark, valence part
    c1 = 1.52 + 0.248 * fL; // See eq. (27)
    c2 = 2.88 - 0.94 * fL; // See eq. (27)
    c3 = -0.095 * fL; // See eq. (27)

    b0 = 0; // See p. 15
    alphaP = 0.9;
    double uVal = exp(b0 * m_t)
            * (c1 * Huval1tab.at(0) + c2 * Huval1tab.at(1)
                    + c3 * Huval1tab.at(2)); // See eq. (27)

    fHuValMx = exp(b0 * m_t)
            * (c1 * Huval1mtab.at(0) + c2 * Huval1mtab.at(1)
                    + c3 * Huval1mtab.at(2)); // See eq. (27);

// d quark, valence part

    c1 = 0.76 + 0.248 * fL; // See table 1 p. 12
    c2 = 3.11 - 1.36 * fL; // See table 1 p. 12
    c3 = -3.99 + 1.15 * fL; // See table 1 p. 12
    alphaP = 0.9;
    b0 = 0; // See p. 15

    double dVal = exp(b0 * m_t)
            * (c1 * Hdval1tab.at(0) + c2 * Hdval1tab.at(1)
                    + c3 * Hdval1tab.at(2)); // See eq. (27)

    // C'est HqVal (-x)
    fHdValMx = exp(b0 * m_t)
            * (c1 * Hdval1mtab.at(0) + c2 * Hdval1mtab.at(1)
                    + c3 * Hdval1mtab.at(2)); // See eq. (27)

// u and d quarks, sea part

    kappa_s = 1. + 0.68 / (1. + 0.52 * log(m_MuF2 / m_MuF2_ref)); // See eq. (36)

    double uSea = kappa_s * quarkDistribution_s.getQuarkDistribution(); // See eq. (35)
    double dSea = uSea;

// u and d quarks, valence + sea parts

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);

// H, charge singlet

//    pGPD_H->setSquareChargeAveraged(
//            U2_ELEC_CHARGE
//                    * (pGPDQuarkFlavorData_u->getSea()
//                            + pGPDQuarkFlavorData_u->getValence())
//                    + D2_ELEC_CHARGE
//                            * (pGPDQuarkFlavorData_d->getSea()
//                                    + pGPDQuarkFlavorData_d->getValence())
//                    + S2_ELEC_CHARGE * pGPDQuarkFlavorData_s->getHq());

    // Set Hq(+)
    quarkDistribution_u.setQuarkDistributionPlus(uVal - fHuValMx + 2 * uSea);
    quarkDistribution_d.setQuarkDistributionPlus(dVal - fHdValMx + 2 * dSea);
    quarkDistribution_s.setQuarkDistributionPlus(
            2 * quarkDistribution_s.getQuarkDistribution());

    // Set Hq(-)
    quarkDistribution_u.setQuarkDistributionMinus(uVal + fHuValMx);
    quarkDistribution_d.setQuarkDistributionMinus(dVal + fHdValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

PartonDistribution GK11TestModel::computeHt() {

    debug(__func__, "Entered function ...");

    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    double delta, etau, etad, Nu, Nd;

    calculateHtCoefs(); // Calculate the K's and the coefficients

// Gluons

    c1 = 3.39 - 0.864 * fL;
    c2 = 1.73 + 0.24 * fL - 0.17 * fL * fL;
    c3 = 0.42 - 0.115 * fL - 0.069 * fL * fL;
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + m_MuF2));

    GluonDistribution gluonDistribution(
            exp(b0 * m_t)
                    * (c1 * Hti1tab.at(0) + c2 * Hti1tab.at(1)
                            + c3 * Hti1tab.at(2)));

    debug(__func__,
            ElemUtils::Formatter() << "c1 = " << c1 << ", c2 = " << c2
                    << ", c3 = " << c3 << ", b0 = " << b0);

// s quark,  Ht_sea = 0 for GK
    quarkDistribution_s.setQuarkDistribution(0.);

// u quark, valence part

    c1 = 0.170 + 0.03 * fL; //parameters from GK3 table 4  and note 11 december 2008
    c2 = 1.340 - 0.02 * fL;
    c3 = 0.120 - 0.40 * fL;

    etau = 0.926;
    delta = 0.48;
    b0 = 0.;

    Nu = tgamma(1. - delta) * tgamma(4.) / tgamma(5. - delta)
            * (c1 + c2 * (1. - delta) / (5. - delta)
                    + c3 * (2. - delta) * (1. - delta) / (6 - delta)
                            / (5. - delta));
//      +  c4 * (3.-delta) * (2.-delta) * (1-delta)/(7.-delta)/(6.-delta)/(5.-delta) ) ;

    debug(__func__,
            ElemUtils::Formatter() << "c1 = " << c1 << ", c2 = " << c2
                    << ", c3 = " << c3 << ", b0 = " << b0 << ", etau = " << etau
                    << ", delta = " << delta << ", Nu = " << Nu);

    double uVal = etau / Nu * exp(b0 * m_t)
            * (c1 * Htuval1tab.at(0) + c2 * Htuval1tab.at(1)
                    + c3 * Htuval1tab.at(2));
    fHtuValMx = etau / Nu * exp(b0 * m_t)
            * (c1 * Htuval1mtab.at(0) + c2 * Htuval1mtab.at(1)
                    + c3 * Htuval1mtab.at(2)); // -pGPDData->getX()

// d quark, valence part

    c1 = -0.320 - 0.040 * fL; // parameters from GK3 table 4
    c2 = -1.427 - 0.176 * fL;
    c3 = 0.692 - 0.068 * fL;

    etad = -0.341;

    Nd = tgamma(1. - delta) * tgamma(4.) / tgamma(5. - delta)
            * (c1 + c2 * (1. - delta) / (5. - delta)
                    + c3 * (2. - delta) * (1. - delta) / (6. - delta)
                            / (5. - delta));

    b0 = 0;

    debug(__func__,
            ElemUtils::Formatter() << "c1 = " << c1 << ", c2 = " << c2
                    << ", c3 = " << c3 << ", b0 = " << b0 << ", etad = " << etad
                    << ", Nd = " << Nd);

    double dVal = etad / Nd * exp(b0 * m_t)
            * (c1 * Htdval1tab.at(0) + c2 * Htdval1tab.at(1)
                    + c3 * Htdval1tab.at(2));
    fHtdValMx = etad / Nd * exp(b0 * m_t)
            * (c1 * Htdval1mtab.at(0) + c2 * Htdval1mtab.at(1)
                    + c3 * Htdval1mtab.at(2)); // -pGPDData->getX()

// u and d quark, sea part
    double uSea = 0.;
    double dSea = 0.;

// u and d quarks, valence + sea parts

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);

    // Set Htq(+)
    quarkDistribution_u.setQuarkDistributionPlus(uVal + fHtuValMx);
    quarkDistribution_d.setQuarkDistributionPlus(dVal + fHtdValMx);
    quarkDistribution_s.setQuarkDistributionPlus(0.);

    // Set Htq(-)
    quarkDistribution_u.setQuarkDistributionMinus(uVal - fHtuValMx);
    quarkDistribution_d.setQuarkDistributionMinus(dVal - fHtdValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

PartonDistribution GK11TestModel::computeE() {
    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    calculateECoefs(); // Calculate the K's and the coefficients

// Gluons

    c1 = 0.779;
    c2 = -c1;
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + m_MuF2));

    GluonDistribution gluonDistribution(
            exp(b0 * m_t) * (c1 * Ei1tab.at(0) + c2 * Ei1tab.at(1)));

// s quark (sea)

    c1 = -0.155;
    c2 = -2 * c1;
    c3 = c1;
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + m_MuF2));

    quarkDistribution_s.setQuarkDistribution(
            exp(b0 * m_t)
                    * (c1 * Es1tab.at(0) + c2 * Es1tab.at(1) + c3 * Es1tab.at(2)));

// u quark, valence part

    c1 = 2.2053;
    c2 = -c1;

    b0 = 0;

    double uVal = exp(b0 * m_t) * (c1 * Euval1tab.at(0) + c2 * Euval1tab.at(1));
    fEuValMx = exp(b0 * m_t) * (c1 * Euval1mtab.at(0) + c2 * Euval1mtab.at(1)); // -pGPDData->getX()

// d quark, valence part

    c1 = -3.114; // DFJK4 version betad = 5.6
    c2 = 8.096;
    c3 = -6.477;
    c4 = 1.295;
    c5 = 0.1296;
    c6 = 0.0362;
    c7 = 0.014516;
    c8 = 0.0070504;

    b0 = 0;

    double dVal = exp(b0 * m_t)
            * (c1 * Edval1tab.at(0) + c2 * Edval1tab.at(1)
                    + c3 * Edval1tab.at(2) + c4 * Edval1tab.at(3)
                    + c5 * Edval1tab.at(4) + c6 * Edval1tab.at(5)
                    + c7 * Edval1tab.at(6) + c8 * Edval1tab.at(7));
    fEdValMx = exp(b0 * m_t)
            * (c1 * Edval1mtab.at(0) + c2 * Edval1mtab.at(1)
                    + c3 * Edval1mtab.at(2) + c4 * Edval1mtab.at(3)
                    + c5 * Edval1mtab.at(4) + c6 * Edval1mtab.at(5)
                    + c7 * Edval1mtab.at(6) + c8 * Edval1mtab.at(7)); // -pGPDData->getX()

// u and d quarks, sea part

//EuSea = Es
    double uSea = quarkDistribution_s.getQuarkDistribution();
//EdSea = Es
    double dSea = quarkDistribution_s.getQuarkDistribution();

// u and d quarks, valence + sea parts

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);

    // Set Eq(+)
    quarkDistribution_u.setQuarkDistributionPlus(uVal - fEuValMx + 2 * uSea);
    quarkDistribution_d.setQuarkDistributionPlus(dVal - fEdValMx + 2 * dSea);
    quarkDistribution_s.setQuarkDistributionPlus(
            2 * quarkDistribution_s.getQuarkDistribution());

    // Set Eq(-)
    quarkDistribution_u.setQuarkDistributionMinus(uVal + fEuValMx);
    quarkDistribution_d.setQuarkDistributionMinus(dVal + fEdValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

PartonDistribution GK11TestModel::computeEt() {
    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    double Nu, Nd;
    double PionPolePx, PionPoleMx;

    calculateEtCoefs(); // Calculate the K's and the coefficients

    PionPolePx = Et_pole(m_x);
    PionPoleMx = Et_pole(-m_x);

// Gluons,  Et_gluons = 0 for GK
    GluonDistribution gluonDistribution(0.);

// s quark,  Et_sea = 0 for GK
    quarkDistribution_s.setQuarkDistributionPlus(0.);

// u quark, valence part

    Nu = 14.0;
    c1 = Nu; // parameters P.Kroll Maple file
    c2 = -2. * Nu;
    c3 = Nu;
    c4 = 0.0;
    b0 = 0.9; // from P.Kroll's email,  14/3/2011

    double fEtuVal = exp(b0 * m_t)
            * (c1 * Etuval1tab.at(0) + c2 * Etuval1tab.at(1)
                    + c3 * Etuval1tab.at(2) + c4 * Etuval1tab.at(3));
    fEtuVal += PionPolePx;
    double uVal = fEtuVal;
    fEtuValMx = exp(b0 * m_t)
            * (c1 * Etuval1mtab.at(0) + c2 * Etuval1mtab.at(1)
                    + c3 * Etuval1mtab.at(2) + c4 * Etuval1mtab.at(3)); // -pGPDData->getX()
    fEtuValMx += PionPoleMx;

// d quark, valence part

    Nd = 4.0;
    c1 = Nd; // parameters P.Kroll Maple file
    c2 = -2. * Nd;
    c3 = Nd;
    c4 = 0.0;
    b0 = 0.9;

    double fEtdVal = exp(b0 * m_t)
            * (c1 * Etdval1tab.at(0) + c2 * Etdval1tab.at(1)
                    + c3 * Etdval1tab.at(2) + c4 * Etdval1tab.at(3));
    fEtdVal -= PionPolePx;
    double dVal = fEtdVal;
    fEtdValMx = exp(b0 * m_t)
            * (c1 * Etdval1mtab.at(0) + c2 * Etdval1mtab.at(1)
                    + c3 * Etdval1mtab.at(2) + c4 * Etdval1mtab.at(3)); // -pGPDData->getX()
    fEtdValMx -= PionPoleMx;

// u and d quark, sea part
    double uSea = 0.;
    double dSea = 0.;

// u and d quarks, valence + sea parts

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);

    // Set Etq(+)
    quarkDistribution_u.setQuarkDistributionPlus(uVal + fEtuValMx);
    quarkDistribution_d.setQuarkDistributionPlus(dVal + fEtdValMx);
    quarkDistribution_s.setQuarkDistributionPlus(0.);

    // Set Eq(-)
    quarkDistribution_u.setQuarkDistributionMinus(uVal - fEtuValMx);
    quarkDistribution_d.setQuarkDistributionMinus(dVal - fEtdValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

//double GK11TestModel::computeSinglet(const QuarkDistribution &quarkDistribution_u,
//        const QuarkDistribution &quarkDistribution_d,
//        const QuarkDistribution &quarkDistribution_s) {
//
//    if (m_nbOfQuarkFlavor == 0) {
//        throwException(__func__, "divided by ZERO !");
//    }
//
//    double result = quarkDistribution_u.getQuarkDistributionPlus()
//            + quarkDistribution_d.getQuarkDistributionPlus()
//            + quarkDistribution_s.getQuarkDistributionPlus();
//    result *= (1 / (2 * m_nbOfQuarkFlavor));
//
//    return result;
//}

void GK11TestModel::calculateHCoefs() {
    int slow_sea = 0; // by default,  fast
    int slow_val = 0; // if  =  1 : slow (full calculation with cln )

    if (log10(m_xi) < (4. / 2.5) * log10(fabs(m_x))) {
        slow_sea = 1;
    }

// For valence

    if ((m_xi < 0.01)
            && (log10(m_xi) < ((-4. / log10(0.6)) * log10(fabs(m_x))))) {
        slow_val = 1;
    }

    calculateHKas(); // comes up with kHgluon,  kHsea,  kHuval,  kHdval

    if (!slow_sea) {
        if (m_x >= 0) {
            Hs1tab.at(0) = Hs1(m_x, 0., kHsea);
            Hs1tab.at(1) = Hs1(m_x, 0.5, kHsea);
            Hs1tab.at(2) = Hs1(m_x, 1., kHsea);
            Hs1tab.at(3) = Hs1(m_x, 1.5, kHsea);

            Hi1tab.at(0) = Hi1(m_x, 0., kHgluon);
            Hi1tab.at(1) = Hi1(m_x, 0.5, kHgluon);
            Hi1tab.at(2) = Hi1(m_x, 1., kHgluon);
            Hi1tab.at(3) = Hi1(m_x, 1.5, kHgluon); ///< TODO: CHECK IT - ADDED BY JAKUB
        } else {
            Hs1tab.at(0) = -Hs1(-m_x, 0., kHsea);
            Hs1tab.at(1) = -Hs1(-m_x, 0.5, kHsea);
            Hs1tab.at(2) = -Hs1(-m_x, 1., kHsea);
            Hs1tab.at(3) = -Hs1(-m_x, 1.5, kHsea);

            Hi1tab.at(0) = Hi1(-m_x, 0., kHgluon);
            Hi1tab.at(1) = Hi1(-m_x, 0.5, kHgluon);
            Hi1tab.at(2) = Hi1(-m_x, 1., kHgluon);
            Hi1tab.at(3) = Hi1(-m_x, 1.5, kHgluon);
        }
    } else {
        if (m_x >= 0) {
            Hs1tab.at(0) = Hs1_alt(m_x, 0., kHsea);
            Hs1tab.at(1) = Hs1_alt(m_x, 0.5, kHsea);
            Hs1tab.at(2) = Hs1_alt(m_x, 1., kHsea);
            Hs1tab.at(3) = Hs1_alt(m_x, 1.5, kHsea);

            Hi1tab.at(0) = Hi1_alt(m_x, 0., kHgluon);
            Hi1tab.at(1) = Hi1_alt(m_x, 0.5, kHgluon);
            Hi1tab.at(2) = Hi1_alt(m_x, 1., kHgluon);
            Hi1tab.at(3) = Hi1_alt(m_x, 1.5, kHgluon);
        } else {
            Hs1tab.at(0) = -Hs1_alt(-m_x, 0., kHsea);
            Hs1tab.at(1) = -Hs1_alt(-m_x, 0.5, kHsea);
            Hs1tab.at(2) = -Hs1_alt(-m_x, 1., kHsea);
            Hs1tab.at(3) = -Hs1_alt(-m_x, 1.5, kHsea);

            Hi1tab.at(0) = Hi1_alt(-m_x, 0., kHgluon);
            Hi1tab.at(1) = Hi1_alt(-m_x, 0.5, kHgluon);
            Hi1tab.at(2) = Hi1_alt(-m_x, 1., kHgluon);
            Hi1tab.at(3) = Hi1_alt(-m_x, 1.5, kHgluon);
        }
    }

    if (!slow_val) {
        Huval1tab.at(0) = Hval1(m_x, 0., kHuval);
        Huval1tab.at(1) = Hval1(m_x, 0.5, kHuval);
        Huval1tab.at(2) = Hval1(m_x, 1., kHuval);

        Hdval1tab.at(0) = Hval1(m_x, 0., kHdval);
        Hdval1tab.at(1) = Hval1(m_x, 0.5, kHdval);
        Hdval1tab.at(2) = Hval1(m_x, 1., kHdval);

        Huval1mtab.at(0) = Hval1(-m_x, 0., kHuval);
        Huval1mtab.at(1) = Hval1(-m_x, 0.5, kHuval);
        Huval1mtab.at(2) = Hval1(-m_x, 1., kHuval);

        Hdval1mtab.at(0) = Hval1(-m_x, 0., kHdval);
        Hdval1mtab.at(1) = Hval1(-m_x, 0.5, kHdval);
        Hdval1mtab.at(2) = Hval1(-m_x, 1., kHdval);

    } else {
        Huval1tab.at(0) = Hval1_alt(m_x, 0., kHuval);
        Huval1tab.at(1) = Hval1_alt(m_x, 0.5, kHuval);
        Huval1tab.at(2) = Hval1_alt(m_x, 1., kHuval);

        Hdval1tab.at(0) = Hval1_alt(m_x, 0., kHdval);
        Hdval1tab.at(1) = Hval1_alt(m_x, 0.5, kHdval);
        Hdval1tab.at(2) = Hval1_alt(m_x, 1., kHdval);

        Huval1mtab.at(0) = Hval1_alt(-m_x, 0., kHuval);
        Huval1mtab.at(1) = Hval1_alt(-m_x, 0.5, kHuval);
        Huval1mtab.at(2) = Hval1_alt(-m_x, 1., kHuval);

        Hdval1mtab.at(0) = Hval1_alt(-m_x, 0., kHdval);
        Hdval1mtab.at(1) = Hval1_alt(-m_x, 0.5, kHdval);
        Hdval1mtab.at(2) = Hval1_alt(-m_x, 1., kHdval);
    }
}

void GK11TestModel::calculateHKas() {
    double alpha, delta;

// gluons

    alpha = 0.15; //slope of gluon trajectory
    delta = 0.10 + 0.06 * fL;
    kHgluon = delta + alpha * m_t;

// sea

    alpha = 0.15; // same as slope of gluon trajectory
    delta = 1.1 + 0.06 * fL - 0.0027 * fL * fL;
    kHsea = delta + alpha * m_t;

// u valence

    alpha = 0.9;
    delta = 0.48;
    kHuval = delta + alpha * m_t;

// d valence

    alpha = 0.9 - 0.05 * fL;
    delta = 0.48 + 0.006 * fL;
    kHdval = delta + alpha * m_t;
}

double GK11TestModel::Hs1(double x, double i, double k) {

    double dummy;

    if ((x - m_xi) < 0) {
        dummy = 15. / 4. / pow(m_xi, 5.)
                * pow((x + m_xi) / (1. + m_xi), 3. + i - k)
                * ((3. * pow(3. + i - k, 2.) - (1. + i - k) * (5. + i - k))
                        * pow(m_xi * m_xi - x, 2.)
                        + 6. * (3. + i - k) * m_xi * (1. - x)
                                * (m_xi * m_xi - x)
                        - 2. * (2. + i - k) * (4. + i - k) * (1. - m_xi * m_xi)
                                * (x * x - m_xi * m_xi)) / (1. + i - k)
                / (2. + i - k) / (3. + i - k) / (4. + i - k) / (5. + i - k)
                - 15. / 4. / pow(m_xi, 5.)
                        * pow((-x + m_xi) / (1. + m_xi), 3. + i - k)
                        * ((3. * pow(3. + i - k, 2.)
                                - (1. + i - k) * (5. + i - k))
                                * pow(m_xi * m_xi + x, 2.)
                                + 6. * (3. + i - k) * m_xi * (1. + x)
                                        * (m_xi * m_xi + x)
                                - 2. * (2. + i - k) * (4. + i - k)
                                        * (1. - m_xi * m_xi)
                                        * (x * x - m_xi * m_xi)) / (1. + i - k)
                        / (2. + i - k) / (3. + i - k) / (4. + i - k)
                        / (5. + i - k);
    } else {
        dummy = 15. / 4. / pow(m_xi, 5.) / (1. + i - k) / (2. + i - k)
                / (3. + i - k) / (4. + i - k) / (5. + i - k)
                * (((3. * pow(3. + i - k, 2.) - (1. + i - k) * (5. + i - k))
                        * pow(m_xi * m_xi - x, 2.)
                        - 2. * (2. + i - k) * (4. + i - k) * (1. - m_xi * m_xi)
                                * (x * x - m_xi * m_xi))
                        * (pow((x + m_xi) / (1. + m_xi), 3. + i - k)
                                - pow((x - m_xi) / (1. - m_xi), 3. + i - k))
                        + 6. * (3. + i - k) * m_xi * (1. - x)
                                * (m_xi * m_xi - x)
                                * (pow((x + m_xi) / (1. + m_xi), 3. + i - k)
                                        + pow((x - m_xi) / (1. - m_xi),
                                                3. + i - k)));
    }

    /*    debug(__func__,
     ElemUtils::Formatter() << "(x=" << x << ", xi=" << m_xi << ", i=" << i
     << ", k=" << k << ") dummy = " << dummy);*/

    return dummy;
}

double GK11TestModel::Hi1(double x, double i, double k) {

    double dummy;

    if ((x - m_xi) < 0) {
        dummy = 15. / 4. / pow(m_xi, 5.)
                * pow((x + m_xi) / (1. + m_xi), 3. + i - k)
                * ((3. * pow(3. + i - k, 2.) - (1. + i - k) * (5. + i - k))
                        * pow(m_xi * m_xi - x, 2.)
                        + 6. * (3. + i - k) * m_xi * (1. - x)
                                * (m_xi * m_xi - x)
                        - 2. * (2. + i - k) * (4. + i - k) * (1. - m_xi * m_xi)
                                * (x * x - m_xi * m_xi)) / (1. + i - k)
                / (2. + i - k) / (3. + i - k) / (4. + i - k) / (5. + i - k)
                + 15. / 4. / pow(m_xi, 5.)
                        * pow((-x + m_xi) / (1. + m_xi), 3. + i - k)
                        * ((3. * pow(3. + i - k, 2.)
                                - (1. + i - k) * (5. + i - k))
                                * pow(m_xi * m_xi + x, 2.)
                                + 6. * (3. + i - k) * m_xi * (1. + x)
                                        * (m_xi * m_xi + x)
                                - 2. * (2. + i - k) * (4. + i - k)
                                        * (1. - m_xi * m_xi)
                                        * (x * x - m_xi * m_xi)) / (1. + i - k)
                        / (2. + i - k) / (3. + i - k) / (4. + i - k)
                        / (5. + i - k);
    } else {
        dummy = 15. / 4. / pow(m_xi, 5.) / (1. + i - k) / (2. + i - k)
                / (3. + i - k) / (4. + i - k) / (5. + i - k)
                * (((3. * pow(3. + i - k, 2.) - (1. + i - k) * (5. + i - k))
                        * pow(m_xi * m_xi - x, 2.)
                        - 2. * (2. + i - k) * (4. + i - k) * (1. - m_xi * m_xi)
                                * (x * x - m_xi * m_xi))
                        * (pow((x + m_xi) / (1. + m_xi), 3. + i - k)
                                - pow((x - m_xi) / (1. - m_xi), 3. + i - k))
                        + 6. * (3. + i - k) * m_xi * (1. - x)
                                * (m_xi * m_xi - x)
                                * (pow((x + m_xi) / (1. + m_xi), 3. + i - k)
                                        + pow((x - m_xi) / (1. - m_xi),
                                                3. + i - k)));
    }

    return dummy;
}

double GK11TestModel::Hs1_alt(double x, double i, double k) {
    cln::cl_R ia, ka, xba, xia;
    cln::cl_R dummya, dummy1, dummy2, dummy3;

    cln::float_format_t prec = cln::float_format(30);

    xia = cl_float(m_xi, prec);
    ka = cl_float(k, prec);
    xba = cl_float(x, prec);
    ia = cl_float(i, prec);

    cln::cl_R xi5 = expt(xia, 5);
    dummy2 = exp((3 + ia - ka) * ln((xba + xia) / (1 + xia)));
    if (fabs(x - m_xi) > 1e-6) {
        dummy1 = exp((3 + ia - ka) * ln((xba - xia) / (1 - xia)));
        dummy3 = exp((3 + ia - ka) * ln((-xba + xia) / (1 + xia)));
    } else {
        dummy1 = 0;
        dummy3 = 0;
    }

    double dummy;

    if ((x - m_xi) < 0) {

        dummya = dummy2
                * ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                        * expt(xia * xia - xba, 2)
                        + 6 * (3 + ia - ka) * xia * (1 - xba)
                                * (xia * xia - xba)
                        - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                                * (xba * xba - xia * xia));

        dummya += -dummy3
                * ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                        * expt(xia * xia + xba, 2)
                        + 6 * (3 + ia - ka) * xia * (1 + xba)
                                * (xia * xia + xba)
                        - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                                * (xba * xba - xia * xia));

        dummya *= cl_float(15, prec) / 4 / xi5 / (1 + ia - ka) / (2 + ia - ka)
                / (3 + ia - ka) / (4 + ia - ka) / (5 + ia - ka);

    } else {
        dummya = ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                * expt(xia * xia - xba, 2)
                - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                        * (xba * xba - xia * xia)) * (dummy2 - dummy1)
                + 6 * (3 + ia - ka) * xia * (1 - xba) * (xia * xia - xba)
                        * (dummy2 + dummy1);

        dummya *= cl_float(15, prec) / 4 / xi5 / (1 + ia - ka) / (2 + ia - ka)
                / (3 + ia - ka) / (4 + ia - ka) / (5 + ia - ka);

    }
    dummy = double_approx(dummya);

    return dummy;
}

double GK11TestModel::Hi1_alt(double x, double i, double k) {
    cln::cl_R ia, ka, xba, xia;
    cln::cl_R dummya, dummy1, dummy2, dummy3;

    cln::float_format_t prec = cln::float_format(30);

    xia = cl_float(m_xi, prec);
    ka = cl_float(k, prec);
    xba = cl_float(x, prec);
    ia = cl_float(i, prec);

    cln::cl_R xi5 = expt(xia, 5);

    dummy2 = exp((3 + ia - ka) * ln((xba + xia) / (1 + xia)));

    if (fabs(x - m_xi) > 1e-6) {
        dummy1 = exp((3 + ia - ka) * ln((xba - xia) / (1 - xia)));
        dummy3 = exp((3 + ia - ka) * ln((-xba + xia) / (1 + xia)));
    } else {
        dummy1 = 0;
        dummy3 = 0;
    }

    double dummy;

    if ((x - m_xi) < 0) {
        dummya = dummy2
                * ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                        * expt(xia * xia - xba, 2)
                        + 6 * (3 + ia - ka) * xia * (1 - xba)
                                * (xia * xia - xba)
                        - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                                * (xba * xba - xia * xia));

        dummya += dummy3
                * ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                        * expt(xia * xia + xba, 2)
                        + 6 * (3 + ia - ka) * xia * (1 + xba)
                                * (xia * xia + xba)
                        - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                                * (xba * xba - xia * xia));

        dummya *= cl_float(15, prec) / 4 / xi5 / (1 + ia - ka) / (2 + ia - ka)
                / (3 + ia - ka) / (4 + ia - ka) / (5 + ia - ka);

    } else {
        dummya = ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                * expt(xia * xia - xba, 2)
                - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                        * (xba * xba - xia * xia)) * (dummy2 - dummy1)
                + 6 * (3 + ia - ka) * xia * (1 - xba) * (xia * xia - xba)
                        * (dummy2 + dummy1);

        dummya *= cl_float(15, prec) / 4 / xi5 / (1 + ia - ka) / (2 + ia - ka)
                / (3 + ia - ka) / (4 + ia - ka) / (5 + ia - ka);

    }

    dummy = double_approx(dummya);

    return dummy;
}

double GK11TestModel::Hval1(double x, double i, double k) {

    double dummy;

    if ((x + m_xi) < 0) {
        dummy = 0.; // no anti-quarks in valence !
    } else if ((x - m_xi) < 0) {
        dummy = 3. / 2. / pow(m_xi, 3.)
                * (pow((x + m_xi) / (1. + m_xi), (2. + i - k))
                        * (m_xi * m_xi - x + (2. + i - k) * m_xi * (1. - x)))
                / (1. + i - k) / (2. + i - k) / (3. + i - k);
    } else {
        dummy = 3. / 2. / pow(m_xi, 3.) / (1. + i - k) / (2. + i - k)
                / (3. + i - k)
                * ((m_xi * m_xi - x)
                        * (pow((x + m_xi) / (1. + m_xi), (2. + i - k))
                                - pow((x - m_xi) / (1. - m_xi), (2. + i - k)))
                        + m_xi * (1. - x) * (2. + i - k)
                                * (pow((x + m_xi) / (1. + m_xi), (2. + i - k))
                                        + pow((x - m_xi) / (1. - m_xi),
                                                (2. + i - k))));
    }

    /*   debug(__func__,
     ElemUtils::Formatter() << "(x=" << x << ", xi=" << m_xi << ", i=" << i
     << ", k=" << k << ") dummy = " << dummy);*/

    return dummy;
}

double GK11TestModel::Hval1_alt(double x, double i, double k) {
    cln::cl_R ia, ka, xba, xia;
    cln::cl_R dummya, dummy1, dummy2;

    cln::float_format_t prec = cln::float_format(30);

    xia = cl_float(m_xi, prec);
    ka = cl_float(k, prec);
    xba = cl_float(x, prec);
    ia = cl_float(i, prec);

    cln::cl_R xi3 = expt(xia, 3);

    if (fabs(x + m_xi) > 1e-6) {
        dummy2 = exp((2 + ia - ka) * ln((xba + xia) / (1 + xia)));
    } else {
        dummy2 = 0;
    }

    if (fabs(x - m_xi) > 1e-6) {
        dummy1 = exp((2 + ia - ka) * ln((xba - xia) / (1 - xia)));
    } else {
        dummy1 = 0;
    }

    double dummy;

    if ((x + m_xi) < 0) {
        dummy = 0.; // no anti-quarks in valence !
    } else if ((x - m_xi) < 0) {
        dummya = dummy2 * (xia * xia - xba + (2 + ia - ka) * xia * (1 - xba));

        dummya *= cl_float(3, prec) / 2 / xi3 / (1 + ia - ka) / (2 + ia - ka)
                / (3 + ia - ka);

    } else {
        dummya = (xia * xia - xba) * (dummy2 - dummy1);

        dummya += (2 + ia - ka) * xia * (1 - xba) * (dummy2 + dummy1);

        dummya *= cl_float(3, prec) / 2 / xi3 / (1 + ia - ka) / (2 + ia - ka)
                / (3 + ia - ka);
    }

    dummy = double_approx(dummya);

    debug(__func__,
            ElemUtils::Formatter() << "(x=" << x << ", xi=" << m_xi << ", i="
                    << i << ", k=" << k << ") dummy = " << dummy);

    return dummy;
}

/** !
 *  \fn void STGK11TestModel::calculateHtCoefs()
 *
 * Generic function for fast calculation.
 *
 * Calculates :
 *    - Hti1  ( i = 0,  0.5,  1 )
 *    - Htval1( i = 0,  1,  2 )
 *
 * Either fast (cln) or slow, depending on kinematic domain in (x,xi)
 * To be called before the evaluation of GPDs.
 *
 * Kinematic boundaries
 * For the sea and gluons : gluons share the same limits as strange/sea as far as slow/fast goes.
 *    ( gluons only for now,  Htilde_sea = 0 for Kroll )
 *
 */
void GK11TestModel::calculateHtCoefs() {
    int slow_sea = 0; // by default,  fast
    int slow_val = 0; // if  =  1 : slow ( full calculation with cln )

    if (log10(m_xi) < (4. / 2.5) * log10(fabs(m_x))) {
        slow_sea = 1;
    }

// For valence

    if ((m_xi < 0.01)
            && (log10(m_xi) < ((-4. / log10(0.6)) * log10(fabs(m_x))))) {
        slow_val = 1;
    }

    calculateHtKas(); // comes up with kHtgluon,  kHstea,  kHtuval,  kHtdval

// No sea Ht for GK.

    debug(__func__,
            ElemUtils::Formatter() << "slow_sea = " << slow_sea
                    << " slow_val = " << slow_val);

//TODO permuter les conditions

    if (!slow_sea) {
        if (m_x >= 0) {
            Hti1tab.at(0) = Hti1(m_x, 0., kHtgluon);
            Hti1tab.at(1) = Hti1(m_x, 0.5, kHtgluon);
            Hti1tab.at(2) = Hti1(m_x, 1.0, kHtgluon);
        } else {
            Hti1tab.at(0) = Hti1(-m_x, 0., kHtgluon);
            Hti1tab.at(1) = Hti1(-m_x, 0.5, kHtgluon);
            Hti1tab.at(2) = Hti1(-m_x, 1.0, kHtgluon);
        }
    } else {
        if (m_x >= 0) {
            Hti1tab.at(0) = Hti1_alt(m_x, 0., kHtgluon);
            Hti1tab.at(1) = Hti1_alt(m_x, 0.5, kHtgluon);
            Hti1tab.at(2) = Hti1_alt(m_x, 1.0, kHtgluon);
        } else {
            Hti1tab.at(0) = Hti1_alt(-m_x, 0., kHtgluon);
            Hti1tab.at(1) = Hti1_alt(-m_x, 0.5, kHtgluon);
            Hti1tab.at(2) = Hti1_alt(-m_x, 1.0, kHtgluon);
        }
    }

    if (!slow_val) {
        Htuval1tab.at(0) = Htval1(m_x, 0., kHtuval);
        Htuval1tab.at(1) = Htval1(m_x, 1., kHtuval);
        Htuval1tab.at(2) = Htval1(m_x, 2., kHtuval);

        Htdval1tab.at(0) = Htuval1tab.at(0); // kHtdval & kHtuval are equal
        Htdval1tab.at(1) = Htuval1tab.at(1); // for u and d for Ht
        Htdval1tab.at(2) = Htuval1tab.at(2); // don't need to recalculate

        Htuval1mtab.at(0) = Htval1(-m_x, 0., kHtuval);
        Htuval1mtab.at(1) = Htval1(-m_x, 1., kHtuval);
        Htuval1mtab.at(2) = Htval1(-m_x, 2., kHtuval);

        Htdval1mtab.at(0) = Htuval1mtab.at(0);
        Htdval1mtab.at(1) = Htuval1mtab.at(1);
        Htdval1mtab.at(2) = Htuval1mtab.at(2);

    } else {
        Htuval1tab.at(0) = Htval1_alt(m_x, 0., kHtuval);
        Htuval1tab.at(1) = Htval1_alt(m_x, 1., kHtuval);
        Htuval1tab.at(2) = Htval1_alt(m_x, 2., kHtuval);

        Htdval1tab.at(0) = Htuval1tab.at(0);
        Htdval1tab.at(1) = Htuval1tab.at(1);
        Htdval1tab.at(2) = Htuval1tab.at(2);

        Htuval1mtab.at(0) = Htval1_alt(-m_x, 0., kHtuval);
        Htuval1mtab.at(1) = Htval1_alt(-m_x, 1., kHtuval);
        Htuval1mtab.at(2) = Htval1_alt(-m_x, 2., kHtuval);

        Htdval1mtab.at(0) = Htuval1mtab.at(0);
        Htdval1mtab.at(1) = Htuval1mtab.at(1);
        Htdval1mtab.at(2) = Htuval1mtab.at(2);
    }
}

/*!
 * \fn void STGK11TestModel::calculateHtKas()
 *
 * Compute t-dependence of GPD Ht correlated to x.
 *
 */
void GK11TestModel::calculateHtKas() {
    double alpha, delta;

// gluons

    alpha = 0.15; //slope of gluon trajectory
    delta = -0.78 + 0.17 * fL;
    kHtgluon = delta + alpha * m_t;

// sea, not modelled by GK.

    kHtsea = 0.;

// u valence

    alpha = 0.45;
    delta = 0.48;
    kHtuval = delta + alpha * m_t;

// d valence

    kHtdval = kHtuval;

    debug(__func__,
            ElemUtils::Formatter() << "(t=" << m_t << ") kHtgluon=" << kHtgluon
                    << " kHtsea=" << kHtsea << " kHtuval=" << kHtuval
                    << " kHtdval=" << kHtdval);
}

/*!
 * \fn void STGK11TestModel::CalculateEtKas()
 *
 * Exactly the same as Hval1_alt and Eval1_alt.
 *
 */
double GK11TestModel::Htval1_alt(double x, double i, double k) {
    return Hval1_alt(x, i, k);
}

/*!
 * \fn double STGK11TestModel::Hti1( double x, double i, double k )
 *
 * Exactly the same as Hs1.
 *
 */
double GK11TestModel::Hti1(double x, double i, double k) {

    return Hs1(x, i, k);
}

/*!
 * \fn void STGK11TestModel::CalculateEtKas()
 *
 * Exactly the same as Hs1_alt.
 *
 */
double GK11TestModel::Hti1_alt(double x, double i, double k) {

//return Hs1_alt(_xi, x, i, k);

    cln::cl_R ia, ka, xba, xia;
    cln::cl_R dummya, dummy1, dummy2, dummy3;

    cln::float_format_t prec = cln::float_format(30);

    xia = cl_float(m_xi, prec);
    ka = cl_float(k, prec);
    xba = cl_float(x, prec);
    ia = cl_float(i, prec);

    cln::cl_R xi5 = expt(xia, 5);

    dummy2 = exp((3 + ia - ka) * ln((xba + xia) / (1 + xia)));
    if (fabs(x - m_xi) > 1e-6) {
        dummy1 = exp((3 + ia - ka) * ln((xba - xia) / (1 - xia)));
        dummy3 = exp((3 + ia - ka) * ln((-xba + xia) / (1 + xia)));
    } else {
        dummy1 = 0;
        dummy3 = 0;
    }

    double dummy;

    if ((x - m_xi) < 0) {

        dummya = dummy2
                * ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                        * expt(xia * xia - xba, 2)
                        + 6 * (3 + ia - ka) * xia * (1 - xba)
                                * (xia * xia - xba)
                        - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                                * (xba * xba - xia * xia));

        dummya += -dummy3
                * ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                        * expt(xia * xia + xba, 2)
                        + 6 * (3 + ia - ka) * xia * (1 + xba)
                                * (xia * xia + xba)
                        - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                                * (xba * xba - xia * xia));

        dummya *= cl_float(15, prec) / 4 / xi5 / (1 + ia - ka) / (2 + ia - ka)
                / (3 + ia - ka) / (4 + ia - ka) / (5 + ia - ka);

    } else {
        dummya = ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                * expt(xia * xia - xba, 2)
                - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                        * (xba * xba - xia * xia)) * (dummy2 - dummy1)
                + 6 * (3 + ia - ka) * xia * (1 - xba) * (xia * xia - xba)
                        * (dummy2 + dummy1);

        dummya *= cl_float(15, prec) / 4 / xi5 / (1 + ia - ka) / (2 + ia - ka)
                / (3 + ia - ka) / (4 + ia - ka) / (5 + ia - ka);

    }
    dummy = double_approx(dummya);

    debug(__func__,
            ElemUtils::Formatter() << "(x=" << x << ", xi=" << m_xi << ", i="
                    << i << ", k=" << k << ") dummy = " << dummy);

    return dummy;
}

/*!
 * \fn void STGK11TestModel::CalculateEtKas()
 *
 * Exactly the same as Hval1 and Eval1.
 *
 */
double GK11TestModel::Htval1(double x, double i, double k) {

    return Hval1(x, i, k);
}

/** !
 *  \fn void STGK11TestModel::calculateECoefs()
 *
 * Generic function for fast calculation.
 *
 * Calculates :
 *    - Es1  (i=0, 1, 2)
 *    - Ei1  (i=0, 1)
 *    - Eval1(i=0, 1, 2) (u)
 *    - Eval1(i=0, .. ,7) (d)
 *
 * Either fast (cln) or slow, depending on kinematic domain in (x,xi)
 * To be called before the evaluation of GPDs.
 *
 * Kinematic boundaries
 * For the sea and gluons : gluons share th(ame limits as strange/sea as far as slow/fast goes.
 *
 */
void GK11TestModel::calculateECoefs() {
    int slow_sea = 0; // by default,  fast
    int slow_val = 0; // if  =  1 : slow ( full calculation with cln )

    if (log10(m_xi) < (4. / 2.5) * log10(fabs(m_x))) {
        slow_sea = 1;
    }

// For valence

    if ((m_xi < 0.01)
            && (log10(m_xi) < ((-4. / log10(0.6)) * log10(fabs(m_x))))) {
        slow_val = 1;
    }

    calculateEKas(); // comes up with kEgluon,  kEsea,  kEuval,  kEdval

    if (!slow_sea) {
        if (m_x >= 0) {
            Es1tab.at(0) = Es1(m_x, 0., kEsea);
            Es1tab.at(1) = Es1(m_x, 1., kEsea);
            Es1tab.at(2) = Es1(m_x, 2., kEsea);

            Ei1tab.at(0) = Ei1(m_x, 0., kEgluon);
            Ei1tab.at(1) = Ei1(m_x, 1., kEgluon);
        } else {
            Es1tab.at(0) = -Es1(-m_x, 0., kEsea);
            Es1tab.at(1) = -Es1(-m_x, 1., kEsea);
            Es1tab.at(2) = -Es1(-m_x, 2., kEsea);

            Ei1tab.at(0) = Ei1(-m_x, 0., kEgluon);
            Ei1tab.at(1) = Ei1(-m_x, 1., kEgluon);
        }
    } else {
        if (m_x >= 0) {
            Es1tab.at(0) = Es1_alt(m_x, 0., kEsea);
            Es1tab.at(1) = Es1_alt(m_x, 1., kEsea);
            Es1tab.at(2) = Es1_alt(m_x, 2., kEsea);

            Ei1tab.at(0) = Ei1_alt(m_x, 0., kEgluon);
            Ei1tab.at(1) = Ei1_alt(m_x, 1., kEgluon);
        } else {
            Es1tab.at(0) = -Es1_alt(-m_x, 0., kEsea);
            Es1tab.at(1) = -Es1_alt(-m_x, 1., kEsea);
            Es1tab.at(2) = -Es1_alt(-m_x, 2., kEsea);

            Ei1tab.at(0) = Ei1_alt(-m_x, 0., kEgluon);
            Ei1tab.at(1) = Ei1_alt(-m_x, 1., kEgluon);
        }
    }

    if (!slow_val) {
        Euval1tab.at(0) = Eval1(m_x, 0., kEuval);
        Euval1tab.at(1) = Eval1(m_x, 1., kEuval);

        Edval1tab.at(0) = Eval1(m_x, 0., kEdval);
        Edval1tab.at(1) = Eval1(m_x, 1., kEdval);
        Edval1tab.at(2) = Eval1(m_x, 2., kEdval);
        Edval1tab.at(3) = Eval1(m_x, 3., kEdval);
        Edval1tab.at(4) = Eval1(m_x, 4., kEdval);
        Edval1tab.at(5) = Eval1(m_x, 5., kEdval);
        Edval1tab.at(6) = Eval1(m_x, 6., kEdval);
        Edval1tab.at(7) = Eval1(m_x, 7., kEdval);

        Euval1mtab.at(0) = Eval1(-m_x, 0., kEuval);
        Euval1mtab.at(1) = Eval1(-m_x, 1., kEuval);

        Edval1mtab.at(0) = Eval1(-m_x, 0., kEdval);
        Edval1mtab.at(1) = Eval1(-m_x, 1., kEdval);
        Edval1mtab.at(2) = Eval1(-m_x, 2., kEdval);
        Edval1mtab.at(3) = Eval1(-m_x, 3., kEdval);
        Edval1mtab.at(4) = Eval1(-m_x, 4., kEdval);
        Edval1mtab.at(5) = Eval1(-m_x, 5., kEdval);
        Edval1mtab.at(6) = Eval1(-m_x, 6., kEdval);
        Edval1mtab.at(7) = Eval1(-m_x, 7., kEdval);

    } else {
        Euval1tab.at(0) = Eval1_alt(m_x, 0., kEuval);
        Euval1tab.at(1) = Eval1_alt(m_x, 1., kEuval);

        Edval1tab.at(0) = Eval1_alt(m_x, 0., kEdval);
        Edval1tab.at(1) = Eval1_alt(m_x, 1., kEdval);
        Edval1tab.at(2) = Eval1_alt(m_x, 2., kEdval);
        Edval1tab.at(3) = Eval1_alt(m_x, 3., kEdval);
        Edval1tab.at(4) = Eval1_alt(m_x, 4., kEdval);
        Edval1tab.at(5) = Eval1_alt(m_x, 5., kEdval);
        Edval1tab.at(6) = Eval1_alt(m_x, 6., kEdval);
        Edval1tab.at(7) = Eval1_alt(m_x, 7., kEdval);

        Euval1mtab.at(0) = Eval1_alt(-m_x, 0., kEuval);
        Euval1mtab.at(1) = Eval1_alt(-m_x, 1., kEuval);

        Edval1mtab.at(0) = Eval1_alt(-m_x, 0., kEdval);
        Edval1mtab.at(1) = Eval1_alt(-m_x, 1., kEdval);
        Edval1mtab.at(2) = Eval1_alt(-m_x, 2., kEdval);
        Edval1mtab.at(3) = Eval1_alt(-m_x, 3., kEdval);
        Edval1mtab.at(4) = Eval1_alt(-m_x, 4., kEdval);
        Edval1mtab.at(5) = Eval1_alt(-m_x, 5., kEdval);
        Edval1mtab.at(6) = Eval1_alt(-m_x, 6., kEdval);
        Edval1mtab.at(7) = Eval1_alt(-m_x, 7., kEdval);
    }

}

/*!
 * \fn void STGK11TestModel::calculateEKas()
 *
 * Compute t-dependence of GPD H correlated to x.
 *
 */
void GK11TestModel::calculateEKas() {
    double alpha, delta;

// gluons

    alpha = 0.15; //slope of gluon trajectory
    delta = 0.10 + 0.06 * fL - 0.0027 * fL * fL;
    kEgluon = delta + alpha * m_t;

// sea

    alpha = 0.15; // same as slope of gluon trajectory
    delta = 1.1 + 0.06 * fL - 0.0027 * fL * fL;
    kEsea = delta + alpha * m_t;

// u valence

    alpha = 0.9;
    delta = 0.48;
    kEuval = delta + alpha * m_t;

// d valence

    alpha = 0.9;
    delta = 0.48;
    kEdval = delta + alpha * m_t;

}

/*!
 * \fn double STGK11TestModel::Es1( double x, double i, double k )
 *
 * Exactly the same as Hs1.
 *
 */
double GK11TestModel::Es1(double x, double i, double k) {
    return Hs1(x, i, k);
}

/*!
 * \fn double STGK11TestModel::Ei1( double x, double i, double k )
 *
 * Exactly the same as Hi1.
 *
 */
double GK11TestModel::Ei1(double x, double i, double k) {
    return Hi1(x, i, k);
}

/*!
 * \fn double STGK11TestModel::Es1_alt( double x, double i, double k )
 *
 * Exactly the same as Hs1_alt.
 *
 */
double GK11TestModel::Es1_alt(double x, double i, double k) {
    return Hs1_alt(x, i, k);
}

/*!
 * \fn double STGK11TestModel::Ei1_alt( double x, double i, double k )
 *
 * Exactly the same as Hi1_alt.
 *
 */
double GK11TestModel::Ei1_alt(double x, double i, double k) {
    return Hi1_alt(x, i, k);
}

/*!
 * \fn double STGK11TestModel::Eval1( double x, double i, double k )
 *
 * Exactly the same as Hval1.
 *
 */
double GK11TestModel::Eval1(double x, double i, double k) {
    return Hval1(x, i, k);
}

/*!
 * \fn void STGK11TestModel::CalculateEtKas()
 *
 * Exactly the same as Hval1_alt.
 *
 */
double GK11TestModel::Eval1_alt(double x, double i, double k) {
    return Hval1_alt(x, i, k);
}

/** !
 *  \fn void STGK11TestModel::calculateEtCoefs()
 *
 * Generic function for fast calculation.
 *
 * Calculates :
 *    - Etval1  ( i = 0,  1,  2,  3 )
 *
 * Either fast (cln) or slow, depending on kinematic domain in (x,xi)
 * To be called before the evaluation of GPDs.
 *
 * Kinematic boundaries
 * For the sea and gluons : gluons share the same limits as strange/sea as far as slow/fast goes.
 *    For the sea ( and gluons later )
 *
 */
void GK11TestModel::calculateEtCoefs() {
    int slow_val = 0; // if  =  1 : slow ( full calculation with cln )

// For valence

    if ((m_xi < 0.04)
            && (log10(m_xi) < ((-4. / log10(0.55)) * log10(fabs(m_x))))) {
        slow_val = 1;
    }

    calculateEtKas(); // comes up with kHgluon,  kHsea,  kHuval,  kHdval

// WARNING : No sea or gluon Et for GK.

    if (!slow_val) {
        Etuval1tab.at(0) = Etval1(m_x, 0., kEtuval);
        Etuval1tab.at(1) = Etval1(m_x, 1., kEtuval);
        Etuval1tab.at(2) = Etval1(m_x, 2., kEtuval);
        Etuval1tab.at(3) = Etval1(m_x, 3., kEtuval);

        Etdval1tab.at(0) = Etuval1tab.at(0); // kEtdval & kEtuval are equal
        Etdval1tab.at(1) = Etuval1tab.at(1); // for u and d for Et,
        Etdval1tab.at(2) = Etuval1tab.at(2); // don't need to recalculate
        Etdval1tab.at(3) = Etuval1tab.at(3);

        Etuval1mtab.at(0) = Etval1(-m_x, 0., kEtuval);
        Etuval1mtab.at(1) = Etval1(-m_x, 1., kEtuval);
        Etuval1mtab.at(2) = Etval1(-m_x, 2., kEtuval);
        Etuval1mtab.at(3) = Etval1(-m_x, 3., kEtuval);

        Etdval1mtab.at(0) = Etuval1mtab.at(0);
        Etdval1mtab.at(1) = Etuval1mtab.at(1);
        Etdval1mtab.at(2) = Etuval1mtab.at(2);
        Etdval1mtab.at(3) = Etuval1mtab.at(3);

    } else {
        Etuval1tab.at(0) = Etval1_alt(m_x, 0., kEtuval);
        Etuval1tab.at(1) = Etval1_alt(m_x, 1., kEtuval);
        Etuval1tab.at(2) = Etval1_alt(m_x, 2., kEtuval);
        Etuval1tab.at(3) = Etval1_alt(m_x, 3., kEtuval);

        Etdval1tab.at(0) = Etuval1tab.at(0);
        Etdval1tab.at(1) = Etuval1tab.at(1);
        Etdval1tab.at(2) = Etuval1tab.at(2);
        Etdval1tab.at(3) = Etuval1tab.at(3);

        Etuval1mtab.at(0) = Etval1_alt(-m_x, 0., kEtuval);
        Etuval1mtab.at(1) = Etval1_alt(-m_x, 1., kEtuval);
        Etuval1mtab.at(2) = Etval1_alt(-m_x, 2., kEtuval);
        Etuval1mtab.at(3) = Etval1_alt(-m_x, 3., kEtuval);

        Etdval1mtab.at(0) = Etuval1mtab.at(0);
        Etdval1mtab.at(1) = Etuval1mtab.at(1);
        Etdval1mtab.at(2) = Etuval1mtab.at(2);
        Etdval1mtab.at(3) = Etuval1mtab.at(3);
    }
}

/*!
 * \fn void STGK11TestModel::calculateEtKas()
 *
 * Compute t-dependence of GPD Et correlated to x.
 *
 */
void GK11TestModel::calculateEtKas() {
    double alpha, delta;

// gluons, not modelled by GK.

    kEtgluon = 0.;

// sea, not modelled by GK.

    kEtsea = 0.;

// u valence

    alpha = 0.45;
    delta = 0.48;
    kEtuval = delta + alpha * m_t;

// d valence

    kEtdval = kEtuval;
}

/* !
 *  \fn double STGK11TestModel::Et_pole( double x )
 *
 *  Pion pole contribution to Et.
 *
 */
double GK11TestModel::Et_pole(double x) {

//TODO extraire les constantes si existantes

    double dum = 0.; //  Initialize to 0
    double eps;
    double eps2;
    double tmin;
    double xbj;
    double Q2 = m_MuF2;
    double tOverQ2 = m_t / Q2;
    double y = (x + m_xi) / 2. / m_xi;
    double MPi2 = PI_ZERO_MASS * PI_ZERO_MASS;
    double gpiNN = 13.4;
    double f_pi = 0.131; // f_pi=0.131 GeV from paper. Actually 130.4 from PDG (Jul2010)
    double Lambda_N2 = 0.51 * 0.51;
    double Fp;
    double FpiNN;

    xbj = 2. * m_xi / (m_xi - tOverQ2 * m_xi + 1. + tOverQ2 * 0.5);
    eps = 2. * xbj * PROTON_MASS / sqrt(Q2);
    eps2 = eps * eps;

    if (eps < 1 && (4. * xbj * (1. - xbj) + eps2) != 0) {

        tmin = -Q2 * (2. * (1. - xbj) * (1 - sqrt(1. - eps2)) + eps2)
                / (4. * xbj * (1. - xbj) + eps2);
        FpiNN = (Lambda_N2 - MPi2) / (Lambda_N2 - (m_t - tmin));
        Fp = -PROTON_MASS * f_pi * (2. * sqrt(2.) * gpiNN * FpiNN)
                / (m_t - MPi2);

        if (x < m_xi && x > -m_xi && m_t < tmin) {
//          dum = ( Fp( pKinematicVariables->getT(), Q2, tmin ) / 4. / pGPDData->getXi() )  *  Phi_pi( ( x + pGPDData->getXi() ) / 2. / pGPDData->getXi() ) ;
            dum = (Fp / 4. / m_xi) * 6. * y * (1. - y);
        }

    }

    return dum;
}

/*!
 * \fn double STGK11TestModel::Etval1( double x, double i, double k )
 *
 *  xdiff:=evalf(xb-xi); xsum:=evalf(xb+xi);
 *  if xsum<0 then    # xb<-xi
 *  Hi:=0 else
 *  if xdiff <0 then  # xb<xi
 *  Hi:= i -> 3/2/xi^3*(((xb+xi)/(1+xi))^(2+i-k)*(xi^2-xb+(2+i-k)*xi*(1-xb)))
 *            /(1+i-k)/(2+i-k)/(3+i-k)
 *  else
 *  Hi:= i -> 3/2/xi^3/(1+i-k)/(2+i-k)/(3+i-k)*((xi^2-xb)*( ((xb+xi)/(1+xi))^(2+i-k)
 *                                 -((xb-xi)/(1-xi))^(2+i-k))
 *                 + xi*(1-xb)*(2+i-k)*(((xb+xi)/(1+xi))^(2+i-k)+ ((xb-xi)/(1-xi))^(2+i-k) ) )
 *  end if: end if:
 *
 */
double GK11TestModel::Etval1(double x, double i, double k) {
    double dum;

    if ((x + m_xi) < 0) { // 0 for x < -pGPDData->getXi()
        dum = 0.0;
    } else { // Two cases for x >= -pGPDData->getXi()
        if ((x - m_xi) < 0) {
            dum =
                    3. / 2. / pow(m_xi, 3.)
                            * (pow((x + m_xi) / (1. + m_xi), (2. + i - k))
                                    * (m_xi * m_xi - x
                                            + (2. + i - k) * m_xi * (1. - x)))
                            / (1. + i - k) / (2. + i - k) / (3. + i - k);
        } else {
            dum = 3. / 2. / pow(m_xi, 3.) / (1. + i - k) / (2. + i - k)
                    / (3. + i - k)
                    * ((m_xi * m_xi - x)
                            * (pow((x + m_xi) / (1. + m_xi), (2. + i - k))
                                    - pow((x - m_xi) / (1. - m_xi),
                                            (2. + i - k)))
                            + m_xi * (1. - x) * (2. + i - k)
                                    * (pow((x + m_xi) / (1. + m_xi),
                                            (2. + i - k))
                                            + pow((x - m_xi) / (1. - m_xi),
                                                    (2. + i - k))));
        }
    }
    return dum;
}

/*!
 * \fn double STGK11TestModel::Etval1_alt( double x, double i, double k )
 *
 * Exactly the same as Hval1_alt and Eval1_alt, Htval1_alt.
 *
 */
double GK11TestModel::Etval1_alt(double x, double i, double k) {
    return Hval1_alt(x, i, k);
}

double GK11TestModel::getB0() const {
    return b0;
}

double GK11TestModel::getC1() const {
    return c1;
}

double GK11TestModel::getC2() const {
    return c2;
}

double GK11TestModel::getC3() const {
    return c3;
}

double GK11TestModel::getC4() const {
    return c4;
}

double GK11TestModel::getC5() const {
    return c5;
}

double GK11TestModel::getC6() const {
    return c6;
}

double GK11TestModel::getC7() const {
    return c7;
}

double GK11TestModel::getC8() const {
    return c8;
}

const std::vector<double>& GK11TestModel::getEdval1mtab() const {
    return Edval1mtab;
}

const std::vector<double>& GK11TestModel::getEdval1tab() const {
    return Edval1tab;
}

const std::vector<double>& GK11TestModel::getEi1tab() const {
    return Ei1tab;
}

const std::vector<double>& GK11TestModel::getEs1tab() const {
    return Es1tab;
}

const std::vector<double>& GK11TestModel::getEtdval1mtab() const {
    return Etdval1mtab;
}

const std::vector<double>& GK11TestModel::getEtdval1tab() const {
    return Etdval1tab;
}

const std::vector<double>& GK11TestModel::getEtuval1mtab() const {
    return Etuval1mtab;
}

const std::vector<double>& GK11TestModel::getEtuval1tab() const {
    return Etuval1tab;
}

const std::vector<double>& GK11TestModel::getEuval1mtab() const {
    return Euval1mtab;
}

const std::vector<double>& GK11TestModel::getEuval1tab() const {
    return Euval1tab;
}

double GK11TestModel::getEdValMx() const {
    return fEdValMx;
}

double GK11TestModel::getEtdValMx() const {
    return fEtdValMx;
}

double GK11TestModel::getEtuValMx() const {
    return fEtuValMx;
}

double GK11TestModel::getEuValMx() const {
    return fEuValMx;
}

double GK11TestModel::getHdValMx() const {
    return fHdValMx;
}

double GK11TestModel::getHtdValMx() const {
    return fHtdValMx;
}

double GK11TestModel::getHtuValMx() const {
    return fHtuValMx;
}

double GK11TestModel::getHuValMx() const {
    return fHuValMx;
}

double GK11TestModel::getL() const {
    return fL;
}

const std::vector<double>& GK11TestModel::getHdval1mtab() const {
    return Hdval1mtab;
}

const std::vector<double>& GK11TestModel::getHdval1tab() const {
    return Hdval1tab;
}

const std::vector<double>& GK11TestModel::getHi1tab() const {
    return Hi1tab;
}

const std::vector<double>& GK11TestModel::getHs1tab() const {
    return Hs1tab;
}

const std::vector<double>& GK11TestModel::getHtdval1mtab() const {
    return Htdval1mtab;
}

const std::vector<double>& GK11TestModel::getHtdval1tab() const {
    return Htdval1tab;
}

const std::vector<double>& GK11TestModel::getHti1tab() const {
    return Hti1tab;
}

const std::vector<double>& GK11TestModel::getHtuval1mtab() const {
    return Htuval1mtab;
}

const std::vector<double>& GK11TestModel::getHtuval1tab() const {
    return Htuval1tab;
}

const std::vector<double>& GK11TestModel::getHuval1mtab() const {
    return Huval1mtab;
}

const std::vector<double>& GK11TestModel::getHuval1tab() const {
    return Huval1tab;
}

double GK11TestModel::getKappaS() const {
    return kappa_s;
}

double GK11TestModel::getEdval() const {
    return kEdval;
}

double GK11TestModel::getEgluon() const {
    return kEgluon;
}

double GK11TestModel::getEsea() const {
    return kEsea;
}

double GK11TestModel::getEtdval() const {
    return kEtdval;
}

double GK11TestModel::getEtgluon() const {
    return kEtgluon;
}

double GK11TestModel::getEtsea() const {
    return kEtsea;
}

double GK11TestModel::getEtuval() const {
    return kEtuval;
}

double GK11TestModel::getEuval() const {
    return kEuval;
}

double GK11TestModel::getHdval() const {
    return kHdval;
}

double GK11TestModel::getHgluon() const {
    return kHgluon;
}

double GK11TestModel::getHsea() const {
    return kHsea;
}

double GK11TestModel::getHtdval() const {
    return kHtdval;
}

double GK11TestModel::getHtgluon() const {
    return kHtgluon;
}

double GK11TestModel::getHtsea() const {
    return kHtsea;
}

double GK11TestModel::getHtuval() const {
    return kHtuval;
}

double GK11TestModel::getHuval() const {
    return kHuval;
}

std::string GK11TestModel::toString() {
    return GPDModule::toString();
}
