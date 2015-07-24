/*
 * DVCSCFFHeavyQuarkModel.cpp
 *
 *  Created on: Feb 11, 2015
 *      Author: kuba
 */

#include "DVCSCFFHeavyQuarkModel.h"

#include <TMath.h>
#include <cmath>
#include <stdexcept>

#include "../../../beans/PerturbativeQCDOrderType.h"
#include "../../../BaseObjectRegistry.h"
#include "../../../FundamentalPhysicalConstants.h"
#include "../../../utils/logger/LoggerManager.h"
#include "../../../utils/stringUtils/Formatter.h"

// Initialise [class]::classId with a unique name.
const unsigned int DVCSCFFHeavyQuarkModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFHeavyQuarkModel("DVCSCFFHeavyQuarkModel"));

DVCSCFFHeavyQuarkModel::DVCSCFFHeavyQuarkModel(const std::string& className) :
        DVCSCFFModel(className), m_betas(std::complex<double>(0.0, 0.0)), m_rs(
                std::complex<double>(0.0, 0.0)), m_betaq(
                std::complex<double>(0.0, 0.0)), m_rq(
                std::complex<double>(0.0, 0.0)), m_TF(
                std::complex<double>(0.5, 0.0)) {

}

DVCSCFFHeavyQuarkModel* DVCSCFFHeavyQuarkModel::clone() const {
    return new DVCSCFFHeavyQuarkModel(*this);
}

DVCSCFFHeavyQuarkModel::~DVCSCFFHeavyQuarkModel() {
    // TODO Auto-generated destructor stub
}

DVCSCFFHeavyQuarkModel::DVCSCFFHeavyQuarkModel(
        const DVCSCFFHeavyQuarkModel& other) :
        DVCSCFFModel(other) {
    m_betas = other.m_betas;
    m_betaq = other.m_betaq;
    m_rs = other.m_rs;
    m_rq = other.m_rq;
    m_TF = other.m_TF;
}

/*void DVCSCFFHeavyQuarkModel::initModule() {
 DVCSCFFModel::initModule();

 m_Q = sqrt(m_Q2);
 m_Zeta = 2. * m_xi / (1 + m_xi);
 m_logQ2OverMu2 = 2. * log(m_Q / m_MuF);
 m_nbOfActiveFlavour = 1;

 RunningAlphaStrong* Alpha = new RunningAlphaStrong();
 Alpha->SetRunningScale(m_MuR);
 m_alphaSOver2Pi = Alpha->GetAlphaS() / (2. * PI);
 delete Alpha;
 Alpha = 0;
 }*/

//void DVCSCFFHeavyQuarkModel::isModuleWellConfigured() {
//}
std::complex<double> DVCSCFFHeavyQuarkModel::KernelGluonNLOA(double x) {
//    m_pLoggerManager->debug(getClassName(), __func__, "entered");
// TODO Add a massless part
    double z = m_xi / x;
    double mq = QUARK_CHARM_MASS;
    double s = -m_Q2 * (z - 1.) / 2. / z;
    double SumSqrCharges; // Sum of square of electric charges of active quark flavours

    m_betas = beta(s, mq);
    m_rs = r(s, mq);
    m_betaq = beta(-m_Q2, mq);
    m_rq = r(-m_Q2, mq);
    std::complex<double> GluonNLOA;

    if (z == 1.) {
        GluonNLOA.real() = 1.0;
        GluonNLOA.imag() = 0.;
    }
    if (z == -1.) {
        GluonNLOA.real() = 1.0;
        GluonNLOA.imag() = 0.;
    }
    if ((z != 1) && (z != -1)) {
        GluonNLOA = m_betas * std::log(m_rs) - m_betaq * std::log(m_rq);
        GluonNLOA *= (2. * z - 6.) / (z - 1.);
        GluonNLOA += std::log(m_rs) * std::log(m_rs)
                - std::log(m_rq) * std::log(m_rq);
        GluonNLOA *= (m_TF / 2. / (z + 1.) / (z + 1.));
    }

    switch (m_nbOfActiveFlavour) {

    case 3:
        SumSqrCharges = 2. / 3.;
        break;

    case 4:
        SumSqrCharges = 10. / 9.;
        break;

    case 5:
        SumSqrCharges = 11. / 9.;
        break;

    case 6:
        SumSqrCharges = 15. / 9.;
        break;

    default:
        throw std::runtime_error(
                Formatter()
                        << "[DVCSCFFModule::computeIntegrals] Erroneous input number of active quark flavours should be an integer between 3 and 6. Number of active quark flavours = "
                        << m_nbOfActiveFlavour);
    }
    //	RunningAlphaStrong* Alpha = new RunningAlphaStrong();
    //	Alpha->SetRunningScale(m_MuR);
    //	GluonNLOV  *= (Alpha->GetAlphaS() / (2. * PI))*C_ELEC_CHARGE*C_ELEC_CHARGE/SumSqrCharges;
    GluonNLOA *= m_alphaSOver2Pi * C_ELEC_CHARGE * C_ELEC_CHARGE
            / SumSqrCharges;
    GluonNLOA /= x * x;

//	GluonNLOA = 0.; /////TESTING

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "x= " << x << "    GluonNLOA RE = "
                    << GluonNLOA.real() << "   GluonNLOA IM = "
                    << GluonNLOA.imag());

    return GluonNLOA;
}

std::complex<double> DVCSCFFHeavyQuarkModel::KernelGluonNLOV(double x) {
    double z = -m_xi / x;
    double mq = QUARK_CHARM_MASS;
    double s = -m_Q2 * (z - 1.) / 2. / z;
    double eta = mq * mq / m_Q2;
    double SumSqrCharges; // Sum of square of electric charges of active quark flavours
    std::complex<double> GluonNLOV;

    m_betas = beta(s, mq);
    m_rs = r(s, mq);
    m_betaq = beta(-m_Q2, mq);
    m_rq = r(-m_Q2, mq);

    if (z == 1.) {
        GluonNLOV.real() = 0.0;
        GluonNLOV.imag() = 0.;
    }
    if (z == -1.) {
        GluonNLOV.real() = 0.0;
        GluonNLOV.imag() = 0.;
    }
    if ((z != 1.) && (z != -1.)) {
        GluonNLOV = m_betas * std::log(m_rs) - m_betaq * std::log(m_rq);
        GluonNLOV *= (2. - z * (6. - 16. * eta)) / (z - 1.);
        GluonNLOV -= (1.
                + 8. * eta * z * (1. - (1. + 4. * eta) * z) / (z - 1.)
                        / (z - 1.)) * std::log(m_rs) * std::log(m_rs);
        GluonNLOV += (1.
                - 2. * eta * (1. + (3. + 8. * eta) * z * z) / (z - 1.)
                        / (z - 1.)) * std::log(m_rq) * std::log(m_rq);
        GluonNLOV *= (m_TF / 2. / (z + 1.) / (z + 1.));
    }

    switch (m_nbOfActiveFlavour) {

    case 3:
        SumSqrCharges = 2. / 3.;
        break;

    case 4:
        SumSqrCharges = 10. / 9.;
        break;

    case 5:
        SumSqrCharges = 11. / 9.;
        break;

    case 6:
        SumSqrCharges = 15. / 9.;
        break;

    default:
        throw std::runtime_error(
                Formatter()
                        << "[DVCSCFFModule::computeIntegrals] Erroneous input number of active quark flavours should be an integer between 3 and 6. Number of active quark flavours = "
                        << m_nbOfActiveFlavour);
    }
//	RunningAlphaStrong* Alpha = new RunningAlphaStrong();
//	Alpha->SetRunningScale(m_MuR);
//	GluonNLOV  *= (Alpha->GetAlphaS() / (2. * PI))*C_ELEC_CHARGE*C_ELEC_CHARGE/SumSqrCharges;
    GluonNLOV *= m_alphaSOver2Pi * C_ELEC_CHARGE * C_ELEC_CHARGE
            / SumSqrCharges;

    GluonNLOV /= x * x;

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "x= " << x << "    GluonNLOV RE = "
                    << GluonNLOV.real() << "   GluonNLOV IM = "
                    << GluonNLOV.imag() << " malpha=" << m_alphaSOver2Pi);

    return GluonNLOV;
}

std::complex<double> DVCSCFFHeavyQuarkModel::beta(double s, double mq) {
    std::complex<double> tmp_beta;
    if (s < 0) {
        tmp_beta.real() = std::sqrt(1. - 4. * mq * mq / s);
        tmp_beta.imag() = 0.;
    }
    if ((s > 0) && (s < 4. * mq * mq)) {
        tmp_beta.real() = 0.;
        tmp_beta.imag() = std::sqrt(-1. + 4. * mq * mq / s);
    }
    if (s >= 4 * mq * mq) {
        tmp_beta.real() = std::sqrt(1. - 4. * mq * mq / s);
        tmp_beta.imag() = 0.;
    }
    return tmp_beta;
}

std::complex<double> DVCSCFFHeavyQuarkModel::r(double s, double mq) {
    std::complex<double> tmp_beta = beta(s, mq);
    return (tmp_beta - 1.) / (tmp_beta + 1.);
}

std::complex<double> DVCSCFFHeavyQuarkModel::KernelQuarkNLOV(double x) {
    return 0.;

}

std::complex<double> DVCSCFFHeavyQuarkModel::KernelQuarkNLOA(double x) {
    return 0.;

}

void DVCSCFFHeavyQuarkModel::computeSubtractionFunctionsV() {
    double LogZeta = log(m_Zeta);
    double LogInvZeta = log((1. - m_Zeta) / m_Zeta);
    double LogInvZeta2 = LogInvZeta * LogInvZeta;
    double DiLogInvZeta = TMath::DiLog(1. - 1. / m_Zeta);
    double Pi2 = PI * PI;

    double RealPartSubtractQuarkLO; // Real part of eq. (B2)
    double ImaginaryPartSubtractQuarkLO; // Imaginary part of eq. (B2)
    double RealPartSubtractQuarkNLOV; // Real part of eq. (B4)
    double ImaginaryPartSubtractQuarkNLOV; // Imaginary part of eq. (B4)
    double RealPartSubtractGluonNLOV; // Real part of eq. (B6)
    double ImaginaryPartSubtractGluonNLOV; // Imaginary part of eq. (B6)

    // LO, 1 / 1 - z

    RealPartSubtractQuarkLO = -LogInvZeta;

    ImaginaryPartSubtractQuarkLO = PI;

    // NLO, quark, vector, eq. (B4)

    RealPartSubtractQuarkNLOV = Pi2 / 2. - 3. * DiLogInvZeta
            + LogInvZeta * (Pi2 + 9. + 3. * LogZeta - LogInvZeta2 / 3.);
    RealPartSubtractQuarkNLOV += m_logQ2OverMu2
            * (Pi2 - 3. * LogInvZeta - LogInvZeta2);
    RealPartSubtractQuarkNLOV *= m_CF / 2.;

    ImaginaryPartSubtractQuarkNLOV = Pi2 / 3. + 9. + 3. * LogZeta - LogInvZeta2
            - m_logQ2OverMu2 * (2. * LogInvZeta + 3);
    ImaginaryPartSubtractQuarkNLOV *= -PI * m_CF / 2.;

    // NLO, gluon, vector, eq. (B6)

    RealPartSubtractGluonNLOV = -1. + Pi2 / 3. * (1. - 3. / 4. * m_Zeta)
            + DiLogInvZeta - LogZeta * LogInvZeta;
    RealPartSubtractGluonNLOV += (2. - m_Zeta) * LogInvZeta
            * (1. - LogInvZeta / 4.);
    RealPartSubtractGluonNLOV += m_logQ2OverMu2 / 2.
            * (1. - (2. - m_Zeta) * LogInvZeta);
    RealPartSubtractGluonNLOV *= 1 / (2. * m_xi);

    ImaginaryPartSubtractGluonNLOV = (2. - m_Zeta)
            * (2. - m_logQ2OverMu2 - LogInvZeta) - 2. * LogZeta;
    ImaginaryPartSubtractGluonNLOV *= -PI / (4. * m_xi);

    // Computation of subtraction terms in eq. (8) and (9)
    // Sums up previous contributions and takes care of LO / NLO and P(olarized) / U(npolarized) subtleties

    // LO, real and imaginary parts

    m_realPartSubtractQuark = RealPartSubtractQuarkLO;
    m_imaginaryPartSubtractQuark = ImaginaryPartSubtractQuarkLO;

    m_realPartSubtractGluon = 0.;
    m_imaginaryPartSubtractGluon = 0.;

    // NLO, real and imaginary parts

    if (m_qcdOrderType == PerturbativeQCDOrderType::NLO) {

        // Real part, quark contribution

//        m_realPartSubtractQuark += m_alphaSOver2Pi * RealPartSubtractQuarkNLOV;
        //  Uncomment above after testing of heavy quark

        // Real part, gluon contribution

//        m_realPartSubtractGluon += m_alphaSOver2Pi * RealPartSubtractGluonNLOV;
        //  Uncomment above after testing of heavy quark

        // Imaginary part, quark contribution

//        m_imaginaryPartSubtractQuark += m_alphaSOver2Pi
//              * ImaginaryPartSubtractQuarkNLOV;
        //  Uncomment above after testing of heavy quark

        // Imaginary part, gluon contribution

//        m_imaginaryPartSubtractGluon += m_alphaSOver2Pi
//                * ImaginaryPartSubtractGluonNLOV;
        //  Uncomment above after testing of heavy quark

    }
}

void DVCSCFFHeavyQuarkModel::computeSubtractionFunctionsA() {
    double LogZeta = log(m_Zeta);
    double LogInvZeta = log((1. - m_Zeta) / m_Zeta);
    double LogInvZeta2 = LogInvZeta * LogInvZeta;
    double DiLogInvZeta = TMath::DiLog(1. - 1. / m_Zeta);
    double Pi2 = PI * PI;

    double RealPartSubtractQuarkLO; // Real part of eq. (B2)
    double ImaginaryPartSubtractQuarkLO; // Imaginary part of eq. (B2)
    double RealPartSubtractQuarkNLOA; // Real part of eq. (B4)
    double ImaginaryPartSubtractQuarkNLOA; // Imaginary part of eq. (B4)
    double RealPartSubtractGluonNLOA; // Real part of eq. (B6)
    double ImaginaryPartSubtractGluonNLOA; // Imaginary part of eq. (B6)

    // LO, 1 / 1 - z

    RealPartSubtractQuarkLO = -LogInvZeta;

    ImaginaryPartSubtractQuarkLO = PI;

    // NLO, quark, axial, eq. (B3)

    RealPartSubtractQuarkNLOA = Pi2 / 6. - DiLogInvZeta
            + LogInvZeta * (Pi2 + 9. + LogZeta - LogInvZeta2 / 3.);
    RealPartSubtractQuarkNLOA += m_logQ2OverMu2
            * (Pi2 - 3. * LogInvZeta - LogInvZeta2);
    RealPartSubtractQuarkNLOA *= m_CF / 2.;

    ImaginaryPartSubtractQuarkNLOA = Pi2 / 3. + 9. + LogZeta - LogInvZeta2
            - m_logQ2OverMu2 * (2. * LogInvZeta + 3);
    ImaginaryPartSubtractQuarkNLOA *= -PI * m_CF / 2.;

    // NLO, gluon, axial, eq. (B5)

    RealPartSubtractGluonNLOA = 1. + Pi2 / 4. * m_Zeta
            + m_Zeta * LogInvZeta * (1. - LogInvZeta / 4.);
    RealPartSubtractGluonNLOA += -m_logQ2OverMu2 / 2.
            * (1. + m_Zeta * LogInvZeta);
    RealPartSubtractGluonNLOA *= 1 / (2. * m_xi);

    ImaginaryPartSubtractGluonNLOA = 2. - LogInvZeta - m_logQ2OverMu2;
    ImaginaryPartSubtractGluonNLOA *= -PI * m_Zeta / (4. * m_xi);

    // Computation of subtraction terms in eq. (8) and (9)
    // Sums up previous contributions and takes care of LO / NLO and P(olarized) / U(npolarized) subtleties

    // LO, real and imaginary parts

    m_realPartSubtractQuark = RealPartSubtractQuarkLO;
    m_imaginaryPartSubtractQuark = ImaginaryPartSubtractQuarkLO;

    m_realPartSubtractGluon = 0.;
    m_imaginaryPartSubtractGluon = 0.;

    // NLO, real and imaginary parts

    if (m_qcdOrderType == PerturbativeQCDOrderType::NLO) {

        // Real part, quark contribution

        //      m_realPartSubtractQuark += m_alphaSOver2Pi * RealPartSubtractQuarkNLOA;
        //  Uncomment above after testing of heavy quark

        // Real part, gluon contribution

//        m_realPartSubtractGluon += m_alphaSOver2Pi * RealPartSubtractGluonNLOA;
//  Uncomment above after testing of heavy quark

        // Imaginary part, quark contribution

//        m_imaginaryPartSubtractQuark += m_alphaSOver2Pi
//                * ImaginaryPartSubtractQuarkNLOA;
//  Uncomment above after testing of heavy quark

        // Imaginary part, gluon contribution

//        m_imaginaryPartSubtractGluon += m_alphaSOver2Pi
//                * ImaginaryPartSubtractGluonNLOA;

//  Uncomment above after testing of heavy quark

    }
}
