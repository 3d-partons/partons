#include "../../../../include/partons/beans/process/DVCSSubProcessType.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

namespace PARTONS {

const std::string DVCSSubProcessType::PARAMETER_NAME_DVCS_SUB_PROCESS_TYPE =
        "dvcs_subprocess_type";

DVCSSubProcessType::DVCSSubProcessType() :
        BaseObject("DVCSSubProcessType"), m_type(DVCSSubProcessType::UNDEFINED) {
}

DVCSSubProcessType::DVCSSubProcessType(Type type) :
        BaseObject("DVCSSubProcessType"), m_type(type) {
}

DVCSSubProcessType::DVCSSubProcessType(
        const std::string &dvcsSubProcessTypeString) :
        BaseObject("DVCSSubProcessType"), m_type(DVCSSubProcessType::UNDEFINED) {
    if (ElemUtils::StringUtils::equals(dvcsSubProcessTypeString, "ALL")) {
        m_type = DVCSSubProcessType::ALL;
    } else if (ElemUtils::StringUtils::equals(dvcsSubProcessTypeString,
            "DVCS")) {
        m_type = DVCSSubProcessType::DVCS;
    } else if (ElemUtils::StringUtils::equals(dvcsSubProcessTypeString, "BH")) {
        m_type = DVCSSubProcessType::BH;
    } else if (ElemUtils::StringUtils::equals(dvcsSubProcessTypeString,
            "INT")) {
        m_type = DVCSSubProcessType::INT;
    } else {
        warn(__func__,
                ElemUtils::Formatter()
                        << "Cannot found DVCSSubProcessType for string = "
                        << dvcsSubProcessTypeString);
    }
}

std::string DVCSSubProcessType::toString() const {
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
    default:
        return "UNDEFINED";
    }
}

DVCSSubProcessType::Type DVCSSubProcessType::getType() const {
    return m_type;
}

void DVCSSubProcessType::setType(Type type) {
    m_type = type;
}

} /* namespace PARTONS */
