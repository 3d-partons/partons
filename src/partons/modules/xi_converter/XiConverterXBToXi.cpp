#include "../../../../include/partons/modules/xi_converter/XiConverterXBToXi.h"

#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

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

PhysicalType<double> XiConverterXBToXi::compute(
        const PhysicalType<double>& xB, const PhysicalType<double>& t,
        const PhysicalType<double>& Q2) {
    initModule();
    isModuleWellConfigured();

    return PhysicalType<double>(xB.getValue() / (2. - xB.getValue()),
            PhysicalUnit::NONE);
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

} /* namespace PARTONS */
