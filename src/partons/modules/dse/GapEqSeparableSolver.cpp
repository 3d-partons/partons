#include "../../../../include/partons/modules/dse/GapEqSeparableSolver.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/linear_algebra/linear_system/LinearSystem.h>
#include <NumA/linear_algebra/matrix/MatrixD.h>
#include <NumA/root_finding/NewtonMD.h>
#include <cmath>

#include "../../../../include/partons/beans/dse/GluonPropagator.h"
#include "../../../../include/partons/beans/dse/QuarkGluonVertex.h"
#include "../../../../include/partons/beans/dse/QuarkPropagator.h"
#include "../../../../include/partons/BaseObjectRegistry.h"

// Initialise [class]::classId with a unique name.
const unsigned int GapEqSeparableSolver::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GapEqSeparableSolver("GapEqSeparableSolver"));

GapEqSeparableSolver::GapEqSeparableSolver(const std::string &className) :
        GapEquationSolverModule(className), m_Amu(0.), m_Bmu(0.) {
}

GapEqSeparableSolver::~GapEqSeparableSolver() {
    // TODO Auto-generated destructor stub
}

GapEqSeparableSolver::GapEqSeparableSolver(const GapEqSeparableSolver& other) :
        GapEquationSolverModule(other) {
    m_Amu = other.m_Amu;
    m_Bmu = other.m_Bmu;
}

GapEqSeparableSolver* GapEqSeparableSolver::clone() const {
    return new GapEqSeparableSolver(*this);
}

void GapEqSeparableSolver::initModule() {
    GapEquationSolverModule::initModule();

    // Separable Angular Integrals
    if (isChangedQp() || isChangedVertex() || isChangedGp() || isChangedNx()
            || isChangedNz()) {
        double k2 = 0., G_k2 = 0., Cz = 0.;
        std::vector<double> angular_integrands;
        m_ThetaA.assign(getVertex()->numberOfBasisElementsUsed(),
                std::vector<std::vector<double> >(getN() + 1,
                        std::vector<double>(getNx(), 0.)));
        m_ThetaM.assign(getVertex()->numberOfBasisElementsUsed(),
                std::vector<std::vector<double> >(getN() + 1,
                        std::vector<double>(getNx(), 0.)));
        for (unsigned int l = 0; l < getNz(); l++) {
            Cz = getWeightsZ().at(l)
                    * sqrt(1 - getNodesZ().at(l) * getNodesZ().at(l));
            for (unsigned int i = 0; i < getN() + 1; i++) {
                for (unsigned int k = 0; k < getNx(); k++) {
                    k2 = k2_func(getRootsS().at(i), getNodesS().at(k),
                            getNodesZ().at(l));
                    G_k2 = getGluonPropagator()->evaluateG(k2);
                    angular_integrands = getVertex()->Angular_Integrands(
                            getRootsS().at(i), getNodesS().at(k), k2);
                    for (unsigned int j = 0;
                            j < getVertex()->numberOfBasisElementsUsed(); j++) {
                        m_ThetaA.at(j).at(i).at(k) += Cz * G_k2
                                * angular_integrands.at(j);
                        m_ThetaM.at(j).at(i).at(k) +=
                                Cz * G_k2
                                        * angular_integrands.at(
                                                j
                                                        + getVertex()->numberOfBasisElementsUsed());
                    }
                }
            }
        }
    }

    // Variables
    if (isChangedQp()) {
        m_Ap.assign(getN() + 1);
        m_Bp.assign(getN() + 1);
        m_sigmaVp.assign(getN() + 1);
        m_sigmaSp.assign(getN() + 1);
        m_SigmaAp.assign(getN());
        m_SigmaMp.assign(getN());
        m_X.assign(get2N());
        m_X0.assign(get2N());
        m_DeltaX.assign(get2N());
        m_G_X0.assign(get2N());
        m_G_X.assign(get2N());
        m_DeltaG.assign(get2N());
    }
    if (isChangedNx()) {
        m_Aq.assign(getNx());
        m_Bq.assign(getNx());
        m_sigmaVq.assign(getNx());
        m_sigmaSq.assign(getNx());
    }

    // Set the booleans to false to not recompute things unnecessarily
    setChangedQp(false);
    setChangedGp(false);
    setChangedVertex(false);
    setChangedNx(false);
    setChangedNz(false);
    setChangedInit(false);
}

void GapEqSeparableSolver::isModuleWellConfigured() {
    GapEquationSolverModule::isModuleWellConfigured();
}

//void GapEqSeparableSolver::computeNewtonIteration() {
//    unsigned int N_newton = getN() * 2;
//    NumA::NewtonMD newtonIteration(N_newton);
//    NumA::VectorD a(getN()), b(getN()), X(N_newton), X0(N_newton), G_X0(
//            N_newton);
//    NumA::MatrixD J_G_X0(N_newton, N_newton);
//    double absDiff_a = 0., absDiff_b = 0., relDiff_a = 0., relDiff_b = 0.; // Difference between two iterations
//    bool noConvergence = true; // Convergence test
//    std::vector<double> qpFunctionsAtRoots;
//    std::vector<double> qpFunctionsAtNodes;
//    std::vector<QuarkPropagator::QPFunction> qpFunctions;
//    std::vector<double> SigmaA_r, SigmaM_r, A_r, B_r, sigmaV_r, sigmaS_r;
//    std::vector<double> A_n, B_n, sigmaV_n, sigmaS_n;
//    std::vector<std::vector<double> > dSigmaA_r, dSigmaM_r, dA_r, dB_r,
//            dsigmaV_a_r, dsigmaV_b_r, dsigmaS_a_r, dsigmaS_b_r;
//    std::vector<std::vector<double> > dA_n, dB_n, dsigmaV_a_n, dsigmaV_b_n,
//            dsigmaS_a_n, dsigmaS_b_n;
//    double denom_r, denom_n, sigmaV2_r, sigmaS2_r, sigmaV2_n, sigmaS2_n;
//    std::vector<double> radial_integrands, radial_integrands_deriv_a,
//            radial_integrands_deriv_b;
//
//    unsigned int n;
//    for (n = getIters(); n < getMaxIter() && noConvergence; n++) {
//
//        G_X0.assign(N_newton, 0.);
//        J_G_X0.assign(N_newton, N_newton, 0.);
//
//        SigmaA_r.assign(getN(), 0.);
//        SigmaM_r.assign(getN(), 0.);
//        A_r.assign(getN(), getAinit());
//        B_r.assign(getN(), getBinit());
//        sigmaV_r.assign(getN(), 0.);
//        sigmaS_r.assign(getN(), 0.);
//
//        dSigmaA_r.assign(getN(), std::vector<double>(getN(), 0.));
//        dSigmaM_r.assign(getN(), std::vector<double>(getN(), 0.));
//        dA_r.assign(getN(), std::vector<double>(getN(), 0.));
//        dB_r.assign(getN(), std::vector<double>(getN(), 0.));
//        dsigmaV_a_r.assign(getN(), std::vector<double>(getN(), 0.));
//        dsigmaV_b_r.assign(getN(), std::vector<double>(getN(), 0.));
//        dsigmaS_a_r.assign(getN(), std::vector<double>(getN(), 0.));
//        dsigmaS_b_r.assign(getN(), std::vector<double>(getN(), 0.));
//
//        A_n.assign(getNx(), getAinit());
//        B_n.assign(getNx(), getBinit());
//        sigmaV_n.assign(getNx(), 0.);
//        sigmaS_n.assign(getNx(), 0.);
//
//        dA_n.assign(getNx(), std::vector<double>(getN(), 0.));
//        dB_n.assign(getNx(), std::vector<double>(getN(), 0.));
//        dsigmaV_a_n.assign(getNx(), std::vector<double>(getN(), 0.));
//        dsigmaV_b_n.assign(getNx(), std::vector<double>(getN(), 0.));
//        dsigmaS_a_n.assign(getNx(), std::vector<double>(getN(), 0.));
//        dsigmaS_b_n.assign(getNx(), std::vector<double>(getN(), 0.));
//
//        for (unsigned int i = 0; i < getN(); i++) {
//            if (n > 0) {
//                qpFunctions.assign(4, QuarkPropagator::SigmaA);
//                qpFunctions.at(1) = QuarkPropagator::SigmaM;
//                qpFunctions.at(2) = QuarkPropagator::A;
//                qpFunctions.at(3) = QuarkPropagator::B;
//                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
//                        getRootsS().at(i));
//                SigmaA_r.at(i) = qpFunctionsAtRoots.at(0);
//                SigmaM_r.at(i) = qpFunctionsAtRoots.at(1);
//                A_r.at(i) = qpFunctionsAtRoots.at(2);
//                B_r.at(i) = qpFunctionsAtRoots.at(3);
//            }
//            denom_r = getRootsS().at(i) * A_r.at(i) * A_r.at(i)
//                    + B_r.at(i) * B_r.at(i);
//            sigmaV_r.at(i) = A_r.at(i) / denom_r;
//            sigmaS_r.at(i) = B_r.at(i) / denom_r;
//
//            sigmaV2_r = sigmaV_r.at(i) * sigmaV_r.at(i);
//            sigmaS2_r = sigmaS_r.at(i) * sigmaS_r.at(i);
//
//            for (unsigned int j = 0; j < getN(); j++) {
//                qpFunctions.assign(4, QuarkPropagator::dSigmaA);
//                qpFunctions.at(1) = QuarkPropagator::dSigmaM;
//                qpFunctions.at(2) = QuarkPropagator::dA;
//                qpFunctions.at(3) = QuarkPropagator::dB;
//                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
//                        getRootsS().at(i), j);
//                dSigmaA_r.at(i).at(j) = qpFunctionsAtRoots.at(0);
//                dSigmaM_r.at(i).at(j) = qpFunctionsAtRoots.at(1);
//                dA_r.at(i).at(j) = qpFunctionsAtRoots.at(2);
//                dB_r.at(i).at(j) = qpFunctionsAtRoots.at(3);
//                dsigmaV_a_r.at(i).at(j) = dA_r.at(i).at(j)
//                        * (sigmaS2_r - getRootsS().at(i) * sigmaV2_r);
//                dsigmaV_b_r.at(i).at(j) = -2 * dB_r.at(i).at(j) * sigmaV_r.at(i)
//                        * sigmaS_r.at(i);
//                dsigmaS_a_r.at(i).at(j) = -2 * getRootsS().at(i)
//                        * dA_r.at(i).at(j) * sigmaV_r.at(i) * sigmaS_r.at(i);
//                dsigmaS_b_r.at(i).at(j) = dB_r.at(i).at(j)
//                        * (getRootsS().at(i) * sigmaV2_r - sigmaS2_r);
//            }
//
//        }
//
//        for (unsigned int k = 0; k < getNx(); k++) {
//            if (n > 0) {
//                qpFunctions.assign(2, QuarkPropagator::A);
//                qpFunctions.at(1) = QuarkPropagator::B;
//                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
//                        getNodesS().at(k));
//                A_n.at(k) = qpFunctionsAtRoots.at(0);
//                B_n.at(k) = qpFunctionsAtRoots.at(1);
//            }
//            denom_n = getNodesS().at(k) * A_n.at(k) * A_n.at(k)
//                    + B_n.at(k) * B_n.at(k);
//            sigmaV_n.at(k) = A_n.at(k) / denom_n;
//            sigmaS_n.at(k) = B_n.at(k) / denom_n;
//
//            sigmaV2_n = sigmaV_n.at(k) * sigmaV_n.at(k);
//            sigmaS2_n = sigmaS_n.at(k) * sigmaS_n.at(k);
//
//            for (unsigned int j = 0; j < getN(); j++) {
//                qpFunctions.assign(2, QuarkPropagator::dA);
//                qpFunctions.at(1) = QuarkPropagator::dB;
//                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
//                        getNodesS().at(k), j);
//                dA_n.at(k).at(j) = qpFunctionsAtRoots.at(0);
//                dB_n.at(k).at(j) = qpFunctionsAtRoots.at(1);
//                dsigmaV_a_n.at(k).at(j) = dA_n.at(k).at(j)
//                        * (sigmaS2_n - getNodesS().at(k) * sigmaV2_n);
//                dsigmaV_b_n.at(k).at(j) = -2 * dB_n.at(k).at(j) * sigmaV_n.at(k)
//                        * sigmaS_n.at(k);
//                dsigmaS_a_n.at(k).at(j) = -2 * getNodesS().at(k)
//                        * dA_n.at(k).at(j) * sigmaV_n.at(k) * sigmaS_n.at(k);
//                dsigmaS_b_n.at(k).at(j) = dB_n.at(k).at(j)
//                        * (getNodesS().at(k) * sigmaV2_n - sigmaS2_n);
//            }
//
//        }
//
//        for (unsigned int i = 0; i < getN(); i++) {
//
//            G_X0.at(i) = SigmaA_r.at(i);
//            G_X0.at(getN() + i) = SigmaM_r.at(i);
//            for (unsigned int k = 0; k < getNx(); k++) {
//                radial_integrands = getVertex()->Radial_Integrands(
//                        getRootsS().at(i), getNodesS().at(k), A_r.at(i),
//                        A_n.at(k), B_r.at(i), B_n.at(k), sigmaV_r.at(i),
//                        sigmaV_n.at(k), sigmaS_r.at(i), sigmaS_n.at(k));
//                for (unsigned int l = 0;
//                        l < getVertex()->numberOfBasisElementsUsed(); l++) {
//                    G_X0.at(i) -= getC().at(k) * radial_integrands.at(l)
//                            * m_ThetaA.at(l).at(i).at(k);
//                    G_X0.at(getN() + i) -=
//                            getC().at(k)
//                                    * radial_integrands.at(
//                                            l
//                                                    + getVertex()->numberOfBasisElementsUsed())
//                                    * m_ThetaM.at(l).at(i).at(k);
//                }
//            }
//
//            for (unsigned int j = 0; j < getN(); j++) {
//                J_G_X0.at(i, j) = dSigmaA_r.at(i).at(j);
//                J_G_X0.at(getN() + i, getN() + j) = dSigmaM_r.at(i).at(j);
//                for (unsigned int k = 0; k < getNx(); k++) {
//                    radial_integrands_deriv_a =
//                            getVertex()->Radial_Integrands_deriv(
//                                    getRootsS().at(i), getNodesS().at(k),
//                                    A_r.at(i), A_n.at(k), dA_r.at(i).at(j),
//                                    dA_n.at(k).at(j), B_r.at(i), B_n.at(k), 0.,
//                                    0., sigmaV_r.at(i), sigmaV_n.at(k),
//                                    sigmaS_r.at(i), sigmaS_n.at(k),
//                                    dsigmaV_a_r.at(i).at(j),
//                                    dsigmaV_a_n.at(k).at(j),
//                                    dsigmaS_a_r.at(i).at(j),
//                                    dsigmaS_a_n.at(k).at(j));
//                    radial_integrands_deriv_b =
//                            getVertex()->Radial_Integrands_deriv(
//                                    getRootsS().at(i), getNodesS().at(k),
//                                    A_r.at(i), A_n.at(k), 0., 0., B_r.at(i),
//                                    B_n.at(k), dB_r.at(i).at(j),
//                                    dB_n.at(k).at(j), sigmaV_r.at(i),
//                                    sigmaV_n.at(k), sigmaS_r.at(i),
//                                    sigmaS_n.at(k), dsigmaV_b_r.at(i).at(j),
//                                    dsigmaV_b_n.at(k).at(j),
//                                    dsigmaS_b_r.at(i).at(j),
//                                    dsigmaS_b_n.at(k).at(j));
//                    for (unsigned int l = 0;
//                            l < getVertex()->numberOfBasisElementsUsed(); l++) {
//                        J_G_X0.at(i, j) -= getC().at(k)
//                                * radial_integrands_deriv_a.at(l)
//                                * m_ThetaA.at(l).at(i).at(k);
//                        J_G_X0.at(getN() + i, j) -=
//                                getC().at(k)
//                                        * radial_integrands_deriv_a.at(
//                                                l
//                                                        + getVertex()->numberOfBasisElementsUsed())
//                                        * m_ThetaM.at(l).at(i).at(k);
//                        J_G_X0.at(i, getN() + j) -= getC().at(k)
//                                * radial_integrands_deriv_b.at(l)
//                                * m_ThetaA.at(l).at(i).at(k);
//                        J_G_X0.at(getN() + i, getN() + j) -=
//                                getC().at(k)
//                                        * radial_integrands_deriv_b.at(
//                                                l
//                                                        + getVertex()->numberOfBasisElementsUsed())
//                                        * m_ThetaM.at(l).at(i).at(k);
//                    }
//                }
//            }
//        }
//        X0 = NumA::VectorD::concatenate(getQuarkPropagator()->getCoeffsA(),
//                getQuarkPropagator()->getCoeffsB());
//        X = newtonIteration.iterate(X0, G_X0, J_G_X0);
//        a = X.sub(0, getN());
//        b = X.sub(getN(), N_newton);
//
//        absDiff_a = (a - getQuarkPropagator()->getCoeffsA()).norm();
//        absDiff_b = (b - getQuarkPropagator()->getCoeffsB()).norm();
//        relDiff_a = absDiff_a
//                / (getQuarkPropagator()->getCoeffsA().norm() + 1.e-16);
//        relDiff_b = absDiff_b
//                / (getQuarkPropagator()->getCoeffsB().norm() + 1.e-16);
//
//        noConvergence = (absDiff_a > getAbsTolerance())
//                || (absDiff_b > getAbsTolerance())
//                || (relDiff_a > getRelTolerance())
//                || (relDiff_b > getRelTolerance());
//        getQuarkPropagator()->setCoeffsA(a);
//        getQuarkPropagator()->setCoeffsB(b);
//
//        // Test
//        ElemUtils::Formatter formatter1, formatter2, formatter3;
//        formatter1 << "Iteration " << n << ".";
//        formatter2 << "A : ";
//        for (unsigned int i = 0; i < getN(); i++) {
//            formatter2 << getQuarkPropagator()->evaluateA(getRootsS().at(i))
//                    << " ";
//        }
//        formatter3 << "B : ";
//        for (unsigned int i = 0; i < getN(); i++) {
//            formatter3 << getQuarkPropagator()->evaluateB(getRootsS().at(i))
//                    << " ";
//        }
//        info(__func__, formatter1.str());
//        debug(__func__, formatter2.str());
//        debug(__func__, formatter3.str());
//    }
//
//    setIters(n);
//}

void GapEqSeparableSolver::computeBroydenIteration() {
    NumA::NewtonMD newtonIteration(get2N(), NumA::LinearSystem::FullQR);

    unsigned int n;
    bool noConvergence = true;
    if (getIters() == 0) {
        m_X = NumA::VectorD::concatenate(NumA::VectorD(getN(), getAinit()),
                NumA::VectorD(getN(), getBinit()));
    }
    m_G_X = G_func(m_X);
    m_DeltaG = m_G_X - m_G_X0;
    if (getIters() == 0) {
        m_J_G_X = NumA::MatrixD::Id(get2N());
    } else {
        m_J_G_X = m_J_G_X0
                + (m_DeltaG - m_J_G_X0 * m_DeltaX).toMatrix()
                        * m_DeltaX.toMatrix().transpose()
                        / (m_DeltaX * m_DeltaX); // Direct Broyden
//        m_J_G_X = m_J_G_X0
//                + (m_DeltaX - m_J_G_X0 * m_DeltaG).toMatrix()
//                        * m_DeltaG.toMatrix().transpose()
//                        / (m_DeltaG * m_DeltaG); // Bad Broyden
    }
    for (n = getIters(); n < getMaxIter() && noConvergence; n++) {
        m_X0 = m_X;
        m_G_X0 = m_G_X;
        m_J_G_X0 = m_J_G_X;
        m_X = newtonIteration.iterate(m_X0, m_G_X0, m_J_G_X0); // Direct Broyden
//        m_X = m_X0 - m_J_G_X0 * m_G_X0; // Bad/Good Broyden
        m_DeltaX = m_X - m_X0;
        m_G_X = G_func(m_X);
        m_DeltaG = m_G_X - m_G_X0;
        m_J_G_X = m_J_G_X0
                + (m_DeltaG - m_J_G_X0 * m_DeltaX).toMatrix()
                        * m_DeltaX.toMatrix().transpose()
                        / (m_DeltaX * m_DeltaX); // Direct Broyden
//        m_J_G_X = m_J_G_X0
//                + (m_DeltaX - m_J_G_X0 * m_DeltaG).toMatrix()
//                        * m_DeltaG.toMatrix().transpose()
//                        / (m_DeltaG * m_DeltaG); // Bad Broyden

        setAbsDifference(m_DeltaX.norm());
        setRelDifference(getAbsDifference() / m_X0.norm());

//        noConvergence = (getAbsDifference() > getAbsTolerance())
//                || (getRelDifference() > getRelTolerance());
        noConvergence = m_G_X.norm() > 1.e-5;

        // Test
        ElemUtils::Formatter formatter1, formatter2, formatter3, formatter4,
                formatter5;
        formatter1 << "Iteration " << n << ".";
        formatter2 << "A : ";
        for (unsigned int i = 0; i < getN(); i++) {
            formatter2 << m_X.at(i) << " ";
        }
        formatter3 << "B : ";
        for (unsigned int i = 0; i < getN(); i++) {
            formatter3 << m_X.at(i + getN()) << " ";
        }
        formatter4 << "M : ";
        for (unsigned int i = 0; i < getN(); i++) {
            formatter4 << m_X.at(i + getN()) / m_X.at(i) << " ";
        }
        formatter5 << "Residue: " << m_G_X.norm();
        info(__func__, formatter1.str());
        debug(__func__, formatter2.str());
        debug(__func__, formatter3.str());
        debug(__func__, formatter4.str());
        debug(__func__, formatter5.str());
    }

    getQuarkPropagator()->setA(m_X.sub(0, getN()));
    getQuarkPropagator()->setB(m_X.sub(getN(), get2N()));

    setConverged(!noConvergence);
    setIters(n);
}

//void GapEqSeparableSolver::computeBroydenIteration() {
//    int N_newton = getN() * 2;
//    NumA::NewtonMD newtonIteration(N_newton);
//    NumA::VectorD a(getN()), b(getN()), X(N_newton), X0(N_newton), G_X0(
//            N_newton), G_X(N_newton), DeltaX(N_newton), DeltaG(N_newton);
//    NumA::MatrixD J_G_X0(N_newton, N_newton), J_G_X(N_newton, N_newton);
//    double absDiff_a = 0., absDiff_b = 0., relDiff_a = 0., relDiff_b = 0.; // Difference between two iterations
//    bool noConvergence = true; // Convergence test
//    std::vector<double> qpFunctionsAtRoots;
//    std::vector<double> qpFunctionsAtNodes;
//    std::vector<QuarkPropagator::QPFunction> qpFunctions;
//    std::vector<double> SigmaA_r, SigmaM_r, A_r, B_r, sigmaV_r, sigmaS_r;
//    std::vector<double> A_n, B_n, sigmaV_n, sigmaS_n;
//    std::vector<std::vector<double> > dSigmaA_r, dSigmaM_r, dA_r, dB_r,
//            dsigmaV_a_r, dsigmaV_b_r, dsigmaS_a_r, dsigmaS_b_r;
//    std::vector<std::vector<double> > dA_n, dB_n, dsigmaV_a_n, dsigmaV_b_n,
//            dsigmaS_a_n, dsigmaS_b_n;
//    double denom_r, denom_n, sigmaV2_r, sigmaS2_r, sigmaV2_n, sigmaS2_n;
//    std::vector<double> radial_integrands, radial_integrands_deriv_a,
//            radial_integrands_deriv_b;
//
//    unsigned int n;
//    for (n = getIters(); n < getMaxIter() && noConvergence; n++) {
//
//        G_X.assign(N_newton, 0.);
//        J_G_X.assign(N_newton, N_newton, 0.);
//
//        SigmaA_r.assign(getN(), 0.);
//        SigmaM_r.assign(getN(), 0.);
//        A_r.assign(getN(), getAinit());
//        B_r.assign(getN(), getBinit());
//        sigmaV_r.assign(getN(), 0.);
//        sigmaS_r.assign(getN(), 0.);
//
//        A_n.assign(getNx(), getAinit());
//        B_n.assign(getNx(), getBinit());
//        sigmaV_n.assign(getNx(), 0.);
//        sigmaS_n.assign(getNx(), 0.);
//
//        if (n == 0) {
//            dSigmaA_r.assign(getN(), std::vector<double>(getN(), 0.));
//            dSigmaM_r.assign(getN(), std::vector<double>(getN(), 0.));
//            dA_r.assign(getN(), std::vector<double>(getN(), 0.));
//            dB_r.assign(getN(), std::vector<double>(getN(), 0.));
//            dsigmaV_a_r.assign(getN(), std::vector<double>(getN(), 0.));
//            dsigmaV_b_r.assign(getN(), std::vector<double>(getN(), 0.));
//            dsigmaS_a_r.assign(getN(), std::vector<double>(getN(), 0.));
//            dsigmaS_b_r.assign(getN(), std::vector<double>(getN(), 0.));
//
//            dA_n.assign(getNx(), std::vector<double>(getN(), 0.));
//            dB_n.assign(getNx(), std::vector<double>(getN(), 0.));
//            dsigmaV_a_n.assign(getNx(), std::vector<double>(getN(), 0.));
//            dsigmaV_b_n.assign(getNx(), std::vector<double>(getN(), 0.));
//            dsigmaS_a_n.assign(getNx(), std::vector<double>(getN(), 0.));
//            dsigmaS_b_n.assign(getNx(), std::vector<double>(getN(), 0.));
//        }
//
//        for (unsigned int i = 0; i < getN(); i++) {
//            if (n > 0) {
//                qpFunctions.assign(4, QuarkPropagator::SigmaA);
//                qpFunctions.at(1) = QuarkPropagator::SigmaM;
//                qpFunctions.at(2) = QuarkPropagator::A;
//                qpFunctions.at(3) = QuarkPropagator::B;
//                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
//                        getRootsS().at(i));
//                SigmaA_r.at(i) = qpFunctionsAtRoots.at(0);
//                SigmaM_r.at(i) = qpFunctionsAtRoots.at(1);
//                A_r.at(i) = qpFunctionsAtRoots.at(2);
//                B_r.at(i) = qpFunctionsAtRoots.at(3);
//            }
//            denom_r = getRootsS().at(i) * A_r.at(i) * A_r.at(i)
//                    + B_r.at(i) * B_r.at(i);
//            sigmaV_r.at(i) = A_r.at(i) / denom_r;
//            sigmaS_r.at(i) = B_r.at(i) / denom_r;
//
//            if (n == 0) {
//                sigmaV2_r = sigmaV_r.at(i) * sigmaV_r.at(i);
//                sigmaS2_r = sigmaS_r.at(i) * sigmaS_r.at(i);
//
//                for (unsigned int j = 0; j < getN(); j++) {
//                    qpFunctions.assign(4, QuarkPropagator::dSigmaA);
//                    qpFunctions.at(1) = QuarkPropagator::dSigmaM;
//                    qpFunctions.at(2) = QuarkPropagator::dA;
//                    qpFunctions.at(3) = QuarkPropagator::dB;
//                    qpFunctionsAtRoots = getQuarkPropagator()->evaluate(
//                            qpFunctions, getRootsS().at(i), j);
//                    dSigmaA_r.at(i).at(j) = qpFunctionsAtRoots.at(0);
//                    dSigmaM_r.at(i).at(j) = qpFunctionsAtRoots.at(1);
//                    dA_r.at(i).at(j) = qpFunctionsAtRoots.at(2);
//                    dB_r.at(i).at(j) = qpFunctionsAtRoots.at(3);
//                    dsigmaV_a_r.at(i).at(j) = dA_r.at(i).at(j)
//                            * (sigmaS2_r - getRootsS().at(i) * sigmaV2_r);
//                    dsigmaV_b_r.at(i).at(j) = -2 * dB_r.at(i).at(j)
//                            * sigmaV_r.at(i) * sigmaS_r.at(i);
//                    dsigmaS_a_r.at(i).at(j) = -2 * getRootsS().at(i)
//                            * dA_r.at(i).at(j) * sigmaV_r.at(i)
//                            * sigmaS_r.at(i);
//                    dsigmaS_b_r.at(i).at(j) = dB_r.at(i).at(j)
//                            * (getRootsS().at(i) * sigmaV2_r - sigmaS2_r);
//                }
//            }
//
//        }
//
//        for (unsigned int k = 0; k < getNx(); k++) {
//            if (n > 0) {
//                qpFunctions.assign(2, QuarkPropagator::A);
//                qpFunctions.at(1) = QuarkPropagator::B;
//                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
//                        getNodesS().at(k));
//                A_n.at(k) = qpFunctionsAtRoots.at(0);
//                B_n.at(k) = qpFunctionsAtRoots.at(1);
//            }
//            denom_n = getNodesS().at(k) * A_n.at(k) * A_n.at(k)
//                    + B_n.at(k) * B_n.at(k);
//            sigmaV_n.at(k) = A_n.at(k) / denom_n;
//            sigmaS_n.at(k) = B_n.at(k) / denom_n;
//
//            if (n == 0) {
//                sigmaV2_n = sigmaV_n.at(k) * sigmaV_n.at(k);
//                sigmaS2_n = sigmaS_n.at(k) * sigmaS_n.at(k);
//
//                for (unsigned int j = 0; j < getN(); j++) {
//                    qpFunctions.assign(2, QuarkPropagator::dA);
//                    qpFunctions.at(1) = QuarkPropagator::dB;
//                    qpFunctionsAtRoots = getQuarkPropagator()->evaluate(
//                            qpFunctions, getNodesS().at(k), j);
//                    dA_n.at(k).at(j) = qpFunctionsAtRoots.at(0);
//                    dB_n.at(k).at(j) = qpFunctionsAtRoots.at(1);
//                    dsigmaV_a_n.at(k).at(j) = dA_n.at(k).at(j)
//                            * (sigmaS2_n - getNodesS().at(k) * sigmaV2_n);
//                    dsigmaV_b_n.at(k).at(j) = -2 * dB_n.at(k).at(j)
//                            * sigmaV_n.at(k) * sigmaS_n.at(k);
//                    dsigmaS_a_n.at(k).at(j) = -2 * getNodesS().at(k)
//                            * dA_n.at(k).at(j) * sigmaV_n.at(k)
//                            * sigmaS_n.at(k);
//                    dsigmaS_b_n.at(k).at(j) = dB_n.at(k).at(j)
//                            * (getNodesS().at(k) * sigmaV2_n - sigmaS2_n);
//                }
//            }
//
//        }
//
//        for (unsigned int i = 0; i < getN(); i++) {
//            G_X.at(i) = SigmaA_r.at(i);
//            G_X.at(getN() + i) = SigmaM_r.at(i);
//            for (unsigned int k = 0; k < getNx(); k++) {
//                radial_integrands = getVertex()->Radial_Integrands(
//                        getRootsS().at(i), getNodesS().at(k), A_r.at(i),
//                        A_n.at(k), B_r.at(i), B_n.at(k), sigmaV_r.at(i),
//                        sigmaV_n.at(k), sigmaS_r.at(i), sigmaS_n.at(k));
//                for (unsigned int l = 0;
//                        l < getVertex()->numberOfBasisElementsUsed(); l++) {
//                    G_X.at(i) -= getC().at(k) * radial_integrands.at(l)
//                            * m_ThetaA.at(l).at(i).at(k);
//                    G_X.at(getN() + i) -=
//                            getC().at(k)
//                                    * radial_integrands.at(
//                                            l
//                                                    + getVertex()->numberOfBasisElementsUsed())
//                                    * m_ThetaM.at(l).at(i).at(k);
//                }
//            }
//        }
//
//        if (n == 0) {
//            for (unsigned int i = 0; i < getN(); i++) {
//                for (unsigned int j = 0; j < getN(); j++) {
//                    J_G_X.at(i, j) = dSigmaA_r.at(i).at(j);
//                    J_G_X.at(getN() + i, getN() + j) = dSigmaM_r.at(i).at(j);
//                    for (unsigned int k = 0; k < getNx(); k++) {
//                        radial_integrands_deriv_a =
//                                getVertex()->Radial_Integrands_deriv(
//                                        getRootsS().at(i), getNodesS().at(k),
//                                        A_r.at(i), A_n.at(k), dA_r.at(i).at(j),
//                                        dA_n.at(k).at(j), B_r.at(i), B_n.at(k),
//                                        0., 0., sigmaV_r.at(i), sigmaV_n.at(k),
//                                        sigmaS_r.at(i), sigmaS_n.at(k),
//                                        dsigmaV_a_r.at(i).at(j),
//                                        dsigmaV_a_n.at(k).at(j),
//                                        dsigmaS_a_r.at(i).at(j),
//                                        dsigmaS_a_n.at(k).at(j));
//                        radial_integrands_deriv_b =
//                                getVertex()->Radial_Integrands_deriv(
//                                        getRootsS().at(i), getNodesS().at(k),
//                                        A_r.at(i), A_n.at(k), 0., 0., B_r.at(i),
//                                        B_n.at(k), dB_r.at(i).at(j),
//                                        dB_n.at(k).at(j), sigmaV_r.at(i),
//                                        sigmaV_n.at(k), sigmaS_r.at(i),
//                                        sigmaS_n.at(k), dsigmaV_b_r.at(i).at(j),
//                                        dsigmaV_b_n.at(k).at(j),
//                                        dsigmaS_b_r.at(i).at(j),
//                                        dsigmaS_b_n.at(k).at(j));
//                        for (unsigned int l = 0;
//                                l < getVertex()->numberOfBasisElementsUsed();
//                                l++) {
//                            J_G_X.at(i, j) -= getC().at(k)
//                                    * radial_integrands_deriv_a.at(l)
//                                    * m_ThetaA.at(l).at(i).at(k);
//                            J_G_X.at(getN() + i, j) -=
//                                    getC().at(k)
//                                            * radial_integrands_deriv_a.at(
//                                                    l
//                                                            + getVertex()->numberOfBasisElementsUsed())
//                                            * m_ThetaM.at(l).at(i).at(k);
//                            J_G_X.at(i, getN() + j) -= getC().at(k)
//                                    * radial_integrands_deriv_b.at(l)
//                                    * m_ThetaA.at(l).at(i).at(k);
//                            J_G_X.at(getN() + i, getN() + j) -=
//                                    getC().at(k)
//                                            * radial_integrands_deriv_b.at(
//                                                    l
//                                                            + getVertex()->numberOfBasisElementsUsed())
//                                            * m_ThetaM.at(l).at(i).at(k);
//                        }
//                    }
//                }
//            }
//        } else {
//            DeltaG = G_X - G_X0;
//            J_G_X = J_G_X0
//                    + (DeltaG - J_G_X0 * DeltaX).toMatrix()
//                            * DeltaX.toMatrix().transpose() / (DeltaX * DeltaX);
//        }
//        X0 = NumA::VectorD::concatenate(getQuarkPropagator()->getCoeffsA(),
//                getQuarkPropagator()->getCoeffsB());
//        G_X0 = G_X;
//        J_G_X0 = J_G_X;
//        X = newtonIteration.iterate(X0, G_X0, J_G_X0);
//        DeltaX = X - X0;
//        a = X.sub(0, getN());
//        b = X.sub(getN(), N_newton);
//
//        absDiff_a = (a - getQuarkPropagator()->getCoeffsA()).norm();
//        absDiff_b = (b - getQuarkPropagator()->getCoeffsB()).norm();
//        relDiff_a = absDiff_a
//                / (getQuarkPropagator()->getCoeffsA().norm() + 1.e-16);
//        relDiff_b = absDiff_b
//                / (getQuarkPropagator()->getCoeffsB().norm() + 1.e-16);
//
//        noConvergence = (absDiff_a > getAbsTolerance())
//                || (absDiff_b > getAbsTolerance())
//                || (relDiff_a > getRelTolerance())
//                || (relDiff_b > getRelTolerance());
//        getQuarkPropagator()->setCoeffsA(a);
//        getQuarkPropagator()->setCoeffsB(b);
//
//        // Test
//        ElemUtils::Formatter formatter1, formatter2, formatter3;
//        formatter1 << "Iteration " << n << ".";
//        formatter2 << "A : ";
//        for (unsigned int i = 0; i < getN(); i++) {
//            formatter2 << getQuarkPropagator()->evaluateA(getRootsS().at(i))
//                    << " ";
//        }
//        formatter3 << "B : ";
//        for (unsigned int i = 0; i < getN(); i++) {
//            formatter3 << getQuarkPropagator()->evaluateB(getRootsS().at(i))
//                    << " ";
//        }
//        info(__func__, formatter1.str());
//        debug(__func__, formatter2.str());
//        debug(__func__, formatter3.str());
//    }
//
//    setIters(n);
//
//}

//void GapEqSeparableSolver::computeIteration() {
//    std::vector<double> SigmaA, SigmaM;
//    NumA::VectorD a, b;
//    double stored_T;
//    double absDiff_a = 0., absDiff_b = 0., relDiff_a = 0., relDiff_b = 0.; // Difference between two iterations
//    bool noConvergence = true; // Convergence test
//    std::vector<double> qpFunctionsAtRoots;
//    std::vector<double> qpFunctionsAtNodes;
//    std::vector<QuarkPropagator::QPFunction> qpFunctions;
//    std::vector<double> A_r, B_r, sigmaV_r, sigmaS_r;
//    std::vector<double> A_n, B_n, sigmaV_n, sigmaS_n;
//    double denom_r, denom_n;
//    std::vector<double> radial_integrands;
//
//    unsigned int n;
//    for (n = getIters(); n < getMaxIter() && noConvergence; n++) {
//        a = getQuarkPropagator()->getCoeffsA();
//        b = getQuarkPropagator()->getCoeffsB();
//        SigmaA.assign(getN(), 0.);
//        SigmaM.assign(getN(), 0.);
//
//        A_r.assign(getN(), getAinit());
//        B_r.assign(getN(), getBinit());
//        sigmaV_r.assign(getN(), 0.);
//        sigmaS_r.assign(getN(), 0.);
//
//        A_n.assign(getNx(), getAinit());
//        B_n.assign(getNx(), getBinit());
//        sigmaV_n.assign(getNx(), 0.);
//        sigmaS_n.assign(getNx(), 0.);
//
//        for (unsigned int i = 0; i < getN(); i++) {
//            if (n > 0) {
//                qpFunctions.assign(2, QuarkPropagator::A);
//                qpFunctions.at(1) = QuarkPropagator::B;
//                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
//                        getRootsS().at(i));
//                A_r.at(i) = qpFunctionsAtRoots.at(0);
//                B_r.at(i) = qpFunctionsAtRoots.at(1);
//            }
//            denom_r = getRootsS().at(i) * A_r.at(i) * A_r.at(i)
//                    + B_r.at(i) * B_r.at(i);
//            sigmaV_r.at(i) = A_r.at(i) / denom_r;
//            sigmaS_r.at(i) = B_r.at(i) / denom_r;
//        }
//
//        for (unsigned int k = 0; k < getNx(); k++) {
//            if (n > 0) {
//                qpFunctions.assign(2, QuarkPropagator::A);
//                qpFunctions.at(1) = QuarkPropagator::B;
//                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
//                        getNodesS().at(k));
//                A_n.at(k) = qpFunctionsAtRoots.at(0);
//                B_n.at(k) = qpFunctionsAtRoots.at(1);
//            }
//            denom_n = getNodesS().at(k) * A_n.at(k) * A_n.at(k)
//                    + B_n.at(k) * B_n.at(k);
//            sigmaV_n.at(k) = A_n.at(k) / denom_n;
//            sigmaS_n.at(k) = B_n.at(k) / denom_n;
//        }
//
//        for (unsigned int i = 0; i < getN(); i++) {
//            for (unsigned int k = 0; k < getNx(); k++) {
//                radial_integrands = getVertex()->Radial_Integrands(
//                        getRootsS().at(i), getNodesS().at(k), A_r.at(i),
//                        A_n.at(k), B_r.at(i), B_n.at(k), sigmaV_r.at(i),
//                        sigmaV_n.at(k), sigmaS_r.at(i), sigmaS_n.at(k));
//                for (unsigned int j = 0;
//                        j < getVertex()->numberOfBasisElementsUsed(); j++) {
//                    SigmaA.at(i) += getC().at(k) * radial_integrands.at(j)
//                            * m_ThetaA.at(j).at(i).at(k);
//                    SigmaM.at(i) +=
//                            getC().at(k)
//                                    * radial_integrands.at(
//                                            j
//                                                    + getVertex()->numberOfBasisElementsUsed())
//                                    * m_ThetaM.at(j).at(i).at(k);
//                }
//            }
//        }
//        getQuarkPropagator()->setCoeffsAfromValueOnNodes(SigmaA);
//        getQuarkPropagator()->setCoeffsBfromValueOnNodes(SigmaM);
//
//        absDiff_a = (a - getQuarkPropagator()->getCoeffsA()).norm();
//        absDiff_b = (b - getQuarkPropagator()->getCoeffsB()).norm();
//        relDiff_a = absDiff_a / (a.norm() + 1.e-16);
//        relDiff_b = absDiff_b / (b.norm() + 1.e-16);
//
//        noConvergence = (absDiff_a > getAbsTolerance())
//                || (absDiff_b > getAbsTolerance())
//                || (relDiff_a > getRelTolerance())
//                || (relDiff_b > getRelTolerance());
//
//        // Test
//        ElemUtils::Formatter formatter1, formatter2, formatter3;
//        formatter1 << "Iteration " << n << ".";
//        formatter2 << "A : ";
//        for (unsigned int i = 0; i < getN(); i++) {
//            formatter2 << getQuarkPropagator()->evaluateA(getRootsS().at(i))
//                    << " ";
//        }
//        formatter3 << "B : ";
//        for (unsigned int i = 0; i < getN(); i++) {
//            formatter3 << getQuarkPropagator()->evaluateB(getRootsS().at(i))
//                    << " ";
//        }
//        info(__func__, formatter1.str());
//        debug(__func__, formatter2.str());
//        debug(__func__, formatter3.str());
//    }
//
//    setIters(n);
//}

//void GapEqSeparableSolver::computeIterations(IterativeType iterativeType) {
//    int N_newton = getN() * 2;
//    NumA::NewtonMD newtonIteration(N_newton);
//    NumA::VectorD a(getN()), b(getN()), a0(getN()), b0(getN()), X(N_newton), X0(
//            N_newton), G_X0(N_newton), G_X(N_newton), DeltaX(N_newton), DeltaG(
//            N_newton);
//    NumA::MatrixD J_G_X0(N_newton, N_newton), J_G_X(N_newton, N_newton);
//    double absDiff_a = 0., absDiff_b = 0., relDiff_a = 0., relDiff_b = 0.; // Difference between two iterations
//    bool noConvergence = true; // Convergence test
//    std::vector<double> qpFunctionsAtRoots;
//    std::vector<double> qpFunctionsAtNodes;
//    std::vector<QuarkPropagator::QPFunction> qpFunctions;
//    std::vector<double> SigmaA_r, SigmaM_r, A_r, B_r, sigmaV_r, sigmaS_r;
//    std::vector<double> A_n, B_n, sigmaV_n, sigmaS_n;
//    std::vector<std::vector<double> > dSigmaA_r, dSigmaM_r, dA_r, dB_r,
//            dsigmaV_a_r, dsigmaV_b_r, dsigmaS_a_r, dsigmaS_b_r;
//    std::vector<std::vector<double> > dA_n, dB_n, dsigmaV_a_n, dsigmaV_b_n,
//            dsigmaS_a_n, dsigmaS_b_n;
//    double denom_r, denom_n, sigmaV2_r, sigmaS2_r, sigmaV2_n, sigmaS2_n;
//    std::vector<double> radial_integrands, radial_integrands_deriv_a,
//            radial_integrands_deriv_b;
//
//    unsigned int n;
//    for (n = getIters(); n < getMaxIter() && noConvergence; n++) {
//
//        G_X.assign(N_newton, 0.);
//        J_G_X.assign(N_newton, N_newton, 0.);
//
//        SigmaA_r.assign(getN(), 0.);
//        SigmaM_r.assign(getN(), 0.);
//        A_r.assign(getN(), getAinit());
//        B_r.assign(getN(), getBinit());
//        sigmaV_r.assign(getN(), 0.);
//        sigmaS_r.assign(getN(), 0.);
//
//        A_n.assign(getNx(), getAinit());
//        B_n.assign(getNx(), getBinit());
//        sigmaV_n.assign(getNx(), 0.);
//        sigmaS_n.assign(getNx(), 0.);
//
//        if ((iterativeType == GapEquationSolverModule::Broyden && n == 0)
//                || iterativeType == GapEquationSolverModule::Newton) {
//            dSigmaA_r.assign(getN(), std::vector<double>(getN(), 0.));
//            dSigmaM_r.assign(getN(), std::vector<double>(getN(), 0.));
//            dA_r.assign(getN(), std::vector<double>(getN(), 0.));
//            dB_r.assign(getN(), std::vector<double>(getN(), 0.));
//            dsigmaV_a_r.assign(getN(), std::vector<double>(getN(), 0.));
//            dsigmaV_b_r.assign(getN(), std::vector<double>(getN(), 0.));
//            dsigmaS_a_r.assign(getN(), std::vector<double>(getN(), 0.));
//            dsigmaS_b_r.assign(getN(), std::vector<double>(getN(), 0.));
//
//            dA_n.assign(getNx(), std::vector<double>(getN(), 0.));
//            dB_n.assign(getNx(), std::vector<double>(getN(), 0.));
//            dsigmaV_a_n.assign(getNx(), std::vector<double>(getN(), 0.));
//            dsigmaV_b_n.assign(getNx(), std::vector<double>(getN(), 0.));
//            dsigmaS_a_n.assign(getNx(), std::vector<double>(getN(), 0.));
//            dsigmaS_b_n.assign(getNx(), std::vector<double>(getN(), 0.));
//        }
//
//        for (unsigned int i = 0; i < getN(); i++) {
//            if (n > 0) {
//                qpFunctions.assign(4, QuarkPropagator::SigmaA);
//                qpFunctions.at(1) = QuarkPropagator::SigmaM;
//                qpFunctions.at(2) = QuarkPropagator::A;
//                qpFunctions.at(3) = QuarkPropagator::B;
//                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
//                        getRootsS().at(i));
//                SigmaA_r.at(i) = qpFunctionsAtRoots.at(0);
//                SigmaM_r.at(i) = qpFunctionsAtRoots.at(1);
//                A_r.at(i) = qpFunctionsAtRoots.at(2);
//                B_r.at(i) = qpFunctionsAtRoots.at(3);
//            }
//            denom_r = getRootsS().at(i) * A_r.at(i) * A_r.at(i)
//                    + B_r.at(i) * B_r.at(i);
//            sigmaV_r.at(i) = A_r.at(i) / denom_r;
//            sigmaS_r.at(i) = B_r.at(i) / denom_r;
//
//            if ((iterativeType == GapEquationSolverModule::Broyden && n == 0)
//                    || iterativeType == GapEquationSolverModule::Newton) {
//                sigmaV2_r = sigmaV_r.at(i) * sigmaV_r.at(i);
//                sigmaS2_r = sigmaS_r.at(i) * sigmaS_r.at(i);
//
//                for (unsigned int j = 0; j < getN(); j++) {
//                    qpFunctions.assign(4, QuarkPropagator::dSigmaA);
//                    qpFunctions.at(1) = QuarkPropagator::dSigmaM;
//                    qpFunctions.at(2) = QuarkPropagator::dA;
//                    qpFunctions.at(3) = QuarkPropagator::dB;
//                    qpFunctionsAtRoots = getQuarkPropagator()->evaluate(
//                            qpFunctions, getRootsS().at(i), j);
//                    dSigmaA_r.at(i).at(j) = qpFunctionsAtRoots.at(0);
//                    dSigmaM_r.at(i).at(j) = qpFunctionsAtRoots.at(1);
//                    dA_r.at(i).at(j) = qpFunctionsAtRoots.at(2);
//                    dB_r.at(i).at(j) = qpFunctionsAtRoots.at(3);
//                    dsigmaV_a_r.at(i).at(j) = dA_r.at(i).at(j)
//                            * (sigmaS2_r - getRootsS().at(i) * sigmaV2_r);
//                    dsigmaV_b_r.at(i).at(j) = -2 * dB_r.at(i).at(j)
//                            * sigmaV_r.at(i) * sigmaS_r.at(i);
//                    dsigmaS_a_r.at(i).at(j) = -2 * getRootsS().at(i)
//                            * dA_r.at(i).at(j) * sigmaV_r.at(i)
//                            * sigmaS_r.at(i);
//                    dsigmaS_b_r.at(i).at(j) = dB_r.at(i).at(j)
//                            * (getRootsS().at(i) * sigmaV2_r - sigmaS2_r);
//                }
//            }
//
//        }
//
//        for (unsigned int k = 0; k < getNx(); k++) {
//            if (n > 0) {
//                qpFunctions.assign(2, QuarkPropagator::A);
//                qpFunctions.at(1) = QuarkPropagator::B;
//                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
//                        getNodesS().at(k));
//                A_n.at(k) = qpFunctionsAtRoots.at(0);
//                B_n.at(k) = qpFunctionsAtRoots.at(1);
//            }
//            denom_n = getNodesS().at(k) * A_n.at(k) * A_n.at(k)
//                    + B_n.at(k) * B_n.at(k);
//            sigmaV_n.at(k) = A_n.at(k) / denom_n;
//            sigmaS_n.at(k) = B_n.at(k) / denom_n;
//
//            if ((iterativeType == GapEquationSolverModule::Broyden && n == 0)
//                    || iterativeType == GapEquationSolverModule::Newton) {
//                sigmaV2_n = sigmaV_n.at(k) * sigmaV_n.at(k);
//                sigmaS2_n = sigmaS_n.at(k) * sigmaS_n.at(k);
//
//                for (unsigned int j = 0; j < getN(); j++) {
//                    qpFunctions.assign(2, QuarkPropagator::dA);
//                    qpFunctions.at(1) = QuarkPropagator::dB;
//                    qpFunctionsAtRoots = getQuarkPropagator()->evaluate(
//                            qpFunctions, getNodesS().at(k), j);
//                    dA_n.at(k).at(j) = qpFunctionsAtRoots.at(0);
//                    dB_n.at(k).at(j) = qpFunctionsAtRoots.at(1);
//                    dsigmaV_a_n.at(k).at(j) = dA_n.at(k).at(j)
//                            * (sigmaS2_n - getNodesS().at(k) * sigmaV2_n);
//                    dsigmaV_b_n.at(k).at(j) = -2 * dB_n.at(k).at(j)
//                            * sigmaV_n.at(k) * sigmaS_n.at(k);
//                    dsigmaS_a_n.at(k).at(j) = -2 * getNodesS().at(k)
//                            * dA_n.at(k).at(j) * sigmaV_n.at(k)
//                            * sigmaS_n.at(k);
//                    dsigmaS_b_n.at(k).at(j) = dB_n.at(k).at(j)
//                            * (getNodesS().at(k) * sigmaV2_n - sigmaS2_n);
//                }
//            }
//
//        }
//
//        for (unsigned int i = 0; i < getN(); i++) {
//            G_X.at(i) = SigmaA_r.at(i);
//            G_X.at(getN() + i) = SigmaM_r.at(i);
//            for (unsigned int k = 0; k < getNx(); k++) {
//                radial_integrands = getVertex()->Radial_Integrands(
//                        getRootsS().at(i), getNodesS().at(k), A_r.at(i),
//                        A_n.at(k), B_r.at(i), B_n.at(k), sigmaV_r.at(i),
//                        sigmaV_n.at(k), sigmaS_r.at(i), sigmaS_n.at(k));
//                for (unsigned int l = 0;
//                        l < getVertex()->numberOfBasisElementsUsed(); l++) {
//                    G_X.at(i) -= getC().at(k) * radial_integrands.at(l)
//                            * m_ThetaA.at(l).at(i).at(k);
//                    G_X.at(getN() + i) -=
//                            getC().at(k)
//                                    * radial_integrands.at(
//                                            l
//                                                    + getVertex()->numberOfBasisElementsUsed())
//                                    * m_ThetaM.at(l).at(i).at(k);
//                }
//            }
//        }
//        DeltaG = G_X - G_X0;
//
//        if ((iterativeType == GapEquationSolverModule::Broyden && n == 0)
//                || iterativeType == GapEquationSolverModule::Newton) {
//            for (unsigned int i = 0; i < getN(); i++) {
//                for (unsigned int j = 0; j < getN(); j++) {
//                    J_G_X.at(i, j) = dSigmaA_r.at(i).at(j);
//                    J_G_X.at(getN() + i, getN() + j) = dSigmaM_r.at(i).at(j);
//                    for (unsigned int k = 0; k < getNx(); k++) {
//                        radial_integrands_deriv_a =
//                                getVertex()->Radial_Integrands_deriv(
//                                        getRootsS().at(i), getNodesS().at(k),
//                                        A_r.at(i), A_n.at(k), dA_r.at(i).at(j),
//                                        dA_n.at(k).at(j), B_r.at(i), B_n.at(k),
//                                        0., 0., sigmaV_r.at(i), sigmaV_n.at(k),
//                                        sigmaS_r.at(i), sigmaS_n.at(k),
//                                        dsigmaV_a_r.at(i).at(j),
//                                        dsigmaV_a_n.at(k).at(j),
//                                        dsigmaS_a_r.at(i).at(j),
//                                        dsigmaS_a_n.at(k).at(j));
//                        radial_integrands_deriv_b =
//                                getVertex()->Radial_Integrands_deriv(
//                                        getRootsS().at(i), getNodesS().at(k),
//                                        A_r.at(i), A_n.at(k), 0., 0., B_r.at(i),
//                                        B_n.at(k), dB_r.at(i).at(j),
//                                        dB_n.at(k).at(j), sigmaV_r.at(i),
//                                        sigmaV_n.at(k), sigmaS_r.at(i),
//                                        sigmaS_n.at(k), dsigmaV_b_r.at(i).at(j),
//                                        dsigmaV_b_n.at(k).at(j),
//                                        dsigmaS_b_r.at(i).at(j),
//                                        dsigmaS_b_n.at(k).at(j));
//                        for (unsigned int l = 0;
//                                l < getVertex()->numberOfBasisElementsUsed();
//                                l++) {
//                            J_G_X.at(i, j) -= getC().at(k)
//                                    * radial_integrands_deriv_a.at(l)
//                                    * m_ThetaA.at(l).at(i).at(k);
//                            J_G_X.at(getN() + i, j) -=
//                                    getC().at(k)
//                                            * radial_integrands_deriv_a.at(
//                                                    l
//                                                            + getVertex()->numberOfBasisElementsUsed())
//                                            * m_ThetaM.at(l).at(i).at(k);
//                            J_G_X.at(i, getN() + j) -= getC().at(k)
//                                    * radial_integrands_deriv_b.at(l)
//                                    * m_ThetaA.at(l).at(i).at(k);
//                            J_G_X.at(getN() + i, getN() + j) -=
//                                    getC().at(k)
//                                            * radial_integrands_deriv_b.at(
//                                                    l
//                                                            + getVertex()->numberOfBasisElementsUsed())
//                                            * m_ThetaM.at(l).at(i).at(k);
//                        }
//                    }
//                }
//            }
//        } else if (iterativeType == GapEquationSolverModule::Broyden) {
//            J_G_X = J_G_X0
//                    + (DeltaG - J_G_X0 * DeltaX).toMatrix()
//                            * DeltaX.toMatrix().transpose() / (DeltaX * DeltaX);
//        }
//        a0 = getQuarkPropagator()->getCoeffsA();
//        b0 = getQuarkPropagator()->getCoeffsB();
//        X0 = NumA::VectorD::concatenate(a0, b0);
//        G_X0 = G_X;
//        J_G_X0 = J_G_X;
//        if (iterativeType == GapEquationSolverModule::Newton
//                || iterativeType == GapEquationSolverModule::Broyden) {
//            X = newtonIteration.iterate(X0, G_X0, J_G_X0);
//            a = X.sub(0, getN());
//            b = X.sub(getN(), N_newton);
//        } else {
//            getQuarkPropagator()->setCoeffsAfromValueOnNodes(
//                    (NumA::VectorD(SigmaA_r) - G_X0.sub(0, getN())).toStdVector());
//            getQuarkPropagator()->setCoeffsBfromValueOnNodes(
//                    (NumA::VectorD(SigmaM_r) - G_X0.sub(0, getN())).toStdVector());
//            a = getQuarkPropagator()->getCoeffsA();
//            b = getQuarkPropagator()->getCoeffsB();
//            X = NumA::VectorD::concatenate(a, b);
//        }
//        DeltaX = X - X0;
//
//        absDiff_a = (a - a0).norm();
//        absDiff_b = (b - b0).norm();
//        relDiff_a = absDiff_a / (a0.norm() + 1.e-16);
//        relDiff_b = absDiff_b / (b0.norm() + 1.e-16);
//
//        noConvergence = (absDiff_a > getAbsTolerance())
//                || (absDiff_b > getAbsTolerance())
//                || (relDiff_a > getRelTolerance())
//                || (relDiff_b > getRelTolerance());
//        getQuarkPropagator()->setCoeffsA(a);
//        getQuarkPropagator()->setCoeffsB(b);
//
//        // Test
//        ElemUtils::Formatter formatter1, formatter2, formatter3;
//        formatter1 << "Iteration " << n << ".";
//        formatter2 << "A : ";
//        for (unsigned int i = 0; i < getN(); i++) {
//            formatter2 << getQuarkPropagator()->evaluateA(getRootsS().at(i))
//                    << " ";
//        }
//        formatter3 << "B : ";
//        for (unsigned int i = 0; i < getN(); i++) {
//            formatter3 << getQuarkPropagator()->evaluateB(getRootsS().at(i))
//                    << " ";
//        }
//        info(__func__, formatter1.str());
//        debug(__func__, formatter2.str());
//        debug(__func__, formatter3.str());
//    }
//
//    setConverged(!noConvergence);
//    setIters(n);
//}

NumA::VectorD GapEqSeparableSolver::G_func(const NumA::VectorD& X) {
    if (X.size() != get2N()) {
        ElemUtils::CustomException(getClassName(), __func__,
                (ElemUtils::Formatter() << "Size of X (" << X.size()
                        << ") is wrong. It should be 2*N = " << get2N() << ".").str());
    }
    m_Ap = X.sub(0, getN());
    m_Bp = X.sub(getN(), get2N());
    m_Aq = getInterpolationMatrix() * m_Ap;
    m_Bq = getInterpolationMatrix() * m_Bp;
    m_Amu = getInterpolationMu() * m_Ap;
    m_Bmu = getInterpolationMu() * m_Bp;
    m_Ap.push_back(m_Amu);
    m_Bp.push_back(m_Bmu);

    double denom;
    m_sigmaVp = m_Ap;
    m_sigmaSp = m_Bp;
    for (unsigned int i = 0; i < getN() + 1; i++) {
        denom = getRootsS().at(i) * m_Ap.at(i) * m_Ap.at(i)
                + m_Bp.at(i) * m_Bp.at(i);
        m_sigmaVp.at(i) /= denom;
        m_sigmaSp.at(i) /= denom;
    }
    m_sigmaVq = m_Aq;
    m_sigmaSq = m_Bq;
    for (unsigned int k = 0; k < getNx(); k++) {
        denom = getNodesS().at(k) * m_Aq.at(k) * m_Aq.at(k)
                + m_Bq.at(k) * m_Bq.at(k);
        m_sigmaVq.at(k) /= denom;
        m_sigmaSq.at(k) /= denom;

    }

    m_G_X0 = m_G_X;
//    m_G_X.assign(get2N());
    m_SigmaAp.assign(getN() + 1, 0.);
    m_SigmaMp.assign(getN() + 1, 0.);
    std::vector<double> radial_integrands;
    for (unsigned int i = 0; i < getN() + 1; i++) {
        for (unsigned int k = 0; k < getNx(); k++) {
            radial_integrands = getVertex()->Radial_Integrands(
                    getRootsS().at(i), getNodesS().at(k), m_Ap.at(i),
                    m_Aq.at(k), m_Bp.at(i), m_Bq.at(k), m_sigmaVp.at(i),
                    m_sigmaVq.at(k), m_sigmaSp.at(i), m_sigmaSq.at(k));
            for (unsigned int l = 0;
                    l < getVertex()->numberOfBasisElementsUsed(); l++) {
                m_SigmaAp.at(i) += getC().at(k) * radial_integrands.at(l)
                        * m_ThetaA.at(l).at(i).at(k);
                m_SigmaMp.at(i) += getC().at(k)
                        * radial_integrands.at(
                                l + getVertex()->numberOfBasisElementsUsed())
                        * m_ThetaM.at(l).at(i).at(k);
            }
        }
    }
    for (unsigned int i = 0; i < getN(); i++) {
        m_G_X.at(i) = m_Ap.at(i) - 1. - m_SigmaAp.at(i) + m_SigmaAp.at(getN());
        m_G_X.at(getN() + i) = m_Bp.at(i) - getM() - m_SigmaMp.at(i)
                + m_SigmaMp.at(getN());
    }
    m_DeltaG = m_G_X - m_G_X0;

    return m_G_X;
}

NumA::MatrixD GapEqSeparableSolver::J_G_func(const NumA::VectorD& X) {
    return NumA::MatrixD();
}
