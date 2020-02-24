#include "../../../../include/partons/utils/plot2D/Plot2D.h"

namespace PARTONS {

Plot2D::Plot2D() {
}

Plot2D::Plot2D(const PhysicalType<double>& x, const PhysicalType<double>& y) {
    m_x = x;
    m_y = y;
}

Plot2D::~Plot2D() {
}

PhysicalType<double> Plot2D::getX() const {
    return m_x;
}

void Plot2D::setX(const PhysicalType<double>& x) {
    m_x = x;
}

PhysicalType<double> Plot2D::getY() const {
    return m_y;
}

void Plot2D::setY(const PhysicalType<double>& y) {
    m_y = y;
}

} /* namespace PARTONS */
