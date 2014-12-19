#include "GPDKinematic.h"

#include "../../utils/stringUtils/Formatter.h"

GPDKinematic::GPDKinematic(double x, double xi, double t, double MuF,
        double MuR)
        : Kinematic(0., t), m_x(x), m_xi(xi), m_MuF(MuF), m_MuR(MuR) {
}

GPDKinematic::~GPDKinematic() {
}

std::string GPDKinematic::toString() {
    return Formatter() << Kinematic::toString() << "m_x = " << m_x << " m_xi = "
            << m_xi << " m_MuF = " << m_MuF << " m_MuR = " << m_MuR;
}

// ##### GETTERS & SETTERS #####

double GPDKinematic::getMuF() const {
    return m_MuF;
}

void GPDKinematic::setMuF(double muF) {
    m_MuF = muF;
}

double GPDKinematic::getMuR() const {
    return m_MuR;
}

void GPDKinematic::setMuR(double muR) {
    m_MuR = muR;
}

double GPDKinematic::getX() const {
    return m_x;
}

void GPDKinematic::setX(double x) {
    m_x = x;
}

double GPDKinematic::getXi() const {
    return m_xi;
}

void GPDKinematic::setXi(double xi) {
    m_xi = xi;
}
