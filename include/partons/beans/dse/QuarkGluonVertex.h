/**
 * @file QuarkGluonVertex.h
 * @author Nabil Chouika (SPhN - CEA Saclay)
 * @date 1 mars 2016
 * @version 1.0
 */

#ifndef QUARKGLUONVERTEX_H_
#define QUARKGLUONVERTEX_H_

#include <string>
#include <vector>

#include "../../BaseObject.h"

namespace PARTONS {

/**
 * @class QuarkGluonVertex
 * @brief This class is a generic quark-gluon vertex. It implements methods used by solver modules (like the quark gap equation).
 * TODO Add the formalism and definitions of \f$ H_A \f$ and \f$ H_M \f$, etc.
 */
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
    virtual std::vector<double> Angular_Integrands(double p2, double q2,
            double k2) const = 0; ///< Angular Integrands
    virtual std::vector<double> Radial_Integrands(double p2, double q2, double A_p2,
            double A_q2, double B_p2, double B_q2, double sigmaV_p2,
            double sigmaV_q2, double sigmaS_p2, double sigmaS_q2) const = 0; ///< Radial Integrands
    virtual std::vector<double> Radial_Integrands_deriv(double p2, double q2, double A_p2,
            double A_q2, double dA_p2, double dA_q2, double B_p2, double B_q2,
            double dB_p2, double dB_q2, double sigmaV_p2, double sigmaV_q2,
            double sigmaS_p2, double sigmaS_q2, double dsigmaV_p2,
            double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const = 0; ///< Derivatives of radial Integrands wrt to coefficient of A or B

protected:
    QuarkGluonVertex(const QuarkGluonVertex& other);

    void basisElementsWarning(const std::string& functionName) const;

    unsigned int m_N; ///< Number of basis elements used: e.g 1 for RL, 3 for BC, etc.
};

} /* namespace PARTONS */

#endif /* QUARKGLUONVERTEX_H_ */
