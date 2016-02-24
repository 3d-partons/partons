#include "../../../../include/partons/beans/dse/GPQC.h"

#include <cmath>

#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"

GPQC::GPQC(double w, double I, double LambdaQCD, int Nf) :
        GluonPropagator("GPQC", w, I, LambdaQCD, Nf) {
}

GPQC::GPQC(const std::string& className, double w, double I, double LambdaQCD,
        int Nf) :
        GluonPropagator(className, w, I, LambdaQCD, Nf) {
}

GPQC::GPQC(const GPQC& other) :
        GluonPropagator(other) {
}

GPQC::~GPQC() {
    // TODO Auto-generated destructor stub
}

GPQC* GPQC::clone() const {
    return new GPQC(*this);
}

std::string GPQC::toString() const {
    return Formatter() << GluonPropagator::toString();
}

// TODO Store factors independent of k2 to avoid unnecessary recalculations
double GPQC::evaluateG(double k2) const {
    return 8. * PI * PI / pow(m_w, 4) * m_D * exp(-k2 / (m_w * m_w))
            /* + 8. * PI * PI * 12. / (33. - 2. * m_Nf) * (1. - exp(-k2))
                    / (k2
                            * log(
                                    exp(2.) - 1.
                                            + pow(
                                                    1.
                                                            + k2
                                                                    / (m_LambdaQCD
                                                                            * m_LambdaQCD),
                                                    2))) */;
}
