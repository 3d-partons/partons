#include "CFFInputData.h"

#include "../../utils/stringUtils/Formatter.h"

CFFInputData::CFFInputData()
        : m_binId(0), m_xB(0.), m_t(0.), m_Q2(0.) {
}

CFFInputData::CFFInputData(const double xB, const double t, const double Q2)
        : m_binId(0), m_xB(xB), m_t(t), m_Q2(Q2) {
}

CFFInputData::CFFInputData(const unsigned int binId, const double xB,
        const double t, const double Q2)
        : m_binId(binId), m_xB(xB), m_t(t), m_Q2(Q2) {
}

CFFInputData::CFFInputData(const CFFInputData &other)
        : GenericData(other) {
    m_binId = other.m_binId;
    m_xB = other.m_xB;
    m_t = other.m_t;
    m_Q2 = other.m_Q2;
}

CFFInputData::~CFFInputData() {

}

std::string CFFInputData::toString() {
    return Formatter() << "[CFFInputData]\n" << "binId = " << m_binId
            << "\nxB = " << m_xB << "\nt = " << m_t << "\nQ2 = " << m_Q2 << "\n";
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
