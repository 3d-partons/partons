#include "../../../../include/partons/modules/gpd/GPDModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/thread/Packet.h>
#include <math.h>
#include <iostream>
#include <utility>

#include "../../../../include/partons/beans/gpd/GPDResult.h"
#include "../../../../include/partons/modules/evolution/gpd/GPDEvolutionModule.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/services/GPDService.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../include/partons/ServiceObjectTyped.h"

namespace PARTONS {

const std::string GPDModule::GPD_MODULE_CLASS_NAME = "GPDModule";

const std::string GPDModule::GPD_TYPE = "GPD_MODULE_GPD_TYPE";

GPDModule::GPDModule(const std::string &className) :
        ModuleObject(className), m_x(0.), m_xi(0.), m_t(0.), m_MuF2(0.), m_MuR2(
                0.), m_gpdType(GPDType::ALL), m_MuF2_ref(0.), m_pGPDEvolutionModule(
                0) {
}

GPDModule::GPDModule(const GPDModule &other) :
        ModuleObject(other) {
    m_x = other.m_x;
    m_xi = other.m_xi;
    m_t = other.m_t;
    m_MuF2 = other.m_MuF2;
    m_MuR2 = other.m_MuR2;

    m_gpdType = other.m_gpdType;

    m_MuF2_ref = other.m_MuF2_ref;

    if (other.m_pGPDEvolutionModule != 0) {
        m_pGPDEvolutionModule = other.m_pGPDEvolutionModule->clone();
    } else {
        m_pGPDEvolutionModule = 0;
    }

    m_listGPDComputeTypeAvailable = other.m_listGPDComputeTypeAvailable;
    m_it = other.m_it;
}

GPDModule::~GPDModule() {
    if (m_pGPDEvolutionModule) {
        setEvolQcdModule(0);
        m_pGPDEvolutionModule = 0;
    }
}

void GPDModule::configure(const ElemUtils::Parameters &parameters) {
    if (parameters.isAvailable(GPDModule::GPD_TYPE)) {
        m_gpdType =
                static_cast<GPDType::Type>(parameters.getLastAvailable().toUInt());
    }

    ModuleObject::configure(parameters);
}

//TODO implement
void GPDModule::initModule() {
    debug(__func__, ElemUtils::Formatter() << "executed");
}

void GPDModule::isModuleWellConfigured() {
    debug(__func__, ElemUtils::Formatter() << "executed");

    // Test variable range

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

    // Test initialization of reference factorization scale

    if (m_MuF2_ref <= 0.) {
        warn(__func__,
                "Square of reference factorization scale should be > 0.");
    }
}

void GPDModule::preCompute(double x, double xi, double t, double MuF2,
        double MuR2, GPDType::Type gpdType) {
    m_x = x;
    m_xi = xi;
    m_t = t;
    m_MuF2 = MuF2;
    m_MuR2 = MuR2;
    m_gpdType = gpdType;

    debug(__func__,
            ElemUtils::Formatter() << "x = " << m_x << "    xi = " << m_xi
                    << "    t = " << m_t << " GeV2    MuF = " << m_MuF2
                    << " GeV    MuR = " << m_MuR2 << " GeV");

    // execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();
}

PartonDistribution GPDModule::compute(const GPDKinematic &kinematic,
        GPDType gpdType, bool evolution) {
    return compute(kinematic.getX(), kinematic.getXi(), kinematic.getT(),
            kinematic.getMuF2(), kinematic.getMuR2(), gpdType.getType(),
            evolution);
}

PartonDistribution GPDModule::compute(double x, double xi, double t,
        double MuF2, double MuR2, GPDType::Type gpdType, bool evolution) {

    preCompute(x, xi, t, MuF2, MuR2, gpdType);

    if (evolution) {
        if (m_pGPDEvolutionModule == 0) {
            evolution = false;
        }
    }

    PartonDistribution partonDistribution;
    m_it = m_listGPDComputeTypeAvailable.find(m_gpdType);
    if (m_it != m_listGPDComputeTypeAvailable.end()) {

        if (evolution && (m_MuF2 != m_MuF2_ref)) {
            partonDistribution = m_pGPDEvolutionModule->compute(m_x, m_xi, m_t,
                    m_MuF2, m_MuR2, this, (m_it->first));
        } else {
            partonDistribution = ((*this).*(m_it->second))();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "GPD("
                        << GPDType(m_gpdType).toString()
                        << ") is not available for this GPD model");
    }

    debug(__func__, ElemUtils::Formatter() << partonDistribution.toString());

    return partonDistribution;
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

const GPDEvolutionModule* GPDModule::getEvolQcdModule() const {
    return m_pGPDEvolutionModule;
}

void GPDModule::setEvolQcdModule(GPDEvolutionModule* pEvolQcdModule) {
    m_pModuleObjectFactory->updateModulePointerReference(m_pGPDEvolutionModule,
            pEvolQcdModule);
    m_pGPDEvolutionModule = pEvolQcdModule;
}

double GPDModule::getMuF2Ref() const {
    return m_MuF2_ref;
}

std::string GPDModule::toString() const {
    return ModuleObject::toString();
}

double GPDModule::getMuF2() const {
    return m_MuF2;
}

void GPDModule::setMuF2(double muF2) {
    m_MuF2 = muF2;
}

double GPDModule::getMuR2() const {
    return m_MuR2;
}

void GPDModule::setMuR2(double muR2) {
    m_MuR2 = muR2;
}

double GPDModule::getT() const {
    return m_t;
}

void GPDModule::setT(double t) {
    m_t = t;
}

double GPDModule::getX() const {
    return m_x;
}

void GPDModule::setX(double x) {
    m_x = x;
}

double GPDModule::getXi() const {
    return m_xi;
}

void GPDModule::setXi(double xi) {
    m_xi = xi;
}

void GPDModule::run() {
    try {
        GPDService* pGPDService =
                Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

        while (!(pGPDService->isEmptyTaskQueue())) {
            GPDKinematic kinematic;
            List<GPDType> gpdTypeList;

            ElemUtils::Packet packet = pGPDService->popTaskFormQueue();
            packet >> kinematic;
            packet >> gpdTypeList;

            debug(__func__,
                    ElemUtils::Formatter() << "objectId = " << getObjectId()
                            << " " << kinematic.toString());

            GPDResult gpdResult;
            gpdResult.setKinematic(kinematic);
            gpdResult.setComputationModuleName(getClassName());

            //Helpful to sort later if kinematic is coming from database
            gpdResult.setIndexId(kinematic.getIndexId());

            for (unsigned int i = 0; i != gpdTypeList.size(); i++) {
                gpdResult.addPartonDistribution(gpdTypeList[i].getType(),
                        compute(kinematic, gpdTypeList[i].getType()));
            }

            pGPDService->add(gpdResult);

            //TODO useful to do a sleep ?
            // sf::sleep(sf::milliseconds(3));
        }
    } catch (std::exception &e) {
        //TODO : Is it possible to print an error log message ?
        std::cerr << e.what() << std::endl;
    }
}

List<GPDType> GPDModule::getListOfAvailableGPDTypeForComputation() const {
    std::map<GPDType::Type, PartonDistribution (GPDModule::*)()>::const_iterator it;
    List<GPDType> listOfAvailableGPDTypeForComputation;

    for (it = m_listGPDComputeTypeAvailable.begin();
            it != m_listGPDComputeTypeAvailable.end(); it++) {
        listOfAvailableGPDTypeForComputation.add(it->first);
    }

    return listOfAvailableGPDTypeForComputation;
}

//TODO replace hardcoded module name
//void GPDModule::prepareComputationConfiguration(
//        const List<ElemUtils::Parameter>& moduleNameList,
//        const unsigned int level) {
//    if (level < moduleNameList.size()) {
//        if (ElemUtils::StringUtils::equals(moduleNameList[level].getName(),
//                "GPDEvolutionModule")) {
//            prepareGPDEvolutionModule(moduleNameList, level);
//        }
//    }
//}
//
//void GPDModule::prepareGPDEvolutionModule(
//        const List<ElemUtils::Parameter>& moduleNameList, unsigned int level) {
//    if (m_pGPDEvolutionModule) {
//        if (!ElemUtils::StringUtils::equals(
//                m_pGPDEvolutionModule->getClassName(),
//                moduleNameList[level].getString())) {
//            delete m_pGPDEvolutionModule;
//            m_pGPDEvolutionModule = 0;
//        }
//    }
//
//    if (!m_pGPDEvolutionModule) {
//        m_pGPDEvolutionModule =
//                Partons::getInstance()->getModuleObjectFactory()->newGPDEvolutionModule(
//                        moduleNameList[level].getString());
//    }
//
//    // m_pGPDEvolutionModule->prepareComputationConfiguration(moduleNameList, ++level);
//}

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

} /* namespace PARTONS */
