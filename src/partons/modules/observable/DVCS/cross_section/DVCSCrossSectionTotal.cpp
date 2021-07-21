#include "../../../../../../include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionTotal.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_vegas.h>
#include <gsl/gsl_rng.h>
#include <cmath>
#include <vector>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string DVCSCrossSectionTotal::DVCS_CROSSSECTION_TOTAL_RANGEXb =
        "DVCSCrossSectionTotal_rangeXb";
const std::string DVCSCrossSectionTotal::DVCS_CROSSSECTION_TOTAL_RANGET =
        "DVCSCrossSectionTotal_rangeT";
const std::string DVCSCrossSectionTotal::DVCS_CROSSSECTION_TOTAL_RANGEQ2 =
        "DVCSCrossSectionTotal_rangeQ2";
const std::string DVCSCrossSectionTotal::DVCS_CROSSSECTION_TOTAL_RANGEY =
        "DVCSCrossSectionTotal_rangeY";
const std::string DVCSCrossSectionTotal::DVCS_CROSSSECTION_TOTAL_N0 =
        "DVCSCrossSectionTotal_N0";
const std::string DVCSCrossSectionTotal::DVCS_CROSSSECTION_TOTAL_N1 =
        "DVCSCrossSectionTotal_N1";

const unsigned int DVCSCrossSectionTotal::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCrossSectionTotal("DVCSCrossSectionTotal"));

DVCSCrossSectionTotal::DVCSCrossSectionTotal(const std::string &className) :
        DVCSCrossSectionUUMinusPhiIntegrated(className), m_nI0(1000), m_nI1(5) {

    m_rangexB = std::pair<double, double>(1.E-4, 0.7);
    m_rangeT = std::pair<double, double>(-1., 0.);
    m_rangeQ2 = std::pair<double, double>(1., 1.E3);
    m_rangeY = std::pair<double, double>(0., 1.);
}

DVCSCrossSectionTotal::DVCSCrossSectionTotal(const DVCSCrossSectionTotal& other) :
        DVCSCrossSectionUUMinusPhiIntegrated(other), m_nI0(other.m_nI0), m_nI1(
                other.m_nI1) {

    m_rangexB = other.m_rangexB;
    m_rangeT = other.m_rangeT;
    m_rangeQ2 = other.m_rangeQ2;
    m_rangeY = other.m_rangeY;
}

DVCSCrossSectionTotal::~DVCSCrossSectionTotal() {
}

DVCSCrossSectionTotal* DVCSCrossSectionTotal::clone() const {
    return new DVCSCrossSectionTotal(*this);
}

void DVCSCrossSectionTotal::configure(const ElemUtils::Parameters &parameters) {

    //run for base
    DVCSCrossSectionUUMinusPhiIntegrated::configure(parameters);

    //check
    if (parameters.isAvailable(DVCS_CROSSSECTION_TOTAL_RANGEXb)) {
        setRangexB(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DVCS_CROSSSECTION_TOTAL_RANGET)) {
        setRangeT(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DVCS_CROSSSECTION_TOTAL_RANGEQ2)) {
        setRangeQ2(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DVCS_CROSSSECTION_TOTAL_RANGEY)) {
        setRangeY(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DVCS_CROSSSECTION_TOTAL_N0)) {
        setNI0(
                ElemUtils::GenericType(
                        parameters.getLastAvailable().getString()).toUInt());
    }

    if (parameters.isAvailable(DVCS_CROSSSECTION_TOTAL_N1)) {
        setNI1(
                ElemUtils::GenericType(
                        parameters.getLastAvailable().getString()).toUInt());
    }
}

double DVCSCrossSectionTotal::DVCSCrossSectionTotalFunction(double* kin,
        size_t dim, void* par) {

    //parameters
    DVCSCrossSectionTotalParameters* params =
            static_cast<DVCSCrossSectionTotalParameters*>(par);
    double xB = kin[0];
    double t = kin[1];
    double Q2 = kin[2];
    double E = params->m_E;

    //check kinematic range
    double y = Q2 / (2 * xB * Constant::PROTON_MASS * E);
    double epsilon = 2 * xB * Constant::PROTON_MASS / sqrt(Q2);
    double eps2 = epsilon * epsilon;
    double epsroot = sqrt(1 + eps2);
    double tfactor = -Q2 / (4 * xB * (1 - xB) + eps2);
    double tmin = tfactor * (2 * (1 - xB) * (1 - epsroot) + eps2);
    double tmax = tfactor * (2 * (1 - xB) * (1 + epsroot) + eps2);
    double xBmin = 2 * Q2 * E / Constant::PROTON_MASS / (4 * E * E - Q2);

    if (xB < xBmin || xB > 1.)
        return 0.;

    if (t > tmin || t < tmax)
        return 0.;

    if (y < params->m_yCut.first || y > params->m_yCut.second)
        return 0.;

    //evaluate
    double result =
            params->m_pDVCSCrossSectionTotal->DVCSCrossSectionUUMinusPhiIntegrated::computeObservable(
                    DVCSObservableKinematic(xB, t, Q2, E, 0.),
                    params->m_gpdType).getValue();

    return result;
}

PhysicalType<double> DVCSCrossSectionTotal::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //result and error
    double res, err;

    //parameters
    DVCSCrossSectionTotalParameters params;

    params.m_pDVCSCrossSectionTotal = this;
    params.m_E = kinematic.getE().getValue();

    //Q2 max
    double maxQ2 = 2 * Constant::PROTON_MASS * kinematic.getE().getValue();

    if (m_rangeQ2.second > maxQ2) {
        warn(__func__,
                ElemUtils::Formatter() << "Upper limit of Q2 "
                        << m_rangeQ2.second
                        << " grater than maximum value  allowed for this energy, "
                        << maxQ2 << ", the later used instead");
    }

    //ranges
    // xB, Q2, t
    double xl[3] = { m_rangexB.first, m_rangeT.first, m_rangeQ2.first };
    double xu[3] = { m_rangexB.second, m_rangeT.second, (
            (m_rangeQ2.second > maxQ2) ? (maxQ2) : (m_rangeQ2.second)) };

    // y cut
    params.m_yCut = m_rangeY;

    //random
    const gsl_rng_type* T = gsl_rng_default;
    gsl_rng* r = gsl_rng_alloc(T);

    //function
    gsl_monte_function G = { &DVCSCrossSectionTotalFunction, 3, &params };

    //run
    gsl_monte_vegas_state *s = gsl_monte_vegas_alloc(3);

    for (size_t i = 0; i < m_nI1; i++) {

        gsl_monte_vegas_integrate(&G, xl, xu, 3, m_nI0, r, s, &res, &err);

        info(__func__,
                ElemUtils::Formatter() << "Intermediate result: cycle: " << i
                        << " result: " << res << " error: " << err << " [nb]");
    }

    //free
    gsl_monte_vegas_free(s);
    gsl_rng_free(r);

    //return
    return PhysicalType<double>(res, PhysicalUnit::NB);
}

void DVCSCrossSectionTotal::printChangeOfRange(const std::string& func,
        const std::string& name, const std::pair<double, double>& oldValues,
        const std::pair<double, double>& newValues) const {
    info(func,
            ElemUtils::Formatter() << "Integration range of " << name
                    << " changed from (" << oldValues.first << ", "
                    << oldValues.second << ") to (" << newValues.first << ", "
                    << newValues.second << ")");
}

std::pair<double, double> DVCSCrossSectionTotal::parseRange(
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

size_t DVCSCrossSectionTotal::getNI0() const {
    return m_nI0;
}

void DVCSCrossSectionTotal::setNI0(size_t nI0) {

    info(__func__,
            ElemUtils::Formatter()
                    << "Number of iteration in single cycle changed from "
                    << m_nI0 << " to " << nI0);
    m_nI0 = nI0;
}

size_t DVCSCrossSectionTotal::getNI1() const {
    return m_nI1;
}

void DVCSCrossSectionTotal::setNI1(size_t nI1) {

    info(__func__,
            ElemUtils::Formatter() << "Number of cycles changed from " << m_nI1
                    << " to " << nI1);
    m_nI1 = nI1;
}

const std::pair<double, double>& DVCSCrossSectionTotal::getRangeQ2() const {
    return m_rangeQ2;
}

void DVCSCrossSectionTotal::setRangeQ2(
        const std::pair<double, double>& rangeQ2) {

    printChangeOfRange(__func__, "Q2", m_rangeQ2, rangeQ2);
    m_rangeQ2 = rangeQ2;
}

const std::pair<double, double>& DVCSCrossSectionTotal::getRangeT() const {
    return m_rangeT;
}

void DVCSCrossSectionTotal::setRangeT(const std::pair<double, double>& rangeT) {

    printChangeOfRange(__func__, "t", m_rangeT, rangeT);
    m_rangeT = rangeT;
}

const std::pair<double, double>& DVCSCrossSectionTotal::getRangexB() const {
    return m_rangexB;
}

void DVCSCrossSectionTotal::setRangexB(
        const std::pair<double, double>& rangexB) {

    printChangeOfRange(__func__, "xB", m_rangexB, rangexB);
    m_rangexB = rangexB;
}

const std::pair<double, double>& DVCSCrossSectionTotal::getRangeY() const {
    return m_rangeY;
}

void DVCSCrossSectionTotal::setRangeY(const std::pair<double, double>& rangeY) {

    printChangeOfRange(__func__, "y", m_rangeY, rangeY);
    m_rangeY = rangeY;
}

} /* namespace PARTONS */

