#include "../../../../include/partons/utils/compare/Differences.h"

#include "../../../../include/partons/utils/stringUtils/Formatter.h"

Differences::Differences() :
        m_absoluteDifference(0.), m_relativeDifference(0.) {

}

Differences::Differences(const double absoluteDifference,
        const double relativeDifference) :
        m_absoluteDifference(absoluteDifference), m_relativeDifference(
                relativeDifference) {
}

Differences::~Differences() {
}

std::string Differences::toString() const {
    return Formatter() << "absolute difference = " << m_absoluteDifference
            << " and relative difference = " << m_relativeDifference;
}

double Differences::getAbsoluteDifference() const {
    return m_absoluteDifference;
}

void Differences::setAbsoluteDifference(double absoluteDifference) {
    m_absoluteDifference = absoluteDifference;
}

double Differences::getRelativeDifference() const {
    return m_relativeDifference;
}

void Differences::setRelativeDifference(double relativeDifference) {
    m_relativeDifference = relativeDifference;
}
