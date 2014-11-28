#include "Vector4D.h"

Vector4D::Vector4D()
        : Vector3D(0., 0., 0.), m_e(0.) {
}

Vector4D::Vector4D(double x, double y, double z, double e)
        : Vector3D(x, y, z), m_e(e) {
}

Vector4D::~Vector4D() {
}

// ##### GETTERS & SETTERS #####

double Vector4D::getE() const {
    return m_e;
}

void Vector4D::setE(double e) {
    m_e = e;
}
