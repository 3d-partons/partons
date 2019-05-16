#include "../../../../include/partons/beans/process/VCSSubProcessType.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

namespace PARTONS {

VCSSubProcessType::VCSSubProcessType() :
        BaseObject("VCSSubProcessType"), m_type(VCSSubProcessType::UNDEFINED) {
}

VCSSubProcessType::VCSSubProcessType(Type type) :
        BaseObject("VCSSubProcessType"), m_type(type) {
}

VCSSubProcessType::VCSSubProcessType(const std::string &subProcessTypeString) :
        BaseObject("VCSSubProcessType"), m_type(VCSSubProcessType::UNDEFINED) {
    if (ElemUtils::StringUtils::equals(subProcessTypeString, "ALL")) {
        m_type = VCSSubProcessType::ALL;
    } else if (ElemUtils::StringUtils::equals(subProcessTypeString, "DVCS")) {
        m_type = VCSSubProcessType::DVCS;
    } else if (ElemUtils::StringUtils::equals(subProcessTypeString, "BH")) {
        m_type = VCSSubProcessType::BH;
    } else if (ElemUtils::StringUtils::equals(subProcessTypeString, "INT")) {
        m_type = VCSSubProcessType::INT;
    } else if (ElemUtils::StringUtils::equals(subProcessTypeString, "TCS")) {
        m_type = VCSSubProcessType::TCS;
    } else if (ElemUtils::StringUtils::equals(subProcessTypeString, "DDVCS")) {
        m_type = VCSSubProcessType::DDVCS;
    } else {
        warn(__func__,
                ElemUtils::Formatter()
                        << "Cannot found VCSSubProcessType for string = "
                        << subProcessTypeString);
    }
}

std::string VCSSubProcessType::toString() const {
    switch (m_type) {
    case ALL:
        return "ALL";
        break;
    case DVCS:
        return "DVCS";
        break;
    case BH:
        return "BH";
        break;
    case INT:
        return "INT";
        break;
    case TCS:
        return "TCS";
        break;
    case DDVCS:
        return "DDVCS";
        break;
    default:
        return "UNDEFINED";
    }
}

VCSSubProcessType::Type VCSSubProcessType::getType() const {
    return m_type;
}

void VCSSubProcessType::setType(Type type) {
    m_type = type;
}

} /* namespace PARTONS */
