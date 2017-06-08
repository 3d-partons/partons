#include "../../../../include/partons/modules/gpd/VGGModel.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <math.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <map>
#include <utility>

#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/utils/MSTWPDF.h"
#include "../../../../include/partons/utils/PartonContent.h"

const unsigned int VGGModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new VGGModel("VGGModel"));

VGGModel::VGGModel(const std::string &className) :
        GPDModule(className), kappa_u(1.6596), kappa_d(-2.0352), b_profile_val(
                1.), b_profile_sea(1.), alphap_val(1.105), alphap_sea(1.105), eta_e_largex_u_s(
                1.713), eta_e_largex_d_s(0.566), g_AXIAL(1.267), m_Forward(0), MathIntegratorModule() {

    gpd_s5 = GPDType::UNDEFINED;
    flavour_s5 = UNDEFINED;
    x_s5 = -1.;

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::E, &GPDModule::computeE));

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Ht, &GPDModule::computeHt));

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Et, &GPDModule::computeEt));

    initFunctorsForIntegrations();
}

VGGModel::VGGModel(const VGGModel& other) :
        kappa_u(1.6596), kappa_d(-2.0352), b_profile_val(1.), b_profile_sea(1.), alphap_val(
                1.105), alphap_sea(1.105), eta_e_largex_u_s(1.713), eta_e_largex_d_s(
                0.566), g_AXIAL(1.267), GPDModule(other), MathIntegratorModule(
                other) {

    //TODO make a clone instance ; create MSTWPDF as a module.
    m_Forward = new MSTWPDF();
    m_Forward->init(
            ElemUtils::PropertiesManager::getInstance()->getString(
                    "grid.directory") + "mstw2008nlo.00.dat");

    gpd_s5 = other.gpd_s5;
    flavour_s5 = other.flavour_s5;
    x_s5 = other.x_s5;

    initFunctorsForIntegrations();
}

VGGModel::~VGGModel() {

    if (m_Forward) {
        delete m_Forward;
        m_Forward = 0;
    }

    if (m_pint_symm_double_distr_reggeH) {
        delete m_pint_symm_double_distr_reggeH;
        m_pint_symm_double_distr_reggeH = 0;
    }

    if (m_pint_symm_double_distr_reggeMxH) {
        delete m_pint_symm_double_distr_reggeMxH;
        m_pint_symm_double_distr_reggeMxH = 0;
    }

    if (m_pint_symm_double_distr_reggeE) {
        delete m_pint_symm_double_distr_reggeE;
        m_pint_symm_double_distr_reggeE = 0;
    }

    if (m_pint_symm_double_distr_reggeMxE) {
        delete m_pint_symm_double_distr_reggeMxE;
        m_pint_symm_double_distr_reggeMxE = 0;
    }

    if (m_pint_symm_double_distr_reggeHt) {
        delete m_pint_symm_double_distr_reggeHt;
        m_pint_symm_double_distr_reggeHt = 0;
    }

    if (m_pint_symm_double_distr_reggeMxHt) {
        delete m_pint_symm_double_distr_reggeMxHt;
        m_pint_symm_double_distr_reggeMxHt = 0;
    }

    if (m_pint_mom2_up_valence_e) {
        delete m_pint_mom2_up_valence_e;
        m_pint_mom2_up_valence_e = 0;
    }
}

void VGGModel::initFunctorsForIntegrations() {

    m_pint_symm_double_distr_reggeH = NumA::Integrator1D::newIntegrationFunctor(
            this, &VGGModel::int_symm_double_distr_reggeH);

    m_pint_symm_double_distr_reggeMxH =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &VGGModel::int_symm_double_distr_reggeMxH);

    m_pint_symm_double_distr_reggeE = NumA::Integrator1D::newIntegrationFunctor(
            this, &VGGModel::int_symm_double_distr_reggeE);

    m_pint_symm_double_distr_reggeMxE =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &VGGModel::int_symm_double_distr_reggeMxE);

    m_pint_symm_double_distr_reggeHt =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &VGGModel::int_symm_double_distr_reggeHt);

    m_pint_symm_double_distr_reggeMxHt =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &VGGModel::int_symm_double_distr_reggeMxHt);

    m_pint_mom2_up_valence_e = NumA::Integrator1D::newIntegrationFunctor(this,
            &VGGModel::int_mom2_up_valence_e);
}

VGGModel* VGGModel::clone() const {
    return new VGGModel(*this);
}

void VGGModel::resolveObjectDependencies() {
    setIntegrator(NumA::IntegratorType1D::DEXP);
}

void VGGModel::configure(const ElemUtils::Parameters &parameters) {
    GPDModule::configure(parameters);
}

std::string VGGModel::toString() const {
    return GPDModule::toString();
}

void VGGModel::isModuleWellConfigured() {
    GPDModule::isModuleWellConfigured();
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

    flavour_s5 = UP_VAL;
    uVal = offforward_distr();

    flavour_s5 = DOWN_VAL;
    dVal = offforward_distr();

    flavour_s5 = UP_SEA;
    uSea = offforward_distr();

    flavour_s5 = DOWN_SEA;
    dSea = offforward_distr();

    sSea = 0.;
    g = 0.;

    //GPDs for -x
    x_s5 = -m_x;

    flavour_s5 = UP_VAL;
    uValMx = offforward_distr();

    flavour_s5 = DOWN_VAL;
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

    //return
    return partonDistribution;
}

PartonDistribution VGGModel::computeE() {

    //GPD
    gpd_s5 = GPDType::E;

    //variables
    double uVal, uSea, dVal, dSea, sSea, g;
    double uValMx, dValMx;

    std::vector<double> emptyParameters;

    //GPDs for x
    x_s5 = m_x;
    flavour_s5 = UP_VAL;
    uVal = kappa_u * offforward_distr()
            / integrate(m_pint_mom2_up_valence_e, 0., 1., emptyParameters);

    flavour_s5 = DOWN_VAL;
    dVal = kappa_d * offforward_distr()
            / integrate(m_pint_mom2_up_valence_e, 0., 1., emptyParameters);

    uSea = 0.;
    dSea = 0.;
    sSea = 0.;
    g = 0.;

    //GPDs for -x
    x_s5 = -m_x;

    flavour_s5 = UP_VAL;
    uValMx = kappa_u * offforward_distr()
            / integrate(m_pint_mom2_up_valence_e, 0., 1., emptyParameters);

    flavour_s5 = DOWN_VAL;
    dValMx = kappa_d * offforward_distr()
            / integrate(m_pint_mom2_up_valence_e, 0., 1., emptyParameters);

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

    //return
    return partonDistribution;
}

PartonDistribution VGGModel::computeHt() {

    //GPD
    gpd_s5 = GPDType::Ht;

    //variables
    double uVal, uSea, dVal, dSea, sSea, g;
    double uValMx, dValMx;

    //form factor
    double formfactor = 1 / g_AXIAL * form_factor_G_A(-m_t);

    //GPDs for x
    x_s5 = m_x;

    flavour_s5 = UP_VAL;
    uVal = formfactor * offforward_pol_distr();

    flavour_s5 = DOWN_VAL;
    dVal = formfactor * offforward_pol_distr();

    flavour_s5 = UP_SEA;
    uSea = formfactor * offforward_pol_distr();

    flavour_s5 = DOWN_SEA;
    dSea = formfactor * offforward_pol_distr();

    flavour_s5 = STRANGE;
    sSea = formfactor * offforward_pol_distr();

    g = 0.;

    //GPDs for -x
    x_s5 = -m_x;

    flavour_s5 = UP_VAL;
    uValMx = formfactor * offforward_pol_distr();

    flavour_s5 = DOWN_VAL;
    dValMx = formfactor * offforward_pol_distr();

    //store results
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);
    quarkDistribution_s.setQuarkDistribution(sSea);

    quarkDistribution_u.setQuarkDistributionPlus(uVal + uValMx);
    quarkDistribution_d.setQuarkDistributionPlus(dVal + dValMx);
    quarkDistribution_s.setQuarkDistributionPlus(0.);

    quarkDistribution_u.setQuarkDistributionMinus(uVal - uValMx + 2 * uSea);
    quarkDistribution_d.setQuarkDistributionMinus(dVal - dValMx + 2 * dSea);
    quarkDistribution_s.setQuarkDistributionMinus(2 * sSea);

    GluonDistribution gluonDistribution(g);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    //return
    return partonDistribution;
}

PartonDistribution VGGModel::computeEt() {

    //GPD
    gpd_s5 = GPDType::Et;

    //variables
    double uVal, uSea, dVal, dSea, sSea, g;
    double uValMx, dValMx;

    //GPDs for x
    x_s5 = m_x;

    uVal = (fabs(x_s5) < m_xi) ?
            (0.5 * form_factor_G_P(m_t) / m_xi * 0.75
                    * (1. - pow(x_s5 / m_xi, 2))) :
            (0.);
    uSea = 0;

    dVal = -uVal;
    dSea = 0;

    sSea = 0.;
    g = 0.;

    //GPDs for -x
    x_s5 = -m_x;

    uValMx =
            (fabs(x_s5) < m_xi) ?
                    (0.5 * form_factor_G_P(m_t) / m_xi * 0.75
                            * (1. - pow(x_s5 / m_xi, 2))) :
                    (0.);
    dValMx = -uValMx;

    //store results
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);
    quarkDistribution_s.setQuarkDistribution(sSea);

    quarkDistribution_u.setQuarkDistributionPlus(uVal + uValMx);
    quarkDistribution_d.setQuarkDistributionPlus(dVal + dValMx);
    quarkDistribution_s.setQuarkDistributionPlus(0.);

    quarkDistribution_u.setQuarkDistributionMinus(uVal - uValMx + 2 * uSea);
    quarkDistribution_d.setQuarkDistributionMinus(dVal - dValMx + 2 * dSea);
    quarkDistribution_s.setQuarkDistributionMinus(2 * sSea);

    GluonDistribution gluonDistribution(g);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    //return
    return partonDistribution;
}

double VGGModel::offforward_distr() {

    //result
    double ofpd;

    //integrated function according to variant
    NumA::FunctionType1D* functor;
    NumA::FunctionType1D* functorMx;

    std::vector<double> emptyParameters;

    //epsilon
    double eps_doubleint;

    //GPD
    switch (gpd_s5) {

    case GPDType::H: {

        functor = m_pint_symm_double_distr_reggeH;
        functorMx = m_pint_symm_double_distr_reggeMxH;

        eps_doubleint = 1.E-3;
    }
        break;

    case GPDType::E: {

        functor = m_pint_symm_double_distr_reggeE;
        functorMx = m_pint_symm_double_distr_reggeMxE;

        eps_doubleint = 1.E-5;
    }
        break;

    default: {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "GPD = " << gpd_s5 << " not defined");
    }
        break;
    }

    //three ranges of x
    if (x_s5 >= m_xi) {

        ofpd = integrate(functor, -(1. - x_s5) / (1. + m_xi),
                (1. - x_s5) / (1. - m_xi), emptyParameters);

    } else if ((-m_xi < x_s5) && (x_s5 < m_xi)) {

        ofpd = integrate(functor, -(1. - x_s5) / (1. + m_xi),
                x_s5 / m_xi - eps_doubleint, emptyParameters);

        if (flavour_s5 != UP_VAL && flavour_s5 != DOWN_VAL) {

            ofpd -= integrate(functorMx, -(1. + x_s5) / (1. + m_xi),
                    -x_s5 / m_xi - eps_doubleint, emptyParameters);
        }

    } else {
        if (flavour_s5 != UP_VAL && flavour_s5 != DOWN_VAL) {

            ofpd = -integrate(functorMx, -(1. + x_s5) / (1. + m_xi),
                    (1. + x_s5) / (1. - m_xi), emptyParameters);
        } else {
            ofpd = 0.;
        }
    }

    //return
    return ofpd;
}

double VGGModel::symm_double_distr_reggeH(double beta, double alpha) {

    //check beta range
    if (beta <= 0.) {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "alpha = " << alpha << ", beta = "
                        << beta << ", argument 0 or negative");
    }

    //update and get pdf
    m_Forward->update(beta, sqrt(m_MuF2));

    double pdf = -1.;
    double b_profile = -1.;
    double funcbetat = -1.;

    switch (flavour_s5) {

    case UP_VAL: {

        pdf = m_Forward->getPartonContent().getUpv() / beta;
//        pdf = test_pdf_up_val(beta);
        b_profile = b_profile_val;
        funcbetat = pow(1. / fabs(beta), (1. - beta) * alphap_val * m_t);
    }
        break;

    case DOWN_VAL: {

        pdf = m_Forward->getPartonContent().getDnv() / beta;
//        pdf = test_pdf_down_val(beta);
        b_profile = b_profile_val;
        funcbetat = pow(1. / fabs(beta), (1. - beta) * alphap_val * m_t);
    }
        break;

    case UP_SEA: {

        pdf = m_Forward->getPartonContent().getUsea() / beta;
//        pdf = test_pdf_up_bar(beta);
        b_profile = b_profile_sea;
        funcbetat = pow(1. / fabs(beta), (1. - beta) * alphap_sea * m_t);
    }
        break;

    case DOWN_SEA: {

        pdf = m_Forward->getPartonContent().getDsea() / beta;
//        pdf = test_pdf_down_bar(beta);
        b_profile = b_profile_sea;
        funcbetat = pow(1. / fabs(beta), (1. - beta) * alphap_sea * m_t);
    }
        break;

    default: {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Flavour = " << flavour_s5
                        << " not defined");
    }
        break;

    }

    //return
    return symm_profile_function(beta, alpha, b_profile) * pdf * funcbetat;
}

double VGGModel::symm_double_distr_reggeE(double beta, double alpha) {

    //check beta range
    if (beta <= 0.) {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "alpha = " << alpha << ", beta = "
                        << beta << ", argument 0 or negative");
    }

    //update and get pdf
    m_Forward->update(beta, sqrt(m_MuF2));

    double pdf = -1.;
    double b_profile = -1.;
    double funcbetat = -1.;

    switch (flavour_s5) {

    case UP_VAL: {

        pdf = m_Forward->getPartonContent().getUpv() / beta;
//        pdf = test_pdf_up_val(beta);
        b_profile = b_profile_val;
        funcbetat = pow(1. - beta, eta_e_largex_u_s)
                * pow(1. / fabs(beta), (1. - beta) * alphap_val * m_t);
    }
        break;

    case DOWN_VAL: {

        pdf = m_Forward->getPartonContent().getDnv() / beta;
//        pdf = test_pdf_down_val(beta);
        b_profile = b_profile_val;
        funcbetat = pow(1. - beta, eta_e_largex_d_s)
                * pow(1. / fabs(beta), (1. - beta) * alphap_val * m_t);
    }
        break;

    default: {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Flavour = " << flavour_s5
                        << " not defined");
    }
        break;

    }

    //return
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

double VGGModel::int_mom2_up_valence_e(double beta, std::vector<double> par) {

    //check beta range
    if (beta <= 0.) {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "x = " << beta
                        << ", argument 0 or negative");
    }

    //update and get pdf
    m_Forward->update(beta, sqrt(m_MuF2));

    double pdf = -1.;
    double eta_e_largex_s = -1.;

    switch (flavour_s5) {

    case UP_VAL: {

        pdf = m_Forward->getPartonContent().getUpv() / beta;
//        pdf = test_pdf_up_val(beta);
        eta_e_largex_s = eta_e_largex_u_s;
    }
        break;

    case DOWN_VAL: {

        pdf = m_Forward->getPartonContent().getDnv() / beta;
//        pdf = test_pdf_down_val(beta);
        eta_e_largex_s = eta_e_largex_d_s;

    }
        break;

    default: {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Flavour = " << flavour_s5
                        << " not defined");
    }
        break;

    }

    //return
    return pow(1. - beta, eta_e_largex_s) * pdf;
}

double VGGModel::offforward_pol_distr() {

    //result
    double ofpd;

    //integration function
    NumA::FunctionType1D* functor;
    NumA::FunctionType1D* functorMx;

    std::vector<double> emptyParameters;

    //epsilon
    double eps_doubleint;

    //GPD
    switch (gpd_s5) {

    case GPDType::Ht: {

        functor = m_pint_symm_double_distr_reggeHt;
        functorMx = m_pint_symm_double_distr_reggeMxHt;

        eps_doubleint = 1.E-3;
    }
        break;

    default: {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "GPD = " << gpd_s5 << " not defined");
    }
        break;
    }

    //three ranges of x
    if (x_s5 >= m_xi) {

        ofpd = integrate(functor, 0., (1. - x_s5) / (1. - m_xi),
                emptyParameters);

    } else if ((-m_xi < x_s5) && (x_s5 < m_xi)) {

        ofpd = integrate(functor, 0.,
                (x_s5 + m_xi) / (2 * m_xi) - eps_doubleint, emptyParameters);

        if (flavour_s5 != UP_VAL && flavour_s5 != DOWN_VAL) {

            ofpd += integrate(functorMx, 0.,
                    (-x_s5 + m_xi) / (2 * m_xi) - eps_doubleint,
                    emptyParameters);
        }

    } else {
        if (flavour_s5 != UP_VAL && flavour_s5 != DOWN_VAL) {

            ofpd = integrate(functorMx, 0., (1. + x_s5) / (1. - m_xi),
                    emptyParameters);
        } else {
            ofpd = 0.;
        }
    }

    //return
    return 1. / (1. + m_xi) * ofpd;
}

double VGGModel::int_symm_double_distr_reggeHt(double alpha,
        std::vector<double> par) {
    return symm_double_distr_reggeHt(
            (x_s5 + m_xi) / (1. + m_xi) - 2 * m_xi / (1. + m_xi) * alpha, alpha);
}

double VGGModel::int_symm_double_distr_reggeMxHt(double alpha,
        std::vector<double> par) {
    return symm_double_distr_reggeHt(
            (-x_s5 + m_xi) / (1. + m_xi) - 2 * m_xi / (1. + m_xi) * alpha,
            alpha);
}

double VGGModel::symm_double_distr_reggeHt(double beta, double alpha) {

    //check beta range
    if (beta <= 0.) {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "alpha = " << alpha << ", beta = "
                        << beta << ", argument 0 or negative");
    }

    //get pdf
    double pdf = -1.;
    double b_profile = -1.;
    double funcbetat = -1.;

    switch (flavour_s5) {

    case UP_VAL: {

        pdf = pol_up_valence(beta);
    }
        break;

    case DOWN_VAL: {

        pdf = pol_down_valence(beta);
    }
        break;

    case UP_SEA: {

        pdf = pol_up_bar(beta);
    }
        break;

    case DOWN_SEA: {

        pdf = pol_down_bar(beta);
    }
        break;

    case STRANGE: {

        pdf = pol_strange_bar(beta);
    }
        break;

    default: {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Flavour = " << flavour_s5
                        << " not defined");
    }
        break;

    }

    //return
    return 6. * alpha * (1. - beta - alpha) * pdf / pow(1. - beta, 3);
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

double VGGModel::pol_up_valence(double x) {

    double up_valence;

    double A_u, eta_1, eta_2, eps_u, gamma_u;

    A_u = 0.6051;
    eta_1 = 0.4089;
    eta_2 = 3.395;
    eps_u = 2.078;
    gamma_u = 14.56;

    up_valence = A_u * pow(x, -1. + eta_1) * pow(1. - x, eta_2)
            * (1. + eps_u * sqrt(x) + gamma_u * x);

    double eta_u, a_u;

    eta_u = 0.918;
    a_u = 0.250;
    A_u = 0.882;

    return eta_u * A_u * pow(x, a_u) * up_valence;
}

double VGGModel::pol_down_valence(double x) {

    double down_valence;

    double A_d, eta_3, eta_4, eps_d, gamma_d;

    A_d = 0.05811;
    eta_3 = 0.2882;
    eta_4 = 3.874;
    eps_d = 34.69;
    gamma_d = 28.96;

    down_valence = A_d * pow(x, -1. + eta_3) * pow(1. - x, eta_4)
            * (1. + eps_d * sqrt(x) + gamma_d * x);

    double eta_d, a_d;

    eta_d = -0.339;
    a_d = 0.231;
    A_d = 1.768;

    return eta_d * A_d * pow(x, a_d) * down_valence;
}

double VGGModel::pol_strange_bar(double x) {

    double sea_distr;

    double lams, ns, es, gs, As;

    lams = 0.2712;
    ns = 7.808;
    es = 2.283;
    gs = 20.69;
    As = 0.2004;

    sea_distr = As * pow(x, -1. - lams) * pow(1. - x, ns)
            * (1. + es * pow(x, 0.5) + gs * x);

    double etas, as;

    etas = -0.054;
    as = 0.576;
    As = 1.6478;

    return etas * As * pow(x, as) * sea_distr;
}

double VGGModel::pol_up_bar(double x) {
    return pol_strange_bar(x);
}

double VGGModel::pol_down_bar(double x) {
    return pol_strange_bar(x);
}

double VGGModel::form_factor_G_A(double Q_sqr) {

    double M_A = 1.03;

    return g_AXIAL / pow(1. + Q_sqr / pow(M_A, 2.), 2.);
}

double VGGModel::form_factor_G_P(double t) {

    double M_pion = 0.13498;

    return g_AXIAL * pow(2. * Constant::PROTON_MASS, 2) / (-t + pow(M_pion, 2));
}

