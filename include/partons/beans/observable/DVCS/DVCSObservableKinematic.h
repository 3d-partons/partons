#ifndef DVCS_OBSERVABLE_KINEMATIC_H
#define DVCS_OBSERVABLE_KINEMATIC_H

/**
 * @file DVCSObservableKinematic.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 November 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "../ObservableKinematic.h"

namespace PARTONS {

/**
 * @class DVCSObservableKinematic
 *
 * @brief Class representing single observable kinematics for DVCS process.
 *
 * This class represents a single observable kinematics for DVCS process (x_{B}, t, \f$Q^{2}\f$, E_{b}, \f$\phi\f$).
 */
class DVCSObservableKinematic: public ObservableKinematic {

public:

    /**
     * Parameter name to set variable \f$x_{B}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_XB;

    /**
     * Parameter name to set variable \f$Q^{2}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_Q2;

    /**
     * Parameter name to set variable \f$E_{b}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_BEAM_ENERGY;

    /**
     * Parameter name to set variable \f$\phi\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_PHI;

    /**
     * Default constructor.
     */
    DVCSObservableKinematic();

    /**
     * Assignment constructor.
     * @param parameters Parameters object storing values to be set marked by DVCSObservableKinematic::PARAMETER_NAME_XB, DVCSObservableKinematic::PARAMETER_NAME_T, DVCSObservableKinematic::PARAMETER_NAME_Q2, DVCSObservableKinematic::PARAMETER_NAME_BEAM_ENERGY and DVCSObservableKinematic::PARAMETER_NAME_PHI.
     */
    DVCSObservableKinematic(const ElemUtils::Parameters &parameters);

    /**
     * Assignment constructor.
     * @param xB Bjorken variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in degrees, Trento convention).
     */
    DVCSObservableKinematic(double xB, double t, double Q2, double E,
            double phi);

    /**
     * Assignment constructor.
     * @param xB Bjorken variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in degrees, Trento convention).
     */
    DVCSObservableKinematic(const PhysicalType<double>& xB,
            const PhysicalType<double>& t, const PhysicalType<double>& Q2,
            const PhysicalType<double>& E, const PhysicalType<double>& phi);

    /**
     * Assignment constructor.
     * @param xB Bjorken variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in degrees, Trento convention).
     */
    DVCSObservableKinematic(const ElemUtils::GenericType& xB,
            const ElemUtils::GenericType& t, const ElemUtils::GenericType& Q2,
            const ElemUtils::GenericType& E, const ElemUtils::GenericType& phi);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSObservableKinematic(const DVCSObservableKinematic &other);

    /**
     * Destructor.
     */
    virtual ~DVCSObservableKinematic();

    virtual std::string toString() const;

    /**
     * Serialize into given Packet.
     * @param packet Target Packet.
     */
    void serialize(ElemUtils::Packet &packet) const;

    /**
     * Retrieve data from given Packet.
     * @param packet Input Packet.
     */
    void unserialize(ElemUtils::Packet &packet);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get Bjorken variable.
     */
    PhysicalType<double> getXB() const;

    /**
     * Set Bjorken variable.
     */
    void setXB(double xB);

    /**
     * Get four-momentum transfer squared of hadron target.
     */
    PhysicalType<double> getT() const;

    /**
     * Set four-momentum transfer squared of hadron target.
     */
    void setT(double t);

    /**
     * Get virtual-photon virtuality.
     */
    PhysicalType<double> getQ2() const;

    /**
     * Set virtual-photon virtuality.
     */
    void setQ2(double Q2);

    /**
     * Get beam energy.
     */
    PhysicalType<double> getE() const;

    /**
     * Set beam energy.
     */
    void setE(double E);

    /**
     * Get angle between leptonic and hadronic planes.
     */
    PhysicalType<double> getPhi() const;

    /**
     * Set angle between leptonic and hadronic planes.
     */
    void setPhi(double phi);

protected:

    virtual void updateHashSum() const;

private:

    /**
     * Bjorken variable.
     */
    PhysicalType<double> m_xB;

    /**
     * Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_t;

    /**
     * Virtual-photon virtuality (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_Q2;

    /**
     * Beam energy (in GeV).
     */
    PhysicalType<double> m_E;

    /**
     * Angle between leptonic and hadronic planes (in degrees, Trento convention).
     */
    PhysicalType<double> m_phi;
};

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        DVCSObservableKinematic& DVCSObservableKinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        DVCSObservableKinematic& DVCSObservableKinematic);

} /* namespace PARTONS */

#endif /* DVCS_OBSERVABLE_KINEMATIC_H */
