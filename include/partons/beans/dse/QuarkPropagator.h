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
    QuarkPropagator(unsigned int N = 50, double m = 5.e-3, double mu = 19,
            double Lambda2 = 1.e5, double epsilon2 = 1.e-4);
    QuarkPropagator(const std::string &className, unsigned int N = 50,
            double m = 5.e-3, double mu = 19, double Lambda2 = 1.e5,
            double epsilon2 = 1.e-4);
    virtual ~QuarkPropagator();

    virtual QuarkPropagator* clone() const = 0;

    virtual std::string toString() const;

    enum QPFunction {
        SigmaA = 0,
        SigmaM,
        A,
        B,
        sigmaV,
        sigmaS,
        dSigmaA,
        dSigmaM,
        dA,
        dB,
        dsigmaV_a,
        dsigmaV_b,
        dsigmaS_a,
        dsigmaS_b,
        ALL,
        UNDEFINED
    };

    unsigned int getN() const;
    virtual void setN(unsigned int n);

    virtual std::vector<double> evaluate(
            const std::vector<QuarkPropagator::QPFunction> & listOfFunctions,
            double p2, unsigned int j = 0) const = 0;

    virtual double evaluateA(double p2) const = 0;
    virtual double evaluateB(double p2) const = 0;
    virtual double differentiateA(double p2, unsigned int j) const = 0;
    virtual double differentiateB(double p2, unsigned int j) const = 0;

    virtual double evaluateSigmaA(double p2) const = 0;
    virtual double evaluateSigmaM(double p2) const = 0;
    virtual double differentiateSigmaA(double p2, unsigned int j) const = 0;
    virtual double differentiateSigmaM(double p2, unsigned int j) const = 0;

    double evaluateM(double p2) const;

    double evaluateSigmaV(double p2) const;
    double evaluateSigmaS(double p2) const;
    double differentiateSigmaV_a(double p2, unsigned int j) const;
    double differentiateSigmaS_a(double p2, unsigned int j) const;
    double differentiateSigmaV_b(double p2, unsigned int j) const;
    double differentiateSigmaS_b(double p2, unsigned int j) const;

    virtual double stox(double p2) const = 0; ///< Change of variable from s=p2 [GeV] to x
    virtual double xtos(double x) const = 0; ///< Change of variable from x to s=p2 [GeV]

    const std::vector<double>& getRoots() const; ///< Get roots of the expansion (e.g Chebyshev nodes)

    const NumA::VectorD& getCoeffsA() const;
    void setCoeffsA(const NumA::VectorD& a);
    void setCoeffsA(double a);
    const double getCoeffA(unsigned int i) const;
    void setCoeffA(unsigned int i, double a);
    virtual void setCoeffsAfromValueOnNodes(
            const std::vector<double>& values) = 0;

    const NumA::VectorD& getCoeffsB() const;
    void setCoeffsB(const NumA::VectorD& b);
    void setCoeffsB(double b);
    const double getCoeffB(unsigned int i) const;
    void setCoeffB(unsigned int i, double b);
    virtual void setCoeffsBfromValueOnNodes(
            const std::vector<double>& values) = 0;

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
    QuarkPropagator(const QuarkPropagator& other);

    NumA::VectorD m_a; ///< coefficients of Sigma_A (or A)
    NumA::VectorD m_b; ///< coefficients of Sigma_M (or B)

    std::vector<double> m_roots; ///< roots of the expansion (e.g Chebyshev nodes)

    unsigned int m_N; ///< Number of values stored
    double m_mu; ///< renormalization point
    double m_m; ///< renormalized mass at renormalization point
    double m_Lambda, m_Lambda2; ///< ultra-violet cut-off
    double m_epsilon, m_epsilon2; ///< infra-red cut-off
};

#endif /* QUARKPROPAGATOR_H_ */
