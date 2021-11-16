#include "../../../../../include/partons/modules/process/GAM2/GAM2ProcessGPSSW21.h"

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"

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
// Polarized cross section (polarizations of all 3 photons specified)
// d(sigma) / dMgg^2 d(-u') dt
// Currently, the kinematics is set for t = t_min
    std::complex<double> H = getConvolCoeffFunctionValue(GPDType::H);

    double tau = 2. * m_xi / (1. + m_xi);
    double M2 = Constant::PROTON_MASS * Constant::PROTON_MASS;
    double SgN = (m_Mgg2 - m_t) / tau + M2;

    double diff_cross_section = 0.;

    diff_cross_section += (1 - m_xi * m_xi) * (H.real() *H.real() + H.imag()*H.imag());

    diff_cross_section /= std::pow(16. * Constant::PI, 3) * SgN * SgN * m_Mgg2;

    diff_cross_section *= 2.; // times 2

    diff_cross_section *= std::pow(Constant::POSITRON_CHARGE, 6);

    return PhysicalType<double>(diff_cross_section, PhysicalUnit::GEVm2);
}

} /* namespace PARTONS */
