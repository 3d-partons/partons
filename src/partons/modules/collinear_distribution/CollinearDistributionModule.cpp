#include "../../../../include/partons/modules/collinear_distribution/CollinearDistributionModule.h"

#include <math.h>

namespace PARTONS {

const std::string CollinearDistributionModule::COLLINEAR_DISTRIBUTION_MODULE_CLASS_NAME = "CollinearDistributionModule";

CollinearDistributionModule::CollinearDistributionModule(const std::string &className) :
        ModuleObject(className, ChannelType::UNDEFINED), m_x(0.), m_MuF2(0.), m_MuR2(0.) {
}

CollinearDistributionModule::CollinearDistributionModule(const CollinearDistributionModule &other) :
        ModuleObject(other), m_x(other.m_x), m_MuF2(
                other.m_MuF2), m_MuR2(other.m_MuR2) {
}

CollinearDistributionModule::~CollinearDistributionModule() {
}

std::string CollinearDistributionModule::toString() const {
    return ModuleObject::toString();
}

void CollinearDistributionModule::resolveObjectDependencies() {
    ModuleObject::resolveObjectDependencies();
}

void CollinearDistributionModule::configure(const ElemUtils::Parameters &parameters) {
    ModuleObject::configure(parameters);
}

PartonDistribution CollinearDistributionModule::compute(const PartonDistributionKinematic &kinematic) {

    //reset kinematics (virtuality)
    setKinematics(kinematic);

    //execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();

    //evaluate
    PartonDistribution partonDistribution;

    //return
    return compute();
}

void CollinearDistributionModule::setKinematics(const PartonDistributionKinematic& kinematic) {

    m_x = kinematic.getX().makeSameUnitAs(PhysicalUnit::NONE).getValue();
    m_MuF2 = kinematic.getMuF2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_MuR2 = kinematic.getMuR2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
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
}

} /* namespace PARTONS */
