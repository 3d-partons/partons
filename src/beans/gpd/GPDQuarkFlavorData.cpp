#include "GPDQuarkFlavorData.h"

#include <sstream>

#include "../QuarkFlavor.h"

GPDQuarkFlavorData::GPDQuarkFlavorData(GPDComputeType &_gpdComputeType,
        QuarkFlavor::Type _quarkFlavorType) :
        m_gpdComputeType(_gpdComputeType), m_quarkFlavor(_quarkFlavorType), Hq(
                0), valence(0), sea(0), singlet(0) {
}

GPDQuarkFlavorData::~GPDQuarkFlavorData() {
}

std::string GPDQuarkFlavorData::toString() {

    std::ostringstream os;

    std::string gpdComputeTypeName = m_gpdComputeType.toString();
    std::string quarkFlavorTypeShortName = m_quarkFlavor.getShortName();

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

QuarkFlavor* GPDQuarkFlavorData::getQuarkFlavor() {
    return &m_quarkFlavor;
}

void GPDQuarkFlavorData::setQuarkFlavor(QuarkFlavor &_quarkFlavor) {
    m_quarkFlavor = _quarkFlavor;
}
