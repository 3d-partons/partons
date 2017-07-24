/**
 * @file QPbyChebyshev.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 22, 2016
 * @version 1.0
 */

#ifndef QPBYCHEBYSHEV_H_
#define QPBYCHEBYSHEV_H_

#include <string>
#include <vector>
#include <NumA/interpolation/Chebyshev.h>

#include "QuarkPropagator.h"

namespace PARTONS {

/**
 * @class QPbyChebyshev
 */
class QPbyChebyshev: public QuarkPropagator {
public:
    QPbyChebyshev(unsigned int N = 50, double m = 5.e-3, double mu = 19,
            double Lambda2 = 1.e5, double epsilon2 = 1.e-4);
    virtual ~QPbyChebyshev();

    virtual QPbyChebyshev* clone() const;

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString() const;

    virtual std::vector<double> evaluate(
            const std::vector<QuarkPropagator::QPFunction> & listOfFunctions,
            double p2, unsigned int j = 0) const;

    virtual double evaluateA(double p2) const;
    virtual double evaluateB(double p2) const;

    virtual NumA::VectorD getInterpolationVector(double x) const; ///< Interpolation vector: transforms the values on the nodes to a value on a given point.
    virtual NumA::MatrixD getInterpolationMatrix(const NumA::VectorD& points) const; ///< Interpolation matrix: transforms the values on the nodes to values on given points.

protected:
    QPbyChebyshev(const QPbyChebyshev& other);

    virtual void updateA(); ///< Update the values of A on the nodes, by using the coefficients.
    virtual void updateB(); ///< Update the values of B on the nodes, by using the coefficients.
    virtual void updateCoeffsA(); ///< Update the coefficients of A, by using the values on the nodes.
    virtual void updateCoeffsB(); ///< Update the coefficients of B, by using the values on the nodes.
    virtual void updateInterpolation(); ///< Update the interpolation (nodes, etc) with the new value of N.

    NumA::Chebyshev m_cheb;
};

} /* namespace PARTONS */

#endif /* QPBYCHEBYSHEV_H_ */
