#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <include/partons/BaseObjectRegistry.h>
#include <include/partons/modules/gpd_subtraction_constant/GPDSubtractionConstantDLMSTW21.h>
#include <include/partons/modules/gpd_subtraction_constant/GPDSubtractionConstantDLMSTW21Evolution.h>
#include <include/partons/modules/gpd_subtraction_constant/GPDSubtractionConstantDLMSTW21Replicas.h>
#include <include/partons/utils/type/PhysicalUnit.h>
#include <cmath>
#include <iterator>

namespace PARTONS {

const std::string GPDSubtractionConstantDLMSTW21::PARAMETER_NAME_REPLICA =
		"replica";

const unsigned int GPDSubtractionConstantDLMSTW21::classId =
		BaseObjectRegistry::getInstance()->registerBaseObject(
				new GPDSubtractionConstantDLMSTW21(
						"GPDSubtractionConstantDLMSTW21"));

GPDSubtractionConstantDLMSTW21::GPDSubtractionConstantDLMSTW21(
		const std::string& className) :
		GPDSubtractionConstantModule(className), m_muF20(0.1), m_M(0.8), m_alpha(
				3.), m_replica(0), m_d1g(0.), m_d1q(0.) {

	//by default load for 0-th replica
	loadParameters(0, false);
}

GPDSubtractionConstantDLMSTW21::~GPDSubtractionConstantDLMSTW21() {
}

GPDSubtractionConstantDLMSTW21* GPDSubtractionConstantDLMSTW21::clone() const {
	return new GPDSubtractionConstantDLMSTW21(*this);
}

GPDSubtractionConstantDLMSTW21::GPDSubtractionConstantDLMSTW21(
		const GPDSubtractionConstantDLMSTW21& other) :
		GPDSubtractionConstantModule(other), m_muF20(other.m_muF20), m_M(
				other.m_M), m_alpha(other.m_alpha), m_replica(other.m_replica), m_d1g(
				other.m_d1g), m_d1q(other.m_d1q) {
}

void GPDSubtractionConstantDLMSTW21::configure(
		const ElemUtils::Parameters &parameters) {

	GPDSubtractionConstantModule::configure(parameters);

	if (parameters.isAvailable(PARAMETER_NAME_REPLICA)) {
		loadParameters(parameters.getLastAvailable().toUInt());
	}
}

void GPDSubtractionConstantDLMSTW21::loadParameters(size_t replica,
		bool printInfo) {

	if (replica >= GPDSubtractionConstantDLMSTW21Replicas::c_nReplicas) {
		throw ElemUtils::CustomException(getClassName(), __func__,
				ElemUtils::Formatter() << "Illegal replica " << replica);
	}

	m_d1g = GPDSubtractionConstantDLMSTW21Replicas::c_Replicas[replica][0];
	m_d1q = GPDSubtractionConstantDLMSTW21Replicas::c_Replicas[replica][1];

	if (printInfo) {
		info(__func__,
				ElemUtils::Formatter() << "Parameters set for replica "
						<< replica);
	}
}

PhysicalType<double> GPDSubtractionConstantDLMSTW21::computeSubtractionConstant() {

	//charges u,d,s,c,b,t
	std::vector<double> charges(6);

	charges.push_back(2 / 3.);	//u
	charges.push_back(-1 / 3.);	//d
	charges.push_back(-1 / 3.);	//s
	charges.push_back(2 / 3.);	//c
	charges.push_back(-1 / .3);	//b
	charges.push_back(2 / 3.);	//t

	//d1 at initial scale
	std::vector<double> d1(4);

	d1.push_back(m_d1g);	//g
	d1.push_back(m_d1q);	//u
	d1.push_back(m_d1q);	//d
	d1.push_back(m_d1q);	//s

	//evolve
	std::vector<double> d1Evolved =
			GPDSubtractionConstantDLMSTW21Evolution::evolveQuarkGluon(m_MuF2,
					m_muF20, d1, 1);

	//evaluate (gluons are skipped)
	double dTermFormFactor = 0.;

	for (size_t i = 1; i < d1Evolved.size(); i++)
		dTermFormFactor += pow(charges.at(i - 1), 2) * d1Evolved.at(i);

	//t-dependence
	double tDep = pow(1. - m_t / pow(m_M, 2), -1 * m_alpha);

	//this takes into account that SC = 4 * dTermFormFactor
	double sCdTermFactor = 4.;

	//return
	return PhysicalType<double>(sCdTermFactor * dTermFormFactor * tDep,
			PhysicalUnit::NONE);
}

double GPDSubtractionConstantDLMSTW21::getMean(
		const std::vector<double>& v) const {

	if (v.size() == 0) {
		throw ElemUtils::CustomException(getClassName(), __func__,
				"vector size is 0");
	}

	double mean = 0.;

	for (int i = 0; i < v.size(); i++) {
		mean += v.at(i);
	}

	return mean / v.size();
}

double GPDSubtractionConstantDLMSTW21::getSigma(
		const std::vector<double>& v) const {

	if (v.size() == 0) {
		throw ElemUtils::CustomException(getClassName(), __func__,
				"vector size is 0");
	}

	double mean = getMean(v);

	double sigma = 0.;

	for (int i = 0; i < v.size(); i++) {
		sigma += pow(mean - v.at(i), 2);
	}

	return sqrt(sigma / double(v.size()));
}

size_t GPDSubtractionConstantDLMSTW21::removeOutliers(
		std::vector<double>& v) const {

	if (v.size() == 0) {
		throw ElemUtils::CustomException(getClassName(), __func__,
				"vector size is 0");
	}

	double meanData = getMean(v);
	double sigmaData = getSigma(v);

	if (sigmaData == 0.) {

		warn(__func__, "sigma size is 0");
		return 0;
	}

	std::vector<double> result;
	std::vector<double>::iterator it;
	size_t nRemoved = 0;

	for (it = v.begin(); it != v.end(); it++) {

		if (fabs((*it) - meanData) / sigmaData > 3.) {
			nRemoved++;
		} else {
			result.push_back(*it);
		}
	}

	v = result;

	if (nRemoved != 0)
		nRemoved += removeOutliers(v);

	return nRemoved;
}

void GPDSubtractionConstantDLMSTW21::getMeanAndUncertainty(
		const std::vector<double>& v, double& mean, double& unc) const {

	std::vector<double> vOutlierFree = v;
	removeOutliers(vOutlierFree);

	mean = getMean(vOutlierFree);
	unc = getSigma(vOutlierFree);
}

} /* namespace PARTONS */
