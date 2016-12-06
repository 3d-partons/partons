#include "../../../../include/partons/beans/double_distribution/DDGauge.h"

#include <ElementaryUtils/string_utils/StringUtils.h>

DDGauge::DDGauge() :
        m_type(DDGauge::UNDEFINED) {
}

DDGauge::DDGauge(DDGauge::Type type) {
    m_type = type;
}

DDGauge::DDGauge(const std::string& gaugeStr) {
    m_type = fromString(gaugeStr);
}

DDGauge::~DDGauge() {
}

DDGauge::operator DDGauge::Type() const {
    return m_type;
}

std::string DDGauge::toString() {

    switch (m_type) {
    case BMKS:
        return "BMKS";
        break;
    case Pobylitsa:
        return "Pobylitsa";
        break;
    default:
        return "UNDEFINED";
    }
}

DDGauge::Type DDGauge::fromString(const std::string& gaugeStr) {
    DDGauge::Type gaugeType = DDGauge::UNDEFINED;

    if (ElemUtils::StringUtils::equalsIgnoreCase(gaugeStr, "BMKS")) {
        gaugeType = DDGauge::BMKS;
    } else if (ElemUtils::StringUtils::equalsIgnoreCase(gaugeStr, "Pobylitsa")) {
        gaugeType = DDGauge::Pobylitsa;
    }

    return gaugeType;
}
