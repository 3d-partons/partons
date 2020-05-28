#include "../../../../include/partons/modules/gpd/GPDHM18.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
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
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {

const unsigned int GPDHM18::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDHM18("GPDHM18"));

const std::string GPDHM18::PARAMETER_NAME_HM18MODEL_M = "HM18MODEL_M";
const std::string GPDHM18::PARAMETER_NAME_HM18MODEL_m = "HM18MODEL_m";
const std::string GPDHM18::PARAMETER_NAME_HM18MODEL_lambda = "HM18MODEL_lambda";
const std::string GPDHM18::PARAMETER_NAME_HM18MODEL_p = "HM18MODEL_p";

GPDHM18::GPDHM18(const std::string &className) :
        GPDModule(className), MathIntegratorModule() {

    initializeFunctorsForIntegrations();

    m_MuF2_ref = 4.;

    m_M = Constant::PROTON_MASS;
    m_m = 0.45;
    m_lambda = 0.75;
    m_p = 1.;

    /**
     * Value of m_N is calculated in GPDHM18::configure using Normalize() function.
     * In this place Normalization() function cann't be run, since it is using
     * integration mechanism, which will be initiated not sooner that in GPDHM18::configure.
     * In order to not to leave the value of m_N not initialize the value returned
     * by Normalization() with initial values of model parameters has been used.
     */
    m_N = 0.048900116463331;

    //relate a specific GPD type with the appropriate function
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Ht, &GPDModule::computeHt));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::E, &GPDModule::computeE));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Et, &GPDModule::computeEt));
}

GPDHM18::GPDHM18(const GPDHM18& other) :
        GPDModule(other), MathIntegratorModule(other) {

    initializeFunctorsForIntegrations();

    m_M = other.m_M;
    m_m = other.m_m;
    m_lambda = other.m_lambda;
    m_p = other.m_p;
    m_N = other.m_N;

}

GPDHM18::~GPDHM18() {
    deleteFunctorsForIntegrations();
}

void GPDHM18::initializeFunctorsForIntegrations() {

    m_pint_IntNorm = NumA::Integrator1D::newIntegrationFunctor(this,
            &GPDHM18::IntNorm);

    m_pint_IntE = NumA::Integrator1D::newIntegrationFunctor(this,
            &GPDHM18::IntE);

    m_pint_IntE0 = NumA::Integrator1D::newIntegrationFunctor(this,
            &GPDHM18::IntE0);

    m_pint_IntH = NumA::Integrator1D::newIntegrationFunctor(this,
            &GPDHM18::IntH);

    m_pint_IntH0 = NumA::Integrator1D::newIntegrationFunctor(this,
            &GPDHM18::IntH0);

    m_pint_IntHt = NumA::Integrator1D::newIntegrationFunctor(this,
            &GPDHM18::IntHt);

    m_pint_IntHt0 = NumA::Integrator1D::newIntegrationFunctor(this,
            &GPDHM18::IntHt0);

    m_pint_IntEt = NumA::Integrator1D::newIntegrationFunctor(this,
            &GPDHM18::IntEt);
}

void GPDHM18::deleteFunctorsForIntegrations() {

    delete m_pint_IntNorm;
    delete m_pint_IntE;
    delete m_pint_IntE0;
    delete m_pint_IntH;
    delete m_pint_IntH0;
    delete m_pint_IntHt;
    delete m_pint_IntHt0;
    delete m_pint_IntEt;
}

GPDHM18* GPDHM18::clone() const {
    return new GPDHM18(*this);
}
void GPDHM18::resolveObjectDependencies() {
    setIntegrator(NumA::IntegratorType1D::DEXP);
}
void GPDHM18::configure(const ElemUtils::Parameters &parameters) {
    MathIntegratorModule::configureIntegrator(parameters);
    GPDModule::configure(parameters);

    if (parameters.isAvailable(GPDHM18::PARAMETER_NAME_HM18MODEL_M)) {

        m_M = parameters.getLastAvailable().toDouble();
        info(__func__,
                ElemUtils::Formatter() << "Parameter "
                        << GPDHM18::PARAMETER_NAME_HM18MODEL_M << " changed to "
                        << m_M);
    }

    if (parameters.isAvailable(GPDHM18::PARAMETER_NAME_HM18MODEL_m)) {

        m_m = parameters.getLastAvailable().toDouble();
        info(__func__,
                ElemUtils::Formatter() << "Parameter "
                        << GPDHM18::PARAMETER_NAME_HM18MODEL_m << " changed to "
                        << m_m);
    }

    if (parameters.isAvailable(GPDHM18::PARAMETER_NAME_HM18MODEL_lambda)) {
        m_lambda = parameters.getLastAvailable().toDouble();
        info(__func__,
                ElemUtils::Formatter() << "Parameter "
                        << GPDHM18::PARAMETER_NAME_HM18MODEL_lambda
                        << " changed to " << m_lambda);
    }

    if (parameters.isAvailable(GPDHM18::PARAMETER_NAME_HM18MODEL_p)) {
        m_p = parameters.getLastAvailable().toDouble();
        info(__func__,
                ElemUtils::Formatter() << "Parameter "
                        << GPDHM18::PARAMETER_NAME_HM18MODEL_p << " changed to "
                        << m_p);
    }

    Normalize();
}
void GPDHM18::isModuleWellConfigured() {
    GPDModule::isModuleWellConfigured();
}
void GPDHM18::initModule() {
    GPDModule::initModule();
}

double GPDHM18::IntNorm(double y, std::vector<double> par) {

    double m2 = pow(m_m, 2);
    double M2 = pow(m_M, 2);
    double lambda2 = pow(m_lambda, 2);
    double coeff = sqrt(Constant::PI) / (4 * M2 * m_p) * tgamma(1 + m_p)
            / tgamma(1.5 + m_p);

    double Num = m2 * (1 + 2 * m_p - y) + 4 * m_m * m_M * m_p * y
            + y * (M2 * (y - 1 + 2 * m_p * y) + lambda2);
    double Den = m2 / M2 - y + lambda2 / M2 * y / (1 - y);

    return coeff * Num / pow(Den, 2 * m_p + 1);
}

void GPDHM18::Normalize() {
    //set variables for integrations
    std::vector<double> parameters;

    double result = integrate(m_pint_IntNorm, 0, 1, parameters);
    m_N = 1 / result;

    info(__func__, ElemUtils::Formatter() << "Normalization set to " << m_N);
}

double GPDHM18::DD_E(double y, double z, double t) {
    double M2 = pow(m_M, 2);
    double m2 = pow(m_m, 2);
    double Num = (m_m / m_M + y) * pow(pow(1 - y, 2) - pow(z, 2), m_p);
    double Den = (1 - y) * m2 / M2 + y * pow(m_lambda, 2) / M2 - y * (1 - y)
            - (pow(1 - y, 2) - pow(z, 2)) * t / 4 / M2;
    return m_N * Num / pow(Den, 2 * m_p + 1);

}

double GPDHM18::IntE0(double z, std::vector<double> par) {
    double x = par[0];
    return (1 - x) * DD_E(x, z, m_t);

}

double GPDHM18::IntE(double y, std::vector<double> par) {
    double x = par[0];
    return (1 - x) / m_xi * DD_E(y, (x - y) / m_xi, m_t);

}

double GPDHM18::DD_H(double y, double z, double t) {
    double M2 = pow(m_M, 2);
    double m2 = pow(m_m, 2);
    double Num1 = pow(pow(1 - y, 2) - pow(z, 2), m_p);
    double Num2 = (-y + y * pow(m_lambda, 2) / M2 + (2 - y) * m2 / M2) * Num1;
    double Den = (1 - y) * m2 / M2 + y * pow(m_lambda, 2) / M2 - y * (1 - y)
            - (pow(1 - y, 2) - pow(z, 2)) * t / 4 / M2;
    return m_N * (1 - 2 * m_p) / (4 * m_p) * Num1 / pow(Den, 2 * m_p)
            + m_N * Num2 / 2 / pow(Den, 2 * m_p + 1);
}

double GPDHM18::IntH0(double z, std::vector<double> par) {
    double x = par[0];
    return DD_H(x, z, m_t) + x * DD_E(x, z, m_t);
}

double GPDHM18::IntH(double y, std::vector<double> par) {
    double x = par[0];
    return 1 / m_xi * DD_H(y, (x - y) / m_xi, m_t)
            + x / m_xi * DD_E(y, (x - y) / m_xi, m_t);
}

double GPDHM18::DD_Ht(double y, double z, double t) {
    double M2 = pow(m_M, 2);
    double m2 = pow(m_m, 2);
    double lambda2 = pow(m_lambda, 2);
    double Num = pow(pow(1 - y, 2) - pow(z, 2), m_p);
    double Den = (1 - y) * m2 / M2 + y * lambda2 / M2 - y * (1 - y)
            - (pow(1 - y, 2) - pow(z, 2)) * t / 4 / M2;
    return m_N / 2 * (m2 / M2 * y - y * lambda2 / M2 + y+2 * y * m_m / m_M )
            * Num / pow(Den, 2 * m_p + 1)
            - m_N * (1 - 2 * m_p) / (4 * m_p) * Num / pow(Den, 2 * m_p);
}

double GPDHM18::IntHt0(double z, std::vector<double> par) {
    double x = par[0];
    return DD_Ht(x, z, m_t);
}

double GPDHM18::IntHt(double y, std::vector<double> par) {
    double x = par[0];
    return 1 / m_xi * DD_Ht(y, (x - y) / m_xi, m_t);
}

double GPDHM18::DD_Et(double y, double z, double t) {
    double M2 = pow(m_M, 2);
    double m2 = pow(m_m, 2);
    double Bracket = pow(1 - y, 2) - pow(z, 2)
            + (1 - y - z / m_xi) * (m_m / m_M + y);
    double Num = Bracket * pow(pow(1 - y, 2) - pow(z, 2), m_p);
    double Den = (1 - y) * m2 / M2 + y * pow(m_lambda, 2) / M2 - y * (1 - y)
            - (pow(1 - y, 2) - pow(z, 2)) * t / 4 / M2;
    return m_N * Num / pow(Den, 2 * m_p + 1);
}

double GPDHM18::IntEt(double y, std::vector<double> par) {
    double x = par[0];
    return 1 / m_xi * DD_Et(y, (x - y) / m_xi, m_t);
}

double GPDHM18::evaluate(double x, NumA::FunctionType1D* p_fun0,
        NumA::FunctionType1D* p_fun) {
    //set variables for integrations
    std::vector<double> parameters;
    parameters.push_back(x);

    //checking kinetic region
    if (fabs(x) > 1 || fabs(m_xi) > 1)
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Calculation not in the kinetic domain.");

    //calculate GPD for x < - xi
    if (x < -fabs(m_xi))
        return 0;

    //calculate GPD for xi == 0
    if (m_xi == 0)
      {
        if (p_fun0 == 0)
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "Integrand not set.");
        else
            return integrate(p_fun0, -1 + x, 1 - x, parameters);
      }

    //calculate GPD for xi <> 0 and x > xi
    if (x > fabs(m_xi))
      {
        if (p_fun == 0)
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "Integrand not set.");
        else
            return integrate(p_fun, (x - m_xi) / (1 - m_xi),
                    (x + m_xi) / (1 + m_xi), parameters);
      }

    //calculate GPD for xi <> 0 and - xi < x < xi
    if (x <= fabs(m_xi) && x >= -fabs(m_xi))
      {
        if (p_fun == 0)
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "Integrand not set.");
        else
            return integrate(p_fun, 0., (x + m_xi) / (1 + m_xi), parameters);
      }

    return 0;
}

PartonDistribution GPDHM18::compute(NumA::FunctionType1D* p_fun0,
        NumA::FunctionType1D* p_fun) {
    //variables
    double aValPx = GPDHM18::evaluate(m_x, p_fun0, p_fun);
    double aValMx = GPDHM18::evaluate(-m_x, p_fun0, p_fun);
    double Sea = 0;
    double g = 0;

    //store
    PartonDistribution partonDistribution;
    QuarkDistribution quarkDistribution(QuarkFlavor::UNDEFINED);
    GluonDistribution gluonDistribution(g);

    quarkDistribution.setQuarkDistribution(aValPx + Sea);
    quarkDistribution.setQuarkDistributionPlus(aValPx - aValMx + 2 * Sea);
    quarkDistribution.setQuarkDistributionMinus(aValPx + aValMx);

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution);

    //return
    return partonDistribution;
}

PartonDistribution GPDHM18::computeE() {
    //compute and return parton distribution for GPH E
    return GPDHM18::compute(m_pint_IntE0, m_pint_IntE);
}

PartonDistribution GPDHM18::computeH() {
    //compute and return parton distribution for GPH H
    return GPDHM18::compute(m_pint_IntH0, m_pint_IntH);
}

PartonDistribution GPDHM18::computeHt() {
    //compute and return parton distribution for GPH Ht
    return GPDHM18::compute(m_pint_IntHt0, m_pint_IntHt);
}

PartonDistribution GPDHM18::computeEt() {
    //No result of GPH Et for xi == 0.
    PartonDistribution no_result;
    if (m_xi == 0) {
        warn(__func__,
                ElemUtils::Formatter()
                        << "GPD Et is divergent for xi==0 in the SDQM by Hwang-Mueller.");
        warn(__func__,
                ElemUtils::Formatter()
                        << "No quark GPD Et result will be returned. ");
        return no_result;
    } else
        //compute and return parton distribution for GPH Et for xi <> 0.
        return GPDHM18::compute(0, m_pint_IntEt);
}

} /* namespace PARTONS */
