// Implementation of GPDs which appear in Goloskokov-Kroll (GK) model in pseudoscalar meson production

#include "../../../../include/partons/modules/gpd/GPDGK19COMPASS.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <utility>

#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"

namespace PARTONS {

const unsigned int GPDGK19COMPASS::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDGK19COMPASS("GPDGK19COMPASS"));

GPDGK19COMPASS::GPDGK19COMPASS(const std::string &className) :
        GPDGK16(className), MathIntegratorModule() {

    m_MuF2_ref = 4.;

//    Htuval1tab = std::vector<double>(4, 0.);
//    Htdval1tab = std::vector<double>(4, 0.);
//    Htuval1mtab = std::vector<double>(4, 0.);
//    Htdval1mtab = std::vector<double>(4, 0.);
    Etuval1tab = std::vector<double>(4, 0.);
    Etdval1tab = std::vector<double>(4, 0.);
    Etuval1mtab = std::vector<double>(4, 0.);
    Etdval1mtab = std::vector<double>(4, 0.);
    HTransuval1tab = std::vector<double>(6, 0.);
    HTransdval1tab = std::vector<double>(6, 0.);
    HTransuval1mtab = std::vector<double>(6, 0.);
    HTransdval1mtab = std::vector<double>(6, 0.);
    ETransuval1tab = std::vector<double>(3, 0.);
    ETransdval1tab = std::vector<double>(3, 0.);
    ETransuval1mtab = std::vector<double>(3, 0.);
    ETransdval1mtab = std::vector<double>(3, 0.);

    kHtgluon = 0.0;
    kHtsea = 0.0;
    kHtuval = 0.0;
    kHtdval = 0.0;

    kEtgluon = 0.0;
    kEtsea = 0.0;
    kEtuval = 0.0;
    kEtdval = 0.0;

    kHTransgluon = 0.0;
    kHTranssea = 0.0;
    kHTransuval = 0.0;
    kHTransdval = 0.0;

    kETransgluon = 0.0;
    kETranssea = 0.0;
    kETransuval = 0.0;
    kETransdval = 0.0;

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::E, &GPDModule::computeE));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Ht, &GPDModule::computeHt));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Et, &GPDModule::computeEt));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::HTrans, &GPDModule::computeHTrans));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::EbarTrans, &GPDModule::computeEbarTrans));

    initFunctorsForIntegrations();
}

GPDGK19COMPASS::GPDGK19COMPASS(const GPDGK19COMPASS& other) :
        GPDGK16(other), MathIntegratorModule(other) {

//    Htuval1tab = other.Htuval1tab;
//    Htdval1tab = other.Htdval1tab;
//    Htuval1mtab = other.Htuval1mtab;
//    Htdval1mtab = other.Htdval1mtab;
    Etuval1tab = other.Etuval1tab;
    Etdval1tab = other.Etdval1tab;
    Etuval1mtab = other.Etuval1mtab;
    Etdval1mtab = other.Etdval1mtab;
    HTransuval1tab = other.HTransuval1tab;
    HTransdval1tab = other.HTransdval1tab;
    HTransuval1mtab = other.HTransuval1mtab;
    HTransdval1mtab = other.HTransdval1mtab;
    ETransuval1tab = other.ETransuval1tab;
    ETransdval1tab = other.ETransdval1tab;
    ETransuval1mtab = other.ETransuval1mtab;
    ETransdval1mtab = other.ETransdval1mtab;

    kHtgluon = other.kHtgluon;
    kHtsea = other.kHtsea;
    kHtuval = other.kHtuval;
    kHtdval = other.kHtdval;

    kEtgluon = other.kEtgluon;
    kEtsea = other.kEtsea;
    kEtuval = other.kEtuval;
    kEtdval = other.kEtdval;

    kHTransgluon = other.kHTransgluon;
    kHTranssea = other.kHTranssea;
    kHTransuval = other.kHTransuval;
    kHTransdval = other.kHTransdval;

    kETransgluon = other.kETransgluon;
    kETranssea = other.kETranssea;
    kETransuval = other.kETransuval;
    kETransdval = other.kETransdval;

    initFunctorsForIntegrations();
}

GPDGK19COMPASS* GPDGK19COMPASS::clone() const {
    return new GPDGK19COMPASS(*this);
}

void GPDGK19COMPASS::resolveObjectDependencies() {

    //run for mother
    GPDGK16::resolveObjectDependencies();

    //set type
    setIntegrator(NumA::IntegratorType1D::DEXP);
}

void GPDGK19COMPASS::configure(const ElemUtils::Parameters &parameters) {

    //run for mother
    GPDGK16::configure(parameters);

    //run for integrator
    MathIntegratorModule::configureIntegrator(parameters);
}

GPDGK19COMPASS::~GPDGK19COMPASS() {

    if (m_pint_HtUp) {
        delete m_pint_HtUp;
        m_pint_HtUp = 0;
    }

    if (m_pint_HtDown) {
        delete m_pint_HtDown;
        m_pint_HtDown = 0;
    }
}

void GPDGK19COMPASS::initModule() {
    GPDGK16::initModule();
}

void GPDGK19COMPASS::isModuleWellConfigured() {
    GPDGK16::isModuleWellConfigured();

}

void GPDGK19COMPASS::initFunctorsForIntegrations() {

    m_pint_HtUp = NumA::Integrator1D::newIntegrationFunctor(this,
            &GPDGK19COMPASS::HtUp);

    m_pint_HtDown = NumA::Integrator1D::newIntegrationFunctor(this,
            &GPDGK19COMPASS::HtDown);
}

double GPDGK19COMPASS::HtUp(double rho, std::vector<double> par) {

    double alpha = par.at(0);

    double Nu, c1, c2, c3, c4;

    Nu = 1.0;

    c1 = 0.213 * Nu;
    c2 = 0.929 * Nu;
    c3 = 12.59 * Nu;
    c4 = -12.57 * Nu;

    double uVal = 3. / (4. * pow(m_xi, 3.)) * pow(rho, -0.32)
            * (c1 + c2 * sqrt(rho) + c3 * rho + c4 * pow(rho, 1.5))
            * exp(
                    m_t
                            * ((-0.961 * log(rho) + 0.545) * pow(1. - rho, 3.)
                                    + 1.264 * rho * pow(1. - rho, 2.)))
            * (m_xi * m_xi * pow(1. - rho, 2.) - pow(alpha - rho, 2.));

    return uVal;

}

double GPDGK19COMPASS::HtDown(double rho, std::vector<double> par) {

    double alpha = par.at(0);

    double Nd, c1, c2, c3, c4, b0;

    Nd = 1.0;
    c1 = -0.204 * Nd;
    c2 = -0.940 * Nd;
    c3 = -0.314 * Nd;
    c4 = 1.524 * Nd;

    double dVal = 3. / (4. * pow(m_xi, 3.)) * pow(rho, -0.32)
            * (c1 + c2 * sqrt(rho) + c3 * rho + c4 * pow(rho, 1.5))
            * exp(
                    m_t
                            * ((-0.861 * log(rho) + 0.206) * pow(1. - rho, 3.)
                                    + 4.198 * rho * pow(1. - rho, 2.)))
            * (m_xi * m_xi * pow(1. - rho, 2.) - pow(alpha - rho, 2.));

    return dVal;

}

PartonDistribution GPDGK19COMPASS::computeHt() {
    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

// Gluons,  Ht_gluons = 0 in GK
    GluonDistribution gluonDistribution(0.);

// s quark,  Ht_sea = 0 in GK
    quarkDistribution_s.setQuarkDistributionPlus(0.);

// u quark, valence part

//    double accuracy = 0.0001;

//    gsl_integration_workspace * w = gsl_integration_workspace_alloc(10000);

    double integration1Da = 0.0;
//   double error1Da = 0.0;
    double integration1Db = 0.0;
//   double error1Db = 0.0;
    double integration1Dc = 0.0;
//  double  error1Dc = 0.0;
    double integration1Dd = 0.0;
//  double  error1Dd = 0.0;

//    double alpha1 = m_x;
//    double alpha2 = -m_x;
    std::vector<double> parAlpha1(1, m_x);
    std::vector<double> parAlpha2(1, -m_x);

//    gsl_function gslFunctionHt1Da; // For HtUp(x, xi, t)
//    gslFunctionHt1Da.function = &GPDGK19COMPASS::HtUp;
//    gslFunctionHt1Da.params = &alpha1;
//
//    gsl_function gslFunctionHt1Db; // For HtUp(-x, xi, t)
//    gslFunctionHt1Db.function = &GPDGK19COMPASS::HtUp;
//    gslFunctionHt1Db.params = &alpha2;
//
//    gsl_function gslFunctionHt1Dc; // For HtDown(x, xi, t)
//    gslFunctionHt1Dc.function = &GPDGK19COMPASS::HtDown;
//    gslFunctionHt1Dc.params = &alpha1;
//
//    gsl_function gslFunctionHt1Dd; // For HtDown(-x, xi, t)
//    gslFunctionHt1Dd.function = &GPDGK19COMPASS::HtDown;
//    gslFunctionHt1Dd.params = &alpha2;

    if (m_x <= -m_xi) {
//        gsl_integration_qags(&gslFunctionHt1Db, (-m_x - m_xi) / (1. - m_xi),
//                (-m_x + m_xi) / (1. + m_xi), 0, 1e-4, 10000, w, &integration1Db,
//                &error1Db);
        integration1Db = integrate(m_pint_HtUp, (-m_x - m_xi) / (1. - m_xi),
                (-m_x + m_xi) / (1. + m_xi), parAlpha2);

//        gsl_integration_qags(&gslFunctionHt1Dd, (-m_x - m_xi) / (1. - m_xi),
//                (-m_x + m_xi) / (1. + m_xi), 0, 1e-4, 10000, w, &integration1Dd,
//                &error1Dd);
        integration1Dd = integrate(m_pint_HtDown, (-m_x - m_xi) / (1. - m_xi),
                (-m_x + m_xi) / (1. + m_xi), parAlpha2);
    } else if (m_x <= m_xi) {
//        gsl_integration_qags(&gslFunctionHt1Da, accuracy,
//                (m_x + m_xi) / (1. + m_xi), 0, 1e-4, 10000, w, &integration1Da,
//                &error1Da);
        integration1Da = integrate(m_pint_HtUp, 0., (m_x + m_xi) / (1. + m_xi),
                parAlpha1);

//        gsl_integration_qags(&gslFunctionHt1Db, accuracy,
//                (-m_x + m_xi) / (1. + m_xi), 0, 1e-4, 10000, w, &integration1Db,
//                &error1Db);
        integration1Db = integrate(m_pint_HtUp, 0., (-m_x + m_xi) / (1. + m_xi),
                parAlpha2);

//        gsl_integration_qags(&gslFunctionHt1Dc, accuracy,
//                (m_x + m_xi) / (1. + m_xi), 0, 1e-4, 10000, w, &integration1Dc,
//                &error1Dc);
        integration1Dc = integrate(m_pint_HtDown, 0., (m_x + m_xi) / (1. + m_xi),
                parAlpha1);

//        gsl_integration_qags(&gslFunctionHt1Dd, accuracy,
//                (-m_x + m_xi) / (1. + m_xi), 0, 1e-4, 10000, w, &integration1Dd,
//                &error1Dd);
        integration1Dd = integrate(m_pint_HtDown, 0., (-m_x + m_xi) / (1. + m_xi),
                parAlpha2);
    } else {
//        gsl_integration_qags(&gslFunctionHt1Da, (m_x - m_xi) / (1. - m_xi),
//                (m_x + m_xi) / (1. + m_xi), 0, 1e-4, 10000, w, &integration1Da,
//                &error1Da);
        integration1Da = integrate(m_pint_HtUp, (m_x - m_xi) / (1. - m_xi),
                (m_x + m_xi) / (1. + m_xi), parAlpha1);

//        gsl_integration_qags(&gslFunctionHt1Dc, (m_x - m_xi) / (1. - m_xi),
//                (m_x + m_xi) / (1. + m_xi), 0, 1e-4, 10000, w, &integration1Dc,
//                &error1Dc);
        integration1Dc = integrate(m_pint_HtDown, (m_x - m_xi) / (1. - m_xi),
                (m_x + m_xi) / (1. + m_xi), parAlpha1);
    }

    // u quark, valence part

    double fHtuVal = integration1Da;

    double uVal = fHtuVal;

    double fHtuValMx = integration1Db;

    // d quark, valence part

    double fHtdVal = integration1Dc;

    double dVal = fHtdVal;

    double fHtdValMx = integration1Dd;

    // u and d quark, sea part
    double uSea = 0.;
    double dSea = 0.;

    // u and d quarks, valence + sea parts

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);

    // Set Etq(+)
    quarkDistribution_u.setQuarkDistributionPlus(uVal + fHtuValMx);
    quarkDistribution_d.setQuarkDistributionPlus(dVal + fHtdValMx);
    quarkDistribution_s.setQuarkDistributionPlus(0.);

    // Set Eq(-)
    quarkDistribution_u.setQuarkDistributionMinus(uVal - fHtuValMx);
    quarkDistribution_d.setQuarkDistributionMinus(dVal - fHtdValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

PartonDistribution GPDGK19COMPASS::computeEt() {
    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    double Nu, Nd, c1, c2, c3, c4, b0;

    double normalization = 1.3 * exp(-m_t * 0.3);

    calculateEtCoefs(); // Calculate the K's and the coefficients

// Gluons,  Et_gluons = 0 in GK
    GluonDistribution gluonDistribution(0.);

// s quark,  Et_sea = 0 in GK
    quarkDistribution_s.setQuarkDistributionPlus(0.);

// u quark, valence part

    Nu = 14.0;
    c1 = Nu;
    c2 = -2. * Nu;
    c3 = Nu;
    c4 = 0.0;
    b0 = 0.9;

    double fEtuVal = normalization * exp(b0 * m_t)
            * (c1 * Etuval1tab.at(0) + c2 * Etuval1tab.at(1)
                    + c3 * Etuval1tab.at(2) + c4 * Etuval1tab.at(3));

    double uVal = fEtuVal;

    double fEtuValMx = normalization * exp(b0 * m_t)
            * (c1 * Etuval1mtab.at(0) + c2 * Etuval1mtab.at(1)
                    + c3 * Etuval1mtab.at(2) + c4 * Etuval1mtab.at(3));

// d quark, valence part

    Nd = 4.0;
    c1 = Nd;
    c2 = -2. * Nd;
    c3 = Nd;
    c4 = 0.0;
    b0 = 0.9;

    double fEtdVal = normalization * exp(b0 * m_t)
            * (c1 * Etdval1tab.at(0) + c2 * Etdval1tab.at(1)
                    + c3 * Etdval1tab.at(2) + c4 * Etdval1tab.at(3));

    double dVal = fEtdVal;

    double fEtdValMx = normalization * exp(b0 * m_t)
            * (c1 * Etdval1mtab.at(0) + c2 * Etdval1mtab.at(1)
                    + c3 * Etdval1mtab.at(2) + c4 * Etdval1mtab.at(3));

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

PartonDistribution GPDGK19COMPASS::computeHTrans() {

    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    double Nu, Nd, c1, c2, c3, c4, c5, c6, b0;

    double normalization = 0.62 * exp(-m_t * 0.26);

    calculateHTransCoefs(); // Calculate the K's and the coefficients

// Gluons,  HTrans_gluons = 0 in GK
    GluonDistribution gluonDistribution(0.);

// s quark,  HTrans_sea = 0 in GK
    quarkDistribution_s.setQuarkDistributionPlus(0.);

// u quark, valence part

    Nu = 1.1;

    c1 = 3.653;
    c2 = -0.583;
    c3 = 19.807;
    c4 = -23.487;
    c5 = -23.46;
    c6 = 24.07;

    b0 = 0.0;

    double fHTransuVal = normalization * exp(b0 * m_t) * Nu
            * (c1 * HTransuval1tab.at(0) + c2 * HTransuval1tab.at(1)
                    + c3 * HTransuval1tab.at(2) + c4 * HTransuval1tab.at(3)
                    + c5 * HTransuval1tab.at(4) + c6 * HTransuval1tab.at(5));

    double uVal = fHTransuVal;

    double fHTransuValMx = normalization * exp(b0 * m_t) * Nu
            * (c1 * HTransuval1mtab.at(0) + c2 * HTransuval1mtab.at(1)
                    + c3 * HTransuval1mtab.at(2) + c4 * HTransuval1mtab.at(3)
                    + c5 * HTransuval1mtab.at(4) + c6 * HTransuval1mtab.at(5));

// d quark, valence part

    Nd = -0.3;

    c1 = 1.924;
    c2 = 0.179;
    c3 = -7.775;
    c4 = 3.504;
    c5 = 5.851;
    c6 = -3.683;

    b0 = 0.0;

    double fHTransdVal = normalization * exp(b0 * m_t) * Nd
            * (c1 * HTransdval1tab.at(0) + c2 * HTransdval1tab.at(1)
                    + c3 * HTransdval1tab.at(2) + c4 * HTransdval1tab.at(3)
                    + c5 * HTransdval1tab.at(4) + c6 * HTransdval1tab.at(5));

    double dVal = fHTransdVal;

    double fHTransdValMx = normalization * exp(b0 * m_t) * Nd
            * (c1 * HTransdval1mtab.at(0) + c2 * HTransdval1mtab.at(1)
                    + c3 * HTransdval1mtab.at(2) + c4 * HTransdval1mtab.at(3)
                    + c5 * HTransdval1mtab.at(4) + c6 * HTransdval1mtab.at(5));

// u and d quark, sea part
    double uSea = 0.;
    double dSea = 0.;

// u and d quarks, valence + sea parts

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);

    // Set Etq(+)
    quarkDistribution_u.setQuarkDistributionPlus(uVal - fHTransuValMx);
    quarkDistribution_d.setQuarkDistributionPlus(dVal - fHTransdValMx);
    quarkDistribution_s.setQuarkDistributionPlus(0.);

    // Set Eq(-)
    quarkDistribution_u.setQuarkDistributionMinus(uVal + fHTransuValMx);
    quarkDistribution_d.setQuarkDistributionMinus(dVal + fHTransdValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

PartonDistribution GPDGK19COMPASS::computeEbarTrans() {

    //available internal variables
    // m_x, m_xi, m_t, m_MuF2, m_MuR2, m_currentGPDComputeType;
    //m_MuF2_ref; // this is set in constructor of GPDGK16: m_MuF2_ref = 4.

    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    double Nu, Nd, c1, c2, c3, b0;

    double normalization = 4.28 * exp(m_t * 0.17);

    calculateETransCoefs(); // Calculate the K's and the coefficients

// Gluons,  ETrans_gluons = 0 for GK
    GluonDistribution gluonDistribution(0.);

// s quark,  ETrans_sea = 0 for GK
    quarkDistribution_s.setQuarkDistributionPlus(0.);

// u quark, valence part

    Nu = 4.83;
    c1 = Nu;
    c2 = -1. * Nu;
    c3 = 0.0;
    b0 = 0.5;

    double fETransuVal = normalization * exp(b0 * m_t)
            * (c1 * ETransuval1tab.at(0) + c2 * ETransuval1tab.at(1)
                    + c3 * ETransuval1tab.at(2));

    double uVal = fETransuVal;

    double fETransuValMx = normalization * exp(b0 * m_t)
            * (c1 * ETransuval1mtab.at(0) + c2 * ETransuval1mtab.at(1)
                    + c3 * ETransuval1mtab.at(2));

// d quark, valence part

    Nd = 3.57;
    c1 = Nd;
    c2 = -2. * Nd;
    c3 = Nd;
    b0 = 0.5;

    double fETransdVal = normalization * exp(b0 * m_t)
            * (c1 * ETransdval1tab.at(0) + c2 * ETransdval1tab.at(1)
                    + c3 * ETransdval1tab.at(2));

    double dVal = fETransdVal;

    double fETransdValMx = normalization * exp(b0 * m_t)
            * (c1 * ETransdval1mtab.at(0) + c2 * ETransdval1mtab.at(1)
                    + c3 * ETransdval1mtab.at(2));

// u and d quark, sea part
    double uSea = 0.;
    double dSea = 0.;

// u and d quarks, valence + sea parts

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);

    // Set Etq(+)
    quarkDistribution_u.setQuarkDistributionPlus(uVal - fETransuValMx);
    quarkDistribution_d.setQuarkDistributionPlus(dVal - fETransdValMx);
    quarkDistribution_s.setQuarkDistributionPlus(0.);

    // Set Eq(-)
    quarkDistribution_u.setQuarkDistributionMinus(uVal + fETransuValMx);
    quarkDistribution_d.setQuarkDistributionMinus(dVal + fETransdValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

void GPDGK19COMPASS::calculateEtCoefs() {

    calculateEtKas();

// WARNING : No sea or gluon Et for GK.

    Etuval1tab.at(0) = ValenceExpansion(m_x, 0., kEtuval);
    Etuval1tab.at(1) = ValenceExpansion(m_x, 1., kEtuval);
    Etuval1tab.at(2) = ValenceExpansion(m_x, 2., kEtuval);
    Etuval1tab.at(3) = ValenceExpansion(m_x, 3., kEtuval);

    Etdval1tab.at(0) = Etuval1tab.at(0); // kEtdval & kEtuval are equal
    Etdval1tab.at(1) = Etuval1tab.at(1); // for u and d for Et,
    Etdval1tab.at(2) = Etuval1tab.at(2); // don't need to recalculate
    Etdval1tab.at(3) = Etuval1tab.at(3);

    Etuval1mtab.at(0) = ValenceExpansion(-m_x, 0., kEtuval);
    Etuval1mtab.at(1) = ValenceExpansion(-m_x, 1., kEtuval);
    Etuval1mtab.at(2) = ValenceExpansion(-m_x, 2., kEtuval);
    Etuval1mtab.at(3) = ValenceExpansion(-m_x, 3., kEtuval);

    Etdval1mtab.at(0) = Etuval1mtab.at(0);
    Etdval1mtab.at(1) = Etuval1mtab.at(1);
    Etdval1mtab.at(2) = Etuval1mtab.at(2);
    Etdval1mtab.at(3) = Etuval1mtab.at(3);

}

void GPDGK19COMPASS::calculateEtKas() {
    double alpha, delta;

// gluons, not modelled by GK.

    kEtgluon = 0.;

// sea, not modelled by GK.

    kEtsea = 0.;

// u valence

    alpha = 0.45;
    delta = 0.32;
    kEtuval = delta + alpha * m_t;

// d valence

    kEtdval = kEtuval;
}

void GPDGK19COMPASS::calculateHTransCoefs() {

    calculateHTransKas();

// WARNING : No sea or gluon HTrans for GK.

    HTransuval1tab.at(0) = ValenceExpansion(m_x, 0., kHTransuval);
    HTransuval1tab.at(1) = ValenceExpansion(m_x, 0.5, kHTransuval);
    HTransuval1tab.at(2) = ValenceExpansion(m_x, 1.0, kHTransuval);
    HTransuval1tab.at(3) = ValenceExpansion(m_x, 1.5, kHTransuval);
    HTransuval1tab.at(4) = ValenceExpansion(m_x, 2.0, kHTransuval);
    HTransuval1tab.at(5) = ValenceExpansion(m_x, 2.5, kHTransuval);

    HTransdval1tab.at(0) = HTransuval1tab.at(0); // kHTransdval & kHTransuval are equal
    HTransdval1tab.at(1) = HTransuval1tab.at(1); // for u and d for HTrans,
    HTransdval1tab.at(2) = HTransuval1tab.at(2); // don't need to recalculate
    HTransdval1tab.at(3) = HTransuval1tab.at(3);
    HTransdval1tab.at(4) = HTransuval1tab.at(4);
    HTransdval1tab.at(5) = HTransuval1tab.at(5);

    HTransuval1mtab.at(0) = ValenceExpansion(-m_x, 0., kHTransuval);
    HTransuval1mtab.at(1) = ValenceExpansion(-m_x, 0.5, kHTransuval);
    HTransuval1mtab.at(2) = ValenceExpansion(-m_x, 1.0, kHTransuval);
    HTransuval1mtab.at(3) = ValenceExpansion(-m_x, 1.5, kHTransuval);
    HTransuval1mtab.at(4) = ValenceExpansion(-m_x, 2.0, kHTransuval);
    HTransuval1mtab.at(5) = ValenceExpansion(-m_x, 2.5, kHTransuval);

    HTransdval1mtab.at(0) = HTransuval1mtab.at(0);
    HTransdval1mtab.at(1) = HTransuval1mtab.at(1);
    HTransdval1mtab.at(2) = HTransuval1mtab.at(2);
    HTransdval1mtab.at(3) = HTransuval1mtab.at(3);
    HTransdval1mtab.at(4) = HTransuval1mtab.at(4);
    HTransdval1mtab.at(5) = HTransuval1mtab.at(5);

}

void GPDGK19COMPASS::calculateHTransKas() {
    double alpha, delta;

// gluons, not modelled by GK.

    kHTransgluon = 0.;

// sea, not modelled by GK.

    kHTranssea = 0.;

// u valence

    alpha = 0.45;
    delta = -0.17;
    kHTransuval = delta + alpha * m_t;

// d valence

    kHTransdval = kHTransuval;
}

void GPDGK19COMPASS::calculateETransCoefs() {

    calculateETransKas();

// WARNING : No sea or gluon ETrans for GK.

    ETransuval1tab.at(0) = ValenceExpansion(m_x, 0., kETransuval);
    ETransuval1tab.at(1) = ValenceExpansion(m_x, 1., kETransuval);
    ETransuval1tab.at(2) = ValenceExpansion(m_x, 2., kETransuval);

    ETransdval1tab.at(0) = ETransuval1tab.at(0); // kETransdval & kETransuval are equal
    ETransdval1tab.at(1) = ETransuval1tab.at(1); // for u and d for ETrans,
    ETransdval1tab.at(2) = ETransuval1tab.at(2); // don't need to recalculate

    ETransuval1mtab.at(0) = ValenceExpansion(-m_x, 0., kETransuval);
    ETransuval1mtab.at(1) = ValenceExpansion(-m_x, 1., kETransuval);
    ETransuval1mtab.at(2) = ValenceExpansion(-m_x, 2., kETransuval);

    ETransdval1mtab.at(0) = ETransuval1mtab.at(0);
    ETransdval1mtab.at(1) = ETransuval1mtab.at(1);
    ETransdval1mtab.at(2) = ETransuval1mtab.at(2);

}

void GPDGK19COMPASS::calculateETransKas() {
    double alpha, delta;

// gluons, not modelled by GK.

    kETransgluon = 0.;

// sea, not modelled by GK.

    kETranssea = 0.;

// u valence

    alpha = 0.45;
    delta = -0.10;
    kETransuval = delta + alpha * m_t;

// d valence

    kETransdval = kETransuval;
}

double GPDGK19COMPASS::ValenceExpansion(double x, double i, double k) {
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

} /* namespace PARTONS */
