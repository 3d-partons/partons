#include "../../../../include/partons/modules/gpd/GPDFit26.h"
#include "../../../../include/partons/modules/gpd/GPDFit26.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include <cmath>
#include <utility>

#include "../../../../include/partons/beans/collinear_distribution/CollinearDistributionKinematic.h"
#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/collinear_distribution/CollinearDistributionLHAPDF.h"

namespace PARTONS {

const unsigned int GPDFit26::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDFit26("GPDFit26"));

const std::string GPDFit26::PARAMETER_NAME_FIT26MODEL_UNPOLPDFSET = "unpol_pdf_set";
const std::string GPDFit26::PARAMETER_NAME_FIT26MODEL_UNPOLPDFREPLICA = "unpol_pdf_member";
const std::string GPDFit26::PARAMETER_NAME_FIT26MODEL_POLPDFSET = "pol_pdf_set";
const std::string GPDFit26::PARAMETER_NAME_FIT26MODEL_POLPDFREPLICA = "pol_pdf_member";

GPDFit26::GPDFit26(const std::string &className) : GPDModule(className) {

    m_MuF2_ref = 4.;

    m_listGPDComputeTypeAvailable.insert(
        std::make_pair(GPDType::H, &GPDModule::computeH));
    m_listGPDComputeTypeAvailable.insert(
        std::make_pair(GPDType::E, &GPDModule::computeE));
    m_listGPDComputeTypeAvailable.insert(
        std::make_pair(GPDType::Ht, &GPDModule::computeHt));
    m_listGPDComputeTypeAvailable.insert(
        std::make_pair(GPDType::Et, &GPDModule::computeEt));

    // pdfs
    m_unpolPDFSetName = "NNPDF30_nlo_as_0118";
    m_unpolPDFReplica = 0;
    m_pUnpolPDF = nullptr;

    m_polPDFSetName = "NNPDFpol11_100";
    m_polPDFReplica = 0;
    m_pPolPDF = nullptr;

    loadPDFs();

    // sset initial parameters.
    setParameters();
}

GPDFit26::GPDFit26(const GPDFit26& other) :
        GPDModule(other) {

    // pdfs
    m_unpolPDFSetName = other.m_unpolPDFSetName;
    m_unpolPDFReplica = other.m_unpolPDFReplica;
    m_pUnpolPDF = nullptr;

    m_polPDFSetName = other.m_polPDFSetName;
    m_polPDFReplica = other.m_polPDFReplica;
    m_pPolPDF = nullptr;

    loadPDFs();

    // set initial parameters.
    setParameters();
}

GPDFit26::~GPDFit26() {

    if (m_pUnpolPDF) {
        delete m_pUnpolPDF;
        m_pUnpolPDF = nullptr;
    }

    if (m_pPolPDF) {
        delete m_pPolPDF;
        m_pPolPDF = nullptr;
    }
}

void GPDFit26::loadPDFs() {

    if (m_pUnpolPDF) {
        delete m_pUnpolPDF;
        m_pUnpolPDF = nullptr;
    }

    if (m_pPolPDF) {
        delete m_pPolPDF;
        m_pPolPDF = nullptr;
    }

    info(__func__, ElemUtils::Formatter() << "loading unpolarized (polarized) pdf set/member: " << m_unpolPDFSetName << "/" << m_unpolPDFReplica <<
        " (" << m_polPDFSetName << "/" << m_polPDFReplica << ")");

    m_pUnpolPDF = LHAPDF::mkPDF(m_unpolPDFSetName, m_unpolPDFReplica);
    m_pPolPDF = LHAPDF::mkPDF(m_polPDFSetName, m_polPDFReplica);
}

GPDFit26* GPDFit26::clone() const {
    return new GPDFit26(*this);
}

void GPDFit26::configure(const ElemUtils::Parameters &parameters) {

    GPDModule::configure(parameters);

    bool reloadPDFs = false;

    if (parameters.isAvailable(PARAMETER_NAME_FIT26MODEL_UNPOLPDFSET)) {
        m_unpolPDFSetName = parameters.getLastAvailable().toString();
        reloadPDFs = true;
    }

    if (parameters.isAvailable(PARAMETER_NAME_FIT26MODEL_UNPOLPDFREPLICA)) {
        m_unpolPDFReplica = parameters.getLastAvailable().toInt();
        reloadPDFs = true;
    }

    if (parameters.isAvailable(PARAMETER_NAME_FIT26MODEL_POLPDFSET)) {
        m_polPDFSetName = parameters.getLastAvailable().toString();
        reloadPDFs = true;
    }

    if (parameters.isAvailable(PARAMETER_NAME_FIT26MODEL_POLPDFREPLICA)) {
        m_polPDFReplica = parameters.getLastAvailable().toInt();
        reloadPDFs = true;
    }

    if (reloadPDFs) loadPDFs();
}

void GPDFit26::setParameters(const std::vector<double>& p) {

    if (p.size() == 0) {

        m_par_H_uval_AmC_E_uval_A = 0.99;
        m_par_H_uval_B = -0.50;
        m_par_H_dval_AmC_E_dval_A = 0.70;
        m_par_H_dval_B = 0.47;

        m_par_E_uvaldval_alpha = 0.69;
        m_par_E_uval_B = -0.69;
        m_par_E_uval_C = -0.92;
        m_par_E_dval_B = -0.54;
        m_par_E_dval_C = -0.73;
    }
    else if (p.size() == 9) {

        m_par_H_uval_AmC_E_uval_A = p[0];
        m_par_H_uval_B = p[1];
        m_par_H_dval_AmC_E_dval_A = p[2];
        m_par_H_dval_B = p[3];

        m_par_E_uvaldval_alpha = p[4];
        m_par_E_uval_B = p[5];
        m_par_E_uval_C = p[6];
        m_par_E_dval_B = p[7];
        m_par_E_dval_C = p[8];
    }
    else {
        throw ElemUtils::CustomException(getClassName(), __func__, "wrong number of input parameters");
    }
}

PartonDistribution GPDFit26::computeH() {
m_xi=0;
    //check
    if (m_xi != m_x && m_xi != 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
            "this GPD model is only defined at xi = x or xi = 0");
    }

    //t-dependance
    auto slope = [](double x, double A, double B, double C) {
       return A * log(1 / x) + B * pow(1. - x, 2) + C * x * (1. - x);
    };

    //variables
    double uVal, uSea, dVal, dSea, s, g;
    double uValMx, dValMx;

    //get PDFs
    double absX = fabs(m_x);

    double pdf_u = m_pUnpolPDF->xfxQ2(2, absX, m_MuF2) / absX;
    double pdf_u_bar = m_pUnpolPDF->xfxQ2(-2, absX, m_MuF2) / absX;
    double pdf_u_val = pdf_u - pdf_u_bar;

    double pdf_d = m_pUnpolPDF->xfxQ2(1, absX, m_MuF2) / absX;
    double pdf_d_bar = m_pUnpolPDF->xfxQ2(-1, absX, m_MuF2) / absX;
    double pdf_d_val = pdf_d - pdf_d_bar;

    double pdf_s = m_pUnpolPDF->xfxQ2(3, absX, m_MuF2) / absX;

    //xi = 0 case
    if (m_xi == 0.) {

        uVal = pdf_u_val * exp(m_t * slope(m_x, m_par_H_uval_AmC_E_uval_A, m_par_H_uval_B, -1 * m_par_H_uval_AmC_E_uval_A));
        dVal = pdf_d_val * exp(m_t * slope(m_x, m_par_H_dval_AmC_E_dval_A, m_par_H_dval_B, -1 * m_par_H_dval_AmC_E_dval_A));
        uValMx = 0.;
        dValMx = 0.;

        uSea = 0.;
        dSea = 0.;
        s = 0.;

        g = 0.;

    }
    //xi = x case
    else {

        uVal = 0.;
        dVal = 0.;
        uValMx = 0.;
        dValMx = 0.;

        uSea = 0.;
        dSea = 0.;
        s = 0.;

        g = 0.;
    }

    //store
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);
    quarkDistribution_s.setQuarkDistribution(s);

    quarkDistribution_u.setQuarkDistributionPlus(uVal - uValMx + 2 * uSea);
    quarkDistribution_d.setQuarkDistributionPlus(dVal - dValMx + 2 * dSea);
    quarkDistribution_s.setQuarkDistributionPlus(2 * s);

    quarkDistribution_u.setQuarkDistributionMinus(uVal + uValMx);
    quarkDistribution_d.setQuarkDistributionMinus(dVal + dValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    GluonDistribution gluonDistribution(g);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    //return
    return partonDistribution;
}

PartonDistribution GPDFit26::computeE() {
    m_xi=0;
    //check
    if (m_xi != m_x && m_xi != 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
            "this GPD model is only defined at xi = x or xi = 0");
    }

    auto pdf = [](double x, double kappa, double alpha, double beta, double gamma) {

        double value = kappa * pow(x, -1 * alpha) * pow(1. - x, beta)
                * (1. + sqrt(x) * gamma);

        double norm =
                tgamma(1. + beta)
                        * ((tgamma(1 - alpha)) / tgamma(2. - alpha + beta)
                                + (gamma * tgamma(1.5 - alpha))
                                        / tgamma(2.5 - alpha + beta));

        //return
        return value / norm;
    };

    //t-dependance
    auto slope = [](double x, double A, double B, double C) {
       return A * log(1 / x) + B * pow(1. - x, 2) + C * x * (1. - x);
    };

    //variables
    double uVal, uSea, dVal, dSea, s, g;
    double uValMx, dValMx;

    //get PDFs
    double absX = fabs(m_x);

    double pdf_u_val = pdf(m_x,  1.67, m_par_E_uvaldval_alpha, 4.65, 4.);
    double pdf_d_val = pdf(m_x, -2.03, m_par_E_uvaldval_alpha, 5.25, 0.);

    //xi = 0 case
    if (m_xi == 0.) {

        uVal = pdf_u_val * exp(m_t * slope(m_x, m_par_H_uval_AmC_E_uval_A, m_par_E_uval_B, m_par_E_uval_C));
        dVal = pdf_d_val * exp(m_t * slope(m_x, m_par_H_uval_AmC_E_uval_A, m_par_E_dval_B, m_par_E_dval_C));
        uValMx = 0.;
        dValMx = 0.;

        uSea = 0.;
        dSea = 0.;
        s = 0.;

        g = 0.;
    }
    //xi = x case
    else {

        uVal = 0.;
        dVal = 0.;
        uValMx = 0.;
        dValMx = 0.;

        uSea = 0.;
        dSea = 0.;
        s = 0.;

        g = 0.;
    }

    //store
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);
    quarkDistribution_s.setQuarkDistribution(s);

    quarkDistribution_u.setQuarkDistributionPlus(uVal - uValMx + 2 * uSea);
    quarkDistribution_d.setQuarkDistributionPlus(dVal - dValMx + 2 * dSea);
    quarkDistribution_s.setQuarkDistributionPlus(2 * s);

    quarkDistribution_u.setQuarkDistributionMinus(uVal + uValMx);
    quarkDistribution_d.setQuarkDistributionMinus(dVal + dValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    GluonDistribution gluonDistribution(g);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    //return
    return partonDistribution;
}

PartonDistribution GPDFit26::computeHt() {

    //variables
    double uVal, uSea, dVal, dSea, sSea, g;
    double uValMx, dValMx;

    //store
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);
    quarkDistribution_s.setQuarkDistribution(sSea);

    quarkDistribution_u.setQuarkDistributionPlus(uVal - uValMx + 2 * uSea);
    quarkDistribution_d.setQuarkDistributionPlus(dVal - dValMx + 2 * dSea);
    quarkDistribution_s.setQuarkDistributionPlus(2 * sSea);

    quarkDistribution_u.setQuarkDistributionMinus(uVal + uValMx);
    quarkDistribution_d.setQuarkDistributionMinus(dVal + dValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    GluonDistribution gluonDistribution(g);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    //return
    return partonDistribution;
}

PartonDistribution GPDFit26::computeEt() {

    //variables
    double uVal, uSea, dVal, dSea, sSea, g;
    double uValMx, dValMx;

    //store
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);
    quarkDistribution_s.setQuarkDistribution(sSea);

    quarkDistribution_u.setQuarkDistributionPlus(uVal - uValMx + 2 * uSea);
    quarkDistribution_d.setQuarkDistributionPlus(dVal - dValMx + 2 * dSea);
    quarkDistribution_s.setQuarkDistributionPlus(2 * sSea);

    quarkDistribution_u.setQuarkDistributionMinus(uVal + uValMx);
    quarkDistribution_d.setQuarkDistributionMinus(dVal + dValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    GluonDistribution gluonDistribution(g);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    //return
    return partonDistribution;
}

} /* namespace PARTONS */
