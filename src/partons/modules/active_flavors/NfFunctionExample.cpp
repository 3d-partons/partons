#include "../../../../include/partons/modules/active_flavors/NfFunctionExample.h"

#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int NfFunctionExample::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new NfFunctionExample("NfFunctionExample"));

NfFunctionExample::NfFunctionExample(const std::string &className) :
        ActiveFlavorsModule(className) {
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

NfFunctionExample::~NfFunctionExample() {
}

NfFunctionExample::NfFunctionExample(const NfFunctionExample& other) :
        ActiveFlavorsModule(other) {
}

NfFunctionExample* NfFunctionExample::clone() const {
    return new NfFunctionExample(*this);
}

void NfFunctionExample::initModule() {
    ActiveFlavorsModule::initModule();
}

void NfFunctionExample::isModuleWellConfigured() {
    ActiveFlavorsModule::isModuleWellConfigured();
}
