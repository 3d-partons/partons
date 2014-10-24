#ifndef MATRIX_D_H
#define MATRIX_D_H

/**
 * @file MatrixD.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 23 October 2014
 *
 * @class MatrixD
 *
 * @brief
 */

#include <vector>
#include <string>

class MatrixD {
public:
	MatrixD();
	MatrixD(unsigned int _rows, unsigned int _cols, ...);
	MatrixD(const MatrixD& rhs);
	virtual ~MatrixD();

	// Matrix/vector operations
	std::vector<double> operator*(const std::vector<double>& rhs);
	//std::vector<double> operator*=(const std::vector<double>& rhs);

	void update(const unsigned int i, const unsigned int j,
			const double value);

	std::string toString();
private:
	std::vector<std::vector<double> > m_matrix;
	unsigned int m_rows;
	unsigned int m_cols;
};

#endif /* MATRIX_D_H */
