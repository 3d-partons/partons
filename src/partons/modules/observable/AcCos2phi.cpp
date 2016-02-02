#include "../../../../include/partons/modules/observable/AcCos2phi.h"

#include <cmath>

#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/observable/Ac.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

// Initialise [class]::classId with a unique name.
const unsigned int AcCos2phi::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new AcCos2phi("AcCos2phi"));

AcCos2phi::AcCos2phi(const std::string &className)
        : FourierObservable(className), m_pAcObservable(0) {
    m_pAcObservable =
            Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    Ac::classId);
}

AcCos2phi::AcCos2phi(const AcCos2phi& other)
        : FourierObservable(other) {
    if (other.m_pAcObservable != 0) {
        m_pAcObservable = other.m_pAcObservable->clone();
    } else {
        m_pAcObservable = 0;
    }
}

AcCos2phi::~AcCos2phi() {
}

AcCos2phi* AcCos2phi::clone() const {
    return new AcCos2phi(*this);
}

//TODO check
double AcCos2phi::functionToIntegrate(std::vector<double> x,
        std::vector<double> params) {
    // x[0] = phi
    return m_pAcObservable->compute(m_pProcess, x[0]) * cos(2 * x[0]);
}
