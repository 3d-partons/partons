#include "../../../../../include/partons/modules/scales/DVCS/ScalesQ2Multiplier.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../../include/partons/beans/Scales.h"

namespace PARTONS {

const std::string DVCSScalesQ2Multiplier::PARAMETER_NAME_LAMBDA = "lambda";

const unsigned int DVCSScalesQ2Multiplier::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSScalesQ2Multiplier("DVCSScalesQ2Multiplier"));

DVCSScalesQ2Multiplier::DVCSScalesQ2Multiplier(const std::string &className) :
        DVCSScalesModule(className), m_lambda(1.) {
}

DVCSScalesQ2Multiplier::DVCSScalesQ2Multiplier(
        const DVCSScalesQ2Multiplier &other) :
        DVCSScalesModule(other), m_lambda(other.m_lambda) {
}

DVCSScalesQ2Multiplier::~DVCSScalesQ2Multiplier() {
}

DVCSScalesQ2Multiplier* DVCSScalesQ2Multiplier::clone() const {
    return new DVCSScalesQ2Multiplier(*this);
}

void DVCSScalesQ2Multiplier::initModule() {
    DVCSScalesModule::initModule();
}

void DVCSScalesQ2Multiplier::isModuleWellConfigured() {
    DVCSScalesModule::isModuleWellConfigured();
}

void DVCSScalesQ2Multiplier::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSScalesModule::configure(parameters);

    if (parameters.isAvailable(DVCSScalesQ2Multiplier::PARAMETER_NAME_LAMBDA)) {

        m_lambda = parameters.getLastAvailable().toDouble();

        info(__func__,
                ElemUtils::Formatter()
                        << DVCSScalesQ2Multiplier::PARAMETER_NAME_LAMBDA
                        << " configured with value = " << m_lambda);
    }
}

Scales DVCSScalesQ2Multiplier::compute(
        const DVCSObservableKinematic& kinematic) {

    initModule();
    isModuleWellConfigured();

    PhysicalType<double> scale(m_lambda * kinematic.getQ2());

    return Scales(scale, scale);
}

} /* namespace PARTONS */
