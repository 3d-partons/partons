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
    QPbyChebyshev(unsigned int N = 0, double m = 0.005, double mu = 19,
            double Lambda2 = 1.e5, double epsilon2 = 1.e-4);
    virtual ~QPbyChebyshev();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString() const;

    virtual double evaluateA(double p2);
    virtual double evaluateB(double p2);

    const std::vector<double>& getCoeffsA() const;
    void setCoeffsA(const std::vector<double>& a);
    const double getCoeffA(unsigned int i);
    void setCoeffA(unsigned int i, double a);

    const std::vector<double>& getCoeffsB() const;
    void setCoeffsB(const std::vector<double>& b);
    const double getCoeffB(unsigned int i);
    void setCoeffB(unsigned int i, double b);

    virtual void setMu(double mu);

private:
    std::vector<double> m_a; ///< coefficients of Sigma_A
    std::vector<double> m_b; ///< coefficients of Sigma_M

    double evaluateSigmaA(double p2);
    double evaluateSigmaM(double p2);
    void updateSigmaAM(double p2);
    double computeSigmaA(double x);
    double computeSigmaM(double x);

    double T(unsigned int n, double x); ///< Evaluates Chebyshev polynomials T_n(x) for |x| < 1

    double stox(double p2); ///< Change of variable from s=p2 [GeV] to x in [-1,1]
    double xtos(double x); ///< Change of variable from x in [-1,1] to s=p2 [GeV]

    double m_p2, m_x, m_xmu, m_SigmaAp2, m_SigmaAmu, m_SigmaMp2, m_SigmaMmu,
            m_Ap2, m_Bp2; ///< Values stored to avoid redundant computations
    bool m_change; ///< says it there was a change in the coefficients
};

#endif /* QPBYCHEBYSHEV_H_ */
