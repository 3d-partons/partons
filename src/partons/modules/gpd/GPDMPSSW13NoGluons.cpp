#include <map>
#include <utility>

#include "../../../../include/partons/beans/gpd/GPDKinematic.h"
//#include "../../../../include/partons/beans/gpd/GPDResult.h"
#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
//#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
//#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/gpd/GPDMPSSW13.h"
#include "../../../../include/partons/modules/gpd/GPDMPSSW13NoGluons.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"
//#include "../../../../include/partons/services/GPDService.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"

const unsigned int GPDMPSSW13NoGluons::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDMPSSW13NoGluons("GPDMPSSW13NoGluons"));

GPDMPSSW13NoGluons::GPDMPSSW13NoGluons(const std::string &className) :
        GPDModule(className) {
    pGPDService = NULL;
    MPSSW13model = NULL;

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));
}

GPDMPSSW13NoGluons::~GPDMPSSW13NoGluons() {
}

GPDMPSSW13NoGluons* GPDMPSSW13NoGluons::clone() const {
    return new GPDMPSSW13NoGluons(*this);
}

void GPDMPSSW13NoGluons::configure(const ElemUtils::Parameters &parameters) {
    GPDModule::configure(parameters);
}

std::string GPDMPSSW13NoGluons::toString() {
    return GPDModule::toString();
}

GPDMPSSW13NoGluons::GPDMPSSW13NoGluons(const GPDMPSSW13NoGluons& other) :
        GPDModule(other) {

    pGPDService = other.pGPDService;
    MPSSW13model = other.MPSSW13model;
}

void GPDMPSSW13NoGluons::isModuleWellConfigured() {
    GPDModule::isModuleWellConfigured();
}

void GPDMPSSW13NoGluons::initModule() {

    pGPDService =
            Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

    //TODO Why ? It's alreeady a GPDModule !
    MPSSW13model =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    GPDMPSSW13::classId);

    GPDModule::initModule();
}

PartonDistribution GPDMPSSW13NoGluons::computeH() {

    //compute kinematics
    GPDKinematic gpdKinematic(m_x, m_xi, m_t, m_MuF2, m_MuR2);

    //calculate MPSSW13
    PartonDistribution partonDistribution = MPSSW13model->compute(gpdKinematic,
            GPDType(GPDType::H), false);
    //set gluon distribution to 0.
    partonDistribution.setGluonDistribution(GluonDistribution(0.));

    return partonDistribution;
}
