#include "../../../../include/partons/beans/dse/QPbyChebyshev.h"

#include <NumA/linear_algebra/vector/VectorD.h>
#include <algorithm>
#include <cmath>
#include <iterator>

#include "../../../../include/partons/FundamentalPhysicalConstants.h"

QPbyChebyshev::QPbyChebyshev(unsigned int N, double m, double mu,
        double Lambda2, double epsilon2) :
        QuarkPropagator("QPbyChebyshev", N, m, mu, Lambda2, epsilon2), m_cheb(N) {
    m_roots = m_cheb.getRoots();
}

QPbyChebyshev* QPbyChebyshev::clone() const {
    return new QPbyChebyshev(*this);
}

QPbyChebyshev::QPbyChebyshev(const QPbyChebyshev& other) :
        QuarkPropagator(other) {
    m_cheb = other.m_cheb;
}

QPbyChebyshev::~QPbyChebyshev() {
    // TODO Auto-generated destructor stub
}

std::vector<double> QPbyChebyshev::evaluate(
        const std::vector<QuarkPropagator::QPFunction> & listOfFunctions,
        double p2, unsigned int j) const {

    if (!listOfFunctions.empty()) {

        bool b_A = false, b_B = false, b_M = false, b_sigmaV = false, b_sigmaS =
                false;
        unsigned int i_A, i_B, i_M, i_sigmaV, i_sigmaS;

        std::vector<double> result;
        if (std::find(listOfFunctions.begin(), listOfFunctions.end(),
                QuarkPropagator::ALL) < listOfFunctions.end()) {
            result.assign(QuarkPropagator::ALL, 0.);
            b_A = true, b_B = true, b_M = true, b_sigmaV = true, b_sigmaS = true;
            i_A = QuarkPropagator::A, i_B = QuarkPropagator::B, i_M =
                    QuarkPropagator::M, i_sigmaV = QuarkPropagator::sigmaV, i_sigmaS =
                    QuarkPropagator::sigmaS;
        } else {
            result.assign(listOfFunctions.size(), 0.);
            for (unsigned int i = 0; i < listOfFunctions.size(); i++) {
                switch (listOfFunctions.at(i)) {
                case QuarkPropagator::A:
                    b_A = true;
                    i_A = i;
                    break;
                case QuarkPropagator::B:
                    b_B = true;
                    i_B = i;
                    break;
                case QuarkPropagator::M:
                    b_M = true;
                    i_M = i;
                    break;
                case QuarkPropagator::sigmaV:
                    b_sigmaV = true;
                    i_sigmaV = i;
                    break;
                case QuarkPropagator::sigmaS:
                    b_sigmaS = true;
                    i_sigmaS = i;
                    break;
                default:
                    warn(__func__, "Undefined function to evaluate!");
                    break;
                }
            }
        }

        double A, B, M, sigmaV, sigmaS;
        double A2, B2, denom, sigmaV2, sigmaS2;
        double x = stox(p2);
        NumA::VectorD coeffsToValue = m_cheb.getCoeffsToValueVector(x);

        if (b_A || b_M || b_sigmaV || b_sigmaS) {
            A = coeffsToValue * m_a;
        }

        if (b_B || b_M || b_sigmaV || b_sigmaS) {
            B = coeffsToValue * m_b;
        }

        if (b_M) {
            M = B / A;
        }

        if (b_sigmaV || b_sigmaS) {
            A2 = A * A;
            B2 = B * B;
            denom = p2 * A2 + B2;
        }

        if (b_sigmaV) {
            sigmaV = A / denom;
        }

        if (b_sigmaS) {
            sigmaS = B / denom;
        }

        if (b_A)
            result.at(i_A) = A;

        if (b_B)
            result.at(i_B) = B;

        if (b_M)
            result.at(i_M) = M;

        if (b_sigmaV)
            result.at(i_sigmaV) = sigmaV;

        if (b_sigmaS)
            result.at(i_sigmaS) = sigmaS;

        return result;
    } else {
        return std::vector<double>();
        warn(__func__, "No function to evaluate!");
    }

}

double QPbyChebyshev::evaluateA(double p2) const {
    double x = stox(p2);
    return m_cheb.getCoeffsToValueVector(x) * m_a;
}

double QPbyChebyshev::evaluateB(double p2) const {
    double x = stox(p2);
    return m_cheb.getCoeffsToValueVector(x) * m_b;
}

std::string QPbyChebyshev::toString() const {
    return QuarkPropagator::toString();
}

void QPbyChebyshev::updateA() {
    m_A = m_cheb.getCoeffsToValuesMatrix() * m_a;
}

void QPbyChebyshev::updateB() {
    m_B = m_cheb.getCoeffsToValuesMatrix() * m_b;
}

void QPbyChebyshev::updateCoeffsA() {
    m_a = m_cheb.getValuesToCoeffsMatrix() * m_A;
}

void QPbyChebyshev::updateCoeffsB() {
    m_b = m_cheb.getValuesToCoeffsMatrix() * m_B;
}

void QPbyChebyshev::updateInterpolation() {
    m_cheb.setN(m_N);
    m_roots = m_cheb.getRoots();
}

NumA::VectorD QPbyChebyshev::getInterpolationVector(double x) const {
    return m_cheb.getValuesToValueVector(x);
}

NumA::MatrixD QPbyChebyshev::getInterpolationMatrix(
        const NumA::VectorD& points) const {
    return m_cheb.getValuesToValuesMatrix(points);
}
