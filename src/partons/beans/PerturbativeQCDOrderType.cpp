#include "../../../include/partons/beans/PerturbativeQCDOrderType.h"

#include "../../../include/partons/utils/stringUtils/StringUtils.h"

const std::string PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE =
        "qcd_order_type";

PerturbativeQCDOrderType::PerturbativeQCDOrderType() :
        m_type(PerturbativeQCDOrderType::UNDEFINED) {
}

PerturbativeQCDOrderType::PerturbativeQCDOrderType(Type type) :
        m_type(type) {
}

const std::string PerturbativeQCDOrderType::toString() {
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

PerturbativeQCDOrderType::Type PerturbativeQCDOrderType::fromString(
        const std::string& gpdTypeStr) {
    PerturbativeQCDOrderType::Type gpdType = PerturbativeQCDOrderType::UNDEFINED;

    if (StringUtils::equals(gpdTypeStr, "LO")) {
        gpdType = PerturbativeQCDOrderType::LO;
    } else if (StringUtils::equals(gpdTypeStr, "NLO")) {
        gpdType = PerturbativeQCDOrderType::NLO;
    } else if (StringUtils::equals(gpdTypeStr, "LL")) {
        gpdType = PerturbativeQCDOrderType::LL;
    } else if (StringUtils::equals(gpdTypeStr, "NLL")) {
        gpdType = PerturbativeQCDOrderType::NLL;
    }

    return gpdType;
}
