#ifndef KINEMATIC_VARIABLES_H
#define KINEMATIC_VARIABLES_H

//TODO completer la documentation

/**
 * @file KinematicVariables.h
 * @date 23/07/2014
 * @author Bryan BERTHOU (CEA)
 * @version 1.0
 */

/**
 * @brief Class representing the kinematic variables (xB, t, Q2) of the Deeply Virtual Compton Scattering (DVCS) model.
 */
class KinematicVariables {

    double xB;      ///< x Bjorken (x = Q^2 / 2p.q)
    double t;       ///<  momentum transfer (Mandelstam variable)
    double Q2;      ///< virtualty of the photon
    double xi;      ///< transverse momentum transfer (xi = xB / (2. - xB))

    /**
     * Compute xi = xB / (2. - xB)
     */
    void updateXi();

public:

    /**
     * Default constructor
     */
    KinematicVariables();

    /**
     * Constructor
     *
     * @param _xB
     * @param _t
     * @param _Q2
     */
    KinematicVariables(double _xB, double _t, double _Q2);

    double getQ2() const;
    void setQ2(double _Q2);
    double getT() const;
    void setT(double _t);
    double getXB() const;
    void setXB(double _xB);
    double getXi() const;
};

#endif /* KINEMATIC_VARIABLES_H */
