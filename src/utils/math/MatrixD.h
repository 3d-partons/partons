#ifndef MATRIX_D_H
#define MATRIX_D_H

/**
 * @file MatrixD.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 23 October 2014
 *
 * @class MatrixD
 *
 * @brief Represents a two-dimensional array of double.
 */

#include <string>
#include <vector>

class MatrixD {
public:
    /**
     * Default constructor \n
     * Initializes an empty matrix.
     */
    MatrixD();

    /**
     * Create matrix with specific dimension and fill it with given values.
     *
     * @param _rows : row length
     * @param _cols : column length
     */
    MatrixD(const unsigned int _rows, const unsigned int _cols, ...);

    /**
     * Copy constructor
     *
     * @param rhs
     */
    MatrixD(const MatrixD& rhs);

    /**
     * Default destructor
     */
    virtual ~MatrixD();

    // Matrix/vector operations
    std::vector<double> operator*(const std::vector<double>& rhs);
    //std::vector<double> operator*=(const std::vector<double>& rhs);

    /**
     * Update the value at the given coordinates.
     *
     * @param i : row coordinate
     * @param j : column coordinate
     * @param value
     */
    void update(const unsigned int i, const unsigned int j, const double value);

    /**
     * Return a formatted characters string to display matrix's values
     *
     * @return std::string
     */
    std::string toString();

private:
    std::vector<std::vector<double> > m_matrix; ///< two-dimensional array of double for represent a matrix
    unsigned int m_rows;    ///< row length
    unsigned int m_cols;    ///< column length
};

#endif /* MATRIX_D_H */
