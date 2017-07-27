/**
 * @file RLVertex.h
 * @author Nabil Chouika (SPhN - CEA Saclay)
 * @date 2 mars 2016
 * @version 1.0
 */

#ifndef RLVERTEX_H_
#define RLVERTEX_H_

#include "RLVertex.h"

#include <string>

#include "QuarkGluonVertex.h"

namespace PARTONS {

/**
 * @class RLVertex
 * @brief Rainbow-Ladder Vertex.
 *
 * \f$ \Gamma^\mu = \gamma^\mu \f$.
 */
class RLVertex: public QuarkGluonVertex {
public:
    RLVertex();
    virtual ~RLVertex();

    virtual RLVertex* clone() const;

    virtual std::string toString() const;

    // Pure virtual methods to be implemented in daughter class.
    virtual std::vector<double> Angular_Integrands(double p2, double q2,
            double k2) const; ///< Angular Integrands
    virtual std::vector<double> Radial_Integrands(double p2, double q2, double A_p2,
            double A_q2, double B_p2, double B_q2, double sigmaV_p2,
            double sigmaV_q2, double sigmaS_p2, double sigmaS_q2) const; ///< Radial Integrands
    virtual std::vector<double> Radial_Integrands_deriv(double p2, double q2, double A_p2,
            double A_q2, double dA_p2, double dA_q2, double B_p2, double B_q2,
            double dB_p2, double dB_q2, double sigmaV_p2, double sigmaV_q2,
            double sigmaS_p2, double sigmaS_q2, double dsigmaV_p2,
            double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const; ///< Derivatives of radial Integrands wrt to coefficient of A or B

protected:
    RLVertex(const std::string &className, unsigned int numberOfBasisElements = 1);
    RLVertex(const RLVertex& other);
};

} /* namespace PARTONS */

#endif /* RLVERTEX_H_ */
