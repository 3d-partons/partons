#include "../../../../include/partons/beans/observable/ObservableKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/utils/type/PhysicalType.h"

namespace PARTONS {

ObservableKinematic::ObservableKinematic(const std::string &className,
        ChannelType::Type channelType) :
        Kinematic(className, channelType) {
}

ObservableKinematic::ObservableKinematic(const std::string &className,
        ChannelType::Type channelType, const ElemUtils::Parameters &parameters) :
        Kinematic(className, channelType) {
}

ObservableKinematic::~ObservableKinematic() {
}

std::string ObservableKinematic::toString() const {
    ElemUtils::Formatter formatter;
    formatter << Kinematic::toString() << " m_xB = " << m_xB << " m_t = " << m_t
            << " (GeV2) m_Q2 = " << m_Q2 << " (GeV2) m_E = " << m_E << " (GeV)";

    if (m_phi.isInitialized()) {
        formatter << " phi = " << m_phi.toString();
    }

    return formatter.str();
}

ObservableKinematic::ObservableKinematic(const ObservableKinematic &other) :
        Kinematic(other) {

}

void ObservableKinematic::serialize(ElemUtils::Packet &packet) const {
    Kinematic::serialize(packet);
}

void ObservableKinematic::unserialize(ElemUtils::Packet &packet) {
    Kinematic::unserialize(packet);
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        ObservableKinematic& observableKinematic) {

    observableKinematic.serialize(packet);
    return packet;
}
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        ObservableKinematic& observableKinematic) {

    observableKinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
