#include "VectorD.h"

#include <stdexcept>

VectorD::VectorD() {
}

VectorD::VectorD(size_t size, double initValue) {
    for (size_t i = 0; i != size; i++) {
        push_back(initValue);
    }
}

VectorD::VectorD(const std::vector<double> &vector) :
        m_vector(vector) {

}

VectorD::~VectorD() {
}

void VectorD::push_back(double value) {
    m_vector.push_back(value);
}

void VectorD::resize(size_t n) {
    m_vector.resize(n);
}

size_t VectorD::size() const {
    return m_vector.size();
}

double VectorD::operator *(const VectorD &rhs) {
    double result = 0.;

    unsigned int vectorSize = m_vector.size();

    if (vectorSize != rhs.size()) {
        throw std::runtime_error(
                "[VectorD::operator *] Vectors have different size");
    }

    for (unsigned int i = 0; i != vectorSize; i++) {
        result += (m_vector[i] * rhs[i]);
    }

    return result;
}

double& VectorD::operator [](size_t n) {
    return m_vector[n];
}

double& VectorD::at(size_t n) {
    return m_vector.at(n);
}

const double& VectorD::at(size_t n) const {
    return m_vector.at(n);
}

const double& VectorD::operator [](size_t n) const {
    return m_vector[n];
}

VectorD VectorD::sub(size_t startPos, size_t endPos) {
    VectorD result;

    for (size_t i = startPos; i != endPos; i++) {
        result.push_back(m_vector[i]);
    }

    return result;
}
