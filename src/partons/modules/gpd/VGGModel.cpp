#include "../../../../include/partons/modules/gpd/VGGModel.h"

#include <math.h>
#include <NumA/integration/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <map>
#include <stdexcept>
#include <utility>

#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/utils/mstwpdf.h"
#include "../../../../include/partons/utils/ParameterList.h"
#include "../../../../include/partons/utils/PropertiesManager.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"

const unsigned int VGGModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new VGGModel("VGGModel"));

//TODO try to avoid global variables
const double VGGModel::eps_doubleint = 0.001;
const double VGGModel::kappa_u = 1.6596;
const double VGGModel::kappa_d = -2.0352;

VGGModel::VGGModel(const std::string &className) :
        GPDModule(className), MathIntegratorModule() {

    b_profile_val = -1.;
    b_profile_sea = -1.;
    alphap_val = -1.;
    alphap_sea = -1.;
    eta_e_largex_u_s = -1.;
    eta_e_largex_d_s = -1.;

    m_Forward = 0;

    gpd_s5 = GPDType::UNDEFINED;
    flavour_s5 = undefined;
    x_s5 = -1.;

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::E, &GPDModule::computeE));

    initFunctorsForIntegrations();
}

VGGModel::~VGGModel() {
    if (m_Forward) {
        delete m_Forward;
        m_Forward = 0;
    }

    if (m_pInt_mom2_up_valence_e) {
        delete m_pInt_mom2_up_valence_e;
        m_pInt_mom2_up_valence_e = 0;
    }
}

void VGGModel::initFunctorsForIntegrations() {
    m_pInt_mom2_up_valence_e = NumA::Integrator1D::newIntegrationFunctor(this,
            &VGGModel::int_mom2_up_valence_e);
}

VGGModel* VGGModel::clone() const {
    return new VGGModel(*this);
}

void VGGModel::init() {

    //TODO are the same for GPDs H and E?
    b_profile_val = 1.;
    b_profile_sea = 1.;
    alphap_val = 1.105;
    alphap_sea = 1.105;
    eta_e_largex_u_s = 1.713;
    eta_e_largex_d_s = 0.566;

    setIntegrator(NumA::IntegratorType1D::GK21_ADAPTIVE);

    m_Forward = new c_mstwpdf(
            PropertiesManager::getInstance()->getString("grid.directory")
                    + "mstw2008nlo.00.dat");
}

void VGGModel::configure(ParameterList parameters) {
}

std::string VGGModel::toString() {
    return GPDModule::toString();
}

VGGModel::VGGModel(const VGGModel& other) :
        GPDModule(other), MathIntegratorModule(other) {

    b_profile_val = other.b_profile_val;
    b_profile_sea = other.b_profile_sea;
    alphap_val = other.alphap_val;
    alphap_sea = other.alphap_sea;
    eta_e_largex_u_s = other.eta_e_largex_u_s;
    eta_e_largex_d_s = other.eta_e_largex_d_s;

    m_Forward = other.m_Forward;

    gpd_s5 = other.gpd_s5;
    flavour_s5 = other.flavour_s5;
    x_s5 = other.x_s5;

    initFunctorsForIntegrations();
}

void VGGModel::isModuleWellConfigured() {
    GPDModule::isModuleWellConfigured();

    if (b_profile_val == -1.)
        throw std::runtime_error("[VGGModel] Unknown b_profile_val");

    if (b_profile_sea == -1.)
        throw std::runtime_error("[VGGModel] Unknown b_profile_sea");

    if (alphap_val == -1.)
        throw std::runtime_error("[VGGModel] Unknown alphap_val");

    if (alphap_sea == -1.)
        throw std::runtime_error("[VGGModel] Unknown alphap_sea");

    if (eta_e_largex_u_s == -1.)
        throw std::runtime_error("[VGGModel] Unknown eta_e_largex_u_s");

    if (eta_e_largex_d_s == -1.)
        throw std::runtime_error("[VGGModel] Unknown eta_e_largex_d_s");

    if (getMathIntegrator() == 0)
        throw std::runtime_error("[VGGModel] MathIntegrationMode is UNDEFINED");

    if (m_Forward == NULL)
        throw std::runtime_error("[VGGModel] PDF module is NULL");
}

void VGGModel::initModule() {

    GPDModule::initModule();
}

PartonDistribution VGGModel::computeH() {

    //GPD
    gpd_s5 = GPDType::H;

    //variables
    double uVal, uSea, dVal, dSea, sSea, g;
    double uValMx, dValMx;

    //GPDs for x
    x_s5 = m_x;

    flavour_s5 = upv;
    uVal = offforward_distr();

    flavour_s5 = dnv;
    dVal = offforward_distr();

    flavour_s5 = upsea;
    uSea = offforward_distr();

    flavour_s5 = dnsea;
    dSea = offforward_distr();

    sSea = 0.;
    g = 0.;

    //GPDs for -x
    x_s5 = -m_x;

    flavour_s5 = upv;
    uValMx = offforward_distr();

    flavour_s5 = dnv;
    dValMx = offforward_distr();

    //store results
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

PartonDistribution VGGModel::computeE() {
    std::vector<double> emptyParameters;

    //GPD
    gpd_s5 = GPDType::E;

    //variables
    double uVal, uSea, dVal, dSea, sSea, g;
    double uValMx, dValMx;

    //GPDs for x
    x_s5 = m_x;
    flavour_s5 = upv;
    uVal = kappa_u * offforward_distr()
            / integrate(m_pInt_mom2_up_valence_e, 0., 1., emptyParameters);

    flavour_s5 = dnv;
    dVal = kappa_d * offforward_distr()
            / integrate(m_pInt_mom2_up_valence_e, 0., 1., emptyParameters);

    uSea = 0.;
    dSea = 0.;
    sSea = 0.;
    g = 0.;

    //GPDs for -x
    x_s5 = -m_x;

    flavour_s5 = upv;
    uValMx = offforward_distr();

    flavour_s5 = dnv;
    dValMx = offforward_distr();

    //store results
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

double VGGModel::offforward_distr() {
    std::vector<double> emptyParameters;

    //result
    double ofpd;

    //integrated function according to variant
    double (VGGModel::*f_dist)(double a, std::vector<double> b) = NULL;
    double (VGGModel::*f_distMx)(double a, std::vector<double> b) = NULL;

    //GPD
    switch (gpd_s5) {

    case GPDType::H: {

        f_dist = &VGGModel::int_symm_double_distr_reggeH;
        f_distMx = &VGGModel::int_symm_double_distr_reggeMxH;

    }
        break;

    case GPDType::E: {

        f_dist = &VGGModel::int_symm_double_distr_reggeE;
        f_distMx = &VGGModel::int_symm_double_distr_reggeMxE;

    }
        break;

    default: {

        throw std::runtime_error(
                Formatter() << "[VGGModel::offforward_distr()] GPD = " << gpd_s5
                        << " not defined");
    }
        break;
    }

    //TODO fuite memoire ; pointer allouer non stop, mais pas desallouer
    NumA::FunctionType1D* f_dist_OneDimensionFunctionType =
            NumA::Integrator1D::newIntegrationFunctor(this, f_dist);

    //TODO fuite memoire ; pointer allouer non stop, mais pas desallouer
    NumA::FunctionType1D* f_distMx_OneDimensionFunctionType =
            NumA::Integrator1D::newIntegrationFunctor(this, f_distMx);

    //three ranges of x
    if (x_s5 >= m_xi) {

        ofpd = integrate(f_dist_OneDimensionFunctionType,
                -(1. - x_s5) / (1. + m_xi), (1. - x_s5) / (1. - m_xi),
                emptyParameters);

    } else if ((-m_xi < x_s5) && (x_s5 < m_xi)) {

        ofpd = integrate(f_dist_OneDimensionFunctionType,
                -(1. - x_s5) / (1. + m_xi), x_s5 / m_xi - eps_doubleint,
                emptyParameters);

        if (flavour_s5 != upv && flavour_s5 != dnv) {
            ofpd -= integrate(f_distMx_OneDimensionFunctionType,
                    -(1. + x_s5) / (1. + m_xi), -x_s5 / m_xi - eps_doubleint,
                    emptyParameters);
        }

    } else {
        if (flavour_s5 != upv && flavour_s5 != dnv) {
            ofpd = -integrate(f_distMx_OneDimensionFunctionType,
                    -(1. + x_s5) / (1. + m_xi), (1. + x_s5) / (1. - m_xi),
                    emptyParameters);
        } else {
            ofpd = 0.;
        }
    }

    return ofpd;
}

double VGGModel::symm_double_distr_reggeH(double beta, double alpha) {

    //check beta range
    if (beta <= 0.) {
        throw std::runtime_error(
                Formatter() << "[VGGModel::symm_double_distr_reggeH()] alpha = "
                        << alpha << ", beta = " << beta
                        << ", argument 0 or negative");
    }

    //update and get pdf
    m_Forward->update(beta, sqrt(m_MuF2));

    double pdf = -1.;
    double b_profile = -1.;
    double funcbetat = -1.;

    switch (flavour_s5) {

    case upv: {

        pdf = m_Forward->cont.upv / beta;
        //pdf = test_pdf_up_val(beta);
        b_profile = b_profile_val;
        funcbetat = pow(1. / fabs(beta), (1. - beta) * alphap_val * m_t);
    }
        break;

    case dnv: {

        pdf = m_Forward->cont.dnv / beta;
        //pdf = test_pdf_down_val(beta);
        b_profile = b_profile_val;
        funcbetat = pow(1. / fabs(beta), (1. - beta) * alphap_val * m_t);
    }
        break;

    case upsea: {

        pdf = m_Forward->cont.usea / beta;
        //pdf = test_pdf_up_bar(beta);
        b_profile = b_profile_sea;
        funcbetat = pow(1. / fabs(beta), (1. - beta) * alphap_sea * m_t);
    }
        break;

    case dnsea: {

        pdf = m_Forward->cont.dsea / beta;
        //pdf = test_pdf_down_bar(beta);
        b_profile = b_profile_sea;
        funcbetat = pow(1. / fabs(beta), (1. - beta) * alphap_sea * m_t);
    }
        break;

    default: {

        throw std::runtime_error(
                Formatter()
                        << "[VGGModel::symm_double_distr_reggeH()] Flavour = "
                        << flavour_s5 << " not defined");
    }
        break;

    }

    return symm_profile_function(beta, alpha, b_profile) * pdf * funcbetat;
}

double VGGModel::symm_double_distr_reggeE(double beta, double alpha) {

    //check beta range
    if (beta <= 0.) {
        throw std::runtime_error(
                Formatter() << "[VGGModel::symm_double_distr_reggeE()] alpha = "
                        << alpha << ", beta = " << beta
                        << ", argument 0 or negative");
    }

    //update and get pdf
    m_Forward->update(beta, sqrt(m_MuF2));

    double pdf = -1.;
    double b_profile = -1.;
    double funcbetat = -1.;

    switch (flavour_s5) {

    case upv: {

        pdf = m_Forward->cont.upv / beta;
        //pdf = test_pdf_up_val(beta);
        b_profile = b_profile_val;
        funcbetat = pow(1. - beta, eta_e_largex_u_s)
                * pow(1. / fabs(beta), (1. - beta) * alphap_val * m_t);
    }
        break;

    case dnv: {

        pdf = m_Forward->cont.dnv / beta;
        //pdf = test_pdf_down_val(beta);
        b_profile = b_profile_val;
        funcbetat = pow(1. - beta, eta_e_largex_d_s)
                * pow(1. / fabs(beta), (1. - beta) * alphap_val * m_t);
    }
        break;

    default: {

        throw std::runtime_error(
                Formatter()
                        << "[VGGModel::symm_double_distr_reggeE()] Flavour = "
                        << flavour_s5 << " not defined");
    }
        break;

    }

    return symm_profile_function(beta, alpha, b_profile) * pdf * funcbetat;
}

double VGGModel::symm_profile_function(double beta, double alpha,
        double b_profile) {

    double norm, bidon;

    norm = 1. / pow(2., 2. * b_profile + 1.) * tgamma(2. * b_profile + 2.)
            / pow(tgamma(b_profile + 1.), 2.);

    bidon = pow(1. - fabs(beta), 2.) - pow(alpha, 2.);
    if (bidon < 0.)
        bidon = 0.;

    return norm * pow(bidon, b_profile)
            / pow(1. - fabs(beta), 2. * b_profile + 1.);
}

double VGGModel::int_symm_double_distr_reggeH(double alpha,
        std::vector<double> par) {
    return symm_double_distr_reggeH(x_s5 - m_xi * alpha, alpha);
}

double VGGModel::int_symm_double_distr_reggeMxH(double alpha,
        std::vector<double> par) {
    return symm_double_distr_reggeH(-x_s5 - m_xi * alpha, alpha);
}

double VGGModel::int_symm_double_distr_reggeE(double alpha,
        std::vector<double> par) {
    return symm_double_distr_reggeE(x_s5 - m_xi * alpha, alpha);
}

double VGGModel::int_symm_double_distr_reggeMxE(double alpha,
        std::vector<double> par) {
    return symm_double_distr_reggeE(-x_s5 - m_xi * alpha, alpha);
}

double VGGModel::int_mom2_up_valence_e(double x, std::vector<double> par) {

    //value
    double beta = x;

    //check beta range
    if (beta <= 0.) {
        throw std::runtime_error(
                Formatter() << "[VGGModel::int_mom2_up_valence_e()] x = "
                        << beta << ", argument 0 or negative");
    }

    //update and get pdf
    m_Forward->update(beta, sqrt(m_MuF2));

    double pdf = -1.;
    double eta_e_largex_s = -1.;

    switch (flavour_s5) {

    case upv: {

        pdf = m_Forward->cont.upv / beta;
        //pdf = test_pdf_up_val(beta);
        eta_e_largex_s = eta_e_largex_u_s;
    }
        break;

    case dnv: {

        pdf = m_Forward->cont.dnv / beta;
        //pdf = test_pdf_down_val(beta);
        eta_e_largex_s = eta_e_largex_d_s;

    }
        break;

    default: {

        throw std::runtime_error(
                Formatter() << "[VGGModel::int_mom2_up_valence_e()] Flavour = "
                        << flavour_s5 << " not defined");
    }
        break;

    }

    return pow(1. - beta, eta_e_largex_s) * pdf;
}

double VGGModel::test_pdf_down_val(double x) {

    double n3, n4, ed, gd, norm;

    n3 = 0.78;
    n4 = 4.57;
    ed = -0.87;
    gd = 0.82;
    norm = 0.263275;

    return 1. / norm * 1. / pow(x, 1. - n3) * pow(1. - x, n4)
            * (1. + ed * pow(x, 0.5) + gd * x);
}

double VGGModel::test_pdf_up_val(double x) {

    double n1, n2, eud, gud, norm;

    n1 = 0.26;
    n2 = 3.82;
    eud = 14.4;
    gud = 16.99;
    norm = 9.790758;

    return 3. / norm * 1. / pow(x, 1. - n1) * pow(1. - x, n2)
            * (1. + eud * pow(x, 0.5) + gud * x) - test_pdf_down_val(x);
}

double VGGModel::test_pdf_sea(double x) {

    double ns, es, gs, As;

    ns = 10.;
    es = -2.21;
    gs = 6.22;
    As = 1.87;

    return As * 1. / x * pow(1. - x, ns) * (1. + es * pow(x, 0.5) + gs * x);
}

double VGGModel::test_pdf_up_bar(double x) {
    return 1. / 5. * test_pdf_sea(x);
}

double VGGModel::test_pdf_down_bar(double x) {
    return 1. / 5. * test_pdf_sea(x);
}

