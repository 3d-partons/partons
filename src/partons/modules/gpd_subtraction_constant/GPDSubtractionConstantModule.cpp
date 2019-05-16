/*
 * GPDSubtractionConstantModule.cpp
 *
 *  Created on: Oct 18, 2016
 *      Author: debian
 */

#include "../../../../include/partons/modules/gpd_subtraction_constant/GPDSubtractionConstantModule.h"

#include <ElementaryUtils/logger/CustomException.h>

#include "../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../include/partons/beans/gpd/GPDSubtractionConstantResult.h"
#include "../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string GPDSubtractionConstantModule::GPD_SUBTRACTION_CONSTANT_MODULE_CLASS_NAME =
        "GPDSubtractionConstantModule";

GPDSubtractionConstantModule::GPDSubtractionConstantModule(
        const std::string& className) :
        ModuleObject(className, ChannelType::UNDEFINED), m_t(0.), m_MuF2(0.), m_MuR2(
                0.), m_currentGPDComputeType(GPDType::UNDEFINED) {
}

GPDSubtractionConstantModule::GPDSubtractionConstantModule(
        const GPDSubtractionConstantModule& other) :
        ModuleObject(other) {

    m_t = other.m_t;
    m_MuF2 = other.m_MuF2;
    m_MuR2 = other.m_MuR2;
    m_currentGPDComputeType = other.m_currentGPDComputeType;
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
        warn(__func__, "Nucleon momentum transfer should be <= 0.");
    }

    if (m_MuF2 <= 0.) {
        warn(__func__, "Square of factorization scale should be > 0.");
    }

    if (m_MuR2 <= 0.) {
        warn(__func__, "Square of renormalization scale should be > 0.");
    }
}

void GPDSubtractionConstantModule::setKinematics(
        const GPDKinematic& kinematic) {

    m_t = kinematic.getT().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_MuF2 = kinematic.getMuF2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_MuR2 = kinematic.getMuR2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
}

void GPDSubtractionConstantModule::setCurrentGPDType(GPDType::Type gpdType) {
    m_currentGPDComputeType = gpdType;
}

GPDSubtractionConstantResult GPDSubtractionConstantModule::compute(
        const GPDKinematic& kinematic, GPDType::Type gpdType) {

    //reset kinematics (virtuality)
    setKinematics(kinematic);

    //set GPD type
    setCurrentGPDType(gpdType);

    //execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    GPDSubtractionConstantResult result(computeSubtractionConstant(),
            kinematic);

    //set module name
    result.setComputationModuleName(getClassName());

    //return
    return result;
}

PhysicalType<double> GPDSubtractionConstantModule::computeSubtractionConstant() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Not defined for the abstract class");
}

} /* namespace PARTONS */
