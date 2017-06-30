#include <map>
#include <utility>

#include "../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/gpd/GPDGK11.h"
#include "../../../../include/partons/modules/gpd/GPDGK11NoGluons.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"

const unsigned int GPDGK11NoGluons::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDGK11NoGluons("GPDGK11NoGluons"));

GPDGK11NoGluons::GPDGK11NoGluons(const std::string &className) :
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

GPDGK11NoGluons::~GPDGK11NoGluons() {
}

GPDGK11NoGluons* GPDGK11NoGluons::clone() const {
    return new GPDGK11NoGluons(*this);
}

void GPDGK11NoGluons::configure(const ElemUtils::Parameters &parameters) {
    GPDModule::configure(parameters);
}

std::string GPDGK11NoGluons::toString() {
    return GPDModule::toString();
}

GPDGK11NoGluons::GPDGK11NoGluons(const GPDGK11NoGluons& other) :
        GPDModule(other) {

    pGPDService = other.pGPDService;
    GKmodel = other.GKmodel;
}

void GPDGK11NoGluons::isModuleWellConfigured() {
    GPDModule::isModuleWellConfigured();
}

void GPDGK11NoGluons::initModule() {

    pGPDService =
            Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

    //TODO Why ? It's alreeady a GPDModule !
    GKmodel = Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
            GPDGK11::classId);

    GPDModule::initModule();
}

PartonDistribution GPDGK11NoGluons::computeH() {

    //compute kinematics
    GPDKinematic gpdKinematic(m_x, m_xi, m_t, m_MuF2, m_MuR2);

    //calculate GK11
    PartonDistribution partonDistribution = GKmodel->compute(gpdKinematic,
            GPDType(GPDType::H), false);
    //set gluon distribution to 0.
    partonDistribution.setGluonDistribution(GluonDistribution(0.));

    return partonDistribution;
}

PartonDistribution GPDGK11NoGluons::computeHt() {

    //compute kinematics
    GPDKinematic gpdKinematic(m_x, m_xi, m_t, m_MuF2, m_MuR2);

    //calculate GK11
    PartonDistribution partonDistribution = GKmodel->compute(gpdKinematic,
            GPDType(GPDType::Ht), false);
    //set gluon distribution to 0.
    partonDistribution.setGluonDistribution(GluonDistribution(0.));

    return partonDistribution;
}

PartonDistribution GPDGK11NoGluons::computeE() {

    //compute kinematics
    GPDKinematic gpdKinematic(m_x, m_xi, m_t, m_MuF2, m_MuR2);

    //calculate GK11
    PartonDistribution partonDistribution = GKmodel->compute(gpdKinematic,
            GPDType(GPDType::E), false);
    //set gluon distribution to 0.
    partonDistribution.setGluonDistribution(GluonDistribution(0.));

    return partonDistribution;
}

PartonDistribution GPDGK11NoGluons::computeEt() {

    //compute kinematics
    GPDKinematic gpdKinematic(m_x, m_xi, m_t, m_MuF2, m_MuR2);

    //calculate GK11
    PartonDistribution partonDistribution = GKmodel->compute(gpdKinematic,
            GPDType(GPDType::Et), false);
    //set gluon distribution to 0.
    partonDistribution.setGluonDistribution(GluonDistribution(0.));

    return partonDistribution;
}
