/*
 * KM10BorderFunctionModel.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: Pawel Sznajder (IPNO)
 */

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
#include "../../../../include/partons/modules/gpd/GPDGK11.h"
#include "../../../../include/partons/modules/gpd_border_function/GPDBorderFunctionKM10.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

const unsigned int GPDBorderFunctionKM10::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDBorderFunctionKM10("GPDBorderFunctionKM10"));

GPDBorderFunctionKM10::GPDBorderFunctionKM10(const std::string& className) :
        GPDBorderFunctionModule(className) {

    m_par_H_b_val = 2.4;        //Eq. (112)
    m_par_H_r_val = 1.11;       //Eq. (112)
    m_par_H_M_val = sqrt(0.64); //Eq. (109)

    m_par_H_b_sea = 4.6;        //Eq. (112)
    m_par_H_r_sea = 1.;         //Eq. (108)
    m_par_H_M_sea = sqrt(0.5);  //Eq. (108)

    m_par_H_n_val = 1.35;       //Eq. (109)
    m_par_H_delta_val = 0.43;   //Eq. (109)
    m_par_H_alpha_val = 0.85;   //Eq. (109)
    m_par_H_p_val = 1;          //Eq. (109)

    //n = 1.5 in https://arxiv.org/pdf/0904.0458v2.pdf but gives wrong result and is not compatible with Fig. 15, n = 0.32 seems to be adequate
    m_par_H_n_sea = 0.32;       //Eq. (108)
    m_par_H_delta_sea = 1.13;   //Eq. (108)
    m_par_H_alpha_sea = 0.15;   //Eq. (108)
    m_par_H_p_sea = 2;          //Eq. (108)

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));

    m_MuF2_ref = 2.;        //does not depend of Q2, muF2_ref set here arbitrary
}

GPDBorderFunctionKM10::GPDBorderFunctionKM10(
        const GPDBorderFunctionKM10& other) :
        GPDBorderFunctionModule(other) {

    m_par_H_b_val = other.m_par_H_b_val;
    m_par_H_r_val = other.m_par_H_r_val;
    m_par_H_M_val = other.m_par_H_M_val;

    m_par_H_b_sea = other.m_par_H_b_sea;
    m_par_H_r_sea = other.m_par_H_r_sea;
    m_par_H_M_sea = other.m_par_H_M_sea;

    m_par_H_n_val = other.m_par_H_n_val;
    m_par_H_delta_val = other.m_par_H_delta_val;
    m_par_H_alpha_val = other.m_par_H_alpha_val;
    m_par_H_p_val = other.m_par_H_p_val;

    m_par_H_n_sea = other.m_par_H_n_sea;
    m_par_H_delta_sea = other.m_par_H_delta_sea;
    m_par_H_alpha_sea = other.m_par_H_alpha_sea;
    m_par_H_p_sea = other.m_par_H_p_sea;
}

GPDBorderFunctionKM10::~GPDBorderFunctionKM10() {
}

GPDBorderFunctionKM10* GPDBorderFunctionKM10::clone() const {
    return new GPDBorderFunctionKM10(*this);
}

void GPDBorderFunctionKM10::resolveObjectDependencies() {
}

void GPDBorderFunctionKM10::configure(
        const ElemUtils::Parameters& parameters) {
    GPDBorderFunctionModule::configure(parameters);
}

std::string GPDBorderFunctionKM10::toString() const {
    GPDBorderFunctionModule::toString();
}

PartonDistribution GPDBorderFunctionKM10::computeH() {

    //parameters
    double n, r, alpha, b, M, p;

    //valence
    n = m_par_H_n_val;
    r = m_par_H_r_val;
    alpha = m_par_H_delta_val + m_par_H_alpha_val * m_t;
    b = m_par_H_b_val;
    M = m_par_H_M_val;
    p = m_par_H_p_val;

    double GPD_Hval = getDiagonalGPD(m_xi, n, r, alpha, b, M, p);

    //sea
    n = m_par_H_n_sea;
    r = m_par_H_r_sea;
    alpha = m_par_H_delta_sea + m_par_H_alpha_sea * m_t;
    b = m_par_H_b_sea;
    M = m_par_H_M_sea;
    p = m_par_H_p_sea;

    double GPD_Hsea = getDiagonalGPD(m_xi, n, r, alpha, b, M, p);

    //store results
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_u.setQuarkDistribution(2 * GPD_Hval + GPD_Hsea);
    quarkDistribution_d.setQuarkDistribution(GPD_Hval + GPD_Hsea);
    quarkDistribution_s.setQuarkDistribution(0.);

    quarkDistribution_u.setQuarkDistributionPlus(2 * GPD_Hval + 2 * GPD_Hsea);
    quarkDistribution_d.setQuarkDistributionPlus(GPD_Hval + 2 * GPD_Hsea);
    quarkDistribution_s.setQuarkDistributionPlus(0.);

    quarkDistribution_u.setQuarkDistributionMinus(2 * GPD_Hval);
    quarkDistribution_d.setQuarkDistributionMinus(GPD_Hval);
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

double GPDBorderFunctionKM10::getDiagonalGPD(double xi, double n, double r,
        double alpha, double b, double M, double p) const {
    return n * r / (1. + xi) * pow(2 * xi / (1 + xi), -1 * alpha)
            * pow((1. - xi) / (1. + xi), b)
            * pow(1. - (1. - xi) / (1. + xi) * (m_t / pow(M, 2)), -1 * p);
}
