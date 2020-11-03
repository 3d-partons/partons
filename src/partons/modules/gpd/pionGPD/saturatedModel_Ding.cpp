#include "../../../../include/partons/modules/gpd/pionGPD/saturatedModel_Ding.h"

#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {


const unsigned int saturatedModel_Ding::classId =
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new saturatedModel_Ding("saturatedModel_Ding"));
saturatedModel_Ding::saturatedModel_Ding(const std::string &className) : PARTONS::GPDModule(className) {
    //relate a specific GPD type with the appropriate function
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(PARTONS::GPDType::H, &PARTONS::GPDModule::computeH));
   /* m_listGPDComputeTypeAvailable.insert(
            std::make_pair(PARTONS::GPDType::E, &PARTONS::GPDModule::computeE));*/
}
saturatedModel_Ding::saturatedModel_Ding(const saturatedModel_Ding& other) : PARTONS::GPDModule(other) {
}
saturatedModel_Ding::~saturatedModel_Ding() {
}
saturatedModel_Ding* saturatedModel_Ding::clone() const {
    return new saturatedModel_Ding(*this);
}
void saturatedModel_Ding::resolveObjectDependencies() {
}
void saturatedModel_Ding::configure(const ElemUtils::Parameters &parameters) {
    PARTONS::GPDModule::configure(parameters);
}
void saturatedModel_Ding::isModuleWellConfigured() {
    PARTONS::GPDModule::isModuleWellConfigured();
}
void saturatedModel_Ding::initModule() {
    PARTONS::GPDModule::initModule();
}
PARTONS::PartonDistribution saturatedModel_Ding::computeH() {
    //result
    PARTONS::PartonDistribution result;
    //your implementation comes here
    //...
    //return
    return result;

}

}
