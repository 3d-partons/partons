#include "GPDResultData.h"

#include <sstream>
#include <utility>

//#include "../QuarkFlavor.h"

GPDResultData::GPDResultData() :
		m_gpdComputeType(GPDComputeType::UNDEFINED), /*squareChargeAveraged(0.),*/m_gluon(
				0.), m_singlet(0.) {
}

GPDResultData::GPDResultData(GPDComputeType &_gpdComputeType) :
		m_gpdComputeType(_gpdComputeType), /*squareChargeAveraged(0.),*/m_gluon(
				0.), m_singlet(0.) {
}

GPDResultData::~GPDResultData() {
	m_gpdQuarkFlavorData.clear();
}

void GPDResultData::addGPDQuarkFlavorData(
		GPDQuarkFlavorData &_gpdQuarkFlavorData) {
	m_gpdQuarkFlavorData.insert(
			std::pair<QuarkFlavor::Type, GPDQuarkFlavorData>(
					_gpdQuarkFlavorData.getQuarkFlavor()->getType(),
					_gpdQuarkFlavorData));
}

GPDQuarkFlavorData* GPDResultData::getGPDQuarkFlavorData(
		QuarkFlavor::Type quarkFlavorType) {
	m_it = m_gpdQuarkFlavorData.find(quarkFlavorType);
	return (m_it != m_gpdQuarkFlavorData.end()) ? &(m_it->second) : 0;
}

//TODO tester cette méthode
std::vector<QuarkFlavor> GPDResultData::listQuarkTypeComputed() {
	std::vector<QuarkFlavor> list;

	if (m_gpdQuarkFlavorData.size() != 0) {

		for (m_it = m_gpdQuarkFlavorData.begin();
				m_it != m_gpdQuarkFlavorData.end(); ++m_it) {
			list.push_back(m_it->first);
		}
	}

	return list;
}

std::string GPDResultData::toString() {

	std::ostringstream os;

	if (m_gpdQuarkFlavorData.size() != 0) {
		for (m_it = m_gpdQuarkFlavorData.begin();
				m_it != m_gpdQuarkFlavorData.end(); ++m_it) {
			os << (m_it->second).toString() << std::endl;
		}
	}

//    os << pGPDComputeType->toString() << " = " << squareChargeAveraged
//            << std::endl;
	os << m_gpdComputeType.toString() << "g = " << m_gluon << std::endl;
	os << m_gpdComputeType.toString() << "Singlet = " << m_singlet << std::endl;

	return os.str();
}

GPDComputeType* GPDResultData::getGpdComputeType() {
	return &m_gpdComputeType;
}

void GPDResultData::setGpdComputeType(GPDComputeType &_gpdComputeType) {
	m_gpdComputeType = _gpdComputeType;
}

void GPDResultData::setGluon(double gluon) {
	m_gluon = gluon;
}

double GPDResultData::getSinglet() const {
	return m_singlet;
}

void GPDResultData::setSinglet(double hsinglet) {
	m_singlet = hsinglet;
}

double GPDResultData::getGluon() const {
	return m_gluon;
}

//double GPDResultData::getSquareChargeAveraged() const {
//    return squareChargeAveraged;
//}
//
//void GPDResultData::setSquareChargeAveraged(double squareChargeAveraged) {
//    this->squareChargeAveraged = squareChargeAveraged;
//}

std::vector<GPDQuarkFlavorData> GPDResultData::getListOfQuarkFlavorData() {
	std::vector<GPDQuarkFlavorData> result;

	for (m_it = m_gpdQuarkFlavorData.begin();
			m_it != m_gpdQuarkFlavorData.end(); m_it++) {
		result.push_back(m_it->second);
	}

	return result;
}

size_t GPDResultData::sizeOfListOfQuarkFlavorData() {
	return m_gpdQuarkFlavorData.size();
}
