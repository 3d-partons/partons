/**
 * @file QuarkPropagator.cpp
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 22, 2016
 * @version 1.0
 *
 * @class QuarkPropagator
 */

#include "../../../../include/partons/beans/dse/QuarkPropagator.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <algorithm>
#include <cmath>
#include <iterator>

QuarkPropagator::QuarkPropagator(const std::string& className, unsigned int N,
        double m, double mu, double Lambda2, double epsilon2) :
        BaseObject(className), m_N(N), m_m(m), m_mu(mu), m_Lambda2(Lambda2), m_epsilon2(
                epsilon2) {
    m_Lambda = sqrt(Lambda2);
    m_epsilon = sqrt(epsilon2);
    if (N > 0) {
        m_a.assign(N);
        m_b.assign(N);
        m_A.assign(N);
        m_B.assign(N);
        m_roots.assign(N);
    }
}

QuarkPropagator::QuarkPropagator(const QuarkPropagator& other) :
        BaseObject(other) {
    m_N = other.getN();
    m_mu = other.getMu();
    m_m = other.getM();
    m_epsilon2 = other.getEpsilon2();
    m_Lambda2 = other.getLambda2();
    m_epsilon = other.getEpsilon();
    m_Lambda = other.getLambda();
    m_a = other.m_a;
    m_b = other.m_b;
    m_A = other.m_A;
    m_B = other.m_B;
    m_roots = other.m_roots;
}

QuarkPropagator::~QuarkPropagator() {
    // TODO Auto-generated destructor stub
}

double QuarkPropagator::stox(double p2) const {
    return log(p2 / (m_Lambda * m_epsilon)) / log(m_Lambda / m_epsilon);
}

double QuarkPropagator::xtos(double x) const {
    return m_Lambda * m_epsilon * exp(x * log(m_Lambda / m_epsilon));
}

const NumA::VectorD& QuarkPropagator::getCoeffsA() const {
    return m_a;
}

void QuarkPropagator::setCoeffsA(const NumA::VectorD& a) {
    if (a.size() != m_N) {
        error(__func__, "Size of vector a is wrong!");
    }
    m_a = a;
    updateA();
}

void QuarkPropagator::setCoeffsA(double a) {
    m_a.assign(m_N, a);
    updateA();
}

double QuarkPropagator::getCoeffA(unsigned int i) const {
    return m_a.at(i);
}

const NumA::VectorD& QuarkPropagator::getCoeffsB() const {
    return m_b;
}

void QuarkPropagator::setCoeffsB(const NumA::VectorD& b) {
    if (b.size() != m_N) {
        error(__func__, "Size of vector b is wrong!");
    }
    m_b = b;
    updateB();
}

void QuarkPropagator::setCoeffsB(double b) {
    m_b.assign(m_N, b);
    updateB();
}

double QuarkPropagator::getCoeffB(unsigned int i) const {
    return m_b.at(i);
}

const NumA::VectorD& QuarkPropagator::getRoots() const {
    return m_roots;
}

unsigned int QuarkPropagator::getN() const {
    return m_N;
}

void QuarkPropagator::setN(unsigned int n) {
    m_N = n;

    if (n > 0) {
        m_a.resize(n, 0.);
        m_b.resize(n, 0.);
        m_A.resize(n, 0.);
        m_B.resize(n, 0.);
        m_roots.resize(n, 0.);
    }
    updateInterpolation();
}

double QuarkPropagator::getM() const {
    return m_m;
}

void QuarkPropagator::setM(double m) {
    m_m = m;
}

double QuarkPropagator::getEpsilon() const {
    return m_epsilon;
}

double QuarkPropagator::getEpsilon2() const {
    return m_epsilon2;
}

void QuarkPropagator::setEpsilon2(double epsilon2) {
    m_epsilon2 = epsilon2;
    m_epsilon = sqrt(epsilon2);
}

double QuarkPropagator::getLambda() const {
    return m_Lambda;
}

double QuarkPropagator::getLambda2() const {
    return m_Lambda2;
}

void QuarkPropagator::setLambda2(double lambda2) {
    m_Lambda2 = lambda2;
    m_Lambda = sqrt(lambda2);
}

double QuarkPropagator::getMu() const {
    return m_mu;
}

void QuarkPropagator::setMu(double mu) {
    m_mu = mu;
}

double QuarkPropagator::evaluateM(double p2) const {
    double A = evaluateA(p2);
    double B = evaluateB(p2);
    return B / A;
}

double QuarkPropagator::evaluateSigmaV(double p2) const {
    double A = evaluateA(p2);
    double B = evaluateB(p2);
    return A / (p2 * A * A + B * B);
}

double QuarkPropagator::evaluateSigmaS(double p2) const {
    double A = evaluateA(p2);
    double B = evaluateB(p2);
    return B / (p2 * A * A + B * B);
}

//double QuarkPropagator::differentiateSigmaV_a(double p2, unsigned int j) const {
//    double A = evaluateA(p2);
//    double B = evaluateB(p2);
//    double Aprime = differentiateA(p2, j);
//    double A2 = A * A;
//    double B2 = B * B;
//    double denom = p2 * A2 + B2;
//
//    return Aprime * (B2 - p2 * A2) / (denom * denom);
//}
//
//double QuarkPropagator::differentiateSigmaV_b(double p2, unsigned int j) const {
//    double A = evaluateA(p2);
//    double B = evaluateB(p2);
//    double Bprime = differentiateB(p2, j);
//    double denom = p2 * A * A + B * B;
//
//    return -2. * Bprime * A * B / (denom * denom);
//}
//
//double QuarkPropagator::differentiateSigmaS_a(double p2, unsigned int j) const {
//    double A = evaluateA(p2);
//    double B = evaluateB(p2);
//    double Aprime = differentiateA(p2, j);
//    double denom = p2 * A * A + B * B;
//
//    return -2. * p2 * Aprime * A * B / (denom * denom);
//}
//
//double QuarkPropagator::differentiateSigmaS_b(double p2, unsigned int j) const {
//    double A = evaluateA(p2);
//    double B = evaluateB(p2);
//    double Bprime = differentiateB(p2, j);
//    double A2 = A * A;
//    double B2 = B * B;
//    double denom = p2 * A2 + B2;
//
//    return Bprime * (p2 * A2 - B2) / (denom * denom);
//}

std::string QuarkPropagator::toString() const {
    return toString(100);
}

std::string QuarkPropagator::toString(unsigned int Npoints) const {
    std::vector<QuarkPropagator::QPFunction> functions(5, QuarkPropagator::A);
    functions.at(1) = QuarkPropagator::B;
    functions.at(2) = QuarkPropagator::M;
    functions.at(3) = QuarkPropagator::sigmaV;
    functions.at(4) = QuarkPropagator::sigmaS;
    return toString(Npoints, functions);
}

std::string QuarkPropagator::toString(unsigned int Npoints,
        std::vector<QuarkPropagator::QPFunction> functions) const {
    double x = -1.;
    double step = 2. / (Npoints - 1);
    std::vector<double> p2(Npoints, 0.);
    std::vector<std::vector<double> > values(Npoints, std::vector<double>());
    for (unsigned int i = 0; i < Npoints; i++) {
        p2.at(i) = xtos(x);
        values.at(i) = evaluate(functions, p2.at(i));
        x = x + step;
    }

    ElemUtils::Formatter formatter;
    formatter << BaseObject::toString();
    formatter << "Results with " << Npoints << " momenta:\n";
    formatter << "p2 [GeV^2]";
    if (std::find(functions.begin(), functions.end(), QuarkPropagator::ALL)
            < functions.end()) {
        formatter << " ; " << enumToString(QuarkPropagator::ALL);
    } else {
        for (unsigned int j = 0; j < functions.size(); j++) {
            formatter << " ; " << enumToString(functions.at(j));
        }
    }
    formatter << "\n";
    for (unsigned int i = 0; i < Npoints; i++) {
        formatter << p2.at(i);
        for (unsigned int j = 0; j < values.at(i).size(); j++) {
            formatter << " ; " << values.at(i).at(j);
        }
        formatter << "\n";
    }
    return formatter.str();
}

std::string QuarkPropagator::enumToString(
        QuarkPropagator::QPFunction function) const {
    switch (function) {
    case QuarkPropagator::A:
        return "A";
        break;
    case QuarkPropagator::B:
        return "B [GeV]";
        break;
    case QuarkPropagator::M:
        return "M [GeV]";
        break;
    case QuarkPropagator::sigmaV:
        return "sigmaV [GeV^-2]";
        break;
    case QuarkPropagator::sigmaS:
        return "sigmaS [GeV^-1]";
        break;
    case QuarkPropagator::ALL:
        return "A ; B [GeV] ; M [GeV] ; sigmaV [GeV^-2] ; sigmaS [GeV^-1]";
        break;
    default:
        return "UNDEFINED";
        break;
    }
}

const NumA::VectorD& QuarkPropagator::getA() const {
    return m_A;
}

void QuarkPropagator::setA(const NumA::VectorD& a) {
    if (a.size() != m_N) {
        error(__func__, "Size of vector a is wrong!");
    }
    m_A = a;
    updateCoeffsA();
}

const NumA::VectorD& QuarkPropagator::getB() const {
    return m_B;
}

void QuarkPropagator::setB(const NumA::VectorD& b) {
    if (b.size() != m_N) {
        error(__func__, "Size of vector b is wrong!");
    }
    m_B = b;
    updateCoeffsB();
}

double QuarkPropagator::getA(unsigned int i) const {
    return m_A[i];
}

void QuarkPropagator::setA(double a) {
    m_A.assign(m_N, a);
    updateCoeffsA();
}

double QuarkPropagator::getB(unsigned int i) const {
    return m_B[i];
}

void QuarkPropagator::setB(double b) {
    m_B.assign(m_N, b);
    updateCoeffsB();
}
