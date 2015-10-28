#include "XBToXi.h"

#include "../../BaseObjectRegistry.h"

// Initialise [class]::classId with a unique name.
const unsigned int XBToXi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new XBToXi("XBToXi"));

XBToXi::XBToXi(const std::string &className) :
        XiConverterModule(className) {
}

XBToXi::~XBToXi() {
}

XBToXi* XBToXi::clone() const {
    return new XBToXi(*this);
}

double XBToXi::compute(double xB, double t, double Q2) {
    initModule();
    isModuleWellConfigured();

    return (xB / (2 - xB));
}

XBToXi::XBToXi(const XBToXi& other) :
        XiConverterModule(other) {
}

void XBToXi::initModule() {
    XiConverterModule::initModule();
}

void XBToXi::isModuleWellConfigured() {
    XiConverterModule::isModuleWellConfigured();
}

