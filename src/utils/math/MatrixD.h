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

#include <stddef.h>
#include <string>
#include <vector>

#include "../vector/VectorD.h"

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
    MatrixD(const size_t _rowLength, const size_t _colLength, ...);

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

    VectorD getLine(const size_t lineIndex);

    // Matrix/vector operations
    VectorD operator*(const VectorD& rhs);
    //std::vector<double> operator*=(const std::vector<double>& rhs);

    /**
     * Update the value at the given coordinates.
     *
     * @param i : row coordinate
     * @param j : column coordinate
     * @param value
     */
    void update(const size_t i, const size_t j, const double value);

    /**
     * Return a formatted characters string to display matrix's values
     *
     * @return std::string
     */
    std::string toString();

    // ##### GETTERS & SETTERS #####

private:
    std::vector<VectorD> m_matrix; ///< two-dimensional array of double for represent a matrix
    size_t m_rowLength;    ///< row length
    size_t m_colLength;    ///< column length
};

#endif /* MATRIX_D_H */
