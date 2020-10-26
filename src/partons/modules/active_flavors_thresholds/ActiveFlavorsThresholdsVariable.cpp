#include "../../../../include/partons/modules/active_flavors_thresholds/ActiveFlavorsThresholdsVariable.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/logger/CustomException.h>
#include <limits>

#include "../../../../include/partons/BaseObjectRegistry.h"

namespace PARTONS {

const std::string ActiveFlavorsThresholdsVariable::PARAM_NAME_THRESHOLDS = "thresholds";

const unsigned int ActiveFlavorsThresholdsVariable::classId = BaseObjectRegistry::getInstance()->registerBaseObject(new ActiveFlavorsThresholdsVariable("ActiveFlavorsThresholdsVariable"));

ActiveFlavorsThresholdsVariable::ActiveFlavorsThresholdsVariable(const std::string &className) :
  ActiveFlavorsThresholdsModule(className), m_thresholds({}) {
}

ActiveFlavorsThresholdsVariable::~ActiveFlavorsThresholdsVariable() {
}

ActiveFlavorsThresholdsVariable::ActiveFlavorsThresholdsVariable(
        const ActiveFlavorsThresholdsVariable& other) :
        ActiveFlavorsThresholdsModule(other), m_thresholds(other.m_thresholds) {
}

ActiveFlavorsThresholdsVariable* ActiveFlavorsThresholdsVariable::clone() const {
    return new ActiveFlavorsThresholdsVariable(*this);
}

void ActiveFlavorsThresholdsVariable::configure(const ElemUtils::Parameters &parameters) {

    ActiveFlavorsThresholdsModule::configure(parameters);

    //check and set
    if (parameters.isAvailable(ActiveFlavorsThresholdsVariable::PARAM_NAME_THRESHOLDS)) {
        setThresholds(parameters.getLastAvailable().toVectorDouble());
        std::ostringstream oss;
        std::copy(m_thresholds.begin(), m_thresholds.end(), std::ostream_iterator<double>(oss, " "));
        info(__func__, ElemUtils::Formatter() << ActiveFlavorsThresholdsVariable::PARAM_NAME_THRESHOLDS << " configured with value = [ " << oss.str() << "] GeV");
    }
}

void ActiveFlavorsThresholdsVariable::resolveObjectDependencies() {
    ActiveFlavorsThresholdsModule::resolveObjectDependencies();
}

void ActiveFlavorsThresholdsVariable::initModule() {
    ActiveFlavorsThresholdsModule::initModule();
}

void ActiveFlavorsThresholdsVariable::isModuleWellConfigured() {
    ActiveFlavorsThresholdsModule::isModuleWellConfigured();
    //check that the vector of thresholds is not empty
    if (m_thresholds.size() == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "The vector of thresholds is empty");
    }
    //check if the vector of thresholds is ordered
    if (!std::is_sorted(m_thresholds.begin(), m_thresholds.end())) {
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "The vector of thresholds is not ordered");
    }
}

void ActiveFlavorsThresholdsVariable::setThresholds(std::vector<double> thresholds) {
    m_thresholds = thresholds;
    reset();
    for (int i = 1; i < (int) thresholds.size(); i++)
      addNfInterval(i, std::max(pow(thresholds[i-1], 2), i*1e-6), std::max(pow(thresholds[i], 2), (i+1)*1e-6));
    addNfInterval(thresholds.size(), pow(thresholds.back(), 2), 1e12);
}

std::vector<double> ActiveFlavorsThresholdsVariable::getThresholds() const {
    // Now check if the module is well-configured
    return m_thresholds;
}

} /* namespace PARTONS */
