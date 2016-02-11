/**
 * @file QuarkPropagator.cpp
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 22, 2016
 * @version 1.0
 *
 * @class QuarkPropagator
 */

#include "../../../../include/partons/beans/dse/QuarkPropagator.h"

#include <cmath>

#include "../../../../include/partons/utils/stringUtils/Formatter.h"

QuarkPropagator::QuarkPropagator(unsigned int N, double m, double mu,
        double Lambda2, double epsilon2) :
        BaseObject("QuarkPropagator"), m_N(N), m_m(m), m_mu(mu), m_Lambda2(
                Lambda2), m_epsilon2(epsilon2) {
    m_Lambda = sqrt(Lambda2);
    m_epsilon = sqrt(epsilon2);
}

QuarkPropagator::QuarkPropagator(const std::string& className, unsigned int N,
        double m, double mu, double Lambda2, double epsilon2) :
        BaseObject(className), m_N(N), m_m(m), m_mu(mu), m_Lambda2(
                Lambda2), m_epsilon2(epsilon2) {
    m_Lambda = sqrt(Lambda2);
    m_epsilon = sqrt(epsilon2);
}

QuarkPropagator::QuarkPropagator(const QuarkPropagator& other) :
        BaseObject(other) {
    m_N = other.getN();
    m_mu = other.getMu();
    m_m = other.getM();
    m_epsilon2 = getEpsilon2();
    m_Lambda2 = getLambda2();
    m_epsilon = getEpsilon();
    m_Lambda = getLambda();
}

QuarkPropagator::~QuarkPropagator() {
    // TODO Auto-generated destructor stub
}

const NumA::VectorD& QuarkPropagator::getCoeffsA() const {
    return m_a;
}

void QuarkPropagator::setCoeffsA(const NumA::VectorD& a) {
    m_a = a;
}

const double QuarkPropagator::getCoeffA(unsigned int i) const {
    return m_a.at(i);
}

void QuarkPropagator::setCoeffA(unsigned int i, double a) {
    m_a.at(i) = a;
}

const NumA::VectorD& QuarkPropagator::getCoeffsB() const {
    return m_b;
}

void QuarkPropagator::setCoeffsB(const NumA::VectorD& b) {
    m_b = b;
}

const double QuarkPropagator::getCoeffB(unsigned int i) const {
    return m_b.at(i);
}

void QuarkPropagator::setCoeffB(unsigned int i, double b) {
    m_b.at(i) = b;
}

const std::vector<double>& QuarkPropagator::getRoots() const {
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
        m_roots.resize(n, 0.);
    }
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

//TODO Complete toString
std::string QuarkPropagator::toString() const {
    return Formatter();
}
