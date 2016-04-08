/**
 * @file QuarkPropagator.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 22, 2016
 * @version 1.0
 *
 * @class QuarkPropagator
 */

#ifndef QUARKPROPAGATOR_H_
#define QUARKPROPAGATOR_H_

#include <string>
#include <vector>
#include <NumA/linear_algebra/vector/VectorD.h>

#include "../../BaseObject.h"

class QuarkPropagator: public BaseObject {
public:
    virtual ~QuarkPropagator();

    virtual QuarkPropagator* clone() const = 0;

    enum QPFunction {
        A = 0,
        B,
        M,
        sigmaV,
        sigmaS,
        ALL,
        UNDEFINED
    };

    virtual std::string toString() const;
    virtual std::string toString(unsigned int Npoints, std::vector<QuarkPropagator::QPFunction> functions) const;
    virtual std::string toString(unsigned int Npoints) const;

    unsigned int getN() const;
    /**
     * Set the order of the expansion/interpolation.
     * Calls a pure virtual method updateInterpolation() that must be implemented
     * in the daughter class in order to update the nodes, etc.
     * @param n Order of the expansion/interpolation
     */
    void setN(unsigned int n);

    virtual std::vector<double> evaluate(
            const std::vector<QuarkPropagator::QPFunction> & listOfFunctions,
            double p2, unsigned int j = 0) const = 0;

    virtual double evaluateA(double p2) const = 0;
    virtual double evaluateB(double p2) const = 0;
    double evaluateM(double p2) const;
    double evaluateSigmaV(double p2) const;
    double evaluateSigmaS(double p2) const;

    virtual double stox(double p2) const; ///< Change of variable from s=p2 [GeV] to x in [-1,1]
    virtual double xtos(double x) const; ///< Change of variable from x in [-1,1] to s=p2 [GeV]

    const NumA::VectorD& getRoots() const; ///< Get roots of the expansion (e.g Chebyshev nodes)
    virtual NumA::VectorD getInterpolationVector(double x) const = 0; ///< Interpolation vector: transforms the values on the nodes to a value on a given point x.
    virtual NumA::MatrixD getInterpolationMatrix(const NumA::VectorD& points) const = 0; ///< Interpolation matrix: transforms the values on the nodes to values on given points.

    const NumA::VectorD& getCoeffsA() const;
    double getCoeffA(unsigned int i) const;
    void setCoeffsA(const NumA::VectorD& a);
    void setCoeffsA(double a);

    const NumA::VectorD& getCoeffsB() const;
    double getCoeffB(unsigned int i) const;
    void setCoeffsB(const NumA::VectorD& b);
    void setCoeffsB(double b);

    const NumA::VectorD& getA() const;
    double getA(unsigned int i) const;
    void setA(const NumA::VectorD& a);
    void setA(double a);

    const NumA::VectorD& getB() const;
    double getB(unsigned int i) const;
    void setB(const NumA::VectorD& b);
    void setB(double b);

    double getM() const;
    void setM(double m);
    double getEpsilon() const;
    double getEpsilon2() const;
    void setEpsilon2(double epsilon2);
    double getLambda() const;
    double getLambda2() const;
    void setLambda2(double lambda2);
    double getMu() const;
    void setMu(double mu);

protected:
    QuarkPropagator(const std::string &className, unsigned int N = 50,
            double m = 5.e-3, double mu = 19, double Lambda2 = 1.e5,
            double epsilon2 = 1.e-4);
    QuarkPropagator(const QuarkPropagator& other);

    virtual void updateA() = 0; ///< Update the values of A on the nodes, by using the coefficients.
    virtual void updateB() = 0; ///< Update the values of B on the nodes, by using the coefficients.
    virtual void updateCoeffsA() = 0; ///< Update the coefficients of A, by using the values on the nodes.
    virtual void updateCoeffsB() = 0; ///< Update the coefficients of B, by using the values on the nodes.
    virtual void updateInterpolation() = 0; ///< Update the interpolation (nodes, etc) with the new value of N.

    std::string enumToString(QuarkPropagator::QPFunction function) const;

    NumA::VectorD m_a; ///< coefficients of A
    NumA::VectorD m_b; ///< coefficients of B

    NumA::VectorD m_A; ///< values of A on the nodes/roots
    NumA::VectorD m_B; ///< values of B on the nodes/roots

    NumA::VectorD m_roots; ///< nodes/roots of the expansion (e.g Chebyshev nodes)

    unsigned int m_N; ///< Number of values stored
    double m_mu; ///< renormalization point
    double m_m; ///< renormalized mass at renormalization point
    double m_Lambda, m_Lambda2; ///< ultra-violet cut-off
    double m_epsilon, m_epsilon2; ///< infra-red cut-off
};

#endif /* QUARKPROPAGATOR_H_ */
