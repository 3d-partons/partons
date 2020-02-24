#ifndef GPD_SUBTRACTION_CONSTANT_KINEMATIC_H
#define GPD_SUBTRACTION_CONSTANT_KINEMATIC_H

/**
 * @file GPDSubtractionConstantKinematic.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 26, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../utils/type/PhysicalType.h"
#include "../../utils/type/PhysicalUnit.h"
#include "../Kinematic.h"

namespace ElemUtils {
class Packet;
} /* namespace ElemUtils */

namespace PARTONS {

/**
 * @class GPDSubtractionConstantKinematic
 *
 * @brief Class representing single GPD subtraction constant kinematics.
 *
 * This class represents a single GPD kinematics (t, \f$\mu_{F}^{2}\f$, \f$\mu_{F}^{2}\f$).
 */
class GPDSubtractionConstantKinematic: public Kinematic {

public:

    static const std::string GPD_SUBTRACTION_CONSTANT_KNEMATIC_CLASS_NAME; ///< Type of the kinematic in XML automation.

    /**
     * Parameter name to set variable \f$t\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_T;

    /**
     * Parameter name to set variable \f$\mu_{F}^{2}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_MUF2;

    /**
     * Parameter name to set variable \f$\mu_{R}^{2}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_MUR2;

    /**
     * Parameter name to set unit of variable \f$t\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_T_UNIT;

    /**
     * Parameter name to set unit of variable \f$\mu_{F}^{2}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_MUF2_UNIT;

    /**
     * Parameter name to set unit of variable \f$\mu_{R}^{2}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_MUR2_UNIT;

    /**
     * Default constructor.
     */
    GPDSubtractionConstantKinematic();

    /**
     * Assignment constructor.
     *
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    GPDSubtractionConstantKinematic(double t, double MuF2, double MuR2);

    /**
     * Assignment constructor.
     *
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    GPDSubtractionConstantKinematic(const PhysicalType<double> &t,
            const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2);

    /**
     * Assignment constructor.
     *
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    GPDSubtractionConstantKinematic(const ElemUtils::GenericType &t,
            const ElemUtils::GenericType &MuF2,
            const ElemUtils::GenericType &MuR2);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GPDSubtractionConstantKinematic(
            const GPDSubtractionConstantKinematic &other);

    /**
     * Destructor.
     */
    virtual ~GPDSubtractionConstantKinematic();

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
     * Is equal operator. Checks if values of kinematic variables are the same.
     */
    bool operator ==(const GPDSubtractionConstantKinematic& other) const;

    /**
     * Is different operator. Checks of values of kinematic variables are different.
     */
    bool operator !=(const GPDSubtractionConstantKinematic& other) const;

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
     * Get factorization scale squared.
     */
    const PhysicalType<double>& getMuF2() const;

    /**
     * Set factorization scale squared.
     */
    void setMuF2(const PhysicalType<double>& muF2);

    /**
     * Set factorization scale squared.
     */
    void setMuF2(double muF2, PhysicalUnit::Type unit = PhysicalUnit::GEV2);

    /**
     * Get renormalization scale squared.
     */
    const PhysicalType<double>& getMuR2() const;

    /**
     * Set renormalization scale squared.
     */
    void setMuR2(const PhysicalType<double>& muR2);

    /**
     * Set renormalization scale squared.
     */
    void setMuR2(double muR2, PhysicalUnit::Type unit = PhysicalUnit::GEV2);

protected:

    virtual void updateHashSum() const;

private:

    /**
     * Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_t;

    /**
     * Factorization scale squared (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_MuF2;

    /**
     * Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_MuR2;
};

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        GPDSubtractionConstantKinematic& kinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        GPDSubtractionConstantKinematic& kinematic);

} /* namespace PARTONS */

#endif /* GPD_SUBTRACTION_CONSTANT_KINEMATIC_H */
