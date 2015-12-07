#include "../../../../include/partons/modules/gpd/MPSSW13ModelNoGluons.h"

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
#include "../../../../include/partons/modules/gpd/MPSSW13Model.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/services/GPDService.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../include/partons/utils/ParameterList.h"

const unsigned int MPSSW13ModelNoGluons::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new MPSSW13ModelNoGluons("MPSSW13ModelNoGluons"));

MPSSW13ModelNoGluons::MPSSW13ModelNoGluons(const std::string &className) :
        GPDModule(className) {
    pGPDService = NULL;
    MPSSW13model = NULL;

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));
}

MPSSW13ModelNoGluons::~MPSSW13ModelNoGluons() {
}

MPSSW13ModelNoGluons* MPSSW13ModelNoGluons::clone() const {
    return new MPSSW13ModelNoGluons(*this);
}

void MPSSW13ModelNoGluons::configure(ParameterList parameters) {
}

std::string MPSSW13ModelNoGluons::toString() {
    return GPDModule::toString();
}

MPSSW13ModelNoGluons::MPSSW13ModelNoGluons(const MPSSW13ModelNoGluons& other) :
        GPDModule(other) {

    pGPDService = other.pGPDService;
    MPSSW13model = other.MPSSW13model;
}

void MPSSW13ModelNoGluons::isModuleWellConfigured() {
    GPDModule::isModuleWellConfigured();
}

void MPSSW13ModelNoGluons::initModule() {

    pGPDService = ServiceObjectRegistry::getGPDService();

    MPSSW13model = ModuleObjectFactory::newGPDModule(MPSSW13Model::classId);

    GPDModule::initModule();
}

PartonDistribution MPSSW13ModelNoGluons::computeH() {

    //compute kinematics
    GPDKinematic gpdKinematic(m_x, m_xi, m_t, m_MuF2, m_MuR2);

    //calculate MPSSW13
    GPDResult gpdResult = pGPDService->computeGPDModelRestrictedByGPDType(
            gpdKinematic, MPSSW13model, GPDType::H);

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
