/**
 * @file GapEquationSolverModule.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 22, 2016
 * @version 1.0
 */

#ifndef GAPEQUATIONSOLVER_H_
#define GAPEQUATIONSOLVER_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <NumA/linear_algebra/matrix/MatrixD.h>
#include <NumA/linear_algebra/vector/VectorD.h>
#include <NumA/utils/Differences.h>
#include <NumA/utils/Tolerances.h>
#include <map>
#include <string>
#include <vector>

#include "../../beans/automation/BaseObjectData.h"
#include "../../ModuleObject.h"

namespace NumA {
class QuadratureIntegrator1D;
} /* namespace NumA */

class QuarkGluonVertex;

class GluonPropagator;
class QuarkPropagator;

/**
 * @class GapEquationSolverModule
 */
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
        Naive = 0, Broyden, Newton
    };

    unsigned int getMaxIter() const;
    void setMaxIter(unsigned int maxIter);
    unsigned int getNx() const;
    void setNx(unsigned int nx);
    unsigned int getNz() const;
    void setNz(unsigned int nz);
    double getAbsTolerance() const;
    void setAbsTolerance(double absTolerance);
    double getRelTolerance() const;
    void setRelTolerance(double relTolerance);
    const NumA::Tolerances& getTolerance() const;
    void setTolerance(const NumA::Tolerances& tolerance);
    const NumA::Differences& getDifference() const;
    double getAbsDifference() const;
    double getRelDifference() const;
    bool isConverged() const;
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

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

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
    double getMu2() const;
    unsigned int getN() const;
    void setN(unsigned int n);
    unsigned int get2N() const;

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
    void setConverged(bool converged);
    void setDifference(const NumA::Differences& difference);
    void setAbsDifference(double absDifference);
    void setRelDifference(double relDifference);
    unsigned int getIters() const;
    void setIters(unsigned int iters);
    const NumA::VectorD& getNodesS() const;
    const NumA::VectorD& getNodesX() const;
    const NumA::VectorD& getNodesZ() const;
    const NumA::VectorD& getRootsS() const;
    const NumA::VectorD& getRootsX() const;
    const NumA::VectorD& getWeightsX() const;
    const NumA::VectorD& getWeightsZ() const;
    const NumA::MatrixD& getInterpolationMatrix() const; ///< Interpolation matrix from the values on the expansion's nodes to the values on quadratures nodes.
    void setInterpolationMatrix(const NumA::MatrixD& interpolationMatrix);
    const NumA::VectorD& getInterpolationMu() const; ///< Interpolation from the values on the expansion's nodes to the value on the renormalization point.
    void setInterpolationMu(const NumA::VectorD& interpolationMu);

    virtual void initModule();
    virtual void isModuleWellConfigured();

//    virtual void computeNewtonIteration() = 0;
    virtual void computeBroydenIteration() = 0;
//    virtual void computeIteration() = 0;
//    virtual void computeIterations(GapEquationSolverModule::IterativeType iterativeType = Naive) = 0;

    // Various functions (virtual means model dependent and must be implemented in daughter class)
    virtual NumA::VectorD G_func(const NumA::VectorD& X) = 0; ///< Computes the function \f$ G \f$ of the non-linear problem \f$ G \left( X \right) = 0 \f$ .
    virtual NumA::MatrixD J_G_func(const NumA::VectorD& X) = 0; ///< Computes the Jacobian matrix \f$ J_G \f$ of the non-linear problem \f$ G \left( X \right) = 0 \f$ .
    double k2_func(double p2, double q2, double z) const;
    double t2_func(double p2, double q2, double z) const;

private:
    GluonPropagator* m_gluonPropagator; ///< GluonPropagator clone
    QuarkPropagator* m_quarkPropagator; ///< QuarkPropagator (pointer not cloned!)
    QuarkGluonVertex* m_vertex; ///< Quark-Gluon Vertex clone

    double m_Lambda2; ///< Ultra-violet cut-off
    double m_epsilon2; ///< Indra-red cut-off
    double m_mu; ///< Renormalization point
    double m_mu2; ///< Renormalization point squared
    double m_m; ///< Renormalized mass
    double m_Ainit, m_Binit; ///< Initialization for the Iterative Solver

    unsigned int m_N; ///< Number of values for representing the propagator
    unsigned int m_2N; ///< Dimension of the problem: \f$ 2 N \f$.
    unsigned int m_Nx; ///< Number of points for integration on x (momentum variable)
    unsigned int m_Nz; ///< Number of points for integration on z (angular variable)

    NumA::VectorD m_roots_x, m_roots_s; ///< Roots for the propagator's expansion (e.g. Chebyshev roots)
    NumA::MatrixD m_interpolationMatrix; ///< Interpolation matrix from the values on the expansion's nodes to the values on quadratures nodes.
    NumA::VectorD m_interpolationMu; ///< Interpolation from the values on the expansion's nodes to the value on the renormalization point.
    NumA::QuadratureIntegrator1D* m_quad_x; ///< Integration quadrature
    NumA::VectorD m_nodes_x, m_nodes_s, m_weights_x; ///< Integration nodes and weights
    NumA::QuadratureIntegrator1D* m_quad_z; ///< Angular integration quadrature
    NumA::VectorD m_nodes_z, m_weights_z; ///< Angular integration nodes and weights

    NumA::Tolerances m_tolerance; ///< Convergence criterion
    NumA::Differences m_difference; ///< Difference in A and B (norm 2)
    unsigned int m_maxIter; ///< Max iterations if the criterion is not fulfilled
    unsigned int m_iters; ///< Number of iterations used
    bool m_converged; ///< Convergence boolean.

    // Keep track of changes
    bool m_changedNx, m_changedNz; ///< Tests if there is a change in the quadrature to avoid recalculations
    bool m_changedVertex, m_changedQP, m_changedGP; ///< Tests if there is a change in the propagator to avoid recalculations
    bool m_changedInit; ///< Tests if there is a change in the init values of A and B, if so reset the iterations

    // Stored calculations
    std::vector<double> m_C; ///< Stored coefficients
};

#endif /* GAPEQUATIONSOLVER_H_ */
