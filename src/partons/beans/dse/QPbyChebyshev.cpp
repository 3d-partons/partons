#include "../../../../include/partons/beans/dse/QPbyChebyshev.h"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <NumA/linear_algebra/vector/VectorD.h>

#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"

QPbyChebyshev::QPbyChebyshev(unsigned int N, double m, double mu,
        double Lambda2, double epsilon2) :
        QuarkPropagator("QPbyChebyshev", N, m, mu, Lambda2, epsilon2) {
    if (N > 0) {
        m_a = NumA::VectorD(N);
        m_b = NumA::VectorD(N);
        m_roots.assign(N, 0.);
        for (int k = 0; k < N; k++) {
            m_roots.at(k) = cos((N - k - 0.5) * PI / N);
        }
    }
}

QPbyChebyshev* QPbyChebyshev::clone() const {
    return new QPbyChebyshev(*this);
}

QPbyChebyshev::QPbyChebyshev(const QPbyChebyshev& other) :
        QuarkPropagator(other) {
    m_a = other.getCoeffsA();
    m_b = other.getCoeffsB();
    m_roots = other.getRoots();
}

QPbyChebyshev::~QPbyChebyshev() {
// TODO Auto-generated destructor stub
}

void QPbyChebyshev::setN(unsigned int n) {
    QuarkPropagator::setN(n);

    for (int k = 0; k < n; k++) {
        m_roots.at(k) = cos((n - k - 0.5) * PI / n); //TODO Move to NumA
    }
}

void QPbyChebyshev::setCoeffsAfromValueOnNodes(
        const std::vector<double>& values) {
    m_a.assign(m_N, 0.);
    double stored_T;
    for (unsigned int i = 0; i < m_N; i++) {
        for (unsigned int k = 0; k < m_N; k++) {
            stored_T = cos(i * (m_N - k - 0.5) * PI / m_N);
            m_a.at(i) += stored_T * values.at(k);
        }
        m_a.at(i) *= 2. / m_N;
    }
}

void QPbyChebyshev::setCoeffsBfromValueOnNodes(
        const std::vector<double>& values) {
    m_b.assign(m_N, 0.);
    double stored_T;
    for (unsigned int i = 0; i < m_N; i++) {
        for (unsigned int k = 0; k < m_N; k++) {
            stored_T = cos(i * (m_N - k - 0.5) * PI / m_N);
            m_b.at(i) += stored_T * values.at(k);
        }
        m_b.at(i) *= 2. / m_N;
    }
}

std::vector<double> QPbyChebyshev::evaluate(
        const std::vector<QuarkPropagator::QPFunction> & listOfFunctions,
        double p2, unsigned int j) const {

    if (!listOfFunctions.empty()) {

        bool b_SigmaA = false, b_SigmaM = false, b_A = false, b_B = false,
                b_sigmaV = false, b_sigmaS = false, b_dSigmaA = false,
                b_dSigmaM = false, b_dA = false, b_dB = false, b_dsigmaV_a =
                        false, b_dsigmaV_b = false, b_dsigmaS_a = false,
                b_dsigmaS_b = false;
        unsigned int i_SigmaA, i_SigmaM, i_A, i_B, i_sigmaV, i_sigmaS,
                i_dSigmaA, i_dSigmaM, i_dA, i_dB, i_dsigmaV_a, i_dsigmaV_b,
                i_dsigmaS_a, i_dsigmaS_b;

        std::vector<double> result;
        if (std::find(listOfFunctions.begin(), listOfFunctions.end(),
                QuarkPropagator::ALL) < listOfFunctions.end()) {
            result.assign(QuarkPropagator::ALL, 0.);
            b_SigmaA = true, b_SigmaM = true, b_A = true, b_B = true, b_sigmaV =
                    true, b_sigmaS = true, b_dSigmaA = true, b_dSigmaM = true, b_dA =
                    true, b_dB = true, b_dsigmaV_a = true, b_dsigmaV_b = true, b_dsigmaS_a =
                    true, b_dsigmaS_b = true;
            i_SigmaA = QuarkPropagator::SigmaA, i_SigmaM =
                    QuarkPropagator::SigmaM, i_A = QuarkPropagator::A, i_B =
                    QuarkPropagator::B, i_sigmaV = QuarkPropagator::sigmaV, i_sigmaS =
                    QuarkPropagator::sigmaS, i_dSigmaA =
                    QuarkPropagator::dSigmaA, i_dSigmaM =
                    QuarkPropagator::dSigmaM, i_dA = QuarkPropagator::dA, i_dB =
                    QuarkPropagator::dB, i_dsigmaV_a =
                    QuarkPropagator::dsigmaV_a, i_dsigmaV_b =
                    QuarkPropagator::dsigmaV_b, i_dsigmaS_a =
                    QuarkPropagator::dsigmaS_a, i_dsigmaS_b =
                    QuarkPropagator::dsigmaS_b;
        } else {
            result.assign(listOfFunctions.size(), 0.);
            for (unsigned int i = 0; i < listOfFunctions.size(); i++) {
                switch (listOfFunctions.at(i)) {
                case QuarkPropagator::SigmaA:
                    b_SigmaA = true;
                    i_SigmaA = i;
                    break;
                case QuarkPropagator::SigmaM:
                    b_SigmaM = true;
                    i_SigmaM = i;
                    break;
                case QuarkPropagator::A:
                    b_A = true;
                    i_A = i;
                    break;
                case QuarkPropagator::B:
                    b_B = true;
                    i_B = i;
                    break;
                case QuarkPropagator::sigmaV:
                    b_sigmaV = true;
                    i_sigmaV = i;
                    break;
                case QuarkPropagator::sigmaS:
                    b_sigmaS = true;
                    i_sigmaS = i;
                    break;
                case QuarkPropagator::dSigmaA:
                    b_dSigmaA = true;
                    i_dSigmaA = i;
                    break;
                case QuarkPropagator::dSigmaM:
                    b_dSigmaM = true;
                    i_dSigmaM = i;
                    break;
                case QuarkPropagator::dA:
                    b_dA = true;
                    i_dA = i;
                    break;
                case QuarkPropagator::dB:
                    b_dB = true;
                    i_dB = i;
                    break;
                case QuarkPropagator::dsigmaV_a:
                    b_dsigmaV_a = true;
                    i_dsigmaV_a = i;
                    break;
                case QuarkPropagator::dsigmaV_b:
                    b_dsigmaV_b = true;
                    i_dsigmaV_b = i;
                    break;
                case QuarkPropagator::dsigmaS_a:
                    b_dsigmaS_a = true;
                    i_dsigmaS_a = i;
                    break;
                case QuarkPropagator::dsigmaS_b:
                    b_dsigmaS_b = true;
                    i_dsigmaS_b = i;
                    break;
                default:
                    error(__func__, "Undefined function to evaluate!");
                    break;
                }
            }
        }

        double SigmaA, SigmaM, A, B, sigmaV, sigmaS, dSigmaA, dSigmaM, dA, dB,
                dsigmaV_a, dsigmaV_b, dsigmaS_a, dsigmaS_b;
        double A2, B2, denom, sigmaV2, sigmaS2;
        double xmu, x;
        std::vector<double> Tix(m_a.size(), 0.), Tixmu(m_a.size(), 0.);

        x = stox(p2);
        if (b_A || b_B || b_sigmaV || b_sigmaS || b_dA || b_dB || b_dsigmaV_a
                || b_dsigmaV_b || b_dsigmaS_a || b_dsigmaS_b) {
            xmu = stox(m_mu * m_mu);
        }

        if (b_SigmaA || b_SigmaM || b_A || b_B || b_sigmaV || b_sigmaS
                || b_dsigmaV_a || b_dsigmaV_b || b_dsigmaS_a || b_dsigmaS_b) {
            for (unsigned int i = 1; i < m_a.size(); i++) {
                Tix.at(i) = T(i, x);
            }
            if (b_A || b_B || b_sigmaV || b_sigmaS || b_dsigmaV_a || b_dsigmaV_b
                    || b_dsigmaS_a || b_dsigmaS_b) {
                for (unsigned int i = 1; i < m_a.size(); i++) {
                    Tixmu.at(i) = Tix.at(i) - T(i, xmu);
                }
            }
        } else {
            Tix.at(j) = T(j, x);
            if (b_dA || b_dB) {
                Tixmu.at(j) = Tix.at(j) - T(j, xmu);
            }
        }

        if (b_SigmaA) {
            SigmaA = (m_a.at(0)) / 2.;
            for (unsigned int i = 1; i < m_a.size(); i++) {
                SigmaA += (m_a.at(i)) * Tix.at(i);
            };
        }

        if (b_SigmaM) {
            SigmaM = (m_b.at(0)) / 2.;
            for (unsigned int i = 1; i < m_b.size(); i++) {
                SigmaM += (m_b.at(i)) * Tix.at(i);
            };
        }

        if (b_A || b_sigmaV || b_sigmaS || b_dsigmaV_a || b_dsigmaV_b
                || b_dsigmaS_a || b_dsigmaS_b) {
            A = 1.;
            for (unsigned int i = 1; i < m_a.size(); i++) {
                A += (m_a.at(i)) * Tixmu.at(i);
            };
        }

        if (b_B || b_sigmaV || b_sigmaS || b_dsigmaV_a || b_dsigmaV_b
                || b_dsigmaS_a || b_dsigmaS_b) {
            B = m_m;
            for (unsigned int i = 1; i < m_b.size(); i++) {
                B += (m_b.at(i)) * Tixmu.at(i);
            };
        }

        if (b_sigmaV || b_sigmaS || b_dsigmaV_a || b_dsigmaV_b || b_dsigmaS_a
                || b_dsigmaS_b) {
            A2 = A * A;
            B2 = B * B;
            denom = p2 * A2 + B2;
        }

        if (b_sigmaV || b_dsigmaV_a || b_dsigmaV_b || b_dsigmaS_a
                || b_dsigmaS_b) {
            sigmaV = A / denom;
        }

        if (b_sigmaS || b_dsigmaV_a || b_dsigmaV_b || b_dsigmaS_a
                || b_dsigmaS_b) {
            sigmaS = B / denom;
        }

        if (b_dsigmaV_a || b_dsigmaS_b) {
            sigmaV2 = sigmaV * sigmaV;
            sigmaS2 = sigmaS * sigmaS;
        }

        if (b_dSigmaA) {
            dSigmaA = Tix.at(j) - 0.5 * (j == 0);
        }

        if (b_dSigmaM) {
            dSigmaM = Tix.at(j) - 0.5 * (j == 0);
        }

        if (b_dA || b_dsigmaV_a || b_dsigmaS_a) {
            dA = Tixmu.at(j);
        }

        if (b_dB || b_dsigmaV_b || b_dsigmaS_b) {
            dB = Tixmu.at(j);
        }

        if (b_dsigmaV_a) {
            dsigmaV_a = dA * (sigmaS2 - p2 * sigmaV2);
        }

        if (b_dsigmaV_b) {
            dsigmaV_b = -2 * dB * sigmaV * sigmaS;
        }

        if (b_dsigmaS_a) {
            dsigmaS_a = -2 * p2 * dA * sigmaV * sigmaS;
        }

        if (b_dsigmaS_b) {
            dsigmaS_b = dB * (p2 * sigmaV2 - sigmaS2);
        }

        if(b_SigmaA)
            result.at(i_SigmaA) = SigmaA;

        if(b_SigmaM)
            result.at(i_SigmaM) = SigmaM;

        if(b_A)
            result.at(i_A) = A;

        if(b_B)
            result.at(i_B) = B;

        if(b_sigmaV)
            result.at(i_sigmaV) = sigmaV;

        if (b_sigmaS)
            result.at(i_sigmaS) = sigmaS;

        if(b_dSigmaA)
            result.at(i_dSigmaA) = dSigmaA;

        if(b_dSigmaM)
            result.at(i_dSigmaM) = dSigmaM;

        if(b_dA)
            result.at(i_dA) = dA;

        if(b_dB)
            result.at(i_dB) = dB;

        if(b_dsigmaV_a)
            result.at(i_dsigmaV_a) = dsigmaV_a;

        if(b_dsigmaV_b)
            result.at(i_dsigmaV_b) = dsigmaV_b;

        if(b_dsigmaS_a)
            result.at(i_dsigmaS_a) = dsigmaS_a;

        if(b_dsigmaS_b)
            result.at(i_dsigmaS_b) = dsigmaS_b;

        return result;
    } else {
        error(__func__, "No function to evaluate!");
    }

}

double QPbyChebyshev::evaluateA(double p2) const {
    double A = 1.;
    double x = stox(p2);
    double xmu = stox(m_mu * m_mu);
    for (unsigned int i = 1; i < m_a.size(); i++) {
        A += (m_a.at(i)) * (T(i, x) - T(i, xmu));
    };

    return A;
}

double QPbyChebyshev::evaluateB(double p2) const {
    double B = m_m;
    double x = stox(p2);
    double xmu = stox(m_mu * m_mu);
    for (unsigned int i = 1; i < m_b.size(); i++) {
        B += (m_b.at(i)) * (T(i, x) - T(i, xmu));
    };

    return B;
}

double QPbyChebyshev::evaluateSigmaA(double p2) const {
    double SigmaA = (m_a.at(0)) / 2.;
    double x = stox(p2);
    for (unsigned int i = 1; i < m_a.size(); i++) {
        SigmaA += (m_a.at(i)) * T(i, x);
    };
    return SigmaA;
}

double QPbyChebyshev::evaluateSigmaM(double p2) const {
    double SigmaM = (m_b.at(0)) / 2.;
    double x = stox(p2);
    for (unsigned int i = 1; i < m_b.size(); i++) {
        SigmaM += (m_b.at(i)) * T(i, x);
    };
    return SigmaM;
}

double QPbyChebyshev::differentiateA(double p2, unsigned int j) const {
    return T(j, stox(p2)) - T(j, stox(m_mu * m_mu));
}

double QPbyChebyshev::differentiateB(double p2, unsigned int j) const {
    return T(j, stox(p2)) - T(j, stox(m_mu * m_mu));
}

double QPbyChebyshev::differentiateSigmaA(double p2, unsigned int j) const {
    return T(j, stox(p2)) - 0.5 * (j == 0);
}

double QPbyChebyshev::differentiateSigmaM(double p2, unsigned int j) const {
    return T(j, stox(p2)) - 0.5 * (j == 0);
}

double QPbyChebyshev::T(unsigned int n, double x) const {
    return cos(n * acos(x)); //TODO Move to NumA
}

double QPbyChebyshev::stox(double p2) const {
    return log(p2 / (m_Lambda * m_epsilon)) / log(m_Lambda / m_epsilon);
}

double QPbyChebyshev::xtos(double x) const {
    return m_Lambda * m_epsilon * exp(x * log(m_Lambda / m_epsilon));
}

std::string QPbyChebyshev::toString() const {
    Formatter formatter;
    formatter << "Coeffs a : ";
    for (unsigned int i = 0; i < m_a.size(); i++) {
        formatter << m_a.at(i) << " , ";
    }
    formatter << "\n";
    formatter << "Coeffs b : ";
    for (unsigned int i = 0; i < m_b.size(); i++) {
        formatter << m_b.at(i) << " , ";
    }
    formatter << "\n";
    return formatter;
}
