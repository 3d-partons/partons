#include "GPDInputData.h"

#include <sstream>

GPDInputData::GPDInputData(double _x, double _xi, double _t, double _MuF,
        double _MuR)
        : m_x(_x), m_xi(_xi), m_t(_t), m_MuF(_MuF), m_MuR(_MuR) {
}

std::string GPDInputData::toString() {
    std::ostringstream os;

    os << "#[GPDInputData]" << std::endl;
    os << "x = " << m_x << std::endl;
    os << "xi = " << m_xi << std::endl;
    os << "t = " << m_t << " GeV2" << std::endl;
    os << "MuF = " << m_MuF << " GeV" << std::endl;
    os << "MuR = " << m_MuR << " GeV" << std::endl;
    os << std::endl;

    return os.str();
}

double GPDInputData::getMuF() const {
    return m_MuF;
}

double GPDInputData::getMuR() const {
    return m_MuR;
}

double GPDInputData::getT() const {
    return m_t;
}

double GPDInputData::getX() const {
    return m_x;
}

double GPDInputData::getXi() const {
    return m_xi;
}

void GPDInputData::setMuF(double muF) {
    m_MuF = muF;
}

void GPDInputData::setMuR(double muR) {
    m_MuR = muR;
}

void GPDInputData::setT(double t) {
    this->m_t = t;
}

void GPDInputData::setX(double x) {
    this->m_x = x;
}

void GPDInputData::setXi(double xi) {
    this->m_xi = xi;
}
