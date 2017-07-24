#include "../../../../include/partons/modules/active_flavors_thresholds/ActiveFlavorsThresholdsQuarkMasses.h"

#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

const unsigned int ActiveFlavorsThresholdsQuarkMasses::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new ActiveFlavorsThresholdsQuarkMasses("ActiveFlavorsThresholdsQuarkMasses"));

ActiveFlavorsThresholdsQuarkMasses::ActiveFlavorsThresholdsQuarkMasses(const std::string &className) :
        ActiveFlavorsThresholdsModule(className) {
    addNfInterval(1, 0., Constant::QUARK_UP_MASS * Constant::QUARK_UP_MASS);
    addNfInterval(2, Constant::QUARK_UP_MASS * Constant::QUARK_UP_MASS,
            Constant::QUARK_DOWN_MASS * Constant::QUARK_DOWN_MASS);
    addNfInterval(3, Constant::QUARK_DOWN_MASS * Constant::QUARK_DOWN_MASS,
            Constant::QUARK_STRANGE_MASS * Constant::QUARK_STRANGE_MASS);
    addNfInterval(4, Constant::QUARK_STRANGE_MASS * Constant::QUARK_STRANGE_MASS,
            Constant::QUARK_CHARM_MASS * Constant::QUARK_CHARM_MASS);
    addNfInterval(5, Constant::QUARK_CHARM_MASS * Constant::QUARK_CHARM_MASS,
            Constant::QUARK_BOTTOM_MASS * Constant::QUARK_BOTTOM_MASS);
    addNfInterval(6, Constant::QUARK_BOTTOM_MASS * Constant::QUARK_BOTTOM_MASS,
            Constant::QUARK_TOP_MASS * Constant::QUARK_TOP_MASS);
}

ActiveFlavorsThresholdsQuarkMasses::~ActiveFlavorsThresholdsQuarkMasses() {
}

ActiveFlavorsThresholdsQuarkMasses::ActiveFlavorsThresholdsQuarkMasses(const ActiveFlavorsThresholdsQuarkMasses& other) :
        ActiveFlavorsThresholdsModule(other) {
}

ActiveFlavorsThresholdsQuarkMasses* ActiveFlavorsThresholdsQuarkMasses::clone() const {
    return new ActiveFlavorsThresholdsQuarkMasses(*this);
}

void ActiveFlavorsThresholdsQuarkMasses::initModule() {
    ActiveFlavorsThresholdsModule::initModule();
}

void ActiveFlavorsThresholdsQuarkMasses::isModuleWellConfigured() {
    ActiveFlavorsThresholdsModule::isModuleWellConfigured();
}
