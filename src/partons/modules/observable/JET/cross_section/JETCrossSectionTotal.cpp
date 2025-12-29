#include "../../../../../../include/partons/modules/observable/JET/cross_section/JETCrossSectionTotal.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_vegas.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_integration.h>
#include <cmath>
#include <vector>

#include "../../../../../../include/partons/beans/observable/JET/JETObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/process/JET/JETProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string JETCrossSectionTotal::JET_CROSSSECTION_TOTAL_RANGEXb =
        "JETCrossSectionTotal_rangeXb";
const std::string JETCrossSectionTotal::JET_CROSSSECTION_TOTAL_RANGET =
        "JETCrossSectionTotal_rangeT";
const std::string JETCrossSectionTotal::JET_CROSSSECTION_TOTAL_RANGEZ =
    "JETCrossSectionTotal_rangeZ";
const std::string JETCrossSectionTotal::JET_CROSSSECTION_TOTAL_RANGEQPerp2 =
    "JETCrossSectionTotal_rangeQPerp2";
const std::string JETCrossSectionTotal::JET_CROSSSECTION_TOTAL_RANGEQ2 =
        "JETCrossSectionTotal_rangeQ2";
const std::string JETCrossSectionTotal::JET_CROSSSECTION_TOTAL_RANGEPHI =
        "JETCrossSectionTotal_rangePhi";
const std::string JETCrossSectionTotal::JET_CROSSSECTION_TOTAL_RANGEY =
        "JETCrossSectionTotal_rangeY";
const std::string JETCrossSectionTotal::JET_CROSSSECTION_TOTAL_N0 =
        "JETCrossSectionTotal_N0";
const std::string JETCrossSectionTotal::JET_CROSSSECTION_TOTAL_N1 =
        "JETCrossSectionTotal_N1";

const unsigned int JETCrossSectionTotal::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new JETCrossSectionTotal("JETCrossSectionTotal"));

JETCrossSectionTotal::JETCrossSectionTotal(const std::string &className) :
        JETObservable(className), m_nI0(5000), m_nI1(5) {

    m_rangexB = std::pair<double, double>(1.E-4, 0.7);
    m_rangeT = std::pair<double, double>(-1., -0.0001);
    m_rangeZ = std::pair<double, double>(0.1, 0.9);
    m_rangeQPerp2 = std::pair<double, double>(1., 10.);
    m_rangeQ2 = std::pair<double, double>(1., 1.E3);
    m_rangePhi = std::pair<double, double>(0., 2*M_PI);
    m_rangeY = std::pair<double, double>(1.E-6, 1.);
}

JETCrossSectionTotal::JETCrossSectionTotal(const JETCrossSectionTotal& other) :
        JETObservable(other), m_nI0(other.m_nI0), m_nI1(
                other.m_nI1) {

    m_rangexB = other.m_rangexB;
    m_rangeT = other.m_rangeT;
    m_rangeZ = other.m_rangeZ;
    m_rangeQPerp2 = other.m_rangeQPerp2;
    m_rangeQ2 = other.m_rangeQ2;
    m_rangePhi = other.m_rangePhi;
    m_rangeY = other.m_rangeY;
}

JETCrossSectionTotal::~JETCrossSectionTotal() {
}

JETCrossSectionTotal* JETCrossSectionTotal::clone() const {
    return new JETCrossSectionTotal(*this);
}

void JETCrossSectionTotal::configure(const ElemUtils::Parameters &parameters) {

    //run for base
    JETObservable::configure(parameters);

    //check
    if (parameters.isAvailable(JET_CROSSSECTION_TOTAL_RANGEXb)) {
        setRangexB(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(JET_CROSSSECTION_TOTAL_RANGET)) {
        setRangeT(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(JET_CROSSSECTION_TOTAL_RANGEZ)) {
        setRangeZ(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(JET_CROSSSECTION_TOTAL_RANGEQPerp2)) {
        setRangeQPerp2(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(JET_CROSSSECTION_TOTAL_RANGEQ2)) {
        setRangeQ2(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(JET_CROSSSECTION_TOTAL_RANGEPHI)) {
        setRangePhi(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(JET_CROSSSECTION_TOTAL_RANGEY)) {
        setRangeY(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(JET_CROSSSECTION_TOTAL_N0)) {
        setNI0(
                ElemUtils::GenericType(
                        parameters.getLastAvailable().getString()).toUInt());
    }

    if (parameters.isAvailable(JET_CROSSSECTION_TOTAL_N1)) {
        setNI1(
                ElemUtils::GenericType(
                        parameters.getLastAvailable().getString()).toUInt());
    }
}

double JETCrossSectionTotal::JETCrossSectionTotalFunctionA(double* kin,
        size_t dim, void* par) {

    //parameters
    JETCrossSectionTotalParameters* params =
            static_cast<JETCrossSectionTotalParameters*>(par);

    const double y = exp(kin[0]);
    const double t = -1 * exp(kin[1]);
    const double z = kin[2];
    const double qPerp2 = exp(kin[3]);
    const double Q2 = exp(kin[4]);

    const double E = params->m_E;

    //jacobian
    const double jacobian = -1 * y * t * qPerp2 * Q2;

    //check kinematic range
    const double xB = Q2 / (2 * y * Constant::PROTON_MASS * E);
    const double epsilon = 2 * xB * Constant::PROTON_MASS / sqrt(Q2);
    const double eps2 = epsilon * epsilon;
    const double epsroot = sqrt(1 + eps2);
    const double tfactor = -Q2 / (4 * xB * (1 - xB) + eps2);
    const double tmin = tfactor * (2 * (1 - xB) * (1 - epsroot) + eps2);
    const double tmax = tfactor * (2 * (1 - xB) * (1 + epsroot) + eps2);
    const double xBmin = 2 * Q2 * E / Constant::PROTON_MASS / (4 * E * E - Q2);

    if (xB < xBmin || xB > 1.)
        return 0.;

    if (t > tmin || t < tmax)
        return 0.;

    if (xB < params->m_xBCut.first || xB > params->m_xBCut.second)
        return 0.;

    //evaluate
    params->m_y = y;
    params->m_t = t;
    params->m_z = z;
    params->m_qPerp2 = qPerp2;
    params->m_Q2 = Q2;

    gsl_integration_workspace* w
            = gsl_integration_workspace_alloc (1000);

    double result, error;

    gsl_function F;
    F.function = &JETCrossSectionTotalFunctionB;
    F.params = par;

    gsl_integration_qag(&F, params->m_phiCut.first, params->m_phiCut.second, 0, 1.E-1, 1000, 6, w, &result, &error);

    gsl_integration_workspace_free (w);

    return result * jacobian;
}

double JETCrossSectionTotal::JETCrossSectionTotalFunctionB(double phi, void* par){

    //parameters
    JETCrossSectionTotalParameters* params =
            static_cast<JETCrossSectionTotalParameters*>(par);

    const double& y = params->m_y;
    const double& t = params->m_t;
    const double& z = params->m_z;
    const double& qPerp2 = params->m_qPerp2;
    const double& Q2 = params->m_Q2;
    const double& E = params->m_E;
    const JetType::Type& jetType = params->m_jetType;

    double xB = Q2 / (2 * y * Constant::PROTON_MASS * E);
    double jacobian = xB / y;

    double result = jacobian * params->m_pJETCrossSectionTotal->getProcessModule()->compute(1, -1, NumA::Vector3D(),
        JETObservableKinematic(xB, t, z, qPerp2, Q2, E, phi, jetType),
        params->m_gpdType).getValue().makeSameUnitAs(PhysicalUnit::NB).getValue();

    if(std::isfinite(result)){
        return result;
    }else{
        std::cout << "Result is unfinite, return 0" << std::endl;
        return 0.;
    }
}

PhysicalType<double> JETCrossSectionTotal::computeObservable(
        const JETObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //disable
    gsl_set_error_handler_off();

    //result and error
    double res, err;

    //parameters
    JETCrossSectionTotalParameters params;

    params.m_pJETCrossSectionTotal = this;
    params.m_E = kinematic.getE().makeSameUnitAs(PhysicalUnit::GEV).getValue();
    params.m_jetType = kinematic.getJetType();

    //Q2 max
    double maxQ2 = 2 * Constant::PROTON_MASS * kinematic.getE().makeSameUnitAs(PhysicalUnit::GEV).getValue();

    if (m_rangeQ2.second > maxQ2) {
        warn(__func__,
                ElemUtils::Formatter() << "Upper limit of Q2 "
                        << m_rangeQ2.second
                        << " grater than maximum value  allowed for this energy, "
                        << maxQ2 << ", the later used instead");
    }

    //ranges
    //y, Q2, t
    double xl[5] = { log(m_rangeY.first), log(-1 * m_rangeT.second), m_rangeZ.first, log(m_rangeQPerp2.first),
        log(m_rangeQ2.first) };
    double xu[5] = { log(m_rangeY.second), log(-1 * m_rangeT.first), m_rangeZ.second, log(m_rangeQPerp2.second),
        log((m_rangeQ2.second > maxQ2) ? (maxQ2) : (m_rangeQ2.second)) };

    //cuts
    params.m_xBCut = m_rangexB;
    params.m_phiCut = m_rangePhi;

    //random
    const gsl_rng_type* T = gsl_rng_default;
    gsl_rng* r = gsl_rng_alloc(T);

    //function
    gsl_monte_function G = { &JETCrossSectionTotalFunctionA, 5, &params };

    //run
    gsl_monte_vegas_state* s = gsl_monte_vegas_alloc(5);

    for (size_t i = 0; i < m_nI1; i++) {

        gsl_monte_vegas_integrate(&G, xl, xu, 5, m_nI0, r, s, &res, &err);

        info(__func__,
                ElemUtils::Formatter() << "Intermediate result: cycle: " << i << " |chi2 - 1|: " << fabs(gsl_monte_vegas_chisq(s) - 1.)
                        << " result: " << res << " error: " << err << " [nb]");
    }

    //free
    gsl_monte_vegas_free(s);
    gsl_rng_free(r);

    //return
    return PhysicalType<double>(res, PhysicalUnit::NB);
}

void JETCrossSectionTotal::printChangeOfRange(const std::string& func,
        const std::string& name, const std::pair<double, double>& oldValues,
        const std::pair<double, double>& newValues) const {
    info(func,
            ElemUtils::Formatter() << "Integration range of " << name
                    << " changed from (" << oldValues.first << ", "
                    << oldValues.second << ") to (" << newValues.first << ", "
                    << newValues.second << ")");
}

std::pair<double, double> JETCrossSectionTotal::parseRange(
        const std::string& str) const {

    // split string
    std::vector<std::string> resultStr = ElemUtils::StringUtils::split(str,
            '|');

    //check size
    if (resultStr.size() != 2) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Unable to parse " << str
                        << " into range");
    }

    //result
    return std::pair<double, double>(
            ElemUtils::GenericType(resultStr.at(0)).toDouble(),
            ElemUtils::GenericType(resultStr.at(1)).toDouble());

}

size_t JETCrossSectionTotal::getNI0() const {
    return m_nI0;
}

void JETCrossSectionTotal::setNI0(size_t nI0) {

    info(__func__,
            ElemUtils::Formatter()
                    << "Number of iteration in single cycle changed from "
                    << m_nI0 << " to " << nI0);
    m_nI0 = nI0;
}

size_t JETCrossSectionTotal::getNI1() const {
    return m_nI1;
}

void JETCrossSectionTotal::setNI1(size_t nI1) {

    info(__func__,
            ElemUtils::Formatter() << "Number of cycles changed from " << m_nI1
                    << " to " << nI1);
    m_nI1 = nI1;
}

const std::pair<double, double>& JETCrossSectionTotal::getRangexB() const {
    return m_rangexB;
}

void JETCrossSectionTotal::setRangexB(
            const std::pair<double, double>& rangexB) {

    printChangeOfRange(__func__, "xB", m_rangexB, rangexB);
    m_rangexB = rangexB;
}

const std::pair<double, double>& JETCrossSectionTotal::getRangeT() const {
    return m_rangeT;
}

void JETCrossSectionTotal::setRangeT(const std::pair<double, double>& rangeT) {

    printChangeOfRange(__func__, "t", m_rangeT, rangeT);
    m_rangeT = rangeT;
}

const std::pair<double, double>& JETCrossSectionTotal::getRangeZ() const {
    return m_rangeZ;
}

void JETCrossSectionTotal::setRangeZ(const std::pair<double, double>& rangeZ) {

    printChangeOfRange(__func__, "z", m_rangeZ, rangeZ);
    m_rangeZ = rangeZ;
}

const std::pair<double, double>& JETCrossSectionTotal::getRangeQPerp2() const {
    return m_rangeQPerp2;
}

void JETCrossSectionTotal::setRangeQPerp2(const std::pair<double, double>& rangeQPerp2) {

    printChangeOfRange(__func__, "qPerp2", m_rangeQPerp2, rangeQPerp2);
    m_rangeQPerp2 = rangeQPerp2;
}

const std::pair<double, double>& JETCrossSectionTotal::getRangeQ2() const {
    return m_rangeQ2;
}

void JETCrossSectionTotal::setRangeQ2(
        const std::pair<double, double>& rangeQ2) {

    printChangeOfRange(__func__, "Q2", m_rangeQ2, rangeQ2);
    m_rangeQ2 = rangeQ2;
}

const std::pair<double, double>& JETCrossSectionTotal::getRangePhi() const {
    return m_rangePhi;
}

void JETCrossSectionTotal::setRangePhi(const std::pair<double, double>& rangePhi) {

    printChangeOfRange(__func__, "phi", m_rangePhi, rangePhi);
    m_rangePhi = rangePhi;
}

const std::pair<double, double>& JETCrossSectionTotal::getRangeY() const {
    return m_rangeY;
}

void JETCrossSectionTotal::setRangeY(const std::pair<double, double>& rangeY) {

    printChangeOfRange(__func__, "y", m_rangeY, rangeY);
    m_rangeY = rangeY;
}

} /* namespace PARTONS */

