#include "Tolerances.h"

#include "MathUtils.h"

Tolerances::Tolerances() : m_absoluteTolerance(0.), m_relativeTolerance(0.) {
}

Tolerances::Tolerances(double absoluteTolerance, double relativeTolerance) :
        m_absoluteTolerance(absoluteTolerance),
                m_relativeTolerance(relativeTolerance) {

    MathUtils::isPositiveDouble(absoluteTolerance);
    MathUtils::isPositiveDouble(relativeTolerance);
}

Tolerances::~Tolerances() {
}

double Tolerances::getAbsoluteTolerance() const {
    return m_absoluteTolerance;
}

void Tolerances::setAbsoluteTolerance(double absoluteTolerance) {
    m_absoluteTolerance = absoluteTolerance;
}

double Tolerances::getRelativeTolerance() const {
    return m_relativeTolerance;
}

void Tolerances::setRelativeTolerance(double relativeTolerance) {
    m_relativeTolerance = relativeTolerance;
}
