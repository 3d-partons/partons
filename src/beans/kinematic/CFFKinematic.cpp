#include "CFFKinematic.h"

#include "../../utils/stringUtils/Formatter.h"

CFFKinematic::CFFKinematic()
        : m_binId(0), m_xi(0.), m_t(0.), m_Q2(0.), m_MuF(0.), m_MuR(0.) {
}

CFFKinematic::CFFKinematic(double xi, double t, double Q2, double MuF, double MuR)
        : m_binId(0), m_xi(xi), m_t(t), m_Q2(Q2), m_MuF(MuF), m_MuR(MuR) {
}

CFFKinematic::CFFKinematic(unsigned int binId, double xi, double t, double Q2, double MuF, double MuR)
        : m_binId(binId), m_xi(xi), m_t(t), m_Q2(Q2), m_MuF(MuF), m_MuR(MuR) {
}

CFFKinematic::~CFFKinematic() {
}

std::string CFFKinematic::toString() {
    return Formatter() << "m_binId = " << m_binId << "m_xi = " << m_xi
    		<< "m_t = " << m_t << " m_Q2 = " << m_Q2
            << " m_MuF = " << m_MuF << " m_MuR = " << m_MuR;
}

// ##### GETTERS & SETTERS #####

unsigned int CFFKinematic::getBinId() const {
    return m_binId;
}

void CFFKinematic::setBinId(unsigned int binId) {
    m_binId = binId;
}

double CFFKinematic::getQ2() const {
    return m_Q2;
}

void CFFKinematic::setQ2(double q2) {
    m_Q2 = q2;
}

double CFFKinematic::getMuF() const {
	return m_MuF;
}

void CFFKinematic::setMuF(double muF) {
	m_MuF = muF;
}

double CFFKinematic::getMuR() const {
	return m_MuR;
}

void CFFKinematic::setMuR(double muR) {
	m_MuR = muR;
}

double CFFKinematic::getT() const {
	return m_t;
}

void CFFKinematic::setT(double t) {
	m_t = t;
}

double CFFKinematic::getXi() const {
	return m_xi;
}

void CFFKinematic::setXi(double xi) {
	m_xi = xi;
}
