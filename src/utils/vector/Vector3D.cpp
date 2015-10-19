#include "Vector3D.h"

#include <sstream>
//#include <string>

Vector3D::Vector3D() :
        Vector2D(0., 0.), m_z(0.) {
}

Vector3D::Vector3D(double x, double y, double z) :
        Vector2D(x, y), m_z(z) {
}

Vector3D::~Vector3D() {
}

double Vector3D::operator*(const Vector3D& rhs) const {
    return getX() * rhs.getX() + getY() * rhs.getY() + m_z * rhs.getZ();
}

// ##### GETTERS & SETTERS #####

double Vector3D::getZ() const {
    return m_z;
}

void Vector3D::setZ(double z) {
    m_z = z;
}

std::string Vector3D::toString() {

    std::ostringstream os;

    os << "vector<" << getX() << ", " << getY() << ", " << m_z << std::endl;

    return os.str();
}
