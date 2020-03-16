/*
 * TCSProcessBDP01NucleiSimple.cpp
 *
 *  Created on: Mar 11, 2020
 *      Author: partons
 */

#include "../../../../../include/partons/modules/process/TCS/TCSProcessBDP01NucleiSimple.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <cmath>
#include <complex>

#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../../../include/partons/beans/observable/TCS/TCSObservableResult.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/process/ProcessModule.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int TCSProcessBDP01NucleiSimple::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSProcessBDP01NucleiSimple("TCSProcessBDP01NucleiSimple"));

TCSProcessBDP01NucleiSimple::TCSProcessBDP01NucleiSimple(
        const std::string& className) :
        TCSProcessBDP01(className), Nuclei(className) {
}

TCSProcessBDP01NucleiSimple::TCSProcessBDP01NucleiSimple(
        const TCSProcessBDP01NucleiSimple& other) :
        TCSProcessBDP01(other), Nuclei(other) {
}

TCSProcessBDP01NucleiSimple::~TCSProcessBDP01NucleiSimple() {
}

TCSProcessBDP01NucleiSimple* TCSProcessBDP01NucleiSimple::clone() const {
    return new TCSProcessBDP01NucleiSimple(*this);
}

void TCSProcessBDP01NucleiSimple::configure(
        const ElemUtils::Parameters& parameters) {

    TCSProcessBDP01::configure(parameters);
    Nuclei::configure(parameters);
}

void TCSProcessBDP01NucleiSimple::initModule() {

    //run for mother
    ProcessModule<TCSObservableKinematic, TCSObservableResult>::initModule();

    //evaluate internal variables
    double s = pow(Constant::PROTON_MASS, 2) + 2. * Constant::PROTON_MASS * m_E;

    double p1cm = m_E * Constant::PROTON_MASS / sqrt(s);
    double E3cm = (s + m_Q2Prim - pow(Constant::PROTON_MASS, 2))
            / (2 * sqrt(s));
    double p3cm = sqrt(pow(E3cm, 2) - m_Q2Prim);

    m_tmin = pow(m_Q2Prim, 2) / (4 * s) - pow(p1cm - p3cm, 2);
    m_tmax = pow(m_Q2Prim, 2) / (4 * s) - pow(p1cm + p3cm, 2);
}

void TCSProcessBDP01NucleiSimple::isModuleWellConfigured() {

    //run for mother
    ProcessModule<TCSObservableKinematic, TCSObservableResult>::isModuleWellConfigured();

    //check if pointer to scale module set
    if (m_pScaleModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "m_pScaleModule is NULL pointer ; Use configure method to configure it");
    }

    //check if pointer to xi module set
    if (m_pXiConverterModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "m_pXiConverterModule is NULL pointer ; Use configure method to configure it");
    }

    //check if pointer to cff module set
    if (isCCFModuleDependent() && m_pConvolCoeffFunctionModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "m_pConvolCoeffFunctionModule is NULL pointer ; Use configure method to configure it");
    }

    //test kinematic domain of t
    if (m_t > m_tmin || m_t < m_tmax) {
        ElemUtils::Formatter formatter;
        formatter << " Input value of t = " << m_t
                << " does not lay between t_max = " << m_tmax << " and t_min = "
                << m_tmin << " (TCS kinematic limits)";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of Q2
    if (m_Q2Prim < 0.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of Q2' = " << m_Q2Prim << " is not > 0";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of E
    if (m_E < 0.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of E = " << m_E << " is not > 0";
        warn(__func__, formatter.str());
    }

    //test if s >= (M + Q')^2
    if (2 * m_E * Constant::PROTON_MASS
            < 2 * Constant::PROTON_MASS * sqrt(m_Q2Prim) + m_Q2Prim) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of E = " << m_E << " and Q2' = " << m_Q2Prim
                << " does not satisfy s >= (M + Q')^2";
        warn(__func__, formatter.str());
    }

    //test beam polarization
    if (fabs(m_beamPolarization) != 0. && fabs(m_beamPolarization) != 1.) {
        ElemUtils::Formatter formatter;
        formatter << "Beam polarization = " << m_beamPolarization
                << "is not +/- 1 or 0";
        warn(__func__, formatter.str());
    }

    //test target polarization
    double targetMag = sqrt(
            pow(m_targetPolarization.getX(), 2)
                    + pow(m_targetPolarization.getY(), 2)
                    + pow(m_targetPolarization.getZ(), 2));

    if (targetMag != 0. && targetMag != 1.) {
        ElemUtils::Formatter formatter;
        formatter << "Magnitude of target polarization ("
                << m_targetPolarization.toString() << ") neither 0 nor 1";
        warn(__func__, formatter.str());
    }
}

PhysicalType<double> TCSProcessBDP01NucleiSimple::CrossSectionBH() {

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

    //add dphi_s and replaces d(cos theta) by dtheta
    return PhysicalType<double>(DiffCrossBH, PhysicalUnit::GEVm2) * sin(m_theta)
            / (2 * Constant::PI);
}

PhysicalType<double> TCSProcessBDP01NucleiSimple::CrossSectionVCS() {

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

    //add dphi_s and replaces d(cos theta) by dtheta
    return PhysicalType<double>(DiffCrossC, PhysicalUnit::GEVm2) * sin(m_theta)
            / (2 * Constant::PI);
}

PhysicalType<double> TCSProcessBDP01NucleiSimple::CrossSectionInterf() {

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
    + m_beamPolarization * sin(m_phi) * M.imag(); // helicity - dependent part. See eq. 32
    DiffCrossINT *= (1. + cos(m_theta) * cos(m_theta)) / sin(m_theta);
    DiffCrossINT *= alpha3 / 4. / Constant::PI;
    DiffCrossINT /= s * s;
    DiffCrossINT *= 1. / (m_t) * Constant::PROTON_MASS / sqrt(m_Q2Prim);
    DiffCrossINT /= tau * sqrt(1. - tau);
    DiffCrossINT *= L0 / L;

    //add dphi_s and replaces d(cos theta) by dtheta
    return PhysicalType<double>(DiffCrossINT, PhysicalUnit::GEVm2)
            * sin(m_theta) / (2 * Constant::PI);
}

} /* namespace PARTONS */
