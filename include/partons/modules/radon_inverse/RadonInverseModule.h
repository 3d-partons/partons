#ifndef RADON_INVERSE_MODULE_H
#define RADON_INVERSE_MODULE_H

/**
 * @file RadonInverseModule.h
 * @author Cedric MEZRAG
 * @author Nabil Chouika (Irfu/SPhN, CEA Saclay)
 * @date March 18, 2016
 * @version 1.0
 */

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <ElementaryUtils/parameters/Parameters.h>
#include <NumA/linear_algebra/matrix/MatrixD.h>
#include <NumA/linear_algebra/vector/VectorD.h>
#include <NumA/linear_algebra/least_squares/lsmrDense.h>
#include <stddef.h>

#include "../../beans/automation/BaseObjectData.h"
#include "../../beans/double_distribution/DDGauge.h"
#include "../../ModuleObject.h"

namespace NumA {
class FunctionTypeMD;
} /* namespace NumA */

/**
 * @class RadonInverseModule
 *
 * @brief Radon Inverse module: uses an incomplete GPD (for example in DGLAP region only) and produces a DD.
 */

class RadonInverseModule: public ModuleObject {
public:
    static const double DD_DOMAIN_HALF_EDGE;

    RadonInverseModule(const std::string &className);
    virtual ~RadonInverseModule();

    /**
     * Clone
     *
     * @return
     */
    virtual RadonInverseModule* clone() const = 0;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual void buildSystem();
    virtual void buildSystem(NumA::FunctionTypeMD* pGPDFunction);
    virtual void buildMatrix(size_t rows = 0) = 0;
    virtual void buildGPDVector();
    virtual void buildGPDVector(NumA::FunctionTypeMD* pGPDFunction);

    virtual void solve();

    /**
     * Main compute method: builds the system (if needed) and solves it.
     * Calls buildSystem() and solve().
     */
    virtual void compute();
    /**
     * Main compute method: builds the system (if needed) and solves it.
     * Calls buildSystem() and solve().
     * @param pGPDFunction GPD function to invert (functor).
     */
    virtual void compute(NumA::FunctionTypeMD* pGPDFunction);

    virtual double computeDD(double beta, double alpha) = 0;
    virtual double computeGPD(double x, double xi) = 0;

    // ##### GETTERS & SETTERS #####

    NumA::FunctionTypeMD* getGPDFunction() const;
    void setGPDFunction(NumA::FunctionTypeMD* pGPDFunction);
    size_t getN() const;
    void setN(size_t N);
    size_t getRows() const;
    size_t getCols() const;
    size_t getRank() const;
    bool isMatrixBuilt() const;
    const DDGauge& getGauge() const;
    void setGauge(const DDGauge& gauge);
    bool isGaugeInVector() const;
    void setGaugeInVector(bool gaugeInVector);
    size_t getMaxiter() const;
    void setMaxiter(size_t maxiter);
    double getTolerance() const;
    void setTolerance(double tolerance);

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

protected:
    /**
     * Copy constructor
     */
    RadonInverseModule(const RadonInverseModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    NumA::FunctionTypeMD* m_pGPDFunction; ///< Function of two variables x and xi, to invert.

    NumA::MatrixD m_radonMatrix; ///< Matrix of the linear problem (Radon Transform).
    NumA::VectorD m_gpdVector; ///< Right-hand side of the equation: vector of the GPD values.
    NumA::VectorD m_ddVector; ///< Result of the discretized DD.
    std::vector<std::pair<double, double> > m_gpdNodes; ///< Nodes in the limited GPD space.

    size_t m_N; ///< Size of the mesh in each direction.
    size_t m_rank; ///< Rank of the Radon Matrix.
    size_t m_n; ///< Number of unknowns, i.e. size of m_ddVector;
    size_t m_m; ///< Number of equations, i.e. size of m_gpdVector;

    NumA::lsmrDense m_solver; ///< Linear solver. TODO: Use a more general object to allow for different solvers.
    double m_tolerance; ///< Tolerance for the iterative solver.
    size_t m_maxiter; ///< Maximum number of iterations.

    /**
     * Builds the mesh (cells) in the DD domain.
     */
    virtual void buildMesh() = 0;

    bool m_matrixBuilt; ///< True if the matrix is already built and doesn't need to be rebuilt.

    DDGauge m_gauge; ///< Gauge used for the inversion.
    bool m_gaugeInVector; ///< True if the 1CDD gauge factor is taken into account in the GPD (divided by the factor), False if it is in the matrix (multiplied by the factor).
private:
};

#endif /* RADON_INVERSE_MODULE_H */
