#include "../../../../include/partons/beans/dse/QPbyChebyshev.h"

#include <cmath>

#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"

QPbyChebyshev::QPbyChebyshev(unsigned int N, double m, double mu,
        double Lambda2, double epsilon2) :
        QuarkPropagator("QPbyChebyshev", N, m, mu, Lambda2, epsilon2) {
    if (N > 0) {
        m_a.assign(N, 0.);
        m_b.assign(N, 0.);
        m_roots.assign(N, 0.);
        for (int k = 0; k < N; k++) {
            m_roots.at(k) = cos((N - k - 0.5) * PI / N);
        }
    }
}

QPbyChebyshev::~QPbyChebyshev() {
// TODO Auto-generated destructor stub
}

void QPbyChebyshev::setN(unsigned int n) {
    QuarkPropagator::setN(n);

    if (n > 0) {
        m_a.resize(n, 0.);
        m_b.resize(n, 0.);
        m_roots.resize(n,0.);
        for (int k = 0; k < n; k++) {
            m_roots.at(k) = cos((n - k - 0.5) * PI / n); //TODO Move to NumA
        }
    }
}

const std::vector<double>& QPbyChebyshev::getCoeffsA() const {
    return m_a;
}

void QPbyChebyshev::setCoeffsA(const std::vector<double>& a) {
    m_a = a;
}

const double QPbyChebyshev::getCoeffA(unsigned int i) {
    return m_a.at(i);
}

void QPbyChebyshev::setCoeffA(unsigned int i, double a) {
    m_a.at(i) = a;
}

const std::vector<double>& QPbyChebyshev::getCoeffsB() const {
    return m_b;
}

void QPbyChebyshev::setCoeffsB(const std::vector<double>& b) {
    m_b = b;
}

const double QPbyChebyshev::getCoeffB(unsigned int i) {
    return m_b.at(i);
}

void QPbyChebyshev::setCoeffB(unsigned int i, double b) {
    m_b.at(i) = b;
}

double QPbyChebyshev::evaluateA(double p2) {
    double A = 1.;
    double x = stox(p2);
    double xmu = stox(m_mu * m_mu);
    for (unsigned int i = 1; i < m_a.size(); i++) {
        A += (m_a.at(i)) * (T(i, x) - T(i, xmu));
    };

    return A;
}

double QPbyChebyshev::evaluateB(double p2) {
    double B = m_m;
    double x = stox(p2);
    double xmu = stox(m_mu * m_mu);
    for (unsigned int i = 1; i < m_b.size(); i++) {
        B += (m_b.at(i)) * (T(i, x) - T(i, xmu));
    };

    return B;
}

double QPbyChebyshev::evaluateSigmaV(double p2) {
    double A = evaluateA(p2);
    double B = evaluateB(p2);
    return A / (p2 * A * A + B * B);
}

double QPbyChebyshev::evaluateSigmaS(double p2) {
    double A = evaluateA(p2);
    double B = evaluateB(p2);
    return B / (p2 * A * A + B * B);
}

double QPbyChebyshev::evaluateSigmaA(double p2) {
    double SigmaA = (m_a.at(0)) / 2.;
    double x = stox(p2);
    for (unsigned int i = 1; i < m_a.size(); i++) {
        SigmaA += (m_a.at(i)) * T(i, x);
    };
    return SigmaA;
}

double QPbyChebyshev::evaluateSigmaM(double p2) {
    double SigmaM = (m_b.at(0)) / 2.;
    double x = stox(p2);
    for (unsigned int i = 1; i < m_b.size(); i++) {
        SigmaM += (m_b.at(i)) * T(i, x);
    };
    return SigmaM;
}

double QPbyChebyshev::T(unsigned int n, double x) {
    return cos(n * acos(x)); //TODO Move to NumA
}

const std::vector<double>& QPbyChebyshev::getRoots() const {
    return m_roots;
}

double QPbyChebyshev::stox(double p2) {
    return log(p2 / (m_Lambda * m_epsilon)) / log(m_Lambda / m_epsilon);
}
double QPbyChebyshev::xtos(double x) {
    return m_Lambda * m_epsilon * exp(x * log(m_Lambda / m_epsilon));
}

std::string QPbyChebyshev::toString() const {
    //TODO Write method
    return Formatter() << "";
}

