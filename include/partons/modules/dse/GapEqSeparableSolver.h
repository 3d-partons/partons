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

#include "GapEquationSolverModule.h"

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

    virtual void computeNewtonInteration();
    virtual void computeIteration();

private:
    // Stored calculations
    std::vector<std::vector<std::vector<double> > > m_ThetaA, m_ThetaM; ///< Stored angular integrals
};

#endif /* GAPEQSEPARABLESOLVER_H_ */
