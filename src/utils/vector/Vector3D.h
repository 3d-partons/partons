#ifndef VECTOR_3D_H
#define VECTOR_3D_H

/**
 * @file Vector3D.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 26 November 2014
 * @version 1.0
 *
 * @class Vector3D
 */

#include "Vector2D.h"

//TODO mathematique function add, sub, ...

class Vector3D: public Vector2D {
public:
    Vector3D();
    Vector3D(double x, double y, double z);
    ~Vector3D();

    // Scalar operation
    double operator*(const Vector3D &rhs) const;

    // ##### GETTERS & SETTERS #####

    double getZ() const;
    void setZ(double z);

private:
    double m_z;
};

#endif /* VECTOR_3D_H */
