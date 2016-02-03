#include "../../../../include/partons/modules/active_flavors/NfFunctionExample.h"

#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int NfFunctionExample::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new NfFunctionExample("NfFunctionExample"));

NfFunctionExample::NfFunctionExample(const std::string &className) :
        ActiveFlavorsModule(className) {
    addNfInterval(1, 0., QUARK_UP_MASS * QUARK_UP_MASS);
    addNfInterval(2, QUARK_UP_MASS * QUARK_UP_MASS,
            QUARK_DOWN_MASS * QUARK_DOWN_MASS);
    addNfInterval(3, QUARK_DOWN_MASS * QUARK_DOWN_MASS,
            QUARK_STRANGE_MASS * QUARK_STRANGE_MASS);
    addNfInterval(4, QUARK_STRANGE_MASS * QUARK_STRANGE_MASS,
            QUARK_CHARM_MASS * QUARK_CHARM_MASS);
    addNfInterval(5, QUARK_CHARM_MASS * QUARK_CHARM_MASS,
            QUARK_BOTTOM_MASS * QUARK_BOTTOM_MASS);
    addNfInterval(6, QUARK_BOTTOM_MASS * QUARK_BOTTOM_MASS,
            QUARK_TOP_MASS * QUARK_TOP_MASS);
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
