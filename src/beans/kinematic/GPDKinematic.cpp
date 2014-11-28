#include "GPDKinematic.h"

GPDKinematic::GPDKinematic(double x, double xB, double t, double MuF,
        double MuR)
        : Kinematic(xB, t), m_x(x), m_MuF(MuF), m_MuR(MuR) {
}

GPDKinematic::~GPDKinematic() {
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
