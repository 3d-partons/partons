#include "../../../../../../include/partons/modules/observable/TCS/cross_section/TCSCrossSectionTotal.h"

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

#include "../../../../../../include/partons/beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string TCSCrossSectionTotal::TCS_CROSSSECTION_TOTAL_RANGET =
        "TCSCrossSectionTotal_rangeT";
const std::string TCSCrossSectionTotal::TCS_CROSSSECTION_TOTAL_RANGEQ2PRIM =
        "TCSCrossSectionTotal_rangeQ2Prim";
const std::string TCSCrossSectionTotal::TCS_CROSSSECTION_TOTAL_RANGEPHI =
        "TCSCrossSectionTotal_rangePhi";
const std::string TCSCrossSectionTotal::TCS_CROSSSECTION_TOTAL_RANGETHETA =
        "TCSCrossSectionTotal_rangeTheta";
const std::string TCSCrossSectionTotal::TCS_CROSSSECTION_TOTAL_RANGEY =
        "TCSCrossSectionTotal_rangeY";
const std::string TCSCrossSectionTotal::TCS_CROSSSECTION_TOTAL_RANGEQ2 =
        "TCSCrossSectionTotal_rangeQ2";
const std::string TCSCrossSectionTotal::TCS_CROSSSECTION_TOTAL_N0 =
        "TCSCrossSectionTotal_N0";
const std::string TCSCrossSectionTotal::TCS_CROSSSECTION_TOTAL_N1 =
        "TCSCrossSectionTotal_N1";
const std::string TCSCrossSectionTotal::TCS_CROSSSECTION_TOTAL_PHIEPSILON =
        "TCSCrossSectionTotal_phiEpsilon";

const unsigned int TCSCrossSectionTotal::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSCrossSectionTotal("TCSCrossSectionTotal"));

TCSCrossSectionTotal::TCSCrossSectionTotal(const std::string &className) :
        TCSCrossSectionUU(className), m_nI0(100), m_nI1(5), m_phiEpsilon(0.){

    m_rangeT = std::pair<double, double>(-1., -0.0001);
    m_rangeQ2Prim = std::pair<double, double>(1., 1.E3);
    m_rangePhi = std::pair<double, double>(0., 2*M_PI);
    m_rangeTheta = std::pair<double, double>(0.7853, 2.3561);
    m_rangeY = std::pair<double, double>(0.01, 1.);
    m_rangeQ2 = std::pair<double, double>(0.000001, 0.1);
}

TCSCrossSectionTotal::TCSCrossSectionTotal(const TCSCrossSectionTotal& other) :
        TCSCrossSectionUU(other), m_nI0(other.m_nI0), m_nI1(
                other.m_nI1), m_phiEpsilon(other.m_phiEpsilon) {

    m_rangeT = other.m_rangeT;
    m_rangeQ2Prim = other.m_rangeQ2Prim;
    m_rangePhi = other.m_rangePhi;
    m_rangeTheta = other.m_rangeTheta;
    m_rangeY = other.m_rangeY;
    m_rangeQ2 = other.m_rangeQ2;
}

TCSCrossSectionTotal::~TCSCrossSectionTotal() {
}

TCSCrossSectionTotal* TCSCrossSectionTotal::clone() const {
    return new TCSCrossSectionTotal(*this);
}

void TCSCrossSectionTotal::configure(const ElemUtils::Parameters &parameters) {

    //run for base
    TCSCrossSectionUU::configure(parameters);

    //check
    if (parameters.isAvailable(TCS_CROSSSECTION_TOTAL_RANGET)) {
        setRangeT(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(TCS_CROSSSECTION_TOTAL_RANGEQ2PRIM)) {
        setRangeQ2Prim(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(TCS_CROSSSECTION_TOTAL_RANGEPHI)) {
        setRangePhi(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(TCS_CROSSSECTION_TOTAL_RANGETHETA)) {
        setRangeTheta(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(TCS_CROSSSECTION_TOTAL_RANGEY)) {
        setRangeY(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(TCS_CROSSSECTION_TOTAL_RANGEQ2)) {
        setRangeQ2(parseRange(parameters.getLastAvailable().getString()));
    }

    if (parameters.isAvailable(TCS_CROSSSECTION_TOTAL_N0)) {
        setNI0(
                ElemUtils::GenericType(
                        parameters.getLastAvailable().getString()).toUInt());
    }

    if (parameters.isAvailable(TCS_CROSSSECTION_TOTAL_N1)) {
        setNI1(
                ElemUtils::GenericType(
                        parameters.getLastAvailable().getString()).toUInt());
    }

    if (parameters.isAvailable(TCS_CROSSSECTION_TOTAL_PHIEPSILON)) {
        setPhiEpsilon(
                ElemUtils::GenericType(
                        parameters.getLastAvailable().getString()).toDouble());
    }
}

double TCSCrossSectionTotal::TCSCrossSectionTotalFunctionA(double* kin,
        size_t dim, void* par) {

    //parameters
    TCSCrossSectionTotalParameters* params =
            static_cast<TCSCrossSectionTotalParameters*>(par);

    double t = -1 * exp(kin[0]);
    double Q2Prim = exp(kin[1]);
    
    double y = exp(kin[2]);
    double Q2 = exp(kin[3]);

    double Ee = params->m_Ee;

    //jacobian
    double jacobian = -1 * t * Q2Prim * y * Q2; 

    //check kinematic range
    double yMin = Q2 / (2. * Constant::PROTON_MASS * Ee); 
    double Q2Min =
            pow(y * Constant::ELECTRON_MASS,
                    2) / (1. - y);

    if (y < yMin)
        return 0.;
    if (Q2 < Q2Min)
        return 0.;

    //flux
    double flux = Constant::FINE_STRUCTURE_CONSTANT / (2 * M_PI * Q2)
            * ((1. + pow(1. - y, 2)) / y - 2 * (1. - y) * Q2Min / (y * Q2));

    //energy
    double Egamma = y * Ee;

    //evaluate internal variables
    double s = pow(Constant::PROTON_MASS, 2) + 2. * Constant::PROTON_MASS * Egamma;

    double p1cm = Egamma * Constant::PROTON_MASS / sqrt(s);
    double E3cm = (s + Q2Prim - pow(Constant::PROTON_MASS, 2))
            / (2 * sqrt(s));
    double p3cm = sqrt(pow(E3cm, 2) - Q2Prim);

    double tmin = pow(Q2Prim, 2) / (4 * s) - pow(p1cm - p3cm, 2);
    double tmax = pow(Q2Prim, 2) / (4 * s) - pow(p1cm + p3cm, 2);

    if (t > tmin || t < tmax)
        return 0.;

    if(2 * Egamma * Constant::PROTON_MASS
                    < 2 * Constant::PROTON_MASS * sqrt(Q2Prim) + Q2Prim)
        return 0.;

    //evaluate
    params->m_t = t;
    params->m_Q2Prim = Q2Prim;
    params->m_Egamma = Egamma;

    double result, error;
    size_t nCall;

    gsl_function F;
    F.function = &TCSCrossSectionTotalFunctionB;
    F.params = par;

//    gsl_integration_workspace* w = gsl_integration_workspace_alloc (1000);

    if(params->m_phiCut.first < (M_PI - params->m_phiEpsilon) && params->m_phiCut.second > (M_PI + params->m_phiEpsilon)){

        double thisResult, thisError;

        gsl_integration_qng(&F, params->m_phiCut.first, M_PI - params->m_phiEpsilon, 0, 1.E-1, 
            &thisResult, &thisError, &nCall);
        result = thisResult;

        gsl_integration_qng(&F, M_PI + params->m_phiEpsilon, params->m_phiCut.second, 0, 1.E-1, 
            &thisResult, &thisError, &nCall);
        result += thisResult;
    
    }else{

        gsl_integration_qng(&F, params->m_phiCut.first, params->m_phiCut.second, 0, 1.E-1, 
            &result, &error, &nCall);
    }
//    gsl_integration_qng(&F, params->m_phiCut.first, params->m_phiCut.second, 0, 1.E-1, 
//        &result, &error, &nCall);

//    gsl_integration_workspace_free(w);

    return flux * result * jacobian;
}

double TCSCrossSectionTotal::TCSCrossSectionTotalFunctionB(double phi, void* par){

    //parameters
    TCSCrossSectionTotalParameters* params =
            static_cast<TCSCrossSectionTotalParameters*>(par);

    params->m_phi = phi;
    
    double result, error;

    gsl_function F;
    F.function = &TCSCrossSectionTotalFunctionC;
    F.params = params;
    size_t nCalls;

    gsl_integration_workspace* w = gsl_integration_workspace_alloc (1000);

    gsl_integration_qag(&F, params->m_thetaCut.first, params->m_thetaCut.second, 0., 1.E-1, 
        1000, 6, w, &result, &error);
//    gsl_integration_qng(&F, params->m_thetaCut.first, params->m_thetaCut.second, 0., 1.E-1, 
//        &result, &error, &nCalls);

    gsl_integration_workspace_free(w);

    return result;
}

double TCSCrossSectionTotal::TCSCrossSectionTotalFunctionC(double theta, void* par){

    //parameters
    TCSCrossSectionTotalParameters* params =
            static_cast<TCSCrossSectionTotalParameters*>(par);

    double t = params->m_t;
    double Q2Prim = params->m_Q2Prim;
    double Egamma = params->m_Egamma;
    double phi = params->m_phi;

    double result = params->m_pTCSCrossSectionTotal->TCSCrossSectionUU::computeObservable(
        TCSObservableKinematic(t, Q2Prim, Egamma, phi, theta),
        params->m_gpdType).getValue();

    if(std::isnan(result)) {
    
        std::cout << __func__ << " NaN detected" << std::endl;;
        std::cout << t << ' ' << Q2Prim << ' ' << Egamma << ' ' << phi << ' ' << theta << std::endl;
        return 0.;
    }

    if(std::isinf(result)) {
    
        std::cout << __func__ << " INF detected" << std::endl;;
        std::cout << t << ' ' << Q2Prim << ' ' << Egamma << ' ' << phi << ' ' << theta << std::endl;
        return 0.;
    }

    return result;
}

PhysicalType<double> TCSCrossSectionTotal::computeObservable(
        const TCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //disable
    gsl_set_error_handler_off();

    //result and error
    double res, err;

    //parameters
    TCSCrossSectionTotalParameters params;

    params.m_pTCSCrossSectionTotal = this;
    params.m_Ee = kinematic.getE().getValue();

    //Q2max
    double maxQ2 = 2 * Constant::PROTON_MASS * kinematic.getE().getValue();

    if (m_rangeQ2.second > maxQ2) {
        warn(__func__,
                ElemUtils::Formatter() << "Upper limit of Q2"
                        << m_rangeQ2.second
                        << " grater than maximum value  allowed for this energy, "
                        << maxQ2 << ", the later used instead");
    }

    //ranges
    //t, Q'2, y, Q2
    double xl[4] = { log(-1 * m_rangeT.second), log(m_rangeQ2Prim.first), log(m_rangeY.first), log(m_rangeQ2.first)};
    double xu[4] = { log(-1 * m_rangeT.first), log(m_rangeQ2Prim.second), log(m_rangeY.second), log(((m_rangeQ2.second > maxQ2) ? (maxQ2) : (m_rangeQ2.second))) };

    //cuts
    params.m_phiCut = m_rangePhi;
    params.m_phiEpsilon = m_phiEpsilon;
    params.m_thetaCut = m_rangeTheta;

    //random
    const gsl_rng_type* T = gsl_rng_default;
    gsl_rng* r = gsl_rng_alloc(T);

    //function
    gsl_monte_function G = { &TCSCrossSectionTotalFunctionA, 4, &params };

    //run
    gsl_monte_vegas_state* s = gsl_monte_vegas_alloc(4);

    for (size_t i = 0; i < m_nI1; i++) {

        gsl_monte_vegas_integrate(&G, xl, xu, 4, m_nI0, r, s, &res, &err);

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

void TCSCrossSectionTotal::printChangeOfRange(const std::string& func,
        const std::string& name, const std::pair<double, double>& oldValues,
        const std::pair<double, double>& newValues) const {
    info(func,
            ElemUtils::Formatter() << "Integration range of " << name
                    << " changed from (" << oldValues.first << ", "
                    << oldValues.second << ") to (" << newValues.first << ", "
                    << newValues.second << ")");
}

std::pair<double, double> TCSCrossSectionTotal::parseRange(
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

size_t TCSCrossSectionTotal::getNI0() const {
    return m_nI0;
}

void TCSCrossSectionTotal::setNI0(size_t nI0) {

    info(__func__,
            ElemUtils::Formatter()
                    << "Number of iteration in single cycle changed from "
                    << m_nI0 << " to " << nI0);
    m_nI0 = nI0;
}

size_t TCSCrossSectionTotal::getNI1() const {
    return m_nI1;
}

void TCSCrossSectionTotal::setNI1(size_t nI1) {

    info(__func__,
            ElemUtils::Formatter() << "Number of cycles changed from " << m_nI1
                    << " to " << nI1);
    m_nI1 = nI1;
}

const std::pair<double, double>& TCSCrossSectionTotal::getRangeT() const {
    return m_rangeT;
}

void TCSCrossSectionTotal::setRangeT(const std::pair<double, double>& rangeT) {

    printChangeOfRange(__func__, "t", m_rangeT, rangeT);
    m_rangeT = rangeT;
}

const std::pair<double, double>& TCSCrossSectionTotal::getRangeQ2Prim() const {
    return m_rangeQ2Prim;
}

void TCSCrossSectionTotal::setRangeQ2Prim(
        const std::pair<double, double>& rangeQ2Prim) {

    printChangeOfRange(__func__, "Q2Prim", m_rangeQ2Prim, rangeQ2Prim);
    m_rangeQ2Prim = rangeQ2Prim;
}

const std::pair<double, double>& TCSCrossSectionTotal::getRangePhi() const {
    return m_rangePhi;
}

void TCSCrossSectionTotal::setRangePhi(const std::pair<double, double>& rangePhi) {

    printChangeOfRange(__func__, "phi", m_rangePhi, rangePhi);
    m_rangePhi = rangePhi;
}

const std::pair<double, double>& TCSCrossSectionTotal::getRangeTheta() const {
    return m_rangeTheta;
}

void TCSCrossSectionTotal::setRangeTheta(const std::pair<double, double>& rangeTheta) {

    printChangeOfRange(__func__, "theta", m_rangeTheta, rangeTheta);
    m_rangeTheta = rangeTheta;
}

const std::pair<double, double>& TCSCrossSectionTotal::getRangeY() const {
    return m_rangeY;
}

void TCSCrossSectionTotal::setRangeY(const std::pair<double, double>& rangeY) {

    printChangeOfRange(__func__, "y", m_rangeY, rangeY);
    m_rangeY = rangeY;
}

const std::pair<double, double>& TCSCrossSectionTotal::getRangeQ2() const {
    return m_rangeQ2;
}

void TCSCrossSectionTotal::setRangeQ2(
        const std::pair<double, double>& rangeQ2) {

    printChangeOfRange(__func__, "Q2", m_rangeQ2, rangeQ2);
    m_rangeQ2 = rangeQ2;
}

double TCSCrossSectionTotal::getPhiEpsilon() const{
    return m_phiEpsilon; 
}

void TCSCrossSectionTotal::setPhiEpsilon(double phiEpsilon){

    info(__func__,
        ElemUtils::Formatter() << "Phi eps changed from " << m_phiEpsilon 
        << " to " << phiEpsilon);

    m_phiEpsilon = phiEpsilon;
}


} /* namespace PARTONS */

