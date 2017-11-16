#include "../../../../include/partons/utils/math/ErrorBar.h"

namespace PARTONS {

ErrorBar::ErrorBar() :
        m_lowerBound(0.), m_upperBound(0.) {
}

ErrorBar::ErrorBar(double lowerBound, double upperBound) :
        m_lowerBound(lowerBound), m_upperBound(upperBound) {
}

ErrorBar::~ErrorBar() {
}

double ErrorBar::getLowerBound() const {
    return m_lowerBound;
}

void ErrorBar::setLowerBound(double lowerBound) {
    m_lowerBound = lowerBound;
}

double ErrorBar::getUpperBound() const {
    return m_upperBound;
}

void ErrorBar::setUpperBound(double upperBound) {
    m_upperBound = upperBound;
}

} /* namespace PARTONS */
