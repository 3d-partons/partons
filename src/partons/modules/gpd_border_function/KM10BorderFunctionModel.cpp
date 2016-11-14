/*
 * KM10BorderFunctionModel.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: Pawel Sznajder (IPNO)
 */

#include "../../../../include/partons/modules/gpd_border_function/KM10BorderFunctionModel.h"

#include <cmath>
#include <map>
#include <utility>

#include "../../../../include/partons/beans/gpd/GPDResult.h"
#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/gpd/GK11Model.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

const unsigned int KM10BorderFunctionModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new KM10BorderFunctionModel("KM10BorderFunctionModel"));

KM10BorderFunctionModel::KM10BorderFunctionModel(const std::string& className) :
        GPDBorderFunctionModule(className) {

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::E, &GPDModule::computeE));

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Ht, &GPDModule::computeHt));

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Et, &GPDModule::computeEt));

    m_pGK11Module = 0;

    m_par_H_b_val = 0.;
    m_par_H_r_val = 0.;
    m_par_H_M_val = 0.;

    m_par_H_b_sea = 0.;
    m_par_H_r_sea = 0.;
    m_par_H_M_sea = 0.;

    m_par_E_b_val = 0.;
    m_par_E_r_val = 0.;
    m_par_E_M_val = 0.;

    m_par_Ht_b_val = 0.;
    m_par_Ht_r_val = 0.;
    m_par_Ht_M_val = 0.;

    m_MuF2_ref = 4.;
}

KM10BorderFunctionModel::~KM10BorderFunctionModel() {
    //TODO delete m_pGK11Module?
}

KM10BorderFunctionModel* KM10BorderFunctionModel::clone() const {
    return new KM10BorderFunctionModel(*this);
}

void KM10BorderFunctionModel::resolveObjectDependencies() {
    m_pGK11Module =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    GK11Model::classId);
}

void KM10BorderFunctionModel::configure(
        const ElemUtils::Parameters& parameters) {
    GPDBorderFunctionModule::configure(parameters);
}

std::string KM10BorderFunctionModel::toString() {
    GPDBorderFunctionModule::toString();
}

PartonDistribution KM10BorderFunctionModel::computeH() {

    //parameters
    double r, alpha, b, M, p;

    //fL
    double fL = log(m_MuF2 / m_MuF2_ref);

    //valence
    //from GK11
    double t_alpha_val = 0.9 - 0.05 * fL;
    double t_delta_val = 0.48 + 0.006 * fL;

    r = m_par_H_r_val;
    alpha = t_delta_val + t_alpha_val * m_t;
    b = m_par_H_b_val;
    M = m_par_H_M_val;
    p = 1;

    double GPD_Hval = getDiagonalGPD(m_xi, r, alpha, b, M, p);

    //sea
    //from GK11
    double t_alpha_sea = 0.15;
    double t_delta_sea = 1.1 + 0.06 * fL - 0.0027 * fL * fL;

    //r = 1. in https://arxiv.org/pdf/0904.0458v2.pdf but gives wrong result and is not compatible with Fig. 15, r = 0.22 seems to be adequate
    r = m_par_H_r_sea;
    alpha = t_delta_sea + t_alpha_sea * m_t;
    b = m_par_H_b_sea;
    M = m_par_H_M_sea;
    p = 2;

    double GPD_Hsea = getDiagonalGPD(m_xi, r, alpha, b, M, p);

    //store results
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    //take into account the forward limit where uval(x) approx. 2*dval(x)
    quarkDistribution_u.setQuarkDistribution(2 * GPD_Hval + GPD_Hsea);
    quarkDistribution_d.setQuarkDistribution(GPD_Hval + GPD_Hsea);
    quarkDistribution_s.setQuarkDistribution(0.);

    quarkDistribution_u.setQuarkDistributionPlus(2 * GPD_Hval + GPD_Hsea);
    quarkDistribution_d.setQuarkDistributionPlus(GPD_Hval + GPD_Hsea);
    quarkDistribution_s.setQuarkDistributionPlus(0.);

    quarkDistribution_u.setQuarkDistributionMinus(2 * GPD_Hval + GPD_Hsea);
    quarkDistribution_d.setQuarkDistributionMinus(GPD_Hval + GPD_Hsea);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    GluonDistribution gluonDistribution(0.);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    //return
    return partonDistribution;
}

PartonDistribution KM10BorderFunctionModel::computeE() {

    //parameters
    double r, alpha, b, M, p;

    //valence
    //from GK11
    double t_alpha_val = 0.9;
    double t_delta_val = 0.48;

    r = m_par_E_r_val;
    alpha = t_delta_val + t_alpha_val * m_t;
    b = m_par_E_b_val;
    M = m_par_E_M_val;
    p = 1;

    double kappa_u = 1.67;
    double kappa_d = -2.03;

    double GPD_Eval = getDiagonalGPD(m_xi, r, alpha, b, M, p);
    double GPD_Esea = 0.;

    //store results
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    //take into account the forward limit -> anomalous magnetic moments
    quarkDistribution_u.setQuarkDistribution(kappa_u * GPD_Eval + GPD_Esea);
    quarkDistribution_d.setQuarkDistribution(kappa_d * GPD_Eval + GPD_Esea);
    quarkDistribution_s.setQuarkDistribution(0.);

    quarkDistribution_u.setQuarkDistributionPlus(kappa_u * GPD_Eval + GPD_Esea);
    quarkDistribution_d.setQuarkDistributionPlus(kappa_d * GPD_Eval + GPD_Esea);
    quarkDistribution_s.setQuarkDistributionPlus(0.);

    quarkDistribution_u.setQuarkDistributionMinus(
            kappa_u * GPD_Eval + GPD_Esea);
    quarkDistribution_d.setQuarkDistributionMinus(
            kappa_d * GPD_Eval + GPD_Esea);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    GluonDistribution gluonDistribution(0.);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    //return
    return partonDistribution;
}

PartonDistribution KM10BorderFunctionModel::computeHt() {

    //parameters
    double r, alpha, b, M, p;

    //valence
    double t_alpha_val = 0.45;
    double t_delta_val = 0.48;

    r = m_par_Ht_r_val;
    alpha = t_delta_val + t_alpha_val * m_t;
    b = m_par_Ht_b_val;
    M = m_par_Ht_M_val;
    p = 1;

    double firstMoment_u = 0.928;
    double firstMoment_d = -0.342;

    double GPD_Htval = getDiagonalGPD(m_xi, r, alpha, b, M, p);
    double GPD_Htsea = 0.;

    //store results
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    //take into account the forward limit -> moments of polarized PDFs from https://arxiv.org/pdf/1208.5234.pdf
    quarkDistribution_u.setQuarkDistribution(
            firstMoment_u * GPD_Htval + GPD_Htsea);
    quarkDistribution_d.setQuarkDistribution(
            firstMoment_d * GPD_Htval + GPD_Htsea);
    quarkDistribution_s.setQuarkDistribution(0.);

    quarkDistribution_u.setQuarkDistributionPlus(
            firstMoment_u * GPD_Htval + GPD_Htsea);
    quarkDistribution_d.setQuarkDistributionPlus(
            firstMoment_d * GPD_Htval + GPD_Htsea);
    quarkDistribution_s.setQuarkDistributionPlus(0.);

    quarkDistribution_u.setQuarkDistributionMinus(
            firstMoment_u * GPD_Htval + GPD_Htsea);
    quarkDistribution_d.setQuarkDistributionMinus(
            firstMoment_d * GPD_Htval + GPD_Htsea);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    GluonDistribution gluonDistribution(0.);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    //return
    return partonDistribution;
}

PartonDistribution KM10BorderFunctionModel::computeEt() {

    //return
    return m_pGK11Module->compute(m_xi, m_xi, m_t, m_MuF2, m_MuR2, GPDType::Et);
}

double KM10BorderFunctionModel::getDiagonalGPD(double xi, double r,
        double alpha, double b, double M, double p) const {
    return r / (1. + xi) * pow(2 * xi / (1 + xi), -1 * alpha)
            * pow((1. - xi) / (1. + xi), b)
            * pow(1. - (1. - xi) / (1. + xi) * (m_t / pow(M, 2)), -1 * p);
}

double KM10BorderFunctionModel::getParEBVal() const {
    return m_par_E_b_val;
}

void KM10BorderFunctionModel::setParEBVal(double parEBVal) {
    m_par_E_b_val = parEBVal;
}

double KM10BorderFunctionModel::getParEMVal() const {
    return m_par_E_M_val;
}

void KM10BorderFunctionModel::setParEMVal(double parEMVal) {
    m_par_E_M_val = parEMVal;
}

double KM10BorderFunctionModel::getParERVal() const {
    return m_par_E_r_val;
}

void KM10BorderFunctionModel::setParERVal(double parERVal) {
    m_par_E_r_val = parERVal;
}

double KM10BorderFunctionModel::getParHBSea() const {
    return m_par_H_b_sea;
}

void KM10BorderFunctionModel::setParHBSea(double parHBSea) {
    m_par_H_b_sea = parHBSea;
}

double KM10BorderFunctionModel::getParHBVal() const {
    return m_par_H_b_val;
}

void KM10BorderFunctionModel::setParHBVal(double parHBVal) {
    m_par_H_b_val = parHBVal;
}

double KM10BorderFunctionModel::getParHMSea() const {
    return m_par_H_M_sea;
}

void KM10BorderFunctionModel::setParHMSea(double parHMSea) {
    m_par_H_M_sea = parHMSea;
}

double KM10BorderFunctionModel::getParHMVal() const {
    return m_par_H_M_val;
}

void KM10BorderFunctionModel::setParHMVal(double parHMVal) {
    m_par_H_M_val = parHMVal;
}

double KM10BorderFunctionModel::getParHRSea() const {
    return m_par_H_r_sea;
}

void KM10BorderFunctionModel::setParHRSea(double parHRSea) {
    m_par_H_r_sea = parHRSea;
}

double KM10BorderFunctionModel::getParHRVal() const {
    return m_par_H_r_val;
}

void KM10BorderFunctionModel::setParHRVal(double parHRVal) {
    m_par_H_r_val = parHRVal;
}

double KM10BorderFunctionModel::getParHtBVal() const {
    return m_par_Ht_b_val;
}

void KM10BorderFunctionModel::setParHtBVal(double parHtBVal) {
    m_par_Ht_b_val = parHtBVal;
}

double KM10BorderFunctionModel::getParHtMVal() const {
    return m_par_Ht_M_val;
}

void KM10BorderFunctionModel::setParHtMVal(double parHtMVal) {
    m_par_Ht_M_val = parHtMVal;
}

double KM10BorderFunctionModel::getParHtRVal() const {
    return m_par_Ht_r_val;
}

void KM10BorderFunctionModel::setParHtRVal(double parHtRVal) {
    m_par_Ht_r_val = parHtRVal;
}
