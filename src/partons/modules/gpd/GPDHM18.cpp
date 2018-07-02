#include "../../../../include/partons/modules/gpd/GPDHM18.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
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

GPDHM18::GPDHM18(const std::string &className) :
        GPDModule(className), MathIntegratorModule() {

    m_MuF2_ref = 4.;

    initFunctorsForIntegrations();

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
    initFunctorsForIntegrations();
}
GPDHM18::~GPDHM18() {

    if (m_pint_int_e) {
        delete m_pint_int_e;
        m_pint_int_e = 0;
    }
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
}
void GPDHM18::isModuleWellConfigured() {
    GPDModule::isModuleWellConfigured();
}
void GPDHM18::initModule() {
    GPDModule::initModule();
}

void GPDHM18::initFunctorsForIntegrations() {

    m_pint_int_e = NumA::Integrator1D::newIntegrationFunctor(this,
            &GPDHM18::int_e);
}

double GPDHM18::int_e(double y, std::vector<double> par) {
    return 2;

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
    double uVal, uSea, dVal, dSea, sSea, g;
    double uValMx, dValMx;

    //minus x
    double Mx = -m_x;

    //set parameters
    std::vector<double> parameters;

    //calculate
    uVal = integrate(m_pint_int_e, -1., 1., parameters);
    dVal = 0.;

    uSea = 0.;
    dSea = 0.;
    sSea = 0.;

    g = 0.;

    uValMx = 0.;
    dValMx = 0.;

    //store
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
PartonDistribution GPDHM18::computeEt() {
    //result
    PartonDistribution result;
    //your implementation comes here
    //return
    return result;
}

} /* namespace PARTONS */
