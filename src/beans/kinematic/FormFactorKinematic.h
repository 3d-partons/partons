#ifndef FORM_FACTOR_KINEMATIC_H
#define FORM_FACTOR_KINEMATIC_H

/**
 * @file FormFactorKinematic.h
 * @author: Bryan BERTHOU (CEA Saclay)
 * @date 26 November 2014
 * @version 1.0
 *
 * @class FormFactorKinematic
 *
 * @brief Class representing the kinematic variables.
 */

#include "Kinematic.h"

class FormFactorKinematic: public Kinematic {
public:
    FormFactorKinematic(double xB, double t, double Q2);
    FormFactorKinematic(unsigned int binId, double xB, double t, double Q2);
    ~FormFactorKinematic();

    // ##### GETTERS & SETTERS #####

    unsigned int getBinId() const;
    void setBinId(unsigned int binId);
    double getQ2() const;
    void setQ2(double q2);

private:
    unsigned int m_binId;
    double m_Q2;    ///< virtualty of the photon
};

#endif /* FORM_FACTOR_KINEMATIC_H */
