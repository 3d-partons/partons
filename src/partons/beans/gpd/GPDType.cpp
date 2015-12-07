#include "../../../../include/partons/beans/gpd/GPDType.h"

#include "../../../../include/partons/utils/stringUtils/StringUtils.h"
//#include "../../utils/GenericType.h"
//#include "../../utils/ParameterList.h"

const std::string GPDType::GPD_TYPE_DB_COLUMN_NAME = "gpd_type";

GPDType::GPDType() :
        m_type(GPDType::UNDEFINED) {
}

GPDType::GPDType(Type type) :
        m_type(type) {
}

/**
 * Copy constructor
 * @param other
 */
GPDType::GPDType(const GPDType &other) {
    m_type = other.m_type;
}

////TODO replace hardcoded GPDType
//GPDType::GPDType(const ParameterList& parameters) :
//        m_type(GPDType::fromString(parameters.get("GPDType").toString())) {
//}

GPDType::operator GPDType::Type() const {
    return m_type;
}

std::string GPDType::toString() {
    switch (m_type) {
    case H:
        return "H";
        break;
    case Ht:
        return "Ht";
        break;
    case E:
        return "E";
        break;
    case Et:
        return "Et";
        break;
    default:
        return "UNDEFINED";
    }
}

GPDType::Type GPDType::getType() const {
    return m_type;
}

void GPDType::setType(Type type) {
    m_type = type;
}

GPDType::Type GPDType::fromString(const std::string& gpdTypeStr) {
    GPDType::Type gpdType = GPDType::UNDEFINED;

    if (StringUtils::equals(gpdTypeStr, "H")) {
        gpdType = GPDType::H;
    } else if (StringUtils::equals(gpdTypeStr, "Ht")) {
        gpdType = GPDType::Ht;
    } else if (StringUtils::equals(gpdTypeStr, "E")) {
        gpdType = GPDType::E;
    } else if (StringUtils::equals(gpdTypeStr, "Et")) {
        gpdType = GPDType::Et;
    }

    return gpdType;
}
