#include "../../../../include/partons/beans/observable/ObservableKinematic.h"

namespace PARTONS {

ObservableKinematic::ObservableKinematic(const std::string &className,
        ChannelType::Type channelType) :
        Kinematic(className, channelType) {
}

ObservableKinematic::~ObservableKinematic() {
}

void ObservableKinematic::configure(const ElemUtils::Parameters &parameters) {
    Kinematic::configure(parameters);
}

std::string ObservableKinematic::toString() const {
    return Kinematic::toString();
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

void ObservableKinematic::serializeIntoStdVector(
        std::vector<double>& vec) const {
    Kinematic::serializeIntoStdVector(vec);
}

void ObservableKinematic::unserializeFromStdVector(
        std::vector<double>::const_iterator& it,
        const std::vector<double>::const_iterator& end) {
    Kinematic::unserializeFromStdVector(it, end);
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
