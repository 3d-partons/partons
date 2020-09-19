#include "../../../../../include/partons/modules/evolution/gpd/GPDEvolutionModule.h"

#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/modules/gpd/GPDModule.h"
#include "../../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongModule.h"
#include "../../../../../include/partons/modules/active_flavors_thresholds/ActiveFlavorsThresholdsModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"

#include <cmath>

namespace PARTONS {

const std::string GPDEvolutionModule::GPD_EVOLUTION_MODULE_CLASS_NAME = "GPDEvolutionModule";

GPDEvolutionModule::GPDEvolutionModule(const std::string &className) :
        ModuleObject(className, ChannelType::UNDEFINED),
	m_x(0), m_xi(0), m_t(0), m_MuF2(0), m_MuR2(0),
	m_MuF2_ref(0),
	m_pertOrder(PerturbativeQCDOrderType::UNDEFINED),
	m_type(GPDType::UNDEFINED),
	m_pRunningAlphaStrongModule(0) {
}

GPDEvolutionModule::~GPDEvolutionModule() {
    if (m_pRunningAlphaStrongModule) {
        setRunningAlphaStrongModule(0);
	m_pRunningAlphaStrongModule = 0;
    }

    if (m_pActiveFlavorsModule) {
        setActiveFlavorsModule(0);
        m_pActiveFlavorsModule = 0;
    }
}

std::string GPDEvolutionModule::toString() const {
    return ModuleObject::toString();
}

void GPDEvolutionModule::resolveObjectDependencies() {
    ModuleObject::resolveObjectDependencies();
}

void GPDEvolutionModule::configure(const ElemUtils::Parameters &parameters) {

    //run for mother classes
    ModuleObject::configure(parameters);

    if (parameters.isAvailable(PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE)) {
      try {
	setPertOrder(static_cast<PerturbativeQCDOrderType::Type>(parameters.getLastAvailable().toUInt()));
      } catch (const std::exception &e) {
	// if an exception is raised it means that it's a string configuration value
	setPertOrder(PerturbativeQCDOrderType(parameters.getLastAvailable().getString()).getType());
      }
      info(__func__, ElemUtils::Formatter() << PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE
	   << " configured with value = " << PerturbativeQCDOrderType(getPertOrder()).toString());
    }
}

void GPDEvolutionModule::prepareSubModules(const std::map<std::string, BaseObjectData>& subModulesData) {

    //run for mother class
    ModuleObject::prepareSubModules(subModulesData);

    //RunningAlphaStrongModule
    std::map<std::string, BaseObjectData>::const_iterator it = subModulesData.find(RunningAlphaStrongModule::RUNNING_ALPHA_STRONG_MODULE_CLASS_NAME);

    //check if there
    if (it != subModulesData.end()) {

        //reset
        if (m_pRunningAlphaStrongModule != 0) {
            setRunningAlphaStrongModule(0);
	    m_pRunningAlphaStrongModule = 0;
        }

        //set
	m_pRunningAlphaStrongModule = Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule((it->second).getModuleClassName());
	info(__func__, ElemUtils::Formatter() << "Configured with RunningAlphaStrongModule = " << m_pRunningAlphaStrongModule->getClassName());
	m_pRunningAlphaStrongModule->configure((it->second).getParameters());
	m_pRunningAlphaStrongModule->prepareSubModules((it->second).getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << getClassName() << " is RunningAlphaStrongModule dependent and you have not provided one");
    }

    //ActiveFlavorsModule
    it = subModulesData.find(ActiveFlavorsThresholdsModule::ACTIVE_FLAVORS_THRESHOLDS_MODULE_CLASS_NAME);

    //check if there
    if (it != subModulesData.end()) {

        //reset
        if (m_pActiveFlavorsModule != 0) {
            setActiveFlavorsModule(0);
            m_pActiveFlavorsModule = 0;
        }

        //set
	m_pActiveFlavorsModule = Partons::getInstance()->getModuleObjectFactory()->newActiveFlavorsThresholdsModule((it->second).getModuleClassName());
	info(__func__, ElemUtils::Formatter() << "Configured with ActiveFlavorsModule = " << m_pActiveFlavorsModule->getClassName());
	m_pActiveFlavorsModule->configure((it->second).getParameters());
	m_pActiveFlavorsModule->prepareSubModules((it->second).getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << getClassName() << " is ActiveFlavorsModule dependent and you have not provided one");
    }
}


void GPDEvolutionModule::setMuF2_ref(const double& MuF2_ref) {
    m_MuF2_ref = MuF2_ref;
}

double GPDEvolutionModule::getMuF2_ref() const {
    return m_MuF2_ref;
}

double GPDEvolutionModule::getMuF_ref() const {
    return sqrt(m_MuF2_ref);
}

void GPDEvolutionModule::setPertOrder(const PerturbativeQCDOrderType::Type& pertOrder) {
  m_pertOrder = pertOrder;
}

PerturbativeQCDOrderType::Type GPDEvolutionModule::getPertOrder() const {
  return m_pertOrder;
}

void GPDEvolutionModule::setGPDType(GPDType::Type type) {
    m_type = type;
}

GPDType::Type GPDEvolutionModule::getGPDType() const {
    return m_type;
}

void GPDEvolutionModule::setRunningAlphaStrongModule(RunningAlphaStrongModule* runningAlphaStrongModule) {
    m_pModuleObjectFactory->updateModulePointerReference(m_pRunningAlphaStrongModule, runningAlphaStrongModule);
    m_pRunningAlphaStrongModule = runningAlphaStrongModule;
}

RunningAlphaStrongModule* GPDEvolutionModule::getRunningAlphaStrongModule() const {
    return m_pRunningAlphaStrongModule;
}

void GPDEvolutionModule::setActiveFlavorsModule(ActiveFlavorsThresholdsModule* activeFlavorsModule) {
    m_pModuleObjectFactory->updateModulePointerReference(m_pActiveFlavorsModule, activeFlavorsModule);
    m_pActiveFlavorsModule = activeFlavorsModule;
}

ActiveFlavorsThresholdsModule* GPDEvolutionModule::getActiveFlavorsModule() const {
    return m_pActiveFlavorsModule;
}

GPDEvolutionModule::GPDEvolutionModule(const GPDEvolutionModule &other) :
        ModuleObject(other) {

    setMuF2_ref(other.getMuF2_ref());
    setPertOrder(other.getPertOrder());
    setGPDType(other.getGPDType());

    if (other.m_pRunningAlphaStrongModule != 0) {
        m_pRunningAlphaStrongModule = m_pModuleObjectFactory->cloneModuleObject(other.m_pRunningAlphaStrongModule);
    } else {
        m_pRunningAlphaStrongModule = 0;
    }

    setKinematics(other.getKinematics());
}

void GPDEvolutionModule::initModule() {
}

void GPDEvolutionModule::isModuleWellConfigured() {

    if (m_MuF2_ref <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "muF2_ref is out of range: " << m_MuF2_ref);
    }

    if (m_pertOrder == PerturbativeQCDOrderType::UNDEFINED) {
        throw ElemUtils::CustomException(getClassName(), __func__, "pQCD order is UNDEFINED");
    }

    if (m_type == GPDType::UNDEFINED) {
        throw ElemUtils::CustomException(getClassName(), __func__, "GPD distribution type is UNDEFINED");
    }

    if (!m_pRunningAlphaStrongModule) {
        throw ElemUtils::CustomException(getClassName(), __func__, "Pointer to RunningAlphaStrong module is NULL");
    }

    if (!m_pActiveFlavorsModule) {
        throw ElemUtils::CustomException(getClassName(), __func__, "Pointer to ActiveFlavorsModule module is NULL");
    }

    if (m_x < -1. || m_x > 1.) {
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "x is out of range: " << m_x);
    }

    if (m_xi < 0. || m_xi > 1.) {
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "xi is out of range: " << m_xi);
    }

    if (m_t > 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "t is out of range: " << m_t);
    }

    if (m_MuF2 <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "muF2 is out of range: " << m_MuF2);
    }

    if (m_MuR2 <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "muR2 is out of range:" << m_MuR2);
    }
}

void GPDEvolutionModule::setKinematics(const GPDKinematic& kinematic) {
    m_x    = kinematic.getX().makeSameUnitAs(PhysicalUnit::NONE).getValue();
    m_xi   = kinematic.getXi().makeSameUnitAs(PhysicalUnit::NONE).getValue();
    m_t    = kinematic.getT().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_MuF2 = kinematic.getMuF2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_MuR2 = kinematic.getMuR2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
}

GPDKinematic GPDEvolutionModule::getKinematics() const {
    return GPDKinematic{m_x, m_xi, m_t, m_MuF2, m_MuR2};
}

PartonDistribution GPDEvolutionModule::compute(const GPDKinematic &kinematic, GPDModule* pGPDModule, const GPDType::Type &type) {

    //reset kinematics
    setKinematics(kinematic);

    //initialize
    initModule();

    //check if input data are fine
    isModuleWellConfigured();

    //return the evolution result
    return compute(pGPDModule, type);
}

} /* namespace PARTONS */

