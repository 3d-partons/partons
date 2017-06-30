#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/xb_to_xi/XiConverterXBToXi.h"

// Initialise [class]::classId with a unique name.
const unsigned int XiConverterXBToXi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new XiConverterXBToXi("XiConverterXBToXi"));

XiConverterXBToXi::XiConverterXBToXi(const std::string &className) :
        XiConverterModule(className) {
}

XiConverterXBToXi::~XiConverterXBToXi() {
}

XiConverterXBToXi* XiConverterXBToXi::clone() const {
    return new XiConverterXBToXi(*this);
}

double XiConverterXBToXi::compute(double xB, double t, double Q2) {
    initModule();
    isModuleWellConfigured();

    return (xB / (2 - xB));
}

XiConverterXBToXi::XiConverterXBToXi(const XiConverterXBToXi& other) :
        XiConverterModule(other) {
}

void XiConverterXBToXi::initModule() {
    XiConverterModule::initModule();
}

void XiConverterXBToXi::isModuleWellConfigured() {
    XiConverterModule::isModuleWellConfigured();
}

