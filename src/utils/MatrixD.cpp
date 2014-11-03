#include "MatrixD.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdexcept>

#include "stringUtils/Formatter.h"

MatrixD::MatrixD() :
		m_rows(0), m_cols(0) {
}

MatrixD::MatrixD(unsigned int _rows, unsigned int _cols, ...) :
		m_rows(_rows), m_cols(_cols) {

	va_list ap;
	va_start(ap, _cols);

	m_matrix.resize(m_rows);
	for (unsigned int i = 0; i < m_matrix.size(); i++) {
		m_matrix[i].resize(m_cols);
		for (unsigned int j = 0; j < m_matrix[i].size(); j++) {
			m_matrix[i][j] = va_arg(ap, double);
		}
	}
}

MatrixD::MatrixD(const MatrixD &other) {
	m_matrix = other.m_matrix;
	m_rows = other.m_rows;
	m_cols = other.m_cols;
}

MatrixD::~MatrixD() {
}

std::vector<double> MatrixD::operator*(const std::vector<double>& rhs) {

	// test vector's size and matrix's size
	if (rhs.size() != m_rows) {
		throw std::runtime_error(
				Formatter() << "[MatrixD::operator*] Vector's size = "
						<< rhs.size() << " does not match matrix size = "
						<< m_rows);
	}

	std::vector<double> result(rhs.size(), 0.0);

	for (unsigned i = 0; i < m_rows; i++) {
		for (unsigned j = 0; j < m_cols; j++) {
			result[i] += this->m_matrix[i][j] * rhs[j];
		}
	}

	return result;
}

//std::vector<double> MatrixD::operator*=(const std::vector<double>& rhs) {
//	std::vector<double> result = (*this) * rhs;
//	return result;
//}

void MatrixD::update(const unsigned int i, const unsigned int j,
		const double value) {
	if (i > m_rows || j > m_cols) {
		throw std::runtime_error(
				Formatter()
						<< "[MatrixD::update] i or j index does not match matrix size rows = "
						<< m_rows << " cols = " << m_cols);
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
