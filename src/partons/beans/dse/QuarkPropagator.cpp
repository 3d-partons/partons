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
		BaseObject(className), m_N(N), m_m(m), m_mu(mu), m_Lambda2(Lambda2), m_epsilon2(
				epsilon2) {
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
	if (a.size() != m_N) {
		error(__func__, "Size of vector a is wrong!");
	}
	m_a = a;
}

void QuarkPropagator::setCoeffsA(double a) {
	m_a.assign(m_N, a);
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
	if (b.size() != m_N) {
		error(__func__, "Size of vector b is wrong!");
	}
	m_b = b;
}

void QuarkPropagator::setCoeffsB(double b) {
	m_b.assign(m_N, b);
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

double QuarkPropagator::differentiateSigmaV_a(double p2, unsigned int j) const {
	double A = evaluateA(p2);
	double B = evaluateB(p2);
	double Aprime = differentiateA(p2, j);
	double A2 = A * A;
	double B2 = B * B;
	double denom = p2 * A2 + B2;

	return Aprime * (B2 - p2 * A2) / (denom * denom);
}

double QuarkPropagator::differentiateSigmaV_b(double p2, unsigned int j) const {
	double A = evaluateA(p2);
	double B = evaluateB(p2);
	double Bprime = differentiateB(p2, j);
	double denom = p2 * A * A + B * B;

	return -2. * Bprime * A * B / (denom * denom);
}

double QuarkPropagator::differentiateSigmaS_a(double p2, unsigned int j) const {
	double A = evaluateA(p2);
	double B = evaluateB(p2);
	double Aprime = differentiateA(p2, j);
	double denom = p2 * A * A + B * B;

	return -2. * p2 * Aprime * A * B / (denom * denom);
}

double QuarkPropagator::differentiateSigmaS_b(double p2, unsigned int j) const {
	double A = evaluateA(p2);
	double B = evaluateB(p2);
	double Bprime = differentiateB(p2, j);
	double A2 = A * A;
	double B2 = B * B;
	double denom = p2 * A2 + B2;

	return Bprime * (p2 * A2 - B2) / (denom * denom);
}

//TODO Complete toString
std::string QuarkPropagator::toString() const {
    Formatter formatter;
    formatter << BaseObject::toString();
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
	return formatter.str();
}
