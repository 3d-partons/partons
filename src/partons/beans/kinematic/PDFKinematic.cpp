#include "../../../../include/partons/beans/kinematic/PDFKinematic.h"

#include "../../../../include/partons/utils/stringUtils/Formatter.h"

PDFKinematic::PDFKinematic() :
        m_x(0.), m_MuF(0.), m_MuR(0.) {
}

PDFKinematic::PDFKinematic(double x, double MuF, double MuR) :
        m_x(x), m_MuF(MuF), m_MuR(MuR) {
}

PDFKinematic::~PDFKinematic() {
}

std::string PDFKinematic::toString() {
    return Formatter() << "m_x = " << m_x << " m_MuF = " << m_MuF << " m_MuR = "
            << m_MuR;
}

double PDFKinematic::getMuF() const {
    return m_MuF;
}

void PDFKinematic::setMuF(double muF) {
    m_MuF = muF;
}

double PDFKinematic::getMuR() const {
    return m_MuR;
}

void PDFKinematic::setMuR(double muR) {
    m_MuR = muR;
}

double PDFKinematic::getX() const {
    return m_x;
}

void PDFKinematic::setX(double x) {
    m_x = x;
}
