#ifndef OBSERVABLE_KINEMATIC_H
#define OBSERVABLE_KINEMATIC_H

/**
 * @file ObservableKinematic.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 November 2014
 * @version 1.0
 *
 * @class ObservableKinematic
 *
 * @brief Class representing the kinematic variables.
 */

#include <string>
#include <vector>

#include "FormFactorKinematic.h"

class ObservableKinematic: public FormFactorKinematic {
public:
    /**
     * Constructor
     *
     * @param xB
     * @param t momentum transfer (Mandelstam variable)
     * @param Q2 virtualty of the photon
     * @param listOfPhi
     */
    ObservableKinematic(double xB, double t, double Q2,
            std::vector<double> listOfPhi);

    /**
     * Default destructor
     */
    virtual ~ObservableKinematic();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString();

    // ##### GETTERS & SETTERS #####

    const std::vector<double>& getListOfPhi() const;
    void setListOfPhi(const std::vector<double>& listOfPhi);

private:
    std::vector<double> m_listOfPhi; ///<  Angles between leptonic and hadronic planes
};

#endif /* OBSERVABLE_KINEMATIC_H */
