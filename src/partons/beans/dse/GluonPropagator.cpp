/**
 * @file GluonPropagator.cpp
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 28, 2016
 * @version 1.0
 *
 * @class GluonPropagator
 */

#include "../../../../include/partons/beans/dse/GluonPropagator.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <cmath>

#include "../../../../include/partons/FundamentalPhysicalConstants.h"

GluonPropagator::GluonPropagator(const std::string& className, double w,
        double I, double LambdaQCD, int Nf) :
        BaseObject(className), m_w(w), m_I(I), m_D(I * w * w), m_c(
                I * w * w * w), m_LambdaQCD(LambdaQCD), m_Nf(Nf), m_factorPer(
                0.), m_tau(exp(2) - 1.), m_LambdaQCD2(LambdaQCD * LambdaQCD), m_factorIR(
                0.), m_w2(w * w) {
    m_factorPer = 8. * Constant::PI * Constant::PI * 12. / (33. - 2. * Nf);
    // /!\ m_factorIR needs to be initialized in the daughter class constructor!
}

GluonPropagator::GluonPropagator(const GluonPropagator& other) :
        BaseObject(other) {
    m_w = other.getW();
    m_D = other.getD();
    m_c = other.getC();
    m_I = other.getI();
    m_LambdaQCD = other.getLambdaQCD();
    m_Nf = other.getNf();
    m_factorPer = other.m_factorPer;
    m_tau = other.m_tau;
    m_LambdaQCD2 = other.m_LambdaQCD2;
    m_factorIR = other.m_factorIR;
    m_w2 = other.m_w2;
}

GluonPropagator::~GluonPropagator() {
    // TODO Auto-generated destructor stub
}

//TODO Complete toString
std::string GluonPropagator::toString() const {
    return BaseObject::toString();
}

double GluonPropagator::evaluateAlpha(double k2) const {
    return k2 * evaluateG(k2) / (4 * Constant::PI);
}

double GluonPropagator::evaluateGper(double k2) const {
    return m_factorPer * (1. - exp(-k2))
            / (k2 * log(m_tau + pow(1. + k2 / m_LambdaQCD2, 2)));
}

void GluonPropagator::updatePer() {
    m_factorPer = 8. * Constant::PI * Constant::PI * 12. / (33. - 2. * m_Nf);
}

double GluonPropagator::getC() const {
    return m_c;
}

void GluonPropagator::setC(double w, double c) {
    if (w <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "w must be positive!");
    }
    m_w = w;
    m_w2 = w * w;
    m_c = c;
    m_D = c / w;
    m_I = m_D / (w * w);
    updateIR();
}

double GluonPropagator::getD() const {
    return m_D;
}

void GluonPropagator::setD(double w, double D) {
    if (w <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "w must be positive!");
    }
    m_w = w;
    m_w2 = w * w;
    m_D = D;
    m_c = D * w;
    m_I = D / (w * w);
    updateIR();
}

double GluonPropagator::getI() const {
    return m_I;
}

void GluonPropagator::setI(double w, double I) {
    if (w <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "w must be positive!");
    }
    m_w = w;
    m_w2 = w * w;
    m_I = I;
    m_D = I * w * w;
    m_c = m_D * w;
    updateIR();
}

double GluonPropagator::getLambdaQCD() const {
    return m_LambdaQCD;
}

void GluonPropagator::setLambdaQCD(double lambdaQCD) {
    m_LambdaQCD = lambdaQCD;
    m_LambdaQCD2 = lambdaQCD * lambdaQCD;
    updatePer();
}

int GluonPropagator::getNf() const {
    return m_Nf;
}

void GluonPropagator::setNf(int nf) {
    m_Nf = nf;
    updatePer();
}

double GluonPropagator::getW() const {
    return m_w;
}
