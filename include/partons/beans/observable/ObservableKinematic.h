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
#include <ElementaryUtils/parameters/GenericType.h>

#include "../../utils/type/PhysicalType.h"
#include "../Kinematic.h"

namespace ElemUtils {
class Parameters;
} /* namespace ElemUtils */

class ObservableKinematic: public Kinematic {
public:
    static const std::string PARAMETER_NAME_XB;
    static const std::string PARAMETER_NAME_T;
    static const std::string PARAMETER_NAME_Q2;
    static const std::string PARAMETER_NAME_PHI;
    static const std::string PARAMETER_NAME_BEAM_ENERGY;

    /**
     * Default constructor
     */
    ObservableKinematic();

    ObservableKinematic(const ElemUtils::Parameters &parameters);

    /**
     * Constructor
     *
     * @param xB Bjorken variable
     * @param t Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
     * @param Q2 Virtuality of the photon in Born approximation (in GeV^2)
     * @param E Beam energy (in GeV)
     * @param phi Angle between leptonic and hadronic planes (in degrees, Trento convention)
     */
    ObservableKinematic(double xB, double t, double Q2, double E, double phi);

    ObservableKinematic(double xB, double t, double Q2, double E);

    ObservableKinematic(const ElemUtils::GenericType& xB,
            const ElemUtils::GenericType& t, const ElemUtils::GenericType& Q2,
            const ElemUtils::GenericType& E, const ElemUtils::GenericType& phi);

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

//    ComparisonReport compare(const ObservableKinematic &referenceObject,
//            const NumA::Tolerances &tolerances) const;

// ##### GETTERS & SETTERS #####

    double getQ2() const;
    void setQ2(double Q2);
    double getT() const;
    void setT(double t);
    double getXB() const;
    void setXB(double xB);
    double getE() const;
    void setE(double E);
    PhysicalType<double> getPhi() const;
    void setPhi(double phi);

    void serialize(ElemUtils::Packet &packet) const;
    void unserialize(ElemUtils::Packet &packet);

protected:
    virtual void updateHashSum() const;

private:
    // TODO : What do we do when we consider Fourier harmonics of DVCS observables?
    double m_xB;    ///< Bjorken variable
    double m_t; ///< Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
    double m_Q2;  ///< Virtuality of the photon in Born approximation (in GeV^2)
    double m_E;  ///< Beam energy (in GeV)
    PhysicalType<double> m_phi; ///<  Angle between leptonic and hadronic planes (in degrees, Trento convention)
};

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        ObservableKinematic& observableKinematic);
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        ObservableKinematic& observableKinematic);

#endif /* OBSERVABLE_KINEMATIC_H */
