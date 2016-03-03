/**
 * @file QuarkGluonVertex.h
 * @author Nabil Chouika (SPhN - CEA Saclay)
 * @date 1 mars 2016
 * @version 1.0
 *
 * @class QuarkGluonVertex
 * @brief This class is a generic quark-gluon vertex. It implements methods used by solver modules (like the quark gap equation).
 * TODO Add the formalism and definitions of \f$ H_A \f$ and \f$ H_M \f$, etc.
 */

#ifndef QUARKGLUONVERTEX_H_
#define QUARKGLUONVERTEX_H_

#include <string>
#include <vector>

#include "../../BaseObject.h"

class QuarkGluonVertex: public BaseObject {
public:
    QuarkGluonVertex(const std::string &className, unsigned int numberOfBasisElements = 1);
    virtual ~QuarkGluonVertex();

    virtual QuarkGluonVertex* clone() const = 0;

    virtual std::string toString() const;

    /**
     * Number of basis elements used: e.g 1 for RL, 3 for BC, etc.
     * @return m_N
     */
    unsigned int numberOfBasisElementsUsed() const;

    // Pure virtual methods to be implemented in daughter class.
    virtual double F_A_func(unsigned int i, double p2, double q2,
            double k2) const = 0; ///< Angular F_A function
    virtual double F_M_func(unsigned int i, double p2, double q2,
            double k2) const = 0; ///< Angular F_M function
    virtual double H_A_func(unsigned int i, double p2, double q2, double A_p2,
            double A_q2, double B_p2, double B_q2, double sigmaV_p2,
            double sigmaV_q2, double sigmaS_p2, double sigmaS_q2) const = 0; ///< H_A function dependent on the iterated functions
    virtual double H_M_func(unsigned int i, double p2, double q2, double A_p2,
            double A_q2, double B_p2, double B_q2, double sigmaV_p2,
            double sigmaV_q2, double sigmaS_p2, double sigmaS_q2) const = 0; ///< H_M function dependent on the iterated functions
    virtual double H_A_deriv(unsigned int i, double p2, double q2, double A_p2,
            double A_q2, double dA_p2, double dA_q2, double B_p2, double B_q2,
            double dB_p2, double dB_q2, double sigmaV_p2, double sigmaV_q2,
            double sigmaS_p2, double sigmaS_q2, double dsigmaV_p2,
            double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const = 0; ///< Derivative of H_A wrt to A or B coefficients
    virtual double H_M_deriv(unsigned int i, double p2, double q2, double A_p2,
            double A_q2, double dA_p2, double dA_q2, double B_p2, double B_q2,
            double dB_p2, double dB_q2, double sigmaV_p2, double sigmaV_q2,
            double sigmaS_p2, double sigmaS_q2, double dsigmaV_p2,
            double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const = 0; ///< Derivative of H_M wrt to A or B coefficients

protected:
    QuarkGluonVertex(const QuarkGluonVertex& other);

    void basisElementsWarning(const std::string& functionName) const;

    unsigned int m_N; ///< Number of basis elements used: e.g 1 for RL, 3 for BC, etc.
};

#endif /* QUARKGLUONVERTEX_H_ */
