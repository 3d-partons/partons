#include "../../../../include/partons/beans/dse/QPbyChebyshev.h"

#include <cmath>

QPbyChebyshev::QPbyChebyshev(unsigned int N, double m, double mu,
        double Lambda2, double epsilon2) :
        QuarkPropagator("QPbyChebyshev", N, m, mu, Lambda2, epsilon2), m_change(
                false) {
    if (N > 0) {
        m_a.assign(N, 0.);
        m_b.assign(N, 0.);
    }
    m_xmu = stox(m_mu * m_mu);
    m_p2 = -2., m_x = -2., m_SigmaAp2 = 0., m_SigmaAmu = 0., m_SigmaMp2 = 0., m_SigmaMmu =
            0., m_Ap2 = 0., m_Bp2 = 0.;
}

QPbyChebyshev::~QPbyChebyshev() {
    // TODO Auto-generated destructor stub
}
const std::vector<double>& QPbyChebyshev::getCoeffsA() const {
    return m_a;
}

void QPbyChebyshev::setCoeffsA(const std::vector<double>& a) {
    m_a = a;
    m_change = true;
}

const double QPbyChebyshev::getCoeffA(unsigned int i) {
    return m_a.at(i);
}

void QPbyChebyshev::setCoeffA(unsigned int i, double a) {
    m_a[i] = a;
    m_change = true;
}

const std::vector<double>& QPbyChebyshev::getCoeffsB() const {
    return m_b;
}

void QPbyChebyshev::setCoeffsB(const std::vector<double>& b) {
    m_b = b;
    m_change = true;
}

const double QPbyChebyshev::getCoeffB(unsigned int i) {
    return m_b.at(i);
}

void QPbyChebyshev::setCoeffB(unsigned int i, double b) {
    m_b[i] = b;
    m_change = true;
}

void QPbyChebyshev::setMu(double mu) {
    QuarkPropagator::setMu(mu);
    m_xmu = stox(m_mu * m_mu);
    m_change = true;
}

double QPbyChebyshev::evaluateA(double p2) {
    updateSigmaAM(p2);
    return 1. + m_SigmaAp2 - m_SigmaAmu;
}

double QPbyChebyshev::evaluateB(double p2) {
    updateSigmaAM(p2);
    return m_m + m_SigmaMp2 - m_SigmaMmu;
}

void QPbyChebyshev::updateSigmaAM(double p2) {
    bool changep2 = false;
    if (p2 != m_p2) {
        m_x = stox(p2);
        m_p2 = p2;
        changep2 = true;
    };
    if (m_change) {
        m_SigmaAmu = computeSigmaA(m_xmu);
        m_SigmaMmu = computeSigmaM(m_xmu);
    };
    if (m_change || changep2) {
        m_SigmaAp2 = computeSigmaA(m_x);
        m_SigmaMp2 = computeSigmaM(m_x);
    };
    m_change = false;
}

double QPbyChebyshev::evaluateSigmaA(double p2) {
    updateSigmaAM(p2);
    return m_SigmaAp2;
}

double QPbyChebyshev::evaluateSigmaM(double p2) {
    updateSigmaAM(p2);
    return m_SigmaMp2;
}

double QPbyChebyshev::computeSigmaA(double x) {
    double SigmaA = (m_a.at(0)) / 2.;
    for (unsigned int i = 1; i < m_N; i++) {
        SigmaA += (m_a.at(i)) * T(i, x);
    };
    return SigmaA;
}

double QPbyChebyshev::computeSigmaM(double x) {
    double SigmaM = (m_b.at(0)) / 2.;
    for (unsigned int i = 1; i < m_N; i++) {
        SigmaM += (m_b.at(i)) * T(i, x);
    };
    return SigmaM;
}

double QPbyChebyshev::T(unsigned int n, double x) {
    return cos(n * acos(x));
}

double QPbyChebyshev::stox(double p2) {
    return log(p2 / (m_Lambda * m_epsilon)) / log(m_Lambda / m_epsilon);
}

double QPbyChebyshev::xtos(double x) {
    return m_Lambda * m_epsilon * exp(x * log(m_Lambda / m_epsilon));
}
