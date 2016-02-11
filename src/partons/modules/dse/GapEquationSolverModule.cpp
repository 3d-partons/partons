/**
 * @file GapEquationSolverModule.cpp
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 22, 2016
 * @version 1.0
 *
 * @class GapEquationSolverModule
 */

#include "../../../../include/partons/modules/dse/GapEquationSolverModule.h"

#include <cmath>
#include <NumA/linear_algebra/matrix/MatrixD.h>
#include <NumA/linear_algebra/vector/VectorD.h>
#include <NumA/root_finding/NewtonMD.h>

#include "../../../../include/partons/beans/dse/GluonPropagator.h"
#include "../../../../include/partons/beans/dse/QuarkPropagator.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/utils/ParameterList.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"

GapEquationSolverModule::GapEquationSolverModule(const std::string &className) :
        ModuleObject(className), m_gluonPropagator(0), m_quarkPropagator(0), m_mu(
                19.), m_m(5.e-3), m_N(50), m_Nx(120), m_Nz(32), m_tolerance(
                1.e-4, 1.e-3), m_maxIter(20), m_Lambda2(1.e5), m_epsilon2(1.e-4) {
}

GapEquationSolverModule::~GapEquationSolverModule() {
    // TODO Auto-generated destructor stub
}

GapEquationSolverModule::GapEquationSolverModule(
        const GapEquationSolverModule& other) :
        ModuleObject(other), m_quarkPropagator(0), m_gluonPropagator(0) {
    m_Lambda2 = other.getLambda2();
    m_epsilon2 = other.getEpsilon2();
    m_mu = other.getMu();
    m_m = other.getM();
    m_N = other.getN();
    m_Nx = other.getNx();
    m_Nz = other.getNz();
    m_tolerance = other.getTolerance();
    m_maxIter = other.getMaxIter();
    if (other.m_gluonPropagator != 0) {
        m_gluonPropagator = other.m_gluonPropagator->clone();
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

        // Gauss-Legendre integration //TODO Implement other cases
        m_quad_x.makeNodeAndWeightVectors(m_Nx);
        m_quad_z.makeNodeAndWeightVectors(m_Nz);
        m_nodes_x = m_quad_x.getNodeNp();
        m_weights_x = m_quad_x.getWeightNp();

        // Propagator expansion's roots
        m_roots_x = m_quarkPropagator->getRoots();
        m_roots_s.assign(m_N,0.);
        for (unsigned int i = 0; i < m_N; i++) {
            m_roots_s.at(i) = m_quarkPropagator->xtos(m_roots_x.at(i));
        }

        // Quadrature nodes and stored coefficients
        m_nodes_s.assign(m_Nx, 0.);
        m_C.assign(m_Nx, 0.);
        double C = (m_Lambda2 * m_epsilon2) * log(m_Lambda2 / m_epsilon2)
                / (2. * pow(2. * PI, 3));
        for (unsigned int k = 0; k < m_Nx; k++) {
            m_nodes_s.at(k) = m_quarkPropagator->xtos(m_nodes_x.at(k));
            m_C.at(k) = C * m_weights_x.at(k)
                    * exp(m_nodes_x.at(k) * log(m_Lambda2 / m_epsilon2));
        }

        // Angular Integrals
        m_ThetaA.assign(m_N, std::vector<double>(m_Nx, 0.));
        m_ThetaM.assign(m_N, std::vector<double>(m_Nx, 0.));
        std::vector<double> parameters(2, 0.);
        for (unsigned int i = 0; i < m_N; i++) {
            parameters.at(0) = m_roots_s.at(i);
            for (unsigned int k = 0; k < m_Nx; k++) {
                parameters.at(1) = m_nodes_s.at(k);
                m_ThetaA.at(i).at(k) = m_quad_z.integrate(this,
                        &GapEquationSolverModule::ThetaA_func, -1., 1.,
                        parameters, m_Nz);
                m_ThetaM.at(i).at(k) = m_quad_z.integrate(this,
                        &GapEquationSolverModule::ThetaM_func, -1., 1.,
                        parameters, m_Nz);
            }
        }
    } else {
        error(__func__, "QuarkPropagator not defined!");
    }
}

void GapEquationSolverModule::computeNewtonInteration() {
    int N_newton = m_N * 2;
    NumA::NewtonMD newtonIteration(N_newton);
    NumA::VectorD a(m_N), b(m_N), X(N_newton), X0(N_newton), G_X0(N_newton);
    NumA::MatrixD J_G_X0(N_newton, N_newton);
    double absDiff_a = 0., absDiff_b = 0., relDiff_a = 0., relDiff_b = 0.; // Difference between two iterations
    bool noConvergence = true; // Convergence test
    for (unsigned int n = 0; n < m_maxIter && noConvergence; n++) {

        G_X0.assign(N_newton, 0.);
        J_G_X0.assign(N_newton, N_newton, 0.);
        for (unsigned int i = 0; i < m_N; i++) {

            G_X0.at(i) = getQuarkPropagator()->evaluateSigmaA(m_roots_s.at(i));
            G_X0.at(m_N + i) = getQuarkPropagator()->evaluateSigmaM(
                    m_roots_s.at(i));
            for (unsigned int k = 0; k < m_Nx; k++) {
                G_X0.at(i) -= m_C.at(k)
                        * H_A_func(m_roots_s.at(i), m_nodes_s.at(k))
                        * m_ThetaA.at(i).at(k);
                G_X0.at(m_N + i) -= m_C.at(k)
                        * H_M_func(m_roots_s.at(i), m_nodes_s.at(k))
                        * m_ThetaM.at(i).at(k);
            }

            for (unsigned int j = 0; j < m_N; j++) {
                J_G_X0.at(i, j) = getQuarkPropagator()->differentiateSigmaA(
                        m_roots_s.at(i), j);
                J_G_X0.at(m_N + i, m_N + j) =
                        getQuarkPropagator()->differentiateSigmaM(
                                m_roots_s.at(i), j);
                for (unsigned int k = 0; k < m_Nx; k++) {
                    J_G_X0.at(i, j) -= m_C.at(k)
                            * H_A_deriv_a(m_roots_s.at(i), m_nodes_s.at(k), j)
                            * m_ThetaA.at(i).at(k);
                    J_G_X0.at(m_N + i, j) -= m_C.at(k)
                            * H_M_deriv_a(m_roots_s.at(i), m_nodes_s.at(k), j)
                            * m_ThetaM.at(i).at(k);
                    J_G_X0.at(i, m_N + j) -= m_C.at(k)
                            * H_A_deriv_b(m_roots_s.at(i), m_nodes_s.at(k), j)
                            * m_ThetaA.at(i).at(k);
                    J_G_X0.at(m_N + i, m_N + j) -= m_C.at(k)
                            * H_M_deriv_b(m_roots_s.at(i), m_nodes_s.at(k), j)
                            * m_ThetaM.at(i).at(k);
                }
            }
        }
        X0 = NumA::VectorD::concatenate(
                m_quarkPropagator->getCoeffsA(),
                m_quarkPropagator->getCoeffsB());
        X = newtonIteration.iterate(X0, G_X0, J_G_X0);
        a = X.sub(0, m_N);
        b = X.sub(m_N, N_newton);

        absDiff_a = (a - m_quarkPropagator->getCoeffsA()).norm();
        absDiff_b = (b - m_quarkPropagator->getCoeffsB()).norm();
        relDiff_a = absDiff_a
                / (m_quarkPropagator->getCoeffsA().norm() + 1.e-16);
        relDiff_b = absDiff_b
                / (m_quarkPropagator->getCoeffsB().norm() + 1.e-16);

        noConvergence = (absDiff_a > getAbsTolerance())
                || (absDiff_b > getAbsTolerance())
                || (relDiff_a > getRelTolerance())
                || (relDiff_b > getRelTolerance());
        m_quarkPropagator->setCoeffsA(a);
        m_quarkPropagator->setCoeffsB(b);

        // Test
        Formatter formatter;
        formatter << "Iteration " << n << " :\n";
        formatter << "A : ";
        for (unsigned int i = 0; i < m_N; i++) {
            formatter << m_quarkPropagator->evaluateA(m_roots_s.at(i)) << " ";
        }
        formatter << "\n" << "B : ";
        for (unsigned int i = 0; i < m_N; i++) {
            formatter << m_quarkPropagator->evaluateB(m_roots_s.at(i)) << " ";
        }
        info(__func__, formatter.str());
    }
}

void GapEquationSolverModule::computeIteration() {
    std::vector<double> SigmaA, SigmaM;
    NumA::VectorD a, b;
    double stored_T;
    double absDiff_a = 0., absDiff_b = 0., relDiff_a = 0., relDiff_b = 0.; // Difference between two iterations
    bool noConvergence = true; // Convergence test
    for (unsigned int n = 0; n < m_maxIter && noConvergence; n++) {
        a = m_quarkPropagator->getCoeffsA();
        b = m_quarkPropagator->getCoeffsB();
        SigmaA.assign(m_N, 0.);
        SigmaM.assign(m_N, 0.);
        for (unsigned int i = 0; i < m_N; i++) {
            for (unsigned int k = 0; k < m_Nx; k++) {
                SigmaA.at(i) += m_C.at(k)
                        * H_A_func(m_roots_s.at(i), m_nodes_s.at(k))
                        * m_ThetaA.at(i).at(k);
                SigmaM.at(i) += m_C.at(k)
                        * H_M_func(m_roots_s.at(i), m_nodes_s.at(k))
                        * m_ThetaM.at(i).at(k);
            }
        }
        m_quarkPropagator->setCoeffsAfromValueOnNodes(SigmaA);
        m_quarkPropagator->setCoeffsBfromValueOnNodes(SigmaM);

        absDiff_a = (a - m_quarkPropagator->getCoeffsA()).norm();
        absDiff_b = (b - m_quarkPropagator->getCoeffsB()).norm();
        relDiff_a = absDiff_a / (a.norm() + 1.e-16);
        relDiff_b = absDiff_b / (b.norm() + 1.e-16);

        noConvergence = (absDiff_a > getAbsTolerance())
                || (absDiff_b > getAbsTolerance())
                || (relDiff_a > getRelTolerance())
                || (relDiff_b > getRelTolerance());

        // Test
        Formatter formatter;
        formatter << "Iteration " << n << " :\n";
        formatter << "A : ";
        for (unsigned int i = 0; i < m_N; i++) {
            formatter << m_quarkPropagator->evaluateA(m_roots_s.at(i)) << " ";
        }
        formatter << "\n" << "B : ";
        for (unsigned int i = 0; i < m_N; i++) {
            formatter << m_quarkPropagator->evaluateB(m_roots_s.at(i)) << " ";
        }
        info(__func__, formatter.str());
    }
}

void GapEquationSolverModule::isModuleWellConfigured() {
}

void GapEquationSolverModule::configure(ParameterList parameters) {
    ModuleObject::configure(parameters);
}

std::string GapEquationSolverModule::toString() const {
    Formatter formatter;
    formatter << ModuleObject::toString();
    return formatter.str();
}

void GapEquationSolverModule::compute(
        GapEquationSolverModule::IterativeType iterativeType) {
    initModule();
    isModuleWellConfigured();
    // Test
//    std::vector<double> nodes_z = gaussLeg_z.getNodeNp();
//    std::vector<double> weights_z = gaussLeg_z.getWeightNp();
//    Formatter formatter;
//    formatter << "Taille des noeuds et poids : ";
//    formatter << nodes_x.size() << " " << weights_x.size() << " "
//            << nodes_z.size() << " " << weights_z.size();
//    info(__func__, formatter.str());

// Chebyshev representation //TODO Implement other cases
//    QPbyChebyshev* pQuarkPropagator = new QPbyChebyshev(m_N, m_m, m_mu,
//            m_Lambda2, m_epsilon2);
//    setQuarkPropagator(pQuarkPropagator);
//    std::vector<double> chebRoots = pQuarkPropagator->getRoots(); //TODO Move to NumA
//
//    // Conversion between x and s
//    std::vector<double> chebRoots_s(m_N, 0.);
//    for (unsigned int i = 0; i < m_N; i++) {
//        chebRoots_s.at(i) = pQuarkPropagator->xtos(chebRoots.at(i));
//    }
//    std::vector<double> nodes_s(m_Nx, 0.);
//    std::vector<double> C(m_Nx, 0.);
//    for (unsigned int k = 0; k < m_Nx; k++) {
//        nodes_s.at(k) = pQuarkPropagator->xtos(m_nodes_x.at(k));
//        C.at(k) = m_C * m_weights_x.at(k)
//                * exp(m_nodes_x.at(k) * log(m_Lambda2 / m_epsilon2));
//    }
//
//    // Angular Integrals
//    std::vector<std::vector<double> > ThetaA(m_N,
//            std::vector<double>(m_Nx, 0.));
//    std::vector<std::vector<double> > ThetaM(m_N,
//            std::vector<double>(m_Nx, 0.));
//    std::vector<double> parameters(2, 0.);
//    for (unsigned int i = 0; i < m_N; i++) {
//        parameters.at(0) = chebRoots_s.at(i);
//        for (unsigned int k = 0; k < m_Nx; k++) {
//            parameters.at(1) = nodes_s.at(k);
//            ThetaA.at(i).at(k) = m_quad_z.integrate(this,
//                    &GapEquationSolverModule::ThetaA_func, -1., 1., parameters,
//                    m_Nz);
//            ThetaM.at(i).at(k) = m_quad_z.integrate(this,
//                    &GapEquationSolverModule::ThetaM_func, -1., 1., parameters,
//                    m_Nz);
//        }
//    }

    switch (iterativeType) {
    case Newton:
        computeNewtonInteration();
        break;
    default:
        computeIteration();
        break;
    }

//    // Natural iterative method //TODO Implement other cases
//    std::vector<double> SigmaA, SigmaM;
//    NumA::VectorD a, b;
//    double stored_T;
//    double absDiff_a = 0., absDiff_b = 0., relDiff_a = 0., relDiff_b = 0.; // Difference between two iterations
//    bool noConvergence = true; // Convergence test
//    for (unsigned int n = 0; n < m_maxIter && noConvergence; n++) {
//        a = NumA::VectorD(m_N, 0.);
//        b = NumA::VectorD(m_N, 0.);
//
//        SigmaA.assign(m_N, 0.);
//        SigmaM.assign(m_N, 0.);
//        for (unsigned int i = 0; i < m_N; i++) {
//            for (unsigned int k = 0; k < m_Nx; k++) {
//                SigmaA.at(i) += C.at(k)
//                        * H_A_func(chebRoots_s.at(i), nodes_s.at(k))
//                        * ThetaA.at(i).at(k);
//                SigmaM.at(i) += C.at(k)
//                        * H_M_func(chebRoots_s.at(i), nodes_s.at(k))
//                        * ThetaM.at(i).at(k);
//            }
//        }
//        for (unsigned int i = 0; i < m_N; i++) {
//            for (unsigned int k = 0; k < m_N; k++) {
//                stored_T = pQuarkPropagator->T(i, chebRoots.at(k));
//                a.at(i) += stored_T * SigmaA.at(k);
//                b.at(i) += stored_T * SigmaM.at(k);
//            }
//            a.at(i) *= 2. / m_N;
//            b.at(i) *= 2. / m_N;
//        }
//        absDiff_a = (a - pQuarkPropagator->getCoeffsA()).norm();
//        absDiff_b = (b - pQuarkPropagator->getCoeffsB()).norm();
//        relDiff_a = absDiff_a
//                / (pQuarkPropagator->getCoeffsA().norm() + 1.e-16);
//        relDiff_b = absDiff_b
//                / (pQuarkPropagator->getCoeffsB().norm() + 1.e-16);
//
//        noConvergence = (absDiff_a > getAbsTolerance())
//                || (absDiff_b > getAbsTolerance())
//                || (relDiff_a > getRelTolerance())
//                || (relDiff_b > getRelTolerance());
//        pQuarkPropagator->setCoeffsA(a);
//        pQuarkPropagator->setCoeffsB(b);
//
//        // Test
//        Formatter formatter;
//        formatter << "Iteration " << n << " :\n";
//        formatter << "A : ";
//        for (unsigned int i = 0; i < m_N; i++) {
//            formatter << pQuarkPropagator->evaluateA(chebRoots_s.at(i)) << " ";
//        }
//        formatter << "\n" << "B : ";
//        for (unsigned int i = 0; i < m_N; i++) {
//            formatter << pQuarkPropagator->evaluateB(chebRoots_s.at(i)) << " ";
//        }
//        info(__func__, formatter.str());
//    }

}

const GluonPropagator* GapEquationSolverModule::getGluonPropagator() const {
    return m_gluonPropagator;
}

void GapEquationSolverModule::setGluonPropagator(
        const GluonPropagator* gluonPropagator) {
    m_gluonPropagator = gluonPropagator->clone();
}

const QuarkPropagator* GapEquationSolverModule::getQuarkPropagator() const {
    return m_quarkPropagator;
}

void GapEquationSolverModule::setQuarkPropagator(
        QuarkPropagator* quarkPropagator) {
// /!\ Don't clone it as setGluonPropagator !!!! IT NEEDS TO BE THE SAME OBJECT!
    m_quarkPropagator = quarkPropagator;
}

double GapEquationSolverModule::getEpsilon2() const {
    return m_epsilon2;
}

void GapEquationSolverModule::setEpsilon2(double epsilon2) {
    m_epsilon2 = epsilon2;
}

double GapEquationSolverModule::getLambda2() const {
    return m_Lambda2;
}

void GapEquationSolverModule::setLambda2(double lambda2) {
    m_Lambda2 = lambda2;
}

double GapEquationSolverModule::getM() const {
    return m_m;
}

void GapEquationSolverModule::setM(double m) {
    m_m = m;
}

int GapEquationSolverModule::getMaxIter() const {
    return m_maxIter;
}

void GapEquationSolverModule::setMaxIter(int maxIter) {
    m_maxIter = maxIter;
}

double GapEquationSolverModule::getMu() const {
    return m_mu;
}

void GapEquationSolverModule::setMu(double mu) {
    m_mu = mu;
}

int GapEquationSolverModule::getN() const {
    return m_N;
}

void GapEquationSolverModule::setN(int n) {
    if (n <= 0) {
        error(__func__,
                "The number of points representing the propagator must be positive!");
    }
    m_N = n;
}

int GapEquationSolverModule::getNx() const {
    return m_Nx;
}

void GapEquationSolverModule::setNx(int nx) {
    m_Nx = nx;
}

int GapEquationSolverModule::getNz() const {
    return m_Nz;
}

void GapEquationSolverModule::setNz(int nz) {
    m_Nz = nz;
}

const Tolerances& GapEquationSolverModule::getTolerance() const {
    return m_tolerance;
}

void GapEquationSolverModule::setTolerance(const Tolerances& tolerance) {
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

double GapEquationSolverModule::ThetaA_func(std::vector<double> z,
        std::vector<double> parameters) const {
    double p2 = parameters.at(0);
    double q2 = parameters.at(1);

    return sqrt(1 - z.at(0) * z.at(0))
            * getGluonPropagator()->evaluateG(k2_func(p2, q2, z.at(0)))
            * F_A_func(p2, q2, k2_func(p2, q2, z.at(0)));
}

double GapEquationSolverModule::ThetaM_func(std::vector<double> z,
        std::vector<double> parameters) const {
    double p2 = parameters.at(0);
    double q2 = parameters.at(1);

    return sqrt(1 - z.at(0) * z.at(0))
            * getGluonPropagator()->evaluateG(k2_func(p2, q2, z.at(0)))
            * F_M_func(p2, q2, k2_func(p2, q2, z.at(0)));
}

double GapEquationSolverModule::k2_func(double p2, double q2, double z) const {
    return p2 + q2 - 2 * sqrt(p2) * sqrt(q2) * z;
}
