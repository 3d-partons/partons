#include "NfInterval.h"

#include "../../utils/stringUtils/Formatter.h"

NfInterval::NfInterval() :
        m_nf(0), m_lowerBound(0.), m_upperBound(0.), m_index(0) {
}

NfInterval::NfInterval(unsigned short nf, double lowerBound, double upperBound,
        unsigned int index) :
        m_nf(nf), m_lowerBound(lowerBound), m_upperBound(upperBound), m_index(
                index) {
}

NfInterval::~NfInterval() {
}

unsigned short NfInterval::getNf() const {
    return m_nf;
}

void NfInterval::setNf(unsigned short nf) {
    m_nf = nf;
}

bool NfInterval::operator <(const NfInterval& other) const {
    return (m_nf < other.m_nf);
}

unsigned int NfInterval::getIndex() const {
    return m_index;
}

void NfInterval::setIndex(unsigned int index) {
    m_index = index;
}

std::string NfInterval::toString() {
    Formatter formatter;

    formatter << "index" << m_index << '\n';
    formatter << "nf = " << m_nf << '\n';
    formatter << "lowerBound = " << m_lowerBound << '\n';
    formatter << "upperBound = " << m_upperBound << '\n';

    return formatter.str();
}

double NfInterval::getLowerBound() const {
    return m_lowerBound;
}

void NfInterval::setLowerBound(double lowerBound) {
    m_lowerBound = lowerBound;
}

double NfInterval::getUpperBound() const {
    return m_upperBound;
}

void NfInterval::setUpperBound(double upperBound) {
    m_upperBound = upperBound;
}
