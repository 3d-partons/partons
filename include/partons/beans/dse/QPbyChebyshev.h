/**
 * @file QPbyChebyshev.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 22, 2016
 * @version 1.0
 *
 * @class QPbyChebyshev
 */

#ifndef QPBYCHEBYSHEV_H_
#define QPBYCHEBYSHEV_H_

#include <string>
#include <vector>

#include "QuarkPropagator.h"

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

    virtual void setN(unsigned int n);

    virtual std::vector<double> evaluate(
            const std::vector<QuarkPropagator::QPFunction> & listOfFunctions,
            double p2, unsigned int j = 0) const;

    virtual double evaluateA(double p2) const;
    virtual double evaluateB(double p2) const;
    virtual double differentiateA(double p2, unsigned int j) const;
    virtual double differentiateB(double p2, unsigned int j) const;

    virtual double evaluateSigmaA(double p2) const;
    virtual double evaluateSigmaM(double p2) const;
    virtual double differentiateSigmaA(double p2, unsigned int j) const;
    virtual double differentiateSigmaM(double p2, unsigned int j) const;

    virtual double stox(double p2) const; ///< Change of variable from s=p2 [GeV] to x in [-1,1]
    virtual double xtos(double x) const; ///< Change of variable from x in [-1,1] to s=p2 [GeV]

    virtual void setCoeffsAfromValueOnNodes(const std::vector<double>& values);
    virtual void setCoeffsBfromValueOnNodes(const std::vector<double>& values);


    //TODO Move Chebyshev utils to NumA
    double T(unsigned int n, double x) const; ///< Evaluates Chebyshev polynomials T_n(x) for |x| < 1

protected:
    QPbyChebyshev(const QPbyChebyshev& other);

};

#endif /* QPBYCHEBYSHEV_H_ */
