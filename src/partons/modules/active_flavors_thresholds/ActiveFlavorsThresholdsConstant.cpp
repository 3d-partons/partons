#include "../../../../include/partons/modules/active_flavors_thresholds/ActiveFlavorsThresholdsConstant.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <limits>

#include "../../../../include/partons/BaseObjectRegistry.h"

namespace PARTONS {

const std::string ActiveFlavorsThresholdsConstant::PARAM_NAME_N_FLAVORS =
        "nFlavors";

const unsigned int ActiveFlavorsThresholdsConstant::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new ActiveFlavorsThresholdsConstant(
                        "ActiveFlavorsThresholdsConstant"));

ActiveFlavorsThresholdsConstant::ActiveFlavorsThresholdsConstant(
        const std::string &className) :
        ActiveFlavorsThresholdsModule(className), m_nFlavors(3) {
    addNfInterval(m_nFlavors, 0., std::numeric_limits<double>::max());
}

ActiveFlavorsThresholdsConstant::~ActiveFlavorsThresholdsConstant() {
}

ActiveFlavorsThresholdsConstant::ActiveFlavorsThresholdsConstant(
        const ActiveFlavorsThresholdsConstant& other) :
        ActiveFlavorsThresholdsModule(other), m_nFlavors(other.m_nFlavors) {
}

ActiveFlavorsThresholdsConstant* ActiveFlavorsThresholdsConstant::clone() const {
    return new ActiveFlavorsThresholdsConstant(*this);
}

void ActiveFlavorsThresholdsConstant::configure(
        const ElemUtils::Parameters &parameters) {

    ActiveFlavorsThresholdsModule::configure(parameters);

    //check and set
    if (parameters.isAvailable(
            ActiveFlavorsThresholdsConstant::PARAM_NAME_N_FLAVORS)) {
        setNFlavors(parameters.getLastAvailable().toUInt());
    }
}

void ActiveFlavorsThresholdsConstant::resolveObjectDependencies() {
    ActiveFlavorsThresholdsModule::resolveObjectDependencies();
}

void ActiveFlavorsThresholdsConstant::initModule() {
    ActiveFlavorsThresholdsModule::initModule();
}

void ActiveFlavorsThresholdsConstant::isModuleWellConfigured() {
    ActiveFlavorsThresholdsModule::isModuleWellConfigured();
}

void ActiveFlavorsThresholdsConstant::setNFlavors(size_t nFlavors) {

    if (nFlavors != m_nFlavors) {

        info(__func__,
                ElemUtils::Formatter()
                        << "Number of active flavors changed from "
                        << m_nFlavors << " to " << nFlavors);

        m_nFlavors = nFlavors;
        reset();
        addNfInterval(m_nFlavors, 0., std::numeric_limits<double>::max());

    }
}

size_t ActiveFlavorsThresholdsConstant::getNFlavors() const {
    return m_nFlavors;
}

} /* namespace PARTONS */
