#ifndef GAM2_CONVOL_COEFF_FUNCTION_KINEMATIC_H
#define GAM2_CONVOL_COEFF_FUNCTION_KINEMATIC_H

/**
 * @file GAM2ConvolCoeffFunctionKinematic.h
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
#include "../../PolarizationType.h"
#include "../ConvolCoeffFunctionKinematic.h"

namespace PARTONS {

/**
 * @class GAM2ConvolCoeffFunctionKinematic
 *
 * @brief Class representing single CCF kinematics for two photon production process.
 *
 * This class represents a single CCF kinematics for two photon production process (xi, t, \f$Q^{2}'\f$, \f$\mu_{F}^{2}\f$, \f$\mu_{F}^{2}\f$).
 */
class GAM2ConvolCoeffFunctionKinematic: public ConvolCoeffFunctionKinematic {

public:

    static const std::string GAM2_CONVOL_COEFF_FUNCTION_KINEMATIC_CLASS_NAME; ///< Type of the kinematic in XML automation.

    /**
     * Default constructor.
     */
    GAM2ConvolCoeffFunctionKinematic();

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param uPrim Four-momentum transfer squared to photon (in \f$GeV^{2}\f$).
     * @param Mgg2 Invariant mass of two photons (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     * @param polG0 Polarization of incoming photon.
     * @param polG1 Polarization of first outgoing photon.
     * @param polG2 Polarization of second outgoing photon.
     * @param phi Linear polarization angle.
     */
    GAM2ConvolCoeffFunctionKinematic(double xi, double t, double uPrim,
            double Mgg2, double MuF2, double MuR2, PolarizationType::Type polG0,
            PolarizationType::Type polG1, PolarizationType::Type polG2,
            double phi);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param uPrim Four-momentum transfer squared to photon (in \f$GeV^{2}\f$).
     * @param Mgg2 Invariant mass of two photons (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     * @param polG0 Polarization of incoming photon.
     * @param polG1 Polarization of first outgoing photon.
     * @param polG2 Polarization of second outgoing photon.
     * @param phi Linear polarization angle.
     */
    GAM2ConvolCoeffFunctionKinematic(const PhysicalType<double> &xi,
            const PhysicalType<double> &t, const PhysicalType<double> &uPrim,
            const PhysicalType<double> &Mgg2, const PhysicalType<double> &MuF2,
            const PhysicalType<double> &MuR2, PolarizationType::Type polG0,
            PolarizationType::Type polG1, PolarizationType::Type polG2,
            const PhysicalType<double> &phi);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param uPrim Four-momentum transfer squared to photon (in \f$GeV^{2}\f$).
     * @param Mgg2 Invariant mass of two photons (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     * @param polG0 Polarization of incoming photon.
     * @param polG1 Polarization of first outgoing photon.
     * @param polG2 Polarization of second outgoing photon.
     * @param phi Linear polarization angle.
     */
    GAM2ConvolCoeffFunctionKinematic(const ElemUtils::GenericType &xi,
            const ElemUtils::GenericType &t,
            const ElemUtils::GenericType &uPrim,
            const ElemUtils::GenericType &Mgg2,
            const ElemUtils::GenericType &MuF2,
            const ElemUtils::GenericType &MuR2, PolarizationType::Type polG0,
            PolarizationType::Type polG1, PolarizationType::Type polG2,
            const ElemUtils::GenericType &phi);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GAM2ConvolCoeffFunctionKinematic(
            const GAM2ConvolCoeffFunctionKinematic &other);

    /**
     * Destructor.
     */
    virtual ~GAM2ConvolCoeffFunctionKinematic();

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
    bool operator ==(const GAM2ConvolCoeffFunctionKinematic& other) const;

    /**
     * Is different operator. Checks of values of kinematic variables are different.
     */
    bool operator !=(const GAM2ConvolCoeffFunctionKinematic& other) const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

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
     * Get polarization state of incoming photon.
     */
    PolarizationType::Type getPolG0() const;

    /**
     * Set polarization state of incoming photon.
     */
    void setPolG0(PolarizationType::Type polG0);

    /**
     * Get polarization state of first outgoing photon.
     */
    PolarizationType::Type getPolG1() const;

    /**
     * Set polarization state of first outgoing photon.
     */
    void setPolG1(PolarizationType::Type polG1);

    /**
     * Get polarization state of second outgoing photon.
     */
    PolarizationType::Type getPolG2() const;

    /**
     * Set polarization state of second outgoing photon.
     */
    void setPolG2(PolarizationType::Type polG2);

    /**
     * Get linear polarization angle.
     */
    const PhysicalType<double>& getPhi() const;

    /**
     * Set linear polarization angle.
     */
    void setPhi(const PhysicalType<double>& Phi);

    /**
     * Set linear polarization angle.
     */
    void setPhi(double Phi, PhysicalUnit::Type unit = PhysicalUnit::RAD);

protected:

    virtual void updateHashSum() const;

private:

    /**
     * Four-momentum transfer squared to photon (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_uPrim;

    /**
     * Invariant mass of two photons (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_Mgg2;

    PolarizationType::Type m_polG0; ///< Polarization state of incoming photon.
    PolarizationType::Type m_polG1; ///< Polarization state of first outgoing photon.
    PolarizationType::Type m_polG2; ///< Polarization state of second outgoing photon.

    /**
     * Linear polarization angle.
     */
    PhysicalType<double> m_phi;
};

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        GAM2ConvolCoeffFunctionKinematic& kinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        GAM2ConvolCoeffFunctionKinematic& kinematic);

} /* namespace PARTONS */

#endif /* GAM2_CONVOL_COEFF_FUNCTION_KINEMATIC_H */
