#include "../../../../../../include/partons/modules/observable/DDVCS/cross_section/DDVCSCrossSectionOverDyOnly.h"

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

const std::string DDVCSCrossSectionOverDyOnly::DDVCS_CROSSSECTION_OVER_DY_ONLY_RANGEXb =
        "DDVCSCrossSectionOverDyOnly_rangeXb";
const std::string DDVCSCrossSectionOverDyOnly::DDVCS_CROSSSECTION_OVER_DY_ONLY_RANGET =
        "DDVCSCrossSectionOverDyOnly_rangeT";
const std::string DDVCSCrossSectionOverDyOnly::DDVCS_CROSSSECTION_OVER_DY_ONLY_RANGEQ2 =
        "DDVCSCrossSectionOverDyOnly_rangeQ2";
const std::string DDVCSCrossSectionOverDyOnly::DDVCS_CROSSSECTION_OVER_DY_ONLY_RANGEQ2PRIM =
        "DDVCSCrossSectionOverDyOnly_rangeQ2PRIM";
const std::string DDVCSCrossSectionOverDyOnly::DDVCS_CROSSSECTION_OVER_DY_ONLY_RANGEPHI =
        "DDVCSCrossSectionOverDyOnly_rangePhi";
const std::string DDVCSCrossSectionOverDyOnly::DDVCS_CROSSSECTION_OVER_DY_ONLY_RANGEPHIL =
        "DDVCSCrossSectionOverDyOnly_rangePhiL";
const std::string DDVCSCrossSectionOverDyOnly::DDVCS_CROSSSECTION_OVER_DY_ONLY_RANGETHETAL =
        "DDVCSCrossSectionOverDyOnly_rangeThetaL";
const std::string DDVCSCrossSectionOverDyOnly::DDVCS_CROSSSECTION_OVER_DY_ONLY_RANGEY =
        "DDVCSCrossSectionOverDyOnly_rangeY";
const std::string DDVCSCrossSectionOverDyOnly::DDVCS_CROSSSECTION_OVER_DY_ONLY_N0 =
        "DDVCSCrossSectionOverDyOnly_N0";
const std::string DDVCSCrossSectionOverDyOnly::DDVCS_CROSSSECTION_OVER_DY_ONLY_N1 =
        "DDVCSCrossSectionOverDyOnly_N1";

const unsigned int DDVCSCrossSectionOverDyOnly::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSCrossSectionOverDyOnly("DDVCSCrossSectionOverDyOnly"));

DDVCSCrossSectionOverDyOnly::DDVCSCrossSectionOverDyOnly(
        const std::string &className) :
        DDVCSCrossSectionUUMinus(className), m_nI0(1000), m_nI1(5) {

    m_rangexB = std::pair<double, double>(0., 1.);
    m_rangeT = std::pair<double, double>(-1., 0.);
    m_rangeQ2 = std::pair<double, double>(1., 1.E3);
    m_rangeQ2Prim = std::pair<double, double>(1., 1.E3);
    m_rangePhi = std::pair<double, double>(0., 2 * M_PI);
    m_rangePhiL = std::pair<double, double>(0., 2 * M_PI);
    m_rangeThetaL = std::pair<double, double>(0., M_PI);
    m_rangeY = std::pair<double, double>(0., 1.);
}

DDVCSCrossSectionOverDyOnly::DDVCSCrossSectionOverDyOnly(
        const DDVCSCrossSectionOverDyOnly& other) :
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

DDVCSCrossSectionOverDyOnly::~DDVCSCrossSectionOverDyOnly() {
}

DDVCSCrossSectionOverDyOnly* DDVCSCrossSectionOverDyOnly::clone() const {
    return new DDVCSCrossSectionOverDyOnly(*this);
}

void DDVCSCrossSectionOverDyOnly::configure(
        const ElemUtils::Parameters &parameters) {

    //run for base
    DDVCSCrossSectionUUMinus::configure(parameters);

    //check
    if (parameters.isAvailable(DDVCS_CROSSSECTION_OVER_DY_ONLY_RANGEXb)) {
        setRangexB(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_OVER_DY_ONLY_RANGET)) {
        setRangeT(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_OVER_DY_ONLY_RANGEQ2)) {
        setRangeQ2(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_OVER_DY_ONLY_RANGEQ2PRIM)) {
        setRangeQ2Prim(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_OVER_DY_ONLY_RANGEPHI)) {
        setRangePhi(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_OVER_DY_ONLY_RANGEPHIL)) {
        setRangePhiL(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_OVER_DY_ONLY_RANGETHETAL)) {
        setRangeThetaL(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_OVER_DY_ONLY_RANGEY)) {
        setRangeY(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_OVER_DY_ONLY_N0)) {
        setNI0(
                ElemUtils::GenericType(
                        parameters.getLastAvailable().getString()).toUInt());
    }

    if (parameters.isAvailable(DDVCS_CROSSSECTION_OVER_DY_ONLY_N1)) {
        setNI1(
                ElemUtils::GenericType(
                        parameters.getLastAvailable().getString()).toUInt());
    }
}

double DDVCSCrossSectionOverDyOnly::DDVCSCrossSectionOverDyOnlyFunction(
        double* kin, size_t dim, void* par) {

    //parameters
    DDVCSCrossSectionOverDyOnlyParameters* params =
            static_cast<DDVCSCrossSectionOverDyOnlyParameters*>(par);

    size_t i = 0;
    double norm = -params->m_Q2
            / (2. * Constant::PROTON_MASS * params->m_E * params->m_y
                    * params->m_y); //dxB = norm * dy

    if (params->m_pDDVCSCrossSectionOverDyOnly->getRangeY().first
            == params->m_pDDVCSCrossSectionOverDyOnly->getRangeY().second) {
        params->m_y = params->m_pDDVCSCrossSectionOverDyOnly->getRangeY().first;
    } else {

        params->m_y = pow(10., kin[i]);
        norm *= pow(10., kin[i]) * log(10.);
        i++;
    }

    if (params->m_pDDVCSCrossSectionOverDyOnly->getRangeT().first
            == params->m_pDDVCSCrossSectionOverDyOnly->getRangeT().second) {
        params->m_t = params->m_pDDVCSCrossSectionOverDyOnly->getRangeT().first;
    } else {

        params->m_t = kin[i];
        i++;
    }

    if (params->m_pDDVCSCrossSectionOverDyOnly->getRangeQ2().first
            == params->m_pDDVCSCrossSectionOverDyOnly->getRangeQ2().second) {
        params->m_Q2 =
                params->m_pDDVCSCrossSectionOverDyOnly->getRangeQ2().first;
    } else {

        params->m_Q2 = pow(10., kin[i]);
        norm *= pow(10., kin[i]) * log(10.);
        i++;

    }

    if (params->m_pDDVCSCrossSectionOverDyOnly->getRangeQ2Prim().first
            == params->m_pDDVCSCrossSectionOverDyOnly->getRangeQ2Prim().second) {
        params->m_Q2Prim =
                params->m_pDDVCSCrossSectionOverDyOnly->getRangeQ2Prim().first;
    } else {

        params->m_Q2Prim = pow(10., kin[i]);
        norm *= pow(10., kin[i]) * log(10.);
        i++;

    }

    double E = params->m_E;

    //check kinematic range
    double xB = params->m_Q2 / (2. * Constant::PROTON_MASS * E * params->m_y);
    if (xB < params->m_xBCut.first || xB > params->m_xBCut.second) {
        return 0.;
    }

    double Q2Bar = 0.5 * (params->m_Q2 - params->m_Q2Prim + params->m_t / 2.);
    double xi = (params->m_Q2 + params->m_Q2Prim)
            / (2. * params->m_Q2 / xB - params->m_Q2 - params->m_Q2Prim
                    + params->m_t);
    double rho = xi * 2. * Q2Bar / (params->m_Q2 + params->m_Q2Prim);

    double Q2min = 4. * xB * Constant::MUON_MASS
            * (Constant::MUON_MASS + Constant::PROTON_MASS) / (1. - xB);
    double Q2max = 4. * pow(E, 2.) * Constant::PROTON_MASS * xB
            / (Constant::PROTON_MASS * xB + 2. * E);
    double Q2PrimMin = 4. * pow(Constant::MUON_MASS, 2.);
    double Q2PrimMax = pow(
            sqrt(pow(Constant::PROTON_MASS, 2.) + params->m_Q2 * (1. / xB - 1.))
                    - Constant::PROTON_MASS, 2.);

    double yMin = params->m_Q2 / (2. * Constant::PROTON_MASS * E);
    double yMax = 1.;

    double tMin = -4. * pow(Constant::PROTON_MASS * xi, 2.) / (1. - xi * xi);
    double tMax = -Q2Bar * (1. - xi * xi) / (rho * (1. - rho));

    if (params->m_Q2 < Q2min || params->m_Q2 > Q2max) {
        return 0.;
    }

    if (params->m_Q2Prim < Q2PrimMin || params->m_Q2Prim > Q2PrimMax) {
        return 0.;
    }

    if (-params->m_t < -tMin || -params->m_t > -tMax) {
        return 0.;
    }

    if (params->m_y < yMin || params->m_y > yMax) {
        return 0.;
    }
    //ranges
    double min[3];
    double max[3];

    min[0] = params->m_pDDVCSCrossSectionOverDyOnly->getRangePhi().first;
    min[1] = params->m_pDDVCSCrossSectionOverDyOnly->getRangePhiL().first;
    min[2] = params->m_pDDVCSCrossSectionOverDyOnly->getRangeThetaL().first;

    max[0] = params->m_pDDVCSCrossSectionOverDyOnly->getRangePhi().second;
    max[1] = params->m_pDDVCSCrossSectionOverDyOnly->getRangePhiL().second;
    max[2] = params->m_pDDVCSCrossSectionOverDyOnly->getRangeThetaL().second;

    //function
    gsl_monte_function GAngle = { &DDVCSCrossSectionOverDyOnlyFunctionAngle, 3,
            par };

    //run
    gsl_monte_vegas_state* sAngle = gsl_monte_vegas_alloc(3);

    double res, err;

    for (size_t i = 0; i < 1; i++) {

        gsl_monte_vegas_integrate(&GAngle, min, max, 3, 100, params->m_r,
                sAngle, &res, &err);

        res *= norm;
        err *= norm;

        std::cout << params->m_y << ' ' << params->m_t << ' ' << params->m_Q2
                << ' ' << params->m_Q2Prim << std::endl;
        std::cout << res << " " << err << std::endl;
    }

    //free
    gsl_monte_vegas_free(sAngle);

    return res;
}

double DDVCSCrossSectionOverDyOnly::DDVCSCrossSectionOverDyOnlyFunctionAngle(
        double* kin, size_t dim, void* par) {

    //parameters
    DDVCSCrossSectionOverDyOnlyParameters* params =
            static_cast<DDVCSCrossSectionOverDyOnlyParameters*>(par);

    double xB = params->m_Q2
            / (2. * Constant::PROTON_MASS * params->m_E * params->m_y);

    //return
    return params->m_pDDVCSCrossSectionOverDyOnly->DDVCSCrossSectionUUMinus::computeObservable(
            DDVCSObservableKinematic(xB, params->m_t, params->m_Q2,
                    params->m_Q2Prim, params->m_E, kin[0], kin[1], kin[2]),
            params->m_gpdType).getValue();
}

PhysicalType<double> DDVCSCrossSectionOverDyOnly::computeObservable(
        const DDVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //result and error
    double res, err;

    //parameters
    DDVCSCrossSectionOverDyOnlyParameters params;

    params.m_pDDVCSCrossSectionOverDyOnly = this;
    params.m_E = kinematic.getE().getValue();
    params.m_xBCut = m_rangexB;
    params.m_y = kinematic.getQ2().getValue()
            / (2. * Constant::PROTON_MASS * kinematic.getE().getValue()
                    * kinematic.getXB().getValue());

    //ranges
    std::vector<double> min;
    std::vector<double> max;

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
    gsl_monte_function G = { &DDVCSCrossSectionOverDyOnlyFunction, min.size(),
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

void DDVCSCrossSectionOverDyOnly::printChangeOfRange(const std::string& func,
        const std::string& name, const std::pair<double, double>& oldValues,
        const std::pair<double, double>& newValues) const {
    info(func,
            ElemUtils::Formatter() << "Integration range of " << name
                    << " changed from (" << oldValues.first << ", "
                    << oldValues.second << ") to (" << newValues.first << ", "
                    << newValues.second << ")");
}

std::pair<double, double> DDVCSCrossSectionOverDyOnly::parseRange(
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

size_t DDVCSCrossSectionOverDyOnly::getNI0() const {
    return m_nI0;
}

void DDVCSCrossSectionOverDyOnly::setNI0(size_t nI0) {

    info(__func__,
            ElemUtils::Formatter()
                    << "Number of iteration in single cycle changed from "
                    << m_nI0 << " to " << nI0);
    m_nI0 = nI0;
}

size_t DDVCSCrossSectionOverDyOnly::getNI1() const {
    return m_nI1;
}

void DDVCSCrossSectionOverDyOnly::setNI1(size_t nI1) {

    info(__func__,
            ElemUtils::Formatter() << "Number of cycles changed from " << m_nI1
                    << " to " << nI1);
    m_nI1 = nI1;
}

const std::pair<double, double>& DDVCSCrossSectionOverDyOnly::getRangexB() const {
    return m_rangexB;
}

void DDVCSCrossSectionOverDyOnly::setRangexB(
        const std::pair<double, double>& rangexB) {

    printChangeOfRange(__func__, "xB", m_rangexB, rangexB);
    m_rangexB = rangexB;
}

const std::pair<double, double>& DDVCSCrossSectionOverDyOnly::getRangeT() const {
    return m_rangeT;
}

void DDVCSCrossSectionOverDyOnly::setRangeT(
        const std::pair<double, double>& rangeT) {

    printChangeOfRange(__func__, "t", m_rangeT, rangeT);
    m_rangeT = rangeT;
}

const std::pair<double, double>& DDVCSCrossSectionOverDyOnly::getRangeQ2() const {
    return m_rangeQ2;
}

void DDVCSCrossSectionOverDyOnly::setRangeQ2(
        const std::pair<double, double>& rangeQ2) {

    printChangeOfRange(__func__, "Q2", m_rangeQ2, rangeQ2);
    m_rangeQ2 = rangeQ2;
}

const std::pair<double, double>& DDVCSCrossSectionOverDyOnly::getRangeQ2Prim() const {
    return m_rangeQ2Prim;
}

void DDVCSCrossSectionOverDyOnly::setRangeQ2Prim(
        const std::pair<double, double>& rangeQ2Prim) {

    printChangeOfRange(__func__, "Q2Prim", m_rangeQ2Prim, rangeQ2Prim);
    m_rangeQ2Prim = rangeQ2Prim;
}

const std::pair<double, double>& DDVCSCrossSectionOverDyOnly::getRangePhi() const {
    return m_rangePhi;
}

void DDVCSCrossSectionOverDyOnly::setRangePhi(
        const std::pair<double, double>& rangePhi) {

    printChangeOfRange(__func__, "phi", m_rangePhi, rangePhi);
    m_rangePhi = rangePhi;
}

const std::pair<double, double>& DDVCSCrossSectionOverDyOnly::getRangePhiL() const {
    return m_rangePhiL;
}

void DDVCSCrossSectionOverDyOnly::setRangePhiL(
        const std::pair<double, double>& rangePhiL) {

    printChangeOfRange(__func__, "phiL", m_rangePhiL, rangePhiL);
    m_rangePhiL = rangePhiL;
}

const std::pair<double, double>& DDVCSCrossSectionOverDyOnly::getRangeThetaL() const {
    return m_rangeThetaL;
}

void DDVCSCrossSectionOverDyOnly::setRangeThetaL(
        const std::pair<double, double>& rangeThetaL) {

    printChangeOfRange(__func__, "thetaL", m_rangeThetaL, rangeThetaL);
    m_rangeThetaL = rangeThetaL;
}

const std::pair<double, double>& DDVCSCrossSectionOverDyOnly::getRangeY() const {
    return m_rangeY;
}

void DDVCSCrossSectionOverDyOnly::setRangeY(
        const std::pair<double, double>& rangeY) {

    printChangeOfRange(__func__, "y", m_rangeY, rangeY);
    m_rangeY = rangeY;
}

} /* namespace PARTONS */

