/*
 * GK11ModelNoGluons.cpp
 *
 *  Created on: Oct 5, 2015
 *      Author: Pawel Sznajder
 */

#include "../../../../include/partons/modules/gpd/GK11ModelNoGluons.h"

#include <map>
#include <utility>

#include "../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../include/partons/beans/gpd/GPDResult.h"
#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/gpd/GK11Model.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/services/GPDService.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../include/partons/utils/ParameterList.h"

const unsigned int GK11ModelNoGluons::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GK11ModelNoGluons("GK11ModelNoGluons"));

GK11ModelNoGluons::GK11ModelNoGluons(const std::string &className) :
        GPDModule(className) {
    pGPDService = NULL;
    GKmodel = NULL;

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Ht, &GPDModule::computeHt));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::E, &GPDModule::computeE));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Et, &GPDModule::computeEt));
}

GK11ModelNoGluons::~GK11ModelNoGluons() {
}

GK11ModelNoGluons* GK11ModelNoGluons::clone() const {
    return new GK11ModelNoGluons(*this);
}

void GK11ModelNoGluons::configure(ParameterList parameters) {
}

std::string GK11ModelNoGluons::toString() {
    return GPDModule::toString();
}

GK11ModelNoGluons::GK11ModelNoGluons(const GK11ModelNoGluons& other) :
        GPDModule(other) {

    pGPDService = other.pGPDService;
    GKmodel = other.GKmodel;
}

void GK11ModelNoGluons::isModuleWellConfigured() {
    GPDModule::isModuleWellConfigured();
}

void GK11ModelNoGluons::initModule() {

    pGPDService = ServiceObjectRegistry::getGPDService();

    GKmodel = ModuleObjectFactory::newGPDModule(GK11Model::classId);

    GPDModule::initModule();
}

PartonDistribution GK11ModelNoGluons::computeH() {

    //compute kinematics
    GPDKinematic gpdKinematic(m_x, m_xi, m_t, m_MuF2, m_MuR2);

    //calculate GK11
    GPDResult gpdResult = pGPDService->computeGPDModelRestrictedByGPDType(
            gpdKinematic, GKmodel, GPDType::H);

    //set
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_u.setQuarkDistribution(
            gpdResult.getPartonDistribution(GPDType::H).getQuarkDistribution(
                    QuarkFlavor::UP).getQuarkDistribution());
    quarkDistribution_d.setQuarkDistribution(
            gpdResult.getPartonDistribution(GPDType::H).getQuarkDistribution(
                    QuarkFlavor::DOWN).getQuarkDistribution());
    quarkDistribution_s.setQuarkDistribution(
            gpdResult.getPartonDistribution(GPDType::H).getQuarkDistribution(
                    QuarkFlavor::STRANGE).getQuarkDistribution());

    quarkDistribution_u.setQuarkDistributionPlus(
            gpdResult.getPartonDistribution(GPDType::H).getQuarkDistribution(
                    QuarkFlavor::UP).getQuarkDistributionPlus());
    quarkDistribution_d.setQuarkDistributionPlus(
            gpdResult.getPartonDistribution(GPDType::H).getQuarkDistribution(
                    QuarkFlavor::DOWN).getQuarkDistributionPlus());
    quarkDistribution_s.setQuarkDistributionPlus(
            gpdResult.getPartonDistribution(GPDType::H).getQuarkDistribution(
                    QuarkFlavor::STRANGE).getQuarkDistributionPlus());

    quarkDistribution_u.setQuarkDistributionMinus(
            gpdResult.getPartonDistribution(GPDType::H).getQuarkDistribution(
                    QuarkFlavor::UP).getQuarkDistributionMinus());
    quarkDistribution_d.setQuarkDistributionMinus(
            gpdResult.getPartonDistribution(GPDType::H).getQuarkDistribution(
                    QuarkFlavor::DOWN).getQuarkDistributionMinus());
    quarkDistribution_s.setQuarkDistributionMinus(
            gpdResult.getPartonDistribution(GPDType::H).getQuarkDistribution(
                    QuarkFlavor::STRANGE).getQuarkDistributionMinus());

    GluonDistribution gluonDistribution(0.);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

PartonDistribution GK11ModelNoGluons::computeHt() {

    //compute kinematics
    GPDKinematic gpdKinematic(m_x, m_xi, m_t, m_MuF2, m_MuR2);

    //calculate GK11
    GPDResult gpdResult = pGPDService->computeGPDModelRestrictedByGPDType(
            gpdKinematic, GKmodel, GPDType::Ht);

    //set
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_u.setQuarkDistribution(
            gpdResult.getPartonDistribution(GPDType::Ht).getQuarkDistribution(
                    QuarkFlavor::UP).getQuarkDistribution());
    quarkDistribution_d.setQuarkDistribution(
            gpdResult.getPartonDistribution(GPDType::Ht).getQuarkDistribution(
                    QuarkFlavor::DOWN).getQuarkDistribution());
    quarkDistribution_s.setQuarkDistribution(
            gpdResult.getPartonDistribution(GPDType::Ht).getQuarkDistribution(
                    QuarkFlavor::STRANGE).getQuarkDistribution());

    quarkDistribution_u.setQuarkDistributionPlus(
            gpdResult.getPartonDistribution(GPDType::Ht).getQuarkDistribution(
                    QuarkFlavor::UP).getQuarkDistributionPlus());
    quarkDistribution_d.setQuarkDistributionPlus(
            gpdResult.getPartonDistribution(GPDType::Ht).getQuarkDistribution(
                    QuarkFlavor::DOWN).getQuarkDistributionPlus());
    quarkDistribution_s.setQuarkDistributionPlus(
            gpdResult.getPartonDistribution(GPDType::Ht).getQuarkDistribution(
                    QuarkFlavor::STRANGE).getQuarkDistributionPlus());

    quarkDistribution_u.setQuarkDistributionMinus(
            gpdResult.getPartonDistribution(GPDType::Ht).getQuarkDistribution(
                    QuarkFlavor::UP).getQuarkDistributionMinus());
    quarkDistribution_d.setQuarkDistributionMinus(
            gpdResult.getPartonDistribution(GPDType::Ht).getQuarkDistribution(
                    QuarkFlavor::DOWN).getQuarkDistributionMinus());
    quarkDistribution_s.setQuarkDistributionMinus(
            gpdResult.getPartonDistribution(GPDType::Ht).getQuarkDistribution(
                    QuarkFlavor::STRANGE).getQuarkDistributionMinus());

    GluonDistribution gluonDistribution(0.);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

PartonDistribution GK11ModelNoGluons::computeE() {

    //compute kinematics
    GPDKinematic gpdKinematic(m_x, m_xi, m_t, m_MuF2, m_MuR2);

    //calculate GK11
    GPDResult gpdResult = pGPDService->computeGPDModelRestrictedByGPDType(
            gpdKinematic, GKmodel, GPDType::H);

    //set
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_u.setQuarkDistribution(
            gpdResult.getPartonDistribution(GPDType::E).getQuarkDistribution(
                    QuarkFlavor::UP).getQuarkDistribution());
    quarkDistribution_d.setQuarkDistribution(
            gpdResult.getPartonDistribution(GPDType::E).getQuarkDistribution(
                    QuarkFlavor::DOWN).getQuarkDistribution());
    quarkDistribution_s.setQuarkDistribution(
            gpdResult.getPartonDistribution(GPDType::E).getQuarkDistribution(
                    QuarkFlavor::STRANGE).getQuarkDistribution());

    quarkDistribution_u.setQuarkDistributionPlus(
            gpdResult.getPartonDistribution(GPDType::E).getQuarkDistribution(
                    QuarkFlavor::UP).getQuarkDistributionPlus());
    quarkDistribution_d.setQuarkDistributionPlus(
            gpdResult.getPartonDistribution(GPDType::E).getQuarkDistribution(
                    QuarkFlavor::DOWN).getQuarkDistributionPlus());
    quarkDistribution_s.setQuarkDistributionPlus(
            gpdResult.getPartonDistribution(GPDType::E).getQuarkDistribution(
                    QuarkFlavor::STRANGE).getQuarkDistributionPlus());

    quarkDistribution_u.setQuarkDistributionMinus(
            gpdResult.getPartonDistribution(GPDType::E).getQuarkDistribution(
                    QuarkFlavor::UP).getQuarkDistributionMinus());
    quarkDistribution_d.setQuarkDistributionMinus(
            gpdResult.getPartonDistribution(GPDType::E).getQuarkDistribution(
                    QuarkFlavor::DOWN).getQuarkDistributionMinus());
    quarkDistribution_s.setQuarkDistributionMinus(
            gpdResult.getPartonDistribution(GPDType::E).getQuarkDistribution(
                    QuarkFlavor::STRANGE).getQuarkDistributionMinus());

    GluonDistribution gluonDistribution(0.);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

PartonDistribution GK11ModelNoGluons::computeEt() {

    //compute kinematics
    GPDKinematic gpdKinematic(m_x, m_xi, m_t, m_MuF2, m_MuR2);

    //calculate GK11
    GPDResult gpdResult = pGPDService->computeGPDModelRestrictedByGPDType(
            gpdKinematic, GKmodel, GPDType::H);

    //set
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_u.setQuarkDistribution(
            gpdResult.getPartonDistribution(GPDType::Et).getQuarkDistribution(
                    QuarkFlavor::UP).getQuarkDistribution());
    quarkDistribution_d.setQuarkDistribution(
            gpdResult.getPartonDistribution(GPDType::Et).getQuarkDistribution(
                    QuarkFlavor::DOWN).getQuarkDistribution());
    quarkDistribution_s.setQuarkDistribution(
            gpdResult.getPartonDistribution(GPDType::Et).getQuarkDistribution(
                    QuarkFlavor::STRANGE).getQuarkDistribution());

    quarkDistribution_u.setQuarkDistributionPlus(
            gpdResult.getPartonDistribution(GPDType::Et).getQuarkDistribution(
                    QuarkFlavor::UP).getQuarkDistributionPlus());
    quarkDistribution_d.setQuarkDistributionPlus(
            gpdResult.getPartonDistribution(GPDType::Et).getQuarkDistribution(
                    QuarkFlavor::DOWN).getQuarkDistributionPlus());
    quarkDistribution_s.setQuarkDistributionPlus(
            gpdResult.getPartonDistribution(GPDType::Et).getQuarkDistribution(
                    QuarkFlavor::STRANGE).getQuarkDistributionPlus());

    quarkDistribution_u.setQuarkDistributionMinus(
            gpdResult.getPartonDistribution(GPDType::Et).getQuarkDistribution(
                    QuarkFlavor::UP).getQuarkDistributionMinus());
    quarkDistribution_d.setQuarkDistributionMinus(
            gpdResult.getPartonDistribution(GPDType::Et).getQuarkDistribution(
                    QuarkFlavor::DOWN).getQuarkDistributionMinus());
    quarkDistribution_s.setQuarkDistributionMinus(
            gpdResult.getPartonDistribution(GPDType::Et).getQuarkDistribution(
                    QuarkFlavor::STRANGE).getQuarkDistributionMinus());

    GluonDistribution gluonDistribution(0.);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

