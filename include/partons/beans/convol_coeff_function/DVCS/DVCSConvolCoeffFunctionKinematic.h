#ifndef DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_H
#define DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_H

/**
 * @file DVCSConvolCoeffFunctionKinematic.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 22, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "../ConvolCoeffFunctionKinematic.h"

namespace PARTONS {

/**
 * @class DVCSConvolCoeffFunctionKinematic
 *
 * @brief Class representing single CCF kinematics for DVCS process.
 *
 * This class represents a single CCF kinematics for DVCS process (xi, t, \f$\mu_{F}^{2}\f$, \f$\mu_{F}^{2}\f$, \f$Q^{2}\f$).
 */
class DVCSConvolCoeffFunctionKinematic: public ConvolCoeffFunctionKinematic {

public:

    /**
     * Default constructor.
     */
    DVCSConvolCoeffFunctionKinematic();

    /**
     * Assignment constructor.
     * @param parameters Parameters object storing values to be set marked by GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI, ObservableKinematic::PARAMETER_NAME_T, ObservableKinematic::PARAMETER_NAME_Q2, GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2, GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2.
     */
    DVCSConvolCoeffFunctionKinematic(const ElemUtils::Parameters &parameters);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    DVCSConvolCoeffFunctionKinematic(double xi, double t, double Q2,
            double MuF2, double MuR2);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality.
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    DVCSConvolCoeffFunctionKinematic(const PhysicalType<double> &xi,
            const PhysicalType<double> &t, const PhysicalType<double> &Q2,
            const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality.
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    DVCSConvolCoeffFunctionKinematic(const ElemUtils::GenericType &xi,
            const ElemUtils::GenericType &t, const ElemUtils::GenericType &Q2,
            const ElemUtils::GenericType &MuF2,
            const ElemUtils::GenericType &MuR2);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSConvolCoeffFunctionKinematic(
            const DVCSConvolCoeffFunctionKinematic &other);

    /**
     * Destructor.
     */
    virtual ~DVCSConvolCoeffFunctionKinematic();

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
     * Get virtual-photon virtuality.
     */
    const PhysicalType<double>& getQ2() const;

    /**
     * Set virtual-photon virtuality.
     */
    void setQ2(const PhysicalType<double>& q2);

    /**
     * Set virtual-photon virtuality.
     */
    void setQ2(double q2);

protected:

    virtual void updateHashSum() const;

private:

    /**
     * Virtual-photon virtuality (in \f$GeV^{2}\f$).
     */
    PhysicalType<double> m_Q2;
};

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        DVCSConvolCoeffFunctionKinematic& kinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        DVCSConvolCoeffFunctionKinematic& kinematic);

} /* namespace PARTONS */

#endif /* DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_H */
