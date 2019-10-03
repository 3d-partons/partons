#ifndef TCS_OBSERVABLE_KINEMATIC_H
#define TCS_OBSERVABLE_KINEMATIC_H

/**
 * @file TCSObservableKinematic.h
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
#include "../ObservableKinematic.h"

namespace PARTONS {

/**
 * @class TCSObservableKinematic
 *
 * @brief Class representing single observable kinematics for TCS process.
 *
 * This class represents a single observable kinematics for TCS process (t, \f$Q^{2}\f$, E_{b}, \f$\phi\f$, \f$\theta\f$).
 */
class TCSObservableKinematic: public ObservableKinematic {

public:

    static const std::string TCS_OBSERVABLE_KNEMATIC_CLASS_NAME; ///< Type of the kinematic in XML automation.

    /**
     * Parameter name to set variable \f$Q^{2}'\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_Q2PRIM;

    /**
     * Parameter name to set variable \f$E_{b}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_BEAM_ENERGY;

    /**
     * Parameter name to set variable \f$\phi\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_PHI;

    /**
     * Parameter name to set variable \f$\theta\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_THETA;

    /**
     * Parameter name to set unit of variable \f$Q^{2}'\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_Q2PRIM_UNIT;

    /**
     * Parameter name to set unit of variable \f$E_{b}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_BEAM_ENERGY_UNIT;

    /**
     * Parameter name to set unit of variable \f$\phi\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_PHI_UNIT;

    /**
     * Parameter name to set unit of variable \f$\theta\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_THETA_UNIT;

    /**
     * Default constructor.
     */
    TCSObservableKinematic();

    /**
     * Assignment constructor.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2Prim Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in radians, Trento convention).
     * @param theta Angle between positively charged lepton and scattered proton in lepton CMS (in radians).
     */
    TCSObservableKinematic(double t, double Q2Prim, double E, double phi,
            double theta);

    /**
     * Assignment constructor.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2Prim Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in radians, Trento convention).
     * @param theta Angle between positively charged lepton and scattered proton in lepton CMS (in radians).
     */
    TCSObservableKinematic(const PhysicalType<double>& t,
            const PhysicalType<double>& Q2Prim, const PhysicalType<double>& E,
            const PhysicalType<double>& phi, const PhysicalType<double>& theta);

    /**
     * Assignment constructor.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2Prim Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in radians, Trento convention).
     * @param theta Angle between positively charged lepton and scattered proton in lepton CMS (in radians).
     */
    TCSObservableKinematic(const ElemUtils::GenericType& t,
            const ElemUtils::GenericType& Q2Prim,
            const ElemUtils::GenericType& E, const ElemUtils::GenericType& phi,
            const ElemUtils::GenericType& theta);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSObservableKinematic(const TCSObservableKinematic &other);

    /**
     * Destructor.
     */
    virtual ~TCSObservableKinematic();

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
    bool operator ==(const TCSObservableKinematic& other) const;

    /**
     * Is different operator. Checks of values of kinematic variables are different.
     */
    bool operator !=(const TCSObservableKinematic& other) const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

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
     * Get outgoing virtual-photon virtuality.
     */
    const PhysicalType<double>& getQ2Prim() const;

    /**
     * Set outgoing virtual-photon virtuality.
     */
    void setQ2Prim(const PhysicalType<double>& Q2Prim);

    /**
     * Set outgoing virtual-photon virtuality.
     */
    void setQ2Prim(double Q2Prim, PhysicalUnit::Type unit = PhysicalUnit::GEV2);

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
     * Get angle between positively charged lepton and scattered proton in lepton CMS (in radians).
     */
    const PhysicalType<double>& getTheta() const;

    /**
     * Set angle between positively charged lepton and scattered proton in lepton CMS (in radians).
     */
    void setTheta(const PhysicalType<double>& theta);

    /**
     * Set angle between positively charged lepton and scattered proton in lepton CMS (in radians).
     */
    void setTheta(double theta, PhysicalUnit::Type unit = PhysicalUnit::RAD);

protected:

    virtual void updateHashSum() const;

private:

    /**
     * Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_t;

    /**
     * Virtual-photon virtuality (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_Q2Prim;

    /**
     * Beam energy (in GeV).
     */
    PhysicalType<double> m_E;

    /**
     * Angle between leptonic and hadronic planes (in radians, Trento convention).
     */
    PhysicalType<double> m_phi;

    /**
     * Angle between positively charged lepton and scattered proton in lepton CMS (in radians).
     */
    PhysicalType<double> m_theta;
};

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        TCSObservableKinematic& TCSObservableKinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        TCSObservableKinematic& TCSObservableKinematic);

} /* namespace PARTONS */

#endif /* TCS_OBSERVABLE_KINEMATIC_H */
