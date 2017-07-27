#ifndef PLOT_2D_H
#define PLOT_2D_H

/**
 * @file Plot2D.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 29 April 2015
 * @version 1.0
 */

namespace PARTONS {

/**
 * @class Plot2D
 *
 * @brief
 */
class Plot2D {
public:
    Plot2D();
    Plot2D(double x, double y);
    ~Plot2D();

    // ##### GETTERS & SETTERS #####

    double getX() const;
    void setX(double x);
    double getY() const;
    void setY(double y);

private:
    double m_x;
    double m_y;
};

} /* namespace PARTONS */

#endif /* PLOT_2D_H */
