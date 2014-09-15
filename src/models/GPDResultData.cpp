#include "GPDResultData.h"

#include <sstream>
#include <utility>

#include "GPDComputeType.h"

GPDResultData::GPDResultData(GPDComputeType* _pGPDComputeType) :
        pGPDComputeType(_pGPDComputeType), /*squareChargeAveraged(0.),*/gluon(
                0.), singlet(0.) {
}

GPDResultData::~GPDResultData() {
    gpdQuarkFlavorData.clear();
}

void GPDResultData::addGPDQuarkFlavorData(
        GPDQuarkFlavorData* _pGPDQuarkFlavorData) {
    gpdQuarkFlavorData.insert(
            std::pair<QuarkFlavor::Type, GPDQuarkFlavorData*>(
                    _pGPDQuarkFlavorData->getQuarkFlavor()->t_,
                    _pGPDQuarkFlavorData));
}

GPDQuarkFlavorData* GPDResultData::getGPDQuarkFlavorData(
        QuarkFlavor::Type quarkFlavorType) {
    return gpdQuarkFlavorData[quarkFlavorType];
}

//TODO tester cette méthode
std::vector<QuarkFlavor> GPDResultData::listQuarkTypeComputed() {
    std::vector<QuarkFlavor> list;

    if (gpdQuarkFlavorData.size() != 0) {

        for (std::map<QuarkFlavor::Type, GPDQuarkFlavorData*>::iterator it =
                gpdQuarkFlavorData.begin(); it != gpdQuarkFlavorData.end();
                ++it) {
            list.push_back(it->first);
        }
    }

    return list;
}

std::string GPDResultData::toString() {

    std::ostringstream os;

    if (gpdQuarkFlavorData.size() != 0) {
        for (std::map<QuarkFlavor::Type, GPDQuarkFlavorData*>::iterator it =
                gpdQuarkFlavorData.begin(); it != gpdQuarkFlavorData.end();
                ++it) {
            os << (it->second)->toString() << std::endl;
        }
    }

//    os << pGPDComputeType->toString() << " = " << squareChargeAveraged
//            << std::endl;
    os << pGPDComputeType->toString() << "g = " << gluon << std::endl;
    os << pGPDComputeType->toString() << "Singlet = " << singlet << std::endl;

    return os.str();
}

GPDComputeType* GPDResultData::getGpdComputeType() const {
    return pGPDComputeType;
}

void GPDResultData::setGpdComputeType(GPDComputeType* gpdComputeType) {
    pGPDComputeType = gpdComputeType;
}

double GPDResultData::getHg() const {
    return gluon;
}

void GPDResultData::setGluon(double hg) {
    gluon = hg;
}

double GPDResultData::getSinglet() const {
    return singlet;
}

void GPDResultData::setSinglet(double hsinglet) {
    singlet = hsinglet;
}

double GPDResultData::getGluon() const {
    return gluon;
}

//double GPDResultData::getSquareChargeAveraged() const {
//    return squareChargeAveraged;
//}
//
//void GPDResultData::setSquareChargeAveraged(double squareChargeAveraged) {
//    this->squareChargeAveraged = squareChargeAveraged;
//}
