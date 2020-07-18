#include "../../../../include/partons/modules/collinear_distribution/CollinearDistributionModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <math.h>
#include <stddef.h>
#include <iostream>
#include <utility>

#include "../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../include/partons/beans/parton_distribution/CollinearDistributionKinematic.h"
#include "../../../../include/partons/beans/Result.h"
#include "../../../../include/partons/modules/evolution/collinear_distribution/CollinearDistributionEvolutionModule.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/services/CollinearDistributionService.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../include/partons/ServiceObjectTyped.h"
#include "../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string CollinearDistributionModule::COLLINEAR_DISTRIBUTION_MODULE_CLASS_NAME = "CollinearDistributionModule";

CollinearDistributionModule::CollinearDistributionModule(const std::string &className) :
        ModuleObject(className, ChannelType::UNDEFINED), m_x(0.), m_MuF2(0.), m_MuR2(0.), m_currentCollinearDistributionComputeType(
                CollinearDistributionType::ALL), m_MuF2_ref(0.), m_pCollinearDistributionEvolutionModule(0) {
}

CollinearDistributionModule::CollinearDistributionModule(const CollinearDistributionModule &other) :
        ModuleObject(other), m_x(other.m_x), m_MuF2(
                other.m_MuF2), m_MuR2(other.m_MuR2), m_currentCollinearDistributionComputeType(
                other.m_currentCollinearDistributionComputeType), m_MuF2_ref(other.m_MuF2_ref) {

    if (other.m_pCollinearDistributionEvolutionModule != 0) {
        m_pCollinearDistributionEvolutionModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pCollinearDistributionEvolutionModule);
    } else {
        m_pCollinearDistributionEvolutionModule = 0;
    }

    m_listCollinearDistributionComputeTypeAvailable = other.m_listCollinearDistributionComputeTypeAvailable;
}

CollinearDistributionModule::~CollinearDistributionModule() {

    if (m_pCollinearDistributionEvolutionModule) {
        setEvolQcdModule(0);
        m_pCollinearDistributionEvolutionModule = 0;
    }
}

std::string CollinearDistributionModule::toString() const {
    return ModuleObject::toString();
}

void CollinearDistributionModule::resolveObjectDependencies() {
    ModuleObject::resolveObjectDependencies();
}

void CollinearDistributionModule::run() {

    try {

        //get service
        CollinearDistributionService* pCollinearDistributionService =
                Partons::getInstance()->getServiceObjectRegistry()->getCollinearDistributionService();

        //run until empty
        while (!(pCollinearDistributionService->isEmptyTaskQueue())) {

            //kinematics
            CollinearDistributionKinematic kinematic;

            //list of collinear distribution types
            List<CollinearDistributionType> colldistTypeList;

            //set
            ElemUtils::Packet packet = pCollinearDistributionService->popTaskFormQueue();
            packet >> kinematic;
            packet >> colldistTypeList;

            //debug information
            debug(__func__,
                    ElemUtils::Formatter() << "objectId = " << getObjectId()
                            << " " << kinematic.toString());

            //object to be returned
            CollinearDistributionResult colldistResult = compute(kinematic, colldistTypeList);

            //helpful to sort later if kinematic is coming from database
            colldistResult.setIndexId(kinematic.getIndexId());

            //add
            pCollinearDistributionService->add(colldistResult);
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void CollinearDistributionModule::configure(const ElemUtils::Parameters &parameters) {
    ModuleObject::configure(parameters);
}

void CollinearDistributionModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    ModuleObject::prepareSubModules(subModulesData);

    std::map<std::string, BaseObjectData>::const_iterator it;

    it = subModulesData.find(
            CollinearDistributionEvolutionModule::COLLINEAR_DISTRIBUTION_EVOLUTION_MODULE_CLASS_NAME);

    if (it != subModulesData.end()) {

        if (m_pCollinearDistributionEvolutionModule) {

            setEvolQcdModule(0);
            m_pCollinearDistributionEvolutionModule = 0;
        }

        if (!m_pCollinearDistributionEvolutionModule) {

            m_pCollinearDistributionEvolutionModule =
                    Partons::getInstance()->getModuleObjectFactory()->newCollinearDistributionEvolutionModule(
                            (it->second).getModuleClassName());

            info(__func__,
                    ElemUtils::Formatter()
                            << "Configure with CollinearDistributionEvolutionModule = "
                            << m_pCollinearDistributionEvolutionModule->getClassName());

            m_pCollinearDistributionEvolutionModule->configure((it->second).getParameters());
            m_pCollinearDistributionEvolutionModule->prepareSubModules(
                    (it->second).getSubModules());
        }
    }
}

PartonDistribution CollinearDistributionModule::compute(const CollinearDistributionKinematic &kinematic,
        CollinearDistributionType::Type colldistType) {

    //create list
    List<CollinearDistributionType> list;

    //add
    list.add(CollinearDistributionType(colldistType));

    //run
    return compute(kinematic, list).getPartonDistribution(colldistType);
}

CollinearDistributionResult CollinearDistributionModule::compute(const CollinearDistributionKinematic &kinematic,
        const List<CollinearDistributionType>& colldistType) {

    //reset kinematics (virtuality)
    setKinematics(kinematic);

    //execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    CollinearDistributionResult result(kinematic);

    //loop over collinear distribution types
    for (size_t i = 0; i < colldistType.size(); i++) {

        //search for pointer to function associated to given collinear distribution type
        m_it = m_listCollinearDistributionComputeTypeAvailable.find(colldistType[i]);

        //check if found
        if (m_it != m_listCollinearDistributionComputeTypeAvailable.end()) {

            //set collinear distribution type
            setCurrentCollinearDistributionType(colldistType[i]);

            //evaluate
            PartonDistribution partonDistribution;

            if (m_pCollinearDistributionEvolutionModule != 0 && (m_MuF2 != m_MuF2_ref)) {

                CollinearDistributionModule* colldistModule =
                        m_pModuleObjectFactory->cloneModuleObject(this);

                partonDistribution = m_pCollinearDistributionEvolutionModule->compute(m_x, m_MuF2, m_MuR2, colldistModule, (m_it->first));

                m_pModuleObjectFactory->updateModulePointerReference(colldistModule,
                          0);
            } else {
                partonDistribution = ((*this).*(m_it->second))();
            }

            //add
            result.addPartonDistribution(colldistType[i], partonDistribution);

        } else {

            //throw error
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "CollinearDistribution("
                            << CollinearDistributionType(colldistType[i]).toString()
                            << ") is not available for this  model");
        }
    }

    //set module name
    result.setComputationModuleName(getClassName());

    //return
    return result;
}

List<CollinearDistributionType> CollinearDistributionModule::getListOfAvailableCollinearDistributionTypeForComputation() const {

    //object to be returned
    List<CollinearDistributionType> listOfAvailableCollinearDistributionTypeForComputation;

    //iterator
    std::map<CollinearDistributionType::Type, PartonDistribution (CollinearDistributionModule::*)()>::const_iterator it;

    //fill list
    for (it = m_listCollinearDistributionComputeTypeAvailable.begin();
            it != m_listCollinearDistributionComputeTypeAvailable.end(); it++) {
        listOfAvailableCollinearDistributionTypeForComputation.add(it->first);
    }

    //return
    return listOfAvailableCollinearDistributionTypeForComputation;
}

double CollinearDistributionModule::getMuF2Ref() const {
    return m_MuF2_ref;
}

const CollinearDistributionEvolutionModule* CollinearDistributionModule::getEvolQcdModule() const {
    return m_pCollinearDistributionEvolutionModule;
}

void CollinearDistributionModule::setEvolQcdModule(CollinearDistributionEvolutionModule* pEvolQcdModule) {
    m_pModuleObjectFactory->updateModulePointerReference(m_pCollinearDistributionEvolutionModule,
            pEvolQcdModule);
    m_pCollinearDistributionEvolutionModule = pEvolQcdModule;
}

PartonDistribution CollinearDistributionModule::computeUnpolPDF() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution CollinearDistributionModule::computePolPDF() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution CollinearDistributionModule::computeTransPDF() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution CollinearDistributionModule::computeUnpolFF() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution CollinearDistributionModule::computePolFF() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution CollinearDistributionModule::computeTransFF() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

void CollinearDistributionModule::setKinematics(const CollinearDistributionKinematic& kinematic) {

    m_x = kinematic.getX().makeSameUnitAs(PhysicalUnit::NONE).getValue();
    m_MuF2 = kinematic.getMuF2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_MuR2 = kinematic.getMuR2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
}

void CollinearDistributionModule::setCurrentCollinearDistributionType(CollinearDistributionType::Type colldistType) {
    m_currentCollinearDistributionComputeType = colldistType;
}

void CollinearDistributionModule::initModule() {
}

void CollinearDistributionModule::isModuleWellConfigured() {

    if (fabs(m_x) > 1.) {
        warn(__func__,
                "Longitudinal momentum fraction should be in [-1., +1.]");
    }

    if (m_MuF2 <= 0.) {
        warn(__func__, "Square of factorization scale should be > 0.");
    }

    if (m_MuR2 <= 0.) {
        warn(__func__, "Square of renormalization scale should be > 0.");
    }

    if (m_MuF2_ref <= 0.) {
        warn(__func__,
                "Square of reference factorization scale should be > 0.");
    }
}

} /* namespace PARTONS */
