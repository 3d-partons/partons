#include "DoubleDistributionKinematic.h"

DoubleDistributionKinematic::DoubleDistributionKinematic() :
        m_beta(0.), m_alpha(0.), m_t(0.), m_MuF2(0.), m_MuR2(0.), m_doubleDistributionType(
                DoubleDistributionType::UNDEFINED) {
}

DoubleDistributionKinematic::DoubleDistributionKinematic(double beta,
        double alpha, double t, double MuF2, double MuR2,
        DoubleDistributionType::Type dualDistributionType) :
        m_beta(beta), m_alpha(alpha), m_t(t), m_MuF2(MuF2), m_MuR2(MuR2), m_doubleDistributionType(
                dualDistributionType) {

}

DoubleDistributionKinematic::~DoubleDistributionKinematic() {
    // TODO Auto-generated destructor stub
}

double DoubleDistributionKinematic::getAlpha() const {
    return m_alpha;
}

void DoubleDistributionKinematic::setAlpha(double alpha) {
    m_alpha = alpha;
}

double DoubleDistributionKinematic::getBeta() const {
    return m_beta;
}

void DoubleDistributionKinematic::setBeta(double beta) {
    m_beta = beta;
}

double DoubleDistributionKinematic::getMuF2() const {
    return m_MuF2;
}

void DoubleDistributionKinematic::setMuF2(double muF2) {
    m_MuF2 = muF2;
}

double DoubleDistributionKinematic::getMuR2() const {
    return m_MuR2;
}

void DoubleDistributionKinematic::setMuR2(double muR2) {
    m_MuR2 = muR2;
}

double DoubleDistributionKinematic::getT() const {
    return m_t;
}

void DoubleDistributionKinematic::setT(double t) {
    m_t = t;
}

const DoubleDistributionType& DoubleDistributionKinematic::getDoubleDistributionType() const {
    return m_doubleDistributionType;
}

void DoubleDistributionKinematic::setDoubleDistributionType(
        const DoubleDistributionType& doubleDistributionType) {
    m_doubleDistributionType = doubleDistributionType;
}
