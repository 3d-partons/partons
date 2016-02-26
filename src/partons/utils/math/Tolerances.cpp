#include "../../../../include/partons/utils/math/Tolerances.h"

#include <sstream>
#include <stdexcept>

#include "../../../../include/partons/utils/stringUtils/Formatter.h"

Tolerances::Tolerances() :
        BaseObject("Tolerances"), m_absoluteTolerance(0.), m_relativeTolerance(
                0.) {
}

Tolerances::Tolerances(double absoluteTolerance, double relativeTolerance) :
        BaseObject("Tolerances"), m_absoluteTolerance(absoluteTolerance), m_relativeTolerance(
                relativeTolerance) {

    isPositiveDouble(absoluteTolerance);
    isPositiveDouble(relativeTolerance);
}

Tolerances::~Tolerances() {
}

double Tolerances::getAbsoluteTolerance() const {
    return m_absoluteTolerance;
}

void Tolerances::setAbsoluteTolerance(double absoluteTolerance) {
    isPositiveDouble(absoluteTolerance);
    m_absoluteTolerance = absoluteTolerance;
}

double Tolerances::getRelativeTolerance() const {
    return m_relativeTolerance;
}

void Tolerances::setRelativeTolerance(double relativeTolerance) {
    isPositiveDouble(relativeTolerance);
    m_relativeTolerance = relativeTolerance;
}

void Tolerances::isPositiveDouble(double tolerance) {

    // Check that tolerance is positive
    std::ostringstream toleranceStream;
    toleranceStream << tolerance;
    std::string toleranceString = toleranceStream.str();
    std::string ErrorMessage = "Tolerance should be >0. Here tolerance = "
            + toleranceString;

    if (tolerance <= 0.)
        throw std::range_error(ErrorMessage);
}

std::string Tolerances::toString() const {
    Formatter formatter;

    formatter << getClassName();
    formatter << " m_absoluteTolerance = " << m_absoluteTolerance;
    formatter << " m_relativeTolerance = " << m_relativeTolerance;

    return formatter.str();
}
