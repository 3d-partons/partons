#ifndef GPD_KINEMATIC_H
#define GPD_KINEMATIC_H

/**
 * @file GPDKinematic.h
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
 * @class GPDKinematic
 *
 * @brief Class representing single GPD kinematics.
 *
 * This class represents a single GPD kinematics (x, xi, t, \f$\mu_{F}^{2}\f$, \f$\mu_{F}^{2}\f$).
 */
class GPDKinematic: public Kinematic {

public:

    static const std::string GPD_KNEMATIC_CLASS_NAME; ///< Type of the kinematic in XML automation.

    /**
     * Parameter name to set variable \f$x\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_X;

    /**
     * Parameter name to set variable \f$\xi\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_XI;

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
     * Parameter name to set unit of variable \f$x\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_X_UNIT;

    /**
     * Parameter name to set unit of variable \f$\xi\f$ via configuration methods.
     */
    static const std::string KINEMATIC_PARAMETER_NAME_XI_UNIT;

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
    GPDKinematic();

    /**
     * Assignment constructor.
     * @param parameters Parameters object storing values to be set marked by GPDKinematic::KINEMATIC_PARAMETER_NAME_X, GPDKinematic::KINEMATIC_PARAMETER_NAME_XI, GPDKinematic::KINEMATIC_PARAMETER_NAME_T, GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2, GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2.
     */
    GPDKinematic(const ElemUtils::Parameters &parameters);

    /**
     * Assignment constructor.
     *
     * @param x Longitudinal momentum fraction of active parton.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    GPDKinematic(double x, double xi, double t, double MuF2, double MuR2);

    /**
     * Assignment constructor.
     *
     * @param x Longitudinal momentum fraction of active parton.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    GPDKinematic(const PhysicalType<double> &x, const PhysicalType<double> &xi,
            const PhysicalType<double> &t, const PhysicalType<double> &MuF2,
            const PhysicalType<double> &MuR2);

    /**
     * Assignment constructor.
     *
     * @param x Longitudinal momentum fraction of active parton.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    GPDKinematic(const ElemUtils::GenericType &x,
            const ElemUtils::GenericType &xi, const ElemUtils::GenericType &t,
            const ElemUtils::GenericType &MuF2,
            const ElemUtils::GenericType &MuR2);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GPDKinematic(const GPDKinematic &other);

    /**
     * Destructor.
     */
    virtual ~GPDKinematic();

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

    bool operator ==(const GPDKinematic& b) const;
    bool operator !=(const GPDKinematic& b) const;

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
     * Get skewness variable.
     */
    const PhysicalType<double>& getXi() const;

    /**
     * Set skewness variable.
     */
    void setXi(const PhysicalType<double>& xi);

    /**
     * Set skewness variable.
     */
    void setXi(double xi, PhysicalUnit::Type unit = PhysicalUnit::NONE);

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
     * Longitudinal momentum fraction of active parton.
     */
    PhysicalType<double> m_x;

    /**
     * Skewness variable.
     */
    PhysicalType<double> m_xi;

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
        GPDKinematic& kinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        GPDKinematic& kinematic);

} /* namespace PARTONS */

#endif /* GPD_KINEMATIC_H */
