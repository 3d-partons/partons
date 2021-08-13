#ifndef GAM2_OBSERVABLE_KINEMATIC_H
#define GAM2_OBSERVABLE_KINEMATIC_H

/**
 * @file GAM2ObservableKinematic.h
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
 * @class GAM2ObservableKinematic
 *
 * @brief Class representing single observable kinematics for two photon production process.
 *
 * This class represents a single observable kinematics for two photon production process (t, \f$Q^{2}\f$, E_{b}, \f$\phi\f$, \f$\theta\f$).
 */
class GAM2ObservableKinematic: public ObservableKinematic {

public:

    static const std::string GAM2_OBSERVABLE_KINEMATIC_CLASS_NAME; ///< Type of the kinematic in XML automation.

    /**
     * Parameter name to set variable \f$u'\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_UPRIM;

    /**
     * Parameter name to set variable \f$M_{\gamma\gamma}^{2}'\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_MGG2;

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
     * Parameter name to set unit of variable \f$u'\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_UPRIM_UNIT;

    /**
     * Parameter name to set unit of variable \f$M_{\gamma\gamma}^{2}'\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_MGG2_UNIT;

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
    GAM2ObservableKinematic();

    /**
     * Assignment constructor.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param uPrim Four-momentum transfer squared to photon (in \f$GeV^{2}\f$).
     * @param Mgg2 Invariant mass of two photons (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in radians, Trento convention).
     * @param theta Angle between positively charged lepton and scattered proton in lepton CMS (in radians).
     */
    GAM2ObservableKinematic(double t, double uPrim, double Mgg2, double E,
            double phi, double theta);

    /**
     * Assignment constructor.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param uPrim Four-momentum transfer squared to photon (in \f$GeV^{2}\f$).
     * @param Mgg2 Invariant mass of two photons (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in radians, Trento convention).
     * @param theta Angle between positively charged lepton and scattered proton in lepton CMS (in radians).
     */
    GAM2ObservableKinematic(const PhysicalType<double>& t,
            const PhysicalType<double>& uPrim, const PhysicalType<double>& Mgg2,
            const PhysicalType<double>& E, const PhysicalType<double>& phi,
            const PhysicalType<double>& theta);

    /**
     * Assignment constructor.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param uPrim Four-momentum transfer squared to photon (in \f$GeV^{2}\f$).
     * @param Mgg2 Invariant mass of two photons (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in radians, Trento convention).
     * @param theta Angle between positively charged lepton and scattered proton in lepton CMS (in radians).
     */
    GAM2ObservableKinematic(const ElemUtils::GenericType& t,
            const ElemUtils::GenericType& uPrim,
            const ElemUtils::GenericType& Mgg2, const ElemUtils::GenericType& E,
            const ElemUtils::GenericType& phi,
            const ElemUtils::GenericType& theta);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GAM2ObservableKinematic(const GAM2ObservableKinematic &other);

    /**
     * Destructor.
     */
    virtual ~GAM2ObservableKinematic();

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
    bool operator ==(const GAM2ObservableKinematic& other) const;

    /**
     * Is different operator. Checks of values of kinematic variables are different.
     */
    bool operator !=(const GAM2ObservableKinematic& other) const;

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
     * Get four-momentum transfer squared to photon.
     */
    const PhysicalType<double>& getUPrim() const;

    /**
     * Set four-momentum transfer squared to photon.
     */
    void setUPrim(const PhysicalType<double>& uPrim);

    /**
     * Set four-momentum transfer squared to photon.
     */
    void setUPrim(double uPrim, PhysicalUnit::Type unit = PhysicalUnit::GEV2);

    /**
     * Get invariant mass of two photons.
     */
    const PhysicalType<double>& getMgg2() const;

    /**
     * Set invariant mass of two photons.
     */
    void setMgg2(const PhysicalType<double>& Mgg2);

    /**
     * Set invariant mass of two photons.
     */
    void setMgg2(double Mgg2, PhysicalUnit::Type unit = PhysicalUnit::GEV2);

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
     * Four-momentum transfer squared to photon (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_uPrim;

    /**
     * Invariant mass of two photons (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_Mgg2;

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
        GAM2ObservableKinematic& GAM2ObservableKinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        GAM2ObservableKinematic& GAM2ObservableKinematic);

} /* namespace PARTONS */

#endif /* GAM2_OBSERVABLE_KINEMATIC_H */
