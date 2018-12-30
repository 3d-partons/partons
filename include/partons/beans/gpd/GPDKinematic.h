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

    //TODO The alias of T is defined in another class (Observable)
    /**
     * Parameter name to set variable \f$x\f$ via configuration methods.
     */
    static const std::string GPD_KINEMATIC_PARAMETER_NAME_X;

    /**
     * Parameter name to set variable \f$\xi\f$ via configuration methods.
     */
    static const std::string GPD_KINEMATIC_PARAMETER_NAME_XI;

    /**
     * Parameter name to set variable \f$\mu_{F}^{2}\f$ via configuration methods.
     */
    static const std::string GPD_KINEMATIC_PARAMETER_NAME_MUF2;

    /**
     * Parameter name to set variable \f$\mu_{R}^{2}\f$ via configuration methods.
     */
    static const std::string GPD_KINEMATIC_PARAMETER_NAME_MUR2;

    /**
     * Default constructor.
     */
    GPDKinematic();

    /**
     * Assignment constructor.
     * @param parameters Parameters object storing values to be set marked by GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_X, GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI, ObservableKinematic::PARAMETER_NAME_T, GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2, GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2.
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

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Set longitudinal momentum fraction of active parton.
     */
    void setX(double x);

    /**
     * Set skewness variable.
     */
    void setXi(double xi);

    /**
     * Set four-momentum transfer squared of hadron target.
     */
    void setT(double t);

    /**
     * Set factorization scale squared.
     */
    void setMuF2(double muF2);

    /**
     * Set renormalization scale squared.
     */
    void setMuR2(double muR2);

    /**
     * Get longitudinal momentum fraction of active parton.
     */
    double getX() const;

    /**
     * Get skewness variable.
     */
    double getXi() const;

    /**
     * Get four-momentum transfer squared of hadron target.
     */
    double getT() const;

    /**
     * Get factorization scale squared.
     */
    double getMuF2() const;

    /**
     * Get renormalization scale squared.
     */
    double getMuR2() const;

protected:

    virtual void updateHashSum() const;

private:

    /**
     * Longitudinal momentum fraction of active parton.
     */
    double m_x;

    /**
     * Skewness variable.
     */
    double m_xi;

    /**
     * Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     */
    double m_t;

    /**
     * Factorization scale squared (in \f$GeV^{2}\f$).
     */
    double m_MuF2;

    /**
     * Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    double m_MuR2;
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
