#include "../../../../include/partons/modules/gpd/GK11ModelNoGluons.h"

#include <map>
#include <utility>

#include "../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/gpd/GK11Model.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"

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

void GK11ModelNoGluons::configure(const ElemUtils::Parameters &parameters) {
    GPDModule::configure(parameters);
}

std::string GK11ModelNoGluons::toString() const {
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

    pGPDService =
            Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

    //TODO Why ? It's alreeady a GPDModule !
    GKmodel = Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
            GK11Model::classId);

    GPDModule::initModule();
}

PartonDistribution GK11ModelNoGluons::computeH() {

    //compute kinematics
    GPDKinematic gpdKinematic(m_x, m_xi, m_t, m_MuF2, m_MuR2);

    //calculate GK11
    PartonDistribution partonDistribution = GKmodel->compute(gpdKinematic,
            GPDType(GPDType::H), false);
    //set gluon distribution to 0.
    partonDistribution.setGluonDistribution(GluonDistribution(0.));

    return partonDistribution;
}

PartonDistribution GK11ModelNoGluons::computeHt() {

    //compute kinematics
    GPDKinematic gpdKinematic(m_x, m_xi, m_t, m_MuF2, m_MuR2);

    //calculate GK11
    PartonDistribution partonDistribution = GKmodel->compute(gpdKinematic,
            GPDType(GPDType::Ht), false);
    //set gluon distribution to 0.
    partonDistribution.setGluonDistribution(GluonDistribution(0.));

    return partonDistribution;
}

PartonDistribution GK11ModelNoGluons::computeE() {

    //compute kinematics
    GPDKinematic gpdKinematic(m_x, m_xi, m_t, m_MuF2, m_MuR2);

    //calculate GK11
    PartonDistribution partonDistribution = GKmodel->compute(gpdKinematic,
            GPDType(GPDType::E), false);
    //set gluon distribution to 0.
    partonDistribution.setGluonDistribution(GluonDistribution(0.));

    return partonDistribution;
}

PartonDistribution GK11ModelNoGluons::computeEt() {

    //compute kinematics
    GPDKinematic gpdKinematic(m_x, m_xi, m_t, m_MuF2, m_MuR2);

    //calculate GK11
    PartonDistribution partonDistribution = GKmodel->compute(gpdKinematic,
            GPDType(GPDType::Et), false);
    //set gluon distribution to 0.
    partonDistribution.setGluonDistribution(GluonDistribution(0.));

    return partonDistribution;
}
