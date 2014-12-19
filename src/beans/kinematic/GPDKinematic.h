#ifndef GPD_KINEMATIC_H
#define GPD_KINEMATIC_H

/**
 * @file GPDKinematic.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 November 2014
 * @version 1.0
 *
 * @class GPDKinematic
 *
 * @brief Class representing the kinematic variables for a GPD model.
 */

#include <string>

#include "Kinematic.h"

class GPDKinematic: public Kinematic {
public:
    /**
     * Constructor
     *
     * @param x Bjorken variable
     * @param xi
     * @param t momentum transfer (Mandelstam variable)
     * @param MuF Factorisation scale
     * @param MuR Re-normalisation scale
     */
    GPDKinematic(double x, double xi, double t, double MuF, double MuR);

    /**
     * Default destructor
     */
    virtual ~GPDKinematic();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString();

    // ##### GETTERS & SETTERS #####

    double getMuF() const;
    void setMuF(double muF);
    double getMuR() const;
    void setMuR(double muR);
    double getX() const;
    void setX(double x);
    double getXi() const;
    void setXi(double xi);

private:
    //TODO name and unit ?
    double m_x;     ///< Bjorken variable
    double m_xi;
    double m_MuF;   ///< Factorisation scale
    double m_MuR;   ///< Re-normalisation scale
};

#endif /* GPD_KINEMATIC_H */
