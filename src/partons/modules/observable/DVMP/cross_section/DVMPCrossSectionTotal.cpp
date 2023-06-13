#include "../../../../../../include/partons/modules/observable/DVMP/cross_section/DVMPCrossSectionTotal.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_vegas.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_integration.h>
#include <cmath>
#include <iostream>
#include <vector>

#include "../../../../../../include/partons/beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/process/DVMP/DVMPProcessModule.h"
#include "../../../../../../include/partons/modules/xi_converter/DVMP/DVMPXiConverterModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string DVMPCrossSectionTotal::DVMP_CROSSSECTION_TOTAL_RANGEXb =
        "DVMPCrossSectionTotal_rangeXb";
const std::string DVMPCrossSectionTotal::DVMP_CROSSSECTION_TOTAL_RANGET =
        "DVMPCrossSectionTotal_rangeT";
const std::string DVMPCrossSectionTotal::DVMP_CROSSSECTION_TOTAL_RANGEQ2 =
        "DVMPCrossSectionTotal_rangeQ2";
const std::string DVMPCrossSectionTotal::DVMP_CROSSSECTION_TOTAL_RANGEPHI =
         "DVMPCrossSectionTotal_rangePhi";
const std::string DVMPCrossSectionTotal::DVMP_CROSSSECTION_TOTAL_RANGEY =
        "DVMPCrossSectionTotal_rangeY";
const std::string DVMPCrossSectionTotal::DVMP_CROSSSECTION_TOTAL_RANGENu =
        "DVMPCrossSectionTotal_rangeNu";
const std::string DVMPCrossSectionTotal::DVMP_CROSSSECTION_TOTAL_N0 =
        "DVMPCrossSectionTotal_N0";
const std::string DVMPCrossSectionTotal::DVMP_CROSSSECTION_TOTAL_N1 =
        "DVMPCrossSectionTotal_N1";
const std::string DVMPCrossSectionTotal::DVMP_CROSSSECTION_TOTAL_VIRTUALPHOTOPRODUCTION =
        "DVMPCrossSectionTotal_virtPhotoProd";

const unsigned int DVMPCrossSectionTotal::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPCrossSectionTotal("DVMPCrossSectionTotal"));

DVMPCrossSectionTotal::DVMPCrossSectionTotal(const std::string &className) :
        DVMPCrossSectionUUUMinus(className), m_nI0(5000), m_nI1(5), m_evaluatePhotoProduction(
                false) {

    m_rangexB = std::pair<double, double>(1.E-4, 0.7);
    m_rangeT = std::pair<double, double>(-1., -0.0001);
    m_rangeQ2 = std::pair<double, double>(1., 1.E3);
    m_rangePhi = std::pair<double, double>(0., 2*M_PI);
    m_rangeY = std::pair<double, double>(1.E-4, 1.);
    m_rangeNu = std::pair<double, double>(0., 1.E3);
}

DVMPCrossSectionTotal::DVMPCrossSectionTotal(const DVMPCrossSectionTotal& other) :
        DVMPCrossSectionUUUMinus(other), m_nI0(other.m_nI0), m_nI1(
                other.m_nI1), m_evaluatePhotoProduction(
                other.m_evaluatePhotoProduction) {

    m_rangexB = other.m_rangexB;
    m_rangeT = other.m_rangeT;
    m_rangeQ2 = other.m_rangeQ2;
    m_rangePhi = other.m_rangePhi;
    m_rangeY = other.m_rangeY;
    m_rangeNu = other.m_rangeNu;
}

DVMPCrossSectionTotal::~DVMPCrossSectionTotal() {
}

DVMPCrossSectionTotal* DVMPCrossSectionTotal::clone() const {
    return new DVMPCrossSectionTotal(*this);
}

void DVMPCrossSectionTotal::configure(const ElemUtils::Parameters &parameters) {

    //run for base
    DVMPCrossSectionUUUMinus::configure(parameters);

    //check
    if (parameters.isAvailable(DVMP_CROSSSECTION_TOTAL_RANGEXb)) {
        setRangexB(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DVMP_CROSSSECTION_TOTAL_RANGET)) {
        setRangeT(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DVMP_CROSSSECTION_TOTAL_RANGEQ2)) {
        setRangeQ2(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DVMP_CROSSSECTION_TOTAL_RANGEPHI)) {
        setRangePhi(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DVMP_CROSSSECTION_TOTAL_RANGEY)) {
        setRangeY(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DVMP_CROSSSECTION_TOTAL_RANGENu)) {
        setRangeNu(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(DVMP_CROSSSECTION_TOTAL_N0)) {
        setNI0(
                ElemUtils::GenericType(
                        parameters.getLastAvailable().getString()).toUInt());
    }

    if (parameters.isAvailable(DVMP_CROSSSECTION_TOTAL_N1)) {
        setNI1(
                ElemUtils::GenericType(
                        parameters.getLastAvailable().getString()).toUInt());
    }

    if (parameters.isAvailable(
            DVMP_CROSSSECTION_TOTAL_VIRTUALPHOTOPRODUCTION)) {
        setEvaluatePhotoProduction(
                ElemUtils::GenericType(
                        parameters.getLastAvailable().getString()).toBoolean());
    }
}

double DVMPCrossSectionTotal::DVMPCrossSectionTotalFunctionA(double* kin,
        size_t dim, void* par) {

    //parameters
    DVMPCrossSectionTotalParameters* params =
            static_cast<DVMPCrossSectionTotalParameters*>(par);

    double y = exp(kin[0]);
    double t = -1 * exp(kin[1]);
    double Q2 = exp(kin[2]);

    double E = params->m_E;
    MesonType::Type mesonType = params->m_mesonType;

    //jacobian
    double jacobian = -1 * y * t * Q2;

    //check kinematic range
    double xB = Q2 / (2 * y * Constant::PROTON_MASS * E);
    double nu = y * E;
    double xi =
            params->m_pDVMPCrossSectionTotal->getProcessModule()->getXiConverterModule()->compute(
                    DVMPObservableKinematic(xB, t, Q2, E, 0., mesonType)).getValue();
    double tmin = -4. * pow(Constant::PROTON_MASS, 2.) * pow(xi, 2.)
            / (1. - pow(xi, 2.));
    double tmax = -1.E6;
    double xBmin = 2 * Q2 * E / Constant::PROTON_MASS / (4 * E * E - Q2);

    if (xB < xBmin || xB > 1.)
        return 0.;

    if (t > tmin || t < tmax)
        return 0.;

    if (xB < params->m_xBCut.first || xB > params->m_xBCut.second)
        return 0.;

    if (nu < params->m_nuCut.first || nu > params->m_nuCut.second)
        return 0.;

    //flux
    double flux = 1.;

    if (params->m_evaluatePhotoProduction) {

        flux *= Constant::FINE_STRUCTURE_CONSTANT * (1. - xB);
        flux /= 2 * M_PI * Q2 * y * E;
        flux *= pow(y, 2) * (1. - (2 * pow(Constant::ELECTRON_MASS, 2)) / Q2)
                + 2. / (1. + Q2 / pow(nu, 2)) * (1. - y - Q2 / pow(2 * E, 2));

        flux *= nu / xB;
    }

    //evaluate
    params->m_y = y;
    params->m_t = t;
    params->m_Q2 = Q2;

    gsl_integration_workspace* w
            = gsl_integration_workspace_alloc (1000);

    double result, error;

    gsl_function F;
    F.function = &DVMPCrossSectionTotalFunctionB;
    F.params = par;

    gsl_integration_qag(&F, params->m_phiCut.first, params->m_phiCut.second, 0, 1.E-1, 1000, 6, w, &result, &error);

    gsl_integration_workspace_free (w);

    return result * jacobian / flux;
}

double DVMPCrossSectionTotal::DVMPCrossSectionTotalFunctionB(double phi, void* par){

    //parameters
    DVMPCrossSectionTotalParameters* params =
            static_cast<DVMPCrossSectionTotalParameters*>(par);

    double y = params->m_y;
    double t = params->m_t;
    double Q2 = params->m_Q2;
    double E = params->m_E;
    MesonType::Type mesonType = params->m_mesonType;

    double xB = Q2 / (2 * y * Constant::PROTON_MASS * E);
    double jacobian = xB / y;

    double result = jacobian * params->m_pDVMPCrossSectionTotal->DVMPCrossSectionUUUMinus::computeObservable(
                    DVMPObservableKinematic(xB, t, Q2, E, phi, mesonType),
                    params->m_gpdType).getValue();
    
    if(std::isfinite(result)){
        return result;
    }else{
        std::cout << "Result is unfinite, return 0" << std::endl;
        return 0;
    }
}

PhysicalType<double> DVMPCrossSectionTotal::computeObservable(
        const DVMPObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //disable
    gsl_set_error_handler_off();

    //result and error
    double res, err;

    //parameters
    DVMPCrossSectionTotalParameters params;

    params.m_pDVMPCrossSectionTotal = this;
    params.m_E = kinematic.getE().getValue();
    params.m_mesonType = kinematic.getMesonType();
    params.m_gpdType = gpdType;

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
    // y, Q2, t
    double xl[3] = { log(m_rangeY.first), log(-1 * m_rangeT.second), log(m_rangeQ2.first) };
    double xu[3] = { log(m_rangeY.second), log(-1 * m_rangeT.first), log(
            (m_rangeQ2.second > maxQ2) ? (maxQ2) : (m_rangeQ2.second)) };

    // cuts
    params.m_xBCut = m_rangexB;
    params.m_nuCut = m_rangeNu;
    params.m_phiCut = m_rangePhi;

    // virtual photo-production
    params.m_evaluatePhotoProduction = m_evaluatePhotoProduction;

    //random
    const gsl_rng_type* T = gsl_rng_default;
    gsl_rng* r = gsl_rng_alloc(T);

    //function
    gsl_monte_function G = { &DVMPCrossSectionTotalFunctionA, 3, &params };

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

void DVMPCrossSectionTotal::printChangeOfRange(const std::string& func,
        const std::string& name, const std::pair<double, double>& oldValues,
        const std::pair<double, double>& newValues) const {
    info(func,
            ElemUtils::Formatter() << "Integration range of " << name
                    << " changed from (" << oldValues.first << ", "
                    << oldValues.second << ") to (" << newValues.first << ", "
                    << newValues.second << ")");
}

std::pair<double, double> DVMPCrossSectionTotal::parseRange(
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

size_t DVMPCrossSectionTotal::getNI0() const {
    return m_nI0;
}

void DVMPCrossSectionTotal::setNI0(size_t nI0) {

    info(__func__,
            ElemUtils::Formatter()
                    << "Number of iteration in single cycle changed from "
                    << m_nI0 << " to " << nI0);
    m_nI0 = nI0;
}

size_t DVMPCrossSectionTotal::getNI1() const {
    return m_nI1;
}

void DVMPCrossSectionTotal::setNI1(size_t nI1) {

    info(__func__,
            ElemUtils::Formatter() << "Number of cycles changed from " << m_nI1
                    << " to " << nI1);
    m_nI1 = nI1;
}

const std::pair<double, double>& DVMPCrossSectionTotal::getRangeQ2() const {
    return m_rangeQ2;
}

void DVMPCrossSectionTotal::setRangeQ2(
        const std::pair<double, double>& rangeQ2) {

    printChangeOfRange(__func__, "Q2", m_rangeQ2, rangeQ2);
    m_rangeQ2 = rangeQ2;
}

const std::pair<double, double>& DVMPCrossSectionTotal::getRangeT() const {
    return m_rangeT;
}

void DVMPCrossSectionTotal::setRangeT(const std::pair<double, double>& rangeT) {

    printChangeOfRange(__func__, "t", m_rangeT, rangeT);
    m_rangeT = rangeT;
}

const std::pair<double, double>& DVMPCrossSectionTotal::getRangexB() const {
    return m_rangexB;
}

void DVMPCrossSectionTotal::setRangexB(
        const std::pair<double, double>& rangexB) {

    printChangeOfRange(__func__, "xB", m_rangexB, rangexB);
    m_rangexB = rangexB;
}

const std::pair<double, double>& DVMPCrossSectionTotal::getRangeY() const {
    return m_rangeY;
}

void DVMPCrossSectionTotal::setRangeY(const std::pair<double, double>& rangeY) {

    printChangeOfRange(__func__, "y", m_rangeY, rangeY);
    m_rangeY = rangeY;
}

const std::pair<double, double>& DVMPCrossSectionTotal::getRangeNu() const {
    return m_rangeNu;
}

void DVMPCrossSectionTotal::setRangeNu(
        const std::pair<double, double>& rangeNu) {

    printChangeOfRange(__func__, "nu", m_rangeNu, rangeNu);
    m_rangeNu = rangeNu;
}

bool DVMPCrossSectionTotal::isEvaluatePhotoProduction() const {
    return m_evaluatePhotoProduction;
}

const std::pair<double, double>& DVMPCrossSectionTotal::getRangePhi() const {
        return m_rangePhi;
}

void DVMPCrossSectionTotal::setRangePhi(const std::pair<double, double>& rangePhi) {

        printChangeOfRange(__func__, "phi", m_rangePhi, rangePhi);
            m_rangePhi = rangePhi;
}

void DVMPCrossSectionTotal::setEvaluatePhotoProduction(
        bool evaluatePhotoProduction) {

    info(__func__,
            ElemUtils::Formatter()
                    << "Evaluation of virtual photo-production set to: "
                    << evaluatePhotoProduction);

    m_evaluatePhotoProduction = evaluatePhotoProduction;
}

} /* namespace PARTONS */

