#ifndef VECTOR_D_H
#define VECTOR_D_H

/**
 * @file VectorD.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 07 September 2015
 * @version 1.0
 *
 * @class VectorD
 */

#include <stddef.h>
#include <vector>

class VectorD {
public:
    VectorD();
    VectorD(size_t size, double initValue);
    VectorD(const std::vector<double> &vector);
    virtual ~VectorD();

    void push_back(double value);
    void resize(size_t n);
    size_t size() const;

    VectorD sub(size_t startPos, size_t endPos);

    double operator *(const VectorD &rhs);
    double& operator [](size_t n);
    double& at(size_t n);
    const double& at(size_t n) const;
    const double& operator [](size_t n) const;

private:
    std::vector<double> m_vector;
};

#endif /* VECTOR_D_H */
