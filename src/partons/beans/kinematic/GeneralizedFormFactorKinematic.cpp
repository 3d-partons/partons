#include "../../../../include/partons/beans/kinematic/GeneralizedFormFactorKinematic.h"

#include "../../../../include/partons/utils/stringUtils/Formatter.h"

GeneralizedFormFactorKinematic::GeneralizedFormFactorKinematic() :
        m_t(0.), m_MuF(0.), m_MuR(0.) {
}

GeneralizedFormFactorKinematic::GeneralizedFormFactorKinematic(double t,
        double MuF, double MuR) :
        m_t(t), m_MuF(MuF), m_MuR(MuR) {
}

GeneralizedFormFactorKinematic::~GeneralizedFormFactorKinematic() {
}

std::string GeneralizedFormFactorKinematic::toString() {
    return Formatter() << "m_t = " << m_t << " m_MuF = " << m_MuF << " m_MuR = "
            << m_MuR;
}

double GeneralizedFormFactorKinematic::getMuF() const {
    return m_MuF;
}

void GeneralizedFormFactorKinematic::setMuF(double muF) {
    m_MuF = muF;
}

double GeneralizedFormFactorKinematic::getMuR() const {
    return m_MuR;
}

void GeneralizedFormFactorKinematic::setMuR(double muR) {
    m_MuR = muR;
}

double GeneralizedFormFactorKinematic::getT() const {
    return m_t;
}

void GeneralizedFormFactorKinematic::setT(double t) {
    m_t = t;
}
