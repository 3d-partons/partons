#include "Vector2D.h"

Vector2D::Vector2D()
        : m_x(0.), m_y(0.) {
}

Vector2D::Vector2D(double x, double y)
        : m_x(x), m_y(y) {
}

Vector2D::~Vector2D() {
}

// ##### GETTERS & SETTERS #####

double Vector2D::getX() const {
    return m_x;
}

void Vector2D::setX(double x) {
    m_x = x;
}

double Vector2D::getY() const {
    return m_y;
}

void Vector2D::setY(double y) {
    m_y = y;
}
