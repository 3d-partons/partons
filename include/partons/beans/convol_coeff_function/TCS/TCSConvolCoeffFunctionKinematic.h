#ifndef TCS_CONVOL_COEFF_FUNCTION_KINEMATIC_H
#define TCS_CONVOL_COEFF_FUNCTION_KINEMATIC_H

/**
 * @file TCSConvolCoeffFunctionKinematic.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 22, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "../../../utils/type/PhysicalUnit.h"
#include "../ConvolCoeffFunctionKinematic.h"

namespace PARTONS {

/**
 * @class TCSConvolCoeffFunctionKinematic
 *
 * @brief Class representing single CCF kinematics for TCS process.
 *
 * This class represents a single CCF kinematics for TCS process (xi, t, \f$Q^{2}'\f$, \f$\mu_{F}^{2}\f$, \f$\mu_{F}^{2}\f$).
 */
class TCSConvolCoeffFunctionKinematic: public ConvolCoeffFunctionKinematic {

public:

    static const std::string TCS_CONVOL_COEFF_FUNCTION_KNEMATIC_CLASS_NAME; ///< Type of the kinematic in XML automation.

    /**
     * Default constructor.
     */
    TCSConvolCoeffFunctionKinematic();

    /**
     * Assignment constructor.
     * @param parameters Parameters object storing values to be set marked by GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI, GPDKinematic::PARAMETER_NAME_T, TCSObservableKinematic::PARAMETER_NAME_Q2Prim, GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2, GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2.
     */
    TCSConvolCoeffFunctionKinematic(const ElemUtils::Parameters &parameters);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2Prim Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    TCSConvolCoeffFunctionKinematic(double xi, double t, double Q2Prim,
            double MuF2, double MuR2);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2Prim Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    TCSConvolCoeffFunctionKinematic(const PhysicalType<double> &xi,
            const PhysicalType<double> &t, const PhysicalType<double> &Q2Prim,
            const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2Prim Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    TCSConvolCoeffFunctionKinematic(const ElemUtils::GenericType &xi,
            const ElemUtils::GenericType &t,
            const ElemUtils::GenericType &Q2Prim,
            const ElemUtils::GenericType &MuF2,
            const ElemUtils::GenericType &MuR2);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSConvolCoeffFunctionKinematic(
            const TCSConvolCoeffFunctionKinematic &other);

    /**
     * Destructor.
     */
    virtual ~TCSConvolCoeffFunctionKinematic();

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
     * Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_Q2Prim;
};

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        TCSConvolCoeffFunctionKinematic& kinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        TCSConvolCoeffFunctionKinematic& kinematic);

} /* namespace PARTONS */

#endif /* TCS_CONVOL_COEFF_FUNCTION_KINEMATIC_H */
