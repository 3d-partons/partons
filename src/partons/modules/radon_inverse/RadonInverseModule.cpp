#include "../../../../include/partons/modules/radon_inverse/RadonInverseModule.h"

#include <cmath>
#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/multi_dimension/FunctionTypeMD.h>

const double RadonInverseModule::DD_DOMAIN_HALF_EDGE = 1. / sqrt(2.);

RadonInverseModule::RadonInverseModule(const std::string &className) :
        ModuleObject(className), m_N(0), m_rank(0), m_m(0), m_n(0), m_pGPDFunction(
                0), m_gauge(DDGauge::Pobylitsa), m_gaugeInVector(true), m_tolerance(
                1.e-16), m_maxiter(0) {
}

RadonInverseModule::~RadonInverseModule() {
}

RadonInverseModule::RadonInverseModule(const RadonInverseModule& other) :
        ModuleObject(other) {
    m_N = other.m_N;
    m_rank = other.m_rank;
    m_m = other.m_m;
    m_n = other.m_n;
    m_pGPDFunction = other.m_pGPDFunction;
    m_gauge = other.m_gauge;
    m_gaugeInVector = other.m_gaugeInVector;
    m_radonMatrix = other.m_radonMatrix;
    m_gpdVector = other.m_gpdVector;
    m_ddVector = other.m_ddVector;
    m_gpdNodes = other.m_gpdNodes;
    m_solver = other.m_solver;
    m_tolerance = other.m_tolerance;
    m_maxiter = other.m_maxiter;
}

void RadonInverseModule::initModule() {
    //TODO implement
}

void RadonInverseModule::configure(const ElemUtils::Parameters& parameters) {
}

const DDGauge& RadonInverseModule::getGauge() const {
    return m_gauge;
}

void RadonInverseModule::setGauge(const DDGauge& gauge) {
    m_gauge = gauge;
}

bool RadonInverseModule::isGaugeInVector() const {
    return m_gaugeInVector;
}

void RadonInverseModule::setGaugeInVector(bool gaugeInVector) {
    m_gaugeInVector = gaugeInVector;
}

void RadonInverseModule::isModuleWellConfigured() {
    //TODO implement
}

NumA::FunctionTypeMD* RadonInverseModule::getGPDFunction() const {
    return m_pGPDFunction;
}

void RadonInverseModule::setGPDFunction(NumA::FunctionTypeMD* pGPDFunction) {
    m_pGPDFunction = pGPDFunction;
}

void RadonInverseModule::buildGPDVector() {
    m_gpdVector = NumA::VectorD(m_m);
    if (m_pGPDFunction != 0) {
        NumA::VectorD x_xi(2);
        double x, xi;
        for (unsigned int k = 0; k < m_m; k++) {
            x = m_gpdNodes[k].first;
            xi = m_gpdNodes[k].second;
            x_xi[0] = x;
            x_xi[1] = xi;
            m_gpdVector[k] = (*m_pGPDFunction)(x_xi);
            if (isGaugeInVector()) {
                switch (getGauge()) {
                case DDGauge::BMKS:
                    m_gpdVector[k] /= x;
                    break;
                case DDGauge::Pobylitsa:
                    m_gpdVector[k] /= (1. - x);
                    break;
                default:
                    break;
                }
            }
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "GPD Function not defined!");
    }
}

void RadonInverseModule::buildGPDVector(NumA::FunctionTypeMD* pGPDFunction) {
    setGPDFunction(pGPDFunction);
    buildGPDVector();
}

void RadonInverseModule::buildSystem() {
    buildMatrix();
    buildGPDVector();
}

void RadonInverseModule::buildSystem(NumA::FunctionTypeMD* pGPDFunction) {
    setGPDFunction(pGPDFunction);
    buildSystem();
}

void RadonInverseModule::solve() {
    ElemUtils::Formatter formatter;

    // Initialize solver
    m_solver.setOutputStream(formatter);
    m_solver.setToleranceA(m_tolerance);
    m_solver.setToleranceB(m_tolerance);
    if (m_maxiter <= 0)
        m_maxiter = 4 * m_n;
    m_solver.setMaximumNumberOfIterations(m_maxiter);
    m_solver.setMatrix(m_radonMatrix);

    // Solve
    m_solver.solve(m_m, m_n, m_gpdVector, m_ddVector);

    debug(__func__, formatter);
}

size_t RadonInverseModule::getN() const {
    return m_N;
}

void RadonInverseModule::setN(size_t N) {
    m_N = N;
    buildMesh();
    m_gpdNodes.reserve(5 * m_n);
}

void RadonInverseModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    ModuleObject::prepareSubModules(subModulesData);
}

size_t RadonInverseModule::getMaxiter() const {
    return m_maxiter;
}

void RadonInverseModule::setMaxiter(size_t maxiter) {
    m_maxiter = maxiter;
}

double RadonInverseModule::getTolerance() const {
    return m_tolerance;
}

void RadonInverseModule::setTolerance(double tolerance) {
    m_tolerance = tolerance;
}
