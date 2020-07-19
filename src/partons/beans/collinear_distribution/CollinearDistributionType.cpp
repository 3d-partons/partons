#include "../../../../include/partons/beans/collinear_distribution/CollinearDistributionType.h"

#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>
#include <vector>

namespace PARTONS {


const std::string CollinearDistributionType::COLLINEAR_DISTRIBUTION_TYPE_DB_COLUMN_NAME = "collinear_distribution_type";

CollinearDistributionType::CollinearDistributionType() :
        m_type(CollinearDistributionType::UNDEFINED) {
}

CollinearDistributionType::CollinearDistributionType(Type type) :
        m_type(type) {
}

/**
 * Copy constructor
 * @param other
 */
CollinearDistributionType::CollinearDistributionType(const CollinearDistributionType &other) {
    m_type = other.m_type;
}

////TODO replace hardcoded CollinearDistributionType
//CollinearDistributionType::CollinearDistributionType(const ParameterList& parameters) :
//        m_type(CollinearDistributionType::fromString(parameters.get("CollinearDistributionType").toString())) {
//}

CollinearDistributionType::operator CollinearDistributionType::Type() const {
    return m_type;
}

std::string CollinearDistributionType::toString() const {
    switch (m_type) {
    case UnpolPDF:
        return "UnpolPDF";
        break;
    case PolPDF:
        return "PolPDF";
        break;
    case TransPDF:
        return "TransPDF";
        break;
    case UnpolFF:
        return "UnpolFF";
        break;
    case PolFF:
        return "PolFF";
        break;
    case TransFF:
        return "TransFF";
        break;
    default:
        return "UNDEFINED";
    }
}

CollinearDistributionType::Type CollinearDistributionType::getType() const {
    return m_type;
}

void CollinearDistributionType::setType(Type type) {
    m_type = type;
}

CollinearDistributionType::Type CollinearDistributionType::fromString(const std::string& colldistTypeStr) {
    CollinearDistributionType::Type colldistType = CollinearDistributionType::UNDEFINED;

    if (ElemUtils::StringUtils::equals(colldistTypeStr, "ALL")) {
        colldistType = CollinearDistributionType::ALL;
    } else if (ElemUtils::StringUtils::equals(colldistTypeStr, "UnpolPDF")) {
        colldistType = CollinearDistributionType::UnpolPDF;
    } else if (ElemUtils::StringUtils::equals(colldistTypeStr, "PolPDF")) {
        colldistType = CollinearDistributionType::PolPDF;
    } else if (ElemUtils::StringUtils::equals(colldistTypeStr, "TransPDF")) {
        colldistType = CollinearDistributionType::TransPDF;
    } else if (ElemUtils::StringUtils::equals(colldistTypeStr, "UnpolFF")) {
        colldistType = CollinearDistributionType::UnpolFF;
    } else if (ElemUtils::StringUtils::equals(colldistTypeStr, "PolFF")) {
        colldistType = CollinearDistributionType::PolFF;
    } else if (ElemUtils::StringUtils::equals(colldistTypeStr, "TransFF")) {
        colldistType = CollinearDistributionType::TransFF;
    }

    //TODO add missing stuff

    return colldistType;
}

void CollinearDistributionType::serialize(ElemUtils::Packet &packet) const {
    packet << static_cast<int>(m_type);
}

void CollinearDistributionType::unserialize(ElemUtils::Packet &packet) {
    int i = 0;
    packet >> i;
    m_type = static_cast<CollinearDistributionType::Type>(i);
}

bool CollinearDistributionType::operator <(const CollinearDistributionType& other) const {
    return (m_type < other.m_type);
}

List<CollinearDistributionType> CollinearDistributionType::getListOfCollinearDistributionTypeFromString(
        const std::string& colldistTypeListAsString) {
    List<CollinearDistributionType> colldistTypeList;

    std::vector<std::string> vectorOfCollinearDistributionTypeList =
            ElemUtils::StringUtils::split(colldistTypeListAsString, '|');

    for (unsigned int i = 0; i != vectorOfCollinearDistributionTypeList.size(); i++) {
        colldistTypeList.add(CollinearDistributionType::fromString(vectorOfCollinearDistributionTypeList[i]));
    }

    return colldistTypeList;
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet, CollinearDistributionType& colldistType) {

    colldistType.serialize(packet);
    return packet;
}
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet, CollinearDistributionType& colldistType) {

    colldistType.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
