/*
 * GPDSubtractionConstantModule.cpp
 *
 *  Created on: Oct 18, 2016
 *      Author: debian
 */

#include "../../../../include/partons/modules/gpd_subtraction_constant/GPDSubtractionConstantModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <utility>

namespace PARTONS {


const std::string GPDSubtractionConstantModule::GPD_SUBTRACTION_CONSTANT_MODULE_CLASS_NAME =
		"GPDSubtractionConstantModule";

GPDSubtractionConstantModule::GPDSubtractionConstantModule(
		const std::string& className) :
		ModuleObject(className), m_xi(0.), m_t(0.), m_MuF2(0.), m_MuR2(0.), m_gpdType(
				GPDType::ALL) {
}

GPDSubtractionConstantModule::GPDSubtractionConstantModule(
		const GPDSubtractionConstantModule& other) :
		ModuleObject(other) {

	m_xi = other.m_xi;
	m_t = other.m_t;
	m_MuF2 = other.m_MuF2;
	m_MuR2 = other.m_MuR2;
	m_gpdType = other.m_gpdType;

	m_listGPDComputeTypeAvailable = other.m_listGPDComputeTypeAvailable;
	m_it = other.m_it;
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

	if (m_xi > 1. || m_xi < 0.) {
		warn(__func__,
				ElemUtils::Formatter()
						<< "Skewness should be in [0., +1.] m_xi = " << m_xi);
	}

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

void GPDSubtractionConstantModule::preCompute(double xi, double t, double MuF2,
		double MuR2, GPDType::Type gpdType) {

	//copy variables
	m_xi = xi;
	m_t = t;
	m_MuF2 = MuF2;
	m_MuR2 = MuR2;
	m_gpdType = gpdType;

	//initialize
	initModule();

	//check if well configured
	isModuleWellConfigured();
}

double GPDSubtractionConstantModule::compute(double xi, double t, double MuF2,
		double MuR2, GPDType::Type gpdType) {

	//pre compute
	preCompute(xi, t, MuF2, MuR2, gpdType);

	//result
	double result;

	//check if available
	m_it = m_listGPDComputeTypeAvailable.find(gpdType);

	if (m_it != m_listGPDComputeTypeAvailable.end()) {

		//execute
		result = ((*this).*(m_it->second))();
	} else {

		//print error
		throw ElemUtils::CustomException(getClassName(), __func__,
				ElemUtils::Formatter() << "GPD(" << GPDType(gpdType).toString()
						<< ") is not available for this GPD model");
	}

	//return
	return result;
}

double GPDSubtractionConstantModule::compute(
		const GPDBorderFunctionKinematic& kinematic, GPDType gpdType) {
	return compute(kinematic.getXi(), kinematic.getT(), kinematic.getMuF2(),
			kinematic.getMuR2(), gpdType.getType());
}

double GPDSubtractionConstantModule::computeH() {

	throw ElemUtils::CustomException(getClassName(), __func__,
			ElemUtils::Formatter() << "Definition in mother class undefined");

	return 0.;
}

double GPDSubtractionConstantModule::computeE() {

	throw ElemUtils::CustomException(getClassName(), __func__,
			ElemUtils::Formatter() << "Definition in mother class undefined");

	return 0.;
}

double GPDSubtractionConstantModule::computeHt() {

	throw ElemUtils::CustomException(getClassName(), __func__,
			ElemUtils::Formatter() << "Definition in mother class undefined");

	return 0.;
}

double GPDSubtractionConstantModule::computeEt() {

	throw ElemUtils::CustomException(getClassName(), __func__,
			ElemUtils::Formatter() << "Definition in mother class undefined");

	return 0.;
}

List<GPDType> GPDSubtractionConstantModule::getListOfAvailableGPDTypeForComputation() const {

	std::map<GPDType::Type, double (GPDSubtractionConstantModule::*)()>::const_iterator it;
	List<GPDType> listOfAvailableGPDTypeForComputation;

	for (it = m_listGPDComputeTypeAvailable.begin();
			it != m_listGPDComputeTypeAvailable.end(); it++) {
		listOfAvailableGPDTypeForComputation.add(it->first);
	}

	return listOfAvailableGPDTypeForComputation;
}


} /* namespace PARTONS */
