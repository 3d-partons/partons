#include "GPDQuarkFlavorData.h"

#include <sstream>

#include "GPDComputeType.h"
#include "QuarkFlavor.h"

GPDQuarkFlavorData::GPDQuarkFlavorData(GPDComputeType* _pGPDComputeType,
        QuarkFlavor* _pQuarkFlavor) :
        pGPDComputeType(_pGPDComputeType), pQuarkFlavor(_pQuarkFlavor), Hq(0), valence(
                0), sea(0), singlet(0) {
}

GPDQuarkFlavorData::~GPDQuarkFlavorData() {
    if (pQuarkFlavor != 0) {
        delete pQuarkFlavor;
        pQuarkFlavor = 0;
    }
}

std::string GPDQuarkFlavorData::toString() {

    std::ostringstream os;

    std::string gpdComputeTypeName = pGPDComputeType->toString();
    std::string quarkFlavorTypeShortName = pQuarkFlavor->getShortName();

    os << gpdComputeTypeName << quarkFlavorTypeShortName << " = " << Hq
            << std::endl;
    os << gpdComputeTypeName << quarkFlavorTypeShortName << "Val = " << valence
            << std::endl;
    os << gpdComputeTypeName << quarkFlavorTypeShortName << "Sea = " << sea
            << std::endl;
    os << gpdComputeTypeName << quarkFlavorTypeShortName << "Singlet = "
            << singlet << std::endl;

    return os.str();
}

double GPDQuarkFlavorData::getHq() const {
    return Hq;
}

void GPDQuarkFlavorData::setHq(double hq) {
    Hq = hq;
}

double GPDQuarkFlavorData::getSea() const {
    return sea;
}

void GPDQuarkFlavorData::setSea(double hqSea) {
    sea = hqSea;
}

double GPDQuarkFlavorData::getSinglet() const {
    return singlet;
}

void GPDQuarkFlavorData::setSinglet(double hqSinglet) {
    singlet = hqSinglet;
}

double GPDQuarkFlavorData::getValence() const {
    return valence;
}

void GPDQuarkFlavorData::setValence(double hqVal) {
    valence = hqVal;
}

QuarkFlavor* GPDQuarkFlavorData::getQuarkFlavor() const {
    return pQuarkFlavor;
}

void GPDQuarkFlavorData::setQuarkFlavor(QuarkFlavor* quarkFlavor) {
    pQuarkFlavor = quarkFlavor;
}
