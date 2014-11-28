#ifndef VECTOR_4D_H
#define VECTOR_4D_H

/**
 * @file Vector4D.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 27 November 2014
 * @version 1.0
 *
 * @class Vector4D
 */

#include "Vector3D.h"

//TODO mathematique function add, sub, ...

class Vector4D: public Vector3D {
public:
    Vector4D();
    Vector4D(double x, double y, double z, double e);
    ~Vector4D();

    // ##### GETTERS & SETTERS #####

    double getE() const;
    void setE(double e);

private:
    double m_e;
};

#endif /* VECTOR_4D_H */
