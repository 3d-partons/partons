#include "../../../../include/partons/modules/radon_inverse/RandBFConstPW.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <NumA/linear_algebra/eigen/LinAlgUtils.h>
#include <NumA/linear_algebra/vector/VectorD.h>
#include <NumA/utils/Interval.h>

#include "../../../../include/partons/beans/double_distribution/DDGauge.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/utils/radon/RadonTools.h"

// Initialise [class]::classId with a unique name.
const unsigned int RandBFConstPW::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new RandBFConstPW("RandBFInverse"));

RandBFConstPW::RandBFConstPW(const std::string &className) :
        RadonInverseModule(className), m_valence(true), m_alphaEven(true), m_triangular(
                true), m_step(0) {
}

RandBFConstPW::~RandBFConstPW() {
}

RandBFConstPW::RandBFConstPW(const RandBFConstPW& other) :
        RadonInverseModule(other) {
    m_valence = other.m_valence;
    m_alphaEven = other.m_alphaEven;
    m_triangular = other.m_triangular;
    m_step = other.m_step;
    m_nodes = other.m_nodes;
    m_ddResult = other.m_ddResult;
    m_indicesUsed = other.m_indicesUsed;
    m_indicesSym = other.m_indicesSym;
    m_indicesFixed = other.m_indicesFixed;
}

RandBFConstPW* RandBFConstPW::clone() const {
    return new RandBFConstPW(*this);
}

void RandBFConstPW::configure(const ElemUtils::Parameters& parameters) {
}

void RandBFConstPW::initModule() {
    //TODO implement
    RadonInverseModule::initModule();
}

void RandBFConstPW::isModuleWellConfigured() {
    //TODO implement
    RadonInverseModule::isModuleWellConfigured();
}

void RandBFConstPW::buildMatrix(size_t maxiter) {
    //TODO random algorithm
    if (maxiter == 0)
        maxiter = m_n * 10;
    m_rank = 0;
    m_radonMatrix = NumA::MatrixD();
    m_gpdNodes = std::vector<std::pair<double, double> >();
    size_t it = 0;
    NumA::VectorD currentLine;
    double x, y, xi;
    srand(time(NULL));
    while (m_rank < m_n and it < maxiter) {
        if (isValence()) {
            x = (rand() / (double) RAND_MAX);
        } else {
            x = (rand() / (double) RAND_MAX) * 2 - 1.;
        }
        y = (rand() / (double) RAND_MAX) * 2 - 1.;
        xi = x * y;
        currentLine = NumA::VectorD(m_n);
        size_t i, j;
        for (size_t k = 0; k < m_n; k++) {
            i = static_cast<long int>(m_indicesUsed[k].first);
            j = static_cast<long int>(m_indicesUsed[k].second);
            currentLine[k] = GPDOfMeshElement(x, xi, i, j);
            if (isAlphaEven() and (i + j != m_N - 1)) {
                currentLine[k] += GPDOfMeshElement(x, xi, m_N - j - 1,
                        m_N - i - 1);
            }
        }
        m_radonMatrix.appendLine(currentLine);
        m_rank = NumA::LinAlgUtils::rank(
                m_radonMatrix.transpose() * m_radonMatrix);
        m_gpdNodes.push_back(std::make_pair(x, xi));
        it++;
    }
    m_m = m_gpdNodes.size();
    m_rank = NumA::LinAlgUtils::rank(m_radonMatrix);
}

bool RandBFConstPW::isAlphaEven() const {
    return m_alphaEven;
}

void RandBFConstPW::setAlphaEven(bool alphaEven) {
    if (m_alphaEven != alphaEven) {
        m_alphaEven = alphaEven;
        buildIndices();
    }
}

bool RandBFConstPW::isValence() const {
    return m_valence;
}

void RandBFConstPW::setValence(bool valence) {
    if (m_valence != valence) {
        m_valence = valence;
        buildIndices();
    }
}

bool RandBFConstPW::isTriangular() const {
    return m_triangular;
}

void RandBFConstPW::setTriangular(bool triangular) {
    if (m_triangular != triangular) {
        m_triangular = triangular;
        info(__func__,
                "The nature of the mesh around beta=0 was changed, you should rebuild the matrix.");
    }
}

void RandBFConstPW::buildIndices() {
    for (size_t i = 0; i < m_N; i++) {
        for (size_t j = 0; j < m_N; i++) {
            if ((i + 1 > j) or !isValence()) {
                if ((i + 1 + j + 1 > m_N) or !isAlphaEven()) {
                    m_indicesUsed.push_back(std::make_pair(i, j));
                } else {
                    m_indicesSym.push_back(std::make_pair(i, j));
                }
            } else {
                m_indicesFixed.push_back(std::make_pair(i, j));
            }
        }
    }
    m_n = m_indicesUsed.size();
    m_ddVector.assign(m_n);
}

void RandBFConstPW::buildMesh() {
    m_nodes = NumA::Interval<double>::computeNodes(-DD_DOMAIN_HALF_EDGE,
            DD_DOMAIN_HALF_EDGE, m_N + 1);
    m_step = 2 * DD_DOMAIN_HALF_EDGE / m_N;
    buildIndices();
}

void RandBFConstPW::solve() {
    RadonInverseModule::solve();
    m_ddResult.assign(m_N, m_N);
    size_t i, j;
    for (size_t k = 0; k < m_n; k++) {
        i = static_cast<long int>(m_indicesUsed[k].first);
        j = static_cast<long int>(m_indicesUsed[k].second);
        m_ddResult.at(i, j) = m_ddVector[k];
    }
    for (size_t k = 0; k < m_indicesSym.size(); k++) {
        i = static_cast<long int>(m_indicesSym[k].first);
        j = static_cast<long int>(m_indicesSym[k].second);
        m_ddResult.at(i, j) = m_ddResult.at(m_N - j - 1, m_N - i - 1);
    }
}

double RandBFConstPW::GPDOfMeshElement(double x, double xi, size_t i,
        size_t j) {
    double result;
    if (isTriangular() and isValence() and (i == j)) {
        result = RadonTools::GPDOfLowerTriangleCell(x, xi, m_nodes[i],
                m_nodes[i + 1], m_nodes[j], m_nodes[j + 1]);
    } else {
        result = RadonTools::GPDOfSquareCell(x, xi, m_nodes[i], m_nodes[i + 1],
                m_nodes[j], m_nodes[j + 1]);
    }
    if (!isGaugeInVector()) {
        switch (getGauge()) {
        case DDGauge::BMKS:
            result *= x;
            break;
        case DDGauge::Pobylitsa:
            result *= 1. - x;
            break;
        default:
            break;
        }
    }
    return result;
}

double RandBFConstPW::computeDD(double beta, double alpha) {
    double u = (beta + alpha) * DD_DOMAIN_HALF_EDGE;
    double v = (-beta + alpha) * DD_DOMAIN_HALF_EDGE;
    size_t i = floor((u - m_nodes[0]) / m_step);
    size_t j = floor((v - m_nodes[0]) / m_step);
    return m_ddResult.at(i, j);
}

double RandBFConstPW::computeGPD(double x, double xi) {
    NumA::VectorD gpdMesh;
    gpdMesh = NumA::VectorD(m_n);
    size_t i, j;
    for (size_t k = 0; k < m_n; k++) {
        i = static_cast<long int>(m_indicesUsed[k].first);
        j = static_cast<long int>(m_indicesUsed[k].second);
        gpdMesh[k] = GPDOfMeshElement(x, xi, i, j);
        if (isAlphaEven() and (i + j != m_N - 1)) {
            gpdMesh[k] += GPDOfMeshElement(x, xi, m_N - j - 1, m_N - i - 1);
        }
    }
    return gpdMesh * m_ddVector;
}

