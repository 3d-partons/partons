/**
 * @file GapEqSeparableSolver.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 26, 2016
 * @version 1.0
 *
 * @class GapEqSeparableSolver
 */

#ifndef GAPEQSEPARABLESOLVER_H_
#define GAPEQSEPARABLESOLVER_H_

#include <string>
#include <vector>
#include <NumA/linear_algebra/vector/VectorD.h>

#include "GapEquationSolverModule.h"

namespace PARTONS {

class GapEqSeparableSolver: public GapEquationSolverModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    GapEqSeparableSolver(const std::string &className);
    virtual ~GapEqSeparableSolver();

    virtual GapEqSeparableSolver* clone() const;

protected:
    /**
     * Copy constructor
     *
     * Use by the factory
     *
     * @param other
     */
    GapEqSeparableSolver(const GapEqSeparableSolver& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

//    virtual void computeNewtonIteration();
    virtual void computeBroydenIteration();
//    virtual void computeIteration();
//    virtual void computeIterations(GapEquationSolverModule::IterativeType iterativeType = Naive);

    // Various functions (virtual means model dependent and must be implemented in daughter class)
    virtual NumA::VectorD G_func(const NumA::VectorD& X); ///< Computes the function \f$ G \f$ of the non-linear problem \f$ G \left( X \right) = 0 \f$ .
    virtual NumA::MatrixD J_G_func(const NumA::VectorD& X); ///< Computes the Jacobian matrix \f$ J_G \f$ of the non-linear problem \f$ G \left( X \right) = 0 \f$ .


private:
    // Stored calculations
    std::vector<std::vector<std::vector<double> > > m_ThetaA, m_ThetaM; ///< Stored angular integrals
    NumA::VectorD m_X0, m_X, m_DeltaX; ///< DeltaXn = Xn - X(n-1) needed by Broyden to compute the step (n+1)
    NumA::VectorD m_Ap, m_Aq, m_Bp, m_Bq, m_sigmaVp, m_sigmaVq, m_sigmaSp, m_sigmaSq;
    double m_Amu, m_Bmu;
    NumA::VectorD m_SigmaAp, m_SigmaMp;
    NumA::VectorD m_G_X0, m_G_X, m_DeltaG; ///< DeltaGn = G_Xn - G_X(n-1) needed by Broyden to compute the step (n+1)
    NumA::MatrixD m_J_G_X0, m_J_G_X; ///< Jacobian matrix stored for Broyden algorithm
};

} /* namespace PARTONS */

#endif /* GAPEQSEPARABLESOLVER_H_ */
