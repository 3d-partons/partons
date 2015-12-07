#include "Tolerances.h"

#include "../stringUtils/Formatter.h"
#include "MathUtils.h"

Tolerances::Tolerances() :
        BaseObject("Tolerances"), m_absoluteTolerance(0.), m_relativeTolerance(
                0.) {
}

Tolerances::Tolerances(double absoluteTolerance, double relativeTolerance) :
        BaseObject("Tolerances"), m_absoluteTolerance(absoluteTolerance), m_relativeTolerance(
                relativeTolerance) {

    MathUtils::isPositiveDouble(absoluteTolerance);
    MathUtils::isPositiveDouble(relativeTolerance);
}

Tolerances::~Tolerances() {
}

double Tolerances::getAbsoluteTolerance() const {
    return m_absoluteTolerance;
}

void Tolerances::setAbsoluteTolerance(double absoluteTolerance) {
    MathUtils::isPositiveDouble(absoluteTolerance);
    m_absoluteTolerance = absoluteTolerance;
}

double Tolerances::getRelativeTolerance() const {
    return m_relativeTolerance;
}

void Tolerances::setRelativeTolerance(double relativeTolerance) {
    MathUtils::isPositiveDouble(relativeTolerance);
    m_relativeTolerance = relativeTolerance;
}

std::string Tolerances::toString() const {
    Formatter formatter;

    formatter << getClassName();
    formatter << " m_absoluteTolerance = " << m_absoluteTolerance;
    formatter << " m_relativeTolerance = " << m_relativeTolerance;

    return formatter.str();
}
