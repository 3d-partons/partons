/**
 * @file GapEquationSolverModule.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 22, 2016
 * @version 1.0
 *
 * @class GapEquationSolverModule
 */

#ifndef GAPEQUATIONSOLVER_H_
#define GAPEQUATIONSOLVER_H_

#include <string>
#include <vector>

#include "../../ModuleObject.h"

class GluonPropagator;
class QuarkPropagator;

class GapEquationSolverModule: public ModuleObject {
public:
    /**
     * Default constructor
     */
    GapEquationSolverModule(const std::string &className);

    virtual GapEquationSolverModule* clone() const = 0;

    /**
     * Default destructor
     */
    virtual ~GapEquationSolverModule();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString() const;

    /**
     * Provides a generic method to configure all types of modules by passing a Parameters object.
     * (See ModuleObject class for more info).
     *
     * @param parameters
     */
    virtual void configure(ParameterList parameters);

    enum QPType {
        Chebyshev = 0, Discrete = 1
    };

    enum IterativeType {
        Naive = 0, Newton = 1
    };

    enum IntegrationType {
        Legendre = 0
    };

    double getEpsilon2() const;
    void setEpsilon2(double epsilon2);
    double getLambda2() const;
    void setLambda2(double lambda2);
    double getM() const;
    void setM(double m);
    int getMaxIter() const;
    void setMaxIter(int maxIter);
    double getMu() const;
    void setMu(double mu);
    int getN() const;
    void setN(int n);
    int getNx() const;
    void setNx(int nx);
    int getNz() const;
    void setNz(int nz);
    double getTolerance() const;
    void setTolerance(double tolerance);
    const GluonPropagator* getGluonPropagator() const;
    void setGluonPropagator(const GluonPropagator* gluonPropagator);

    QuarkPropagator* compute(GapEquationSolverModule::QPType qpType,
            GapEquationSolverModule::IterativeType iterativeType,
            GapEquationSolverModule::IntegrationType integrationType);

protected:
    /**
     * Copy constructor
     *
     * @param other
     */
    GapEquationSolverModule(const GapEquationSolverModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:
    GluonPropagator* m_gluonPropagator;

    double m_Lambda2, m_Lambda; ///< Ultra-violet cut-off
    double m_epsilon2, m_epsilon; ///< Indra-red cut-off
    double m_mu; ///< Renormalization point
    double m_m; ///< Renormalized mass

    int m_N; ///< Number of values for representing the propagator
    int m_Nx; ///< Number of points for integration on x (momentum variable)
    int m_Nz; ///< Number of points for integration on z (angular variable)

    double m_tolerance; ///< Convergence criterion
    int m_maxIter; ///< Max iterations if the criterion is not fulfilled

    // Stored calculations
    double m_C;

    // Functions to integrate
    // Angular integrals //TODO make pure virtual methods and implement in daughter class
    double ThetaA_func(std::vector<double> z, std::vector<double> parameters);
    double ThetaM_func(std::vector<double> z, std::vector<double> parameters);

    //TODO make pure virtual methods and implement in daughter class
    // Various function (model dependent)
    double F_func(double p2, double q2, double k2);
    double k2_func(double p2, double q2, double z);
};

#endif /* GAPEQUATIONSOLVER_H_ */
