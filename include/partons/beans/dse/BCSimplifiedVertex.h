/**
 * @file BCSimplifiedVertex.h
 * @author Nabil Chouika (SPhN - CEA Saclay)
 * @date 2 mars 2016
 * @version 1.0
 *
 * @class BCSimplifiedVertex
 * @brief Simplified Ball-Chiu Vertex.
 *
 * \f$ \Gamma_\mu = \gamma_\mu \frac{1}{2} \left( A \left(p^2 \right) + A \left( q^2 \right) \right) \f$.
 */

#ifndef BCSIMPLIFIEDVERTEX_H_
#define BCSIMPLIFIEDVERTEX_H_

#include <string>

#include "RLVertex.h"

class BCSimplifiedVertex: public RLVertex {
public:
    BCSimplifiedVertex();
    virtual ~BCSimplifiedVertex();

    virtual BCSimplifiedVertex* clone() const;

    virtual std::string toString() const;

    // Pure virtual methods to be implemented in daughter class.
    virtual double H_A_func(unsigned int i, double p2, double q2, double A_p2,
            double A_q2, double B_p2, double B_q2, double sigmaV_p2,
            double sigmaV_q2, double sigmaS_p2, double sigmaS_q2) const; ///< H_A function dependent on the iterated functions
    virtual double H_M_func(unsigned int i, double p2, double q2, double A_p2,
            double A_q2, double B_p2, double B_q2, double sigmaV_p2,
            double sigmaV_q2, double sigmaS_p2, double sigmaS_q2) const; ///< H_M function dependent on the iterated functions
    virtual double H_A_deriv(unsigned int i, double p2, double q2, double A_p2,
            double A_q2, double dA_p2, double dA_q2, double B_p2, double B_q2,
            double dB_p2, double dB_q2, double sigmaV_p2, double sigmaV_q2,
            double sigmaS_p2, double sigmaS_q2, double dsigmaV_p2,
            double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const; ///< Derivative of H_A wrt to A or B coefficients
    virtual double H_M_deriv(unsigned int i, double p2, double q2, double A_p2,
            double A_q2, double dA_p2, double dA_q2, double B_p2, double B_q2,
            double dB_p2, double dB_q2, double sigmaV_p2, double sigmaV_q2,
            double sigmaS_p2, double sigmaS_q2, double dsigmaV_p2,
            double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const; ///< Derivative of H_M wrt to A or B coefficients

protected:
    BCSimplifiedVertex(const std::string &className);
    BCSimplifiedVertex(const BCSimplifiedVertex& other);
};

#endif /* BCSIMPLIFIEDVERTEX_H_ */
