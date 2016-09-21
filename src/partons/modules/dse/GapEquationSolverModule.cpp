/**
 * @file GapEquationSolverModule.cpp
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 22, 2016
 * @version 1.0
 *
 * @class GapEquationSolverModule
 */

#include "../../../../include/partons/modules/dse/GapEquationSolverModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/integration/one_dimension/GaussLegendreIntegrator1D.h>
#include <cmath>

#include "../../../../include/partons/beans/dse/MTGluonPropagator.h"
#include "../../../../include/partons/beans/dse/QuarkPropagator.h"
#include "../../../../include/partons/beans/dse/RLVertex.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

GapEquationSolverModule::GapEquationSolverModule(const std::string &className) :
        ModuleObject(className), m_gluonPropagator(0), m_quarkPropagator(0), m_vertex(
                0), m_quad_x(0), m_quad_z(0), m_mu(19.), m_mu2(19 * 19), m_m(
                5.e-3), m_N(50), m_2N(100), m_Nx(120), m_Nz(32), m_tolerance(
                1.e-4, 1.e-3), m_maxIter(20), m_Lambda2(1.e5), m_epsilon2(
                1.e-4), m_Ainit(1.), m_Binit(m_m), m_iters(0), m_converged(
                false), m_changedQP(true), m_changedGP(true), m_changedVertex(
                true), m_changedNx(true), m_changedNz(true), m_changedInit(
                false) {
}

GapEquationSolverModule::~GapEquationSolverModule() {
    // Delete GluonPropagator clone
    delete m_gluonPropagator;
    m_gluonPropagator = 0;
    // Delete QuarkGluonVertex clone
    delete m_vertex;
    m_vertex = 0;
    // /!\ Do no delete QuarkPropagator! IT IS NOT A CLONE!

    // Delete Momentum quadrature
    delete m_quad_x;
    m_quad_x = 0;
    // Delete Angular quadrature
    delete m_quad_z;
    m_quad_z = 0;
}

GapEquationSolverModule::GapEquationSolverModule(
        const GapEquationSolverModule& other) :
        ModuleObject(other), m_quarkPropagator(0), m_gluonPropagator(0), m_vertex(
                0) {
    //TODO Add objects copy
    m_Lambda2 = other.getLambda2();
    m_epsilon2 = other.getEpsilon2();
    m_mu = other.getMu();
    m_mu2 = other.getMu2();
    m_m = other.getM();
    m_N = other.getN();
    m_2N = other.get2N();
    m_Nx = other.getNx();
    m_Nz = other.getNz();
    m_tolerance = other.getTolerance();
    m_maxIter = other.getMaxIter();
    m_Ainit = other.getAinit();
    m_Binit = other.getBinit();
    m_iters = other.m_iters;
    m_converged = other.m_converged;
    m_changedQP = other.m_changedQP;
    m_changedGP = other.m_changedGP;
    m_changedVertex = other.m_changedVertex;
    m_changedNx = other.m_changedNx;
    m_changedNz = other.m_changedNz;
    m_changedInit = other.m_changedInit;
    m_quarkPropagator = other.m_quarkPropagator;
    if (other.m_gluonPropagator != 0) {
        m_gluonPropagator = other.m_gluonPropagator->clone();
    } else {
        m_gluonPropagator = 0;
    }
    if (other.m_vertex != 0) {
        m_vertex = other.m_vertex->clone();
    } else {
        m_vertex = 0;
    }
    if (other.m_quad_x != 0) {
        m_quad_x = other.m_quad_x->clone();
    } else {
        m_quad_x = 0;
    }
    if (other.m_quad_z != 0) {
        m_quad_z = other.m_quad_z->clone();
    } else {
        m_quad_z = 0;
    }
}

void GapEquationSolverModule::initModule() {

    if (m_quarkPropagator != 0) {
        // Harmonization between QuarkPropagator variables and this module
        setN(m_quarkPropagator->getN());
        setMu(m_quarkPropagator->getMu());
        setM(m_quarkPropagator->getM());
        setLambda2(m_quarkPropagator->getLambda2());
        setEpsilon2(m_quarkPropagator->getEpsilon2());
    } else {
        ElemUtils::CustomException(getClassName(), __func__,
                "QuarkPropagator not defined!");
    }

    // GluonPropagator and QuarkGluonVertex default initialization if not available
    if (m_gluonPropagator == 0) {
        warn(__func__,
                "GluonPropagator not defined! Using default Maris-Tandy (MT) model instead.");
        m_gluonPropagator = new MTGluonPropagator();
    }
    if (m_vertex == 0) {
        warn(__func__,
                "QuarkGluonVertex not defined! Using default Rainbow-Ladder (RL) model instead.");
        m_vertex = new RLVertex();
    }

    // Quadratures' default initialization if not available
    if (m_quad_x == 0) {
        warn(__func__,
                "Momentum quadrature not defined! Using default Gauss-Legendre quadrature.");
        m_quad_x = new NumA::GaussLegendreIntegrator1D();
    }
    if (m_quad_z == 0) {
        warn(__func__,
                "Angular quadrature not defined! Using default Gauss-Legendre quadrature.");
        m_quad_z = new NumA::GaussLegendreIntegrator1D();
    }

    // Quadrature nodes and weights
    if (m_changedNx) {
        m_quad_x->setN(m_Nx);
        m_nodes_x = m_quad_x->getNodes();
        m_weights_x = m_quad_x->getWeights();
    }
    if (m_changedNz) {
        m_quad_z->setN(m_Nz);
        m_nodes_z = m_quad_z->getNodes();
        m_weights_z = m_quad_z->getWeights();
    }

    // Propagator expansion's roots
    if (m_changedQP) {
        m_roots_x = m_quarkPropagator->getRoots();
        m_roots_s.assign(m_N + 1, 0.);
        for (unsigned int i = 0; i < m_N; i++) {
            m_roots_s.at(i) = m_quarkPropagator->xtos(m_roots_x.at(i));
        }
        m_roots_s.at(m_N) = m_mu2;
        m_interpolationMatrix = m_quarkPropagator->getInterpolationMatrix(
                m_nodes_x);
        m_interpolationMu = m_quarkPropagator->getInterpolationVector(
                m_quarkPropagator->stox(m_mu2));
    }

    // Quadrature nodes and stored coefficients
    if (m_changedQP || m_changedNx) {
        m_nodes_s.assign(m_Nx, 0.);
        m_C.assign(m_Nx, 0.);
        double C = (m_Lambda2 * m_epsilon2) * log(m_Lambda2 / m_epsilon2)
                / (2. * pow(2. * PI, 3));
        for (unsigned int k = 0; k < m_Nx; k++) {
            m_nodes_s.at(k) = m_quarkPropagator->xtos(m_nodes_x.at(k));
            m_C.at(k) = C * m_weights_x.at(k)
                    * exp(m_nodes_x.at(k) * log(m_Lambda2 / m_epsilon2));
        }
    }

    // Restart the iterations if necessary
    if (m_changedInit || m_changedQP) {
        reset();
    }

    // Set the booleans to false to not recompute things unnecessarily.
    // This part needs to be implemented in the daughter class!
    // DO NOT UNCOMMENT HERE, but copy it in the initModule of the daughter class.
//    setChangedQp(false);
//    setChangedGp(false);
//    setChangedVertex(false);
//    setChangedNx(false);
//    setChangedNz(false);
//    setChangedInit(false);
}

void GapEquationSolverModule::isModuleWellConfigured() {
}

void GapEquationSolverModule::configure(
        const ElemUtils::Parameters &parameters) {
    ModuleObject::configure(parameters);
}

std::string GapEquationSolverModule::toString() const {
    return ElemUtils::Formatter() << ModuleObject::toString();
}

void GapEquationSolverModule::compute(
        GapEquationSolverModule::IterativeType iterativeType) {
    initModule();
    isModuleWellConfigured();

//    switch (iterativeType) {
//    case Newton:
//        computeNewtonIteration();
//        break;
//    case Broyden:
//        computeBroydenIteration();
//        break;
//    default:
//        computeIteration();
//        break;
//    }
//    computeIterations(iterativeType);
    computeBroydenIteration();

    // Write the result
    info(__func__, "p2 [GeV^2] ; A ; B [GeV] ; M [GeV]");
    double p2, A, B, M;
    for (unsigned int i = 0; i < m_N; i++) {
        ElemUtils::Formatter formatter;
        p2 = m_roots_s.at(i);
        A = m_quarkPropagator->evaluateA(p2);
        B = m_quarkPropagator->evaluateB(p2);
        M = B / A;
        formatter << p2 << " ; " << A << " ; " << B << " ; " << M;
        info(__func__, formatter.str());
    }

}

const GluonPropagator* GapEquationSolverModule::getGluonPropagator() const {
    return m_gluonPropagator;
}

void GapEquationSolverModule::setGluonPropagator(
        const GluonPropagator* gluonPropagator) {
    if (m_gluonPropagator != gluonPropagator) {
        if (m_gluonPropagator != 0) {
            delete m_gluonPropagator;
            m_gluonPropagator = 0;
        }
        m_changedGP = true;
        m_gluonPropagator = gluonPropagator->clone();
    } else {
        warn(__func__,
                "Trying to set the GluonPropagator pointer with the same pointer, you're playing a dangerous game!");
    }
}

const QuarkPropagator* GapEquationSolverModule::getQuarkPropagator() const {
    return m_quarkPropagator;
}

// Protected non-const method to be used in daughter classes.
QuarkPropagator* GapEquationSolverModule::getQuarkPropagator() {
    return m_quarkPropagator;
}

void GapEquationSolverModule::setQuarkPropagator(
        QuarkPropagator* quarkPropagator) {
    m_changedQP = m_changedQP || (m_quarkPropagator != quarkPropagator)
            || (m_quarkPropagator->getClassName()
                    != quarkPropagator->getClassName());

// /!\ Don't clone it as setGluonPropagator !!!! IT NEEDS TO BE THE SAME OBJECT! And do not destroy in the destructor!
    m_quarkPropagator = quarkPropagator;
    setN(m_quarkPropagator->getN());
    setMu(m_quarkPropagator->getMu());
    setM(m_quarkPropagator->getM());
    setLambda2(m_quarkPropagator->getLambda2());
    setEpsilon2(m_quarkPropagator->getEpsilon2());
    setBinit(m_quarkPropagator->getM());
}

const QuarkGluonVertex* GapEquationSolverModule::getVertex() const {
    return m_vertex;
}

void GapEquationSolverModule::setVertex(const QuarkGluonVertex* vertex) {
    if (m_vertex != vertex) {
        if (m_vertex != 0) {
            delete m_vertex;
            m_vertex = 0;
        }
        m_changedVertex = true;
        m_vertex = vertex->clone();
    } else {
        warn(__func__,
                "Trying to set the QuarkGluonVertex pointer with the same pointer, you're playing a dangerous game!");
    }
}

double GapEquationSolverModule::getEpsilon2() const {
    return m_epsilon2;
}

void GapEquationSolverModule::setEpsilon2(double epsilon2) {
    if (epsilon2 <= 0) {
        ElemUtils::CustomException(getClassName(), __func__,
                "The IR cut-off must be positive (and small enough)!");
    }
    m_changedQP = m_changedQP || (epsilon2 != m_epsilon2);
    m_epsilon2 = epsilon2;
}

double GapEquationSolverModule::getLambda2() const {
    return m_Lambda2;
}

void GapEquationSolverModule::setLambda2(double lambda2) {
    if (lambda2 <= 0) {
        ElemUtils::CustomException(getClassName(), __func__,
                "The UV cut-off must be positive (and large enough)!");
    }
    m_changedQP = m_changedQP || (lambda2 != m_Lambda2);
    m_Lambda2 = lambda2;
}

double GapEquationSolverModule::getM() const {
    return m_m;
}

void GapEquationSolverModule::setM(double m) {
    if (m < 0) {
        ElemUtils::CustomException(getClassName(), __func__,
                "The quark mass must be positive!");
    }
    m_changedQP = m_changedQP || (m != m_m);
    m_m = m;
}

unsigned int GapEquationSolverModule::getMaxIter() const {
    return m_maxIter;
}

void GapEquationSolverModule::setMaxIter(unsigned int maxIter) {
    m_maxIter = maxIter;
}

double GapEquationSolverModule::getMu() const {
    return m_mu;
}

void GapEquationSolverModule::setMu(double mu) {
    if (mu <= 0) {
        ElemUtils::CustomException(getClassName(), __func__,
                "The renormalization point must be positive (and large enough)!");
    }
    m_changedQP = m_changedQP || (mu != m_mu);
    m_mu = mu;
    m_mu2 = mu * mu;
}

double GapEquationSolverModule::getMu2() const {
    return m_mu2;
}

unsigned int GapEquationSolverModule::getN() const {
    return m_N;
}

void GapEquationSolverModule::setN(unsigned int n) {
    if (n <= 0) {
        ElemUtils::CustomException(getClassName(), __func__,
                "The number of points representing the propagator must be positive!");
    }
    m_changedQP = m_changedQP || (n != m_N);
    m_N = n;
    m_2N = 2 * n;
}

unsigned int GapEquationSolverModule::get2N() const {
    return m_2N;
}

unsigned int GapEquationSolverModule::getNx() const {
    return m_Nx;
}

void GapEquationSolverModule::setNx(unsigned int nx) {
    if (nx <= 0) {
        ElemUtils::CustomException(getClassName(), __func__,
                "The number of nodes of integration must be positive!");
    }
    m_changedNx = m_changedNx || (nx != m_Nx);
    m_Nx = nx;
}

unsigned int GapEquationSolverModule::getNz() const {
    return m_Nz;
}

void GapEquationSolverModule::setNz(unsigned int nz) {
    if (nz <= 0) {
        ElemUtils::CustomException(getClassName(), __func__,
                "The number of nodes of integration must be positive!");
    }
    m_changedNz = m_changedNz || (nz != m_Nz);
    m_Nz = nz;
}

const NumA::Tolerances& GapEquationSolverModule::getTolerance() const {
    return m_tolerance;
}

void GapEquationSolverModule::setTolerance(const NumA::Tolerances& tolerance) {
    m_tolerance = tolerance;
}

double GapEquationSolverModule::getAbsTolerance() const {
    return m_tolerance.getAbsoluteTolerance();
}

void GapEquationSolverModule::setAbsTolerance(double absTolerance) {
    m_tolerance.setAbsoluteTolerance(absTolerance);
}

double GapEquationSolverModule::getRelTolerance() const {
    return m_tolerance.getRelativeTolerance();
}

void GapEquationSolverModule::setRelTolerance(double relTolerance) {
    m_tolerance.setRelativeTolerance(relTolerance);
}

double GapEquationSolverModule::getAinit() const {
    return m_Ainit;
}

void GapEquationSolverModule::setAinit(double ainit) {
    m_changedInit = m_changedInit || (ainit != m_Ainit);
    m_Ainit = ainit;
}

double GapEquationSolverModule::getBinit() const {
    return m_Binit;
}

void GapEquationSolverModule::setBinit(double binit) {
    m_changedInit = m_changedInit || (binit != m_Binit);
    m_Binit = binit;
}

void GapEquationSolverModule::reset() {
    m_iters = 0;
    m_quarkPropagator->setA(getAinit());
    m_quarkPropagator->setB(getBinit());
    setConverged(false);
}

double GapEquationSolverModule::k2_func(double p2, double q2, double z) const {
    return p2 + q2 - 2 * sqrt(p2) * sqrt(q2) * z;
}

double GapEquationSolverModule::t2_func(double p2, double q2, double z) const {
    return p2 + q2 + 2 * sqrt(p2) * sqrt(q2) * z;
}

const std::vector<double>& GapEquationSolverModule::getC() const {
    return m_C;
}

void GapEquationSolverModule::setC(const std::vector<double>& c) {
    m_C = c;
}

bool GapEquationSolverModule::isChangedGp() const {
    return m_changedGP;
}

void GapEquationSolverModule::setChangedGp(bool changedGp) {
    m_changedGP = changedGp;
}

bool GapEquationSolverModule::isChangedInit() const {
    return m_changedInit;
}

void GapEquationSolverModule::setChangedInit(bool changedInit) {
    m_changedInit = changedInit;
}

bool GapEquationSolverModule::isChangedNx() const {
    return m_changedNx;
}

void GapEquationSolverModule::setChangedNx(bool changedNx) {
    m_changedNx = changedNx;
}

bool GapEquationSolverModule::isChangedNz() const {
    return m_changedNz;
}

void GapEquationSolverModule::setChangedNz(bool changedNz) {
    m_changedNz = changedNz;
}

bool GapEquationSolverModule::isChangedQp() const {
    return m_changedQP;
}

void GapEquationSolverModule::setChangedQp(bool changedQp) {
    m_changedQP = changedQp;
}

bool GapEquationSolverModule::isChangedVertex() const {
    return m_changedVertex;
}

void GapEquationSolverModule::setChangedVertex(bool changedVertex) {
    m_changedVertex = changedVertex;
}

unsigned int GapEquationSolverModule::getIters() const {
    return m_iters;
}

void GapEquationSolverModule::setIters(unsigned int iters) {
    m_iters = iters;
}

const NumA::VectorD& GapEquationSolverModule::getNodesS() const {
    return m_nodes_s;
}

const NumA::VectorD& GapEquationSolverModule::getNodesX() const {
    return m_nodes_x;
}

const NumA::VectorD& GapEquationSolverModule::getNodesZ() const {
    return m_nodes_z;
}

const NumA::VectorD& GapEquationSolverModule::getRootsS() const {
    return m_roots_s;
}

const NumA::VectorD& GapEquationSolverModule::getRootsX() const {
    return m_roots_x;
}

const NumA::VectorD& GapEquationSolverModule::getWeightsX() const {
    return m_weights_x;
}

const NumA::VectorD& GapEquationSolverModule::getWeightsZ() const {
    return m_weights_z;
}

bool GapEquationSolverModule::isConverged() const {
    return m_converged;
}

void GapEquationSolverModule::setConverged(bool converged) {
    m_converged = converged;
}

const NumA::Differences& GapEquationSolverModule::getDifference() const {
    return m_difference;
}

void GapEquationSolverModule::setDifference(
        const NumA::Differences& difference) {
    m_difference = difference;
}

double GapEquationSolverModule::getAbsDifference() const {
    return m_difference.getAbsoluteDifference();
}

double GapEquationSolverModule::getRelDifference() const {
    return m_difference.getRelativeDifference();
}

void GapEquationSolverModule::setAbsDifference(double absDifference) {
    m_difference.setAbsoluteDifference(absDifference);
}

void GapEquationSolverModule::setRelDifference(double relDifference) {
    m_difference.setRelativeDifference(relDifference);
}

const NumA::MatrixD& GapEquationSolverModule::getInterpolationMatrix() const {
    return m_interpolationMatrix;
}

void GapEquationSolverModule::setInterpolationMatrix(
        const NumA::MatrixD& interpolationMatrix) {
    m_interpolationMatrix = interpolationMatrix;
}

const NumA::VectorD& GapEquationSolverModule::getInterpolationMu() const {
    return m_interpolationMu;
}

void GapEquationSolverModule::setInterpolationMu(
        const NumA::VectorD& interpolationMu) {
    m_interpolationMu = interpolationMu;
}
