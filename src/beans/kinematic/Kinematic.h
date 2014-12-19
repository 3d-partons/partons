#ifndef KINEMATIC_H
#define KINEMATIC_H

/**
 * @file Kinematic.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 November 2014
 * @version 1.0
 *
 * @class Kinematic
 *
 * @brief Class representing the kinematic variables.
 */

#include <string>

class Kinematic {
public:
    /**
     * Constructor
     *
     * @param xB Bjorken variable
     * @param t momentum transfer (Mandelstam variable)
     */
    Kinematic(double xB, double t);

    /**
     * Default destructor
     */
    virtual ~Kinematic();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString();

    // ##### GETTERS & SETTERS #####

    double getT() const;
    void setT(double t);
    double getXB() const;
    void setXB(double xB);

private:
    //TODO unit ?
    double m_xB;    ///< x Bjorken
    double m_t;     ///< momentum transfer (Mandelstam variable)
};

#endif /* KINEMATIC_H */
