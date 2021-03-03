#include "../../../../../include/partons/modules/evolution/collinear_distribution/CollinearDistributionEvolutionModule.h"

#include "../../../../../include/partons/beans/collinear_distribution/CollinearDistributionKinematic.h"
#include "../../../../../include/partons/modules/collinear_distribution/CollinearDistributionModule.h"
#include "../../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongModule.h"
#include "../../../../../include/partons/modules/active_flavors_thresholds/ActiveFlavorsThresholdsModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"

#include <cmath>

namespace PARTONS {

const std::string CollinearDistributionEvolutionModule::COLLINEAR_DISTRIBUTION_EVOLUTION_MODULE_CLASS_NAME = "CollinearDistributionEvolutionModule";

CollinearDistributionEvolutionModule::CollinearDistributionEvolutionModule(const std::string &className) :
        ModuleObject(className, ChannelType::UNDEFINED),
	m_x(0), m_MuF2(0), m_MuR2(0),
	m_pertOrder(PerturbativeQCDOrderType::NNLO),
	m_type(CollinearDistributionType::UnpolPDF),
	m_pRunningAlphaStrongModule(0),
	m_pActiveFlavorsModule(0) {
}

CollinearDistributionEvolutionModule::CollinearDistributionEvolutionModule(const CollinearDistributionEvolutionModule &other) :
        ModuleObject(other) {

    setPertOrder(other.getPertOrder());
    setCollinearDistributionType(other.getCollinearDistributionType());

    if (other.m_pRunningAlphaStrongModule != 0) {
        m_pRunningAlphaStrongModule = m_pModuleObjectFactory->cloneModuleObject(other.m_pRunningAlphaStrongModule);
    } else {
        m_pRunningAlphaStrongModule = 0;
    }

    if (other.m_pActiveFlavorsModule != 0) {
        m_pActiveFlavorsModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pActiveFlavorsModule);
    } else {
        m_pActiveFlavorsModule = 0;
    }

    setKinematics(other.getKinematics());
}

CollinearDistributionEvolutionModule::~CollinearDistributionEvolutionModule() {
    if (m_pRunningAlphaStrongModule) {
        setRunningAlphaStrongModule(0);
	m_pRunningAlphaStrongModule = 0;
    }
    if (m_pActiveFlavorsModule) {
        setActiveFlavorsModule(0);
	m_pActiveFlavorsModule = 0;
    }
}

std::string CollinearDistributionEvolutionModule::toString() const {
    return ModuleObject::toString();
}

void CollinearDistributionEvolutionModule::resolveObjectDependencies() {
    ModuleObject::resolveObjectDependencies();
}

void CollinearDistributionEvolutionModule::configure(const ElemUtils::Parameters &parameters) {

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

    if (parameters.isAvailable(CollinearDistributionType::COLLINEAR_DISTRIBUTION_TYPE_DB_COLUMN_NAME)) {
        setCollinearDistributionType(CollinearDistributionType{}.fromString(parameters.getLastAvailable().getString()));
        info(__func__, ElemUtils::Formatter() << CollinearDistributionType::COLLINEAR_DISTRIBUTION_TYPE_DB_COLUMN_NAME
	     << " configured with value = " << CollinearDistributionType(getCollinearDistributionType()).toString());
    }
}

void CollinearDistributionEvolutionModule::prepareSubModules(const std::map<std::string, BaseObjectData>& subModulesData) {

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

void CollinearDistributionEvolutionModule::setPertOrder(const PerturbativeQCDOrderType::Type& pertOrder) {
  m_pertOrder = pertOrder;
}

PerturbativeQCDOrderType::Type CollinearDistributionEvolutionModule::getPertOrder() const {
  return m_pertOrder;
}

void CollinearDistributionEvolutionModule::setCollinearDistributionType(CollinearDistributionType::Type type) {
    m_type = type;
}

CollinearDistributionType::Type CollinearDistributionEvolutionModule::getCollinearDistributionType() const {
    return m_type;
}

void CollinearDistributionEvolutionModule::setRunningAlphaStrongModule(RunningAlphaStrongModule* runningAlphaStrongModule) {
    m_pModuleObjectFactory->updateModulePointerReference(m_pRunningAlphaStrongModule, runningAlphaStrongModule);
    m_pRunningAlphaStrongModule = runningAlphaStrongModule;
}

RunningAlphaStrongModule* CollinearDistributionEvolutionModule::getRunningAlphaStrongModule() const {
    return m_pRunningAlphaStrongModule;
}

void CollinearDistributionEvolutionModule::setActiveFlavorsModule(ActiveFlavorsThresholdsModule* activeFlavorsModule) {
    m_pModuleObjectFactory->updateModulePointerReference(m_pActiveFlavorsModule, activeFlavorsModule);
    m_pActiveFlavorsModule = activeFlavorsModule;
}

ActiveFlavorsThresholdsModule* CollinearDistributionEvolutionModule::getActiveFlavorsModule() const {
    return m_pActiveFlavorsModule;
}



void CollinearDistributionEvolutionModule::initModule() {
}

void CollinearDistributionEvolutionModule::isModuleWellConfigured() {

    if (m_pertOrder == PerturbativeQCDOrderType::UNDEFINED) {
        throw ElemUtils::CustomException(getClassName(), __func__, "pQCD order is UNDEFINED");
    }

    if (m_type == CollinearDistributionType::UNDEFINED) {
        throw ElemUtils::CustomException(getClassName(), __func__, "collinear distribution type is UNDEFINED");
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

    if (m_MuF2 <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "muF2 is out of range: " << m_MuF2);
    }

    if (m_MuR2 <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "muR2 is out of range:" << m_MuR2);
    }
}

void CollinearDistributionEvolutionModule::setKinematics(const CollinearDistributionKinematic& kinematic) {
    m_x    = kinematic.getX().makeSameUnitAs(PhysicalUnit::NONE).getValue();
    m_MuF2 = kinematic.getMuF2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_MuR2 = kinematic.getMuR2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
}

CollinearDistributionKinematic CollinearDistributionEvolutionModule::getKinematics() const {
    return CollinearDistributionKinematic{m_x, m_MuF2, m_MuR2};
}

PartonDistribution CollinearDistributionEvolutionModule::compute(const CollinearDistributionKinematic &kinematic, CollinearDistributionModule* pCollinearDistributionModule) {

    //reset kinematics
    setKinematics(kinematic);

    //initialize
    initModule();

    //check if input data are fine
    isModuleWellConfigured();

    //return the evolution result
    return compute(pCollinearDistributionModule);
}

} /* namespace PARTONS */

