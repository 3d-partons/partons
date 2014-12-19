#ifndef FORM_FACTOR_KINEMATIC_H
#define FORM_FACTOR_KINEMATIC_H

/**
 * @file FormFactorKinematic.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 November 2014
 * @version 1.0
 *
 * @class FormFactorKinematic
 *
 * @brief Class representing the kinematic variables.
 */

#include <string>

#include "Kinematic.h"

class FormFactorKinematic: public Kinematic {
public:
    /**
     * Constructor
     *
     * @param xB
     * @param t momentum transfer (Mandelstam variable)
     * @param Q2 virtualty of the photon
     */
    FormFactorKinematic(double xB, double t, double Q2);

    /**
     * Constructor with a binId
     *
     * @param binId
     * @param xB
     * @param t momentum transfer (Mandelstam variable)
     * @param Q2 virtualty of the photon
     */
    FormFactorKinematic(unsigned int binId, double xB, double t, double Q2);

    /**
     * Default destructor
     */
    virtual ~FormFactorKinematic();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString();

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
