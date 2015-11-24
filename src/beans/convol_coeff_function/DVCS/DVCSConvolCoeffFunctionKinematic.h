#ifndef DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_H
#define DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_H

/**
 * @file DVCSConvolCoeffFunctionKinematic.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 22 July 2015
 * @version 1.0
 *
 * @class DVCSConvolCoeffFunctionKinematic
 *
 * @brief
 */

#include <string>

#include "../../Kinematic.h"

class ParameterList;

class DVCSConvolCoeffFunctionKinematic: public Kinematic {
public:
    DVCSConvolCoeffFunctionKinematic();
    DVCSConvolCoeffFunctionKinematic(ParameterList &parameterList);
    DVCSConvolCoeffFunctionKinematic(double xi, double t, double Q2,
            double MuF2, double MuR2);
    DVCSConvolCoeffFunctionKinematic(unsigned int binId, double xi, double t,
            double Q2, double MuF2, double MuR2);

    virtual ~DVCSConvolCoeffFunctionKinematic();

    virtual std::string toString();

    unsigned int getBinId() const;
    double getMuF2() const;
    double getMuR2() const;
    double getQ2() const;
    double getT() const;
    double getXi() const;

private:
    unsigned int m_binId;
    double m_xi;    ///< Skewness
    double m_t; ///< Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
    double m_Q2;  ///< Virtuality of the photon in Born approximation (in GeV^2)
    double m_MuF2;   ///< Factorization scale (in GeV^2)
    double m_MuR2;   ///< Renormalization scale (in GeV^2)
};

#endif /* DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_H */
