#include "GPDQuarkFlavorData.h"

#include <sstream>

#include "../QuarkFlavor.h"

GPDQuarkFlavorData::GPDQuarkFlavorData(GPDComputeType::Type _gpdComputeType,
		QuarkFlavor::Type _quarkFlavorType) :
		m_gpdComputeType(_gpdComputeType), m_quarkFlavor(_quarkFlavorType), m_partonDistribution(
				0.), m_partonDistributionMinus(0.), m_partonDistributionPlus(
				0.), valence(0.), sea(0.), m_partonDistributionSinglet(0.), quark(
				0.), antiQuark(0.) {
}

GPDQuarkFlavorData::~GPDQuarkFlavorData() {
}

std::string GPDQuarkFlavorData::toString() {

	std::ostringstream os;

	std::string gpdComputeTypeName = m_gpdComputeType.toString();
	std::string quarkFlavorTypeShortName = m_quarkFlavor.getShortName();

	os << gpdComputeTypeName << quarkFlavorTypeShortName << " = "
			<< m_partonDistribution << std::endl;
	os << gpdComputeTypeName << quarkFlavorTypeShortName << "Val = " << valence
			<< std::endl;
	os << gpdComputeTypeName << quarkFlavorTypeShortName << "Sea = " << sea
			<< std::endl;
	os << gpdComputeTypeName << quarkFlavorTypeShortName << "Singlet = "
			<< m_partonDistributionSinglet << std::endl;

	return os.str();
}

std::string GPDQuarkFlavorData::toStringGeneric() {

	std::ostringstream os;

	std::string gpdComputeTypeName = m_gpdComputeType.toString();
	std::string quarkFlavorTypeShortName = m_quarkFlavor.getShortName();

	os << gpdComputeTypeName << quarkFlavorTypeShortName << " = "
			<< m_partonDistribution << std::endl;
	os << gpdComputeTypeName << quarkFlavorTypeShortName << "(-) = "
			<< m_partonDistributionMinus << std::endl;
	os << gpdComputeTypeName << quarkFlavorTypeShortName << "(+) = "
			<< m_partonDistributionPlus << std::endl;
	os << gpdComputeTypeName << quarkFlavorTypeShortName << "Singlet = "
			<< m_partonDistributionSinglet << std::endl;

	return os.str();
}

double GPDQuarkFlavorData::getSea() const {
	return sea;
}

void GPDQuarkFlavorData::setSea(double hqSea) {
	sea = hqSea;
}

double GPDQuarkFlavorData::getValence() const {
	return valence;
}

void GPDQuarkFlavorData::setValence(double hqVal) {
	valence = hqVal;
}

QuarkFlavor* GPDQuarkFlavorData::getQuarkFlavor() {
	return &m_quarkFlavor;
}

void GPDQuarkFlavorData::setQuarkFlavor(QuarkFlavor &_quarkFlavor) {
	m_quarkFlavor = _quarkFlavor;
}

double GPDQuarkFlavorData::getPartonDistributionMinus() const {
	return m_partonDistributionMinus;
}

void GPDQuarkFlavorData::setPartonDistributionMinus(
		double partonDistributionMinus) {
	m_partonDistributionMinus = partonDistributionMinus;
}

double GPDQuarkFlavorData::getPartonDistributionPlus() const {
	return m_partonDistributionPlus;
}

void GPDQuarkFlavorData::setPartonDistributionPlus(
		double partonDistributionPlus) {
	m_partonDistributionPlus = partonDistributionPlus;
}

double GPDQuarkFlavorData::getPartonDistribution() const {
	return m_partonDistribution;
}

void GPDQuarkFlavorData::setPartonDistribution(double partonDistribution) {
	m_partonDistribution = partonDistribution;
}

double GPDQuarkFlavorData::getPartonDistributionSinglet() const {
	return m_partonDistributionSinglet;
}

void GPDQuarkFlavorData::setPartonDistributionSinglet(
		double partonDistributionSinglet) {
	m_partonDistributionSinglet = partonDistributionSinglet;
}
