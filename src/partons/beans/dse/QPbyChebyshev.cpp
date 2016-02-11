#include "../../../../include/partons/beans/dse/QPbyChebyshev.h"

#include <cmath>
#include <NumA/linear_algebra/vector/VectorD.h>

#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"

QPbyChebyshev::QPbyChebyshev(unsigned int N, double m, double mu,
        double Lambda2, double epsilon2) :
        QuarkPropagator("QPbyChebyshev", N, m, mu, Lambda2, epsilon2) {
    if (N > 0) {
        m_a = NumA::VectorD(N);
        m_b = NumA::VectorD(N);
        m_roots.assign(N, 0.);
        for (int k = 0; k < N; k++) {
            m_roots.at(k) = cos((N - k - 0.5) * PI / N);
        }
    }
}

QPbyChebyshev* QPbyChebyshev::clone() const {
    return new QPbyChebyshev(*this);
}

QPbyChebyshev::QPbyChebyshev(const QPbyChebyshev& other) :
        QuarkPropagator(other) {
    m_a = other.getCoeffsA();
    m_b = other.getCoeffsB();
    m_roots = other.getRoots();
}

QPbyChebyshev::~QPbyChebyshev() {
// TODO Auto-generated destructor stub
}

void QPbyChebyshev::setN(unsigned int n) {
    QuarkPropagator::setN(n);

    for (int k = 0; k < n; k++) {
        m_roots.at(k) = cos((n - k - 0.5) * PI / n); //TODO Move to NumA
    }
}

void QPbyChebyshev::setCoeffsAfromValueOnNodes(
        const std::vector<double>& values) {
    m_a.assign(m_N,0.);
    double stored_T;
    for (unsigned int i = 0; i < m_N; i++) {
        for (unsigned int k = 0; k < m_N; k++) {
            stored_T = cos(i * (m_N - k - 0.5) * PI / m_N);
            m_a.at(i) += stored_T * values.at(k);
        }
        m_a.at(i) *= 2. / m_N;
    }
}

void QPbyChebyshev::setCoeffsBfromValueOnNodes(
        const std::vector<double>& values) {
    m_b.assign(m_N,0.);
    double stored_T;
    for (unsigned int i = 0; i < m_N; i++) {
        for (unsigned int k = 0; k < m_N; k++) {
            stored_T = cos(i * (m_N - k - 0.5) * PI / m_N);
            m_b.at(i) += stored_T * values.at(k);
        }
        m_b.at(i) *= 2. / m_N;
    }
}

double QPbyChebyshev::evaluateA(double p2) const {
    double A = 1.;
    double x = stox(p2);
    double xmu = stox(m_mu * m_mu);
    for (unsigned int i = 1; i < m_a.size(); i++) {
        A += (m_a.at(i)) * (T(i, x) - T(i, xmu));
    };

    return A;
}

double QPbyChebyshev::evaluateB(double p2) const {
    double B = m_m;
    double x = stox(p2);
    double xmu = stox(m_mu * m_mu);
    for (unsigned int i = 1; i < m_b.size(); i++) {
        B += (m_b.at(i)) * (T(i, x) - T(i, xmu));
    };

    return B;
}

double QPbyChebyshev::evaluateSigmaA(double p2) const {
    double SigmaA = (m_a.at(0)) / 2.;
    double x = stox(p2);
    for (unsigned int i = 1; i < m_a.size(); i++) {
        SigmaA += (m_a.at(i)) * T(i, x);
    };
    return SigmaA;
}

double QPbyChebyshev::evaluateSigmaM(double p2) const {
    double SigmaM = (m_b.at(0)) / 2.;
    double x = stox(p2);
    for (unsigned int i = 1; i < m_b.size(); i++) {
        SigmaM += (m_b.at(i)) * T(i, x);
    };
    return SigmaM;
}

double QPbyChebyshev::evaluateSigmaV(double p2) const {
    double A = evaluateA(p2);
    double B = evaluateB(p2);
    return A / (p2 * A * A + B * B);
}

double QPbyChebyshev::evaluateSigmaS(double p2) const {
    double A = evaluateA(p2);
    double B = evaluateB(p2);
    return B / (p2 * A * A + B * B);
}

double QPbyChebyshev::differentiateA(double p2, unsigned int j) const {
    return T(j, stox(p2)) - T(j, stox(m_mu * m_mu));
}

double QPbyChebyshev::differentiateB(double p2, unsigned int j) const {
    return T(j, stox(p2)) - T(j, stox(m_mu * m_mu));
}

double QPbyChebyshev::differentiateSigmaA(double p2, unsigned int j) const {
    return T(j, stox(p2)) - 0.5 * (j == 0);
}

double QPbyChebyshev::differentiateSigmaM(double p2, unsigned int j) const {
    return T(j, stox(p2)) - 0.5 * (j == 0);
}

double QPbyChebyshev::differentiateSigmaV_a(double p2, unsigned int j) const {
    double A = evaluateA(p2);
    double B = evaluateB(p2);
    double Aprime = differentiateA(p2, j);
    double A2 = A * A;
    double B2 = B * B;
    double denom = p2 * A2 + B2;

    return Aprime * (B2 - p2 * A2) / (denom * denom);
}

double QPbyChebyshev::differentiateSigmaV_b(double p2, unsigned int j) const {
    double A = evaluateA(p2);
    double B = evaluateB(p2);
    double Bprime = differentiateB(p2, j);
    double denom = p2 * A * A + B * B;

    return -2. * Bprime * A * B / (denom * denom);
}

double QPbyChebyshev::differentiateSigmaS_a(double p2, unsigned int j) const {
    double A = evaluateA(p2);
    double B = evaluateB(p2);
    double Aprime = differentiateA(p2, j);
    double denom = p2 * A * A + B * B;

    return -2. * p2 * Aprime * A * B / (denom * denom);
}

double QPbyChebyshev::differentiateSigmaS_b(double p2, unsigned int j) const {
    double A = evaluateA(p2);
    double B = evaluateB(p2);
    double Bprime = differentiateB(p2, j);
    double A2 = A * A;
    double B2 = B * B;
    double denom = p2 * A2 + B2;

    return Bprime * (p2 * A2 - B2) / (denom * denom);
}

double QPbyChebyshev::T(unsigned int n, double x) const {
    return cos(n * acos(x)); //TODO Move to NumA
}

double QPbyChebyshev::stox(double p2) const {
    return log(p2 / (m_Lambda * m_epsilon)) / log(m_Lambda / m_epsilon);
}

double QPbyChebyshev::xtos(double x) const {
    return m_Lambda * m_epsilon * exp(x * log(m_Lambda / m_epsilon));
}

std::string QPbyChebyshev::toString() const {
    Formatter formatter;
    formatter << "Coeffs a : ";
    for (unsigned int i = 0; i < m_a.size(); i++) {
        formatter << m_a.at(i) << " , ";
    }
    formatter << "\n";
    formatter << "Coeffs b : ";
    for (unsigned int i = 0; i < m_b.size(); i++) {
        formatter << m_b.at(i) << " , ";
    }
    formatter << "\n";
    return formatter;
}
