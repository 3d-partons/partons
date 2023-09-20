// Implementation of helicity amplitudes and partial cross sections appearing in Goloskokov-Kroll (GK) model
// in pseudoscalar meson (pi+ and pi0) production.

#include "../../../../../include/partons/modules/convol_coeff_function/DVMP/DVMPCFFISSK15.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <cmath>
#include <utility>

#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/MesonType.h"
#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/modules/gpd/GPDModule.h"
#include "../../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongStandard.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"

namespace PARTONS {

const unsigned int DVMPCFFISSK15::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPCFFISSK15("DVMPCFFISSK15"));

DVMPCFFISSK15::DVMPCFFISSK15(const std::string &className) :
        DVMPConvolCoeffFunctionModule(className), m_pRunningAlphaStrongModule(
                0), m_gpdResultXiXiPlus(0.) {

    //relate GPD types with functions to be used
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &DVMPConvolCoeffFunctionModule::computeCFF));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::E,
                    &DVMPConvolCoeffFunctionModule::computeCFF));

    //init
    initFunctorsForIntegrations();
}

DVMPCFFISSK15::DVMPCFFISSK15(const DVMPCFFISSK15 &other) :
        DVMPConvolCoeffFunctionModule(other) {

    //clone alpaS module
    if (other.m_pRunningAlphaStrongModule != 0) {
        m_pRunningAlphaStrongModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pRunningAlphaStrongModule);
    } else {
        m_pRunningAlphaStrongModule = 0;
    }

    m_gpdResultXiXiPlus = other.m_gpdResultXiXiPlus;

    //init
    initFunctorsForIntegrations();
}

DVMPCFFISSK15* DVMPCFFISSK15::clone() const {
    return new DVMPCFFISSK15(*this);
}

DVMPCFFISSK15::~DVMPCFFISSK15() {

    // destroy alphaS module
    if (m_pRunningAlphaStrongModule != 0) {
        setRunningAlphaStrongModule(0);
        m_pRunningAlphaStrongModule = 0;
    }

    //destroy integrators
    if (m_pReGluonLO) {
        delete m_pReGluonLO;
        m_pReGluonLO = 0;
    }
}

void DVMPCFFISSK15::configure(const ElemUtils::Parameters &parameters) {
    DVMPConvolCoeffFunctionModule::configure(parameters);
}

void DVMPCFFISSK15::resolveObjectDependencies() {

    //run for mother
    DVMPConvolCoeffFunctionModule::resolveObjectDependencies();

    //set integrator
    setIntegrator(NumA::IntegratorType1D::DEXP);

    //set alpha_s module
    m_pRunningAlphaStrongModule =
            Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                    RunningAlphaStrongStandard::classId);
}

void DVMPCFFISSK15::initModule() {
    DVMPConvolCoeffFunctionModule::initModule();
}

void DVMPCFFISSK15::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    DVMPConvolCoeffFunctionModule::prepareSubModules(subModulesData);

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

RunningAlphaStrongModule* DVMPCFFISSK15::getRunningAlphaStrongModule() const {
    return m_pRunningAlphaStrongModule;
}

void DVMPCFFISSK15::setRunningAlphaStrongModule(
        RunningAlphaStrongModule* pRunningAlphaStrongModule) {
    m_pModuleObjectFactory->updateModulePointerReference(
            m_pRunningAlphaStrongModule, pRunningAlphaStrongModule);
    m_pRunningAlphaStrongModule = pRunningAlphaStrongModule;
}

void DVMPCFFISSK15::isModuleWellConfigured() {
    DVMPConvolCoeffFunctionModule::isModuleWellConfigured();
}

void DVMPCFFISSK15::initFunctorsForIntegrations() {
    m_pReGluonLO = NumA::Integrator1D::newIntegrationFunctor(this,
            &DVMPCFFISSK15::reGluonLO);
}

std::complex<double> DVMPCFFISSK15::computeCFF() {

    //check pQCD
    if (m_qcdOrderType != PerturbativeQCDOrderType::LO) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "QCD order: "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString()
                        << " not implemented");
    }

    //check meson type
    if (m_mesonType != MesonType::JPSI && m_mesonType != MesonType::UPSILON) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Meson type: "
                        << MesonType(m_mesonType).toString()
                        << " not implemented");
    }

    //evaluate GPD at xixi line
    m_gpdResultXiXiPlus =
            2
                    * m_pGPDModule->compute(
                            GPDKinematic(m_xi, m_xi, m_t, m_MuF2, m_MuR2),
                            m_currentGPDComputeType).getGluonDistribution().getGluonDistribution();

    //alphaS
    double aS = m_pRunningAlphaStrongModule->compute(m_MuF2);

    //Im
    double im = -0.5 * aS * M_PI * m_gpdResultXiXiPlus;

    //Re
    std::vector<double> emptyParameters;

    double re = 0.;

//    re += integrate(m_pReGluonLO, -1., -m_xi, emptyParameters);
//    re += integrate(m_pReGluonLO, -m_xi, 0., emptyParameters);
//    re += integrate(m_pReGluonLO, 0., m_xi, emptyParameters);
//    re += integrate(m_pReGluonLO, m_xi, 1., emptyParameters);
//
//    re += log((1. - m_xi) / (1. + m_xi)) * m_gpdResultXiXiPlus;
//
//    re *= 0.5 * aS;

    //return
    return std::complex<double>(re, im);
}

double DVMPCFFISSK15::reGluonLO(double x, std::vector<double> params) {
    return (2
            * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
                    m_currentGPDComputeType).getGluonDistribution().getGluonDistribution()
            - m_gpdResultXiXiPlus) / (x - m_xi);
}

} /* namespace PARTONS */

