#include "Kinematic.h"

#include "../../utils/stringUtils/Formatter.h"

Kinematic::Kinematic(double xB, double t)
        : m_xB(xB), m_t(t) {
}

Kinematic::~Kinematic() {
}

std::string Kinematic::toString() {
    return Formatter() << "m_xB = " << m_xB << " m_t = " << m_t;
}

// ##### GETTERS & SETTERS #####

double Kinematic::getT() const {
    return m_t;
}

void Kinematic::setT(double t) {
    m_t = t;
}

double Kinematic::getXB() const {
    return m_xB;
}

void Kinematic::setXB(double xB) {
    m_xB = xB;
}
