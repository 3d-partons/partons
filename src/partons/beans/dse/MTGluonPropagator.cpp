#include "../../../../include/partons/beans/dse/MTGluonPropagator.h"

#include <cmath>

#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"

MTGluonPropagator::MTGluonPropagator(double w, double I, double LambdaQCD,
        int Nf) :
        GluonPropagator("MTGluonPropagator", w, I, LambdaQCD, Nf) {
    m_factorIR = 4. * PI * PI / pow(m_w, 6) * m_D;
}

MTGluonPropagator::MTGluonPropagator(const std::string& className, double w,
        double I, double LambdaQCD, int Nf) :
        GluonPropagator(className, w, I, LambdaQCD, Nf) {
    m_factorIR = 4. * PI * PI / pow(m_w, 6) * m_D;
}

MTGluonPropagator::MTGluonPropagator(const MTGluonPropagator& other) :
        GluonPropagator(other) {
}

MTGluonPropagator::~MTGluonPropagator() {
    // TODO Auto-generated destructor stub
}

MTGluonPropagator* MTGluonPropagator::clone() const {
    return new MTGluonPropagator(*this);
}

std::string MTGluonPropagator::toString() const {
    return Formatter() << GluonPropagator::toString();
}

// TODO Store factors independent of k2 to avoid unnecessary recalculations
double MTGluonPropagator::evaluateG(double k2) const {
    return m_factorIR * k2 * exp(-k2 / m_w2) + evaluateGper(k2);
}

void MTGluonPropagator::updateIR() {
    m_factorIR = 4. * PI * PI / pow(m_w, 6) * m_D;
}
