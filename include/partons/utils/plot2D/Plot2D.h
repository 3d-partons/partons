#ifndef PLOT_2D_H
#define PLOT_2D_H

/**
 * @file Plot2D.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 29 April 2015
 * @version 1.0
 */

#include "../type/PhysicalType.h"

namespace PARTONS {

/**
 * @class Plot2D
 *
 * @brief Container to store a single point in 2D space
 */
class Plot2D {

public:

    /**
     * Default constructor.
     */
    Plot2D();

    /**
     * Assignment constructor.
     */
    Plot2D(const PhysicalType<double>& x, const PhysicalType<double>& y);

    /**
     * Destructor.
     */
    ~Plot2D();

    /**
     * Get x value.
     */
    PhysicalType<double> getX() const;

    /**
     * Set x value.
     */
    void setX(const PhysicalType<double>& x);

    /**
     * Get y value.
     */
    PhysicalType<double> getY() const;

    /**
     * Set y value.
     */
    void setY(const PhysicalType<double>& y);

private:

    /**
     * x value.
     */
    PhysicalType<double> m_x;

    /**
     * y value.
     */
    PhysicalType<double> m_y;
};

} /* namespace PARTONS */

#endif /* PLOT_2D_H */
