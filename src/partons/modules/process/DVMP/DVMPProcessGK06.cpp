#include "../../../../../include/partons/modules/process/DVMP/DVMPProcessGK06.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <cmath>

#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/MesonType.h"
#include "../../../../../include/partons/beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/xi_converter/DVMP/DVMPXiConverterModule.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVMPProcessGK06::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPProcessGK06("DVMPProcessGK06"));

DVMPProcessGK06::DVMPProcessGK06(const std::string &className) :
        DVMPProcessModule(className), m_W2(0.), m_gamma(0.), m_eps(0.), m_xi(0.) {
}

DVMPProcessGK06::DVMPProcessGK06(const DVMPProcessGK06& other) :
        DVMPProcessModule(other), m_W2(other.m_W2), m_gamma(other.m_gamma), m_eps(
                other.m_eps), m_xi(other.m_xi) {
}

DVMPProcessGK06* DVMPProcessGK06::clone() const {
    return new DVMPProcessGK06(*this);
}

DVMPProcessGK06::~DVMPProcessGK06() {
}

void DVMPProcessGK06::initModule() {

    //run for mother
    DVMPProcessModule::initModule();

    //evaluate W, gamma and epsilon
    m_W2 = m_Q2 / m_xB + pow(Constant::PROTON_MASS, 2.0) - m_Q2;

    m_gamma = 2 * m_xB * Constant::PROTON_MASS / sqrt(m_Q2);

    m_eps = (1. - m_y - pow(m_y * m_gamma / 2., 2))
            / (1. - m_y + pow(m_y, 2) / 2. + pow(m_y * m_gamma / 2., 2));

    //evaluate xi
    m_xi =
            getXiConverterModule()->compute(
                    DVMPObservableKinematic(m_xB, m_t, m_Q2, m_E, m_phi,
                            m_mesonType)).getValue();

    //reevaluate t_min
    m_tmin = -4. * pow(Constant::PROTON_MASS, 2.) * pow(m_xi, 2.)
            / (1. - pow(m_xi, 2.));
}

void DVMPProcessGK06::isModuleWellConfigured() {
    DVMPProcessModule::isModuleWellConfigured();
}

PhysicalType<double> DVMPProcessGK06::CrossSection() {

    //check meson type
    if (m_mesonType != MesonType::PI0 && m_mesonType != MesonType::PIPLUS) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No implementation for meson "
                        << MesonType(m_mesonType).toString());
    }

    //result
    double result = Constant::FINE_STRUCTURE_CONSTANT
            * (m_W2 - pow(Constant::PROTON_MASS, 2))
            / (16 * pow(M_PI, 2) * pow(m_E, 2) * pow(Constant::PROTON_MASS, 2)
                    * m_Q2 * (1. - m_eps));

    result /= 32 * M_PI * (m_W2 - pow(Constant::PROTON_MASS, 2))
            * sqrt(lambdaFunction(m_W2, -m_Q2, pow(Constant::PROTON_MASS, 2)));

    result *= CrossSectionT() + m_eps * CrossSectionL()
            + m_eps * cos(2 * m_phi) * CrossSectionTT()
            + sqrt(2 * m_eps * (1. + m_eps)) * cos(m_phi) * CrossSectionLT();

    result *= 1. + m_beamHelicity * CrossSectionALU() * sin(m_phi)
            + m_targetPolarization.getZ() * CrossSectionAUL() * sin(m_phi);

    //apply dW2/dxB
    result *= m_Q2 / pow(m_xB, 2);

    //divide by 2pi to have dsigma/...dphiS
    result /= 2 * Constant::PI;

    return PhysicalType<double>(result, PhysicalUnit::GEVm2);
}

double DVMPProcessGK06::CrossSectionL() {

    // Longitudinal partial cross section. See Eq. (43) in arxiv:0906.0460

    std::complex<double> amplitude0m0p = Amplitude0m0p();
    std::complex<double> amplitude0p0p = Amplitude0p0p();

    return 2
            * (pow(std::abs(amplitude0p0p), 2.)
                    + pow(std::abs(amplitude0m0p), 2.));
}

double DVMPProcessGK06::CrossSectionT() {

    // Transverse partial cross section. See Eq. (43) in arxiv:0906.0460

    std::complex<double> amplitude0mpp = Amplitude0mpp();
    std::complex<double> amplitude0ppp = Amplitude0ppp();
    std::complex<double> amplitude0pmp = Amplitude0pmp();
    std::complex<double> amplitude0mmp = Amplitude0mmp();

    return pow(std::abs(amplitude0mpp), 2.) + pow(std::abs(amplitude0mmp), 2.)
            + pow(std::abs(amplitude0ppp), 2.)
            + pow(std::abs(amplitude0pmp), 2.);
}

double DVMPProcessGK06::CrossSectionLT() {

    std::complex<double> amplitude0m0p = Amplitude0m0p();
    std::complex<double> amplitude0p0p = Amplitude0p0p();
    std::complex<double> amplitude0mpp = Amplitude0mpp();
    std::complex<double> amplitude0ppp = Amplitude0ppp();
    std::complex<double> amplitude0pmp = Amplitude0pmp();
    std::complex<double> amplitude0mmp = Amplitude0mmp();

    // Partial cross section of the interference part LT. See Eq. (43) in arxiv:0906.0460

    return -1 * sqrt(2.)
            * (std::real(
                    std::conj(amplitude0m0p) * (amplitude0mpp - amplitude0mmp)
                            + std::conj(amplitude0p0p)
                                    * (amplitude0ppp - amplitude0pmp)));
}

double DVMPProcessGK06::CrossSectionTT() {

    std::complex<double> amplitude0mpp = Amplitude0mpp();
    std::complex<double> amplitude0ppp = Amplitude0ppp();
    std::complex<double> amplitude0pmp = Amplitude0pmp();
    std::complex<double> amplitude0mmp = Amplitude0mmp();

    // Partial cross section of the interference part TT. See Eq. (43) in arxiv:0906.0460

    return -1
            * (std::real(
                    std::conj(amplitude0mpp) * amplitude0mmp
                            + std::conj(amplitude0ppp) * amplitude0pmp));
}

double DVMPProcessGK06::CrossSectionAUL() {

    std::complex<double> amplitude0m0p = Amplitude0m0p();
    std::complex<double> amplitude0p0p = Amplitude0p0p();
    std::complex<double> amplitude0mpp = Amplitude0mpp();
    std::complex<double> amplitude0ppp = Amplitude0ppp();
    std::complex<double> amplitude0pmp = Amplitude0pmp();
    std::complex<double> amplitude0mmp = Amplitude0mmp();

    // A_{UL} * sigma_{0}. See Eq. (47) in arxiv:0906.0460

    double cosThetaGamma = sqrt(
            1.
                    - pow(m_gamma, 2) * (1. - m_y - pow(m_y * m_gamma / 2., 2))
                            / (1. + pow(m_gamma, 2)));
    double sinThetaGamma = sqrt(1. - pow(cosThetaGamma, 2));

    double sigma0 = 0.5 * (CrossSectionT() + m_eps * CrossSectionL());

    double A = std::imag(
            (std::conj(amplitude0ppp) + std::conj(amplitude0pmp))
                    * amplitude0p0p
                    + (std::conj(amplitude0mpp) + std::conj(amplitude0mmp))
                            * amplitude0m0p);

    double B = 2 * m_eps * std::imag(std::conj(amplitude0m0p) * amplitude0p0p);
    B -= m_eps * std::imag(std::conj(amplitude0mpp) * amplitude0ppp);
    B -= std::imag(
            std::conj(amplitude0mpp) * amplitude0pmp
                    - std::conj(amplitude0ppp) * amplitude0mmp);

    return (sinThetaGamma * B - sqrt(m_eps * (1. + m_eps)) * cosThetaGamma * A)
            / sigma0;
}

double DVMPProcessGK06::CrossSectionALU() {

    std::complex<double> amplitude0m0p = Amplitude0m0p();
    std::complex<double> amplitude0p0p = Amplitude0p0p();
    std::complex<double> amplitude0mpp = Amplitude0mpp();
    std::complex<double> amplitude0ppp = Amplitude0ppp();
    std::complex<double> amplitude0pmp = Amplitude0pmp();
    std::complex<double> amplitude0mmp = Amplitude0mmp();

    // A_{LU} * sigma_{0}. See Eq. (49) in arxiv:0906.0460

    double cosThetaGamma = sqrt(
            1.
                    - pow(m_gamma, 2) * (1. - m_y - pow(m_y * m_gamma / 2., 2))
                            / (1. + pow(m_gamma, 2)));

    double sigma0 = 0.5 * (CrossSectionT() + m_eps * CrossSectionL());

    double A = std::imag(
            (std::conj(amplitude0ppp) - std::conj(amplitude0pmp))
                    * amplitude0p0p
                    + (std::conj(amplitude0mpp) - std::conj(amplitude0mmp))
                            * amplitude0m0p);

    return (-1 * sqrt(m_eps * (1. - m_eps)) * cosThetaGamma * A) / sigma0;
}

double DVMPProcessGK06::lambdaFunction(double a, double b, double c) const {
    return pow(a, 2.0) + pow(b, 2.0) + pow(c, 2.0)
            - 2.0 * (a * b + a * c + b * c);
}

double DVMPProcessGK06::poleResidue() {

//the residue of the pole. See Eq. (8) in https://arxiv.org/pdf/0906.0460.pdf

//The fit parameter to be used in pion form factor
    double FitParameter = 0.5;

//A parameter to be used in pion-nucleon vertex
    double LambdaN = 0.44;

//Pion-Nucleon coupling constant
    double PionNucleonCoupling = 13.1;

//Pion form factor
    double FF = 1.0 / (1.0 + m_Q2 / FitParameter);

//Pion-Nucleon vertex
    double PionNucleonVertex = (pow(LambdaN, 2.0)
            - pow(Constant::MESON_PIPLUS_MASS, 2.0))
            / (pow(LambdaN, 2.0) - m_t);

//The residue of the pole
    double residue = sqrt(2.0) * PionNucleonCoupling * FF * PionNucleonVertex;

    return residue;

}

double DVMPProcessGK06::poleAmplitude0p0p() {

    double poleAmplitude = -Constant::POSITRON_CHARGE * 2.0
            * Constant::PROTON_MASS * m_xi * sqrt(m_Q2)
            / sqrt(1.0 - pow(m_xi, 2.0)) * poleResidue()
            / (m_t - pow(Constant::MESON_PIPLUS_MASS, 2.0));

    return poleAmplitude;
}

double DVMPProcessGK06::poleAmplitude0m0p() {

    double poleAmplitude = Constant::POSITRON_CHARGE * sqrt(m_Q2)
            * sqrt(-(m_t - m_tmin)) * poleResidue()
            / (m_t - pow(Constant::MESON_PIPLUS_MASS, 2.0));

    return poleAmplitude;
}

double DVMPProcessGK06::poleAmplitude0ppp() {

    double poleAmplitude = Constant::POSITRON_CHARGE * 2.0 * sqrt(2.0) * m_xi
            * Constant::PROTON_MASS * sqrt(-(m_t - m_tmin)) * poleResidue()
            / (m_t - pow(Constant::MESON_PIPLUS_MASS, 2.0));

    return poleAmplitude;
}

double DVMPProcessGK06::poleAmplitude0pmp() {

    double poleAmplitude = -Constant::POSITRON_CHARGE * 2.0 * sqrt(2.0) * m_xi
            * Constant::PROTON_MASS * sqrt(-(m_t - m_tmin)) * poleResidue()
            / (m_t - pow(Constant::MESON_PIPLUS_MASS, 2.0));

    return poleAmplitude;
}

double DVMPProcessGK06::poleAmplitude0mpp() {

    double poleAmplitude = Constant::POSITRON_CHARGE * sqrt(2.0)
            * (m_t - m_tmin) * sqrt(1.0 - pow(m_xi, 2.0)) * poleResidue()
            / (m_t - pow(Constant::MESON_PIPLUS_MASS, 2.0));

    return poleAmplitude;
}

double DVMPProcessGK06::poleAmplitude0mmp() {

    double poleAmplitude = -Constant::POSITRON_CHARGE * sqrt(2.0)
            * (m_t - m_tmin) * sqrt(1.0 - pow(m_xi, 2.0)) * poleResidue()
            / (m_t - pow(Constant::MESON_PIPLUS_MASS, 2.0));

    return poleAmplitude;
}

std::complex<double> DVMPProcessGK06::Amplitude0p0p() {

//the handbag amplitude
    std::complex<double> amplitude0p0p = sqrt(1. - pow(m_xi, 2.))
            * Constant::POSITRON_CHARGE / sqrt(m_Q2)
            * (getConvolCoeffFunctionValue(GPDType::Ht)
                    - pow(m_xi, 2.) / (1. - pow(m_xi, 2.))
                            * getConvolCoeffFunctionValue(GPDType::Et));

//switch over mesons
    switch (m_mesonType) {

//pi0
    case MesonType::PI0: {

        return amplitude0p0p;

    }

        break;

        //pi+
    case MesonType::PIPLUS: {

        return amplitude0p0p + poleAmplitude0p0p();

    }

        break;

        //???
    default: {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No implementation for meson: "
                        << MesonType(m_mesonType).toString());
    }

        break;
    }

    return std::complex<double>(0., 0.);
}

std::complex<double> DVMPProcessGK06::Amplitude0m0p() {

//the handbag amplitude
    std::complex<double> amplitude0m0p = Constant::POSITRON_CHARGE / sqrt(m_Q2)
            * sqrt(-(m_t - m_tmin)) * m_xi / (2. * Constant::PROTON_MASS)
            * getConvolCoeffFunctionValue(GPDType::Et);

//switch over mesons
    switch (m_mesonType) {

//pi0
    case MesonType::PI0: {

        return amplitude0m0p;

    }

        break;

        //pi+
    case MesonType::PIPLUS: {

        return amplitude0m0p + poleAmplitude0m0p();

    }

        break;

        //???
    default: {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No implementation for meson: "
                        << MesonType(m_mesonType).toString());
    }

        break;
    }

    return std::complex<double>(0., 0.);
}

std::complex<double> DVMPProcessGK06::Amplitude0ppp() {

//the handbag amplitude
    std::complex<double> amplitude0ppp = -1.0 * Constant::POSITRON_CHARGE
            * sqrt(-(m_t - m_tmin)) / (4. * Constant::PROTON_MASS)
            * getConvolCoeffFunctionValue(GPDType::EbarTrans);

//switch over mesons
    switch (m_mesonType) {

//pi0
    case MesonType::PI0: {

        return amplitude0ppp;

    }

        break;

        //pi+
    case MesonType::PIPLUS: {

        return amplitude0ppp + poleAmplitude0ppp();

    }

        break;

        //???
    default: {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No implementation for meson: "
                        << MesonType(m_mesonType).toString());
    }

        break;
    }

    return std::complex<double>(0., 0.);
}

std::complex<double> DVMPProcessGK06::Amplitude0pmp() {

//the handbag amplitude
    std::complex<double> amplitude0pmp = -1.0 * Constant::POSITRON_CHARGE
            * sqrt(-(m_t - m_tmin)) / (4. * Constant::PROTON_MASS)
            * getConvolCoeffFunctionValue(GPDType::EbarTrans);

//switch over mesons
    switch (m_mesonType) {

//pi0
    case MesonType::PI0: {

        return amplitude0pmp;

    }

        break;

        //pi+
    case MesonType::PIPLUS: {

        return amplitude0pmp + poleAmplitude0pmp();

    }

        break;

        //???
    default: {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No implementation for meson: "
                        << MesonType(m_mesonType).toString());
    }

        break;
    }

    return std::complex<double>(0., 0.);
}

std::complex<double> DVMPProcessGK06::Amplitude0mpp() {

//the handbag amplitude
    std::complex<double> amplitude0mpp = Constant::POSITRON_CHARGE
            * sqrt(1. - pow(m_xi, 2.))
            * getConvolCoeffFunctionValue(GPDType::HTrans);

//switch over mesons
    switch (m_mesonType) {

//pi0
    case MesonType::PI0: {

        return amplitude0mpp;

    }

        break;

        //pi+
    case MesonType::PIPLUS: {

        return amplitude0mpp + poleAmplitude0mpp();

    }

        break;

        //???
    default: {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No implementation for meson: "
                        << MesonType(m_mesonType).toString());
    }

        break;
    }

    return std::complex<double>(0., 0.);
}

std::complex<double> DVMPProcessGK06::Amplitude0mmp() {

//the handbag amplitude
    std::complex<double> amplitude0mmp = 0.0;

//switch over mesons
    switch (m_mesonType) {

//pi0
    case MesonType::PI0: {

        return amplitude0mmp;

    }

        break;

        //pi+
    case MesonType::PIPLUS: {

        return amplitude0mmp + poleAmplitude0mmp();

    }

        break;

        //???
    default: {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No implementation for meson: "
                        << MesonType(m_mesonType).toString());
    }

        break;
    }

    return std::complex<double>(0., 0.);
}

} /* namespace PARTONS */
