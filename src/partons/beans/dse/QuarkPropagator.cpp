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
        BaseObject("QuarkPropagator"), m_N(N), m_m(m), m_mu(mu) {
    m_Lambda = sqrt(Lambda2);
    m_epsilon = sqrt(epsilon2);
}

QuarkPropagator::QuarkPropagator(const std::string& className, unsigned int N,
        double m, double mu, double Lambda2, double epsilon2) :
        BaseObject(className), m_N(N), m_m(m), m_mu(mu) {
    m_Lambda = sqrt(Lambda2);
    m_epsilon = sqrt(epsilon2);
}

QuarkPropagator::~QuarkPropagator() {
    // TODO Auto-generated destructor stub
}

unsigned int QuarkPropagator::getN() const {
    return m_N;
}

void QuarkPropagator::setN(unsigned int n) {
    m_N = n;
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

void QuarkPropagator::setEpsilon(double epsilon) {
    m_epsilon = epsilon;
}

double QuarkPropagator::getLambda() const {
    return m_Lambda;
}

void QuarkPropagator::setLambda(double lambda) {
    m_Lambda = lambda;
}

double QuarkPropagator::getMu() const {
    return m_mu;
}

void QuarkPropagator::setMu(double mu) {
    m_mu = mu;
}

//TODO Complete toString
std::string QuarkPropagator::toString() const {
    return Formatter() << "";
}
