#include "ServiceFunctionNames.h"

#include "../utils/stringUtils/StringUtils.h"

ServiceFunctionNames::ServiceFunctionNames()
        : m_type(ServiceFunctionNames::UNDEFINED) {
}

ServiceFunctionNames::ServiceFunctionNames(Type type)
        : m_type(type) {
}

ServiceFunctionNames::ServiceFunctionNames(const std::string &string)
        : m_type(fromString(string)) {

}

ServiceFunctionNames::ServiceFunctionNames(const ServiceFunctionNames &other) {
    m_type = other.m_type;
}

ServiceFunctionNames::operator ServiceFunctionNames::Type() const {
    return m_type;
}

ServiceFunctionNames::Type ServiceFunctionNames::fromString(
        const std::string &string) {
    ServiceFunctionNames::Type returnType = ServiceFunctionNames::UNDEFINED;

    if (StringUtils::equals(string, "computeGPDModel")) {
        returnType = ServiceFunctionNames::GPD_SERVICE_COMPUTE_GPD_MODEL;
    } else if (StringUtils::equals(string,
            "computeGPDModelRestrictedByGPDType")) {
        returnType =
                ServiceFunctionNames::GPD_SERVICE_COMPUTE_GPD_MODEL_RESTRICTED_BY_GPD_TYPE;
    } else if (StringUtils::equals(string, "computeGPDModelWithEvolution")) {
        returnType =
                ServiceFunctionNames::GPD_SERVICE_COMPUTE_GPD_MODEL_WITH_EVOLUTION;
    } else {
        //TODO print in log an error for input string
    }

    return ServiceFunctionNames(returnType);
}

std::string ServiceFunctionNames::toString() {
    switch (m_type) {
    case ServiceFunctionNames::GPD_SERVICE_COMPUTE_GPD_MODEL:
        return "H";
        break;
    case ServiceFunctionNames::GPD_SERVICE_COMPUTE_GPD_MODEL_RESTRICTED_BY_GPD_TYPE:
        return "Ht";
        break;
    case ServiceFunctionNames::GPD_SERVICE_COMPUTE_GPD_MODEL_WITH_EVOLUTION:
        return "E";
        break;
    default:
        return "UNDEFINED";
    }
}

ServiceFunctionNames::Type ServiceFunctionNames::getType() const {
    return m_type;
}

void ServiceFunctionNames::setType(Type type) {
    m_type = type;
}
