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

#include "../../../../include/partons/beans/dse/GPMT.h"
#include "../../../../include/partons/beans/dse/QuarkPropagator.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/utils/ParameterList.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"

GapEquationSolverModule::GapEquationSolverModule(const std::string &className) :
        ModuleObject(className), m_gluonPropagator(0), m_quarkPropagator(0), m_mu(
                19.), m_m(5.e-3), m_N(50), m_Nx(120), m_Nz(32), m_tolerance(
                1.e-4, 1.e-3), m_maxIter(20), m_Lambda2(1.e5), m_epsilon2(
                1.e-4), m_Ainit(1.), m_Binit(m_m), m_iters(0), m_changeQP(true), m_changeGP(
                true), m_changeNx(true), m_changeNz(true), m_changeInit(false) {
}

GapEquationSolverModule::~GapEquationSolverModule() {
    delete m_gluonPropagator;
    m_gluonPropagator = 0;
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
    m_Ainit = other.getAinit();
    m_Binit = other.getBinit();
    m_iters = other.m_iters;
    m_changeQP = other.m_changeQP;
    m_changeGP = other.m_changeGP;
    m_changeNx = other.m_changeNx;
    m_changeNz = other.m_changeNz;
    m_changeInit = other.m_changeInit;
    if (other.m_gluonPropagator != 0) {
        m_gluonPropagator = other.m_gluonPropagator->clone();
    }
    if (other.m_quarkPropagator != 0) {
        m_quarkPropagator = other.m_quarkPropagator;
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
        if (m_changeNx) {
            m_quad_x.makeNodeAndWeightVectors(m_Nx);
            m_nodes_x = m_quad_x.getNodeNp();
            m_weights_x = m_quad_x.getWeightNp();
        }
        if (m_changeNz) {
            m_quad_z.makeNodeAndWeightVectors(m_Nz);
            m_nodes_z = m_quad_z.getNodeNp();
            m_weights_z = m_quad_z.getWeightNp();
        }

        // Propagator expansion's roots
        if (m_changeQP) {
            m_roots_x = m_quarkPropagator->getRoots();
            m_roots_s.assign(m_N, 0.);
            for (unsigned int i = 0; i < m_N; i++) {
                m_roots_s.at(i) = m_quarkPropagator->xtos(m_roots_x.at(i));
            }
        }

        // Quadrature nodes and stored coefficients
        if (m_changeQP || m_changeNx) {
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

        // Angular Integrals
        if (m_changeQP || m_changeGP || m_changeNx || m_changeNz) {
            m_ThetaA.assign(m_N, std::vector<double>(m_Nx, 0.));
            m_ThetaM.assign(m_N, std::vector<double>(m_Nx, 0.));
            double k2 = 0., G_k2 = 0., Cz = 0.;
            if (m_gluonPropagator == 0) {
                warn(__func__,
                        "GluonPropagator not defined! Using default MT model instead.");
                m_gluonPropagator = new GPMT();
            }
            for (unsigned int l = 0; l < m_Nz; l++) {
                Cz = m_weights_z.at(l)
                        * sqrt(1 - m_nodes_z.at(l) * m_nodes_z.at(l));
                for (unsigned int i = 0; i < m_N; i++) {
                    for (unsigned int k = 0; k < m_Nx; k++) {
                        k2 = k2_func(m_roots_s.at(i), m_nodes_s.at(k),
                                m_nodes_z.at(l));
                        G_k2 = getGluonPropagator()->evaluateG(k2);
                        m_ThetaA.at(i).at(k) += Cz * G_k2
                                * F_A_func(m_roots_s.at(i), m_nodes_s.at(k),
                                        k2);
                        m_ThetaM.at(i).at(k) += Cz * G_k2
                                * F_M_func(m_roots_s.at(i), m_nodes_s.at(k),
                                        k2);
                    }
                }
            }
        }
        if (m_changeInit || m_changeQP) {
            m_iters = 0;
        }

        m_changeQP = false;
        m_changeGP = false;
        m_changeNx = false;
        m_changeNz = false;
        m_changeInit = false;
    } else {
        error(__func__, "QuarkPropagator not defined!");
    }
}

void GapEquationSolverModule::isModuleWellConfigured() {
}

void GapEquationSolverModule::computeNewtonInteration() {
    int N_newton = m_N * 2;
    NumA::NewtonMD newtonIteration(N_newton);
    NumA::VectorD a(m_N), b(m_N), X(N_newton), X0(N_newton), G_X0(N_newton);
    NumA::MatrixD J_G_X0(N_newton, N_newton);
    double absDiff_a = 0., absDiff_b = 0., relDiff_a = 0., relDiff_b = 0.; // Difference between two iterations
    bool noConvergence = true; // Convergence test
    std::vector<double> qpFunctionsAtRoots;
    std::vector<double> qpFunctionsAtNodes;
    std::vector<QuarkPropagator::QPFunction> qpFunctions;
    std::vector<double> SigmaA_r, SigmaM_r, A_r, B_r, sigmaV_r, sigmaS_r;
    std::vector<double> A_n, B_n, sigmaV_n, sigmaS_n;
    std::vector<std::vector<double> > dSigmaA_r, dSigmaM_r, dA_r, dB_r,
            dsigmaV_a_r, dsigmaV_b_r, dsigmaS_a_r, dsigmaS_b_r;
    std::vector<std::vector<double> > dA_n, dB_n, dsigmaV_a_n, dsigmaV_b_n,
            dsigmaS_a_n, dsigmaS_b_n;
    double denom_r, denom_n, sigmaV2_r, sigmaS2_r, sigmaV2_n, sigmaS2_n;

    unsigned int n;
    for (n = m_iters; n < m_maxIter && noConvergence; n++) {

        G_X0.assign(N_newton, 0.);
        J_G_X0.assign(N_newton, N_newton, 0.);

        SigmaA_r.assign(m_N, 0.);
        SigmaM_r.assign(m_N, 0.);
        A_r.assign(m_N, m_Ainit);
        B_r.assign(m_N, m_Binit);
        sigmaV_r.assign(m_N, 0.);
        sigmaS_r.assign(m_N, 0.);

        dSigmaA_r.assign(m_N, std::vector<double>(m_N, 0.));
        dSigmaM_r.assign(m_N, std::vector<double>(m_N, 0.));
        dA_r.assign(m_N, std::vector<double>(m_N, 0.));
        dB_r.assign(m_N, std::vector<double>(m_N, 0.));
        dsigmaV_a_r.assign(m_N, std::vector<double>(m_N, 0.));
        dsigmaV_b_r.assign(m_N, std::vector<double>(m_N, 0.));
        dsigmaS_a_r.assign(m_N, std::vector<double>(m_N, 0.));
        dsigmaS_b_r.assign(m_N, std::vector<double>(m_N, 0.));

        A_n.assign(m_Nx, m_Ainit);
        B_n.assign(m_Nx, m_Binit);
        sigmaV_n.assign(m_Nx, 0.);
        sigmaS_n.assign(m_Nx, 0.);

        dA_n.assign(m_Nx, std::vector<double>(m_N, 0.));
        dB_n.assign(m_Nx, std::vector<double>(m_N, 0.));
        dsigmaV_a_n.assign(m_Nx, std::vector<double>(m_N, 0.));
        dsigmaV_b_n.assign(m_Nx, std::vector<double>(m_N, 0.));
        dsigmaS_a_n.assign(m_Nx, std::vector<double>(m_N, 0.));
        dsigmaS_b_n.assign(m_Nx, std::vector<double>(m_N, 0.));

        for (unsigned int i = 0; i < m_N; i++) {
            if (n > 0) {
                qpFunctions.assign(4, QuarkPropagator::SigmaA);
                qpFunctions.at(1) = QuarkPropagator::SigmaM;
                qpFunctions.at(2) = QuarkPropagator::A;
                qpFunctions.at(3) = QuarkPropagator::B;
                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
                        m_roots_s.at(i));
                SigmaA_r.at(i) = qpFunctionsAtRoots.at(0);
                SigmaM_r.at(i) = qpFunctionsAtRoots.at(1);
                A_r.at(i) = qpFunctionsAtRoots.at(2);
                B_r.at(i) = qpFunctionsAtRoots.at(3);
            }
            denom_r = m_roots_s.at(i) * A_r.at(i) * A_r.at(i)
                    + B_r.at(i) * B_r.at(i);
            sigmaV_r.at(i) = A_r.at(i) / denom_r;
            sigmaS_r.at(i) = B_r.at(i) / denom_r;

            sigmaV2_r = sigmaV_r.at(i) * sigmaV_r.at(i);
            sigmaS2_r = sigmaS_r.at(i) * sigmaS_r.at(i);

            for (unsigned int j = 0; j < m_N; j++) {
                qpFunctions.assign(4, QuarkPropagator::dSigmaA);
                qpFunctions.at(1) = QuarkPropagator::dSigmaM;
                qpFunctions.at(2) = QuarkPropagator::dA;
                qpFunctions.at(3) = QuarkPropagator::dB;
                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
                        m_roots_s.at(i), j);
                dSigmaA_r.at(i).at(j) = qpFunctionsAtRoots.at(0);
                dSigmaM_r.at(i).at(j) = qpFunctionsAtRoots.at(1);
                dA_r.at(i).at(j) = qpFunctionsAtRoots.at(2);
                dB_r.at(i).at(j) = qpFunctionsAtRoots.at(3);
                dsigmaV_a_r.at(i).at(j) = dA_r.at(i).at(j)
                        * (sigmaS2_r - m_roots_s.at(i) * sigmaV2_r);
                dsigmaV_b_r.at(i).at(j) = -2 * dB_r.at(i).at(j) * sigmaV_r.at(i)
                        * sigmaS_r.at(i);
                dsigmaS_a_r.at(i).at(j) = -2 * m_roots_s.at(i)
                        * dA_r.at(i).at(j) * sigmaV_r.at(i) * sigmaS_r.at(i);
                dsigmaS_b_r.at(i).at(j) = dB_r.at(i).at(j)
                        * (m_roots_s.at(i) * sigmaV2_r - sigmaS2_r);
            }

        }

        for (unsigned int k = 0; k < m_Nx; k++) {
            if (n > 0) {
                qpFunctions.assign(2, QuarkPropagator::A);
                qpFunctions.at(1) = QuarkPropagator::B;
                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
                        m_nodes_s.at(k));
                A_n.at(k) = qpFunctionsAtRoots.at(0);
                B_n.at(k) = qpFunctionsAtRoots.at(1);
            }
            denom_n = m_nodes_s.at(k) * A_n.at(k) * A_n.at(k)
                    + B_n.at(k) * B_n.at(k);
            sigmaV_n.at(k) = A_n.at(k) / denom_n;
            sigmaS_n.at(k) = B_n.at(k) / denom_n;

            sigmaV2_n = sigmaV_n.at(k) * sigmaV_n.at(k);
            sigmaS2_n = sigmaS_n.at(k) * sigmaS_n.at(k);

            for (unsigned int j = 0; j < m_N; j++) {
                qpFunctions.assign(2, QuarkPropagator::dA);
                qpFunctions.at(1) = QuarkPropagator::dB;
                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
                        m_nodes_s.at(k), j);
                dA_n.at(k).at(j) = qpFunctionsAtRoots.at(0);
                dB_n.at(k).at(j) = qpFunctionsAtRoots.at(1);
                dsigmaV_a_n.at(k).at(j) = dA_n.at(k).at(j)
                        * (sigmaS2_n - m_nodes_s.at(k) * sigmaV2_n);
                dsigmaV_b_n.at(k).at(j) = -2 * dB_n.at(k).at(j) * sigmaV_n.at(k)
                        * sigmaS_n.at(k);
                dsigmaS_a_n.at(k).at(j) = -2 * m_nodes_s.at(k)
                        * dA_n.at(k).at(j) * sigmaV_n.at(k) * sigmaS_n.at(k);
                dsigmaS_b_n.at(k).at(j) = dB_n.at(k).at(j)
                        * (m_nodes_s.at(k) * sigmaV2_n - sigmaS2_n);
            }

        }

        for (unsigned int i = 0; i < m_N; i++) {

            G_X0.at(i) = SigmaA_r.at(i);
            G_X0.at(m_N + i) = SigmaM_r.at(i);
            for (unsigned int k = 0; k < m_Nx; k++) {
                G_X0.at(i) -= m_C.at(k)
                        * H_A_func(A_r.at(i), A_n.at(k), B_r.at(i), B_n.at(k),
                                sigmaV_r.at(i), sigmaV_n.at(k), sigmaS_r.at(i),
                                sigmaS_n.at(k)) * m_ThetaA.at(i).at(k);
                G_X0.at(m_N + i) -= m_C.at(k)
                        * H_M_func(A_r.at(i), A_n.at(k), B_r.at(i), B_n.at(k),
                                sigmaV_r.at(i), sigmaV_n.at(k), sigmaS_r.at(i),
                                sigmaS_n.at(k)) * m_ThetaM.at(i).at(k);
            }

//            G_X0.at(i) = getQuarkPropagator()->evaluateSigmaA(m_roots_s.at(i));
//            G_X0.at(m_N + i) = getQuarkPropagator()->evaluateSigmaM(
//                    m_roots_s.at(i));
//            for (unsigned int k = 0; k < m_Nx; k++) {
//                G_X0.at(i) -= m_C.at(k)
//                        * H_A_func(m_roots_s.at(i), m_nodes_s.at(k))
//                        * m_ThetaA.at(i).at(k);
//                G_X0.at(m_N + i) -= m_C.at(k)
//                        * H_M_func(m_roots_s.at(i), m_nodes_s.at(k))
//                        * m_ThetaM.at(i).at(k);
//            }

            for (unsigned int j = 0; j < m_N; j++) {
                J_G_X0.at(i, j) = dSigmaA_r.at(i).at(j);
                J_G_X0.at(m_N + i, m_N + j) = dSigmaM_r.at(i).at(j);
                for (unsigned int k = 0; k < m_Nx; k++) {
                    J_G_X0.at(i, j) -= m_C.at(k)
                            * H_A_deriv_a(A_r.at(i), A_n.at(k),
                                    dA_r.at(i).at(j), dA_n.at(k).at(j),
                                    B_r.at(i), B_n.at(k), sigmaV_r.at(i),
                                    sigmaV_n.at(k), sigmaS_r.at(i),
                                    sigmaS_n.at(k), dsigmaV_a_r.at(i).at(j),
                                    dsigmaV_b_r.at(i).at(j),
                                    dsigmaV_a_n.at(k).at(j),
                                    dsigmaV_b_n.at(k).at(j),
                                    dsigmaS_a_r.at(i).at(j),
                                    dsigmaS_b_r.at(i).at(j),
                                    dsigmaS_a_n.at(k).at(j),
                                    dsigmaS_b_n.at(k).at(j))
                            * m_ThetaA.at(i).at(k);
                    J_G_X0.at(m_N + i, j) -= m_C.at(k)
                            * H_M_deriv_a(A_r.at(i), A_n.at(k),
                                    dA_r.at(i).at(j), dA_n.at(k).at(j),
                                    B_r.at(i), B_n.at(k), sigmaV_r.at(i),
                                    sigmaV_n.at(k), sigmaS_r.at(i),
                                    sigmaS_n.at(k), dsigmaV_a_r.at(i).at(j),
                                    dsigmaV_b_r.at(i).at(j),
                                    dsigmaV_a_n.at(k).at(j),
                                    dsigmaV_b_n.at(k).at(j),
                                    dsigmaS_a_r.at(i).at(j),
                                    dsigmaS_b_r.at(i).at(j),
                                    dsigmaS_a_n.at(k).at(j),
                                    dsigmaS_b_n.at(k).at(j))
                            * m_ThetaM.at(i).at(k);
                    J_G_X0.at(i, m_N + j) -= m_C.at(k)
                            * H_A_deriv_b(A_r.at(i), A_n.at(k), B_r.at(i),
                                    B_n.at(k), dB_r.at(i).at(j),
                                    dB_n.at(k).at(j), sigmaV_r.at(i),
                                    sigmaV_n.at(k), sigmaS_r.at(i),
                                    sigmaS_n.at(k), dsigmaV_a_r.at(i).at(j),
                                    dsigmaV_b_r.at(i).at(j),
                                    dsigmaV_a_n.at(k).at(j),
                                    dsigmaV_b_n.at(k).at(j),
                                    dsigmaS_a_r.at(i).at(j),
                                    dsigmaS_b_r.at(i).at(j),
                                    dsigmaS_a_n.at(k).at(j),
                                    dsigmaS_b_n.at(k).at(j))
                            * m_ThetaA.at(i).at(k);
                    J_G_X0.at(m_N + i, m_N + j) -= m_C.at(k)
                            * H_M_deriv_b(A_r.at(i), A_n.at(k), B_r.at(i),
                                    B_n.at(k), dB_r.at(i).at(j),
                                    dB_n.at(k).at(j), sigmaV_r.at(i),
                                    sigmaV_n.at(k), sigmaS_r.at(i),
                                    sigmaS_n.at(k), dsigmaV_a_r.at(i).at(j),
                                    dsigmaV_b_r.at(i).at(j),
                                    dsigmaV_a_n.at(k).at(j),
                                    dsigmaV_b_n.at(k).at(j),
                                    dsigmaS_a_r.at(i).at(j),
                                    dsigmaS_b_r.at(i).at(j),
                                    dsigmaS_a_n.at(k).at(j),
                                    dsigmaS_b_n.at(k).at(j))
                            * m_ThetaM.at(i).at(k);
                }

//                J_G_X0.at(i, j) = getQuarkPropagator()->differentiateSigmaA(
//                        m_roots_s.at(i), j);
//                J_G_X0.at(m_N + i, m_N + j) =
//                        getQuarkPropagator()->differentiateSigmaM(
//                                m_roots_s.at(i), j);
//                for (unsigned int k = 0; k < m_Nx; k++) {
//                    J_G_X0.at(i, j) -= m_C.at(k)
//                            * H_A_deriv_a(m_roots_s.at(i), m_nodes_s.at(k), j)
//                            * m_ThetaA.at(i).at(k);
//                    J_G_X0.at(m_N + i, j) -= m_C.at(k)
//                            * H_M_deriv_a(m_roots_s.at(i), m_nodes_s.at(k), j)
//                            * m_ThetaM.at(i).at(k);
//                    J_G_X0.at(i, m_N + j) -= m_C.at(k)
//                            * H_A_deriv_b(m_roots_s.at(i), m_nodes_s.at(k), j)
//                            * m_ThetaA.at(i).at(k);
//                    J_G_X0.at(m_N + i, m_N + j) -= m_C.at(k)
//                            * H_M_deriv_b(m_roots_s.at(i), m_nodes_s.at(k), j)
//                            * m_ThetaM.at(i).at(k);
//                }
            }
        }
        X0 = NumA::VectorD::concatenate(m_quarkPropagator->getCoeffsA(),
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
        Formatter formatter1, formatter2, formatter3;
        formatter1 << "Iteration " << n << ".";
        formatter2 << "A : ";
        for (unsigned int i = 0; i < m_N; i++) {
            formatter2 << m_quarkPropagator->evaluateA(m_roots_s.at(i)) << " ";
        }
        formatter3 << "B : ";
        for (unsigned int i = 0; i < m_N; i++) {
            formatter3 << m_quarkPropagator->evaluateB(m_roots_s.at(i)) << " ";
        }
        info(__func__, formatter1.str());
        debug(__func__, formatter2.str());
        debug(__func__, formatter3.str());
    }

    m_iters = n;
}

void GapEquationSolverModule::computeIteration() {
    std::vector<double> SigmaA, SigmaM;
    NumA::VectorD a, b;
    double stored_T;
    double absDiff_a = 0., absDiff_b = 0., relDiff_a = 0., relDiff_b = 0.; // Difference between two iterations
    bool noConvergence = true; // Convergence test
    std::vector<double> qpFunctionsAtRoots;
    std::vector<double> qpFunctionsAtNodes;
    std::vector<QuarkPropagator::QPFunction> qpFunctions;
    std::vector<double> A_r, B_r, sigmaV_r, sigmaS_r;
    std::vector<double> A_n, B_n, sigmaV_n, sigmaS_n;
    double denom_r, denom_n;

    unsigned int n;
    for (n = m_iters; n < m_maxIter && noConvergence; n++) {
        a = m_quarkPropagator->getCoeffsA();
        b = m_quarkPropagator->getCoeffsB();
        SigmaA.assign(m_N, 0.);
        SigmaM.assign(m_N, 0.);

        A_r.assign(m_N, m_Ainit);
        B_r.assign(m_N, m_Binit);
        sigmaV_r.assign(m_N, 0.);
        sigmaS_r.assign(m_N, 0.);

        A_n.assign(m_Nx, m_Ainit);
        B_n.assign(m_Nx, m_Binit);
        sigmaV_n.assign(m_Nx, 0.);
        sigmaS_n.assign(m_Nx, 0.);

        for (unsigned int i = 0; i < m_N; i++) {
            if (n > 0) {
                qpFunctions.assign(2, QuarkPropagator::A);
                qpFunctions.at(1) = QuarkPropagator::B;
                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
                        m_roots_s.at(i));
                A_r.at(i) = qpFunctionsAtRoots.at(0);
                B_r.at(i) = qpFunctionsAtRoots.at(1);
            }
            denom_r = m_roots_s.at(i) * A_r.at(i) * A_r.at(i)
                    + B_r.at(i) * B_r.at(i);
            sigmaV_r.at(i) = A_r.at(i) / denom_r;
            sigmaS_r.at(i) = B_r.at(i) / denom_r;
        }

        for (unsigned int k = 0; k < m_Nx; k++) {
            if (n > 0) {
                qpFunctions.assign(2, QuarkPropagator::A);
                qpFunctions.at(1) = QuarkPropagator::B;
                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
                        m_nodes_s.at(k));
                A_n.at(k) = qpFunctionsAtRoots.at(0);
                B_n.at(k) = qpFunctionsAtRoots.at(1);
            }
            denom_n = m_nodes_s.at(k) * A_n.at(k) * A_n.at(k)
                    + B_n.at(k) * B_n.at(k);
            sigmaV_n.at(k) = A_n.at(k) / denom_n;
            sigmaS_n.at(k) = B_n.at(k) / denom_n;
        }

        for (unsigned int i = 0; i < m_N; i++) {
            for (unsigned int k = 0; k < m_Nx; k++) {
                SigmaA.at(i) += m_C.at(k)
                        * H_A_func(A_r.at(i), A_n.at(k), B_r.at(i), B_n.at(k),
                                sigmaV_r.at(i), sigmaV_n.at(k), sigmaS_r.at(i),
                                sigmaS_n.at(k)) * m_ThetaA.at(i).at(k);
                SigmaM.at(i) += m_C.at(k)
                        * H_M_func(A_r.at(i), A_n.at(k), B_r.at(i), B_n.at(k),
                                sigmaV_r.at(i), sigmaV_n.at(k), sigmaS_r.at(i),
                                sigmaS_n.at(k)) * m_ThetaM.at(i).at(k);
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
        Formatter formatter1, formatter2, formatter3;
        formatter1 << "Iteration " << n << ".";
        formatter2 << "A : ";
        for (unsigned int i = 0; i < m_N; i++) {
            formatter2 << m_quarkPropagator->evaluateA(m_roots_s.at(i)) << " ";
        }
        formatter3 << "B : ";
        for (unsigned int i = 0; i < m_N; i++) {
            formatter3 << m_quarkPropagator->evaluateB(m_roots_s.at(i)) << " ";
        }
        info(__func__, formatter1.str());
        debug(__func__, formatter2.str());
        debug(__func__, formatter3.str());
    }

    m_iters = n;
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

    switch (iterativeType) {
    case Newton:
        computeNewtonInteration();
        break;
    default:
        computeIteration();
        break;
    }

    // Write the result
    info(__func__, "p2 [GeV^2] ; A ; B [GeV] ; M [GeV]");
    double p2, A, B, M;
    for (unsigned int i = 0; i < m_N; i++) {
    	Formatter formatter;
    	p2 = m_roots_s.at(i);
    	A = m_quarkPropagator->evaluateA(p2);
    	B = m_quarkPropagator->evaluateB(p2);
    	M = B/A;
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
        m_changeGP = true;
        m_gluonPropagator = gluonPropagator->clone();
    } else {
        warn(__func__,
                "Trying to set the GluonPropagator pointer with the same pointer, you're playing a dangerous game!");
    }
}

const QuarkPropagator* GapEquationSolverModule::getQuarkPropagator() const {
    return m_quarkPropagator;
}

void GapEquationSolverModule::setQuarkPropagator(
        QuarkPropagator* quarkPropagator) {
    m_changeQP = m_changeQP || (m_quarkPropagator != quarkPropagator)
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

double GapEquationSolverModule::getEpsilon2() const {
    return m_epsilon2;
}

void GapEquationSolverModule::setEpsilon2(double epsilon2) {
    if (epsilon2 <= 0) {
        error(__func__, "The IR cut-off must be positive (and small enough)!");
    }
    m_changeQP = m_changeQP || (epsilon2 != m_epsilon2);
    m_epsilon2 = epsilon2;
}

double GapEquationSolverModule::getLambda2() const {
    return m_Lambda2;
}

void GapEquationSolverModule::setLambda2(double lambda2) {
    if (lambda2 <= 0) {
        error(__func__, "The UV cut-off must be positive (and large enough)!");
    }
    m_changeQP = m_changeQP || (lambda2 != m_Lambda2);
    m_Lambda2 = lambda2;
}

double GapEquationSolverModule::getM() const {
    return m_m;
}

void GapEquationSolverModule::setM(double m) {
    if (m < 0) {
        error(__func__, "The quark mass must be positive!");
    }
    m_changeQP = m_changeQP || (m != m_m);
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
    if (mu <= 0) {
        error(__func__,
                "The renormalization point must be positive (and large enough)!");
    }
    m_changeQP = m_changeQP || (mu != m_mu);
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
    m_changeQP = m_changeQP || (n != m_N);
    m_N = n;
}

int GapEquationSolverModule::getNx() const {
    return m_Nx;
}

void GapEquationSolverModule::setNx(int nx) {
    if (nx <= 0) {
        error(__func__, "The number of nodes of integration must be positive!");
    }
    m_changeNx = m_changeNx || (nx != m_Nx);
    m_Nx = nx;
}

int GapEquationSolverModule::getNz() const {
    return m_Nz;
}

void GapEquationSolverModule::setNz(int nz) {
    if (nz <= 0) {
        error(__func__, "The number of nodes of integration must be positive!");
    }
    m_changeNz = m_changeNz || (nz != m_Nz);
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
    m_changeInit = m_changeInit || (ainit != m_Ainit);
    m_Ainit = ainit;
}

double GapEquationSolverModule::getBinit() const {
    return m_Binit;
}

void GapEquationSolverModule::setBinit(double binit) {
    m_changeInit = m_changeInit || (binit != m_Binit);
    m_Binit = binit;
}

void GapEquationSolverModule::reset() {
    m_iters = 0;
    m_quarkPropagator->setCoeffsA(0.);
    m_quarkPropagator->setCoeffsB(0.);
}

double GapEquationSolverModule::k2_func(double p2, double q2, double z) const {
    return p2 + q2 - 2 * sqrt(p2) * sqrt(q2) * z;
}
