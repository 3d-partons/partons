#include "../../../include/partons/beans/Kinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

Kinematic::Kinematic() :
        BaseObject("Kinematic"), m_id(-1) {
}

Kinematic::Kinematic(const std::string &className) :
        BaseObject(className), m_id(-1) {
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
