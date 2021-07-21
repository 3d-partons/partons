#include "../../../../include/partons/beans/gpd/GPDType.h"

#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>
#include <vector>

namespace PARTONS {

GPDType::GPDType() :
        m_type(GPDType::UNDEFINED) {
}

GPDType::GPDType(Type type) :
        m_type(type) {
}

GPDType::GPDType(const GPDType &other) {
    m_type = other.m_type;
}

GPDType::operator GPDType::Type() const {
    return m_type;
}

std::string GPDType::toString() const {

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

    case HTrans:
        return "HTrans";
        break;
    case HtTrans:
        return "HtTrans";
        break;
    case ETrans:
        return "ETrans";
        break;
    case EtTrans:
        return "EtTrans";
        break;

    case H3p:
        return "H3p";
        break;
    case Ht3p:
        return "Ht3p";
        break;
    case E3p:
        return "E3p";
        break;
    case Et3p:
        return "Et3p";
        break;

    case H3m:
        return "H3m";
        break;
    case Ht3m:
        return "Ht3m";
        break;
    case E3m:
        return "E3m";
        break;
    case Et3m:
        return "Et3m";
        break;

    case EbarTrans:
        return "EbarTrans";
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

    if (ElemUtils::StringUtils::equals(gpdTypeStr, "ALL")) {
        gpdType = GPDType::ALL;
    }

    else if (ElemUtils::StringUtils::equals(gpdTypeStr, "H")) {
        gpdType = GPDType::H;
    } else if (ElemUtils::StringUtils::equals(gpdTypeStr, "Ht")) {
        gpdType = GPDType::Ht;
    } else if (ElemUtils::StringUtils::equals(gpdTypeStr, "E")) {
        gpdType = GPDType::E;
    } else if (ElemUtils::StringUtils::equals(gpdTypeStr, "Et")) {
        gpdType = GPDType::Et;
    }

    else if (ElemUtils::StringUtils::equals(gpdTypeStr, "HTrans")) {
        gpdType = GPDType::HTrans;
    } else if (ElemUtils::StringUtils::equals(gpdTypeStr, "HtTrans")) {
        gpdType = GPDType::HtTrans;
    } else if (ElemUtils::StringUtils::equals(gpdTypeStr, "ETrans")) {
        gpdType = GPDType::ETrans;
    } else if (ElemUtils::StringUtils::equals(gpdTypeStr, "EtTrans")) {
        gpdType = GPDType::EtTrans;
    }

    else if (ElemUtils::StringUtils::equals(gpdTypeStr, "H3p")) {
        gpdType = GPDType::H3p;
    } else if (ElemUtils::StringUtils::equals(gpdTypeStr, "Ht3p")) {
        gpdType = GPDType::Ht3p;
    } else if (ElemUtils::StringUtils::equals(gpdTypeStr, "E3p")) {
        gpdType = GPDType::E3p;
    } else if (ElemUtils::StringUtils::equals(gpdTypeStr, "Et3p")) {
        gpdType = GPDType::Et3p;
    }

    else if (ElemUtils::StringUtils::equals(gpdTypeStr, "H3m")) {
        gpdType = GPDType::H3m;
    } else if (ElemUtils::StringUtils::equals(gpdTypeStr, "Ht3m")) {
        gpdType = GPDType::Ht3m;
    } else if (ElemUtils::StringUtils::equals(gpdTypeStr, "E3m")) {
        gpdType = GPDType::E3m;
    } else if (ElemUtils::StringUtils::equals(gpdTypeStr, "Et3m")) {
        gpdType = GPDType::Et3m;
    }

    else if (ElemUtils::StringUtils::equals(gpdTypeStr, "EbarTrans")) {
        gpdType = GPDType::EbarTrans;
    }

    return gpdType;
}

void GPDType::serialize(ElemUtils::Packet &packet) const {
    packet << static_cast<int>(m_type);
}

void GPDType::unserialize(ElemUtils::Packet &packet) {

    int i = 0;
    packet >> i;
    m_type = static_cast<GPDType::Type>(i);
}

bool GPDType::operator <(const GPDType& other) const {
    return (m_type < other.m_type);
}

List<GPDType> GPDType::getListOfGPDTypeFromString(
        const std::string& gpdTypeListAsString) {

    List<GPDType> gpdTypeList;

    std::vector<std::string> vectorOfGPDTypeList =
            ElemUtils::StringUtils::split(gpdTypeListAsString, '|');

    for (unsigned int i = 0; i != vectorOfGPDTypeList.size(); i++) {
        gpdTypeList.add(GPDType::fromString(vectorOfGPDTypeList[i]));
    }

    return gpdTypeList;
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet, GPDType& gpdType) {

    gpdType.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet, GPDType& gpdType) {

    gpdType.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
