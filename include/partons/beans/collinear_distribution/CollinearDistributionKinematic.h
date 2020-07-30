#ifndef COLLINEAR_DISTRIBUTION_KINEMATIC_H
#define COLLINEAR_DISTRIBUTION_KINEMATIC_H

/**
 * @file CollinearDistributionKinematic.h
 * @author: Valerio BERTONE (CEA Saclay)
 * @date July 17, 2020
 * @version 1.0
 */

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../utils/type/PhysicalType.h"
#include "../../utils/type/PhysicalUnit.h"
#include "../Kinematic.h"

namespace ElemUtils {
class Packet;
} /* namespace ElemUtils */

namespace PARTONS {

/**
 * @class CollinearDistributionKinematic
 *
 * @brief Class representing a single collinear-distribution kinematics.
 *
 * This class represents a single collinear-distribution kinematics (x, \f$\mu_{F}^{2}\f$, \f$\mu_{F}^{2}\f$).
 */
class CollinearDistributionKinematic: public Kinematic {

public:

    static const std::string COLLINEAR_DISTRIBUTION_KINEMATIC_CLASS_NAME; ///< Type of the kinematic in XML automation.

    /**
     * Parameter name to set variable \f$x\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_X;

    /**
     * Parameter name to set variable \f$\mu_{F}^{2}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_MUF2;

    /**
     * Parameter name to set variable \f$\mu_{R}^{2}\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_MUR2;

    /**
     * Parameter name to set unit of variable \f$x\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_X_UNIT;

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
    CollinearDistributionKinematic();

    /**
     * Assignment constructor.
     *
     * @param x Longitudinal momentum fraction of active parton.
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    CollinearDistributionKinematic(double x, double MuF2, double MuR2);

    /**
     * Assignment constructor.
     *
     * @param x Longitudinal momentum fraction of active parton.
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    CollinearDistributionKinematic(const PhysicalType<double> &x, const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2);

    /**
     * Assignment constructor.
     *
     * @param x Longitudinal momentum fraction of active parton.
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    CollinearDistributionKinematic(const ElemUtils::GenericType &x, const ElemUtils::GenericType &MuF2, const ElemUtils::GenericType &MuR2);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    CollinearDistributionKinematic(const CollinearDistributionKinematic &other);

    /**
     * Destructor.
     */
    virtual ~CollinearDistributionKinematic();

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
    bool operator ==(const CollinearDistributionKinematic& other) const;

    /**
     * Is different operator. Checks of values of kinematic variables are different.
     */
    bool operator !=(const CollinearDistributionKinematic& other) const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get longitudinal momentum fraction of active parton.
     */
    const PhysicalType<double>& getX() const;

    /**
     * Set longitudinal momentum fraction of active parton.
     */
    void setX(const PhysicalType<double>& x);

    /**
     * Set longitudinal momentum fraction of active parton.
     */
    void setX(double x, PhysicalUnit::Type unit = PhysicalUnit::NONE);

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
     * Longitudinal momentum fraction of active parton.
     */
    PhysicalType<double> m_x;

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
 * Stream operator to serialize class into Packet. See also CollinearDistributionType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet, CollinearDistributionKinematic& kinematic);

/**
 * Stream operator to retrieve class from Packet. See also CollinearDistributionType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet, CollinearDistributionKinematic& kinematic);

} /* namespace PARTONS */

#endif /* COLLINEAR_DISTRIBUTION_KINEMATIC_H */
