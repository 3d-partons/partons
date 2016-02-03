/**
 * @file GapEquationSolverModule.cpp
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 22, 2016
 * @version 1.0
 *
 * @class GapEquationSolverModule
 */

#include "../../../../include/partons/modules/dse/GapEquationSolverModule.h"

#include <NumA/integration/GLNPIntegrationMode.h>
#include <cmath>

#include "../../../../include/partons/beans/dse/GluonPropagator.h"
#include "../../../../include/partons/beans/dse/QPbyChebyshev.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/utils/ParameterList.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"

GapEquationSolverModule::GapEquationSolverModule(const std::string &className) :
        ModuleObject(className), m_gluonPropagator(0) {

    setLambda2(1.e5);
    setEpsilon2(1.e-4);
    m_mu = 19.;
    m_m = 5.e-3;
    m_N = 50;
    m_Nx = 120;
    m_Nz = 32;
    m_tolerance = 1.e-4;
    m_maxIter = 20;
}

GapEquationSolverModule::~GapEquationSolverModule() {
    // TODO Auto-generated destructor stub
}

GapEquationSolverModule::GapEquationSolverModule(
        const GapEquationSolverModule& other) :
        ModuleObject(other) {
    setLambda2(other.getLambda2());
    setEpsilon2(other.getEpsilon2());
    m_mu = other.getMu();
    m_m = other.getM();
    m_N = other.getN();
    m_Nx = other.getNx();
    m_Nz = other.getNz();
    m_tolerance = other.getTolerance();
    m_maxIter = other.getMaxIter();

}

void GapEquationSolverModule::initModule() {
    m_C = (m_Lambda2 * m_epsilon2) * log(m_Lambda2 / m_epsilon2)
            / (2. * pow(2. * PI, 3));
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

QuarkPropagator* GapEquationSolverModule::compute(
        GapEquationSolverModule::QPType qpType,
        GapEquationSolverModule::IterativeType iterativeType,
        GapEquationSolverModule::IntegrationType integrationType) {

    initModule();

    // Gauss-Legendre integration //TODO Implement other cases
    NumA::GLNPIntegrationMode gaussLeg_x, gaussLeg_z;
    gaussLeg_x.makeNodeAndWeightVectors(m_Nx);
    gaussLeg_z.makeNodeAndWeightVectors(m_Nz);
    std::vector<double> nodes_x = gaussLeg_x.getNodeNp();
    std::vector<double> weights_x = gaussLeg_x.getWeightNp();
    // Test
//    std::vector<double> nodes_z = gaussLeg_z.getNodeNp();
//    std::vector<double> weights_z = gaussLeg_z.getWeightNp();
//    Formatter formatter;
//    formatter << "Taille des noeuds et poids : ";
//    formatter << nodes_x.size() << " " << weights_x.size() << " "
//            << nodes_z.size() << " " << weights_z.size();
//    info(__func__, formatter.str());

// Chebyshev representation //TODO Implement other cases
    QPbyChebyshev* pQuarkPropagator = new QPbyChebyshev(m_N, m_m, m_mu,
            m_Lambda2, m_epsilon2);
    std::vector<double> chebRoots = pQuarkPropagator->getRoots(); //TODO Move to NumA

    // Conversion between x and s
    std::vector<double> chebRoots_s(m_N, 0.);
    for (unsigned int i = 0; i < m_N; i++) {
        chebRoots_s.at(i) = pQuarkPropagator->xtos(chebRoots.at(i));
    }
    std::vector<double> nodes_s(m_Nx, 0.);
    std::vector<double> C(m_Nx, 0.);
    for (unsigned int k = 0; k < m_Nx; k++) {
        nodes_s.at(k) = pQuarkPropagator->xtos(nodes_x.at(k));
        C.at(k) = m_C * weights_x.at(k)
                * exp(nodes_x.at(k) * log(m_Lambda2 / m_epsilon2));
    }

    // Angular Integrals
    std::vector<std::vector<double> > ThetaA(m_N,
            std::vector<double>(m_Nx, 0.));
    std::vector<std::vector<double> > ThetaM(m_N,
            std::vector<double>(m_Nx, 0.));
    std::vector<double> parameters(2, 0.);
    for (unsigned int i = 0; i < m_N; i++) {
        parameters.at(0) = chebRoots_s.at(i);
        for (unsigned int k = 0; k < m_Nx; k++) {
            parameters.at(1) = nodes_s.at(k);
            ThetaA.at(i).at(k) = gaussLeg_z.integrate(this,
                    &GapEquationSolverModule::ThetaA_func, -1., 1., parameters,
                    m_Nz);
            ThetaM.at(i).at(k) = gaussLeg_z.integrate(this,
                    &GapEquationSolverModule::ThetaM_func, -1., 1., parameters,
                    m_Nz);
        }
    }

    // Natural iterative method //TODO Implement other cases
    std::vector<double> SigmaA, SigmaM, a, b;
    double stored_T;
    double diff_a = 0., diff_b = 0.; // Difference between two iterations
    bool noConvergence = true; // Convergence test
    for (unsigned int n = 0; n < m_maxIter && noConvergence; n++) {
        SigmaA.assign(m_N, 0.);
        SigmaM.assign(m_N, 0.);
        for (unsigned int i = 0; i < m_N; i++) {
            for (unsigned int k = 0; k < m_Nx; k++) {
                SigmaA.at(i) += C.at(k)
                        * pQuarkPropagator->evaluateSigmaV(nodes_s.at(k))
                        * ThetaA.at(i).at(k);
                SigmaM.at(i) += C.at(k)
                        * pQuarkPropagator->evaluateSigmaS(nodes_s.at(k))
                        * ThetaM.at(i).at(k);
            }
        }
        a.assign(m_N, 0.);
        b.assign(m_N, 0.);
        diff_a = 0.;
        diff_b = 0.; // Difference between two iterations
        for (unsigned int i = 0; i < m_N; i++) {
            for (unsigned int k = 0; k < m_N; k++) {
                stored_T = pQuarkPropagator->T(i, chebRoots.at(k));
                a.at(i) += stored_T * SigmaA.at(k);
                b.at(i) += stored_T * SigmaM.at(k);
            }
            a.at(i) *= 2. / m_N;
            b.at(i) *= 2. / m_N;
            diff_a += pow(a.at(i) - pQuarkPropagator->getCoeffA(i), 2);
            diff_b += pow(b.at(i) - pQuarkPropagator->getCoeffB(i), 2);
        }

        noConvergence = (sqrt(diff_a) > m_tolerance)
                || (sqrt(diff_b) > m_tolerance);
        pQuarkPropagator->setCoeffsA(a);
        pQuarkPropagator->setCoeffsB(b);

        // Test
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
    }

    return pQuarkPropagator;

}

const GluonPropagator* GapEquationSolverModule::getGluonPropagator() const {
    return m_gluonPropagator;
}

void GapEquationSolverModule::setGluonPropagator(
        const GluonPropagator* gluonPropagator) {
    m_gluonPropagator = gluonPropagator->clone();
}

double GapEquationSolverModule::getEpsilon2() const {
    return m_epsilon2;
}

void GapEquationSolverModule::setEpsilon2(double epsilon2) {
    m_epsilon2 = epsilon2;
    m_epsilon = sqrt(epsilon2);
}

double GapEquationSolverModule::getLambda2() const {
    return m_Lambda2;
}

void GapEquationSolverModule::setLambda2(double lambda2) {
    m_Lambda2 = lambda2;
    m_Lambda = sqrt(lambda2);
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

double GapEquationSolverModule::getTolerance() const {
    return m_tolerance;
}

void GapEquationSolverModule::setTolerance(double tolerance) {
    m_tolerance = tolerance;
}

double GapEquationSolverModule::ThetaA_func(std::vector<double> z,
        std::vector<double> parameters) {
    double p2 = parameters.at(0);
    double q2 = parameters.at(1);

    return sqrt(1 - z.at(0) * z.at(0))
            * m_gluonPropagator->evaluateG(k2_func(p2, q2, z.at(0)))
            * F_func(p2, q2, k2_func(p2, q2, z.at(0)));
}

double GapEquationSolverModule::ThetaM_func(std::vector<double> z,
        std::vector<double> parameters) {
    double p2 = parameters.at(0);
    double q2 = parameters.at(1);

    return sqrt(1 - z.at(0) * z.at(0)) * 4
            * m_gluonPropagator->evaluateG(k2_func(p2, q2, z.at(0)));
}

double GapEquationSolverModule::F_func(double p2, double q2, double k2) {
    return 4. / 3.
            * ((p2 + q2) / 2. + (p2 - q2) * (p2 - q2) / 2. / (k2 + 1.e-16) - k2)
            / (p2 + 1.e-16);
}

double GapEquationSolverModule::k2_func(double p2, double q2, double z) {
    return p2 + q2 - 2 * sqrt(p2) * sqrt(q2) * z;
}
