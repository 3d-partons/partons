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
#include <NumA/integration/GLNPIntegrationMode.h>

#include "../../ModuleObject.h"
#include "../../utils/math/Tolerances.h"

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

    enum IterativeType {
        Naive = 0, Newton = 1
    };

    int getMaxIter() const;
    void setMaxIter(int maxIter);
    int getNx() const;
    void setNx(int nx);
    int getNz() const;
    void setNz(int nz);
    double getAbsTolerance() const;
    void setAbsTolerance(double absTolerance);
    double getRelTolerance() const;
    void setRelTolerance(double relTolerance);
    const Tolerances& getTolerance() const;
    void setTolerance(const Tolerances& tolerance);
    double getAinit() const;
    void setAinit(double ainit);
    double getBinit() const;
    void setBinit(double binit);

    const GluonPropagator* getGluonPropagator() const;
    void setGluonPropagator(const GluonPropagator* gluonPropagator);
    const QuarkPropagator* getQuarkPropagator() const;
    void setQuarkPropagator(QuarkPropagator* quarkPropagator);

    virtual void compute(IterativeType iterativeType = Naive);

protected:
    /**
     * Copy constructor
     *
     * @param other
     */
    GapEquationSolverModule(const GapEquationSolverModule &other);

    double getEpsilon2() const;
    void setEpsilon2(double epsilon2);
    double getLambda2() const;
    void setLambda2(double lambda2);
    double getM() const;
    void setM(double m);
    double getMu() const;
    void setMu(double mu);
    int getN() const;
    void setN(int n);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual void computeNewtonInteration();
    virtual void computeIteration();

    // Angular integrands (functions integrated in the angular integrals)
    double ThetaA_func(std::vector<double> z,
            std::vector<double> parameters) const;
    double ThetaM_func(std::vector<double> z,
            std::vector<double> parameters) const;

    // Various functions (virtual means model dependent and must be implemented in daughter class)
    virtual double F_A_func(double p2, double q2, double k2) const = 0; ///< Angular F_A function
    virtual double F_M_func(double p2, double q2, double k2) const = 0; ///< Angular F_M function
    virtual double H_A_func(double p2, double q2) const = 0; ///< H_A function dependent on the iterated functions
    virtual double H_M_func(double p2, double q2) const = 0; ///< H_M function dependent on the iterated functions
    virtual double H_A_deriv_a(double p2, double q2, unsigned int j) const = 0;
    virtual double H_M_deriv_a(double p2, double q2, unsigned int j) const = 0;
    virtual double H_A_deriv_b(double p2, double q2, unsigned int j) const = 0;
    virtual double H_M_deriv_b(double p2, double q2, unsigned int j) const = 0;
    virtual double H_A_func(double A_p2, double A_q2, double B_p2, double B_q2,
            double sigmaV_p2, double sigmaV_q2, double sigmaS_p2,
            double sigmaS_q2) const = 0; ///< H_A function dependent on the iterated functions
    virtual double H_M_func(double A_p2, double A_q2, double B_p2, double B_q2,
            double sigmaV_p2, double sigmaV_q2, double sigmaS_p2,
            double sigmaS_q2) const = 0; ///< H_M function dependent on the iterated functions
    virtual double H_A_deriv_a(double A_p2, double A_q2, double dA_p2,
            double dA_q2, double B_p2, double B_q2, double sigmaV_p2,
            double sigmaV_q2, double sigmaS_p2, double sigmaS_q2,
            double dsigmaV_a_p2, double dsigmaV_b_p2, double dsigmaV_a_q2,
            double dsigmaV_b_q2, double dsigmaS_a_p2, double dsigmaS_b_p2,
            double dsigmaS_a_q2, double dsigmaS_b_q2) const = 0;
    virtual double H_M_deriv_a(double A_p2, double A_q2, double dA_p2,
            double dA_q2, double B_p2, double B_q2, double sigmaV_p2,
            double sigmaV_q2, double sigmaS_p2, double sigmaS_q2,
            double dsigmaV_a_p2, double dsigmaV_b_p2, double dsigmaV_a_q2,
            double dsigmaV_b_q2, double dsigmaS_a_p2, double dsigmaS_b_p2,
            double dsigmaS_a_q2, double dsigmaS_b_q2) const = 0;
    virtual double H_A_deriv_b(double A_p2, double A_q2, double B_p2,
            double B_q2, double dB_p2, double dB_q2, double sigmaV_p2,
            double sigmaV_q2, double sigmaS_p2, double sigmaS_q2,
            double dsigmaV_a_p2, double dsigmaV_b_p2, double dsigmaV_a_q2,
            double dsigmaV_b_q2, double dsigmaS_a_p2, double dsigmaS_b_p2,
            double dsigmaS_a_q2, double dsigmaS_b_q2) const = 0;
    virtual double H_M_deriv_b(double A_p2, double A_q2, double B_p2,
            double B_q2, double dB_p2, double dB_q2, double sigmaV_p2,
            double sigmaV_q2, double sigmaS_p2, double sigmaS_q2,
            double dsigmaV_a_p2, double dsigmaV_b_p2, double dsigmaV_a_q2,
            double dsigmaV_b_q2, double dsigmaS_a_p2, double dsigmaS_b_p2,
            double dsigmaS_a_q2, double dsigmaS_b_q2) const = 0;
    double k2_func(double p2, double q2, double z) const;

private:
    GluonPropagator* m_gluonPropagator; ///< GluonPropagator clone
    QuarkPropagator* m_quarkPropagator; ///< QuarkPropagator clone

    double m_Lambda2; ///< Ultra-violet cut-off
    double m_epsilon2; ///< Indra-red cut-off
    double m_mu; ///< Renormalization point
    double m_m; ///< Renormalized mass
    double m_Ainit, m_Binit; ///< Intialization for the Iterative Solver

    int m_N; ///< Number of values for representing the propagator
    int m_Nx; ///< Number of points for integration on x (momentum variable)
    int m_Nz; ///< Number of points for integration on z (angular variable)

    std::vector<double> m_roots_x, m_roots_s; ///< Roots for the propagator's expansion (e.g. Chebyshev roots)
    NumA::GLNPIntegrationMode m_quad_x; ///< Integration quadrature
    std::vector<double> m_nodes_x, m_nodes_s, m_weights_x; ///< Integration nodes and weights
    NumA::GLNPIntegrationMode m_quad_z; ///< Angular integration quadrature
    std::vector<double> m_nodes_z, m_weights_z; ///< Angular integration nodes and weights

    Tolerances m_tolerance; ///< Convergence criterion
    int m_maxIter; ///< Max iterations if the criterion is not fulfilled

    // Stored calculations
    std::vector<double> m_C; ///< Stored coefficients
    std::vector<std::vector<double> > m_ThetaA, m_ThetaM; ///< Stored angular integrals
};

#endif /* GAPEQUATIONSOLVER_H_ */
