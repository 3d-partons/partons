#include "AcCos2phi.h"

#include <cmath>

#include "AcObservable.h"

AcCos2phi::AcCos2phi() :
        FourrierObservable("AcCos2phi"), m_pAcObservable(new AcObservable()) {
}

AcCos2phi::~AcCos2phi() {
}

AcCos2phi* AcCos2phi::clone() const {
    return new AcCos2phi(*this);
}

//TODO check
double AcCos2phi::functionToIntegrate(double *x, double *params) {
    // x[0] = phi
    return m_pAcObservable->compute(m_pDVCSModule, x[0]) * cos(2 * x[0]);
}
