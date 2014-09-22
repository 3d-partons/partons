#include "GPDResultData.h"

#include <sstream>
#include <utility>

#include "GPDComputeType.h"

GPDResultData::GPDResultData(GPDComputeType* _pGPDComputeType) :
        m_pGPDComputeType(_pGPDComputeType), /*squareChargeAveraged(0.),*/m_gluon(
                0.), m_singlet(0.) {
}

GPDResultData::~GPDResultData() {
    m_gpdQuarkFlavorData.clear();
}

void GPDResultData::addGPDQuarkFlavorData(
        GPDQuarkFlavorData* _pGPDQuarkFlavorData) {
    m_gpdQuarkFlavorData.insert(
            std::pair<QuarkFlavor::Type, GPDQuarkFlavorData*>(
                    _pGPDQuarkFlavorData->getQuarkFlavor()->t_,
                    _pGPDQuarkFlavorData));
}

GPDQuarkFlavorData* GPDResultData::getGPDQuarkFlavorData(
        QuarkFlavor::Type quarkFlavorType) {
    return m_gpdQuarkFlavorData[quarkFlavorType];
}

//TODO tester cette méthode
std::vector<QuarkFlavor> GPDResultData::listQuarkTypeComputed() {
    std::vector<QuarkFlavor> list;

    if (m_gpdQuarkFlavorData.size() != 0) {

        for (std::map<QuarkFlavor::Type, GPDQuarkFlavorData*>::iterator it =
                m_gpdQuarkFlavorData.begin(); it != m_gpdQuarkFlavorData.end();
                ++it) {
            list.push_back(it->first);
        }
    }

    return list;
}

std::string GPDResultData::toString() {

    std::ostringstream os;

    if (m_gpdQuarkFlavorData.size() != 0) {
        for (std::map<QuarkFlavor::Type, GPDQuarkFlavorData*>::iterator it =
                m_gpdQuarkFlavorData.begin(); it != m_gpdQuarkFlavorData.end();
                ++it) {
            os << (it->second)->toString() << std::endl;
        }
    }

//    os << pGPDComputeType->toString() << " = " << squareChargeAveraged
//            << std::endl;
    os << m_pGPDComputeType->toString() << "g = " << m_gluon << std::endl;
    os << m_pGPDComputeType->toString() << "Singlet = " << m_singlet
            << std::endl;

    return os.str();
}

GPDComputeType* GPDResultData::getGpdComputeType() const {
    return m_pGPDComputeType;
}

void GPDResultData::setGpdComputeType(GPDComputeType* gpdComputeType) {
    m_pGPDComputeType = gpdComputeType;
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
