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
    return TCSProcessBDP01::CrossSectionBH() / PhysicalType<double>(2. * Constant::PI, PhysicalUnit::GEVm2);
}

PhysicalType<double> TCSProcessBDPGW19::CrossSectionVCS() {
    return TCSProcessBDP01::CrossSectionVCS() / PhysicalType<double>(2. * Constant::PI, PhysicalUnit::GEVm2);
}

PhysicalType<double> TCSProcessBDPGW19::CrossSectionInterf() {

    /*
     * Polarisaton - dependent part in the limit DeltaT -> 0
     * All formulae are in my notes "TCS with transversely polarised target"
     */

    double psi = 0.; /// polarisation angle, temporarily set by hand

    double Mp2 = Constant::PROTON_MASS * Constant::PROTON_MASS;
    double alpha3 = Constant::FINE_STRUCTURE_CONSTANT
            * Constant::FINE_STRUCTURE_CONSTANT
            * Constant::FINE_STRUCTURE_CONSTANT; // fine structure constant powered 3
    double s = Mp2 + 2. * Constant::PROTON_MASS * m_E;

    std::complex<double> H = getConvolCoeffFunctionValue(GPDType::H);
    std::complex<double> E = getConvolCoeffFunctionValue(GPDType::E);
    std::complex<double> Ht = getConvolCoeffFunctionValue(GPDType::Ht);
    std::complex<double> Et = getConvolCoeffFunctionValue(GPDType::Et);

    double tau = m_Q2Prim / (s - Mp2);
    double etha = tau / (2. - tau);

    double r = sqrt(
            (s - m_Q2Prim - Mp2) * (s - m_Q2Prim - Mp2) - 4. * m_Q2Prim * Mp2);

    double b = m_Q2Prim * (s - Mp2 - m_Q2Prim) + m_t * (s + m_Q2Prim - Mp2);
    b *= cos(m_theta) / r;

    double A = 2. * (m_Q2Prim * sin(m_theta) * sin(m_theta) + (m_t - 3. * m_Q2Prim) / 2.) * (m_t - m_Q2Prim) - b * b;
    A *= 4. / ((m_Q2Prim - m_t) * (m_Q2Prim - m_t) - b * b); /// A defined in eq. 20

    double B = (m_t - b + m_Q2Prim) / (m_t - b - m_Q2Prim) + (m_t + b + m_Q2Prim) / (m_t + b - m_Q2Prim);
    B *= 2. * etha; /// B defined after eq. 26

    double DiffCrossSpin = (4. * etha - 2. * etha * etha * etha) / (1. - etha * etha) * E.imag() + 2. * etha * H.imag();
    DiffCrossSpin *= Constant::PROTON_MASS * A;
    DiffCrossSpin += B * (2. * Constant::PROTON_MASS * Ht.real() + m_t * Et.real() / 2. / Constant::PROTON_MASS);
    DiffCrossSpin *= 2.7928 / (1. - m_t / 0.71) / (1. - m_t / 0.71) * sqrt(m_Q2Prim) * sin(m_theta); /// (F_1 + F_2) Q' sin(theta)
    DiffCrossSpin *= alpha3 / (32. * Constant::PI * Constant::PI) / (s * s * m_t * m_Q2Prim);
    DiffCrossSpin *= sin(psi - m_phi);

    return TCSProcessBDP01::CrossSectionInterf()  / PhysicalType<double>(2. * Constant::PI, PhysicalUnit::GEVm2)
            + PhysicalType<double>(DiffCrossSpin, PhysicalUnit::GEVm2);
}

} /* namespace PARTONS */
