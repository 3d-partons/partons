#include "../../../../../include/partons/modules/convol_coeff_function/DDVCS/DDVCSCFFTEST.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <utility>

#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/modules/gpd/GPDModule.h"

namespace PARTONS {

const unsigned int DDVCSCFFTEST::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSCFFTEST("DDVCSCFFTEST"));

DDVCSCFFTEST::DDVCSCFFTEST(const std::string &className) :
        DDVCSConvolCoeffFunctionModule(className) {

    //relate CFF types with functions
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &DDVCSConvolCoeffFunctionModule::computeUnpolarized));

    //set functors
    initFunctorsForIntegrations();
}

DDVCSCFFTEST::DDVCSCFFTEST(const DDVCSCFFTEST &other) :
        DDVCSConvolCoeffFunctionModule(other) {

    //set functors
    initFunctorsForIntegrations();
}

void DDVCSCFFTEST::resolveObjectDependencies() {

    //resolve for parent module
    DDVCSConvolCoeffFunctionModule::resolveObjectDependencies();

    //set integration type
    setIntegrator(NumA::IntegratorType1D::DEXP);
}

DDVCSCFFTEST::~DDVCSCFFTEST() {

    // destroy functors
    if (m_pConvolution) {
        delete m_pConvolution;
        m_pConvolution = 0;
    }

}

DDVCSCFFTEST* DDVCSCFFTEST::clone() const {
    return new DDVCSCFFTEST(*this);
}

void DDVCSCFFTEST::initFunctorsForIntegrations() {
    m_pConvolution = NumA::Integrator1D::newIntegrationFunctor(this,
            &DDVCSCFFTEST::convolution);
}

void DDVCSCFFTEST::initModule() {

    //init parent module
    DDVCSConvolCoeffFunctionModule::initModule();
}

void DDVCSCFFTEST::isModuleWellConfigured() {

    //check parent module
    DDVCSConvolCoeffFunctionModule::isModuleWellConfigured();

    //check if GPD module set
    if (m_pGPDModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "GPDModule* is NULL");
    }

    //check pQCD order
    if (m_qcdOrderType == PerturbativeQCDOrderType::UNDEFINED) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "QCDOrderType is UNDEFINED");
    }

    //check if LO or NLO
    if (m_qcdOrderType != PerturbativeQCDOrderType::LO) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Perturbative QCD order can only be LO,  here: "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }
}

std::complex<double> DDVCSCFFTEST::computeUnpolarized() {

    //internal variables to be used
    m_xi;
    m_eta;
    m_t;
    m_MuF2;
    m_MuR2;
    m_Q2;
    m_Q2Prim;

    m_pGPDModule;   //pointer to GPD module
    m_currentGPDComputeType; //GPD type
    m_qcdOrderType; //pQCD order

    //parameters
    std::vector<double> parameters;

    double re = integrate(m_pConvolution, 0., 1., parameters);

    return std::complex<double>(re, 0.);
}

std::complex<double> DDVCSCFFTEST::computePolarized() {
    return std::complex<double>(0., 0.);
}

double DDVCSCFFTEST::convolution(double x, std::vector<double> params) {
    return x
            * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
                    GPDType::H).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistributionPlus();
}

} /* namespace PARTONS */
