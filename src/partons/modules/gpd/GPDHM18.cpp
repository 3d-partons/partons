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

namespace PARTONS {

const unsigned int GPDHM18::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDHM18("GPDHM18"));

const std::string GPDHM18::PARAMETER_NAME_HM18MODEL_m = "HM18MODEL_m";
const std::string GPDHM18::PARAMETER_NAME_HM18MODEL_lambda = "HM18MODEL_lambda";
const std::string GPDHM18::PARAMETER_NAME_HM18MODEL_p = "HM18MODEL_p";

GPDHM18::GPDHM18(const std::string &className) :
        GPDModule(className), MathIntegratorModule() {

    m_MuF2_ref = 4.;

    m_m = 0.45;
    m_lambda = 0.75;
    m_p = 1.;
    m_N = 0.021973799001564008;

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

    m_m = other.m_m;
    m_lambda = other.m_lambda;
    m_p = other.m_p;
    m_N = other.m_N;
}
GPDHM18::~GPDHM18() {
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

    if (parameters.isAvailable(GPDHM18::PARAMETER_NAME_HM18MODEL_m)) {

        m_m = parameters.getLastAvailable().toDouble();
        info(__func__,
                ElemUtils::Formatter() << "Parameter "
                        << GPDHM18::PARAMETER_NAME_HM18MODEL_m << " changed to "
                        << m_m);
    }

    if (parameters.isAvailable(GPDHM18::PARAMETER_NAME_HM18MODEL_lambda)) {
        m_lambda = parameters.getLastAvailable().toDouble();
    }

    if (parameters.isAvailable(GPDHM18::PARAMETER_NAME_HM18MODEL_p)) {
        m_p = parameters.getLastAvailable().toDouble();
    }
}
void GPDHM18::isModuleWellConfigured() {
    GPDModule::isModuleWellConfigured();
}
void GPDHM18::initModule() {
    GPDModule::initModule();
}

double GPDHM18::int_e(double y, double z, double t) {
    double m_M = 1; //Constant::PROTON_MASS;
    double M2 = pow(m_M, 2);
    double m2 = pow(m_m, 2);
    double Num = (m_m / m_M + y) * pow(pow(1 - y, 2) - pow(z, 2), m_p);
    double Den = (1 - y) * m2 / M2 + y * pow(m_lambda, 2) / M2 - y * (1 - y)
            - (pow(1 - y, 2) - pow(z, 2)) * t / 4 / M2;
    return m_N * Num / pow(Den, 2 * m_p + 1);

}

double GPDHM18::intE0(double z, std::vector<double> par) {
    double x = static_cast<QuarkFlavor::Type>(int(par[1]));
    return (1 - m_x) * int_e(m_x, z, m_t);

}

double GPDHM18::intE(double y, std::vector<double> par) {
    double x = static_cast<QuarkFlavor::Type>(int(par[1]));
    return (1 - m_x) / m_xi * int_e(y, (m_x - y) / m_xi, m_t);

}

PartonDistribution GPDHM18::computeH() {
    //result
    PartonDistribution result;
    //your implementation comes here
    //return
    return result;
}

PartonDistribution GPDHM18::computeHt() {
    //result
    PartonDistribution result;
    //your implementation comes here
    //return
    return result;
}
PartonDistribution GPDHM18::computeE() {

    //variables
    double aVal, Sea, g;
    double aValMx;

    //minus x
    double Mx = -m_x;

    //set variables for integrations
    NumA::FunctionType1D* integrant;
    std::vector<double> parameters;

    //calculate GPD E for x < - xi
    if (m_x < -m_xi)
        aVal = 0;

    //calculate GPD E for xi == 0
    if (m_xi == 0) {
        integrant = NumA::Integrator1D::newIntegrationFunctor(this,
                &GPDHM18::intE0);
        parameters.push_back(m_x);
        aVal = integrate(integrant, -1 + m_x, 1 - m_x, parameters);
        delete integrant;
    }
    //calculate GPD E for xi <> 0 and x > xi
    if (m_x > m_xi && m_xi > 0) {
        integrant = NumA::Integrator1D::newIntegrationFunctor(this,
                &GPDHM18::intE);
        parameters.push_back(m_x);
        aVal = integrate(integrant, (m_x - m_xi) / (1 - m_xi),
                (m_x + m_xi) / (1 + m_xi), parameters);
        delete integrant;
    }
    //calculate GPD E for xi <> 0 and - xi < x < xi
    if (m_x <= m_xi && m_x >= -m_xi && m_xi > 0) {
        integrant = NumA::Integrator1D::newIntegrationFunctor(this,
                &GPDHM18::intE);
        parameters.push_back(m_x);
        aVal = integrate(integrant, 0., (m_x + m_xi) / (1 + m_xi), parameters);
        delete integrant;
    }

    Sea = 0.;
    g = 0.;
    aValMx = 0.;

    //store
    QuarkDistribution quarkDistribution_a(QuarkFlavor::UNDEFINED);
    GluonDistribution gluonDistribution(g);
    PartonDistribution partonDistribution;

    quarkDistribution_a.setQuarkDistribution(aVal + Sea);
    quarkDistribution_a.setQuarkDistributionPlus(aVal - aValMx + 2 * Sea);
    quarkDistribution_a.setQuarkDistributionMinus(aVal + aValMx);

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_a);

    //return
    return partonDistribution;
}
PartonDistribution GPDHM18::computeEt() {
    //result
    PartonDistribution result;
    //your implementation comes here
    //return
    return result;
}

} /* namespace PARTONS */
