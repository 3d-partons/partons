#include "../../../../../include/partons/modules/process/TCS/TCSProcessBDP01.h"

#include <cmath>
#include <complex>

#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int TCSProcessBDP01::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSProcessBDP01("TCSProcessBDP01"));

TCSProcessBDP01::TCSProcessBDP01(const std::string &className) :
        TCSProcessModule(className) {
}

TCSProcessBDP01::~TCSProcessBDP01() {
}

TCSProcessBDP01::TCSProcessBDP01(const TCSProcessBDP01& other) :
        TCSProcessModule(other) {
}

TCSProcessBDP01* TCSProcessBDP01::clone() const {
    return new TCSProcessBDP01(*this);
}

void TCSProcessBDP01::initModule() {
    TCSProcessModule::initModule();
}

void TCSProcessBDP01::isModuleWellConfigured() {
    TCSProcessModule::isModuleWellConfigured();
}

double F1(double t) { // form factor F_1
    double Mp2 = Constant::PROTON_MASS * Constant::PROTON_MASS;
    double G_E = 1. / (1. - t / 0.71) / (1. - t / 0.71); // electric form factor
    double muP = 2.7928; // proton magnetic moment
    double x = -t / 4. / Mp2;
    return (x * muP + 1.) / (1. + x) * G_E;
}

double F2(double t) { // form factor F_1
    double Mp2 = Constant::PROTON_MASS * Constant::PROTON_MASS;
    double G_E = 1. / (1. - t / 0.71) / (1. - t / 0.71); // electric form factor
    double muP = 2.7928; // proton magnetic moment
    double x = -t / 4. / Mp2;
    return (muP - 1.) / (1. + x) * G_E;
}

double FB(double t) { // (F_1 + F_2)^2
    double G_M = 2.7928 / (1. - t / 0.71) / (1. - t / 0.71); // magnetic form factor. 2.79 is the proton magnetic moment
    return G_M * G_M;
}

double FA(double t) { // (F_1^2 - (t/4Mp2)F_2^2)
    double Mp2 = Constant::PROTON_MASS * Constant::PROTON_MASS;
    double G_E = 1. / (1. - t / 0.71) / (1. - t / 0.71); // electric form factor
    double x = -t / 4. / Mp2;
    return F1(t) * F1(t) + x * F2(t) * F2(t);
}

PhysicalType<double> TCSProcessBDP01::CrossSectionBH() {

    double Mp2 = Constant::PROTON_MASS * Constant::PROTON_MASS;
    double alpha3 = Constant::FINE_STRUCTURE_CONSTANT
            * Constant::FINE_STRUCTURE_CONSTANT
            * Constant::FINE_STRUCTURE_CONSTANT; // subtle structure constant powered 3

    // all variables are defined as in the source paper
    double s = Mp2 + 2. * Constant::PROTON_MASS * m_E;

    double r = sqrt(
            (s - m_Q2Prim - Mp2) * (s - m_Q2Prim - Mp2) - 4. * m_Q2Prim * Mp2);

    double DeltaT2 = -m_t * m_t * s - Mp2 * m_Q2Prim * m_Q2Prim;
    DeltaT2 += m_t * ((s + Mp2) * m_Q2Prim - (s - Mp2) * (s - Mp2));
    DeltaT2 /= (s - Mp2) * (s - Mp2);

    double a = r * cos(m_theta);    // eq. 15

    double b = m_Q2Prim * (s - Mp2 - m_Q2Prim) + m_t * (s + m_Q2Prim - Mp2);
    b *= cos(m_theta) / r;
    b += -2. * (s - Mp2) * sqrt(m_Q2Prim) * sqrt(DeltaT2) * sin(m_theta)
            * cos(m_phi) / r;       // eq. 15

    double L = (m_Q2Prim - m_t) * (m_Q2Prim - m_t) - b * b;
    L /= 4.;        // eq. 20

    double A = (s - Mp2) * (s - Mp2) * DeltaT2;
    A += -m_t * a * (a + b) - Mp2 * b * b;
    A += -m_t * (4. * Mp2 - m_t) * m_Q2Prim;        // eq. 18

    double B = (m_Q2Prim + m_t) * (m_Q2Prim + m_t) + b * b; // eq. 18

    double DiffCrossBH = FB(m_t) * B / 2. - FA(m_t) * A / m_t; // Differential cross section, eq. 17
    DiffCrossBH *= alpha3 / 4. / Constant::PI;
    DiffCrossBH /= -m_t * L;
    DiffCrossBH /= (s - Mp2) * (s - Mp2);

    return PhysicalType<double>(DiffCrossBH, PhysicalUnit::GEVm2);
}

PhysicalType<double> TCSProcessBDP01::CrossSectionVCS() {

    double Mp2 = Constant::PROTON_MASS * Constant::PROTON_MASS;
    double alpha3 = Constant::FINE_STRUCTURE_CONSTANT
            * Constant::FINE_STRUCTURE_CONSTANT
            * Constant::FINE_STRUCTURE_CONSTANT; // subtle structure constant powered 3
    double s = Mp2 + 2. * Constant::PROTON_MASS * m_E;

    std::complex<double> H = getConvolCoeffFunctionValue(GPDType::H);
    std::complex<double> E = getConvolCoeffFunctionValue(GPDType::E);
    std::complex<double> Ht = getConvolCoeffFunctionValue(GPDType::Ht);
    std::complex<double> Et = getConvolCoeffFunctionValue(GPDType::Et);

    double tau = m_Q2Prim / (s - Mp2);
    double etha2 = tau * tau / (2. - tau) / (2. - tau);

    double DiffCrossC = (1. - etha2)
            * (std::abs(H) * std::abs(H) + std::abs(Ht) * std::abs(Ht)); // eq. 26
    DiffCrossC += -2. * etha2 * (E * conj(H) + Et * conj(Ht)).real();
    DiffCrossC += -(etha2 + m_t / 4. / Mp2) * std::abs(E) * std::abs(E);
    DiffCrossC += -etha2 * m_t / 4. / Mp2 * std::abs(Et) * std::abs(Et);
    DiffCrossC *= alpha3 / 8. / Constant::PI / s / s;       // eq. 25
    DiffCrossC /= 2. * m_Q2Prim;
    DiffCrossC *= (1. + cos(m_theta) * cos(m_theta));

    return PhysicalType<double>(DiffCrossC, PhysicalUnit::GEVm2);
}

PhysicalType<double> TCSProcessBDP01::CrossSectionInterf() {

    double Mp2 = Constant::PROTON_MASS * Constant::PROTON_MASS;
    double alpha3 = Constant::FINE_STRUCTURE_CONSTANT
            * Constant::FINE_STRUCTURE_CONSTANT
            * Constant::FINE_STRUCTURE_CONSTANT; // subtle structure constant powered 3

    // all variables are defined as in the source paper
    double s = Mp2 + 2. * Constant::PROTON_MASS * m_E;

    double r = sqrt(
            (s - m_Q2Prim - Mp2) * (s - m_Q2Prim - Mp2) - 4. * m_Q2Prim * Mp2);

    double DeltaT2 = -m_t * m_t * s - Mp2 * m_Q2Prim * m_Q2Prim;
    DeltaT2 += m_t * ((s + Mp2) * m_Q2Prim - (s - Mp2) * (s - Mp2));
    DeltaT2 /= (s - Mp2) * (s - Mp2);

    double a = r * cos(m_theta);    // eq. 15

    double b = m_Q2Prim * (s - Mp2 - m_Q2Prim) + m_t * (s + m_Q2Prim - Mp2);
    b *= cos(m_theta) / r;
    b += -2. * (s - Mp2) * sqrt(m_Q2Prim) * sqrt(DeltaT2) * sin(m_theta)
            * cos(m_phi) / r;       // eq. 15

    double L = (m_Q2Prim - m_t) * (m_Q2Prim - m_t) - b * b;
    L /= 4.;        // eq. 20
    double L0 = m_Q2Prim * m_Q2Prim * sin(m_theta) * sin(m_theta) / 4.;

    std::complex<double> H = getConvolCoeffFunctionValue(GPDType::H);
    std::complex<double> E = getConvolCoeffFunctionValue(GPDType::E);
    std::complex<double> Ht = getConvolCoeffFunctionValue(GPDType::Ht);

    double tau = m_Q2Prim / (s - Mp2);
    double etha = tau / (2. - tau);
    double t0 = -4. * etha * etha * Mp2 / (1. - etha * etha);

    std::complex<double> M = H * F1(m_t); // see eq. 33
    M += -m_t / (4. * Mp2) * E * F2(m_t);
    M += -etha * Ht * (F1(m_t) + F2(m_t));
    M *= 2. * sqrt(t0 - m_t) / Constant::PROTON_MASS * (1. - etha)
            / (1. + etha);

    double DiffCrossINT = cos(m_phi) * M.real() // eq. 30
    + double(m_beamPolarization) * sin(m_phi) * M.imag(); // helicity - dependent part. See eq. 32
    DiffCrossINT *= (1. + cos(m_theta) * cos(m_theta)) / sin(m_theta);
    DiffCrossINT *= alpha3 / 4. / Constant::PI;
    DiffCrossINT /= s * s;
    DiffCrossINT *= 1. / (m_t) * Constant::PROTON_MASS / sqrt(m_Q2Prim);
    DiffCrossINT /= tau * sqrt(1. - tau);
    DiffCrossINT *= L0 / L;

    return PhysicalType<double>(DiffCrossINT, PhysicalUnit::GEVm2);
}

} /* namespace PARTONS */
