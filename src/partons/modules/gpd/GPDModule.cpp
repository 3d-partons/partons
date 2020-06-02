#include "../../../../include/partons/modules/gpd/GPDModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <math.h>
#include <stddef.h>
#include <iostream>
#include <utility>

#include "../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../include/partons/beans/Result.h"
#include "../../../../include/partons/modules/evolution/gpd/GPDEvolutionModule.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/services/GPDService.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../include/partons/ServiceObjectTyped.h"
#include "../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string GPDModule::GPD_MODULE_CLASS_NAME = "GPDModule";

GPDModule::GPDModule(const std::string &className) :
        ModuleObject(className, ChannelType::UNDEFINED), m_x(0.), m_xi(0.), m_t(
                0.), m_MuF2(0.), m_MuR2(0.), m_currentGPDComputeType(
                GPDType::ALL), m_MuF2_ref(0.), m_pGPDEvolutionModule(0) {
}

GPDModule::GPDModule(const GPDModule &other) :
        ModuleObject(other), m_x(other.m_x), m_xi(other.m_xi), m_t(other.m_t), m_MuF2(
                other.m_MuF2), m_MuR2(other.m_MuR2), m_currentGPDComputeType(
                other.m_currentGPDComputeType), m_MuF2_ref(other.m_MuF2_ref) {

    if (other.m_pGPDEvolutionModule != 0) {
        m_pGPDEvolutionModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pGPDEvolutionModule);
    } else {
        m_pGPDEvolutionModule = 0;
    }

    m_listGPDComputeTypeAvailable = other.m_listGPDComputeTypeAvailable;
}

GPDModule::~GPDModule() {

    if (m_pGPDEvolutionModule) {
        setEvolQcdModule(0);
        m_pGPDEvolutionModule = 0;
    }
}

std::string GPDModule::toString() const {
    return ModuleObject::toString();
}

void GPDModule::resolveObjectDependencies() {
    ModuleObject::resolveObjectDependencies();
}

void GPDModule::run() {

    try {

        //get service
        GPDService* pGPDService =
                Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

        //run until empty
        while (!(pGPDService->isEmptyTaskQueue())) {

            //kinematics
            GPDKinematic kinematic;

            //list of GPD types
            List<GPDType> gpdTypeList;

            //set
            ElemUtils::Packet packet = pGPDService->popTaskFormQueue();
            packet >> kinematic;
            packet >> gpdTypeList;

            //debug information
            debug(__func__,
                    ElemUtils::Formatter() << "objectId = " << getObjectId()
                            << " " << kinematic.toString());

            //object to be returned
            GPDResult gpdResult = compute(kinematic, gpdTypeList);

            //helpful to sort later if kinematic is coming from database
            gpdResult.setIndexId(kinematic.getIndexId());

            //add
            pGPDService->add(gpdResult);
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void GPDModule::configure(const ElemUtils::Parameters &parameters) {
    ModuleObject::configure(parameters);
}

void GPDModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    ModuleObject::prepareSubModules(subModulesData);

    std::map<std::string, BaseObjectData>::const_iterator it;

    it = subModulesData.find(
            GPDEvolutionModule::GPD_EVOLUTION_MODULE_CLASS_NAME);

    if (it != subModulesData.end()) {

        if (m_pGPDEvolutionModule) {

            setEvolQcdModule(0);
            m_pGPDEvolutionModule = 0;
        }

        if (!m_pGPDEvolutionModule) {

            m_pGPDEvolutionModule =
                    Partons::getInstance()->getModuleObjectFactory()->newGPDEvolutionModule(
                            (it->second).getModuleClassName());

            info(__func__,
                    ElemUtils::Formatter()
                            << "Configure with GPDEvolutionModule = "
                            << m_pGPDEvolutionModule->getClassName());

            m_pGPDEvolutionModule->configure((it->second).getParameters());
            m_pGPDEvolutionModule->prepareSubModules(
                    (it->second).getSubModules());
        }
    }
}

PartonDistribution GPDModule::compute(const GPDKinematic &kinematic,
        GPDType::Type gpdType) {

    //create list
    List<GPDType> list;

    //add
    list.add(GPDType(gpdType));

    //run
    return compute(kinematic, list).getPartonDistribution(gpdType);
}

GPDResult GPDModule::compute(const GPDKinematic &kinematic,
        const List<GPDType>& gpdType) {

    //reset kinematics (virtuality)
    setKinematics(kinematic);

    //execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    GPDResult result(kinematic);

    //loop over GPD types
    for (size_t i = 0; i < gpdType.size(); i++) {

        //search for pointer to function associated to given GPD type
        m_it = m_listGPDComputeTypeAvailable.find(gpdType[i]);

        //check if found
        if (m_it != m_listGPDComputeTypeAvailable.end()) {

            //set GPD type
            setCurrentGPDType(gpdType[i]);

            //evaluate
            PartonDistribution partonDistribution;

            if (m_pGPDEvolutionModule != 0 && (m_MuF2 != m_MuF2_ref)) {

                GPDModule* gpdModule =
                        m_pModuleObjectFactory->cloneModuleObject(this);

                partonDistribution = m_pGPDEvolutionModule->compute(m_x, m_xi,
                        m_t, m_MuF2, m_MuR2, gpdModule, (m_it->first));

                m_pModuleObjectFactory->updateModulePointerReference(gpdModule,
                        0);
            } else {
                partonDistribution = ((*this).*(m_it->second))();
            }

            //add
            result.addPartonDistribution(gpdType[i], partonDistribution);

        } else {

            //throw error
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "GPD("
                            << GPDType(gpdType[i]).toString()
                            << ") is not available for this  model");
        }
    }

    //set module name
    result.setComputationModuleName(getClassName());

    //return
    return result;
}

List<GPDType> GPDModule::getListOfAvailableGPDTypeForComputation() const {

    //object to be returned
    List<GPDType> listOfAvailableGPDTypeForComputation;

    //iterator
    std::map<GPDType::Type, PartonDistribution (GPDModule::*)()>::const_iterator it;

    //fill list
    for (it = m_listGPDComputeTypeAvailable.begin();
            it != m_listGPDComputeTypeAvailable.end(); it++) {
        listOfAvailableGPDTypeForComputation.add(it->first);
    }

    //return
    return listOfAvailableGPDTypeForComputation;
}

double GPDModule::getMuF2Ref() const {
    return m_MuF2_ref;
}

const GPDEvolutionModule* GPDModule::getEvolQcdModule() const {
    return m_pGPDEvolutionModule;
}

void GPDModule::setEvolQcdModule(GPDEvolutionModule* pEvolQcdModule) {
    m_pModuleObjectFactory->updateModulePointerReference(m_pGPDEvolutionModule,
            pEvolQcdModule);
    m_pGPDEvolutionModule = pEvolQcdModule;
}

PartonDistribution GPDModule::computeH() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution GPDModule::computeE() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution GPDModule::computeHt() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution GPDModule::computeEt() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution GPDModule::computeHTrans() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution GPDModule::computeETrans() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution GPDModule::computeHtTrans() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution GPDModule::computeEtTrans() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution GPDModule::computeH3p() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution GPDModule::computeE3p() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution GPDModule::computeHt3p() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution GPDModule::computeEt3p() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution GPDModule::computeH3m() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution GPDModule::computeE3m() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution GPDModule::computeHt3m() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution GPDModule::computeEt3m() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

PartonDistribution GPDModule::computeEbarTrans() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your implementation  ; must be implemented in daughter class");
}

void GPDModule::setKinematics(const GPDKinematic& kinematic) {

    m_x = kinematic.getX().makeSameUnitAs(PhysicalUnit::NONE).getValue();
    m_xi = kinematic.getXi().makeSameUnitAs(PhysicalUnit::NONE).getValue();
    m_t = kinematic.getT().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_MuF2 = kinematic.getMuF2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_MuR2 = kinematic.getMuR2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
}

void GPDModule::setCurrentGPDType(GPDType::Type gpdType) {
    m_currentGPDComputeType = gpdType;
}

void GPDModule::initModule() {
}

void GPDModule::isModuleWellConfigured() {

    if (fabs(m_x) > 1.) {
        warn(__func__,
                "Longitudinal momentum fraction should be in [-1., +1.]");
    }

    if (m_xi > 1. || m_xi < 0.) {
        warn(__func__,
                ElemUtils::Formatter()
                        << "Skewness should be in [0., +1.] m_xi = " << m_xi);
    }

    if (m_t > 0.) {
        warn(__func__, "Nucleon momentum transfer should be <= 0.");
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
