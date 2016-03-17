#include "../../../../include/partons/modules/dse/GapEqSeparableSolver.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/linear_algebra/matrix/MatrixD.h>
#include <NumA/linear_algebra/vector/VectorD.h>
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
        GapEquationSolverModule(className) {

}
GapEqSeparableSolver::~GapEqSeparableSolver() {
    // TODO Auto-generated destructor stub
}

GapEqSeparableSolver::GapEqSeparableSolver(const GapEqSeparableSolver& other) :
        GapEquationSolverModule(other) {
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
                std::vector<std::vector<double> >(getN(),
                        std::vector<double>(getNx(), 0.)));
        m_ThetaM.assign(getVertex()->numberOfBasisElementsUsed(),
                std::vector<std::vector<double> >(getN(),
                        std::vector<double>(getNx(), 0.)));
        for (unsigned int l = 0; l < getNz(); l++) {
            Cz = getWeightsZ().at(l)
                    * sqrt(1 - getNodesZ().at(l) * getNodesZ().at(l));
            for (unsigned int i = 0; i < getN(); i++) {
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

void GapEqSeparableSolver::computeNewtonInteration() {
    int N_newton = getN() * 2;
    NumA::NewtonMD newtonIteration(N_newton);
    NumA::VectorD a(getN()), b(getN()), X(N_newton), X0(N_newton), G_X0(
            N_newton);
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
    std::vector<double> radial_integrands, radial_integrands_deriv_a,
            radial_integrands_deriv_b;

    unsigned int n;
    for (n = getIters(); n < getMaxIter() && noConvergence; n++) {

        G_X0.assign(N_newton, 0.);
        J_G_X0.assign(N_newton, N_newton, 0.);

        SigmaA_r.assign(getN(), 0.);
        SigmaM_r.assign(getN(), 0.);
        A_r.assign(getN(), getAinit());
        B_r.assign(getN(), getBinit());
        sigmaV_r.assign(getN(), 0.);
        sigmaS_r.assign(getN(), 0.);

        dSigmaA_r.assign(getN(), std::vector<double>(getN(), 0.));
        dSigmaM_r.assign(getN(), std::vector<double>(getN(), 0.));
        dA_r.assign(getN(), std::vector<double>(getN(), 0.));
        dB_r.assign(getN(), std::vector<double>(getN(), 0.));
        dsigmaV_a_r.assign(getN(), std::vector<double>(getN(), 0.));
        dsigmaV_b_r.assign(getN(), std::vector<double>(getN(), 0.));
        dsigmaS_a_r.assign(getN(), std::vector<double>(getN(), 0.));
        dsigmaS_b_r.assign(getN(), std::vector<double>(getN(), 0.));

        A_n.assign(getNx(), getAinit());
        B_n.assign(getNx(), getBinit());
        sigmaV_n.assign(getNx(), 0.);
        sigmaS_n.assign(getNx(), 0.);

        dA_n.assign(getNx(), std::vector<double>(getN(), 0.));
        dB_n.assign(getNx(), std::vector<double>(getN(), 0.));
        dsigmaV_a_n.assign(getNx(), std::vector<double>(getN(), 0.));
        dsigmaV_b_n.assign(getNx(), std::vector<double>(getN(), 0.));
        dsigmaS_a_n.assign(getNx(), std::vector<double>(getN(), 0.));
        dsigmaS_b_n.assign(getNx(), std::vector<double>(getN(), 0.));

        for (unsigned int i = 0; i < getN(); i++) {
            if (n > 0) {
                qpFunctions.assign(4, QuarkPropagator::SigmaA);
                qpFunctions.at(1) = QuarkPropagator::SigmaM;
                qpFunctions.at(2) = QuarkPropagator::A;
                qpFunctions.at(3) = QuarkPropagator::B;
                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
                        getRootsS().at(i));
                SigmaA_r.at(i) = qpFunctionsAtRoots.at(0);
                SigmaM_r.at(i) = qpFunctionsAtRoots.at(1);
                A_r.at(i) = qpFunctionsAtRoots.at(2);
                B_r.at(i) = qpFunctionsAtRoots.at(3);
            }
            denom_r = getRootsS().at(i) * A_r.at(i) * A_r.at(i)
                    + B_r.at(i) * B_r.at(i);
            sigmaV_r.at(i) = A_r.at(i) / denom_r;
            sigmaS_r.at(i) = B_r.at(i) / denom_r;

            sigmaV2_r = sigmaV_r.at(i) * sigmaV_r.at(i);
            sigmaS2_r = sigmaS_r.at(i) * sigmaS_r.at(i);

            for (unsigned int j = 0; j < getN(); j++) {
                qpFunctions.assign(4, QuarkPropagator::dSigmaA);
                qpFunctions.at(1) = QuarkPropagator::dSigmaM;
                qpFunctions.at(2) = QuarkPropagator::dA;
                qpFunctions.at(3) = QuarkPropagator::dB;
                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
                        getRootsS().at(i), j);
                dSigmaA_r.at(i).at(j) = qpFunctionsAtRoots.at(0);
                dSigmaM_r.at(i).at(j) = qpFunctionsAtRoots.at(1);
                dA_r.at(i).at(j) = qpFunctionsAtRoots.at(2);
                dB_r.at(i).at(j) = qpFunctionsAtRoots.at(3);
                dsigmaV_a_r.at(i).at(j) = dA_r.at(i).at(j)
                        * (sigmaS2_r - getRootsS().at(i) * sigmaV2_r);
                dsigmaV_b_r.at(i).at(j) = -2 * dB_r.at(i).at(j) * sigmaV_r.at(i)
                        * sigmaS_r.at(i);
                dsigmaS_a_r.at(i).at(j) = -2 * getRootsS().at(i)
                        * dA_r.at(i).at(j) * sigmaV_r.at(i) * sigmaS_r.at(i);
                dsigmaS_b_r.at(i).at(j) = dB_r.at(i).at(j)
                        * (getRootsS().at(i) * sigmaV2_r - sigmaS2_r);
            }

        }

        for (unsigned int k = 0; k < getNx(); k++) {
            if (n > 0) {
                qpFunctions.assign(2, QuarkPropagator::A);
                qpFunctions.at(1) = QuarkPropagator::B;
                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
                        getNodesS().at(k));
                A_n.at(k) = qpFunctionsAtRoots.at(0);
                B_n.at(k) = qpFunctionsAtRoots.at(1);
            }
            denom_n = getNodesS().at(k) * A_n.at(k) * A_n.at(k)
                    + B_n.at(k) * B_n.at(k);
            sigmaV_n.at(k) = A_n.at(k) / denom_n;
            sigmaS_n.at(k) = B_n.at(k) / denom_n;

            sigmaV2_n = sigmaV_n.at(k) * sigmaV_n.at(k);
            sigmaS2_n = sigmaS_n.at(k) * sigmaS_n.at(k);

            for (unsigned int j = 0; j < getN(); j++) {
                qpFunctions.assign(2, QuarkPropagator::dA);
                qpFunctions.at(1) = QuarkPropagator::dB;
                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
                        getNodesS().at(k), j);
                dA_n.at(k).at(j) = qpFunctionsAtRoots.at(0);
                dB_n.at(k).at(j) = qpFunctionsAtRoots.at(1);
                dsigmaV_a_n.at(k).at(j) = dA_n.at(k).at(j)
                        * (sigmaS2_n - getNodesS().at(k) * sigmaV2_n);
                dsigmaV_b_n.at(k).at(j) = -2 * dB_n.at(k).at(j) * sigmaV_n.at(k)
                        * sigmaS_n.at(k);
                dsigmaS_a_n.at(k).at(j) = -2 * getNodesS().at(k)
                        * dA_n.at(k).at(j) * sigmaV_n.at(k) * sigmaS_n.at(k);
                dsigmaS_b_n.at(k).at(j) = dB_n.at(k).at(j)
                        * (getNodesS().at(k) * sigmaV2_n - sigmaS2_n);
            }

        }

        for (unsigned int i = 0; i < getN(); i++) {

            G_X0.at(i) = SigmaA_r.at(i);
            G_X0.at(getN() + i) = SigmaM_r.at(i);
            for (unsigned int k = 0; k < getNx(); k++) {
                radial_integrands = getVertex()->Radial_Integrands(
                        getRootsS().at(i), getNodesS().at(k), A_r.at(i),
                        A_n.at(k), B_r.at(i), B_n.at(k), sigmaV_r.at(i),
                        sigmaV_n.at(k), sigmaS_r.at(i), sigmaS_n.at(k));
                for (unsigned int l = 0;
                        l < getVertex()->numberOfBasisElementsUsed(); l++) {
                    G_X0.at(i) -= getC().at(k) * radial_integrands.at(l)
                            * m_ThetaA.at(l).at(i).at(k);
                    G_X0.at(getN() + i) -=
                            getC().at(k)
                                    * radial_integrands.at(
                                            l
                                                    + getVertex()->numberOfBasisElementsUsed())
                                    * m_ThetaM.at(l).at(i).at(k);
                }
            }

            for (unsigned int j = 0; j < getN(); j++) {
                J_G_X0.at(i, j) = dSigmaA_r.at(i).at(j);
                J_G_X0.at(getN() + i, getN() + j) = dSigmaM_r.at(i).at(j);
                for (unsigned int k = 0; k < getNx(); k++) {
                    radial_integrands_deriv_a =
                            getVertex()->Radial_Integrands_deriv(
                                    getRootsS().at(i), getNodesS().at(k),
                                    A_r.at(i), A_n.at(k), dA_r.at(i).at(j),
                                    dA_n.at(k).at(j), B_r.at(i), B_n.at(k), 0.,
                                    0., sigmaV_r.at(i), sigmaV_n.at(k),
                                    sigmaS_r.at(i), sigmaS_n.at(k),
                                    dsigmaV_a_r.at(i).at(j),
                                    dsigmaV_a_n.at(k).at(j),
                                    dsigmaS_a_r.at(i).at(j),
                                    dsigmaS_a_n.at(k).at(j));
                    radial_integrands_deriv_b =
                            getVertex()->Radial_Integrands_deriv(
                                    getRootsS().at(i), getNodesS().at(k),
                                    A_r.at(i), A_n.at(k), 0., 0., B_r.at(i),
                                    B_n.at(k), dB_r.at(i).at(j),
                                    dB_n.at(k).at(j), sigmaV_r.at(i),
                                    sigmaV_n.at(k), sigmaS_r.at(i),
                                    sigmaS_n.at(k), dsigmaV_b_r.at(i).at(j),
                                    dsigmaV_b_n.at(k).at(j),
                                    dsigmaS_b_r.at(i).at(j),
                                    dsigmaS_b_n.at(k).at(j));
                    for (unsigned int l = 0;
                            l < getVertex()->numberOfBasisElementsUsed(); l++) {
                        J_G_X0.at(i, j) -= getC().at(k)
                                * radial_integrands_deriv_a.at(l)
                                * m_ThetaA.at(l).at(i).at(k);
                        J_G_X0.at(getN() + i, j) -=
                                getC().at(k)
                                        * radial_integrands_deriv_a.at(
                                                l
                                                        + getVertex()->numberOfBasisElementsUsed())
                                        * m_ThetaM.at(l).at(i).at(k);
                        J_G_X0.at(i, getN() + j) -= getC().at(k)
                                * radial_integrands_deriv_b.at(l)
                                * m_ThetaA.at(l).at(i).at(k);
                        J_G_X0.at(getN() + i, getN() + j) -=
                                getC().at(k)
                                        * radial_integrands_deriv_b.at(
                                                l
                                                        + getVertex()->numberOfBasisElementsUsed())
                                        * m_ThetaM.at(l).at(i).at(k);
                    }
                }
            }
        }
        X0 = NumA::VectorD::concatenate(getQuarkPropagator()->getCoeffsA(),
                getQuarkPropagator()->getCoeffsB());
        X = newtonIteration.iterate(X0, G_X0, J_G_X0);
        a = X.sub(0, getN());
        b = X.sub(getN(), N_newton);

        absDiff_a = (a - getQuarkPropagator()->getCoeffsA()).norm();
        absDiff_b = (b - getQuarkPropagator()->getCoeffsB()).norm();
        relDiff_a = absDiff_a
                / (getQuarkPropagator()->getCoeffsA().norm() + 1.e-16);
        relDiff_b = absDiff_b
                / (getQuarkPropagator()->getCoeffsB().norm() + 1.e-16);

        noConvergence = (absDiff_a > getAbsTolerance())
                || (absDiff_b > getAbsTolerance())
                || (relDiff_a > getRelTolerance())
                || (relDiff_b > getRelTolerance());
        getQuarkPropagator()->setCoeffsA(a);
        getQuarkPropagator()->setCoeffsB(b);

        // Test
        ElemUtils::Formatter formatter1, formatter2, formatter3;
        formatter1 << "Iteration " << n << ".";
        formatter2 << "A : ";
        for (unsigned int i = 0; i < getN(); i++) {
            formatter2 << getQuarkPropagator()->evaluateA(getRootsS().at(i))
                    << " ";
        }
        formatter3 << "B : ";
        for (unsigned int i = 0; i < getN(); i++) {
            formatter3 << getQuarkPropagator()->evaluateB(getRootsS().at(i))
                    << " ";
        }
        info(__func__, formatter1.str());
        debug(__func__, formatter2.str());
        debug(__func__, formatter3.str());
    }

    setIters(n);
}

void GapEqSeparableSolver::computeIteration() {
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
    std::vector<double> radial_integrands;

    unsigned int n;
    for (n = getIters(); n < getMaxIter() && noConvergence; n++) {
        a = getQuarkPropagator()->getCoeffsA();
        b = getQuarkPropagator()->getCoeffsB();
        SigmaA.assign(getN(), 0.);
        SigmaM.assign(getN(), 0.);

        A_r.assign(getN(), getAinit());
        B_r.assign(getN(), getBinit());
        sigmaV_r.assign(getN(), 0.);
        sigmaS_r.assign(getN(), 0.);

        A_n.assign(getNx(), getAinit());
        B_n.assign(getNx(), getBinit());
        sigmaV_n.assign(getNx(), 0.);
        sigmaS_n.assign(getNx(), 0.);

        for (unsigned int i = 0; i < getN(); i++) {
            if (n > 0) {
                qpFunctions.assign(2, QuarkPropagator::A);
                qpFunctions.at(1) = QuarkPropagator::B;
                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
                        getRootsS().at(i));
                A_r.at(i) = qpFunctionsAtRoots.at(0);
                B_r.at(i) = qpFunctionsAtRoots.at(1);
            }
            denom_r = getRootsS().at(i) * A_r.at(i) * A_r.at(i)
                    + B_r.at(i) * B_r.at(i);
            sigmaV_r.at(i) = A_r.at(i) / denom_r;
            sigmaS_r.at(i) = B_r.at(i) / denom_r;
        }

        for (unsigned int k = 0; k < getNx(); k++) {
            if (n > 0) {
                qpFunctions.assign(2, QuarkPropagator::A);
                qpFunctions.at(1) = QuarkPropagator::B;
                qpFunctionsAtRoots = getQuarkPropagator()->evaluate(qpFunctions,
                        getNodesS().at(k));
                A_n.at(k) = qpFunctionsAtRoots.at(0);
                B_n.at(k) = qpFunctionsAtRoots.at(1);
            }
            denom_n = getNodesS().at(k) * A_n.at(k) * A_n.at(k)
                    + B_n.at(k) * B_n.at(k);
            sigmaV_n.at(k) = A_n.at(k) / denom_n;
            sigmaS_n.at(k) = B_n.at(k) / denom_n;
        }

        for (unsigned int i = 0; i < getN(); i++) {
            for (unsigned int k = 0; k < getNx(); k++) {
                radial_integrands = getVertex()->Radial_Integrands(
                        getRootsS().at(i), getNodesS().at(k), A_r.at(i),
                        A_n.at(k), B_r.at(i), B_n.at(k), sigmaV_r.at(i),
                        sigmaV_n.at(k), sigmaS_r.at(i), sigmaS_n.at(k));
                for (unsigned int j = 0;
                        j < getVertex()->numberOfBasisElementsUsed(); j++) {
                    SigmaA.at(i) += getC().at(k) * radial_integrands.at(j)
                            * m_ThetaA.at(j).at(i).at(k);
                    SigmaM.at(i) +=
                            getC().at(k)
                                    * radial_integrands.at(
                                            j
                                                    + getVertex()->numberOfBasisElementsUsed())
                                    * m_ThetaM.at(j).at(i).at(k);
                }
            }
        }
        getQuarkPropagator()->setCoeffsAfromValueOnNodes(SigmaA);
        getQuarkPropagator()->setCoeffsBfromValueOnNodes(SigmaM);

        absDiff_a = (a - getQuarkPropagator()->getCoeffsA()).norm();
        absDiff_b = (b - getQuarkPropagator()->getCoeffsB()).norm();
        relDiff_a = absDiff_a / (a.norm() + 1.e-16);
        relDiff_b = absDiff_b / (b.norm() + 1.e-16);

        noConvergence = (absDiff_a > getAbsTolerance())
                || (absDiff_b > getAbsTolerance())
                || (relDiff_a > getRelTolerance())
                || (relDiff_b > getRelTolerance());

        // Test
        ElemUtils::Formatter formatter1, formatter2, formatter3;
        formatter1 << "Iteration " << n << ".";
        formatter2 << "A : ";
        for (unsigned int i = 0; i < getN(); i++) {
            formatter2 << getQuarkPropagator()->evaluateA(getRootsS().at(i))
                    << " ";
        }
        formatter3 << "B : ";
        for (unsigned int i = 0; i < getN(); i++) {
            formatter3 << getQuarkPropagator()->evaluateB(getRootsS().at(i))
                    << " ";
        }
        info(__func__, formatter1.str());
        debug(__func__, formatter2.str());
        debug(__func__, formatter3.str());
    }

    setIters(n);
}
