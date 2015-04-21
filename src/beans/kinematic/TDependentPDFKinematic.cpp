#include "TDependentPDFKinematic.h"

#include "../../utils/stringUtils/Formatter.h"

TDependentPDFKinematic::TDependentPDFKinematic()
: m_x(0.), m_t(0.), m_MuF(0.), m_MuR(0.) {
}

TDependentPDFKinematic::TDependentPDFKinematic(double x, double t, double MuF,
		double MuR)
: m_x(x), m_t(t), m_MuF(MuF), m_MuR(MuR) {
}

TDependentPDFKinematic::~TDependentPDFKinematic() {
}

std::string TDependentPDFKinematic::toString() {
    return Formatter() << "m_x = " << m_x << "m_t = " << m_t
            << " m_MuF = " << m_MuF << " m_MuR = " << m_MuR;
}

double TDependentPDFKinematic::getMuF() const {
	return m_MuF;
}

void TDependentPDFKinematic::setMuF(double muF) {
	m_MuF = muF;
}

double TDependentPDFKinematic::getMuR() const {
	return m_MuR;
}

void TDependentPDFKinematic::setMuR(double muR) {
	m_MuR = muR;
}

double TDependentPDFKinematic::getT() const {
	return m_t;
}

void TDependentPDFKinematic::setT(double t) {
	m_t = t;
}

double TDependentPDFKinematic::getX() const {
	return m_x;
}

void TDependentPDFKinematic::setX(double x) {
	m_x = x;
}
