#include "../../../../../include/partons/modules/process/TCS/TCSProcessBDPGW19.h"

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int TCSProcessBDPGW19::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSProcessBDPGW19("TCSProcessBDPGW19"));

TCSProcessBDPGW19::TCSProcessBDPGW19(const std::string &className) :
        TCSProcessBDP01(className) {
}

TCSProcessBDPGW19::~TCSProcessBDPGW19() {
}

TCSProcessBDPGW19::TCSProcessBDPGW19(const TCSProcessBDPGW19& other) :
        TCSProcessBDP01(other) {
}

TCSProcessBDPGW19* TCSProcessBDPGW19::clone() const {
    return new TCSProcessBDPGW19(*this);
}

PhysicalType<double> TCSProcessBDPGW19::CrossSectionBH() {
    return TCSProcessBDP01::CrossSectionBH();
}

PhysicalType<double> TCSProcessBDPGW19::CrossSectionVCS() {
    return TCSProcessBDP01::CrossSectionVCS()
            + PhysicalType<double>(0., PhysicalUnit::GEVm2);
}

PhysicalType<double> TCSProcessBDPGW19::CrossSectionInterf() {
    return TCSProcessBDP01::CrossSectionInterf();
}

} /* namespace PARTONS */
