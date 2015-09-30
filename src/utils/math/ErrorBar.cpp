#include "ErrorBar.h"

ErrorBar::ErrorBar() :
        m_upperBound(0.), m_lowerBound(0.) {
}

ErrorBar::ErrorBar(double upperBound, double lowerBound) :
        m_upperBound(upperBound), m_lowerBound(lowerBound) {
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
