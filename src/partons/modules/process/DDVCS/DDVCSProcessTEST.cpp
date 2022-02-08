#include "../../../../../include/partons/modules/process/DDVCS/DDVCSProcessTEST.h"

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DDVCSProcessTEST::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSProcessTEST("DDVCSProcessTEST"));

DDVCSProcessTEST::DDVCSProcessTEST(const std::string &className) :
        DDVCSProcessModule(className) {
}

DDVCSProcessTEST::DDVCSProcessTEST(const DDVCSProcessTEST& other) :
        DDVCSProcessModule(other) {
}

DDVCSProcessTEST::~DDVCSProcessTEST() {
}

DDVCSProcessTEST* DDVCSProcessTEST::clone() const {
    return new DDVCSProcessTEST(*this);
}

void DDVCSProcessTEST::initModule() {

    // Init mother class
    DDVCSProcessModule::initModule();
}

void DDVCSProcessTEST::isModuleWellConfigured() {

    //check mother class
    DDVCSProcessModule::isModuleWellConfigured();
}

PhysicalType<double> DDVCSProcessTEST::CrossSectionBH() {
    return PhysicalType<double>(1., PhysicalUnit::GEVm2);
}

PhysicalType<double> DDVCSProcessTEST::CrossSectionVCS() {
    return PhysicalType<double>(2., PhysicalUnit::GEVm2);
}

PhysicalType<double> DDVCSProcessTEST::CrossSectionInterf() {
    return PhysicalType<double>(3., PhysicalUnit::GEVm2);
}

} /* namespace PARTONS */
