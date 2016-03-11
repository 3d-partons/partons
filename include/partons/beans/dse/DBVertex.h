/**
 * @file DBVertex.h
 * @author Nabil Chouika (SPhN - CEA Saclay)
 * @date 8 mars 2016
 * @version 1.0
 *
 * @class DBVertex
 * @brief DCSB-improved vertex. See [arxiv:1207.5300], appendix A.2.
 */

#ifndef DBVERTEX_H_
#define DBVERTEX_H_

#include <string>
#include <vector>

#include "BCVertex.h"

class DBVertex: public BCVertex {
public:
    DBVertex();
    virtual ~DBVertex();

    virtual DBVertex* clone() const;

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
    DBVertex(const std::string &className, unsigned int numberOfBasisElements = 5);
    DBVertex(const DBVertex& other);
};

#endif /* DBVERTEX_H_ */
