#include "../../../../include/partons/modules/radon_inverse/RandomRadonConstPW.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/linear_algebra/LinAlgUtils.h>
#include <NumA/linear_algebra/vector/VectorD.h>
#include <NumA/utils/Interval.h>

#include "../../../../include/partons/beans/double_distribution/DDGauge.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/utils/radon/RadonTools.h"

// Initialise [class]::classId with a unique name.
const unsigned int RandomRadonConstPW::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new RandomRadonConstPW("RandomRadonConstPW"));

const std::string RandomRadonConstPW::PARAMETER_NAME_QUARK_GPD = "quark_gpd";
const std::string RandomRadonConstPW::PARAMETER_NAME_ALPHA_EVEN = "alpha_even";
const std::string RandomRadonConstPW::PARAMETER_NAME_TRIANGULAR = "triangular";

RandomRadonConstPW::RandomRadonConstPW(const std::string &className) :
        RadonInverseModule(className), m_quarkGPD(true), m_alphaEven(true), m_triangular(
                true), m_step(0) {
}

RandomRadonConstPW::~RandomRadonConstPW() {
}

RandomRadonConstPW::RandomRadonConstPW(const RandomRadonConstPW& other) :
        RadonInverseModule(other) {
    m_quarkGPD = other.m_quarkGPD;
    m_alphaEven = other.m_alphaEven;
    m_triangular = other.m_triangular;
    m_step = other.m_step;
    m_nodes = other.m_nodes;
    m_ddResult = other.m_ddResult;
    m_indicesUsed = other.m_indicesUsed;
    m_indicesSym = other.m_indicesSym;
    m_indicesFixed = other.m_indicesFixed;
}

RandomRadonConstPW* RandomRadonConstPW::clone() const {
    return new RandomRadonConstPW(*this);
}

void RandomRadonConstPW::configure(const ElemUtils::Parameters& parameters) {
    RadonInverseModule::configure(parameters);

    if (parameters.isAvailable(RandomRadonConstPW::PARAMETER_NAME_QUARK_GPD)) {
        setQuarkGPD(parameters.getLastAvailable().toBoolean());

        info(__func__,
                ElemUtils::Formatter()
                        << RandomRadonConstPW::PARAMETER_NAME_QUARK_GPD
                        << " configured with value = " << isQuarkGPD() << ".");
    }
    if (parameters.isAvailable(RandomRadonConstPW::PARAMETER_NAME_ALPHA_EVEN)) {
        setAlphaEven(parameters.getLastAvailable().toBoolean());

        info(__func__,
                ElemUtils::Formatter()
                        << RandomRadonConstPW::PARAMETER_NAME_ALPHA_EVEN
                        << " configured with value = " << isAlphaEven() << ".");
    }
    if (parameters.isAvailable(RandomRadonConstPW::PARAMETER_NAME_TRIANGULAR)) {
        setTriangular(parameters.getLastAvailable().toBoolean());

        info(__func__,
                ElemUtils::Formatter()
                        << RandomRadonConstPW::PARAMETER_NAME_TRIANGULAR
                        << " configured with value = " << isTriangular()
                        << ".");
    }
}

void RandomRadonConstPW::initModule() {
    //TODO implement
    RadonInverseModule::initModule();
}

void RandomRadonConstPW::isModuleWellConfigured() {
    //TODO implement
    RadonInverseModule::isModuleWellConfigured();
}

void RandomRadonConstPW::buildMatrix(size_t rows) {
    info(__func__, "Building the Radon Matrix...");

    if (rows <= 0)
        m_m = m_n * 4;
    else
        m_m = rows;
    m_rank = 0;
    m_radonMatrix = NumA::MatrixD(m_m, m_n);
    m_gpdNodes = std::vector<std::pair<double, double> >();
    NumA::VectorD currentLine;
    double x, y, xi;
    srand(time(NULL));
    for (size_t it = 0; it < m_m; it++) {
        if (isQuarkGPD()) {
            x = (rand() / (double) RAND_MAX);
        } else {
            x = (rand() / (double) RAND_MAX) * 2 - 1.;
        }
        y = (rand() / (double) RAND_MAX) * 2 - 1.;
        xi = x * y;
        size_t i, j;
        for (size_t k = 0; k < m_n; k++) {
            i = static_cast<long int>(m_indicesUsed[k].first);
            j = static_cast<long int>(m_indicesUsed[k].second);
            m_radonMatrix.at(it, k) = GPDOfMeshElement(x, xi, i, j);
            if (isAlphaEven() and (i + j != m_N - 1)) {
                m_radonMatrix.at(it, k) += GPDOfMeshElement(x, xi, m_N - j - 1,
                        m_N - i - 1);
            }
        }
//        m_rank = NumA::LinAlgUtils::rank(
//                m_radonMatrix.transpose() * m_radonMatrix);
        m_gpdNodes.push_back(std::make_pair(x, xi));
    }
//    m_m = m_gpdNodes.size();
    m_rank = NumA::LinAlgUtils::rank(m_radonMatrix);
    m_matrixBuilt = true;
    info(__func__,
            ElemUtils::Formatter() << "Radon Matrix built with " << m_m
                    << " rows and " << m_n << " columns. The rank is " << m_rank
                    << ".");
}

bool RandomRadonConstPW::isAlphaEven() const {
    return m_alphaEven;
}

void RandomRadonConstPW::setAlphaEven(bool alphaEven) {
    if (m_alphaEven != alphaEven) {
        m_alphaEven = alphaEven;
        buildIndices();
        m_matrixBuilt = false;
    }
}

bool RandomRadonConstPW::isQuarkGPD() const {
    return m_quarkGPD;
}

void RandomRadonConstPW::setQuarkGPD(bool quarkGPD) {
    if (m_quarkGPD != quarkGPD) {
        m_quarkGPD = quarkGPD;
        buildIndices();
        m_matrixBuilt = false;
    }
}

bool RandomRadonConstPW::isTriangular() const {
    return m_triangular;
}

void RandomRadonConstPW::setTriangular(bool triangular) {
    if (m_triangular != triangular) {
        m_triangular = triangular;
        m_matrixBuilt = false;
//        info(__func__,
//                "The nature of the mesh around beta=0 was changed, you should rebuild the matrix.");
    }
}

void RandomRadonConstPW::buildIndices() {
    m_indicesUsed.clear();
    m_indicesSym.clear();
    m_indicesFixed.clear();
    for (size_t i = 0; i < m_N; i++) {
        for (size_t j = 0; j < m_N; j++) {
            if ((i + 1 > j) or !isQuarkGPD()) {
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

void RandomRadonConstPW::buildMesh() {
    m_nodes = NumA::Interval<double>::computeNodes(-DD_DOMAIN_HALF_EDGE,
            DD_DOMAIN_HALF_EDGE, m_N + 1);
    m_step = 2 * DD_DOMAIN_HALF_EDGE / m_N;
    buildIndices();
}

void RandomRadonConstPW::solve() {
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

double RandomRadonConstPW::GPDOfMeshElement(double x, double xi, size_t i,
        size_t j) {
    double result;
    if (isTriangular() and isQuarkGPD() and (i == j)) {
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

double RandomRadonConstPW::computeDD(double beta, double alpha) {
    double u = (beta + alpha) * DD_DOMAIN_HALF_EDGE;
    double v = (-beta + alpha) * DD_DOMAIN_HALF_EDGE;
    size_t i = floor((u - m_nodes[0]) / m_step);
    size_t j = floor((v - m_nodes[0]) / m_step);
    return m_ddResult.at(i, j);
}

double RandomRadonConstPW::computeGPD(double x, double xi) {
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

