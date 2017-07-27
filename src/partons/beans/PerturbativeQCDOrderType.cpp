#include "../../../include/partons/beans/PerturbativeQCDOrderType.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

namespace PARTONS {


const std::string PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE =
        "qcd_order_type";

PerturbativeQCDOrderType::PerturbativeQCDOrderType() :
        BaseObject("PerturbativeQCDOrderType"), m_type(
                PerturbativeQCDOrderType::UNDEFINED) {
}

PerturbativeQCDOrderType::PerturbativeQCDOrderType(Type type) :
        BaseObject("PerturbativeQCDOrderType"), m_type(type) {
}

PerturbativeQCDOrderType::PerturbativeQCDOrderType(
        const std::string &perturbativeQCDOrderTypeString) :
        BaseObject("PerturbativeQCDOrderType"), m_type(
                PerturbativeQCDOrderType::UNDEFINED) {
    if (ElemUtils::StringUtils::equals(perturbativeQCDOrderTypeString, "LO")) {
        m_type = PerturbativeQCDOrderType::LO;
    } else if (ElemUtils::StringUtils::equals(perturbativeQCDOrderTypeString,
            "NLO")) {
        m_type = PerturbativeQCDOrderType::NLO;
    } else if (ElemUtils::StringUtils::equals(perturbativeQCDOrderTypeString,
            "LL")) {
        m_type = PerturbativeQCDOrderType::LL;
    } else if (ElemUtils::StringUtils::equals(perturbativeQCDOrderTypeString,
            "NLL")) {
        m_type = PerturbativeQCDOrderType::NLL;
    } else {
        warn(__func__,
                ElemUtils::Formatter()
                        << "Cannot found PerturbativeQCDOrderType for string = "
                        << perturbativeQCDOrderTypeString);
    }
}

std::string PerturbativeQCDOrderType::toString() const {
    switch (m_type) {
    case LO:
        return "LO";
        break;
    case NLO:
        return "NLO";
        break;
    case NNLO:
        return "NNLO";
        break;
    case LL:
        return "LL";
        break;
    case NLL:
        return "NLL";
        break;
    default:
        return "UNDEFINED";
    }
}

PerturbativeQCDOrderType::Type PerturbativeQCDOrderType::getType() const {
    return m_type;
}

void PerturbativeQCDOrderType::setType(Type type) {
    m_type = type;
}

} /* namespace PARTONS */
