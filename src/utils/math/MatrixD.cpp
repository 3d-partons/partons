#include "MatrixD.h"

#include <stdarg.h>
#include <stdexcept>

#include "../stringUtils/Formatter.h"

MatrixD::MatrixD() :
        m_rowLength(0), m_colLength(0) {
}

// TODO test matrix construction
MatrixD::MatrixD(const size_t _rowLength, const size_t _colLength, ...) :
        m_rowLength(_rowLength), m_colLength(_colLength) {

    va_list ap;
    va_start(ap, _colLength);

    m_matrix.resize(m_colLength);
    for (unsigned int i = 0; i < m_matrix.size(); i++) {
        m_matrix[i].resize(m_rowLength);
        // fill each line by values
        for (unsigned int j = 0; j < m_matrix[i].size(); j++) {
        m_matrix[i][j] = va_arg(ap, double);
    }
}
}

MatrixD::MatrixD(const MatrixD &other) {
m_matrix = other.m_matrix;
m_rowLength = other.m_rowLength;
m_colLength = other.m_colLength;
}

MatrixD::~MatrixD() {
}

VectorD MatrixD::operator*(const VectorD& rhs) {

// test vector's size and matrix's size
if (rhs.size() != m_rowLength) {
    throw std::runtime_error(
            Formatter() << "[MatrixD::operator*] Vector's size = " << rhs.size()
                    << " does not match matrix size = " << m_rowLength);
}

VectorD result(rhs.size(), 0.0);

for (unsigned i = 0; i < m_rowLength; i++) {
    for (unsigned j = 0; j < m_colLength; j++) {
        result[i] += this->m_matrix[i][j] * rhs[j];
    }
}

return result;
}

//std::vector<double> MatrixD::operator*=(const std::vector<double>& rhs) {
//	std::vector<double> result = (*this) * rhs;
//	return result;
//}

void MatrixD::update(const size_t i, const size_t j,
    const double value) {
if (i > m_rowLength || j > m_colLength) {
    throw std::runtime_error(
            Formatter()
                    << "[MatrixD::update] i or j index does not match matrix size rows = "
                    << m_rowLength << " cols = " << m_colLength);
}

m_matrix[i][j] = value;
}

std::string MatrixD::toString() {
Formatter formatter;

for (unsigned int i = 0; i < m_matrix.size(); i++) {
    for (unsigned int j = 0; j < m_matrix[i].size(); j++) {
        formatter << m_matrix[i][j];
        formatter << " ";
    }

    formatter << "\n";
}

return formatter.str();
}

VectorD MatrixD::getLine(const size_t lineIndex) {
if (lineIndex >= m_matrix.size()) {
    //TODO print more information
    throw std::runtime_error("[MatrixD::getLine] lineIndex is out of bound");
}

return m_matrix[lineIndex];
}

