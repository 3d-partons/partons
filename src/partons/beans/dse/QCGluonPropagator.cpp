#include "../../../../include/partons/beans/dse/QCGluonPropagator.h"

#include <cmath>

#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"

QCGluonPropagator::QCGluonPropagator(double w, double I, double LambdaQCD,
        int Nf) :
        GluonPropagator("QCGluonPropagator", w, I, LambdaQCD, Nf) {
    m_factorIR = 8. * PI * PI / pow(m_w, 4) * m_D;
}

QCGluonPropagator::QCGluonPropagator(const std::string& className, double w,
        double I, double LambdaQCD, int Nf) :
        GluonPropagator(className, w, I, LambdaQCD, Nf) {
    m_factorIR = 8. * PI * PI / pow(m_w, 4) * m_D;
}

QCGluonPropagator::QCGluonPropagator(const QCGluonPropagator& other) :
        GluonPropagator(other) {
}

QCGluonPropagator::~QCGluonPropagator() {
    // TODO Auto-generated destructor stub
}

QCGluonPropagator* QCGluonPropagator::clone() const {
    return new QCGluonPropagator(*this);
}

std::string QCGluonPropagator::toString() const {
    return Formatter() << GluonPropagator::toString();
}

// TODO Store factors independent of k2 to avoid unnecessary recalculations
double QCGluonPropagator::evaluateG(double k2) const {
    return m_factorIR * exp(-k2 / m_w2) /*+ evaluateGper(k2)*/;
}

void QCGluonPropagator::updateIR() {
    m_factorIR = 8. * PI * PI / pow(m_w, 4) * m_D;
}
