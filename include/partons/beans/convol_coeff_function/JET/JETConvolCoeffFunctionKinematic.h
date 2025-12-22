#ifndef JET_CONVOL_COEFF_FUNCTION_KINEMATIC_H
#define JET_CONVOL_COEFF_FUNCTION_KINEMATIC_H

/**
 * @file JETConvolCoeffFunctionKinematic.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 22, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../utils/type/PhysicalType.h"
#include "../../../utils/type/PhysicalUnit.h"
#include "../../../beans/JetType.h"
#include "../../../beans/JetCFFType.h"
#include "../ConvolCoeffFunctionKinematic.h"

namespace PARTONS {

/**
 * @class JETConvolCoeffFunctionKinematic
 *
 * @brief Class representing single CCF kinematics for jet production.
 *
 * This class represents a single CCF kinematics for jet production (xi, t, z, $q_{\perp}^2$,\f$Q^{2}\f$, \f$\mu_{F}^{2}\f$, \f$\mu_{F}^{2}\f$, jetType).
 */
class JETConvolCoeffFunctionKinematic: public ConvolCoeffFunctionKinematic {

public:

    static const std::string JET_CONVOL_COEFF_FUNCTION_KINEMATIC_CLASS_NAME; ///< Type of the kinematic in XML automation.

    /**
     * Parameter name to set meson polarization via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_JET_CFF_TYPE;

    /**
     * Default constructor.
     */
    JETConvolCoeffFunctionKinematic();

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param z Longitudinal "minus” momentum fraction carried by the parton forming the jet.
     * @param qPerp2 Square of transverse momentum of parton forming the jet.
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     * @param jetType Jet type.
     * @param jetCFFType CFF type.
     */
    JETConvolCoeffFunctionKinematic(double xi, double t, double z, double qPerp2, double Q2,
            double MuF2, double MuR2, JetType::Type jetType, JetCFFType::Type jetCFFType);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param z Longitudinal "minus” momentum fraction carried by the parton forming the jet.
     * @param qPerp2 Square of transverse momentum of parton forming the jet.
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     * @param jetType Jet type.
     * @param jetCFFType CFF type.
     */
    JETConvolCoeffFunctionKinematic(const PhysicalType<double> &xi,
            const PhysicalType<double> &t, const PhysicalType<double> &z, const PhysicalType<double> &qPerp2,
            const PhysicalType<double> &Q2, const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2,
            JetType::Type jetType, JetCFFType::Type jetCFFType);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param z Longitudinal "minus” momentum fraction carried by the parton forming the jet.
     * @param qPerp2 Square of transverse momentum of parton forming the jet.
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     * @param jetType Jet type.
     * @param jetCFFType CFF type.
     */
    JETConvolCoeffFunctionKinematic(const ElemUtils::GenericType &xi,
            const ElemUtils::GenericType &t,
            const ElemUtils::GenericType &z,
            const ElemUtils::GenericType &qPerp2,
            const ElemUtils::GenericType &Q2,
            const ElemUtils::GenericType &MuF2,
            const ElemUtils::GenericType &MuR2,
            JetType::Type jetType, JetCFFType::Type jetCFFType);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    JETConvolCoeffFunctionKinematic(
            const JETConvolCoeffFunctionKinematic &other);

    /**
     * Destructor.
     */
    virtual ~JETConvolCoeffFunctionKinematic();

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
    bool operator ==(const JETConvolCoeffFunctionKinematic& other) const;

    /**
     * Is different operator. Checks of values of kinematic variables are different.
     */
    bool operator !=(const JETConvolCoeffFunctionKinematic& other) const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

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
     * Get jet type.
     */
    JetType::Type getJetType() const;

    /**
     * Set jet type.
     */
    void setJetType(JetType::Type jetType);

    /**
     * Get jet CFF type.
     */
    JetCFFType::Type getJetCFFType() const;

    /**
     * Set jet CFF type.
     */
    void setJetCFFType(JetCFFType::Type jetCFFType);

protected:

    virtual void updateHashSum() const;

private:

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
     * Jet type.
     */
    JetType::Type m_jetType;

    /**
     * Jet CFF type.
     */
    JetCFFType::Type m_jetCFFType;
};

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        JETConvolCoeffFunctionKinematic& kinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        JETConvolCoeffFunctionKinematic& kinematic);

} /* namespace PARTONS */

#endif /* JET_CONVOL_COEFF_FUNCTION_KINEMATIC_H */
