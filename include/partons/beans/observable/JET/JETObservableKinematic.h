#ifndef JET_OBSERVABLE_KINEMATIC_H
#define JET_OBSERVABLE_KINEMATIC_H

/**
 * @file JETObservableKinematic.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 November 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../utils/type/PhysicalType.h"
#include "../../../utils/type/PhysicalUnit.h"
#include "../../../beans/JetType.h"
#include "../ObservableKinematic.h"

namespace PARTONS {

/**
 * @class JETObservableKinematic
 *
 * @brief Class representing single observable kinematics for JET process.
 *
 * This class represents a single observable kinematics for JET process (x_{B}, t, z, \f$q_{\perp}^{2}\f$, \f$Q^{2}\f$, E_{b}, \f$\phi\f$).
 */
class JETObservableKinematic: public ObservableKinematic {

public:

    static const std::string JET_OBSERVABLE_KINEMATIC_CLASS_NAME; ///< Type of the kinematic in XML automation.

    /**
     * Parameter name to set variable \f$x_{B}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_XB;

    /**
     * Parameter name to set variable \f$z\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_Z;

    /**
     * Parameter name to set variable \f$q_{\perp}^{2}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_QPERP2;

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
     * Parameter name to set unit of variable \f$x_{B}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_XB_UNIT;

    /**
     * Parameter name to set unit of variable \f$z\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_Z_UNIT;

    /**
     * Parameter name to set unit of variable \f$q_{\perp}^{2}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_QPERP2_UNIT;

    /**
     * Parameter name to set unit of variable \f$Q^{2}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_Q2_UNIT;

    /**
     * Parameter name to set unit of variable \f$E_{b}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_BEAM_ENERGY_UNIT;

    /**
     * Parameter name to set unit of variable \f$\phi\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_PHI_UNIT;

    /**
     * Parameter name to set jet type.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_JET_TYPE;

    /**
     * Default constructor.
     */
    JETObservableKinematic();

    /**
     * Assignment constructor.
     * @param xB Bjorken variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param z Longitudinal "minus” momentum fraction carried by the parton forming the jet.
     * @param qPerp2 Square of transverse momentum of parton forming the jet.
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in radians, Trento convention).
     * @param jetType Jet type.
     */
    JETObservableKinematic(double xB, double t, double z, double qPerp2, double Q2, double E,
            double phi, JetType::Type jetType);

    /**
     * Assignment constructor.
     * @param xB Bjorken variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param z Longitudinal "minus” momentum fraction carried by the parton forming the jet.
     * @param qPerp2 Square of transverse momentum of parton forming the jet.
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in radians, Trento convention).
     * @param jetType Jet type.
     */
    JETObservableKinematic(const PhysicalType<double>& xB,
            const PhysicalType<double>& t, const PhysicalType<double>& z,
            const PhysicalType<double>& qPerp2, const PhysicalType<double>& Q2,
            const PhysicalType<double>& E, const PhysicalType<double>& phi, JetType::Type jetType);

    /**
     * Assignment constructor.
     * @param xB Bjorken variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param z Longitudinal "minus” momentum fraction carried by the parton forming the jet.
     * @param qPerp2 Square of transverse momentum of parton forming the jet.
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in radians, Trento convention).
     * @param jetType Jet type.
     */
    JETObservableKinematic(const ElemUtils::GenericType& xB,
            const ElemUtils::GenericType& t, const ElemUtils::GenericType& z,
            const ElemUtils::GenericType& qPerp2, const ElemUtils::GenericType& Q2,
            const ElemUtils::GenericType& E, const ElemUtils::GenericType& phi, JetType::Type jetType);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    JETObservableKinematic(const JETObservableKinematic &other);

    /**
     * Destructor.
     */
    virtual ~JETObservableKinematic();

    virtual void configure(const ElemUtils::Parameters &parameters);
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

    /**
     * Serialize to std::vector<double>.
     */
    void serializeIntoStdVector(std::vector<double>& vec) const;

    /**
     * Unserialize from std::vector<double>.
     */
    void unserializeFromStdVector(std::vector<double>::const_iterator& it,
            const std::vector<double>::const_iterator& end);

    /**
     * Is equal operator. Checks if values of kinematic variables are the same.
     */
    bool operator ==(const JETObservableKinematic& other) const;

    /**
     * Is different operator. Checks of values of kinematic variables are different.
     */
    bool operator !=(const JETObservableKinematic& other) const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get Bjorken variable.
     */
    const PhysicalType<double>& getXB() const;

    /**
     * Set Bjorken variable.
     */
    void setXB(const PhysicalType<double>& xB);

    /**
     * Set Bjorken variable.
     */
    void setXB(double xB, PhysicalUnit::Type unit = PhysicalUnit::NONE);

    /**
     * Get four-momentum transfer squared of hadron target.
     */
    const PhysicalType<double>& getT() const;

    /**
     * Set four-momentum transfer squared of hadron target.
     */
    void setT(const PhysicalType<double>& t);

    /**
     * Set four-momentum transfer squared of hadron target.
     */
    void setT(double t, PhysicalUnit::Type unit = PhysicalUnit::GEV2);

    /**
    * Get longitudinal "minus” momentum fraction carried by the parton forming the jet.
    */
    const PhysicalType<double>& getZ() const;

    /**
     * Set longitudinal "minus” momentum fraction carried by the parton forming the jet.
     */
    void setZ(const PhysicalType<double>& z);

    /**
     * Set longitudinal "minus” momentum fraction carried by the parton forming the jet.
     */
    void setZ(double z, PhysicalUnit::Type unit = PhysicalUnit::NONE);

    /**
     * Get square of transverse momentum of parton forming the jet.
     */
    const PhysicalType<double>& getQPerp2() const;

    /**
     * Set square of transverse momentum of parton forming the jet.
     */
    void setQPerp2(const PhysicalType<double>& qPerp2);

    /**
     * Set square of transverse momentum of parton forming the jet.
     */
    void setQPerp2(double qPerp2, PhysicalUnit::Type unit = PhysicalUnit::GEV2);

    /**
     * Get virtual-photon virtuality.
     */
    const PhysicalType<double>& getQ2() const;

    /**
     * Set virtual-photon virtuality.
     */
    void setQ2(const PhysicalType<double>& Q2);

    /**
     * Set virtual-photon virtuality.
     */
    void setQ2(double Q2, PhysicalUnit::Type unit = PhysicalUnit::GEV2);

    /**
     * Get beam energy.
     */
    const PhysicalType<double>& getE() const;

    /**
     * Set beam energy.
     */
    void setE(const PhysicalType<double>& E);

    /**
     * Set beam energy.
     */
    void setE(double E, PhysicalUnit::Type unit = PhysicalUnit::GEV);

    /**
     * Get angle between leptonic and hadronic planes (in radians).
     */
    const PhysicalType<double>& getPhi() const;

    /**
     * Set angle between leptonic and hadronic planes (in radians).
     */
    void setPhi(const PhysicalType<double>& phi);

    /**
     * Set angle between leptonic and hadronic planes (in radians).
     */
    void setPhi(double phi, PhysicalUnit::Type unit = PhysicalUnit::RAD);

    /**
     * Get jet type.
     */
    JetType::Type getJetType() const;

    /**
     * Set jet type.
     */
    void setJetType(JetType::Type jetType);

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
     * Longitudinal "minus” momentum fraction carried by the parton forming the jet.
     */
    PhysicalType<double> m_z;

    /**
     * Square of transverse momentum of parton forming the jet (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_qPerp2;

    /**
     * Virtual-photon virtuality (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_Q2;

    /**
     * Beam energy (in GeV).
     */
    PhysicalType<double> m_E;

    /**
     * Angle between leptonic and hadronic planes (in radians, Trento convention).
     */
    PhysicalType<double> m_phi;

    /**
     * Jet type.
     */
    JetType::Type m_jetType;
};

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        JETObservableKinematic& JETObservableKinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        JETObservableKinematic& JETObservableKinematic);

} /* namespace PARTONS */

#endif /* JET_OBSERVABLE_KINEMATIC_H */
