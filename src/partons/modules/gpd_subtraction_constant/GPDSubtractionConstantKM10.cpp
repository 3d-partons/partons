#include "../../../../include/partons/modules/gpd_subtraction_constant/GPDSubtractionConstantKM10.h"

#include <cmath>
#include <utility>

#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"

const unsigned int GPDSubtractionConstantKM10::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDSubtractionConstantKM10(
                        "GPDSubtractionConstantKM10"));

GPDSubtractionConstantKM10::GPDSubtractionConstantKM10(
        const std::string& className) :
        GPDSubtractionConstantModule(className) {

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H,
                    &GPDSubtractionConstantModule::computeH));

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::E,
                    &GPDSubtractionConstantModule::computeE));

    m_par_H_C = 6.;          //Eq. (112)
    m_par_H_M_sub = 1.5;     //Eq. (112)
}

GPDSubtractionConstantKM10::~GPDSubtractionConstantKM10() {
}

GPDSubtractionConstantKM10* GPDSubtractionConstantKM10::clone() const {
    return new GPDSubtractionConstantKM10(*this);
}

GPDSubtractionConstantKM10::GPDSubtractionConstantKM10(
        const GPDSubtractionConstantKM10& other) :
        GPDSubtractionConstantModule(other) {

    m_par_H_C = other.m_par_H_C;
    m_par_H_M_sub = other.m_par_H_M_sub;
}

double GPDSubtractionConstantKM10::computeH() {
    return m_par_H_C * pow(1. - m_t * pow(m_par_H_M_sub, -2), -2);
}

double GPDSubtractionConstantKM10::computeE() {
    return -1 * computeH();
}
