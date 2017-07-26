/*
 * GPDBorderFunctionModule.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: Pawel Sznajder (IPNO)
 */

#include "../../../../include/partons/modules/gpd_border_function/GPDBorderFunctionModule.h"

namespace PARTONS {


GPDBorderFunctionModule::GPDBorderFunctionModule(const std::string &className) :
        GPDModule(className) {
}

GPDBorderFunctionModule::~GPDBorderFunctionModule() {
}

void GPDBorderFunctionModule::run() {
    GPDModule::run();
}

void GPDBorderFunctionModule::configure(
        const ElemUtils::Parameters& parameters) {
    GPDModule::configure(parameters);
}

PartonDistribution GPDBorderFunctionModule::compute(
        const GPDBorderFunctionKinematic& kinematic, GPDType gpdType,
        bool evolution) {
    return GPDModule::compute(static_cast<GPDKinematic>(kinematic), gpdType,
            evolution);
}

PartonDistribution GPDBorderFunctionModule::compute(double xi, double t,
        double MuF2, double MuR2, GPDType::Type gpdType, bool evolution) {
    return GPDModule::compute(xi, xi, t, MuF2, MuR2, gpdType, evolution);
}

PartonDistribution GPDBorderFunctionModule::computeH() {
    return GPDModule::computeH();
}

PartonDistribution GPDBorderFunctionModule::computeE() {
    return GPDModule::computeE();
}

PartonDistribution GPDBorderFunctionModule::computeHt() {
    return GPDModule::computeHt();
}

PartonDistribution GPDBorderFunctionModule::computeEt() {
    return GPDModule::computeEt();
}

std::string GPDBorderFunctionModule::toString() const{
    return GPDModule::toString();
}

GPDBorderFunctionModule::GPDBorderFunctionModule(
        const GPDBorderFunctionModule& other) :
        GPDModule(other) {
}

void GPDBorderFunctionModule::initModule() {
    GPDModule::initModule();
}

void GPDBorderFunctionModule::isModuleWellConfigured() {
    GPDModule::isModuleWellConfigured();
}

} /* namespace PARTONS */
