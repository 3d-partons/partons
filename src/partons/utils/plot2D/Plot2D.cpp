#include "../../../../include/partons/utils/plot2D/Plot2D.h"

namespace PARTONS {


Plot2D::Plot2D() :
        m_x(0.), m_y(0.) {
}

Plot2D::Plot2D(double x, double y) {
    m_x = x;
    m_y = y;
}

Plot2D::~Plot2D() {
}

double Plot2D::getX() const {
    return m_x;
}

void Plot2D::setX(double x) {
    m_x = x;
}

double Plot2D::getY() const {
    return m_y;
}

void Plot2D::setY(double y) {
    m_y = y;
}

} /* namespace PARTONS */
