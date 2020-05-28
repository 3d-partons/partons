#include "../../../../../include/partons/modules/process/DVMP/DVMPProcessGK06.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <cmath>
#include <complex>

#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/MesonType.h"
#include "../../../../../include/partons/beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/xi_converter/DVMP/DVMPXiConverterModule.h"
#include "../../../../../include/partons/modules/xi_converter/XiConverterModule.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVMPProcessGK06::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPProcessGK06("DVMPProcessGK06"));

DVMPProcessGK06::DVMPProcessGK06(const std::string &className) :
        DVMPProcessModule(className), m_W2(0.), m_xi(0.) {
}

DVMPProcessGK06::DVMPProcessGK06(const DVMPProcessGK06& other) :
        DVMPProcessModule(other), m_W2(other.m_W2), m_xi(other.m_xi) {
}

DVMPProcessGK06* DVMPProcessGK06::clone() const {
    return new DVMPProcessGK06(*this);
}

DVMPProcessGK06::~DVMPProcessGK06() {
}

void DVMPProcessGK06::initModule() {

    //run for mother
    DVMPProcessModule::initModule();

    //evaluate W
    m_W2 = m_Q2 / m_xB + pow(Constant::PROTON_MASS, 2.0) - m_Q2;

    //evaluate xi
    m_xi =
            getXiConverterModule()->compute(
                    DVMPObservableKinematic(m_xB, m_t, m_Q2, m_E, m_phi,
                            m_mesonType)).getValue();
}

void DVMPProcessGK06::isModuleWellConfigured() {
    DVMPProcessModule::isModuleWellConfigured();
}

PhysicalType<double> DVMPProcessGK06::CrossSection() {

    //check meson type
    if (m_mesonType != MesonType::PI0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No implementation for meson "
                        << MesonType(m_mesonType).toString());
    }

    //variables //TODO TBC
    double s = pow(Constant::PROTON_MASS, 2) + 2. * Constant::PROTON_MASS * m_E;
    double gamma = 2 * m_xB * Constant::PROTON_MASS / sqrt(m_Q2);
    double eps = (1. - m_y - pow(m_y * gamma / 2., 2))
            / (1. - m_y + pow(m_y, 2) / 2. + pow(m_y * gamma / 2., 2));

    //result //TODO TBC
    double result = Constant::FINE_STRUCTURE_CONSTANT
            * (s - pow(Constant::PROTON_MASS, 2))
            / (16. * pow(M_PI, 2) * pow(m_E, 2) * pow(Constant::PROTON_MASS, 2)
                    * m_Q2 * (1. - eps));

    //TODO TBC
    result *= CrossSectionT() + eps * CrossSectionL()
            + eps * cos(2 * m_phi) * CrossSectionTT()
            + sqrt(2 * eps * (1. + eps)) * cos(m_phi) * CrossSectionLT();

    //divide by 2pi to have dsigma/...dphiS
    result /= 2. * Constant::PI;

    return PhysicalType<double>(result, PhysicalUnit::GEVm2);
}

double DVMPProcessGK06::CrossSectionL() {

    // Longitudinal partial cross section. See Eq. (43) in arxiv:0906.0460

    std::complex<double> amplitude0m0p = getConvolCoeffFunctionValue(
            GPDType::Et);
    //TODO TBC
    std::complex<double> amplitude0p0p = getConvolCoeffFunctionValue(
            GPDType::Ht)
            - 2 * Constant::PROTON_MASS * m_xi / sqrt(1. - pow(m_xi, 2))
                    / sqrt(-(m_t - m_tmin)) * amplitude0m0p;

    double result = 1.
            / (32.0 * M_PI * (m_W2 - pow(Constant::PROTON_MASS, 2.0))
                    * sqrt(
                            lambdaFunction(m_W2, -m_Q2,
                                    pow(Constant::PROTON_MASS, 2))));

    result *= 2.;

    result *= pow(std::abs(amplitude0p0p), 2.)
            + pow(std::abs(amplitude0m0p), 2.);

    return result;
}

double DVMPProcessGK06::CrossSectionT() {

    // Transverse partial cross section. See Eq. (43) in arxiv:0906.0460

    std::complex<double> amplitude0mpp = getConvolCoeffFunctionValue(
            GPDType::HTrans);
    std::complex<double> amplitude0ppp = getConvolCoeffFunctionValue(
            GPDType::ETrans);
    std::complex<double> amplitude0pmp = getConvolCoeffFunctionValue(
            GPDType::ETrans);
    std::complex<double> amplitude0mmp(0., 0.);

    double result = 1.
            / (32.0 * M_PI * (m_W2 - pow(Constant::PROTON_MASS, 2.0))
                    * sqrt(
                            lambdaFunction(m_W2, -m_Q2,
                                    pow(Constant::PROTON_MASS, 2))));

    result *= pow(std::abs(amplitude0mpp), 2.)
            + pow(std::abs(amplitude0mmp), 2.)
            + pow(std::abs(amplitude0ppp), 2.)
            + pow(std::abs(amplitude0pmp), 2.);

    return result;
}

double DVMPProcessGK06::CrossSectionLT() {

    std::complex<double> amplitude0m0p = getConvolCoeffFunctionValue(
            GPDType::Et);
    //TODO TBC
    std::complex<double> amplitude0p0p = getConvolCoeffFunctionValue(
            GPDType::Ht)
            - 2 * Constant::PROTON_MASS * m_xi / sqrt(1. - pow(m_xi, 2))
                    / sqrt(-(m_t - m_tmin)) * amplitude0m0p;
    std::complex<double> amplitude0mpp = getConvolCoeffFunctionValue(
            GPDType::HTrans);
    std::complex<double> amplitude0ppp = getConvolCoeffFunctionValue(
            GPDType::ETrans);
    std::complex<double> amplitude0pmp = getConvolCoeffFunctionValue(
            GPDType::ETrans);
    std::complex<double> amplitude0mmp(0., 0.);

    // Partial cross section of the interference part LT. See Eq. (43) in arxiv:0906.0460

    double result = 1.
            / (32.0 * M_PI * (m_W2 - pow(Constant::PROTON_MASS, 2.0))
                    * sqrt(
                            lambdaFunction(m_W2, -m_Q2,
                                    pow(Constant::PROTON_MASS, 2))));

    result *= -1 * sqrt(2.);

    result *= std::real(
            std::conj(amplitude0m0p) * (amplitude0mpp - amplitude0mmp)
                    + std::conj(amplitude0p0p)
                            * (amplitude0ppp - amplitude0pmp));

    return result;
}

double DVMPProcessGK06::CrossSectionTT() {

    std::complex<double> amplitude0mpp = getConvolCoeffFunctionValue(
            GPDType::HTrans);
    std::complex<double> amplitude0ppp = getConvolCoeffFunctionValue(
            GPDType::ETrans);
    std::complex<double> amplitude0pmp = getConvolCoeffFunctionValue(
            GPDType::ETrans);
    std::complex<double> amplitude0mmp(0., 0.);

    // Partial cross section of the interference part TT. See Eq. (43) in arxiv:0906.0460

    double result = 1.
            / (32.0 * M_PI * (m_W2 - pow(Constant::PROTON_MASS, 2.0))
                    * sqrt(
                            lambdaFunction(m_W2, -m_Q2,
                                    pow(Constant::PROTON_MASS, 2))));

    result *= -1;

    result *= std::real(
            std::conj(amplitude0mpp) * amplitude0mmp
                    + std::conj(amplitude0ppp) * amplitude0pmp);

    return result;
}

double DVMPProcessGK06::lambdaFunction(double a, double b, double c) const {
    return pow(a, 2.0) + pow(b, 2.0) + pow(c, 2.0)
            - 2.0 * (a * b + a * c + b * c);
}

} /* namespace PARTONS */
