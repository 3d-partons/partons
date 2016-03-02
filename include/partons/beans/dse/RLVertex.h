/**
 * @file RLVertex.h
 * @author Nabil Chouika (SPhN - CEA Saclay)
 * @date 2 mars 2016
 * @version 1.0
 *
 * @class RLVertex
 * @brief Rainbow-Ladder Vertex
 */

#ifndef RLVERTEX_H_
#define RLVERTEX_H_

#include "RLVertex.h"

#include <string>

#include "QuarkGluonVertex.h"

class RLVertex: QuarkGluonVertex {
public:
    RLVertex(const std::string &className);
    virtual ~RLVertex();

    virtual RLVertex* clone() const;

    virtual std::string toString() const;

    // Pure virtual methods to be implemented in daughter class.
    virtual double F_A_func(unsigned int i, double p2, double q2,
            double k2) const ; ///< Angular F_A function
    virtual double F_M_func(unsigned int i, double p2, double q2,
            double k2) const ; ///< Angular F_M function
    virtual double H_A_func(unsigned int i, double A_p2, double A_q2,
            double B_p2, double B_q2, double sigmaV_p2, double sigmaV_q2,
            double sigmaS_p2, double sigmaS_q2) const; ///< H_A function dependent on the iterated functions
    virtual double H_M_func(unsigned int i, double A_p2, double A_q2,
            double B_p2, double B_q2, double sigmaV_p2, double sigmaV_q2,
            double sigmaS_p2, double sigmaS_q2) const; ///< H_M function dependent on the iterated functions
    virtual double H_A_deriv(unsigned int i, double A_p2, double A_q2,
            double dA_p2, double dA_q2, double B_p2, double B_q2, double dB_p2,
            double dB_q2, double sigmaV_p2, double sigmaV_q2, double sigmaS_p2,
            double sigmaS_q2, double dsigmaV_p2,
            double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const; ///< Derivative of H_A wrt to A or B coefficients
    virtual double H_M_deriv(unsigned int i, double A_p2, double A_q2,
            double dA_p2, double dA_q2, double B_p2, double B_q2, double dB_p2,
            double dB_q2, double sigmaV_p2, double sigmaV_q2, double sigmaS_p2,
            double sigmaS_q2, double dsigmaV_p2,
            double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const; ///< Derivative of H_M wrt to A or B coefficients

protected:
    RLVertex(const RLVertex& other);
};

#endif /* RLVERTEX_H_ */
