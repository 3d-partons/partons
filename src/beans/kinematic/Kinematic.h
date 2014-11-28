#ifndef KINEMATIC_H
#define KINEMATIC_H

/**
 * @file Kinematic.h
 * @author: Bryan BERTHOU (CEA Saclay)
 * @date 26 November 2014
 * @version 1.0
 *
 * @class Kinematic
 *
 * @brief Class representing the kinematic variables.
 */

class Kinematic {
public:
    Kinematic(double xB, double t);
    ~Kinematic();

    // ##### GETTERS & SETTERS #####

    double getT() const;
    void setT(double t);
    double getXB() const;
    void setXB(double xB);

private:
    double m_xB;    ///< x Bjorken
    double m_t;     ///< momentum transfer (Mandelstam variable)
};

#endif /* KINEMATIC_H */
