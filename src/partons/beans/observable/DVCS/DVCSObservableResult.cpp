#include "../../../../../include/partons/beans/observable/DVCS/DVCSObservableResult.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

DVCSObservableResult::DVCSObservableResult() :
        ObservableResult("DVCSObservableResult", ChannelType::DVCS) {
}

DVCSObservableResult::DVCSObservableResult(
        const DVCSObservableKinematic& kinematic) :
        ObservableResult("DVCSObservableResult", ChannelType::DVCS) {
    m_kinematic = kinematic;
}

DVCSObservableResult::DVCSObservableResult(const DVCSObservableResult& other) :
        ObservableResult(other) {
    m_kinematic = other.m_kinematic;
}

DVCSObservableResult::~DVCSObservableResult() {
}

std::string DVCSObservableResult::toString() const {

    ElemUtils::Formatter formatter;

    formatter << ObservableResult::toString();
    formatter << "Kinematics: " << m_kinematic.toString();

    return formatter.str();
}

bool DVCSObservableResult::operator <(const DVCSObservableResult& other) const {
    return (m_kinematic < other.m_kinematic);
}

const DVCSObservableKinematic& DVCSObservableResult::getKinematic() const {
    return m_kinematic;
}

void DVCSObservableResult::setKinematic(const DVCSObservableKinematic &kinematic) {
    m_kinematic = kinematic;
}

} /* namespace PARTONS */
