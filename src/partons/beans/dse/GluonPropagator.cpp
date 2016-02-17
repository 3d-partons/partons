/**
 * @file GluonPropagator.cpp
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 28, 2016
 * @version 1.0
 *
 * @class GluonPropagator
 */

#include "../../../../include/partons/beans/dse/GluonPropagator.h"

#include <cmath>

#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"

GluonPropagator::GluonPropagator(double w, double I, double LambdaQCD, int Nf) :
        BaseObject("GluonPropagator"), m_LambdaQCD(LambdaQCD), m_Nf(Nf) {
    setI(w, I);
}

GluonPropagator::GluonPropagator(const std::string& className, double w,
        double I, double LambdaQCD, int Nf) :
        BaseObject(className), m_LambdaQCD(LambdaQCD), m_Nf(Nf) {
    setI(w, I);
}

GluonPropagator::GluonPropagator(const GluonPropagator& other) :
        BaseObject(other) {
    m_w = other.getW();
    m_D = other.getD();
    m_c = other.getC();
    m_I = other.getI();
    m_LambdaQCD = other.getLambdaQCD();
    m_Nf = other.getNf();
}

GluonPropagator::~GluonPropagator() {
    // TODO Auto-generated destructor stub
}

//TODO Complete toString
std::string GluonPropagator::toString() const {
    return Formatter() << BaseObject::toString();
}

double GluonPropagator::evaluateAlpha(double k2) const {
    return k2 * evaluateG(k2) / (4 * PI);
}

double GluonPropagator::getC() const {
    return m_c;
}

void GluonPropagator::setC(double w, double c) {
    if (w <= 0.) {
        error(__func__, "w must be positive!");
    }
    m_w = w;
    m_c = c;
    m_D = c / w;
    m_I = m_D / (w * w);
}

double GluonPropagator::getD() const {
    return m_D;
}

void GluonPropagator::setD(double w, double D) {
    if (w <= 0.) {
        error(__func__, "w must be positive!");
    }
    m_w = w;
    m_D = D;
    m_c = D * w;
    m_I = D / (w * w);
}

double GluonPropagator::getI() const {
    return m_I;
}

void GluonPropagator::setI(double w, double I) {
    if (w <= 0.) {
        error(__func__, "w must be positive!");
    }
    m_w = w;
    m_I = I;
    m_D = I * w * w;
    m_c = m_D * w;
}

double GluonPropagator::getLambdaQCD() const {
    return m_LambdaQCD;
}

void GluonPropagator::setLambdaQCD(double lambdaQCD) {
    m_LambdaQCD = lambdaQCD;
}

int GluonPropagator::getNf() const {
    return m_Nf;
}

void GluonPropagator::setNf(int nf) {
    m_Nf = nf;
}

double GluonPropagator::getW() const {
    return m_w;
}
