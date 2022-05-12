#include "../../../../../../include/partons/modules/observable/DDVCS/cross_section/DDVCSCrossSectionTotal.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_vegas.h>
#include <cmath>
#include <vector>

#include "../../../../../../include/partons/beans/observable/DDVCS/DDVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string DDVCSCrossSectionTotal::DDVCS_CROSSSECTION_TOTAL_RANGEXb =
        "DDVCSCrossSectionTotal_rangeXb";
const std::string DDVCSCrossSectionTotal::DDVCS_CROSSSECTION_TOTAL_RANGET =
        "DDVCSCrossSectionTotal_rangeT";
const std::string DDVCSCrossSectionTotal::DDVCS_CROSSSECTION_TOTAL_RANGEQ2 =
        "DDVCSCrossSectionTotal_rangeQ2";
const std::string DDVCSCrossSectionTotal::DDVCS_CROSSSECTION_TOTAL_RANGEQ2PRIM =
        "DDVCSCrossSectionTotal_rangeQ2PRIM";
const std::string DDVCSCrossSectionTotal::DDVCS_CROSSSECTION_TOTAL_RANGEPHI =
        "DDVCSCrossSectionTotal_rangePhi";
const std::string DDVCSCrossSectionTotal::DDVCS_CROSSSECTION_TOTAL_RANGEPHIL =
        "DDVCSCrossSectionTotal_rangePhiL";
const std::string DDVCSCrossSectionTotal::DDVCS_CROSSSECTION_TOTAL_RANGETHETAL =
        "DDVCSCrossSectionTotal_rangeThetaL";
const std::string DDVCSCrossSectionTotal::DDVCS_CROSSSECTION_TOTAL_RANGEY =
        "DDVCSCrossSectionTotal_rangeY";
const std::string DDVCSCrossSectionTotal::DDVCS_CROSSSECTION_TOTAL_N0 =
        "DDVCSCrossSectionTotal_N0";
const std::string DDVCSCrossSectionTotal::DDVCS_CROSSSECTION_TOTAL_N1 =
        "DDVCSCrossSectionTotal_N1";

const unsigned int DDVCSCrossSectionTotal::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSCrossSectionTotal("DDVCSCrossSectionTotal"));

DDVCSCrossSectionTotal::DDVCSCrossSectionTotal(const std::string &className) :
        DDVCSCrossSectionUUMinus(className), m_nI0(1000), m_nI1(5) {

    m_rangexB = std::pair<double, double>(0., 1.);
    m_rangeT = std::pair<double, double>(-1., 0.);
    m_rangeQ2 = std::pair<double, double>(1., 1.E3);
    m_rangeQ2Prim = std::pair<double, double>(1., 1.E3);
    m_rangePhi = std::pair<double, double>(0., 2 * M_PI);
    m_rangePhiL = std::pair<double, double>(0., 2 * M_PI);
    m_rangeThetaL = std::pair<double, double>(0., 2 * M_PI);
    m_rangeY = std::pair<double, double>(0., 1.);
}

DDVCSCrossSectionTotal::DDVCSCrossSectionTotal(
        const DDVCSCrossSectionTotal& other) :
        DDVCSCrossSectionUUMinus(other), m_nI0(other.m_nI0), m_nI1(other.m_nI1) {

    m_rangexB = other.m_rangexB;
    m_rangeT = other.m_rangeT;
    m_rangeQ2 = other.m_rangeQ2;
    m_rangeQ2Prim = other.m_rangeQ2Prim;
    m_rangePhi = other.m_rangePhi;
    m_rangePhiL = other.m_rangePhiL;
    m_rangeThetaL = other.m_rangeThetaL;
    m_rangeY = other.m_rangeY;
}

DDVCSCrossSectionTotal::~DDVCSCrossSectionTotal() {
}

DDVCSCrossSectionTotal* DDVCSCrossSectionTotal::clone() const {
    return new DDVCSCrossSectionTotal(*this);
}

void DDVCSCrossSectionTotal::configure(
        const ElemUtils::Parameters &parameters) {

    //run for base
    DDVCSCrossSectionUUMinus::configure(parameters);

    //check
    if (parameters.isAvailable(DDVCS_CROSSSECTION_TOTAL_RANGEXb)) {
        setRangexB(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_TOTAL_RANGET)) {
        setRangeT(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_TOTAL_RANGEQ2)) {
        setRangeQ2(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_TOTAL_RANGEQ2PRIM)) {
        setRangeQ2Prim(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_TOTAL_RANGEPHI)) {
        setRangePhi(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_TOTAL_RANGEPHIL)) {
        setRangePhiL(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_TOTAL_RANGETHETAL)) {
        setRangeThetaL(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_TOTAL_RANGEY)) {
        setRangeY(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_TOTAL_N0)) {
        setNI0(
                ElemUtils::GenericType(
                        parameters.getLastAvailable().getString()).toUInt());
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_TOTAL_N1)) {
        setNI1(
                ElemUtils::GenericType(
                        parameters.getLastAvailable().getString()).toUInt());
    }
}

double DDVCSCrossSectionTotal::DDVCSCrossSectionTotalFunction(double* kin,
        size_t dim, void* par) {

    //parameters
    DDVCSCrossSectionTotalParameters* params =
            static_cast<DDVCSCrossSectionTotalParameters*>(par);

    size_t i = 0;
    double norm = 1.;

    if (params->m_pDDVCSCrossSectionTotal->getRangexB().first
            == params->m_pDDVCSCrossSectionTotal->getRangexB().second) {
        params->m_xB = params->m_pDDVCSCrossSectionTotal->getRangexB().first;
    } else {

        params->m_xB = pow(10., kin[i]);
        norm *= pow(10., kin[i]) * log(10.);
        i++;
    }

    if (params->m_pDDVCSCrossSectionTotal->getRangeT().first
            == params->m_pDDVCSCrossSectionTotal->getRangeT().second) {
        params->m_t = params->m_pDDVCSCrossSectionTotal->getRangeT().first;
    } else {

        params->m_t = kin[i];
        i++;
    }

    if (params->m_pDDVCSCrossSectionTotal->getRangeQ2().first
            == params->m_pDDVCSCrossSectionTotal->getRangeQ2().second) {
        params->m_Q2 = params->m_pDDVCSCrossSectionTotal->getRangeQ2().first;
    } else {

        params->m_Q2 = pow(10., kin[i]);
        norm *= pow(10., kin[i]) * log(10.);
        i++;

    }

    if (params->m_pDDVCSCrossSectionTotal->getRangeQ2Prim().first
            == params->m_pDDVCSCrossSectionTotal->getRangeQ2Prim().second) {
        params->m_Q2Prim =
                params->m_pDDVCSCrossSectionTotal->getRangeQ2Prim().first;
    } else {

        params->m_Q2Prim = pow(10., kin[i]);
        norm *= pow(10., kin[i]) * log(10.);
        i++;

    }

    double E = params->m_E;

    //check kinematic range
    double m_BM_Q2 = 0.5 * (params->m_Q2 - params->m_Q2Prim + params->m_t / 2.);
    double m_BM_eta = (params->m_Q2 + params->m_Q2Prim)
            / (2. * params->m_Q2 / params->m_xB - params->m_Q2
                    - params->m_Q2Prim + params->m_t);
    double m_BM_xi = m_BM_eta * 2. * m_BM_Q2
            / (params->m_Q2 + params->m_Q2Prim);

    double m_BM_xBmin = params->m_Q2 / (2. * Constant::PROTON_MASS * E);
    double m_BM_xBmax = 1.;
    double m_BM_Qcal2min = 4. * params->m_xB * Constant::MUON_MASS
            * (Constant::MUON_MASS + Constant::PROTON_MASS)
            / (1. - params->m_xB);
    double m_BM_Qcal2max = 4. * pow(E, 2.) * Constant::PROTON_MASS
            * params->m_xB / (Constant::PROTON_MASS * params->m_xB + 2. * E);
    double m_BM_Mll2min = 4. * pow(Constant::MUON_MASS, 2.);
    double m_BM_Mll2max = pow(
            sqrt(
                    pow(Constant::PROTON_MASS, 2.)
                            + params->m_Q2 * (1. / params->m_xB - 1.))
                    - Constant::PROTON_MASS, 2.);
    double m_BM_tmin = -4. * pow(Constant::PROTON_MASS * m_BM_eta, 2.)
            / (1. - m_BM_eta * m_BM_eta);
    double m_BM_tmax = -m_BM_Q2 * (1. - m_BM_eta * m_BM_eta)
            / (m_BM_xi * (1. - m_BM_xi));

    if (params->m_xB < m_BM_xBmin || params->m_xB > m_BM_xBmax) {
        return 0.;
    }

    if (params->m_Q2 < m_BM_Qcal2min || params->m_Q2 > m_BM_Qcal2max) {
        return 0.;
    }

    if (params->m_Q2Prim < m_BM_Mll2min || params->m_Q2Prim > m_BM_Mll2max) {
        return 0.;
    }

    if (-params->m_t < -m_BM_tmin || -params->m_t > -m_BM_tmax) {
        return 0.;
    }

    double y = params->m_Q2 / (2 * params->m_xB * Constant::PROTON_MASS * E);

    if (y < params->m_yCut.first || y > params->m_yCut.second)
        return 0.;

    //ranges
    double min[3];
    double max[3];

    min[0] = params->m_pDDVCSCrossSectionTotal->getRangePhi().first;
    min[1] = params->m_pDDVCSCrossSectionTotal->getRangePhiL().first;
    min[2] = params->m_pDDVCSCrossSectionTotal->getRangeThetaL().first;

    max[0] = params->m_pDDVCSCrossSectionTotal->getRangePhi().second;
    max[1] = params->m_pDDVCSCrossSectionTotal->getRangePhiL().second;
    max[2] = params->m_pDDVCSCrossSectionTotal->getRangeThetaL().second;

    //function
    gsl_monte_function GAngle = { &DDVCSCrossSectionTotalFunctionAngle, 3, par };

    //run
    gsl_monte_vegas_state* sAngle = gsl_monte_vegas_alloc(3);

    double res, err;

    for (size_t i = 0; i < 1; i++) {

        gsl_monte_vegas_integrate(&GAngle, min, max, 3, 100, params->m_r,
                sAngle, &res, &err);

        res *= norm;
        err *= norm;

        std::cout << res << " " << err << std::endl;
    }

    //free
    gsl_monte_vegas_free(sAngle);

    return res;
}

double DDVCSCrossSectionTotal::DDVCSCrossSectionTotalFunctionAngle(double* kin,
        size_t dim, void* par) {

    //parameters
    DDVCSCrossSectionTotalParameters* params =
            static_cast<DDVCSCrossSectionTotalParameters*>(par);

    //return
    return params->m_pDDVCSCrossSectionTotal->DDVCSCrossSectionUUMinus::computeObservable(
            DDVCSObservableKinematic(params->m_xB, params->m_t, params->m_Q2,
                    params->m_Q2Prim, params->m_E, kin[0], kin[1], kin[2]),
            params->m_gpdType).getValue();
}

PhysicalType<double> DDVCSCrossSectionTotal::computeObservable(
        const DDVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //result and error
    double res, err;

    //parameters
    DDVCSCrossSectionTotalParameters params;

    params.m_pDDVCSCrossSectionTotal = this;
    params.m_E = kinematic.getE().getValue();
    params.m_yCut = m_rangeY;

    //ranges
    std::vector<double> min;
    std::vector<double> max;

    if (m_rangexB.first != m_rangexB.second) {

        min.push_back(log10(m_rangexB.first));
        max.push_back(log10(m_rangexB.second));
    }

    if (m_rangeT.first != m_rangeT.second) {

        min.push_back(m_rangeT.first);
        max.push_back(m_rangeT.second);
    }

    if (m_rangeQ2.first != m_rangeQ2.second) {

        min.push_back(log10(m_rangeQ2.first));
        max.push_back(log10(m_rangeQ2.second));
    }

    if (m_rangeQ2Prim.first != m_rangeQ2Prim.second) {

        min.push_back(log10(m_rangeQ2Prim.first));
        max.push_back(log10(m_rangeQ2Prim.second));
    }

    //info
    info(__func__,
            ElemUtils::Formatter() << "Number of dimensions: " << min.size());

    //random
    params.m_r = gsl_rng_alloc(gsl_rng_default);

    //function
    gsl_monte_function G = { &DDVCSCrossSectionTotalFunction, min.size(),
            &params };

    //run
    gsl_monte_vegas_state* s = gsl_monte_vegas_alloc(min.size());

    for (size_t i = 0; i < m_nI1; i++) {

        gsl_monte_vegas_integrate(&G, &(min[0]), &(max[0]), min.size(), m_nI0,
                params.m_r, s, &res, &err);

        info(__func__,
                ElemUtils::Formatter() << "Intermediate result: cycle: " << i
                        << " result: " << res << " error: " << err << " [nb]");
    }

    //free
    gsl_monte_vegas_free(s);
    gsl_rng_free(params.m_r);

    //return
    return PhysicalType<double>(res, PhysicalUnit::NB).makeSameUnitAs(
            PhysicalUnit::NB);
}

void DDVCSCrossSectionTotal::printChangeOfRange(const std::string& func,
        const std::string& name, const std::pair<double, double>& oldValues,
        const std::pair<double, double>& newValues) const {
    info(func,
            ElemUtils::Formatter() << "Integration range of " << name
                    << " changed from (" << oldValues.first << ", "
                    << oldValues.second << ") to (" << newValues.first << ", "
                    << newValues.second << ")");
}

std::pair<double, double> DDVCSCrossSectionTotal::parseRange(
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

size_t DDVCSCrossSectionTotal::getNI0() const {
    return m_nI0;
}

void DDVCSCrossSectionTotal::setNI0(size_t nI0) {

    info(__func__,
            ElemUtils::Formatter()
                    << "Number of iteration in single cycle changed from "
                    << m_nI0 << " to " << nI0);
    m_nI0 = nI0;
}

size_t DDVCSCrossSectionTotal::getNI1() const {
    return m_nI1;
}

void DDVCSCrossSectionTotal::setNI1(size_t nI1) {

    info(__func__,
            ElemUtils::Formatter() << "Number of cycles changed from " << m_nI1
                    << " to " << nI1);
    m_nI1 = nI1;
}

const std::pair<double, double>& DDVCSCrossSectionTotal::getRangexB() const {
    return m_rangexB;
}

void DDVCSCrossSectionTotal::setRangexB(
        const std::pair<double, double>& rangexB) {

    printChangeOfRange(__func__, "xB", m_rangexB, rangexB);
    m_rangexB = rangexB;
}

const std::pair<double, double>& DDVCSCrossSectionTotal::getRangeT() const {
    return m_rangeT;
}

void DDVCSCrossSectionTotal::setRangeT(
        const std::pair<double, double>& rangeT) {

    printChangeOfRange(__func__, "t", m_rangeT, rangeT);
    m_rangeT = rangeT;
}

const std::pair<double, double>& DDVCSCrossSectionTotal::getRangeQ2() const {
    return m_rangeQ2;
}

void DDVCSCrossSectionTotal::setRangeQ2(
        const std::pair<double, double>& rangeQ2) {

    printChangeOfRange(__func__, "Q2", m_rangeQ2, rangeQ2);
    m_rangeQ2 = rangeQ2;
}

const std::pair<double, double>& DDVCSCrossSectionTotal::getRangeQ2Prim() const {
    return m_rangeQ2Prim;
}

void DDVCSCrossSectionTotal::setRangeQ2Prim(
        const std::pair<double, double>& rangeQ2Prim) {

    printChangeOfRange(__func__, "Q2Prim", m_rangeQ2Prim, rangeQ2Prim);
    m_rangeQ2Prim = rangeQ2Prim;
}

const std::pair<double, double>& DDVCSCrossSectionTotal::getRangePhi() const {
    return m_rangePhi;
}

void DDVCSCrossSectionTotal::setRangePhi(
        const std::pair<double, double>& rangePhi) {

    printChangeOfRange(__func__, "phi", m_rangePhi, rangePhi);
    m_rangePhi = rangePhi;
}

const std::pair<double, double>& DDVCSCrossSectionTotal::getRangePhiL() const {
    return m_rangePhiL;
}

void DDVCSCrossSectionTotal::setRangePhiL(
        const std::pair<double, double>& rangePhiL) {

    printChangeOfRange(__func__, "phiL", m_rangePhiL, rangePhiL);
    m_rangePhiL = rangePhiL;
}

const std::pair<double, double>& DDVCSCrossSectionTotal::getRangeThetaL() const {
    return m_rangeThetaL;
}

void DDVCSCrossSectionTotal::setRangeThetaL(
        const std::pair<double, double>& rangeThetaL) {

    printChangeOfRange(__func__, "thetaL", m_rangeThetaL, rangeThetaL);
    m_rangeThetaL = rangeThetaL;
}

const std::pair<double, double>& DDVCSCrossSectionTotal::getRangeY() const {
    return m_rangeY;
}

void DDVCSCrossSectionTotal::setRangeY(
        const std::pair<double, double>& rangeY) {

    printChangeOfRange(__func__, "y", m_rangeY, rangeY);
    m_rangeY = rangeY;
}

} /* namespace PARTONS */

