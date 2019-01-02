/*
 * GPDSubtractionConstantModule.cpp
 *
 *  Created on: Oct 18, 2016
 *      Author: debian
 */

#include "../../../../include/partons/modules/gpd_subtraction_constant/GPDSubtractionConstantModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>

namespace PARTONS {

const std::string GPDSubtractionConstantModule::GPD_SUBTRACTION_CONSTANT_MODULE_CLASS_NAME =
        "GPDSubtractionConstantModule";

GPDSubtractionConstantModule::GPDSubtractionConstantModule(
        const std::string& className) :
        ModuleObject(className, ChannelType::UNDEFINED), m_t(0.), m_MuF2(0.), m_MuR2(0.) {
}

GPDSubtractionConstantModule::GPDSubtractionConstantModule(
        const GPDSubtractionConstantModule& other) :
        ModuleObject(other) {

    m_t = other.m_t;
    m_MuF2 = other.m_MuF2;
    m_MuR2 = other.m_MuR2;
}

GPDSubtractionConstantModule::~GPDSubtractionConstantModule() {
}

GPDSubtractionConstantModule* GPDSubtractionConstantModule::clone() const {
    return new GPDSubtractionConstantModule(*this);
}

void GPDSubtractionConstantModule::configure(
        const ElemUtils::Parameters& parameters) {
    ModuleObject::configure(parameters);
}

std::string GPDSubtractionConstantModule::toString() const {
    return ModuleObject::toString();
}

void GPDSubtractionConstantModule::resolveObjectDependencies() {
    ModuleObject::resolveObjectDependencies();
}

void GPDSubtractionConstantModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    ModuleObject::prepareSubModules(subModulesData);
}

void GPDSubtractionConstantModule::initModule() {
}

void GPDSubtractionConstantModule::isModuleWellConfigured() {

    if (m_t > 0.) {
        warn(__func__,
                ElemUtils::Formatter()
                        << "Nucleon momentum transfer should be <= 0. m_t = "
                        << m_t);
    }

    if (m_MuF2 < 0.) {
        warn(__func__,
                ElemUtils::Formatter()
                        << "Square of factorization scale should be > 0. m_MuF2 = "
                        << m_MuF2);
    }

    if (m_MuR2 < 0.) {
        warn(__func__,
                ElemUtils::Formatter()
                        << "Square of renormalization scale should be > 0. m_MuR2"
                        << m_MuR2);
    }
}

void GPDSubtractionConstantModule::preCompute(double t, double MuF2,
        double MuR2) {

    //copy variables
    m_t = t;
    m_MuF2 = MuF2;
    m_MuR2 = MuR2;

    //initialize
    initModule();

    //check if well configured
    isModuleWellConfigured();
}

double GPDSubtractionConstantModule::compute(double t, double MuF2,
        double MuR2) {

    //pre compute
    preCompute(t, MuF2, MuR2);

    //compute and return
    return computeSubtractionConstant();
}

double GPDSubtractionConstantModule::compute(
        const GPDBorderFunctionKinematic& gpdBorderFunctionKinematic) {
    return compute(gpdBorderFunctionKinematic.getT(),
            gpdBorderFunctionKinematic.getMuF2(),
            gpdBorderFunctionKinematic.getMuR2());
}

double GPDSubtractionConstantModule::computeSubtractionConstant() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Not defined for the abstract class");
}

} /* namespace PARTONS */
