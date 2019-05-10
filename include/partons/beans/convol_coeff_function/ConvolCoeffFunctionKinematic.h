#ifndef CONVOL_COEFF_FUNCTION_KINEMATIC_H
#define CONVOL_COEFF_FUNCTION_KINEMATIC_H

/**
 * @file ConvolCoeffFunctionKinematic.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 22, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../utils/type/PhysicalType.h"
#include "../../utils/type/PhysicalUnit.h"
#include "../channel/ChannelType.h"
#include "../Kinematic.h"

namespace ElemUtils {
class Packet;
} /* namespace ElemUtils */

namespace PARTONS {

/**
 * @class ConvolCoeffFunctionKinematic
 *
 * @brief Abstract class representing single CCF kinematics.
 *
 * This is an abstract class representing a single CCF kinematics (xi, t, \f$\mu_{F}^{2}\f$, \f$\mu_{F}^{2}\f$).
 */
class ConvolCoeffFunctionKinematic: public Kinematic {

public:

    /**
     * Destructor.
     */
    virtual ~ConvolCoeffFunctionKinematic();

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
    //*** GETTERS AND SETTERS ********************************
    //********************************************************

    /**
     * Get skewness variable.
     */
    const PhysicalType<double>& getXi() const;

    /**
     * Get four-momentum transfer squared of hadron target.
     */
    const PhysicalType<double>& getT() const;

    /**
     * Get factorization scale squared.
     */
    const PhysicalType<double>& getMuF2() const;

    /**
     * Get renormalization scale squared.
     */
    const PhysicalType<double>& getMuR2() const;

    /**
     * Set skewness variable.
     */
    void setXi(const PhysicalType<double>& xi);

    /**
     * Set skewness variable.
     */
    void setXi(double xi, PhysicalUnit::Type unit = PhysicalUnit::NONE);

    /**
     * Set four-momentum transfer squared of hadron target.
     */
    void setT(const PhysicalType<double>& t);

    /**
     * Set four-momentum transfer squared of hadron target.
     */
    void setT(double t, PhysicalUnit::Type unit = PhysicalUnit::GEV2);

    /**
     * Set factorization scale squared.
     */
    void setMuF2(const PhysicalType<double>& muF2);

    /**
     * Set factorization scale squared.
     */
    void setMuF2(double muF2, PhysicalUnit::Type unit = PhysicalUnit::GEV2);

    /**
     * Set renormalization scale squared.
     */
    void setMuR2(const PhysicalType<double>& muR2);

    /**
     * Set renormalization scale squared.
     */
    void setMuR2(double muR2, PhysicalUnit::Type unit = PhysicalUnit::GEV2);

protected:

    /**
     * Default constructor.
     */
    ConvolCoeffFunctionKinematic(const std::string &className,
            ChannelType::Type channelType);

    /**
     * Assignment constructor.
     * @param parameters Parameters object storing values to be set marked by GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI, ObservableKinematic::PARAMETER_NAME_T, GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2, GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2.
     */
    ConvolCoeffFunctionKinematic(const std::string &className,
            ChannelType::Type channelType,
            const ElemUtils::Parameters &parameters);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    ConvolCoeffFunctionKinematic(const std::string &className,
            ChannelType::Type channelType, double xi, double t, double MuF2,
            double MuR2);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    ConvolCoeffFunctionKinematic(const std::string &className,
            ChannelType::Type channelType, const PhysicalType<double> &xi,
            const PhysicalType<double> &t, const PhysicalType<double> &MuF2,
            const PhysicalType<double> &MuR2);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    ConvolCoeffFunctionKinematic(const std::string &className,
            ChannelType::Type channelType, const ElemUtils::GenericType &xi,
            const ElemUtils::GenericType &t, const ElemUtils::GenericType &MuF2,
            const ElemUtils::GenericType &MuR2);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ConvolCoeffFunctionKinematic(const ConvolCoeffFunctionKinematic &other);

    virtual void updateHashSum() const = 0;

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
        ConvolCoeffFunctionKinematic& kinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        ConvolCoeffFunctionKinematic& kinematic);

} /* namespace PARTONS */

#endif /* CONVOL_COEFF_FUNCTION_KINEMATIC_H */
