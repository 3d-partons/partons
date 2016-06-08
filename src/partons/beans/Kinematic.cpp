#include "../../../include/partons/beans/Kinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

Kinematic::Kinematic() :
        BaseObject("Kinematic"), m_id(-1), m_hashSum(
                ElemUtils::StringUtils::EMPTY) {
}

Kinematic::Kinematic(const std::string &className) :
        BaseObject(className), m_id(-1), m_hashSum(
                ElemUtils::StringUtils::EMPTY) {
}

Kinematic::~Kinematic() {
}

int Kinematic::getId() const {
    return m_id;
}

bool Kinematic::operator <(const Kinematic& other) const {
    return (m_id < other.m_id);
}

void Kinematic::setId(int id) {
    m_id = id;
}

std::string Kinematic::toString() const {
    return ElemUtils::Formatter() << "m_id = " << m_id;
}

const std::string& Kinematic::getHashSum() const {
    if (m_hashSum.empty()) {
        updateHashSum();
    }
    return m_hashSum;
}

void Kinematic::setHashSum(const std::string& hashSum) const {
    m_hashSum = hashSum;
}
