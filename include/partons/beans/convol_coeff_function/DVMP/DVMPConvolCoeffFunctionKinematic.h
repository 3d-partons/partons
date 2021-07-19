#ifndef DVMP_CONVOL_COEFF_FUNCTION_KINEMATIC_H
#define DVMP_CONVOL_COEFF_FUNCTION_KINEMATIC_H

/**
 * @file DVMPConvolCoeffFunctionKinematic.h
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
#include "../../MesonPolarization.h"
#include "../../MesonType.h"
#include "../ConvolCoeffFunctionKinematic.h"

namespace PARTONS {

/**
 * @class DVMPConvolCoeffFunctionKinematic
 *
 * @brief Class representing single CCF kinematics for DVMP process.
 *
 * This class represents a single CCF kinematics for DVMP process (xi, t, \f$Q^{2}\f$, \f$\mu_{F}^{2}\f$, \f$\mu_{F}^{2}\f$, mesonType, mesonPolarization).
 */
class DVMPConvolCoeffFunctionKinematic: public ConvolCoeffFunctionKinematic {

public:

    static const std::string DVMP_CONVOL_COEFF_FUNCTION_KNEMATIC_CLASS_NAME; ///< Type of the kinematic in XML automation.

    /**
     * Parameter name to set meson polarization via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_MESON_POLARIZATION;

    /**
     * Parameter name to set unit of meson polarization via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_MESON_POLARIZATION_UNIT;

    /**
     * Default constructor.
     */
    DVMPConvolCoeffFunctionKinematic();

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     * @param mesonType Meson type.
     * @param mesonPolarization Meson polarization.
     */
    DVMPConvolCoeffFunctionKinematic(double xi, double t, double Q2,
            double MuF2, double MuR2, MesonType::Type mesonType,
            MesonPolarization::Type mesonPolarization);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     * @param mesonType Meson type.
     * @param mesonPolarization Meson polarization.
     */
    DVMPConvolCoeffFunctionKinematic(const PhysicalType<double> &xi,
            const PhysicalType<double> &t, const PhysicalType<double> &Q2,
            const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2,
            MesonType::Type mesonType,
            MesonPolarization::Type mesonPolarization);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     * @param mesonType Meson type.
     * @param mesonPolarization Meson polarization.
     */
    DVMPConvolCoeffFunctionKinematic(const ElemUtils::GenericType &xi,
            const ElemUtils::GenericType &t, const ElemUtils::GenericType &Q2,
            const ElemUtils::GenericType &MuF2,
            const ElemUtils::GenericType &MuR2, MesonType::Type mesonType,
            MesonPolarization::Type mesonPolarization);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPConvolCoeffFunctionKinematic(
            const DVMPConvolCoeffFunctionKinematic &other);

    /**
     * Destructor.
     */
    virtual ~DVMPConvolCoeffFunctionKinematic();

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
    bool operator ==(const DVMPConvolCoeffFunctionKinematic& other) const;

    /**
     * Is different operator. Checks of values of kinematic variables are different.
     */
    bool operator !=(const DVMPConvolCoeffFunctionKinematic& other) const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

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
     * Get meson type.
     */
    MesonType::Type getMesonType() const;

    /**
     * Set meson type.
     */
    void setMesonType(MesonType::Type mesonType);

    /**
     * Get meson polarization.
     */
    MesonPolarization::Type getMesonPolarization() const;

    /**
     * Set meson polarization.
     */
    void setMesonPolarization(MesonPolarization::Type mesonPolarization);

protected:

    virtual void updateHashSum() const;

private:

    /**
     * Virtual-photon virtuality (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_Q2;

    /**
     * Meson type.
     */
    MesonType::Type m_mesonType;

    /**
     * Meson polarization.
     */
    MesonPolarization::Type m_mesonPolarization;
};

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        DVMPConvolCoeffFunctionKinematic& kinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        DVMPConvolCoeffFunctionKinematic& kinematic);

} /* namespace PARTONS */

#endif /* DVMP_CONVOL_COEFF_FUNCTION_KINEMATIC_H */
