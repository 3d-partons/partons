#include "../../../../include/partons/modules/double_distribution/DoubleDistributionFromOverlap.h"

#include <utility>

#include "../../../../include/partons/beans/double_distribution/DoubleDistributionType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"

namespace PARTONS {


const unsigned int DoubleDistributionFromOverlap::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DoubleDistributionFromOverlap(
                        "DoubleDistributionFromOverlap"));

DoubleDistributionFromOverlap::DoubleDistributionFromOverlap(
        const std::string &className) :
        DoubleDistributionModule(className) {
    setInversionDependent(true);

    // Tell to parent class which dual distribution compute type are available for this model.
    // If there is just computeF() and computeG() available just remove computeK() line.

    m_listOfAvailableDoubleDistributionFunctions.insert(
            std::make_pair(DoubleDistributionType::F,
                    &DoubleDistributionModule::computeF));
    m_listOfAvailableDoubleDistributionFunctions.insert(
            std::make_pair(DoubleDistributionType::G,
                    &DoubleDistributionModule::computeG));
    m_listOfAvailableDoubleDistributionFunctions.insert(
            std::make_pair(DoubleDistributionType::K,
                    &DoubleDistributionModule::computeK));

}

DoubleDistributionFromOverlap::~DoubleDistributionFromOverlap() {
}

DoubleDistributionFromOverlap::DoubleDistributionFromOverlap(
        const DoubleDistributionFromOverlap& other) :
        DoubleDistributionModule(other) {

    // Copy each class variables ; see copy constructor from parent class "DualDistributionModule"
}

DoubleDistributionFromOverlap* DoubleDistributionFromOverlap::clone() const {
    return new DoubleDistributionFromOverlap(*this);
}

void DoubleDistributionFromOverlap::resolveObjectDependencies() {
}

void DoubleDistributionFromOverlap::initModule() {
    // Before call parent init.
    DoubleDistributionModule::initModule();

    // Compute some variables depend from kinematic value before use this specific module.
    // See "src/modules/convolCoeffFunction/DVCSCFFModel::initModule()" for example.
}

void DoubleDistributionFromOverlap::isModuleWellConfigured() {
    // Before check if parent class is well configured
    DoubleDistributionModule::isModuleWellConfigured();

    //TODO check variables range of this module and other stuff
    // See "src/modules/EvolQCDModule::isModuleWellConfigured()" for example.
}

PartonDistribution DoubleDistributionFromOverlap::computeF() {
    PartonDistribution partonDistribution;

    return partonDistribution;
}

PartonDistribution DoubleDistributionFromOverlap::computeG() {
    PartonDistribution partonDistribution;

    return partonDistribution;
}

PartonDistribution DoubleDistributionFromOverlap::computeK() {
    PartonDistribution partonDistribution;

    return partonDistribution;
}


} /* namespace PARTONS */
