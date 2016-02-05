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

#include "../../BaseObject.h"

class QuarkPropagator: public BaseObject {
public:
    QuarkPropagator(unsigned int N = 0, double m = 0.005, double mu = 19,
            double Lambda2 = 1.e5, double epsilon2 = 1.e-4);
    QuarkPropagator(const std::string &className, unsigned int N = 0, double m =
            0.005, double mu = 19, double Lambda2 = 1.e5, double epsilon2 =
            1.e-4);
    virtual ~QuarkPropagator();

    virtual std::string toString() const;

    unsigned int getN() const;
    virtual void setN(unsigned int n);

    virtual double evaluateA(double p2) const = 0;
    virtual double evaluateB(double p2) const = 0;
    virtual double differentiateA(double p2, unsigned int j) const = 0;
    virtual double differentiateB(double p2, unsigned int j) const = 0;

    virtual double evaluateSigmaA(double p2) const = 0;
    virtual double evaluateSigmaM(double p2) const = 0;
    virtual double differentiateSigmaA(double p2, unsigned int j) const = 0;
    virtual double differentiateSigmaM(double p2, unsigned int j) const = 0;

    virtual double evaluateSigmaV(double p2) const = 0;
    virtual double evaluateSigmaS(double p2) const = 0;
    virtual double differentiateSigmaV_a(double p2, unsigned int j) const = 0;
    virtual double differentiateSigmaS_a(double p2, unsigned int j) const = 0;
    virtual double differentiateSigmaV_b(double p2, unsigned int j) const = 0;
    virtual double differentiateSigmaS_b(double p2, unsigned int j) const = 0;

    double getM() const;
    void setM(double m);
    double getEpsilon() const;
    void setEpsilon(double epsilon);
    double getLambda() const;
    void setLambda(double lambda);
    double getMu() const;
    virtual void setMu(double mu);

protected:
    unsigned int m_N; ///< Number of values stored
    double m_mu; ///< renormalization point
    double m_m; ///< renormalized mass at renormalization point
    double m_Lambda; ///< ultra-violet cut-off
    double m_epsilon; ///< infra-red cut-off
};

#endif /* QUARKPROPAGATOR_H_ */
