#include "../../../../include/partons/beans/active_flavors_thresholds/ActiveFlavorsThresholds.h"

#include <ElementaryUtils/string_utils/Formatter.h>

namespace PARTONS {



ActiveFlavorsThresholds::ActiveFlavorsThresholds() :
        m_nf(0), m_lowerBound(0.), m_upperBound(0.), m_index(0) {
}

ActiveFlavorsThresholds::ActiveFlavorsThresholds(unsigned short nf, double lowerBound, double upperBound,
        unsigned int index) :
        m_nf(nf), m_lowerBound(lowerBound), m_upperBound(upperBound), m_index(
                index) {
}

ActiveFlavorsThresholds::~ActiveFlavorsThresholds() {
}

unsigned short ActiveFlavorsThresholds::getNf() const {
    return m_nf;
}

void ActiveFlavorsThresholds::setNf(unsigned short nf) {
    m_nf = nf;
}

bool ActiveFlavorsThresholds::operator <(const ActiveFlavorsThresholds& other) const {
    return (m_nf < other.m_nf);
}

unsigned int ActiveFlavorsThresholds::getIndex() const {
    return m_index;
}

void ActiveFlavorsThresholds::setIndex(unsigned int index) {
    m_index = index;
}

std::string ActiveFlavorsThresholds::toString() const{
    ElemUtils::Formatter formatter;

    formatter << "index" << m_index << '\n';
    formatter << "nf = " << m_nf << '\n';
    formatter << "lowerBound = " << m_lowerBound << '\n';
    formatter << "upperBound = " << m_upperBound << '\n';

    return formatter.str();
}

double ActiveFlavorsThresholds::getLowerBound() const {
    return m_lowerBound;
}

void ActiveFlavorsThresholds::setLowerBound(double lowerBound) {
    m_lowerBound = lowerBound;
}

double ActiveFlavorsThresholds::getUpperBound() const {
    return m_upperBound;
}

void ActiveFlavorsThresholds::setUpperBound(double upperBound) {
    m_upperBound = upperBound;
}

} /* namespace PARTONS */
