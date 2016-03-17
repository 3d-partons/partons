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

//#include <NumA/integration/one_dimension/GLNPIntegrator1D.h>
#include <NumA/utils/Tolerances.h>
#include <string>
#include <vector>

#include "../../ModuleObject.h"

namespace NumA {
class QuadratureIntegrator1D;
} /* namespace NumA */

class QuarkGluonVertex;

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
    virtual void configure(const ElemUtils::Parameters &parameters);

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
    const NumA::Tolerances& getTolerance() const;
    void setTolerance(const NumA::Tolerances& tolerance);
    double getAinit() const;
    void setAinit(double ainit);
    double getBinit() const;
    void setBinit(double binit);
    void reset();

    const GluonPropagator* getGluonPropagator() const;
    void setGluonPropagator(const GluonPropagator* gluonPropagator);
    const QuarkPropagator* getQuarkPropagator() const;
    void setQuarkPropagator(QuarkPropagator* quarkPropagator);
    const QuarkGluonVertex* getVertex() const;
    void setVertex(const QuarkGluonVertex* vertex);

    virtual void compute(IterativeType iterativeType = Naive);

protected:
    /**
     * Copy constructor
     *
     * @param other
     */
    GapEquationSolverModule(const GapEquationSolverModule &other);

    QuarkPropagator* getQuarkPropagator();
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

    const std::vector<double>& getC() const;
    void setC(const std::vector<double>& c);
    bool isChangedGp() const;
    void setChangedGp(bool changedGp);
    bool isChangedInit() const;
    void setChangedInit(bool changedInit);
    bool isChangedNx() const;
    void setChangedNx(bool changedNx);
    bool isChangedNz() const;
    void setChangedNz(bool changedNz);
    bool isChangedQp() const;
    void setChangedQp(bool changedQp);
    bool isChangedVertex() const;
    void setChangedVertex(bool changedVertex);
    int getIters() const;
    void setIters(int iters);
    const std::vector<double>& getNodesS() const;
    const std::vector<double>& getNodesX() const;
    const std::vector<double>& getNodesZ() const;
    const std::vector<double>& getRootsS() const;
    const std::vector<double>& getRootsX() const;
    const std::vector<double>& getWeightsX() const;
    const std::vector<double>& getWeightsZ() const;

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual void computeNewtonInteration() = 0;
    virtual void computeIteration() = 0;

    // Various functions (virtual means model dependent and must be implemented in daughter class)
    double k2_func(double p2, double q2, double z) const;
    double t2_func(double p2, double q2, double z) const;

private:
    GluonPropagator* m_gluonPropagator; ///< GluonPropagator clone
    QuarkPropagator* m_quarkPropagator; ///< QuarkPropagator (pointer not cloned!)
    QuarkGluonVertex* m_vertex; ///< Quark-Gluon Vertex clone

    double m_Lambda2; ///< Ultra-violet cut-off
    double m_epsilon2; ///< Indra-red cut-off
    double m_mu; ///< Renormalization point
    double m_m; ///< Renormalized mass
    double m_Ainit, m_Binit; ///< Intialization for the Iterative Solver

    int m_N; ///< Number of values for representing the propagator
    int m_Nx; ///< Number of points for integration on x (momentum variable)
    int m_Nz; ///< Number of points for integration on z (angular variable)

    std::vector<double> m_roots_x, m_roots_s; ///< Roots for the propagator's expansion (e.g. Chebyshev roots)
    NumA::QuadratureIntegrator1D* m_quad_x; ///< Integration quadrature
    std::vector<double> m_nodes_x, m_nodes_s, m_weights_x; ///< Integration nodes and weights
    NumA::QuadratureIntegrator1D* m_quad_z; ///< Angular integration quadrature
    std::vector<double> m_nodes_z, m_weights_z; ///< Angular integration nodes and weights

    NumA::Tolerances m_tolerance; ///< Convergence criterion
    int m_maxIter; ///< Max iterations if the criterion is not fulfilled
    int m_iters; ///< Number of iterations used

    // Keep track of changes
    bool m_changedNx, m_changedNz; ///< Tests if there is a change in the quadrature to avoid recalculations
    bool m_changedVertex, m_changedQP, m_changedGP; ///< Tests if there is a change in the propagator to avoid recalculations
    bool m_changedInit; ///< Tests if there is a change in the init values of A and B, if so reset the iterations

    // Stored calculations
    std::vector<double> m_C; ///< Stored coefficients
};

#endif /* GAPEQUATIONSOLVER_H_ */
