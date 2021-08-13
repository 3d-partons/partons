#include "../../../../../include/partons/modules/convol_coeff_function/GAM2/GAM2CFFStandard.h"

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
#include "../../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongStandard.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"

namespace PARTONS {

const unsigned int GAM2CFFStandard::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GAM2CFFStandard("GAM2CFFStandard"));

GAM2CFFStandard::GAM2CFFStandard(const std::string &className) :
        GAM2ConvolCoeffFunctionModule(className), m_pRunningAlphaStrongModule(0) {

    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &GAM2ConvolCoeffFunctionModule::computeUnpolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::E,
                    &GAM2ConvolCoeffFunctionModule::computeUnpolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Ht,
                    &GAM2ConvolCoeffFunctionModule::computePolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Et,
                    &GAM2ConvolCoeffFunctionModule::computePolarized));

    initFunctorsForIntegrations();
}

GAM2CFFStandard::GAM2CFFStandard(const GAM2CFFStandard &other) :
        GAM2ConvolCoeffFunctionModule(other) {

    if (other.m_pRunningAlphaStrongModule != 0) {
        m_pRunningAlphaStrongModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pRunningAlphaStrongModule);
    } else {
        m_pRunningAlphaStrongModule = 0;
    }

    initFunctorsForIntegrations();
}

GAM2CFFStandard* GAM2CFFStandard::clone() const {
    return new GAM2CFFStandard(*this);
}

void GAM2CFFStandard::resolveObjectDependencies() {
    GAM2ConvolCoeffFunctionModule::resolveObjectDependencies();

    setIntegrator(NumA::IntegratorType1D::DEXP);

    m_pRunningAlphaStrongModule =
            Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                    RunningAlphaStrongStandard::classId);
}

void GAM2CFFStandard::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    GAM2ConvolCoeffFunctionModule::prepareSubModules(subModulesData);

    std::map<std::string, BaseObjectData>::const_iterator it;

    it = subModulesData.find(
            RunningAlphaStrongModule::RUNNING_ALPHA_STRONG_MODULE_CLASS_NAME);

    if (it != subModulesData.end()) {

        if (m_pRunningAlphaStrongModule != 0) {
            setRunningAlphaStrongModule(0);
            m_pRunningAlphaStrongModule = 0;
        }

        if (!m_pRunningAlphaStrongModule) {
            m_pRunningAlphaStrongModule =
                    Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                            (it->second).getModuleClassName());
            info(__func__,
                    ElemUtils::Formatter()
                            << "Configure with RunningAlphaStrongModule = "
                            << m_pRunningAlphaStrongModule->getClassName());
            m_pRunningAlphaStrongModule->configure(
                    (it->second).getParameters());
        }
    }
}

void GAM2CFFStandard::initFunctorsForIntegrations() {
    m_pExampleIntegration = NumA::Integrator1D::newIntegrationFunctor(this,
            &GAM2CFFStandard::exampleIntegration);
}

GAM2CFFStandard::~GAM2CFFStandard() {

    if (m_pRunningAlphaStrongModule) {
        setRunningAlphaStrongModule(0);
        m_pRunningAlphaStrongModule = 0;
    }

    if (m_pExampleIntegration) {
        delete m_pExampleIntegration;
        m_pExampleIntegration = 0;
    }
}

void GAM2CFFStandard::initModule() {
    GAM2ConvolCoeffFunctionModule::initModule();
}

void GAM2CFFStandard::isModuleWellConfigured() {

    GAM2ConvolCoeffFunctionModule::isModuleWellConfigured();

    if (m_pGPDModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "GPDModule* is NULL");
    }

    if (m_qcdOrderType == PerturbativeQCDOrderType::UNDEFINED) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "QCDOrderType is UNDEFINED");
    }

    if (m_qcdOrderType != PerturbativeQCDOrderType::LO
            && m_qcdOrderType != PerturbativeQCDOrderType::NLO) {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Erroneous input, perturbative QCD order can only be LO or NLO. Here Order = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }

    if (getMathIntegrator() == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "MathIntegrationMode is UNDEFINED");
    }

    if (m_pRunningAlphaStrongModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "RunningAlphaStrongModule* is NULL");
    }
}

std::complex<double> GAM2CFFStandard::computeUnpolarized() {

    //variables to use
    m_currentGPDComputeType;
    m_pRunningAlphaStrongModule;
    m_pGPDModule;
    m_qcdOrderType;
    m_xi;
    m_t;
    m_uPrim;
    m_Mgg2;
    m_MuF2;
    m_MuR2;
    m_polG0;
    m_polG1;
    m_polG2;

    std::vector<double> emptyParameters;

    double resultA = integrate(m_pExampleIntegration, -1, 1.,
            emptyParameters);
    double resultB = m_pRunningAlphaStrongModule->compute(m_MuF2);

    return std::complex<double>(resultA, resultB);
}

std::complex<double> GAM2CFFStandard::computePolarized() {
    return std::complex<double>();
}

RunningAlphaStrongModule* GAM2CFFStandard::getRunningAlphaStrongModule() const {
    return m_pRunningAlphaStrongModule;
}

void GAM2CFFStandard::setRunningAlphaStrongModule(
        RunningAlphaStrongModule* pRunningAlphaStrongModule) {
    m_pModuleObjectFactory->updateModulePointerReference(
            m_pRunningAlphaStrongModule, pRunningAlphaStrongModule);
    m_pRunningAlphaStrongModule = pRunningAlphaStrongModule;
}

double GAM2CFFStandard::exampleIntegration(double x,
        std::vector<double> params) {
    return m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution();
}

} /* namespace PARTONS */
