#include "../../../../../include/partons/modules/process/JET/JETProcessPSSW26.h"

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"
#include "../../../../../include/partons/modules/scales/JET/JETScalesModule.h"
#include "../../../../../include/partons/modules/xi_converter/JET/JETXiConverterModule.h"
#include "../../../../../include/partons/beans/Scales.h"

namespace PARTONS {

const unsigned int JETProcessPSSW26::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new JETProcessPSSW26("JETProcessPSSW26"));

JETProcessPSSW26::JETProcessPSSW26(const std::string &className) :
        JETProcessModule(className) {
}

JETProcessPSSW26::~JETProcessPSSW26() {
}

JETProcessPSSW26::JETProcessPSSW26(const JETProcessPSSW26& other) :
        JETProcessModule(other) {
}

JETProcessPSSW26* JETProcessPSSW26::clone() const {
    return new JETProcessPSSW26(*this);
}

double JETProcessPSSW26::alphaS_2loop(double mu2) const{

    //chosen such as alphaS(MZ^2) = 0.1165 as in https://arxiv.org/pdf/hep-ph/0505263
    constexpr int Nf = 5;
    constexpr double Lambda = 0.19;

    const double L = std::log(mu2 / (Lambda * Lambda));

    constexpr double beta0 = 11.0 - 2.0 / 3.0 * Nf;
    constexpr double beta1 = 102.0 - 38.0 / 3.0 * Nf;

    const double logL = std::log(L);

    return (4.0 * M_PI / (beta0 * L)) *
           (1.0 - (beta1 / (beta0 * beta0)) * (logL / L));
}

PhysicalType<double> JETProcessPSSW26::CrossSection() {


    //number of colors
    constexpr double Nc = 3.;

    //quark mass
    double m;

    switch(m_jetType){

        case JetType::UP:
        case JetType::DOWN:
        case JetType::STRANGE:{
    	    m = 0.; break;
        }

        case JetType::CHARM:{
    	    m = 1.25; break;
        }

        default:{
            throw ElemUtils::CustomException(getClassName(), __func__, "Unknown jet type for conversion");
        }
    }

    double m2 = m * m;

    //quark charge
    double eq;

    switch(m_jetType){

        case JetType::UP:{
            eq = 2/3.; break;
        }

        case JetType::DOWN:{
            eq = -1/3.; break;
        }

        case JetType::STRANGE:{
            eq = -2/3.; break;
        }

        case JetType::CHARM:{
            eq = 2/3.; break;
        }

        default:{
    	    throw ElemUtils::CustomException(getClassName(), __func__, "Unknown jet type for conversion");
        }
    }

    //mu2 (Eq. 15 from https://arxiv.org/pdf/hep-ph/0505263)
    double mu2 = pow(m, 2) + m_z * (1. - m_z) * m_Q2;

    //kinematics
    JETObservableKinematic kin(m_xB, m_t, m_z, m_qPerp2, m_Q2, m_E, m_phi, m_jetType);

    //compute scales
    double muR2 = m_pScaleModule->compute(kin).getMuR2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();

    //compute xi
    double xi = m_pXiConverterModule->compute(kin).getValue();

    // ==============

    //CFFs
    std::complex<double> CFF_L = m_jetConvolCoeffFunctionResult.find(JetCFFType::LL)->second.getResult(GPDType::H);
    std::complex<double> CFF_TL = m_jetConvolCoeffFunctionResult.find(JetCFFType::TL)->second.getResult(GPDType::H);
    std::complex<double> CFF_TT1 = m_jetConvolCoeffFunctionResult.find(JetCFFType::TT1)->second.getResult(GPDType::H);
    std::complex<double> CFF_TT2 = m_jetConvolCoeffFunctionResult.find(JetCFFType::TT2)->second.getResult(GPDType::H);

    //sigma0 (Eq. 35 from https://arxiv.org/pdf/hep-ph/0505263)
    double sigma0 = Constant::FINE_STRUCTURE_CONSTANT * pow(alphaS_2loop(muR2) * eq, 2) / (16 * M_PI * Nc * (1. - xi * xi)) *  m_qPerp2 / pow(m_qPerp2 + mu2, 4);

    //sigmaT (Eq. 34 from https://arxiv.org/pdf/hep-ph/0505263)
    double sigmaT = m2 / m_qPerp2 * std::norm(CFF_TL) + std::norm(CFF_TT1) + std::norm(CFF_TT2);

    //sigmaT (Eq. 34 from https://arxiv.org/pdf/hep-ph/0505263)
    double sigmaTT = (CFF_TT1 * std::conj(CFF_TT2)).real();

    //sigmaT (Eq. 34 from https://arxiv.org/pdf/hep-ph/0505263)
    double sigmaL = 4 * pow(m_z * (1. - m_z), 2) * (m_Q2 / m_qPerp2) * std::norm(CFF_L);

    //sigmaT (Eq. 34 from https://arxiv.org/pdf/hep-ph/0505263)
    double sigmaLT = 2 * (m_z * (1. - m_z)) * sqrt(m_Q2 / m_qPerp2) * (CFF_L * std::conj(CFF_TT1 - CFF_TT2)).real();

    //dsigma/dQ2dy (Eq. 33 from https://arxiv.org/pdf/hep-ph/0505263)
    double sigma = sigma0 * Constant::FINE_STRUCTURE_CONSTANT / (M_PI * m_Q2 * m_y) * (
        (1. + pow(1. - m_y, 2)) / 2. * sigmaT -
        2 * (1. - m_y) * cos(2 * m_phi) * sigmaTT +
        (1. - m_y) * sigmaL -
        (2. - m_y) * sqrt(1. - m_y) * cos(m_phi) * sigmaLT
    );

    //dsigma/dQ2dy -> dsigma/dQ2dxB
    sigma *= m_y / m_xB;

    //return
    return PhysicalType<double>(sigma, PhysicalUnit::GEVm2);
}

} /* namespace PARTONS */
