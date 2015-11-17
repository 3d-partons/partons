#include "Kinematic.h"

#include "../utils/stringUtils/Formatter.h"

Kinematic::Kinematic() :
        BaseObject("Kinematic"), m_id(0) {
}

Kinematic::Kinematic(const std::string &className) :
        BaseObject(className), m_id(0) {
}

Kinematic::~Kinematic() {
}

unsigned int Kinematic::getId() const {
    return m_id;
}

bool Kinematic::operator <(const Kinematic& other) const {
    return (m_id < other.m_id);
}

void Kinematic::setId(unsigned int id) {
    m_id = id;
}

std::string Kinematic::toString() const {
    return Formatter() << "m_id = " << m_id;
}
