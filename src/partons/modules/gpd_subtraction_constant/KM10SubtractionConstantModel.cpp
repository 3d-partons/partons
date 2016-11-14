/*
 * KM10SubtractionConstantModel.cpp
 *
 *  Created on: Oct 18, 2016
 *      Author: Pawel Sznajder (IPNO)
 */

#include "../../../../include/partons/modules/gpd_subtraction_constant/KM10SubtractionConstantModel.h"

#include <cmath>
#include <map>
#include <utility>

#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"

const unsigned int KM10SubtractionConstantModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new KM10SubtractionConstantModel(
                        "KM10SubtractionConstantModel"));

KM10SubtractionConstantModel::KM10SubtractionConstantModel(
        const std::string& className) :
        GPDSubtractionConstantModule(className) {

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H,
                    &GPDSubtractionConstantModule::computeH));

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::E,
                    &GPDSubtractionConstantModule::computeE));

    m_par_H_C = 0.;
    m_par_H_M_sub = 0.;
}

KM10SubtractionConstantModel::~KM10SubtractionConstantModel() {
}

KM10SubtractionConstantModel* KM10SubtractionConstantModel::clone() const {
    return new KM10SubtractionConstantModel(*this);
}

KM10SubtractionConstantModel::KM10SubtractionConstantModel(
        const KM10SubtractionConstantModel& other) :
        GPDSubtractionConstantModule(other) {

    m_par_H_C = other.m_par_H_C;
    m_par_H_M_sub = other.m_par_H_M_sub;
}

double KM10SubtractionConstantModel::computeH() {
    return m_par_H_C * pow(1. - m_t * pow(m_par_H_M_sub, -2), -2);
}

double KM10SubtractionConstantModel::computeE() {
    return -1 * computeH();
}

double KM10SubtractionConstantModel::getParHC() const {
    return m_par_H_C;
}

void KM10SubtractionConstantModel::setParHC(double parHC) {
    m_par_H_C = parHC;
}

double KM10SubtractionConstantModel::getParHMSub() const {
    return m_par_H_M_sub;
}

void KM10SubtractionConstantModel::setParHMSub(double parHMSub) {
    m_par_H_M_sub = parHMSub;
}
