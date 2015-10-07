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

#include "../../BaseObject.h"

class ParameterList;

class ObservableKinematic: public BaseObject {
public:
    static const std::string PARAMETER_NAME_XB;
    static const std::string PARAMETER_NAME_T;
    static const std::string PARAMETER_NAME_Q2;
    static const std::string PARAMETER_NAME_PHI;

    /**
     * Default constructor
     */
    ObservableKinematic();

    ObservableKinematic(ParameterList &parameterList);

    /**
     * Constructor
     *
     * @param xB Bjorken variable
     * @param t Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
     * @param Q2 Virtuality of the photon in Born approximation (in GeV^2)
     * @param listOfPhi list of angles between leptonic and hadronic planes (in degrees, Trento convention)
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
    double getQ2() const;
    void setQ2(double Q2);
    double getT() const;
    void setT(double t);
    double getXB() const;
    void setXB(double xB);

private:
    // TODO : What do we do when we consider Fourier harmonics of DVCS observables?
    double m_xB;    ///< Bjorken variable
    double m_t; ///< Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
    double m_Q2;  ///< Virtuality of the photon in Born approximation (in GeV^2)
    double m_MuF2;
    double m_MuR2;
    std::vector<double> m_listOfPhi; ///<  Angles between leptonic and hadronic planes (in degrees, Trento convention)
};

#endif /* OBSERVABLE_KINEMATIC_H */
