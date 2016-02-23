#include "../../../../include/partons/beans/dse/GPMT.h"

#include <cmath>

#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"

GPMT::GPMT(double w, double I, double LambdaQCD, int Nf) :
        GluonPropagator("GPMT", w, I, LambdaQCD, Nf) {
}

GPMT::GPMT(const std::string& className, double w, double I, double LambdaQCD,
        int Nf) :
        GluonPropagator(className, w, I, LambdaQCD, Nf) {
}

GPMT::GPMT(const GPMT& other) :
        GluonPropagator(other) {
}

GPMT::~GPMT() {
    // TODO Auto-generated destructor stub
}

GPMT* GPMT::clone() const {
    return new GPMT(*this);
}

std::string GPMT::toString() const {
    return Formatter() << GluonPropagator::toString();
}

// TODO Store factors independent of k2 to avoid unnecessary recalculations
double GPMT::evaluateG(double k2) const {
    return 4. * PI * PI / pow(m_w, 6) * m_D * k2 * exp(-k2 / (m_w * m_w))
            + 8. * PI * PI * 12. / (33. - 2. * m_Nf) * (1. - exp(-k2))
                    / (k2
                            * log(
                                    exp(2.) - 1.
                                            + pow(
                                                    1.
                                                            + k2
                                                                    / (m_LambdaQCD
                                                                            * m_LambdaQCD),
                                                    2)));
}
