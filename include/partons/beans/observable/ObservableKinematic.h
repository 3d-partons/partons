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

#include "../../utils/type/PhysicalType.h"
#include "../Kinematic.h"

class ComparisonReport;
class Packet;
class ParameterList;
class Tolerances;

class ObservableKinematic: public Kinematic {
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
     * @param phi angle between leptonic and hadronic planes (in degrees, Trento convention)
     */
    ObservableKinematic(double xB, double t, double Q2, double phi);

    ObservableKinematic(double xB, double t, double Q2);

    ObservableKinematic(const std::string &xB, const std::string &t,
            const std::string &Q2, const std::string &phi);

    /**
     * Default destructor
     */
    virtual ~ObservableKinematic();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString() const;

    ComparisonReport compare(const ObservableKinematic &referenceObject,
            const Tolerances &tolerances) const;

    // ##### GETTERS & SETTERS #####

    double getQ2() const;
    void setQ2(double Q2);
    double getT() const;
    void setT(double t);
    double getXB() const;
    void setXB(double xB);
    PhysicalType<double> getPhi() const;
    void setPhi(double phi);

    void serialize(Packet &packet) const;
    void unserialize(Packet &packet);

private:
    // TODO : What do we do when we consider Fourier harmonics of DVCS observables?
    double m_xB;    ///< Bjorken variable
    double m_t; ///< Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
    double m_Q2;  ///< Virtuality of the photon in Born approximation (in GeV^2)
    PhysicalType<double> m_phi; ///<  Angle between leptonic and hadronic planes (in degrees, Trento convention)
};

Packet& operator <<(Packet& packet, ObservableKinematic& observableKinematic);
Packet& operator >>(Packet& packet, ObservableKinematic& observableKinematic);

#endif /* OBSERVABLE_KINEMATIC_H */
