#include "../../../../../include/partons/modules/process/GAM2/GAM2ProcessGPSSW21.h"

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int GAM2ProcessGPSSW21::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GAM2ProcessGPSSW21("GAM2ProcessGPSSW21"));

GAM2ProcessGPSSW21::GAM2ProcessGPSSW21(const std::string &className) :
        GAM2ProcessModule(className) {
}

GAM2ProcessGPSSW21::~GAM2ProcessGPSSW21() {
}

GAM2ProcessGPSSW21::GAM2ProcessGPSSW21(const GAM2ProcessGPSSW21& other) :
        GAM2ProcessModule(other) {
}

GAM2ProcessGPSSW21* GAM2ProcessGPSSW21::clone() const {
    return new GAM2ProcessGPSSW21(*this);
}

void GAM2ProcessGPSSW21::initModule() {

    // Init mother class
    GAM2ProcessModule::initModule();

    // Define own stuff

}

void GAM2ProcessGPSSW21::isModuleWellConfigured() {

    //check mother class
    GAM2ProcessModule::isModuleWellConfigured();

    // Define own stuff.

}

PhysicalType<double> GAM2ProcessGPSSW21::CrossSection() {

    std::cout << m_dvcsConvolCoeffFunctionResult.getResult(GPDType::H) << std::endl;

    return PhysicalType<double>(1.1234, PhysicalUnit::GEVm2);
}

} /* namespace PARTONS */
