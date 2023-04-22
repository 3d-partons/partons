#ifndef DDVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_H
#define DDVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_H

/**
 * @file DDVCSConvolCoeffFunctionKinematic.h
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
#include "../ConvolCoeffFunctionKinematic.h"

namespace PARTONS {

/**
 * @class DDVCSConvolCoeffFunctionKinematic
 *
 * @brief Class representing single CCF kinematics for DDVCS process.
 *
 * This class represents a single CCF kinematics for DDVCS process (xi, t, \f$Q^{2}\f$, \f$Q'^{2}\f$, \f$\mu_{F}^{2}\f$, \f$\mu_{F}^{2}\f$).
 */
class DDVCSConvolCoeffFunctionKinematic: public ConvolCoeffFunctionKinematic {

public:

    static const std::string DDVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_CLASS_NAME; ///< Type of the kinematic in XML automation.

    /**
     * Default constructor.
     */
    DDVCSConvolCoeffFunctionKinematic();

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param Q2Prim Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    DDVCSConvolCoeffFunctionKinematic(double xi, double t, double Q2,
            double Q2Prim, double MuF2, double MuR2);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param Q2Prim Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    DDVCSConvolCoeffFunctionKinematic(const PhysicalType<double> &xi,
            const PhysicalType<double> &t, const PhysicalType<double> &Q2,
            const PhysicalType<double> &Q2Prim,
            const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param Q2Prim Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    DDVCSConvolCoeffFunctionKinematic(const ElemUtils::GenericType &xi,
            const ElemUtils::GenericType &t, const ElemUtils::GenericType &Q2,
            const ElemUtils::GenericType &Q2Prim,
            const ElemUtils::GenericType &MuF2,
            const ElemUtils::GenericType &MuR2);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DDVCSConvolCoeffFunctionKinematic(
            const DDVCSConvolCoeffFunctionKinematic &other);

    /**
     * Destructor.
     */
    virtual ~DDVCSConvolCoeffFunctionKinematic();

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
    bool operator ==(const DDVCSConvolCoeffFunctionKinematic& other) const;

    /**
     * Is different operator. Checks of values of kinematic variables are different.
     */
    bool operator !=(const DDVCSConvolCoeffFunctionKinematic& other) const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

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

protected:

    virtual void updateHashSum() const;

private:

    /**
     * Virtual-photon virtuality (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_Q2;

    /**
     * Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_Q2Prim;

};

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        DDVCSConvolCoeffFunctionKinematic& kinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        DDVCSConvolCoeffFunctionKinematic& kinematic);

} /* namespace PARTONS */

#endif /* DDVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_H */
