#ifndef GPD_KINEMATIC_H
#define GPD_KINEMATIC_H

/**
 * @file GPDKinematic.h
 * @author: Bryan BERTHOU (CEA Saclay)
 * @date 26 November 2014
 * @version 1.0
 *
 * @class GPDKinematic
 *
 * @brief Class representing the kinematic variables.
 */

#include "Kinematic.h"

class GPDKinematic: public Kinematic {
public:
    GPDKinematic(double x, double xB, double t, double MuF, double MuR);
    ~GPDKinematic();

    // ##### GETTERS & SETTERS #####

    double getMuF() const;
    void setMuF(double muF);
    double getMuR() const;
    void setMuR(double muR);
    double getX() const;
    void setX(double x);

private:
    double m_x;     ///< Bjorken variable
    double m_MuF;   ///< Factorisation
    double m_MuR;   ///< Re-normalisation
};

#endif /* GPD_KINEMATIC_H */
