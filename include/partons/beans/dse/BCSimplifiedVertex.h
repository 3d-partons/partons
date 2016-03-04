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
    virtual std::vector<double> Radial_Integrands(double p2, double q2, double A_p2,
            double A_q2, double B_p2, double B_q2, double sigmaV_p2,
            double sigmaV_q2, double sigmaS_p2, double sigmaS_q2) const; ///< Radial Integrands
    virtual std::vector<double> Radial_Integrands_deriv(double p2, double q2, double A_p2,
            double A_q2, double dA_p2, double dA_q2, double B_p2, double B_q2,
            double dB_p2, double dB_q2, double sigmaV_p2, double sigmaV_q2,
            double sigmaS_p2, double sigmaS_q2, double dsigmaV_p2,
            double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const; ///< Derivatives of radial Integrands wrt to coefficient of A or B

protected:
    BCSimplifiedVertex(const std::string &className);
    BCSimplifiedVertex(const BCSimplifiedVertex& other);
};

#endif /* BCSIMPLIFIEDVERTEX_H_ */
