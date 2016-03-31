#include "../../../../include/partons/modules/gpd/MMS13Model.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/integration/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <map>
#include <utility>

#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/utils/mstwpdf.h"

const unsigned int MMS13Model::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new MMS13Model("MMS13Model"));

const std::string MMS13Model::PARAMETER_NAME_MMS13MODEL_NHpE = "MMS13Model_NHpE";
const std::string MMS13Model::PARAMETER_NAME_MMS13MODEL_NE = "MMS13Model_NE";
const std::string MMS13Model::PARAMETER_NAME_MMS13MODEL_C = "MMS13Model_C";

MMS13Model::MMS13Model(const std::string &className) :
        GPDModule(className), MathIntegratorModule() {

    m_NE = 1;
    m_NHpE = 1;
    m_C = 1.53;

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::E, &GPDModule::computeE));

    initFunctorsForIntegrations();
}

MMS13Model::~MMS13Model() {

    if (m_Forward) {
        delete m_Forward;
        m_Forward = 0;
    }
}

void MMS13Model::initFunctorsForIntegrations() {

    m_pint_IntHpEDDval = NumA::Integrator1D::newIntegrationFunctor(this,
            &MMS13Model::IntHpEDDval);

    m_pint_IntEvalPlusAB = NumA::Integrator1D::newIntegrationFunctor(this,
            &MMS13Model::IntEvalPlusAB);

    m_pint_IntEvalPlusA = NumA::Integrator1D::newIntegrationFunctor(this,
            &MMS13Model::IntEvalPlusA);

    m_pint_IntEvalPlusB = NumA::Integrator1D::newIntegrationFunctor(this,
            &MMS13Model::IntEvalPlusB);
}

MMS13Model* MMS13Model::clone() const {
    return new MMS13Model(*this);
}

void MMS13Model::init() {

    m_Forward = new c_mstwpdf(
            ElemUtils::PropertiesManager::getInstance()->getString(
                    "grid.directory") + "mstw2008nlo.00.dat");

    setIntegrator(NumA::IntegratorType1D::GK21_ADAPTIVE);
}

void MMS13Model::configure(const ElemUtils::Parameters &parameters) {

    GPDModule::configure(parameters);

    if (parameters.isAvailable(MMS13Model::PARAMETER_NAME_MMS13MODEL_NHpE)) {
        m_NHpE = parameters.getLastAvailable().toInt();
    }

    if (parameters.isAvailable(MMS13Model::PARAMETER_NAME_MMS13MODEL_NE)) {
        m_NE = parameters.getLastAvailable().toInt();
    }

    if (parameters.isAvailable(MMS13Model::PARAMETER_NAME_MMS13MODEL_C)) {
        m_C = parameters.getLastAvailable().toDouble();
    }
}

std::string MMS13Model::toString() {
    return GPDModule::toString();
}

MMS13Model::MMS13Model(const MMS13Model& other) :
        GPDModule(other), MathIntegratorModule(other) {

    m_NE = other.m_NE;
    m_NHpE = other.m_NHpE;
    m_C = other.m_C;

    //TODO one should copy this object (requires copy constructor of c_mstwpdf, not done now at it will be replaced by a PDF service)
    m_Forward = other.m_Forward;

    initFunctorsForIntegrations();
}

void MMS13Model::isModuleWellConfigured() {
    GPDModule::isModuleWellConfigured();
}

void MMS13Model::initModule() {
    GPDModule::initModule();
}

PartonDistribution MMS13Model::computeH() {

    //variables
    double uVal, uSea, dVal, dSea, sSea, g;
    double uValMx, dValMx;

    //minus x
    double Mx = -m_x;

    //calculate
    uVal = HpEDDVal(m_x, QuarkFlavor::UP, m_NHpE)
            - EValPlus(m_x, QuarkFlavor::UP, m_NE) + DTerm(m_x);
    dVal = HpEDDVal(m_x, QuarkFlavor::DOWN, m_NHpE)
            - EValPlus(m_x, QuarkFlavor::DOWN, m_NE) + DTerm(m_x);

    uSea = 0.;
    dSea = 0.;
    sSea = 0.;

    g = 0.;

    uValMx = HpEDDVal(Mx, QuarkFlavor::UP, m_NHpE)
            - EValPlus(Mx, QuarkFlavor::UP, m_NE) + DTerm(Mx);
    dValMx = HpEDDVal(Mx, QuarkFlavor::DOWN, m_NHpE)
            - EValPlus(Mx, QuarkFlavor::DOWN, m_NE) + DTerm(Mx);

    //store
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

PartonDistribution MMS13Model::computeE() {

    //variables
    double uVal, uSea, dVal, dSea, sSea, g;
    double uValMx, dValMx;

    //minus x
    double Mx = -m_x;

    //calculate
    uVal = EValPlus(m_x, QuarkFlavor::UP, m_NE) - DTerm(m_x);
    dVal = EValPlus(m_x, QuarkFlavor::DOWN, m_NE) - DTerm(m_x);

    uSea = 0.;
    dSea = 0.;
    sSea = 0.;

    g = 0.;

    uValMx = EValPlus(Mx, QuarkFlavor::UP, m_NE) - DTerm(Mx);
    dValMx = EValPlus(Mx, QuarkFlavor::DOWN, m_NE) - DTerm(Mx);

    //store
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

double MMS13Model::forwardHval(double beta, QuarkFlavor::Type flavor) const {

    //check x
    if (beta <= 0.) {
        error(__FUNCTION__, ElemUtils::Formatter() << "Illegal x " << beta);
    }

    //parameters
    double alpha_prim;

    //update pdf
    m_Forward->update(beta, sqrt(m_MuF2));

    //check flavor
    switch (flavor) {

    case QuarkFlavor::UP: {

        alpha_prim = 0.9;

        return pow(beta, -1 * alpha_prim * m_t) * m_Forward->cont.upv / beta;
    }
        break;

    case QuarkFlavor::DOWN: {

        alpha_prim = 0.9;

        return pow(beta, -1 * alpha_prim * m_t) * m_Forward->cont.dnv / beta;
    }
        break;

    default: {
        error(__FUNCTION__,
                ElemUtils::Formatter() << "Flavor "
                        << QuarkFlavor(flavor).toString()
                        << " not supported by this function");
        return 0.;
    }
        break;
    }

    return 0.;
}

double MMS13Model::forwardEval(double beta, QuarkFlavor::Type flavor) const {

    //check x
    if (beta <= 0.) {
        error(__FUNCTION__, ElemUtils::Formatter() << "Illegal x " << beta);
    }

    //parameters
    double kappa, nu, mu, alpha_prim;

    //check flavor
    switch (flavor) {

    case QuarkFlavor::UP: {

        alpha_prim = 0.9;

        kappa = 1.67;
        nu = 4;
        mu = 0.48;

        return pow(beta, -1 * alpha_prim * m_t)
                * forwardEvalFunction(beta, kappa, nu, mu);
    }
        break;

    case QuarkFlavor::DOWN: {

        alpha_prim = 0.9;

        kappa = -2.03;
        nu = 5.6;
        mu = 0.48;

        return pow(beta, -1 * alpha_prim * m_t)
                * forwardEvalFunction(beta, kappa, nu, mu);
    }
        break;

    default: {
        error(__FUNCTION__,
                ElemUtils::Formatter() << "Flavor "
                        << QuarkFlavor(flavor).toString()
                        << " not supported by this function");
        return 0.;
    }
        break;
    }

    return 0.;
}

double MMS13Model::forwardEvalFunction(double beta, double kappa, double nu,
        double mu) const {
    return kappa * pow(beta, -1 * mu) * pow(1. - beta, nu)
            * tgamma(2. - mu + nu) / tgamma(1. - mu) / tgamma(1. + nu);
}

double MMS13Model::profileFunction(double beta, double alpha, int N) {
    return tgamma(N + 1.5) * pow(pow(1. - fabs(beta), 2) - pow(alpha, 2), N)
            / tgamma(N + 1) / pow(1. - fabs(beta), 2 * N + 1) / sqrt(M_PI);
}

double MMS13Model::DTerm(double x) const {

    if (fabs(x) < m_xi) {

        double alpha = x / m_xi;

        return m_C * alpha * (1. - pow(alpha, 2));
    } else {
        return 0.;
    }
}

double MMS13Model::HpEDDVal(double x, QuarkFlavor::Type flavor, int N) {

    //set parameters
    std::vector<double> parameters;

    parameters.push_back(x);
    parameters.push_back(static_cast<int>(flavor));
    parameters.push_back(N);

    //limits
    double limit1 = (x - m_xi) / (1. - m_xi);
    double limit2 = (x + m_xi) / (1. + m_xi);

    double limit3 = 0.;
    double limit4 = (x + m_xi) / (1. + m_xi);

    //integrate
    if (x >= m_xi) {
        return integrate(m_pint_IntHpEDDval, limit1, limit2, parameters);
    } else if ((-m_xi < x) && (x < m_xi)) {
        return integrate(m_pint_IntHpEDDval, limit3, limit4, parameters);
    } else {
        return 0.;
    }
}

double MMS13Model::EValPlus(double x, QuarkFlavor::Type flavor, int N) {

    //set parameters
    std::vector<double> parameters;

    parameters.push_back(x);
    parameters.push_back(static_cast<int>(flavor));
    parameters.push_back(N);

    //limits
    double limit1 = (x - m_xi) / (1. - m_xi);
    double limit2 = (x + m_xi) / (1. + m_xi);

    double limit3 = 0.;
    double limit4A = (x + m_xi) / (1. + m_xi);
    double limit4B = (m_xi - fabs(x)) / m_xi;

    //integrate
    if (x >= m_xi) {
        return x * integrate(m_pint_IntEvalPlusA, limit1, limit2, parameters);
    } else if ((-m_xi < x) && (x < m_xi)) {

        if (limit4A > limit4B) {
            return x
                    * (integrate(m_pint_IntEvalPlusAB, limit3, limit4B,
                            parameters)
                            + integrate(m_pint_IntEvalPlusA, limit4B, limit4A,
                                    parameters));
        } else {
            return x
                    * (integrate(m_pint_IntEvalPlusAB, limit3, limit4A,
                            parameters)
                            - integrate(m_pint_IntEvalPlusB, limit4A, limit4B,
                                    parameters));
        }

    } else {
        return 0.;
    }
}

double MMS13Model::IntHpEDDval(double beta, std::vector<double> par) {
    return (1 / m_xi)
            * (forwardHval(beta, static_cast<QuarkFlavor::Type>(int(par[1])))
                    + forwardEval(beta,
                            static_cast<QuarkFlavor::Type>(int(par[1]))))
            * profileFunction(beta, (par[0] - beta) / m_xi, int(par[2]));
}

double MMS13Model::IntEvalPlusAB(double beta, std::vector<double> par) {

    if (beta == 0) {
        return 0.;
    } else {
        return IntEvalPlusA(beta, par) - IntEvalPlusB(beta, par);
    }
}

double MMS13Model::IntEvalPlusA(double beta, std::vector<double> par) {
    return (1 / m_xi)
            * forwardEval(beta, static_cast<QuarkFlavor::Type>(int(par[1])))
            * profileFunction(beta, (par[0] - beta) / m_xi, int(par[2])) / beta;
}

double MMS13Model::IntEvalPlusB(double beta, std::vector<double> par) {
    return (1 / m_xi)
            * forwardEval(beta, static_cast<QuarkFlavor::Type>(int(par[1])))
            * profileFunction(beta, par[0] / m_xi, int(par[2])) / beta;
}

