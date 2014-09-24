#include "CFFInputData.h"

CFFInputData::CFFInputData(double xB, double t, double Q2) :
        m_binId(0), m_xB(xB), m_t(t), m_Q2(Q2) {
}

CFFInputData::CFFInputData(unsigned int binId, double xB, double t, double Q2) :
        m_binId(binId), m_xB(xB), m_t(t), m_Q2(Q2) {
}

CFFInputData::~CFFInputData() {

}

std::string CFFInputData::toString() {
    //TODO implement
    return "";
}

double CFFInputData::getQ2() const {
    return m_Q2;
}

void CFFInputData::setQ2(double q2) {
    m_Q2 = q2;
}

double CFFInputData::getT() const {
    return m_t;
}

void CFFInputData::setT(double t) {
    m_t = t;
}

double CFFInputData::getXB() const {
    return m_xB;
}

void CFFInputData::setXB(double xB) {
    m_xB = xB;
}

unsigned int CFFInputData::getBinId() const {
    return m_binId;
}

void CFFInputData::setBinId(unsigned int binId) {
    m_binId = binId;
}
